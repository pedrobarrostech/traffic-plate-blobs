/*#include <iostream>
#include <math.h>
#include <string>
#include "cv.h"
#include "ml.h"
#include "highgui.h"


using namespace cv;
using namespace std;

bool plotSupportVectors = true;
int numTrainingPoints = 200;
int numTestPoints = 2000;
int size = 200;
int eq = 0;

// accuracy
float evaluate(cv::Mat& predicted, cv::Mat& actual) {
    assert(predicted.rows == actual.rows);
    int t = 0;
    int f = 0;
    for(int i = 0; i < actual.rows; i++) {
        float p = predicted.at<float>(i,0);
        float a = actual.at<float>(i,0);
        if((p >= 0.0 && a >= 0.0) || (p <= 0.0 &&  a <= 0.0)) {
            t++;
        } else {
            f++;
        }
    }
    return (t * 1.0) / (t + f);
}

// plot data and class
void plot_binary(cv::Mat& data, cv::Mat& classes, string name) {
    cv::Mat plot(size, size, CV_8UC3);
    plot.setTo(cv::Scalar(255.0,255.0,255.0));
    for(int i = 0; i < data.rows; i++) {

        float x = data.at<float>(i,0) * size;
        float y = data.at<float>(i,1) * size;

        if(classes.at<float>(i, 0) > 0) {
            cv::circle(plot, Point(x,y), 2, CV_RGB(255,0,0),1);
        } else {
            cv::circle(plot, Point(x,y), 2, CV_RGB(0,255,0),1);
        }
    }
    cv::imshow(name, plot);
}

// function to learn
int f(float x, float y, int equation) {
    switch(equation) {
    case 0:
        return y > sin(x*10) ? -1 : 1;
        break;
    case 1:
        return y > cos(x * 10) ? -1 : 1;
        break;
    case 2:
        return y > 2*x ? -1 : 1;
        break;
    case 3:
        return y > tan(x*10) ? -1 : 1;
        break;
    default:
        return y > cos(x*10) ? -1 : 1;
    }
}

// label data with equation
cv::Mat labelData(cv::Mat points, int equation) {
    cv::Mat labels(points.rows, 1, CV_32FC1);
    for(int i = 0; i < points.rows; i++) {
             float x = points.at<float>(i,0);
             float y = points.at<float>(i,1);
             labels.at<float>(i, 0) = f(x, y, equation);
        }
    return labels;
}


void mlp(cv::Mat& trainingData, cv::Mat& trainingClasses, cv::Mat& testData, cv::Mat& testClasses) {

    cv::Mat layers = cv::Mat(4, 1, CV_32SC1);

    layers.row(0) = cv::Scalar(2);
    layers.row(1) = cv::Scalar(10);
    layers.row(2) = cv::Scalar(15);
    layers.row(3) = cv::Scalar(1);

    CvANN_MLP mlp;
    CvANN_MLP_TrainParams params;
    CvTermCriteria criteria;
    criteria.max_iter = 100;
    criteria.epsilon = 0.00001f;
    criteria.type = CV_TERMCRIT_ITER | CV_TERMCRIT_EPS;
    params.train_method = CvANN_MLP_TrainParams::BACKPROP;
    params.bp_dw_scale = 0.05f;
    params.bp_moment_scale = 0.05f;
    params.term_crit = criteria;

    mlp.create(layers);

    // train
    mlp.train(trainingData, trainingClasses, cv::Mat(), cv::Mat(), params);

    cv::Mat response(1, 1, CV_32FC1);
    cv::Mat predicted(testClasses.rows, 1, CV_32F);
    for(int i = 0; i < testData.rows; i++) {
        cv::Mat response(1, 1, CV_32FC1);
        cv::Mat sample = testData.row(i);

        mlp.predict(sample, response);
        predicted.at<float>(i,0) = response.at<float>(0,0);

    }

    cout << "Accuracy_{MLP} = " << evaluate(predicted, testClasses) << endl;
    plot_binary(testData, predicted, "Predictions Backpropagation");
}



int main5(int argc, char** argv) {

	cout << "OpenCV Training SVM Automatic Number Plate Recognition\n";
	cout << "\n";

	char* path_Plates;
	char* path_NoPlates;
	int numPlates;
	int numNoPlates;
	int imageWidth=144;
	int imageHeight=33;

	//Check if user specify image to process
	if(argc >= 5 )
	{
		numPlates= atoi(argv[1]);
		numNoPlates= atoi(argv[2]);
		path_Plates= argv[3];
		path_NoPlates= argv[4];

	}else{
		cout << "Usage:\n" << argv[0] << " <num Plate Files> <num Non Plate Files> <path to plate folder files> <path to non plate files> \n";
		return 0;
	}

	Mat classes;//(numPlates+numNoPlates, 1, CV_32FC1);
	Mat trainingData;//(numPlates+numNoPlates, imageWidth*imageHeight, CV_32FC1 );

	Mat trainingImages;
	vector<int> trainingLabels;

	for(int i=0; i< numPlates; i++)
	{

		stringstream ss(stringstream::in | stringstream::out);
		ss << path_Plates << i << ".jpg";
		Mat img=imread(ss.str(), 0);
		img= img.reshape(1, 1);
		trainingImages.push_back(img);
		trainingLabels.push_back(1);
	}

	for(int i=0; i< numNoPlates; i++)
	{
		stringstream ss(stringstream::in | stringstream::out);
		ss << path_NoPlates << i << ".jpg";
		Mat img=imread(ss.str(), 0);
		img= img.reshape(1, 1);
		trainingImages.push_back(img);
		trainingLabels.push_back(0);

	}

	Mat(trainingImages).copyTo(trainingData);
	//trainingData = trainingData.reshape(1,trainingData.rows);
	trainingData.convertTo(trainingData, CV_32FC1);
	Mat(trainingLabels).copyTo(classes);




    cv::Mat testData(numTestPoints, 2, CV_32FC1);
    cv::randu(testData,0,1);

    cv::Mat trainingClasses = labelData(trainingData, eq);
    cv::Mat testClasses = labelData(testData, eq);

    plot_binary(trainingData, trainingClasses, "Training Data");
    plot_binary(testData, testClasses, "Test Data");


    mlp(trainingData, trainingClasses, testData, testClasses);

    cv::waitKey();

    return 0;
}
*/
