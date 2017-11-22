
/*
	Filter.cpp
	[Author]: Claudiu Furca
	[Copyright 2016]: (c) Amprent.	All rights reseved.
*/

#include "Filter.h"

/* Define here all the namespace`s */
namespace
{
	/*! Define the model filter - square */
	//KERNEL_SIZE = 3
	cv::Mat _ccShape(KERNEL_SIZE, KERNEL_SIZE, LAYER_TYPE_CV8U, Scalar(1));
}

/* Create a layer mask for the model image */
void Filter::applyMaskOnModel(const Mat& input, Mat& output)
{
	Mat _input = input.clone();
	Mat imageGray, morph, _morph;
	convertToGray(_input, imageGray);
	/*! Create a shape distorion effect in order to provide a closer perspective to reality 
		or apply pyrDown => faster  */
	//morphologyEx(imageGray, morph, MORPH_RECT, _ccShape);
	//morphologyEx(morph, _morph, MORPH_CLOSE, _ccShape);
	Mat scharr;
	applyScharrFiter(imageGray, scharr);
	output.push_back(scharr);
}

/* Create layer mask for image scene */
void Filter::applyMaskOnScene(const Mat& input, Mat& output)
{
	clock_t t0, t1;
	Mat _input = input.clone();
	Mat_<Vec3b> rgbInput(_input.size(), Vec3b(255, 255, 255));
	rgbInput = _input;
	/* Apply Color Mask Filter */
	t0 = clock();
	if (rgbInput.channels() == 3)
	{
		processColors(rgbInput);			//@ Computation(avg 20 trails): 0.047 ms
	}
	imshow("processColors", rgbInput);
	//t1 = clock();
	//std::cout << "\nComputation Time to processColors: " << static_cast<double>(t1 - t0) / CLOCKS_PER_SEC << " m/s" << endl;

	//t0 = clock();
	Mat imageGray;
	convertToGray(rgbInput, imageGray);		 //@ Computation(avg 20 trails): 0.010 ms
	//t1 = clock();
	//std::cout << "\nComputation Time to convertToGray: " << static_cast<double>(t1 - t0) / CLOCKS_PER_SEC << " m/s" << endl;

	Mat morph, morphClose, enhance;
	//t0 = clock();
	applyMorphology(imageGray, morph);		 //@ Computation(avg 20 trails): 0.006 ms
	//t1 = clock();
	//std::cout << "\nComputation Time to applyMorphology: " << static_cast<double>(t1 - t0) / CLOCKS_PER_SEC << " m/s" << endl;
	//imshow("applyMorphology", morph);
	//t0 = clock();
	Mat laplace;
	applyLaplace(morph, laplace);			 //@ Computation(avg 20 trails): 0.015 ms
	//t1 = clock();
	//std::cout << "\nComputation Time to applyLaplace: " << static_cast<double>(t1 - t0) / CLOCKS_PER_SEC << " m/s" << endl;
	//imshow("applyLaplace", laplace);
	//t0 = clock();
	Mat thresh;
	applyThreshold(laplace, thresh, 127);	 //@ Computation(undefiend - depending on image): lwo - 0.0000001 ms to hight - 0.001 ms
	//t1 = clock();
	//std::cout << "\nComputation Time to applyThreshold: " << static_cast<double>(t1 - t0) / CLOCKS_PER_SEC << " m/s" << endl;
	//imshow("applyThreshold", thresh);
	output.push_back(thresh);				 //@ Total Computation(avg 20 trails): 0.094 ms 
}

/* Reduce the colors from 3 channels matrix(0,255), where MAX_VALUE represents the max color representation 
   Example; (0,255,0) = green; thus CC being composed only by Scalar MAX_VALUES; taking a MAX_VALUE / 2(color in distortion from capturing)
   and returing MAX_VALUE/5 will enhance the colors. and MAX_VALUe/4 was made in order to remvoe the 
   white spots and returing MAX_VALUE to force the dark areas to merge */
inline uchar Filter::reduceVal(const uchar color)
{
	if (color < MAX_VALUE / 4) return 0;	
	if (color < MAX_VALUE / 2) return MAX_VALUE / 5;	
	return MAX_VALUE;
}
//MAX_VALUE = 255
/* Split image channel into RGB, and Apply Color reduction to enhance
   detection of CC Square. 
   !Note: Dark color will become black, light will become white and reduce 
   coloring for rest */
void Filter::processColors(const Mat& input)
{
	uchar* pixelPtr = input.data;
	for (int i = 0; i < input.rows; i++)
	{
		for (int j = 0; j < input.cols; j++)
		{
			const int pi = i * input.cols * 3 + j * 3;
			pixelPtr[pi + 0] = reduceVal(pixelPtr[pi + 0]); // B
			pixelPtr[pi + 1] = reduceVal(pixelPtr[pi + 1]); // G
			pixelPtr[pi + 2] = reduceVal(pixelPtr[pi + 2]); // R
		}
	}
}

/* Convert the Matrix to Grayscale channel and return the output */
void Filter::convertToGray(const Mat& input, Mat& output)
{
	const int numChannes = input.channels();

	if (numChannes == 4)
	{
#if TARGET_IPHONE_SIMULATOR
		cv::cvtColor(input, gray, cv::COLOR_BGRA2GRAY);
#else
		cv::cvtColor(input, output, CV_BGRA2GRAY);
#endif
	}
	else if (numChannes == 3)
	{
		cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
	}
	else if (numChannes == 1)
	{
		output = input;
	}
}

/*! Create customize boxFilter */
void Filter::applyBoxFilter(const Mat& input, Mat& output)
{
	bool m_normalize = true;
	//ANCHOR_POINTS = Point(-1, -1)
	//BORDER_TYPE = BORDER_DEFAULT
	//GRADIENT_WEIGHT = 0.5
	//_IT = 1
	Mat filter;
	boxFilter(input, filter, LAYER_TYPE_CV8U, _ccShape.size(), ANCHOR_POINTS, m_normalize, BORDER_TYPE);
	addWeighted(filter * GRADIENT_WEIGHT, _IT, filter * GRADIENT_WEIGHT, _IT, BORDER_TYPE, output);
}

/* Apply Schaar Filtering on Matrix and return the output */
void Filter::applyScharrFiter(const Mat& input, Mat& output)
{
	Mat input_gray, c_input_x, c_input_y, abs_input_x, abs_input_y;
	const int scale = 1;	const int delta = 0;	const int ddepth = CV_16S;
	/*! Gradient X */
	Scharr(input, c_input_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(c_input_x, abs_input_x);
	/*! Gradient Y */
	Scharr(input, c_input_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(c_input_y, abs_input_y);
	/*! Total Gradient (approximate) */
	Mat gradient; 
	gradient.create(input.size(), input.type());
	//GRADIENT_WEIGHT = 0.5
	//_IT = 1
	/*! Add Weight on Layer image */
	addWeighted(abs_input_x * GRADIENT_WEIGHT, _IT, abs_input_y * GRADIENT_WEIGHT, _IT, BORDER_TYPE, gradient);
	output.push_back(gradient);
}

/* Apply Morphology Filtering on Matrix and return the output */
void Filter::applyMorphology(const Mat& input, Mat& output)
{
	/*! Define the model filter - square */
	//KERNEL_SIZE = 3
	cv::Mat _ccShape(KERNEL_SIZE, KERNEL_SIZE, LAYER_TYPE_CV8U, Scalar(1));
	Mat morph, _output, morphDL;
	//_output.create(input.size(), input.type());
	/*! Use dilate operation to remote detials, 
		use close operation to remote details,
		use rect operation to enhance edges of the square */
	morphologyEx(input, morphDL, MORPH_DILATE, _ccShape);
	morphologyEx(morphDL, morph, MORPH_CLOSE, _ccShape);
	morphologyEx(morph, _output, MORPH_RECT, _ccShape);
	/*! Retrive */
	output.push_back(_output);
}

/* Apply loadFilter2D on Matrix and return the output */
void Filter::loadFilter2D(const Mat& input, Mat& output)
{
	const int delta = 0;
	//KERNEL_SIZE = 3;
	//BORDER_TYPE = BORDER_DEFAULT
	/*! Create the kerne, using in depth of image(32-bit unsigned int) and Update it the size for a normalized box filter*/
	Mat kernel = Mat::ones(KERNEL_SIZE, KERNEL_SIZE, CV_32F) / (float)(KERNEL_SIZE*KERNEL_SIZE);
	/* Aplly Filter2D using; in depth kernel, Point and color space 8-bit unsigned uchar */	
	Mat filter, abs_filter;
	filter2D(input, filter, LAYER_TYPE_CV8U, kernel, ANCHOR_POINTS, delta, BORDER_TYPE);
	/*! */
	convertScaleAbs(filter, abs_filter);
	/*! Prepare the output having the input image size and color space(8-bit usigned uchar) 
		Total gradient from image + the weight */
	Mat gradient(input.size(), LAYER_TYPE_CV8U);
	//ANCHOR_POINTS Point(-1, -1)
	//_IT = 1
	//GRADIENT_WEIGHT = 0.5
	/*! Add Weight(=enhance edges) on Layer image */
	addWeighted(abs_filter * GRADIENT_WEIGHT, _IT, abs_filter * GRADIENT_WEIGHT, _IT, BORDER_TYPE, gradient);
	/*! Retrive - the image filtered & enhance edge */
	output.push_back(gradient);
}

/* Apply Threshold using binary output */
void Filter::applyThreshold(const Mat& input, Mat& output, int thresh)
{
	//MAX_VALUE = 255
	//T_THRESH = 1
	Mat _input = input.clone();
	/*! Check if the value of thresh has been changed -> ifCHanged = adjust threshold, 
	    accordingly to the desired thresh value */
	if (thresh == T_THRESH)
		threshold(_input, output, T_THRESH, MAX_VALUE, THRESHHOLD_TYPE_BINARY);
	else
		threshold(_input, output, thresh, MAX_VALUE, THRESHHOLD_TYPE_BINARY);
}

/* Remove the unnecessary noise */
void Filter::removeNoise(const Mat& input, Mat& output, int type)
{
	//ANCHOR_POINTS = Point(-1, -1)
	//_IT = 1
	//BORDER_TYPE = BORDER_DEFAULT
	/*! Create the working layer */
	Mat _smooth, _output;
	_output.create(input.size(), input.type());
	switch (type)
	{
	case DL_ER: 
		/*! Use Dilate followed by Erode to eliminate small gaps in some countours */
		dilate(input, _smooth, _ccShape, ANCHOR_POINTS, _IT, BORDER_TYPE);
		erode(_smooth, _output, _ccShape, ANCHOR_POINTS, _IT, BORDER_TYPE); break;
	case DL: 		
		dilate(input, _output, _ccShape, ANCHOR_POINTS, _IT, BORDER_TYPE);  break;
	case ER:
		erode(input, _output, _ccShape, ANCHOR_POINTS, _IT, BORDER_TYPE);  break;
	default:
		ImageManager::failTest(T_FAIL); break;
	}	
	output.push_back(_output);
}

/*! Apply Laplace to find the contours in image */
void Filter::applyLaplace(const Mat& input, Mat& output)
{
	Mat _input = input.clone(); 
	Mat input_gray, c_input_x, c_input_y, abs_input_x, abs_input_y;
	const int scale = 1;	const int delta = 0; 
	/*! Apply Laplace using CV_8U - 0 channels for faster computation using KERNEL_SIZE = 3 */
	Laplacian(_input, c_input_x, LAYER_TYPE_CV8U, KERNEL_SIZE, scale, delta, BORDER_TYPE);
	convertScaleAbs(c_input_x, abs_input_x);
	//Laplacian(_input, c_input_y, ddepth, 3, 0, 1, BORDER_TYPE);
	//convertScaleAbs(c_input_y, abs_input_y);
	/*! Total Gradient (approximate) */
	Mat gradient;
	gradient.create(input.size(), input.type());
	/*! Add Weight on Layer image */
	addWeighted(abs_input_x * GRADIENT_WEIGHT, _IT, abs_input_x * GRADIENT_WEIGHT, _IT, BORDER_TYPE, gradient);
	output.push_back(gradient);
}


/*! Total Computation time using this layer mask was approx 10s
which was way to hight for a real time tracking, thus a second layer mask-approach was established,
~NOTE: This method is not being used */
void Filter::firstApproachMask(const Mat& input, Mat& output)
{
	/*! Apply Box filter, normalize to CV_8UC3 and add weight (-) */
	Mat _input;
	applyBoxFilter(input, _input);
	Mat enhance;
	bilateralFilter(_input, enhance, 15, 80, 80, BORDER_TYPE);
	/*! Apply loadFiltering and add weight */
	Mat filter;
	loadFilter2D(enhance, filter);
	/*! Apply Scharr to enhance edges */
	Mat scharr;
	applyScharrFiter(filter, scharr);
	/*! CV_8UC3 is an 8-bit unsigned integer matrix/image with 3 channels,
	Although it is most common that this means an RGB (or actually BGR) */
	Mat_<Vec3b> rgbInput(_input.size(), Vec3b(255, 255, 255));
	rgbInput = scharr;
	/* Apply Color Mask Filter */
	if (rgbInput.channels() == 3)
	{
		processColors(rgbInput);
	}
	/*! Apply morphology for edge enhancement */
	Mat morph;
	applyMorphology(rgbInput, morph);
	/*! Remove Noise */
	Mat smooth;
	removeNoise(morph, smooth, DL);
	/* Convert to Gray */
	Mat imageGray;
	convertToGray(smooth, imageGray);
	/* Apply Threshold */
	Mat thresh;
	applyThreshold(imageGray, thresh, 100);
	output.push_back(thresh);
}