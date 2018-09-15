#include<opencv2/opencv.hpp>
#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

RNG rng(1);

int main(int argc, char const *argv[])
{
	VideoCapture cap(0);//Open dfault camera

	if(cap.isOpened()==false)
	{
		cout<<"Cannot open camera"<<endl;
		cin.get();
		return -1;
	}

	Mat OrgVid;  
 	Mat hsvVid;  
 	Mat tVid;   
 	Mat org;

 	vector<Vec3f>v3fCircles;

 	int lowH = 25;       
 	int highH = 45;

 	int lowS = 26;       
 	int highS = 139;

 	int lowV = 95;      
 	int highV = 255;

	while(true)
	{
		bool bSuccess = cap.read(OrgVid);	
		if(bSuccess==false)
		{
			cout<<"Video Camera was disconnected"<<endl;
			cin.get();
			break;
		}	

		cvtColor(OrgVid, hsvVid, CV_BGR2HSV);      

  		inRange(hsvVid, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), tVid);

		GaussianBlur(tVid, tVid, Size(5, 5), 0);   
  		dilate(tVid, tVid, 0);      
  		erode(tVid, tVid, getStructuringElement(MORPH_ELLIPSE, Size(3, 3))); 

  		 vector<vector<Point> > contours;
  		 vector<Vec4i> hierarchy;

  		findContours( tVid, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

		/// Approximate contours to polygons + get bounding rects and circles
		vector<vector<Point> > contours_poly( contours.size() );
  		vector<Point2f>center( contours.size() );
  		vector<float>radius( contours.size() );

  		for( int i = 0; i < contours.size(); i++ )
    	 { 
    	   approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
    	   minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
    	 }
    	 Mat drawing = Mat::zeros( tVid.size(), CV_8UC3 );
  		for( int i = 0; i< contours.size(); i++ )
    	 {
      		drawContours( drawing, contours_poly, i, (0,0,255), 1, 8, vector<Vec4i>(), 0, Point() );
      		circle( drawing, center[i], (int)radius[i], (0,0,255), 2, 8, 0 );
     	 }

        imshow("Original", OrgVid);
		imshow("Thresh",tVid);
		imshow("D",drawing);

		if(waitKey(10)==27)
		{
			cout<<"Exit"<<endl;
			break;
		}
	}
	return 0;
}
