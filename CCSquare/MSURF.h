
/*
	MSURF.h
	[Author]: Claudiu Furca
	[Copyright 2016]: (c) Amprent.	All rights reseved.
*/

#ifndef MSUF_h
#define MSURF_h

/* Declare here all the include files */
#include "HeaderFiles.h"

/* Declare here all the defines */
#define MIN_HESSIAN 400

/* Class declaration */
class MSURF
{
/* Declare here all the public`s */
public:
	/*! Apply SURF to detect and compute the shape */
	static void MSURF::SURFDetectAndCompute(const cv::Mat& inputModel, const cv::Mat& inputScene, cv::Mat& outputModel, cv::Mat& outputScene);
	static void MSURF::useSurfTesting(const cv::Mat& img_object, const cv::Mat& img_scene);
/* Declare here all the private`s */
private:
	/*! Declare here all the methods declaration */
	//
	/*! Declare here all the variables declaration */
	const char* docSurf = "http://docs.opencv.org/2.4/doc/tutorials/features2d/feature_flann_matcher/feature_flann_matcher.html";
};

#endif /*! END SURF_h*/