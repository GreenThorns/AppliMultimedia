
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

/*This program shows how highgui enables us to take mouse inputs.
In this code we use mouse input to draw a circle on an image.
The mouse is dragged from the center to one of the points on the
circumference. 'c' can be pressed to remove the drawn circles. */

// Points to store the center of the circle and a point on the
//circumference

using namespace cv;
using namespace std;
 
bool leftDown=false,leftup=false;
Mat img=imread("mark.jpg");
Point cor1,cor2;
Rect box;
 
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
		Mat temp_img=img.clone();
		rectangle(temp_img,cor1,pt,Scalar(0,255,0)); //drawing a rectangle continuously
		imshow("Original",temp_img);
	 
	}
	if(leftDown==true&&leftup==true) //when the selection is done
	{
	 
		box.width=abs(cor1.x-cor2.x);
		box.height=abs(cor1.y-cor2.y);
		box.x=min(cor1.x,cor2.x);
		box.y=min(cor1.y,cor2.y);
		Mat crop(img,box); //Selecting a ROI(region of interest) from the original pic
		namedWindow("Cropped Image");
		imshow("Cropped Image",crop); //showing the cropped image
		leftDown=false;
		leftup=false;
	 
	}
 
}
 
int main()
{

	namedWindow("Original");
	imshow("Original",img);
	while(char(waitKey(1)!=27))
        {
  	    namedWindow("Original");
	    imshow("Original",img);;
  	    putText(img,"Choose corner, and drag, Press ESC to exit and S to save" ,Point(10,30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255,255,255), 2 );

	setMouseCallback("Original",mouse_call); //setting the mouse callback for selecting the region with mouse
	 
	}
	return 0;
}
