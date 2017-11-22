
/*
	Run/Main.cpp
	[Author]: Claudiu Furca
	[Copyright 2016]: (c) Amprent.	All rights reseved.
*/

#include "HeaderFiles.h"

namespace
{
	/*! Model image path - used for creating a model mask */
	const char* ccModel = "Resources/ccModel_3x.jpg";
	/* Other resources used for testing */
	const char* imgTestCC = "Resources/imgTestCC.jpg";
	const char* testMultiCCImg_org = "Resources/testMultiCCImg_org.jpg";
	const char* multiCCExperiment = "Resources/multiCCExperiment_0.5m.jpg";
	const char* multiCCExperiment_1m = "Resources/multiCCExperiment_1m.jpg";
	const char* multiCCExperiment_2m = "Resources/multiCCExperiment_2m.jpg";
	const char* detectImg = "Resources/detectShapeImg.png";
}

/* MAIN RUN */
int main()
{
	clock_t t0, t1;
	const int x = 0;
	const int y = 0;
	Mat img_object = imread(ccModel, CV_LOAD_IMAGE_GRAYSCALE);	
	//Mat img_scene = imread(testMultiCCImg_org, LAYER_TYPE_BGR2GRAY);
	Mat img_scene = imread(multiCCExperiment, LAYER_TYPE_BGR2GRAY);
	//Mat img_scene = imread(multiCCExperiment_1m, CV_LOAD_IMAGE_COLOR);
	/*! Copy the  Image scene to Matrix */
	Mat _img_scene = img_scene.clone();

	if (_img_scene.data)
	{
		/*! Compute image scene */
		Mat testDrawing;
		CCSquare::getDrawOutput(_img_scene, testDrawing);
	}

	/*! Print matrix */
	//cout << "img_object = " << endl << " " << img_object << endl << endl;
	//imshow("img_object", img_object);

	waitKey();					//TO DELETE IF MOBILE 

	return 0;
}