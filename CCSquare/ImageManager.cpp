
/*
	ImageManager.cpp
	[Author]: Claudiu Furca
	[Copyright 2016]: (c) Amprent.	All rights reseved.
*/

/* Declare here all the include files */
#include "ImageManager.h"

/*! Declare here all the variables declaration */
namespace
{
	RNG rng(12345);
	clock_t t0, t1;
}

/* Apply a layer mask in order to remove unecessary details which does not fit with the shape(CC) */
void ImageManager::getImageMaskContour(const Mat& input, Mat& output, vector<vector<Point> >& out_contours)
{
	Mat _input = input.clone();		
	Mat scene_mask;
	t0 = clock();
	/* Apply the Mask on the Image Scene */
	Filter::applyMaskOnScene(_input, scene_mask);
	//Filter::firstApproachMask(_input, scene_mask);
	t1 = clock();
	std::cout << "\nComputation Time[applyMaskOnScene]: " << static_cast<double>(t1 - t0) / CLOCKS_PER_SEC << " m/s" << endl;
	/*! Get all the contours detected in the image */
	vector<vector<Point> > _out_contours;
	_findContour(scene_mask, out_contours);
	output.push_back(scene_mask);
	//out_contours = _out_contours;
}

/*  [Define Helper Function]
Find Contours in the input image */
void ImageManager::_findContour(Mat& input, vector<vector<Point> >& contours)
{
	Mat _input = input.clone();
	/*! Find the contours in the image layer */
	findContours(_input, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
}

/* [Define Helper Function]
Find the Contours in the input image and draw the outcome */
void ImageManager::findContoursAndDraw(const Mat& input, Mat& output)
{
	Mat _input(input);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	/*! Find Contours */
	findContours(_input, contours, hierarchy, CV_RETR_LIST, CHAIN_APPROX_SIMPLE, Point(0,0));


	/*! Draw Contours */
	Mat drawing = Mat::zeros(_input.size(), CV_8UC3);
	/*! Find the convex hull object for each contour */
	//vector<vector<Point> > hull(contours.size());
	for (int i = 0; i< contours.size(); i++)
	{
		//convexHull(Mat(contours[i]), hull[i], false);
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		// Rect& box
		//drawContours(drawing, v_ccShape, i, color, lineThickness, lineType, hierarchy, 0, Point(-box.x, -box.y));
		//drawContours(drawing, hull, i, color, 2, 8, hierarchy, 0, Point());
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}
	output.push_back(drawing);
}	

void ImageManager::getBoundingRectMean(const Mat& input, vector<Point>& contours, Scalar& r_mean, Mat& output, Rect& rectShape)
{
	Mat _input = input.clone();
	/*! Get bounding Rect around, detected shape */
	Rect box = boundingRect(Mat(contours));
	/*! Set the value of pixels not in the contours region to zero */
	Mat boxImage = Mat(_input, box);
	/* Obtain the mean intesity */
	r_mean = mean(boxImage);
	output.push_back(boxImage);
	rectShape = box;
}

/* [Define Helper Function]
Draw on Image */
void ImageManager::drawOnImage(vector<vector<Point> >& v_ccShape, const Mat& org_img, Mat& drawImage, Rect& rectShape)
{
	Mat _org_img = org_img.clone();

	vector<Vec4i> hierarchy;

	int lineThickness = 2;
	int lineType = 8;
	if (_org_img.data && _org_img.rows > 0 && v_ccShape.size() > 0)
	{
		Mat finish_draw = Mat::zeros(_org_img.size(), CV_8UC3);
		finish_draw = _org_img;

		for (size_t i = 0; i < v_ccShape.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			const Point* p = &v_ccShape[i][0];
			int n = (int)v_ccShape[i].size();
			//(finish_draw, &p, &n, 1, true, Scalar(255, 0, 0) , 1, LINE_AA);
			rectangle(finish_draw, rectShape.tl(), rectShape.br(), color, 2, 8, 0);			//rectShape[i].tl /br
		}
		drawImage.push_back(finish_draw);
	}	
}

void ImageManager::copyMat(const Mat& input, Mat& output)
{
	Mat image = input.clone();
	output.push_back(image);
}

/* Get the Total Ammount of Channels from the image*/
int ImageManager::getNumberChannels(const Mat& input)
{
	return input.channels();
}

/* Return The Type of the Matrix */
int ImageManager::getTypeMatrix(const Mat& input)
{
	return input.type();
}

Rect ImageManager::rotateRect90(Mat& input)
{
	int angle = 90;
	RotatedRect rRect = RotatedRect(Point2f(100, 100), Size2f(100, 50), angle);

	Point2f vertices[4];
	rRect.points(vertices);
	for (int i = 0; i < 4; i++)
		line(input, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));

	Rect brect = rRect.boundingRect();
	rectangle(input, brect, Scalar(255, 0, 0));

	return brect;
}

/* [Testing Feature] */
void ImageManager::failTest(int type)
{
	if (type == T_FAIL)
	{
		printf("\n@Test Fail: -- @\t");
	}		
	else if(type == T_PASS)
	{
		printf("\n@Test Pass: -- @\t");
	}
	else
	{
		printf("\n@Test Pass: -- @ %d\t", type);
	}
}

/* [Testing Feature Draw] */
void ImageManager::failtestDraw(const Mat& input, void*)
{
	Mat _input = input.clone();
	int c = 0;
	if (!_input.data && _input.rows <= 0)
	{
		printf("\n@Fail Test Draw: -- @ %d\t", c);
		c++;
	}
	else
	{
		/*! Show Outcome in a new window ++*/
		namedWindow("@[Test Pass]Display Outcome: ==> on screen " + c, CV_WINDOW_AUTOSIZE);
		imshow("@[Test Pass]Display Outcome: == > on screen " + c, _input);
		c++;
	}
}