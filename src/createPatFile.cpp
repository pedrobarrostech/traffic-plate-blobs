#include <iostream>
#include <stdlib.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/features2d/features2d.hpp>

using namespace cv;
using namespace std;



int main (int argc, char** argv){

	IplImage* src;
	src = cvLoadImage(argv[1], 1);
	int result;

	IplImage *destination = cvCreateImage( cvSize(10,10) , src->depth, src->nChannels );
	cvResize(src, destination);

	//int col, row;
	printf("%s", argv[1]);
	printf("%s", ";");


	for( int row = 0; row < destination->height; row++ ){
		for ( int col = 0; col < destination->width; col++ )
	    {

			printf("%u", (unsigned int) CV_IMAGE_ELEM(destination, unsigned char, row, col));
			printf("%s", ";");



	    }
	}

	printf("%s", "\n");
	waitKey(0);


	return 0;
}

