#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

 int main( int argc, char** argv )
 {
    VideoCapture cap(0); //capture the video from web cam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

  int lB = 0;
 int hB = 255;

  int lG = 0; 
 int  hG = 255;

  int lR = 0;
 int hR = 255;

  //Create trackbars in "Control" window
 cvCreateTrackbar("LowB", "Control", &lB, 255); //Hue (0 - 179)
 cvCreateTrackbar("HighB", "Control", &hB, 255);

  cvCreateTrackbar("LowG", "Control", &lG, 255); //Saturation (0 - 255)
 cvCreateTrackbar("HighG", "Control", &hG, 255);

  cvCreateTrackbar("LowR", "Control", &lR, 255); //Value (0 - 255)
 cvCreateTrackbar("HighR", "Control", &hR, 255);

    while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }
        Mat imgThresholded;

    inRange(imgOriginal, Scalar(lB, lG, lR), Scalar(hB, hG, hR), imgThresholded); //Threshold the image
      
    GaussianBlur(imgThresholded, imgThresholded, Size(7, 7), 0);   
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3))); 
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));         
   imshow("Thresholded Image", imgThresholded); //show the thresholded image
   imshow("Original", imgOriginal); //show the original image

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break; 
       }
    }

   return 0;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////