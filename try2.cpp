
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int ddepth = CV_16S;
double e = 0;
int ct = 300;
int x = 0;
double ar = 0;
int erosion_type = MORPH_ELLIPSE;
int erosion_size = 18;
int dilation_size = 18;	
Mat frame, hsv, mask, gauss, erosion_dst, dilation_dst;
void Erosion( int, void* );
void Dilation( int, void* );

int main( int argc, char** argv )
{
	VideoCapture cap(0);
	namedWindow("Video",1);
	while(1)
	{
		cap >> frame;
		//imshow("Video", frame);
		cvtColor(frame, hsv, CV_BGR2HSV);
		//imshow("HSV", hsv);
		inRange(hsv, Scalar(20,50,10), Scalar(40,255,255), mask);
		//imshow("Range", mask);
		GaussianBlur(mask, gauss, Size(9,9), BORDER_DEFAULT);
		//cvtColor(gauss, gray, CV_RGB2GRAY);
		//dilate(erode, dilate, element);
		Erosion( 0, 0 );
  		Dilation( 0, 0 );
		vector<vector<Point> > contours;
		vector<Point> approx;
		vector<Vec4i> hierarchy;
		findContours(dilation_dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0,0));
		vector<Rect> boundRect(contours.size());
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
     	//Mat drawing = Mat::zeros( canny.size(), CV_8UC3 );
		for(int i=0; i<contours.size(); i++)
		{
			//drawContours(frame, contours, i, Scalar(0,0,255), 2, 8, hierarchy, 0, Point() );
			//boundRect[i] = boundingRect(contours[i]);
			minEnclosingCircle( contours[i], center[i], radius[i] );
			//if(boundRect[i].width < boundRect[i].height) x = boundRect[i].width;
			//else x = boundRect[i].height;
			//cout<<x;
			e = 0.01*arcLength(contours[i], true);
			approxPolyDP( Mat(contours[i]), approx, e, true );
			double area = contourArea(approx);
			ar = 3.14*radius[i]*radius[i];
			cout<<ar<<"   "<<area<<endl;
			if((area/ar)>0.7&&area>500)
			{
				//vector<Moments> mu(contours.size());
				circle(frame,mc[i],radius[i],Scalar(250,0,0),2,10);
				//drawContours( frame, , i, Scalar(0,0,255), 2, 8, hierarchy, 0, Point() );
				//imshow("Video1", frame);
			}
		}
		//double area = contourArea(contours[1]);
		//
		//double area = contourArea(approx);
		//cout<<area<<endl;
		namedWindow( "Contours", WINDOW_AUTOSIZE );
  		imshow( "Contours", frame );
		if(waitKey(30)=='q') break;
	}
	return 0;
}
void Erosion( int, void* )
{
  int erosion_type = MORPH_RECT;

  Mat element = getStructuringElement( erosion_type,
                       Size( 2*erosion_size , 2*erosion_size));

  /// Apply the erosion operation
  erode( mask, erosion_dst, element );
  imshow( "Erosion Demo", erosion_dst );
}

/** @function Dilation */
void Dilation( int, void* )
{
  int dilation_type = MORPH_RECT; 

  Mat element = getStructuringElement( dilation_type,
                       Size( 2*dilation_size + 1, 2*dilation_size+1 ));
  /// Apply the dilation operation
  dilate( erosion_dst, dilation_dst, element );
  imshow( "Dilation Demo", dilation_dst );
}