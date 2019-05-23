//#include "pch.h"

#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>

using namespace cv;
using namespace std;

Mat reference = imread("van_gogh.jpg");
Mat image = reference.clone();
Mat temp = image.clone();
String selectedTool = "None";

//Brightness tool
Mat brightness_dst;
int beta = 50;
int alpha = 50;
int const max_beta = 100;
int const max_alpha = 100;

//ErosionTool
Mat ErosionDilation_dst;
int erosiondilation = 0;
int erosiondilation_size = 0;
int erosiondilation_elem = 0;
int const max_elem = 2;
int const max_size = 21;

//Variable for Canny edge detection tool

Mat src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int edge_ratio = 3;
int kernel_size = 3;

String window_name = "Edge Map";


//Variables for crop tool

bool leftDown=false,leftup=false;
Point cor1,cor2;
Rect box;

//Variables for rotate tool

Mat src,  rotate_dst, M;
int phi= 180;
int xsize = 10;
int const max_phi = 360;
int const max_xsize = 100;

void mouse_callback(int event, int x, int y, int flags, void* param) {

	if (event == 1)
	{
		//cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == 2)
	{
		//cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == 3)
	{
		//cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == 0)
	{
		//cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;

	}

}

void help() {

	cout << "Help:" << endl;
	cout << "ESC: Exit" << endl;
	cout << "B: Brightness Tool" << endl;
	cout << "C: Clear modifications" << endl;
	cout << "E: Erosion Tool" << endl;
	cout << "F: Canny Edge Detection Tool" << endl;
	cout << "R: Resize Tool" << endl;
	cout << "T: crop Tool" << endl;
	cout << "O: rotate Tool" << endl;

}

void BrightnessContrast(int, void*)
{
	float alpha2 = (float)alpha / 50;
	float beta2 = beta - 50;
	Mat element = Mat::zeros(image.size(), image.type());
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			for (int c = 0; c < image.channels(); c++) {
				element.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha2*image.at<Vec3b>(y, x)[c] + beta2);
			}
		}
	}
	brightness_dst = element;
	imshow("Ajust Brightness and Contrast", brightness_dst);


}

void brightness_tool() {

	alpha = 50;
	beta = 50;

	String windowName = "Ajust Brightness and Contrast";
	namedWindow(windowName, WINDOW_AUTOSIZE);

	moveWindow("Ajust Brightness and Contrast", image.cols, 0);
	createTrackbar("Brightness :\n ", "Ajust Brightness and Contrast",
		&beta, max_beta,
		BrightnessContrast);
	createTrackbar("contrast :\n ", "Ajust Brightness and Contrast",
		&alpha, max_alpha,
		BrightnessContrast);

	BrightnessContrast(0, 0);

	int k = 0;
	while (k != 27) {

		//2490368 = Haut
		//2424832 = Gauche
		//2621440 = Bas
		//2555904 = Droite

		if ( CV_MAJOR_VERSION < 3)
	  	{
	      // Old OpenCV 2 code goes here. 
	  		k = waitKey(20);

	  	} 
	  	else
	  	{
	      // New OpenCV 3 code goes here. 
	  		//k = waitKeyEx(20);
	  	}

		

		if (k == 27 || k == 1048603) { //ESC
			break;
		}

		if (k == 115 || k == 1048691) { //S
			brightness_dst.copyTo(image);
			break;
		}

		
		
	}

	cout << "Back to Menu" << endl;
	destroyWindow(windowName);
	help();
	
}

void ErosionDilation(int, void*)
{
	int erosiondilation_type = 0;
	if (erosiondilation_elem == 0) { erosiondilation_type = MORPH_ELLIPSE; }
	else if (erosiondilation_elem == 1) { erosiondilation_type = MORPH_RECT; }
	else if (erosiondilation_elem == 2) { erosiondilation_type = MORPH_CROSS; }

	if (erosiondilation == 0) {
		Mat element = getStructuringElement(erosiondilation_type,
			Size(2 * erosiondilation_size + 1, 2 * erosiondilation_size + 1),
			Point(erosiondilation_size, erosiondilation_size));
		erode(image, ErosionDilation_dst, element);
	}
	else if (erosiondilation == 1) {
		Mat element2 = getStructuringElement(erosiondilation_type,
			Size(2 * erosiondilation_size + 1, 2 * erosiondilation_size + 1),
			Point(erosiondilation_size, erosiondilation_size));
		dilate(image, ErosionDilation_dst, element2);

	}
	imshow("Erosion/Dilation", ErosionDilation_dst);
}

void erosion_tool() {

	String windowName = "Erosion/Dilation";

	namedWindow(windowName, WINDOW_AUTOSIZE);
	moveWindow(windowName, image.cols, 0);
	createTrackbar("erosion or dilation:\n 0: dilation \n 1: erosion ", windowName,
		&erosiondilation, 1,
		ErosionDilation);
	createTrackbar("shape type:\n 0: Rect \n 1: Cross \n 2: Ellipse", windowName,
		&erosiondilation_elem, max_elem,
		ErosionDilation);
	createTrackbar("size:\n 2n +1", windowName,
		&erosiondilation_size, max_size,
		ErosionDilation);
	ErosionDilation(0, 0);

	int k = 0;
	while (k != 27) {

		//2490368 = Haut
		//2424832 = Gauche
		//2621440 = Bas
		//2555904 = Droite

		if ( CV_MAJOR_VERSION < 3)
	  	{
	      // Old OpenCV 2 code goes here. 
	  		k = waitKey(20);

	  	} 
	  	else
	  	{
	      // New OpenCV 3 code goes here. 
	  		//k = waitKeyEx(20);
	  	}


		if (k == 27 || k == 1048603) { //ESC
			break;
		}

		if (k == 115 || k == 1048691) { //S
			ErosionDilation_dst.copyTo(image);
			break;
		}



	}

	cout << "Back to Menu" << endl;
	destroyWindow(windowName);
	help();
}


void RotateAngle( int, void* )
{
   Point2f center(src.cols/2, src.rows/2);
   double rotationAngle=phi-180;
   double scale=(float)xsize/10;

   Point2f pc(src.cols/2., src.rows/2.);
   M = getRotationMatrix2D(pc, rotationAngle, scale);

   warpAffine(src, rotate_dst, M, src.size());

   imshow( "Ajust rotation angle", rotate_dst );


}


void RotateAngle_main()
{
  
  src = image;
  
  namedWindow( "Ajust rotation angle", WINDOW_AUTOSIZE );
  moveWindow( "Ajust rotation angle", src.cols, 0 );
  createTrackbar( "Ajust angle :\n ", "Ajust rotation angle",
          &phi, max_phi,
          RotateAngle );
  createTrackbar( "Ajust crop factor :\n ", "Ajust rotation angle",
          &xsize, max_xsize,
          RotateAngle );

  RotateAngle( 0, 0 );
  waitKey(0);

}

void CannyThreshold(int, void*)
{
	/// Reduce noise with a kernel 3x3
	blur(src_gray, detected_edges, Size(3, 3));

	/// Canny detector
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*edge_ratio, kernel_size);

	/// Using Canny's output as a mask, we display our result
	dst = Scalar::all(0);

	image.copyTo(dst, detected_edges);
	imshow(window_name, dst);
}

void edgeDetection() {

	int sKey = 1048691;
	int escapeKey = 1048603;
	int k = 0;

	while (k != 27) { //while the escape button is not hit 

		dst.create(image.size(), image.type());

		/// Convert the image to grayscale
		cvtColor(image, src_gray, COLOR_BGR2GRAY);

		/// Create a window
		namedWindow(window_name, WINDOW_AUTOSIZE);

		/// Create a Trackbar for user to enter threshold
		createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

		/// Show the image
		CannyThreshold(0, 0);


		if ( CV_MAJOR_VERSION < 3)
	  	{
	      // Old OpenCV 2 code goes here. 
	  		k = waitKey(20);

	  	} 
	  	else
	  	{
	      // New OpenCV 3 code goes here. 
	  		//k = waitKeyEx(20);
	  	}

		if (k == 27 || k == escapeKey) { //ESC
			break;
		}

	}

	cout << "Back to Menu" << endl;
	destroyWindow(window_name);
	help();

}

void resize_tool() {

	double addX = 0;
	double addY = 0;
	int change = 1;

	int k = 0;
	while (k != 27) {

		Mat aff = image.clone();
		
		/*
		String windowName = "Resize Tool";
		namedWindow(windowName);
		imshow(windowName, aff);
		*/

		if ( CV_MAJOR_VERSION < 3)
	  	{
	      // Old OpenCV 2 code goes here. 
	  		k = waitKey(20);

	  	} 
	  	else
	  	{
	      // New OpenCV 3 code goes here. 
	  		//k = waitKeyEx(20);
	  	}

		
		if (k == 2490368 || k == 1113938) { //fleche haut
			addY += 0.05;
			change = 1;
		}

		if (k == 2621440 || k == 1113940) { //fleche bas 
			addY -= 0.05;
			change = 1;
		}

		if (k == 2555904 || k == 1113939) { // fleche droite
			addX += 0.05;
			change = 1;
		}

		if (k == 2424832 || k == 1113937) { //fleche gauche 
			addX -= 0.05;
			change = 1;
		}

		if (k == 27 || k == 1048603) { //ESC
			break;
		}

		

		if(change){
			cout << addX << " / " << addY << endl;
			change = 0;
		}
		
		resize(image,aff,Size(),1+addX,1+addY);
		String windowName = "Resize Tool";
		namedWindow(windowName, WINDOW_AUTOSIZE);
		imshow(windowName,aff);

		if (k == 115 || k == 1048691) { //S
			aff.copyTo(image);
			destroyWindow(windowName);
			//destroyWindow(windowName2);
			break;
		}
		
	}

	cout << "Back to Menu" << endl;
	
	help();
}


void mouse_call(int event,int x,int y,int,void*)
{
	if(event==EVENT_LBUTTONDOWN)
	{
		leftDown=true;
		cor1.x=x;
		cor1.y=y;

	 
	}
	if(event==EVENT_LBUTTONUP)
	{

		leftup=true;
		cor2.x=x;
		cor2.y=y;

	}
 
	if(leftDown==true&&leftup==false) //when the left button is down
	{
		Point pt;
		pt.x=x;
		pt.y=y;
		Mat temp_img=image.clone();
		rectangle(temp_img,cor1,pt,Scalar(0,255,0)); //drawing a rectangle continuously
		imshow("Original",temp_img);
	 
	}
	if(leftDown==true&&leftup==true) //when the selection is done
	{
	 
		box.width=abs(cor1.x-cor2.x);
		box.height=abs(cor1.y-cor2.y);
		box.x=min(cor1.x,cor2.x);
		box.y=min(cor1.y,cor2.y);
		Mat crop(image,box); //Selecting a ROI(region of interest) from the original pic
		namedWindow("Cropped Image");
		imshow("Cropped Image",crop); //showing the cropped image
		leftDown=false;
		leftup=false;
	 
	}
 
}


void crop_tool()
{

	namedWindow("Original");
	imshow("Original",image);

	int k =0;

	while(k !=27 || k!= 1048603)
        {

        if ( CV_MAJOR_VERSION < 3)
	  	{
	      // Old OpenCV 2 code goes here. 
	  		k = waitKey(20);

	  	} 
	  	else
	  	{
	      // New OpenCV 3 code goes here. 
	  		//k = waitKeyEx(20);
	  	}

  	    namedWindow("Original");
	    imshow("Original",image);
  	    putText(image,"Choose corner, and drag, Press ESC to exit and S to save" ,Point(10,30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255,255,255), 2 );

	setMouseCallback("Original",mouse_call); //setting the mouse callback for selecting the region with mouse
	 
	}
	
}


int cropAndResize_students() {

	Mat scaleD, scaleU;

	double scaleX = 0.6;
	double scaleY = 0.6;

	// Scaling down the image 0.6 times
	resize(image, scaleD, scaleD.size(), scaleX, scaleY, INTER_LINEAR);

	// Scaling up the image 1.8 times
	resize(image, scaleU, scaleU.size(), scaleX * 2, scaleY * 2, INTER_LINEAR);

	Mat crop = image;
	Range(crop.rows / 2, crop.cols / 2);

	String windowName = "Original";
	String windowName2 = "x0.6";
	String windowName3 = "x1.8";
	String windowName4 = "Cropped";

	namedWindow(windowName);
	namedWindow(windowName2);
	namedWindow(windowName3);
	namedWindow(windowName4);

	imshow(windowName, image);
	imshow(windowName2, scaleD);
	imshow(windowName3, scaleU);
	imshow(windowName4, crop);

	waitKey(0);

	destroyWindow(windowName);
	destroyWindow(windowName2);
	destroyWindow(windowName3);
	destroyWindow(windowName4);

	return 0;

}

void clear() {

	reference.copyTo(image);
}

int main(int argc, char** argv)
{

	help();

	if (reference.empty())
	{
		cout << "Could not open or find the image" << endl;
		system("pause");
		return -1;
	}

	String windowName = "Original";
	namedWindow(windowName, WINDOW_AUTOSIZE);

	setMouseCallback(windowName, mouse_callback, 0);

	int k = 0;

	while (k != 27) {

		imshow(windowName, image);
		//putText(temp, "ESC to exit / c to clear", Point(10, 30), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255), 2);
		

		if ( CV_MAJOR_VERSION < 3)
	  	{
	      // Old OpenCV 2 code goes here.
	      	
	  		k = waitKey(20); 

	  	} 
	  	else
	  	{
	      // New OpenCV 3 code goes here. 
	  		//k = waitKeyEx(20);
	  	}


		


	  	if (k == 116 || k == 1048692 ){ //T

			cout << "Tool: Crop selected" << endl;
			crop_tool();

		}
		

		if (k == 99 || k == 1048675) { //C
			cout << "Image cleared" << endl;
			clear();
		}

		if (k == 98 || k == 1048674) { //B
			cout << "Tool: Brightness selected" << endl;
			brightness_tool();
		}

		if (k == 114 || k == 1048690) { //R
			cout << "Tool: Resize selected" << endl;
			resize_tool();
		}

		if (k == 101 || k == 1048677) { //E
			cout << "Tool: Erosion selected" << endl;
			erosion_tool();
		}

		if (k == 102 || k == 1048678) { //F 
			cout << "Tool: Edge Detection selected" << endl;
			edgeDetection();
		}

		if (k == 111 || k == 1048687) { //O
			cout << "Tool: Rotate selected" << endl;
			RotateAngle_main();
		}
	
		
	}

	cout << "Finish" << endl;
}