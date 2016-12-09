#include <iostream>
#include <stdlib.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/features2d/features2d.hpp>

using namespace cv;
using namespace std;

/**
 * Rotate an image
 */

 char filename[25];

void rotate(cv::Mat& src, double angle, cv::Mat& dst)
{
    int len = std::max(src.cols, src.rows);
    cv::Point2f pt(len/2., len/2.);
    cv::Mat r = cv::getRotationMatrix2D(pt, angle, 1.0);

    cv::warpAffine(src, dst, r, cv::Size(len, len));
}

void saveImg(IplImage *img)
{
    time_t t = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime (&t);
    strftime(buf, sizeof(buf), filename , &tstruct);
    cvSaveImage(buf,img);
    cvShowImage("sign gray", img);
    printf("%s",filename);
}



int main (int argc, char** argv){
    sprintf(filename, argv[1],25);
	int result,cont;
	IplImage* src;
	src = cvLoadImage(argv[1], 1);
    

	IplImage *destination = cvCreateImage( cvSize(40,40) , src->depth, src->nChannels );
	cvResize(src, destination);

    Mat mat_img(destination );
    rotate(mat_img, -45, mat_img);
    
    IplImage* destination2= new IplImage(mat_img);
    cvSetImageROI(destination2,cvRect(0,0,40,40));
    Mat mat_img2(destination2 );
    rotate(mat_img2, 45, mat_img2);

    // flood fill with red  
    CvPoint seedPoint = cvPoint(1,1); 
    cvFloodFill(destination2, seedPoint, CV_RGB(255,255,255), CV_RGB(8,90,60), CV_RGB(255,255,255),NULL,4,NULL);  
    
    CvPoint seedPoint2 = cvPoint(39,0);  
    cvFloodFill(destination2, seedPoint2, CV_RGB(255,255,255), CV_RGB(8,90,60), CV_RGB(255,255,255),NULL,4,NULL);  
    
    CvPoint seedPoint3 = cvPoint(39,39);  
    cvFloodFill(destination2, seedPoint3, CV_RGB(255,255,255), CV_RGB(8,90,60), CV_RGB(255,255,255),NULL,4,NULL);  
    
    CvPoint seedPoint4 = cvPoint(0,39);  
    cvFloodFill(destination2, seedPoint4, CV_RGB(255,255,255), CV_RGB(8,90,60), CV_RGB(255,255,255),NULL,4,NULL);  
    
    cvSetImageROI(destination2,cvRect(1,1,38,38));

    IplImage *destination3 = cvCreateImage( cvSize(25,25) , src->depth, src->nChannels );
    cvResize(destination2, destination3);
    //cvErode(destination2,destination2,NULL,0.5);

    //IplImage* destination3= new IplImage(mat_img2);

    //saveImg(destination3);

    cvShowImage("Test",destination2);
	waitKey(3600);

    cont=0;
    for ( int row = 0; row < destination->width; row++ ) {
        for( int col = 0; col < destination->height; col++ ){
    
            if(CV_IMAGE_ELEM(destination, unsigned char, row, col) < 128){
                result = 1;
            }
            else{
                result = 0;
            }

            if(result == 1){
                cont++;
            }

            //printf("%d", result);
            //printf("%s", ";");

        }
        printf("%d", cont);
        printf("%s", ";");
        
        cont=0;


    }

    printf("%s", " ");
    printf("%s", ";");    


    //printf("%s", "\n");

    //printf("%d", destination->width);
    //printf("%d", destination->height);

    if(strstr (argv[1],"direita") != 0){
    	printf("%d", 0);
    	printf("%s", ";");
    	printf("%d", 0);
    	printf("%s", ";");
    	printf("%d", 1);
        printf("%s", ";");
    }
    if(strstr (argv[1],"esquerda") != 0){
    	printf("%d", 1);
    	printf("%s", ";");
    	printf("%d", 0);
    	printf("%s", ";");
    	printf("%d", 0);
        printf("%s", ";");
    }
    if(strstr (argv[1],"frente") != 0){
    	printf("%d", 0);
    	printf("%s", ";");
    	printf("%d", 1);
    	printf("%s", ";");
    	printf("%d", 0);
        printf("%s", ";");
    }

	printf("%s", "\n");


	return 0;
}