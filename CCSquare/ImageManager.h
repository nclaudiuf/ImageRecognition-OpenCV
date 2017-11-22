
/*
	ImageManager.h
	[Author]: Claudiu Furca
	[Copyright 2016]: (c) Amprent.	All rights reseved.
*/

#ifndef ImageManager_h
#define ImageManager_h

/* Declare here all the include files */
#include "HeaderFiles.h"

/* Declare here all the defines */
#define CV_LOAD_IMAGE_COLOR       1
#define CV_LOAD_IMAGE_GRAYSCALE   0
#define CV_LOAD_IMAGE_UNCHANGED  -1
#define LAYER_TYPE_CV8U 0
#define LAYER_TYPE_BGR2GRAY	8

#define MODEL_WEIGHT 0.85
/*! Values used for testing */
#define T_FAIL 1
#define T_PASS 2

/* Class declaration */
class ImageManager
{
/* Declare here all the public`s */
public:
	/*! Retrive the layer mask contours */
	static void ImageManager::getImageMaskContour(const cv::Mat& input,cv:: Mat& output, std::vector<std::vector<cv::Point> >& out_contours);
	/*  [Define Helper Function] Find Contours in the input image */
	static void ImageManager::_findContour(cv::Mat& input, std::vector<std::vector<cv::Point> >& output);
	/* Apply SURF to detect and compute the shape */
	/*! Find the mean in the Bounding box around an contour */
	static void ImageManager::getBoundingRectMean(const cv::Mat& input, std::vector<cv::Point>& contours,
		cv::Scalar& r_mean, cv::Mat& output, cv::Rect& rectShape);
	/*! */
	static void ImageManager::drawOnImage(std::vector<std::vector<cv::Point> >& v_ccShape, const cv::Mat& org_img,
		cv::Mat& drawImage, cv::Rect& rectShape);
	/*! */
	static void ImageManager::copyMat(const cv::Mat& input, cv::Mat& output);
	/*! */
	static int ImageManager::getNumberChannels(const cv::Mat& input);
	/*! */
	static int ImageManager::getTypeMatrix(const cv::Mat& input);
	/* */
	static cv::Rect ImageManager::rotateRect90(cv::Mat& input);
	/* [Define Helper Function] Find the Contours in the input image and draw the outcome */
	static void ImageManager::findContoursAndDraw(const cv::Mat& input, cv::Mat& output);
	/* [Testing Feature] */
	static void ImageManager::failTest(int type);
	/* [Testing Feature Draw] */
	static void ImageManager::failtestDraw(const cv::Mat& input, void*);
	
/* Declare here all the private`s */
private:
	/*! Declare here all the methods declaration */
	//
	/*! Declare here all the variables declaration */
	//
};

#endif /* End ImageManager_h*/


