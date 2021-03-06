//Author(s): Tyler Henson

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <pthread.h>

#define THRESHOLD 160
#define MAX_CORNERS 20
#define QUALITY_LEVEL 0.2
#define MIN_DISTANCE 50
#define BLOCK_SIZE 3
#define USE_HARRIS false
#define TOLERANCE 20
#define LEFT 225
#define RIGHT 325
#define CROP_TOP 90
#define CROP_LEFT 0
#define CROP_HEIGHT 240
#define CROP_WIDTH 640
#define MOVE_RIGHT 1
#define MOVE_LEFT -1
#define STOP 0

struct camera_data
{
	int *direction_ptr;
	pthread_mutex_t *dir_mutex_ptr;
	int *end_thread_ptr;
	pthread_mutex_t *end_mutex_ptr;
	int *processed_frame_ptr;
};

void *locate(void *data);

#endif
