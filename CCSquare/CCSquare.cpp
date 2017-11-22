/*
	CCSquare.cpp
	[Author]: Claudiu Furca
	[Copyright 2016]: (c) Amprent.	All rights reseved.
*/

/* Declare here all the include files */
#include "CCSquare.h"

/*! Declare here all the variables declaration */
namespace 
{
	Mat surfDetectionModel;
	vector<vector<Point> > _modelContour;
	Mat _mask;
	clock_t t0, t1;
	bool restoreImage = false;
}

/* [Define Helper Function]
Finds a coine of angle between vector points
from pt0->pt1 and from pt0->pt2 */
static double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

/* Construct a 4x4 square shape init,
	Detect and Compute the model image */
CCSquare::CCSquare() : m_ccShape(KERNEL_SIZE, KERNEL_SIZE, LAYER_TYPE_CV8U, Scalar(1))
{
	Mat ccMaskModel = imread(ccModel, CV_LOAD_IMAGE_GRAYSCALE);
	Mat _inputObject = ccMaskModel.clone();
	Mat imageDownObject;
	/* Blur the model and downsample it */
	pyrDown(_inputObject, imageDownObject, Size(_inputObject.cols / 2, _inputObject.rows / 2));
	imageDownObject.create(Size(_inputObject.cols / 2, _inputObject.rows / 2), LAYER_TYPE_CV8U);
	if (imageDownObject.data && imageDownObject.rows > 0)
	{
		/*! Apply Image Filtering and get the layer mask from the CC Model image */
		Filter::applyMaskOnModel(imageDownObject, _mask);
		/*! Obtain the contour of the model area */
		ImageManager::_findContour(_mask, _modelContour);
		imshow("Mask", _mask);
		surfDetectionModel.push_back(_mask);
	}
}

/* [Define MAIN Function]
	Detected the CC Square shape and draw it`s contours in the canvas */
void CCSquare::getDrawOutput(const Mat& img_scene, Mat& drawingOutput)
{
	/* Get the instance of CC Square Model */
	CCSquare::CCSquare();
	/* Receive Image */
	Mat _input = img_scene.clone();
	Mat imageDownScene;
	//t0 = clock();
	/* blur the image and downsample it */
	pyrDown(_input, imageDownScene, Size(_input.cols / 2, _input.rows / 2));				//@ Computation(avg 20 trails): 0.024 ms
	//t1 = clock();
	//std::cout << "\nComputation Time [imageDownScene]: " << static_cast<double>(t1 - t0) / CLOCKS_PER_SEC << " m/s" << endl;
	imageDownScene.create(Size(_input.cols / 2, _input.rows / 2), _input.type());

	if (_input.data && imageDownScene.data)
	{
		Mat result;		Rect rectShape;
		vector<vector<Point> >  outputCCSquare;
		imshow("imageDownScene", imageDownScene);
		//t0 = clock();
		/*! Detect shape */
		detectCCSquare(imageDownScene, result, outputCCSquare, rectShape);		
		//detectCCSquare(_input, result, outputCCSquare, rectShape);
		//t1 = clock();
		//std::cout << "\nTotal Computation Time: " << static_cast<double>(t1 - t0) / CLOCKS_PER_SEC << " m/s" << endl;

		imshow("result", result);				//TO DELETE IF MOBILE 

		if (outputCCSquare.size() > 0)
		{
			/*! Draw */
			Mat drawOnImage;
			ImageManager::drawOnImage(outputCCSquare, imageDownScene, drawOnImage, rectShape);
			/* If needed restore the image size to Full size */
			if (restoreImage)
			{
				Mat restoreSize;
				pyrUp(drawOnImage, restoreSize, Size(drawOnImage.cols * 2, drawOnImage.rows * 2));
				/*! Test Drawing */
				imshow("drawOnImage", restoreSize);			//TO DELETE IF MOBILE 
				drawingOutput.push_back(restoreSize);
			}
			else
			{
				/*! Test Drawing */
				imshow("drawOnImage", drawOnImage);			//TO DELETE IF MOBILE 
				drawingOutput.push_back(drawOnImage);
			}		
		}
		else	
			drawingOutput.push_back(_input);	
	}
	waitKey(0);				//TO DELETE IF MOBILE 
}

/* [Define Helper Function] 
	Apply layer mask on the image scene and get the contours */
void CCSquare::detectCCSquare(const Mat& input, Mat& output, vector<vector<Point> >& outputCCSquare, Rect& rectShape)
{
	/*! Get the Working Image with the Mask */
	Mat _input = input.clone();	
	Mat scene_mask;
	/*! Apply Filter Mask Layer on the input image layer */
	vector<vector<Point> > out_contours;
	/*! Retrive all the contours detected in the image scene */
	ImageManager::getImageMaskContour(_input, scene_mask, out_contours);
	/*! Find the CC shape in the image Scene (Filter +/ Mask + Contour Layer +/ SURF +/ FLANN Match +/ Square rule */
	if (out_contours.size() > 0)
	{
		scene_mask.create(_input.size(), LAYER_TYPE_CV8U);
		/*! Find the CC Square shape in the canvas */
		FindCCSquare(scene_mask, _input, out_contours, outputCCSquare, rectShape);
		/*! Retrive outcome if any (vector<vector<Point> >) */
		output.push_back(scene_mask);	
	}
}

/* [Define Helper Function]
	Finds the square in the image */
void CCSquare::FindCCSquare(const Mat& input, const Mat& org_img, const vector<vector<Point> >& _contours, 
	vector<vector<Point> >& outputCCSquare, Rect& rectShape)
{
	/* Fresh start */
	outputCCSquare.clear();
	/*! Get the Working Image with the Mask */
	Mat out_contours = input.clone();
	vector<Point> approx;		vector<KeyPoint> _surfMatchesScene;
	/* Test the obtain contours for a match */
	for (size_t i = 0; i < _contours.size(); i++)
	{	
		/*! Approximate the contours with accuracy proportional to the contour perimeter of the shape */
		approxPolyDP(Mat(_contours[i]), approx, arcLength(Mat(_contours[i]), true)*0.02, true);
		/*! Make Sure For a good match
		Square contours should have 4 vertices after approximation,
		ApproxPoly for large areas or uncleared images (to filter out noisy contours)
		!Note: Due to the performance of the input image or other factors the vertices ( less < 4 )
			vertices filters from range 3 - 8(without)
		!Note: absolute value of an area is used because area may be positive
			or negative -> in accordance with the contour orietation */
		if (fabs(approx.size()) > 3 && fabs(contourArea(Mat(approx))) >= CC_SIZE / 3 && fabs(contourArea(Mat(approx))) <= CC_SIZE * 3)
		{
			/*! Match the CC Shape and the contour Area and get the ratio,
				an comparison with a matrix from an image(still) ~ accurate tested under +- light,
				an ratio over 0.45 has been optain after matching model image with image scene,
			!Note: as in real time frame capturing might result in blurred or distored matrices, 
			thus an tolerance level will be apply */
			double ratio = matchShapes(_modelContour[0], approx, CV_CONTOURS_MATCH_I3, 0);
			/*! If a match found */
			if (ratio > MATCH_RATIO)
			{
				Mat boxView;	Scalar _mean;	Rect _rectShape;
				/*! Obtain the bounding Rect from contours mean */
				ImageManager::getBoundingRectMean(out_contours, approx, _mean, boxView, _rectShape);
				
				for (size_t it = 0; it < _mean.rows; it++)
				{
					if (_mean[it] > MEAN_RATIO && boxView.rows > 0 || boxView.cols > 0)
					{		
						Mat _model_descriptor;		Mat _scene_descriptor;
						/*! Use SURF to Detect and Compute */
						MSURF::SURFDetectAndCompute(surfDetectionModel, boxView, _model_descriptor, _scene_descriptor);
						/*! Use Flann to match the Image Scene and the model */
						vector<DMatch> good_matches;	double maxFlannDist = 0;
						m_FLANN::getFlannMatch(_model_descriptor, _scene_descriptor, good_matches);
						
						/* Handle Exception */
						if(good_matches.size() > 0)
						/*! Got a match */
						if (good_matches[0].distance > MAX_FLAN_DIST)
						{
							double maxCosine = 0;
							for (int j = 2; j < 5; j++)
							{
								/*!  Find the maximum cosine of the angle between joint edges */
								double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
								maxCosine = MAX(maxCosine, cosine);
							}
							/*! If cosines of all angles are small, all angles are ~90 degree,
								write quandrange vertices to result sequence */
							if (maxCosine < COS_RATIO)
							{
								/*! Get the CC Square Shape and its shape */
								outputCCSquare.push_back(approx);
								rectShape = _rectShape;
							}														
						}						
					}
				}
			}
		}		
	}
}




