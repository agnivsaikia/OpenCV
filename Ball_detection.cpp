#include<opencv2/opencv.hpp>
#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

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

   		HoughCircles(tVid,v3fCircles,CV_HOUGH_GRADIENT,2,tVid.rows / 4,100,50,10,800);    

     	 for (int i = 0; i < v3fCircles.size(); i++) 
     	 {  if((v3fCircles[i][2]>=15)&&(v3fCircles[i][2]<=30))
     	 	{               
   				cout << "Ball position X = "<< v3fCircles[i][0]<<",\tY = "<< v3fCircles[i][1]<<",\tRadius = "<< v3fCircles[i][2]<< "\n";     // radius of circle
		  		circle(OrgVid,Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]),3,Scalar(0, 255, 0),CV_FILLED);
				//circle(OrgVid,Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]),(int)v3fCircles[i][2],Scalar(0, 0, 255),3); 
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