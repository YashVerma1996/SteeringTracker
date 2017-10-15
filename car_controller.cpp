/**
 * Provides functions to control the car game on the basis of
 * the marker positions.
 *
 * @author vermayash8
 */
#include "car_controller.h"
#include <math.h>
#include <opencv2/opencv.hpp>
#include "constants.h"
using namespace std;
using namespace cv;

/** Booleans to store current state of controls. */
bool isTurningRight = false ;
bool isTurningLeft = false ;
bool areBrakesApplied = true ;
bool isAccelarating = false ;

/**
 * Returns the steering tilt angle on the basis of location
 * of the left and right markers.
 *
 * @return steering tilt angle.
 */
double getSteeringTiltAngle(Point p1, Point p2) {
    double slope = double(p2.y - p1.y) / (p2.x - p1.x) ;
    return atan(slope) * 180 / PI ;
}

/**
 * Moves the car on the basis of markers' position.
 *
 * @param markers all markers array.
 * @param cameraFeed cameraFeed on which line can be drawn.
 */
void moveTheCar(vector<Marker> &markers, Mat &cameraFeed) {
    // p1 and p2 are points for left and right turn steering controls.
    Point p1 = markers[0].getCoordinates() ;
    Point p2 = markers[1].getCoordinates() ;
    // draw the line joining p1 and p2.
    cv::line(cameraFeed, p1, p2, Scalar(255, 0, 0), 2);
    double tiltAngle = getSteeringTiltAngle(p1, p2);

    /* Handling Up Arrow Key */
    if (markers[2].isVisible() && isAccelarating) {
        cout << "AcceleratorMarker is isVisible = true;Leaving accelerator\n";
        system("xdotool keyup Up");
        isAccelarating = false ;
    }
    else if (!markers[2].isVisible() && !isAccelarating) {
        cout << "AcceleratorMarker is not visible ; PressingAccelerator\n";
        system("xdotool keydown Up");
        isAccelarating = true ;
    }

    /*Handling Down Arrow Key*/
    if (markers[3].isVisible() && areBrakesApplied) {
        cout << "BrakeMarker is isVisible = true;Leaving brakes\n";
        system("xdotool keyup Down");
        areBrakesApplied = false ;
    }
    else if (!markers[3].isVisible() && !areBrakesApplied) {
        cout << "BrakeMarker is not visible ; Applying Brakes\n";
        system("xdotool keydown Down");
        areBrakesApplied = true ;
    }

    /*Handling right turn event*/
    if (tiltAngle < (-MIN_TILT_ANGLE)) {
        cv::putText(cameraFeed, "Turn Right", cv::Point(5, 50), 1, 1, Scalar(255, 0, 0), 2);
        if (!isTurningRight) {
            //Either it is moving straight or turning left, so stop it from turning left and start turning right.
            system("xdotool keyup Left keydown Right");
            isTurningLeft = false ;
            isTurningRight = true ;
        }
    }
    else if (tiltAngle > MIN_TILT_ANGLE) {
        putText(cameraFeed, "Turn Left", Point(5, 50), 1, 1, Scalar(255, 0, 0), 2);
        if (!isTurningLeft) {
            //Either it is moving straight or turning right, so stop it from turning right and start turning left.
            system("xdotool keyup Right keydown Left");
            isTurningLeft = true ;
            isTurningRight = false ;
        }
    }
    else {
        putText(cameraFeed, "Straight", Point(5, 50), 1, 1, Scalar(255, 0, 0), 2);
        if (isTurningLeft || isTurningRight) {
            system("xdotool keyup Left keyup Right");
            isTurningLeft = false ;
            isTurningRight = false ;
        }
    }
}
