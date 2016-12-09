#include <opencv2/opencv.hpp>    // opencv general include file
#include <opencv2/ml/ml.hpp>          // opencv machine learning include file
#include <stdio.h>
#include <fstream>
#define ATTRIBUTES 256  //Number of pixels per sample.16X16
#define CLASSES 10                  //Number of distinct labels.
int main4( int argc, char** argv )
{
    //read the model from the XML file and create the neural network.
    CvANN_MLP nnetwork;
    CvFileStorage* storage = cvOpenFileStorage( "param.xml", 0, CV_STORAGE_READ );
    CvFileNode *n = cvGetFileNodeByName(storage,0,"DigitOCR");
    nnetwork.read(storage,n);
    cvReleaseFileStorage(&storage);

    //your code here
    // ...Generate cv::Mat data(1,ATTRIBUTES,CV_32S) which will contain the pixel
    // ... data for the digit to be recognized
    // ...

    int maxIndex = 0;
    cv::Mat classOut(1,CLASSES,CV_32F);
    //prediction
    //nnetwork.predict(data, classOut);
    float value;
    float maxValue=classOut.at<float>(0,0);
    for(int index=1;index<CLASSES;index++)
    {   value = classOut.at<float>(0,index);
            if(value>maxValue)
            {   maxValue = value;
                maxIndex=index;
            }
    }
    //maxIndex is the predicted class.

}
