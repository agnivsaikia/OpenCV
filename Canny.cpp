#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int length(int n)
{
   int len = 0;
   if (n < 0) { len = 1; n = -n; }
   while (n > 9) {
        n /= 10;
        len++;
   }
   return len+1;
}

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
 	Mat edge;
 	Mat tVid;
 	Mat hsvVid;

 	int lowH = 24;       
 	int highH = 35;

 	int lowS = 99;       
 	int highS = 199;

 	int lowV = 50;      
 	int highV = 198;

 	while(true)
 	{
 		bool bSuccess = cap.read(OrgVid);	
		if(bSuccess==false)
		{
			cout<<"Video Camera was disconnected"<<endl;
			cin.get();
			break;
		}	

 		GaussianBlur(OrgVid, OrgVid, Size(5, 5), 0);   
  		dilate(OrgVid, OrgVid, 0);      
  		erode(OrgVid, OrgVid, getStructuringElement(MORPH_ELLIPSE, Size(3, 3))); 

  		cvtColor(OrgVid, hsvVid, CV_BGR2HSV);      

  		inRange(hsvVid, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), tVid);


 		Canny(tVid, edge, 75, 100, 3, false);

 		vector<vector<Point> > contours;
  		 vector<Vec4i> hierarchy;

 		findContours( edge, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

 		vector<vector<Point> > contours_poly( contours.size() );
  		vector<Point2f>center( contours.size() );
  		vector<float>radius( contours.size() );

  		for( int i = 0; i < contours.size(); i++ )
    	 { 
    	   approxPolyDP( Mat(contours[i]),contours_poly[i],0.1*arcLength(contours[i],true), true );
    	   double area = contourArea(contours[i],false);
    	   if(area>30)
    	 {
      		drawContours( OrgVid, contours_poly, i, (0,0,255), 1, 8, vector<Vec4i>(), 0, Point() );
      		circle( OrgVid, center[i], (int)radius[i], (0,0,255), 2, 8, 0 );
     	 }
     	}
 		imshow("Original", OrgVid);
		imshow("Edge",edge);

			if(waitKey(10)==27)
			{
				cout<<"Exit"<<endl;
				break;
			}
	}
	return 0;
}