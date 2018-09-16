#include<opencv2/opencv.hpp>
#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv; 
using namespace std;

int main(int argc, char const *argv[])
{
	VideoCapture cap(1);//Open dfault camera

	if(cap.isOpened()==false)
	{
		cout<<"Cannot open camera"<<endl;
		cin.get();
		return -1;
	}

	Mat OrgVid;  
 	Mat hsvVid;  
 	Mat tVid;   
 	Mat contours;

 	int lowH = 19;//0;//22;//28;       
 	int highH = 58;//50;//40;//36;//42;

 	int lowS = 23;//49;//136;//68;//23;       
 	int highS = 255;//228;//235;//130;

 	int lowV = 0;//9;//77;//145;      
 	int highV = 255;//100;//90;//255;

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

		GaussianBlur(tVid, tVid, Size(7,7), 0);     
  		erode(tVid, tVid, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));      
  		dilate( tVid, tVid, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

  		vector<vector<Point> > contours;
		vector<Point> approx;
		vector<Vec4i> hierarchy;
		findContours(tVid, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0,0));
		vector<Moments> mu(contours.size() );
		vector<Point2f>center( contours.size() );
  		vector<float>radius( contours.size() );
  		for( int i = 0; i < contours.size(); i++ )
     	{ 
     		mu[i] = moments( contours[i], false ); 
     	}
		vector<Point2f> mc(contours.size());
  		for(int i = 0; i<contours.size(); i++)
     	{ 
     		mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
     	}
		for(int i=0; i<contours.size(); i++)
		{
			minEnclosingCircle( contours[i], center[i], radius[i] );
			double e = 0.01*arcLength(contours[i], true);
			approxPolyDP( Mat(contours[i]), approx, e, true );
			double area = contourArea(approx);
			double ar = 3.14*radius[i]*radius[i];
			if((area/ar)>0.7&&area>500)
			{
				circle(OrgVid,mc[i],radius[i],Scalar(250,0,0),2,5);
			}
		}
		imshow("Original", OrgVid);
		imshow("Thresh",tVid);

		if(waitKey(10)==27)
		{
			cout<<"Exit"<<endl;
			break;
		}
	}
	return 0;
}