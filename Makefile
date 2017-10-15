####################################
# OpenCV default install locations #
# Check yours and replace.         #
####################################

OPENCV_LIBPATH=/usr/local/lib
OPENCV_INCLUDEPATH=/usr/local/include

GCC_OPTS=-O3 -m64
GCC_OPENCV_OPTS=`pkg-config --cflags --libs opencv`

tracker: SteeringMain.o Marker.o Makefile
	g++ -o tracker Marker.o SteeringMain.o  $(GCC_OPENCV_OPTS)

SteeringMain.o: SteeringMain.cpp Marker.cpp Constants.h Marker.h
	g++ -c SteeringMain.cpp

Marker.o: Marker.cpp Marker.h Constants.h
	g++ -c Marker.cpp

clean:
	rm -f *.o tracker
	find . -type f -name '*.exr' | grep -v memorial | xargs rm -f


