/**
 * Controls the flow of the program. Execution starts from this file.
 *
 * @author vermayash8
 */
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "marker.h"
#include "constants.h"
#include "car_controller.h"

using namespace cv ;
using namespace std;

const string videoWindowName = "SteeringTracker" ;
Mat cameraFeed, HSV ;
// Markers that will be used to track steering controls.
vector<Marker> markers(4);

/**
 * Mirrors the camera feed taken from the webcam.
 * After mirroring the feed becomes more relatable to as seen
 * in the mirror which makes calibrating the markers easy.
 */
void mirrorCameraFeed() {
	int cols = FRAME_WIDTH ;
	int rows = FRAME_HEIGHT ;
	Vec3b temp ;
	for (int i = 0 ; i < FRAME_HEIGHT ; i++)
		for (int j = 0 ; j <= FRAME_WIDTH / 2 ; j++) {
			temp = cameraFeed.at<Vec3b>(i, j) ;
			cameraFeed.at<Vec3b>(i, j) = cameraFeed.at<Vec3b>(i, FRAME_WIDTH - j) ;
			cameraFeed.at<Vec3b>(i, FRAME_WIDTH - j) = temp ;
		}
}

/**
 * Calibrates the markers in order when rectangular region of interest
 * is selected on screen.
 * 
 * @param event the event caused.
 * @param x x coordinate of the point of event.
 * @param y y coordinate of the point of event.
 * @param param pointer to the Mat.
 */
void clickAndDrag_Rectangle(int event, int x, int y, int flags, void* param) {
	for(auto marker : markers) {
		if(!marker.isCalibrated()) {
			marker.calibrate(event, x, y, (Mat*) param, HSV);
			break;
		}
	}
}

/**
 * Execution starts from here.
 */
int main() {
	// Get default VideoCapture device i.e. webcam.
	VideoCapture capture(0) ;
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

	// initialize the window for showing video to the user.
	namedWindow(videoWindowName);

	// set mouse callback to allow handling of marker calibration and selection.
	setMouseCallback(videoWindowName, clickAndDrag_Rectangle, &cameraFeed);

	while (1) {
		capture.read(cameraFeed);
		mirrorCameraFeed() ;
		// convert to HSV color system from rgb.
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);

		// Calibration will be performed in clickAndDrag function.
		bool allMarkersCalibrated = true;
		for (auto marker : markers) {
			// Trigger tracking operations for every marker.
			marker.performTrackingOperations(HSV, cameraFeed);
			if (!marker.isCalibrated()) {
				allMarkersCalibrated = false;
			}
		}
		// start controlling the game if all markers have been calibrated.
		if (allMarkersCalibrated) {
			moveTheCar(markers, cameraFeed);
		}
		// display video and results in output window.
		imshow(videoWindowName, cameraFeed);
		// wait for 30ms window and listen to keyevents.
		switch (waitKey(30)) {
		case 27 : // Escape key to end the program.
			return 0 ;
		case 'c' :
			// Removes the previous calibration and allows the markers to be calibrated again.
			for (auto marker : markers) {
				marker.setCalibrationMode(true);
			}
			cout << "Please calibrate the markers." ;
			break;
		}
	}
}
