#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <sstream>
#include <pthread.h>
#include <ctime>

using namespace cv;
using namespace std;

struct thread_data{
	int *direction_ptr;
	pthread_mutex_t *mute_ptr;
};

void *find(void *data)
{
	//cout << getBuildInformation() << endl;
	thread_data *my_data;
	my_data = (struct thread_data *)data;
	int *direction = my_data->direction_ptr;
	pthread_mutex_t *mute = my_data->mute_ptr;

	//initialize Mats for processing each frame
	Mat src, src_gray, canny_out, contour_out, contour_out_8U;
	//set edge detector threshold
	int threshold = 210;

	//initialize video input stream
	VideoCapture stream(-1);

	Size frameSize = Size((int) stream.get(CV_CAP_PROP_FRAME_WIDTH),
							(int) stream.get(CV_CAP_PROP_FRAME_HEIGHT));
	int fcc = static_cast<int>(stream.get(CV_CAP_PROP_FOURCC));
	int fps = static_cast<int>(stream.get(CV_CAP_PROP_FPS));

	if(!stream.isOpened())
	{
		cout << "CAMERA IS NOT WORKING" << endl;
		pthread_exit(0);
	}

	VideoWriter save("../../media/out.avi", fcc, fps, frameSize, true);
	VideoWriter save2("../../media/out2.avi", fcc, fps, frameSize, true);

	if(!save.isOpened())
	{
		cout << "OUTPUT VIDEO COULD NOT BE OPENED" << endl;
		pthread_exit(0);
	}

	//set corner detector parameters
	int maxCorners = 20;
	double qualityLevel = 0.2;
	double minDistance = 50;
	int blocksize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;
	double tolerance = 20.0;

	struct timespec start;
	struct timespec end;
	//initialize arrays needed for contour detection
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	//initialize array for corners returned from corner detector
	vector<Point2f> corners;

	while(true)
	{
		clock_gettime(CLOCK_MONOTONIC, &start);

		//initialize four corner points
		Point2f top_l(1000.f, -1000.f), top_r(-1000.f, -1000.f),
				bot_l(1000.f, 1000.f), bot_r(-1000.f, 1000.f);

		//load frame from camera
		stream.read(src);
		//convert to grayscale and blur
		cvtColor( src, src_gray, CV_BGR2GRAY );
		blur( src_gray, src_gray, Size(3,3) );

		//Perform edge detection
		Canny(src_gray, canny_out, threshold, threshold*2, 3 );

		//Perform contour detection, don't include children
		findContours( canny_out, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

		//draw contours
		contour_out = Mat::zeros( canny_out.size(), CV_8UC3 );
		for( int i = 0; i< contours.size(); i++ )
		{
			drawContours( contour_out, contours, i, Scalar(255.), -2, 8, hierarchy, 0, Point() );
		}

		//convert to 8U for corner detector
		cvtColor(contour_out, contour_out_8U, CV_BGR2GRAY);
		contour_out_8U.convertTo(contour_out_8U, CV_8U);

		//detect corners
		goodFeaturesToTrack(contour_out_8U,
				corners,
				maxCorners,
				qualityLevel,
				minDistance,
				Mat(),
				blocksize,
				useHarrisDetector);

		//find outer corners
		//determine outer corners
		for (int i = 0; i < corners.size(); i++) {
			if ((corners[i].x <= top_l.x + tolerance && corners[i].y >= top_l.y) ||
					(corners[i].x <= top_l.x && corners[i].y >= top_l.y - tolerance))
				top_l = corners[i];
			if ((corners[i].x >= top_r.x - tolerance && corners[i].y >= top_r.y) ||
					(corners[i].x >= top_r.x && corners[i].y >= top_r.y - tolerance))
				top_r = corners[i];
			if ((corners[i].x <= bot_l.x + tolerance && corners[i].y <= bot_l.y) ||
					(corners[i].x <= bot_l.x && corners[i].y <= bot_l.y + tolerance))
				bot_l = corners[i];
			if ((corners[i].x >= bot_r.x - tolerance && corners[i].y <= bot_r.y) ||
					(corners[i].x >= bot_r.x && corners[i].y <= bot_r.y + tolerance))
				bot_r = corners[i];
		}

		//draw outer corners
		circle(contour_out, top_l, 10, Scalar(255.), -1);
		circle(contour_out, top_r, 10, Scalar(255.), -1);
		circle(contour_out, bot_l, 10, Scalar(255.), -1);
		circle(contour_out, bot_r, 10, Scalar(255.), -1);

		//point for location of direction circle
		//Point2f dir = Point2f(320, 240);

		pthread_mutex_lock(mute);
		//determine if left, right, or centered
		if (top_l.x < 100 && bot_l.x < 100)
			*direction = -1;
			//dir = Point2f(10,10);
		else if (top_l.x > 115 && bot_l.x > 115)
			*direction = 1;
			//dir = Point2f(630, 10);
		else if (top_l.x < 116 && top_l.x > 99 && bot_l.x < 116 && bot_l.x > 99)
			*direction = 0;
			//dir = Point2f(320, 10);
		cout << "In the Thread" << endl;
		pthread_mutex_unlock(mute);

		//draw direction circle, middle of screen if bad data, otherwise shows direction
		//circle(contour_out, dir, 10, Scalar(255.), -1);

		clock_gettime(CLOCK_MONOTONIC, &end);

		save.write(contour_out);
		save2.write(src);

		long elapsed = (long)(end.tv_sec - start.tv_sec)*1000;
		elapsed += (end.tv_nsec - start.tv_nsec)/1000000;

		cout << "Took " << elapsed << "ms to process" << endl;


	}
	pthread_exit(0);
}

int main()
{
	pthread_t thread;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	int direction = 0;

	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);

	thread_data data;
	data.direction_ptr = &direction;
	data.mute_ptr = &mutex;

	pthread_create(&thread, &attr, find, (void *)&data);

	/*while(true)
	{
		pthread_mutex_lock(&mutex);
		cout << direction << endl;
		pthread_mutex_unlock(&mutex);
	}*/

	void *status;
	pthread_join(thread, &status);
}
