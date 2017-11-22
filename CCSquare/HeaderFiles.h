
/*
	HeaderFiles.h
	[Author]: Claudiu Furca
	[Copyright 2016]: (c) Amprent.	All rights reseved.
*/

#ifndef HeaderFiles_h
#define HeaderFiles_h

/* Include here all the necessary`s OpenCV  */
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <ctime>
#include <map>
#include <string>

#include <opencv2\opencv.hpp>
#include <opencv2\core.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\calib3d.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <opencv2\xfeatures2d.hpp>
#include <opencv2\xfeatures2d\cuda.hpp>
#include <opencv2\xfeatures2d\nonfree.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\imgcodecs\imgcodecs.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\ml.hpp>
#include <opencv2\objdetect.hpp>
#include <opencv2\shape.hpp>
#include <opencv2\photo.hpp>
#include <opencv2\flann.hpp>
#include <opencv2\flann\flann.hpp>
#include <opencv2\flann\matrix.h>
#include <opencv2\flann\general.h>
#include <opencv2\flann\miniflann.hpp>
#include <opencv2\flann\allocator.h>

/* Include here all the necessary headers */
#include "CCSquare.h"
#include "Filter.h"
#include "ImageManager.h"
#include "FLANN.h"
#include "MSURF.h"

/* Include Here All the Necessary Namespaces */
using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;
using namespace cv::cuda;

#endif //End HeaderFiles_h