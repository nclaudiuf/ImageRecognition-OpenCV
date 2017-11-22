
/*
	CCSquare.h
	[Author]: Claudiu Furca
	[Copyright 2016]: (c) Amprent.	All rights reseved.  
*/

#ifndef CCSquare_h
#define CCSquare_h

/* Declare here all the include files */
#include "HeaderFiles.h"

/* Declare here all the define`s */
/*! [Angle Tolerance]The tolerance level for the angles detected in the contours */
#define TOLERANCE 0.1
/*! [Define here the CC max square area perimeter] */
#define CC_SIZE 144
#define SIDES 4
/* Matching Parameters */
/*! Best value match =~ over 0.5 > */
#define MATCH_RATIO 0.5
#define MEAN_RATIO 57
#define COS_RATIO 0.55
/*! Flann Match Ratio */
#define MAX_FLAN_DIST 0.3

/* Class declaration */
class CCSquare
{
/* Declare here all the public`s */
public:
	CCSquare();
	/*! Detect the CC Squares */
	static void CCSquare::getDrawOutput(const cv::Mat& img_scene, cv::Mat& drawingOutput);
	static void CCSquare::detectCCSquare(const cv::Mat& input, cv::Mat& output,
		std::vector<std::vector<cv::Point> >& outputCCSquare, cv::Rect& rectShape);
/* Declare here all the private`s */
private:	
	/*! Declare here all the methods declaration */
	static void CCSquare::FindCCSquare(const cv::Mat& input, const cv::Mat& org_img, const std::vector<std::vector<cv::Point> >& _contours,
		std::vector<std::vector<cv::Point> >& outputCCSquare, cv::Rect& rectShape);
	/*! Declare here all the variables declaration */
	cv::Mat m_ccShape;
	std::vector<std::vector<cv::Point> > v_ccShape;
	/*! Model image path - used for creating a model mask */
	const char* ccModel = "Resources/ccModel_3x.jpg";
};

#endif //End CCSquare_h