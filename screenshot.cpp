#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
	VideoCapture cap(0);

	if(cap.isOpened()==false)
	{
		cout<<"Cannot open camera"<<endl;
		cin.get();
		return -1;
	}

	string wname="Window name";
	namedWindow(wname);

	while(true)
	{
		Mat frame;
		bool bsuccess = cap.read(frame);
		if(bsuccess==false)
		{
			cout<<"Video was diconnected"<<endl;
			cin.get();
			break;
		}

		cvtColor(frame,frame, COLOR_BGR2GRAY);
		imshow(wname,frame);

		if(waitKey(10)==32)
		{
			imwrite("/home/agniv/opencv_agniv/Screenshot.jpg",frame);
		}

		if(waitKey(10)==27)
		{
			cout<<"Exit"<<endl;
			break;
		}

	}

	return 0;
}