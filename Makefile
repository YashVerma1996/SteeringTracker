####################################
# OpenCV default install locations #
# Check yours and replace.         #
####################################

OPENCV_LIBPATH=/usr/local/lib
OPENCV_INCLUDEPATH=/usr/local/include

GCC_OPTS= -m64 -std=c++11
GCC_OPENCV_OPTS=`pkg-config --cflags --libs opencv`

tracker: steering_main.o marker.o car_controller.o Makefile
	g++ -o tracker marker.o steering_main.o car_controller.o $(GCC_OPENCV_OPTS)

steering_main.o: steering_main.cpp constants.h marker.h car_controller.h
	g++ -c steering_main.cpp $(GCC_OPTS)

marker.o: marker.cpp marker.h constants.h
	g++ -c marker.cpp $(GCC_OPTS)

car_controller.o: car_controller.cpp car_controller.h marker.h constants.h
	g++ -c car_controller.cpp $(GCC_OPTS)

clean:
	rm -f *.o tracker
	find . -type f -name '*.exr' | grep -v memorial | xargs rm -f


