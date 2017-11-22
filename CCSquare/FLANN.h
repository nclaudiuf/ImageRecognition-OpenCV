
/*
	FLANN.h
	[Author]: Claudiu Furca
	[Copyright 2016]: (c) Amprent.	All rights reseved.
*/

/* Declare here all the include files */
#include "HeaderFiles.h"

#ifndef m_FLANN_h
#define m_FLANN_h

/* Declare here all the defines */
/*! Arrange the Flann matches in order; */
/*! KNN doc http://docs.opencv.org/2.4.4/modules/flann/doc/flann_fast_approximate_nearest_neighbor_search.html */
#define KNN 100
#define LAYER_TYPE_CV32F 5

// bring the flann parameters into this namespace
using ::cvflann::get_param;
using ::cvflann::print_params;

// bring the flann distances into this namespace
using ::cvflann::L2_Simple;
using ::cvflann::L2;
using ::cvflann::L1;
using ::cvflann::MinkowskiDistance;
using ::cvflann::MaxDistance;
using ::cvflann::HammingLUT;
using ::cvflann::Hamming;
using ::cvflann::Hamming2;
using ::cvflann::HistIntersectionDistance;
using ::cvflann::HellingerDistance;
using ::cvflann::ChiSquareDistance;
using ::cvflann::KL_Divergence;

/* Class declaration */
class m_FLANN
{
	/* Declare here all the public`s */
public:
	/* Flann Match input image */
	static void m_FLANN::getFlannMatch(const cv::Mat& model_descriptor, const cv::Mat& scene_descriptor,
		std::vector<cv::DMatch>& matche);

private:
	/*! Declare here all the variables declaration */
	static void m_FLANN::searchMatch(const cv::Mat& model_descriptor, const cv::Mat& scene_descriptor,
		std::vector<cv::DMatch>& matches);
	/*! Declare here all the variables declaration */
	const char* doc1_Flann = "http://stackoverflow.com/questions/23635921/lsh-slower-than-bruteforce-matching";
	const char* doc2_Flann = "http://answers.opencv.org/question/10929/how-to-use-opencv244-flann/";
	/*! to add _SCL_SECURE_NO_WARNINGS; & _CRT_SECURE_NO_WARNINGS -> Project -> C++ -> Processor */
	const char* erro4996 = "https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=EN-US&k=k(C4996)&rd=true";
};

#endif /* ! END FLANN_h*/