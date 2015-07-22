#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{ 
	Mat img = imread( "C:\\Users\\acer1\\Pictures\\pr1.png"); // Открытие файла
	imshow("Original", img);
	Mat yellow1, yellow = Mat(img.size(), CV_8UC3);
    for( int i = 0; i < img.rows; i++ )
		for( int j = 0; j < img.cols; j++ )
			if ( img.at<Vec3b>(i,j)[2] > 254 && 
					img.at<Vec3b>(i,j)[1] > 241 &&
					img.at<Vec3b>(i,j)[0] < 64 )
			{
				yellow.at<Vec3b>(i,j)[0] = 255;
				yellow.at<Vec3b>(i,j)[1] = 255;
				yellow.at<Vec3b>(i,j)[2] = 255;
            }
			else
			{
				yellow.at<Vec3b>(i,j)[0] = 0;
				yellow.at<Vec3b>(i,j)[1] = 0;
				yellow.at<Vec3b>(i,j)[2] = 0;
			}
	int thresh = 100;
	RNG rng(12345);
	cvtColor( yellow, yellow1, CV_BGR2GRAY );
	blur( yellow1, yellow1, Size(3,3) );
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	threshold( yellow1, threshold_output, 136, 255, THRESH_BINARY );
	findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );
	for( int i = 0; i < contours.size(); i++ )
	{ 
		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
		boundRect[i] = boundingRect( Mat(contours_poly[i]) );
		minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
	}
	for( int i = 0; i< contours.size(); i++ )
	{
		//Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		Scalar color = Scalar(255, 0, 0);
		drawContours( img, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
		rectangle( img, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
		circle( img, center[i], (int)radius[i], color, 2, 8, 0 );
	}
	imwrite("C:\\Users\\acer1\\Pictures\\pr2.png", img );
	imshow("New", img);
	imshow("yellow", yellow);
	waitKey();
	return 0;
} 
