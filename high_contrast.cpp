#include<opencv2/opencv.hpp>
#include<iostream>

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

	string wname ="My Camera Feed";
	namedWindow(wname);

	string cname="High contrast feed";
	namedWindow(cname);

	while(true)
	{
		Mat frame;
		bool bSuccess = cap.read(frame);	
		if(bSuccess==false)
		{
			cout<<"Video Camera was disconnected"<<endl;
			cin.get();
			break;
		}	

		Mat highcontrast;

		frame.convertTo(highcontrast,-1,2,0);

		imshow(cname,highcontrast);
		imshow(wname, frame);

		if(waitKey(10)==27)
		{
			cout<<"Exit"<<endl;
			break;
		}
	}
	return 0;
}