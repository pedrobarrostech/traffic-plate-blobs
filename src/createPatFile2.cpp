#include <iostream>
#include <stdlib.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/features2d/features2d.hpp>

using namespace cv;
using namespace std;




int main (int argc, char** argv){
    
    int result,cont;
    //IplImage* dst;
    //dst = cvLoadImage(argv[1]); 
    // first convert the image to grayscale
    //IplImage* src = cvCreateImage(cvSize(dst->width,dst->height), dst->depth, dst->nChannels );
    //cvtColor(dst,src , CV_RGB2GRAY);

    // then adjust the threshold to actually make it binary
    //threshold(dst, dst, 100, 255, CV_THRESH_BINARY); 

    
    //cvCvtColor(dst, src, CV_RGB2GRAY);

    //cvSetImageROI(dst, cvRect(0, 6, 25, 12));  


    Mat im_gray;   // no nead to load the Mat with anything when declaring it.
    Mat im_rgb  = imread(argv[1]);
    cvtColor(im_rgb, im_gray,CV_RGB2GRAY);
    // INSTEAD of the above two lines you could have cv::Mat im_gray = imread("img1.png",CV_LOAD_IMAGE_GRAYSCALE);

    // the following is an alternative to Mat img_bw = im_gray > 128
    Mat img_bw;
    threshold(im_gray, img_bw, 128.0, 255.0, THRESH_BINARY);

    IplImage* src = new IplImage(img_bw);
    //cvShowImage("Test",src);
    //waitKey(3600);

    cont=0;
    for( int col = 0; col < src->height; col++ ){
        for ( int row = 0; row < src->width; row++ ) {
        
    
            if(CV_IMAGE_ELEM(src, unsigned char, row, col) == 0){
                result = 1;
            }
            else{
                result = 0;
            }

            if(result == 1){
                cont++;
            }

            //printf("%d", CV_IMAGE_ELEM(src, unsigned char, row, col));
            //printf("%s", ";");

        }
        printf("%d", cont);
        printf("%s", ";");
        //printf("%s", "\n");
        
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