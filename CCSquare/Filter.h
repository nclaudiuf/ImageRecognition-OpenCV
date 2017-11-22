
/*
	Filter.h
	[Author]: Claudiu Furca
	[Copyright 2016]: (c) Amprent.	All rights reseved.
*/

#ifndef Filter_h
#define Filter_h

/* Declare here all the include files */
#include "HeaderFiles.h"

/* Declare here all the defines */
#define GRADIENT_WEIGHT 0.5
/*! Value used for adaptiveThreshold, threshold */
#define MAX_VALUE 255
#define ADAPTIVE_METHOD_MEAN ADAPTIVE_THRESH_MEAN_C
#define ADAPTIVE_METHOD_GAUSSIAN ADAPTIVE_THRESH_GAUSSIAN_C
#define THRESHHOLD_TYPE_BINARY THRESH_BINARY
#define THRESHHOLD_TYPE_BINARY_INV THRESH_BINARY_INV
#define T_THRESH 1
#define BLOCK_SIZE 9
#define CONSTANT 15
/*! [General Definitions] */
#define ANCHOR_POINTS Point(-1, -1)
#define M_SIZE Size(3, 3)
#define BORDER_TYPE BORDER_DEFAULT
#define _IT 1
#define KERNEL_SIZE 3

/* [Remove Noise Features] */
enum Noise 
{
	DL_ER = 1, DL = 2, ER = 3
};

/* Class declaration */
class Filter
{
/* Declare here all the public`s */
public:
	/*! Apply layer mask on the model image */
	static void Filter::applyMaskOnModel(const cv::Mat& input, cv::Mat& output);
	/*! Apply layer mask on the scene image */
	static void Filter::applyMaskOnScene(const cv::Mat& input, cv::Mat& output);
	/*! Convert a Matrix to Grayscale channel */
	static void Filter::convertToGray(const cv::Mat& input, cv::Mat& output);
	/*! Apply box filter and add weight -*/
	static void Filter::applyBoxFilter(const cv::Mat& input, cv::Mat& output);
	/*! Apply Schaar Filtering on the Matrix  */
	static void Filter::applyScharrFiter(const cv::Mat& input, cv::Mat& output);
	/*! Apply Morphology Filtering on the Matrix */
	static void Filter::applyMorphology(const cv::Mat& input, cv::Mat& output);
	/*! Apply Filter2D on the Matrix */
	static void Filter::loadFilter2D(const cv::Mat& input, cv::Mat& output);
	/*! Apply Threshold - use default value T_THRESH(1) || thresh */
	static void Filter::applyThreshold(const cv::Mat& input, cv::Mat& output, int thresh);
	/*! Remove the unnecessary noise */
	static void Filter::removeNoise(const cv::Mat& input, cv::Mat& output, int type);
	/*! Apply Laplace contour enhancement */
	static void Filter::applyLaplace(const cv::Mat& input, cv::Mat& output);
	static void Filter::firstApproachMask(const cv::Mat& input, cv::Mat& output);
private:
	/* Declare here all the methods declaration */
	static inline uchar Filter::reduceVal(const uchar val);
	static void Filter::processColors(const cv::Mat& input);	
	/* Declare here all the variables declaration */
	//
};

#endif // !End Filter_h