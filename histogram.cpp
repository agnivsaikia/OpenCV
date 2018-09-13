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

	string hname = "Histogram equivalent";
	namedWindow(hname);

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

	Mat hist_equalized_image;
    cvtColor(frame, hist_equalized_image, COLOR_BGR2YCrCb);

    //Split the image into 3 channels; Y, Cr and Cb channels respectively and store it in a std::vector
    vector<Mat> vec_channels;
    split(hist_equalized_image, vec_channels); 

    //Equalize the histogram of only the Y channel 
    equalizeHist(vec_channels[0], vec_channels[0]);

    //Merge 3 channels in the vector to form the color image in YCrCB color space.
    merge(vec_channels, hist_equalized_image); 
        
    //Convert the histogram equalized image from YCrCb to BGR color space again
    cvtColor(hist_equalized_image, hist_equalized_image, COLOR_YCrCb2BGR);

    imshow(wname,frame);
    imshow(hname,hist_equalized_image);

		if(waitKey(10)==27)
		{
			cout<<"Exit"<<endl;
			break;
		}
	}
	return 0;
}