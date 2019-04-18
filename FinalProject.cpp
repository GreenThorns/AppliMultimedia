
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <string>

using namespace cv;
using namespace std;

Mat reference = imread("van_gogh.jpg");
Mat image = reference.clone();
Mat temp = image.clone();
String selectedTool = "None";

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

void brightness_tool() {

	Mat modified;
	image.copyTo(modified);

	String windowName = "Brightness Tool";
	//namedWindow(windowName, WINDOW_AUTOSIZE);

	int contrast = 1;
	int brightness = 0;

	int k = 0;
	while (k != 27) {

		//code pour OpenCV V4

		//2490368 = Haut
		//2424832 = Gauche
		//2621440 = Bas
		//2555904 = Droite

		//code pour OpenCV V2

		//1113938 = Haut
		//1113937 = Gauche
		//1113940 = Bas
		//1113939 = Droite



		if (contrast < 1) {
			contrast = 1;
		}

		imshow(windowName, modified);
		image.convertTo(modified, -1, contrast, brightness);

		//k = waitKeyEx(20); //V4

		k = waitKey(20); //V2

		/*
		if (k == 2490368) {
			cout << "Contrast+" << endl;
			contrast += 0.5;
		}

		if (k == 2621440) {
			cout << "Contrast-" << endl;
			contrast -= 0.01;
		}

		*/

		if (k == 2555904 || k == 1113939) {
			brightness += 3;
			cout << brightness << endl;
		}

		if (k == 2424832 || k == 1113937) {
			brightness -= 3;
			cout << brightness << endl;
		}

		if (k == 27 || k == 1048603 ) { //ESC
			break;
		}

		if (k == 115) { //S
			modified.copyTo(image);
			break;
		}

		
		
	}

	cout << "Back to Menu" << endl;
	destroyWindow(windowName);
}

void resize_tool() {

	double addX = 0;
	double addY = 0;

	int k = 0;
	while (k != 27) {

		Mat aff = image.clone();
		String windowName = "Resize Tool";
		namedWindow(windowName);
		imshow(windowName, aff);


		//k = waitKeyEx(0); //V4
		k = waitKey(0);//V2
		
		if (k == 2490368) {
			addY += 0.1;
		}

		if (k == 2621440) {
			addY -= 0.1;
		}

		if (k == 2555904) {
			addX += 0.1;
		}

		if (k == 2424832) {
			addX -= 0.1;
		}

		if (k == 27) { //ESC
			break;
		}

		if (k == 115) { //S
			aff.copyTo(image);
			break;
		}
		destroyWindow(windowName);
		cout << addX << " / " << addY << endl;
		resize(image, aff, aff.size(), 1 + addX, 1 + addY, INTER_LINEAR);
	}

	cout << "Back to Menu" << endl;
	//destroyWindow(windowName);
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
		putText(temp, "ESC to exit / c to clear", Point(10, 30), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255), 2);
		//k = waitKeyEx(20); //V4
		k = waitKey(20); //V2

		//cout << k << endl;
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
	}

	cout << "Finish" << endl;
}