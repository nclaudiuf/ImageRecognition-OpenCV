
/*
	MSURF.cpp
	[Author]: Claudiu Furca
	[Copyright 2016]: (c) Amprent.	All rights reseved.
*/

/* Declare here all the include files */
#include "HeaderFiles.h"

/* Detect and Track the features in the Mask model using SURF opencv*/
void MSURF::SURFDetectAndCompute(const Mat& inputModel, const Mat& inputScene, Mat& outputModel, Mat& outputScene)
{
	if (inputModel.rows > 0 && inputScene.rows > 0 && inputModel.type() == LAYER_TYPE_CV8U && inputScene.type() == LAYER_TYPE_CV8U)
	{
		Mat _inputModel = inputModel.clone();
		Mat _inputScene = inputScene.clone();
		vector<KeyPoint> keypointsModel;
		vector<KeyPoint> keypointsScene;

		/*! Create the SurfFeatureDetector to pointer */
		Ptr<FeatureDetector> detector = SurfFeatureDetector::create();
		/*! Detect the keypoints using SURF Detector */
		detector->detect(_inputModel, keypointsModel);
		detector->clear();
		detector->detect(_inputScene, keypointsScene);
		detector->clear();

		/*! Calculate descriptors (feature vectors) */
		Mat _outputModel;	
		Mat scene_output;

		/*! Create the SurfDescriptorExtractor to a pointer */
		Ptr<DescriptorExtractor> extractor = SurfDescriptorExtractor::create();
		detector->compute(_inputModel, keypointsModel, _outputModel);
		extractor->clear();
		detector->compute(_inputScene, keypointsScene, scene_output);
		extractor->clear();

		/*! retrive & clean up after you */
		outputScene.push_back(scene_output);
		outputModel.push_back(_outputModel);		
		_inputModel.release();
		_inputScene.release();
		scene_output.release();
		_outputModel.release();
		keypointsModel.clear();
		keypointsScene.clear();		
	}
}

/* This method represents an external SURF Test - use for @ */
void MSURF::useSurfTesting(const Mat& img_object,const  Mat& img_scene)
{
	clock_t t0, t1;

	Mat ccMaskModel = img_object;
	Mat model_mask;

	/*! Apply Image Filtering and get the layer mask from the CC Model image */
	Filter::applyMaskOnModel(img_object, model_mask);
	//_mask.create(ccMaskModel.size(), LAYER_TYPE_CV8U);
	imshow("ccMaskModel", model_mask);

	Mat _input = img_scene;
	/*! Apply Filter Mask Layer on the input image layer */
	Mat ccMask;
	vector<vector<Point> > out_contours;
	ImageManager::getImageMaskContour(_input, ccMask, out_contours);
	imshow("ccMask", ccMask);

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;
	Ptr<SurfFeatureDetector> detector = SurfFeatureDetector::create(minHessian);

	std::vector<KeyPoint> keypoints_1, keypoints_2;
	detector->detect(model_mask, keypoints_1);
	detector->detect(ccMask, keypoints_2);

	//cout << "\nccMask size d " << ccMask.size();
	//cout << "\n_mask size d " << _mask.size();

	//-- Step 2: Calculate descriptors (feature vectors)
	Ptr<SurfDescriptorExtractor > extractor = SurfDescriptorExtractor::create();

	Mat descriptors_1, descriptors_2;

	extractor->compute(model_mask, keypoints_1, descriptors_1);
	extractor->compute(ccMask, keypoints_2, descriptors_2);

	//cout << "\nccMask size e " << ccMask.size();
	//cout << "\n_mask size e " << _mask.size();

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	//FlannBasedMatcher matcher;
	//std::vector< DMatch > matches;
	//matcher.match(descriptors_1, descriptors_2, matches);

	/*! Search for Nearest Neighor */
	//flannIndex.knnSearch(_object_descriptor, _indices, _dists, KNN, flann::SearchParams());
	const cv::Ptr<cv::flann::IndexParams>& indexParams = new cv::flann::KDTreeIndexParams(4);
	const cv::Ptr<cv::flann::SearchParams>& searchParams = new cv::flann::SearchParams();

	//cout << "\n_descriptors_1 type " << _descriptors_1.type();
	//cout << "\n_descriptors_2 type " << _descriptors_2.type();
	cout << "\ndescriptors_1 type " << descriptors_1.type();
	cout << "\ndescriptors_2 type " << descriptors_2.type();

	//cout << "\n_descriptors_1 size " << _descriptors_1.size();
	//cout << "\n_descriptors_2 size " << _descriptors_2.size();
	cout << "\ndescriptors_1 size " << descriptors_1.size();
	cout << "\ndescriptors_2 size " << descriptors_2.size();

	//vector<DMatch> matches;
	std::vector<std::vector<cv::DMatch> > matches;
	BFMatcher matcher(cv::NORM_L2);
	matcher.add(descriptors_1);
	matcher.train();

	t0 = clock();
	matcher.knnMatch(descriptors_1, descriptors_2, matches, KNN);
	//FlannBasedMatcher matcher(indexParams, searchParams);
	//matcher.add(descriptors_1);
	//matcher.train();
	//matcher.match(_descriptors_2, matches);
	t1 = clock();
	std::cout << "\nTotal Computation knnMatch " << static_cast<double>(t1 - t0) / CLOCKS_PER_SEC << " m/s" << endl;

	double max_dist = 0; double min_dist = 100;
	cout << "\ndescriptors_1 size & " << descriptors_1.size();
	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_1.rows; i++)
	{
		double dist = matches[i][0].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);

	//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
	//-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
	//-- small)
	//-- PS.- radiusMatch can also be used here.
	std::vector< DMatch > good_matches;

	for (int i = 0; i < descriptors_1.rows; i++)
	{
		if (matches[i][0].distance <= max(2 * min_dist, 0.1))
		{
			good_matches.push_back(matches[i][0]);
		}
	}

	//-- Draw only "good" matches
	Mat img_matches;
	drawMatches(img_object, keypoints_1, img_scene, keypoints_2,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (int i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_1[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_2[good_matches[i].trainIdx].pt);
	}

	//if (obj.size() > 0 && scene.size() > 0)
	//{
	Mat H = findHomography(obj, scene, CV_RANSAC);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(img_object.cols, 0);
	obj_corners[2] = cvPoint(img_object.cols, img_object.rows); obj_corners[3] = cvPoint(0, img_object.rows);
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches, scene_corners[0] + Point2f(img_scene.cols, 0), scene_corners[1] + Point2f(img_scene.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(img_scene.cols, 0), scene_corners[2] + Point2f(img_scene.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(img_scene.cols, 0), scene_corners[3] + Point2f(img_scene.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(img_scene.cols, 0), scene_corners[0] + Point2f(img_scene.cols, 0), Scalar(0, 255, 0), 4);

	//-- Show detected matches
	imshow("Good Matches & Object detection", img_matches);
	//}	
}