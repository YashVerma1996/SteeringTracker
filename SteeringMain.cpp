#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Marker.h"
#include "Constants.h"

using namespace cv ;
using namespace std;

const std::string windowName = "SteeringTracker" ;
Mat cameraFeed, HSV ;
Marker marker1, marker2 ;

double getSteeringTiltAngle(Point p1, Point p2){
	double slope = double(p2.y-p1.y)/(p2.x-p1.x) ;
	return atan(slope) * 180/PI ;
}

void clickAndDrag_Rectangle(int event, int x, int y, int flags, void* param){
	if(! marker1.isCalibrated()){
		marker1.calibrate(event, x, y, (Mat*)param, HSV) ;
	}
	else if(! marker2.isCalibrated()){
		marker2.calibrate(event, x, y, (Mat*)param, HSV) ;
	}
}

int main(){
	
	VideoCapture capture(0) ;
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);

	namedWindow(windowName);
	setMouseCallback(windowName, clickAndDrag_Rectangle, &cameraFeed);

	double tiltAngle ;
	while(1){
		
		capture.read(cameraFeed);
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);

		//Calibration will be performed in clickAndDrag function
		marker1.performTrackingOperations(HSV, cameraFeed);
		marker2.performTrackingOperations(HSV, cameraFeed);		


		if(marker1.isCalibrated() && marker2.isCalibrated()){
			
			Point p1 = marker1.getCoordinates() ;
			Point p2 = marker2.getCoordinates() ;

			line(cameraFeed,p1,p2,Scalar(255,0,0),2);
			
			tiltAngle = getSteeringTiltAngle(p1, p2) ;
			cout<<"TiltAngle = "<<tiltAngle<<"\n"; 
			if(tiltAngle<(-20.0)){	
				putText(cameraFeed, "Turn Right", Point(5,50), 1, 1, Scalar(255,0,0), 2);
				// system("xdotool type 'right\n'");
			}
			else if(tiltAngle>20.0){
				putText(cameraFeed, "Turn Left", Point(5,50), 1, 1, Scalar(255,0,0), 2);
				// system("xdotool type 'left\n'");
			}
		}
			
		imshow(windowName, cameraFeed);	

		switch(waitKey(30)){
			case 27 : 
				return 0 ;
			case 'c' : 
					marker1.setCalibrationMode(true);
					marker1.setCalibrationMode(true);
					cout<<"Please calibrate the markers." ; 
				break; 	
		}

	}


}