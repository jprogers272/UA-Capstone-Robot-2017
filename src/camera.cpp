//Author(s): Tyler Henson

#include "camera.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <pthread.h>
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;

void *locate(void *data)
{
	cout << "In the thread" << endl;
	//initialize local variables for shared data
	camera_data *my_data;
	my_data = (struct camera_data *)data;
	int *direction = my_data->direction_ptr;
	pthread_mutex_t *dir_mutex = my_data->dir_mutex_ptr;
	int *end_thread = my_data->end_thread_ptr;
	pthread_mutex_t *end_mutex = my_data->end_mutex_ptr;
	int *processed_frame = my_data->processed_frame_ptr;

	//initialize Mats for processing each frame
	Mat src, cropped, src_gray, canny_out, contour_out, contour_out_8U;

	//initialize video input stream
	VideoCapture stream(-1);

	//set cropping region of interest
	Rect myROI(0, 90, 640, 240);

	//initialize arrays needed for contour detection
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	//initialize array for corners returned from corner detector
	vector<Point2f> corners;

	while(true)
	{
		
		//initialize four corner points
		Point2f top_l(1000.f, -1000.f), top_r(-1000.f, -1000.f),
				bot_l(1000.f, 1000.f), bot_r(-1000.f, 1000.f);

		//load frame from camera
		stream.read(src);
		//crop image
		cropped = src(myROI);
		//convert to grayscale and blur
		cvtColor( cropped, src_gray, CV_BGR2GRAY );
		blur( src_gray, src_gray, Size(3,3) );
		 
		//Perform edge detection
		Canny(src_gray, canny_out, THRESHOLD, THRESHOLD*2, 3 );

		

		//Perform contour detection, don't include children
		findContours( canny_out, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		
		//draw contours
		contour_out = Mat::zeros( canny_out.size(), CV_8UC3 );
		for( unsigned int i = 0; i< contours.size(); i++ )
		{
			drawContours( contour_out, contours, i, Scalar(255.), -2, 8, hierarchy, 0, Point() );
		}

		//convert to 8U for corner detector
		cvtColor(contour_out, contour_out_8U, CV_BGR2GRAY);
		contour_out_8U.convertTo(contour_out_8U, CV_8U);
		
		//detect corners
		goodFeaturesToTrack(contour_out_8U,
				corners,
				MAX_CORNERS,
				QUALITY_LEVEL,
				MIN_DISTANCE,
				Mat(),
				BLOCK_SIZE,
				USE_HARRIS);

		//find outer corners
		//determine outer corners
		for (unsigned int i = 0; i < corners.size(); i++) {
			if ((corners[i].x <= top_l.x + TOLERANCE && corners[i].y >= top_l.y) ||
					(corners[i].x <= top_l.x && corners[i].y >= top_l.y - TOLERANCE))
				top_l = corners[i];
			if ((corners[i].x >= top_r.x - TOLERANCE && corners[i].y >= top_r.y) ||
					(corners[i].x >= top_r.x && corners[i].y >= top_r.y - TOLERANCE))
				top_r = corners[i];
			if ((corners[i].x <= bot_l.x + TOLERANCE && corners[i].y <= bot_l.y) ||
					(corners[i].x <= bot_l.x && corners[i].y <= bot_l.y + TOLERANCE))
				bot_l = corners[i];
			if ((corners[i].x >= bot_r.x - TOLERANCE && corners[i].y <= bot_r.y) ||
					(corners[i].x >= bot_r.x && corners[i].y <= bot_r.y + TOLERANCE))
				bot_r = corners[i];
		}

		pthread_mutex_lock(dir_mutex);
		cout << top_l.x << endl;
		//determine if left, right, or centered
		if (top_l.x < LEFT && bot_l.x < LEFT)
			*direction = MOVE_LEFT;
		else if (top_l.x > RIGHT && bot_l.x > RIGHT)
			*direction = MOVE_RIGHT;
		else if (top_l.x < RIGHT && top_l.x > LEFT && bot_l.x < RIGHT && bot_l.x > LEFT)
			*direction = STOP;
		*processed_frame = 1;
		pthread_mutex_unlock(dir_mutex);

		pthread_mutex_lock(end_mutex);
		if(*end_thread){
			pthread_mutex_unlock(end_mutex);
			cout << "ending thread" << endl;
			pthread_exit(0);
		}
		pthread_mutex_unlock(end_mutex);

	}
}

