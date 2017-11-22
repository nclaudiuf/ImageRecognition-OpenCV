
/*
	FLANN.cpp
	[Author]: Claudiu Furca
	[Copyright 2016]: (c) Amprent.	All rights reseved.
*/

/* Declare here all the include files */
#include "FLANN.h"

//#pragma warning(disable : 4996)	if needed
//__declspec(deprecated)
/* Use FLANN to Match an input image and get the output */
void m_FLANN::getFlannMatch(const Mat& model_descriptor, const Mat& scene_descriptor, vector<DMatch>& good_matches)
{
	/* Check for data input */
	if (model_descriptor.rows > 0 && scene_descriptor.rows > 0)
	{
		/*! Copy main Matrices */
		Mat _model_descriptor = model_descriptor.clone();
		Mat _scene_descriptor = scene_descriptor.clone();

		/*! Search for a match */
		vector<DMatch> matches;
		searchMatch(_model_descriptor, _scene_descriptor, matches);

		/*! Quick calculation of max and min distances between keypoints */
		double max_dist = 0; double min_dist = 100;
		for (int i = 0; i < _model_descriptor.rows; i++)
		{
			double dist = matches[i].distance;
			if (dist < min_dist) min_dist = dist;
			if (dist > max_dist) max_dist = dist;
		}
			//printf("-- Max dist : %f \n", max_dist);
			//printf("-- Min dist : %f \n", min_dist);

			vector< DMatch > _good_matches;
			/*! Retrive only "good" matches (i.e. whose distance is less than 2*min_dist */
			for (int i = 0; i < _model_descriptor.rows; i++)
				if (matches[i].distance <= max(2 * min_dist, 0.1))
					_good_matches.push_back(matches[i]);

			/*! Retrive only good matches */
			good_matches = _good_matches;		
			/*! Clean after you */
			_good_matches.clear();
			_model_descriptor.release();
			_scene_descriptor.release();
			matches.clear();
		}			
}

/* Search for a match(model-scene) using Brute Force Match */
void m_FLANN::searchMatch(const Mat& model_descriptor, const Mat& scene_descriptor, vector<DMatch>& matches)
{
	/*Testing timer */
	clock_t t0, t1;

	Mat _model_descriptor = model_descriptor.clone();
	Mat _scene_descriptor = scene_descriptor.clone();
	/*! Contruct BruteForce MAtcher */
	BFMatcher matcher(NORM_L2);
	/*! Find a match */
	matcher.match(_model_descriptor, _scene_descriptor, matches);
}

/* Search for a FlannBasedMatcher, this method is not used, 
  only for the learning process */
void searchBasedMatch(const Mat& model_descriptor, const Mat& scene_descriptor, vector<DMatch>& matches)
{	
	/*Testing timer */
	clock_t t0, t1;
	/*! Construct the KDTreeIndexParams */
	const cv::Ptr<cv::flann::IndexParams>& indexParams = new cv::flann::KDTreeIndexParams(4);
	/*! Define the search paramater */
	const cv::Ptr<cv::flann::SearchParams>& searchParams = new cv::flann::SearchParams();
	/*! Construct */
	FlannBasedMatcher matcher;						//(indexParams, searchParams);
	/*! Train the model object */
	matcher.add(model_descriptor);
	matcher.train();
	t0 = clock();
	/*! Search for a match */
	matcher.match(model_descriptor, scene_descriptor, matches);	
	t1 = clock();
	std::cout << "\nComputation Time to knnMatch" << static_cast<double>(t1 - t0) / CLOCKS_PER_SEC << " m/s" << endl;
	/*! Clear the train descriptor collection */
	matcher.clear();
}

