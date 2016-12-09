#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/features2d/features2d.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

using namespace cv;
using namespace std;
using namespace tesseract;



void frameDeteccaoPlaca( IplImage* frame );
IplImage* encontraPlaca(CvSeq* contours, IplImage* frame_gray, IplImage* frame );
void tesseractOCR(IplImage *placa, int x, int y, int width, int height);
void saveImg(IplImage *img);
double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 );


Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;



/*double angle(CvPoint* pt1, CvPoint* pt2, CvPoint* pt0) {
	double dx1 = pt1->x - pt0->x;
	double dy1 = pt1->y - pt0->y;
	double dx2 = pt2->x - pt0->x;
	double dy2 = pt2->y - pt0->y;
	return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}*/

void tesseractOCR(IplImage *placa, int x, int y, int width, int height) {

	char *outText;

	TessBaseAPI *api = new TessBaseAPI();
	// Initialize tesseract-ocr with English, without specifying tessdata path
	if (api->Init(NULL, "eng")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}
	api->SetVariable("tessedit_char_whitelist", "1234567890ABCDEFGHIJKLMNOPGRSTUVXYZ");

	// Open input image with leptonica library
	api->SetImage((uchar*) placa->imageData, placa->width, placa->height,
				placa->nChannels, placa->widthStep);

	// Get OCR result
	outText = api->GetUTF8Text();
	printf("OCR output:\n%s", outText);

	// Destroy used object and release memory
	api->End();
	delete [] outText;


}

void frameDeteccaoPlaca(IplImage* frame) {

	IplImage* frame_gray = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);
	IplImage* frame_canny = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);
	IplImage* frame_canny2 = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);
	IplImage* frame_merge = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);
	// Escala cinza
	cvCvtColor(frame, frame_gray, CV_BGR2GRAY);
	// Retira ruidos
	cvSmooth(frame_gray, frame_gray, CV_GAUSSIAN, 3, 3);

	// Filtros canny cinza e canny colorida
	cvCanny(frame_gray, frame_canny, 0, 255, 3);

	CvSeq* contours = 0;
	CvMemStorage* storage = NULL;

	if (storage == NULL)
		storage = cvCreateMemStorage(0);
	else
		cvClearMemStorage(storage);


	cvFindContours(frame_canny, storage, &contours, sizeof(CvContour),CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	encontraPlaca(contours,frame_gray,frame);
	//-- Mostra imagem
	//cvShowImage("Deteccao da placa", encontraPlaca(contours,frame_gray,frame) );
	//cvShowImage("Deteccao da placa", frame);
	// Libera recursos
	cvReleaseImage(&frame_gray);
	//cvReleaseImage(&frame_gray_s);
	cvReleaseImage(&frame_canny);
	cvReleaseImage(&frame_canny2);
	cvReleaseImage(&frame_merge);
	cvReleaseMemStorage(&storage);
}


IplImage* encontraPlaca(CvSeq* contours, IplImage* frame_gray,
		IplImage* frame) {
	IplImage* licenca = NULL;
	for (; contours != NULL; contours = contours->h_next) {
		CvSeq* approxContour = cvApproxPoly(contours, contours->header_size,
				contours->storage, CV_POLY_APPROX_DP,
				cvContourPerimeter(contours) * 0.05, 0);

		// desenha retangulo vermelho
		cvDrawContours(frame, approxContour, CV_RGB(255, 0, 0),
				CV_RGB(255, 0, 0), -1, CV_FILLED, 8);


		CvRect box = cvBoundingRect(approxContour);


	}

	licenca = cvCreateImage(cvSize(52,52), IPL_DEPTH_8U, 1);
	IplImage *destination = cvCreateImage ( cvSize(80 , 80 ), IPL_DEPTH_8U, 1 );

	//cvShowImage("sign gray", frame_gray);

	cvResize(frame_gray, destination, CV_INTER_LINEAR);

	//recorta imagem
	CvRect cropRect = cvRect(12, 13, 52,52); // ROI in source image
	cvSetImageROI(destination, cropRect);
	cvCopy(destination, licenca, NULL); // Copies only crop region
	cvResetImageROI(destination);

	//trata placa comum
	cvThreshold(licenca, licenca, 0, 255,		CV_THRESH_TRUNC | CV_THRESH_OTSU);
	cvThreshold(licenca, licenca, 0, 255,	CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	cvThreshold(licenca, licenca, 0, 255,	CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	cvDilate(licenca,licenca,NULL,0.5);




	//cvShowImage("roi chars", licenca);

	//tesseractOCR(licenca, 0,0, 65, 30);



	saveImg(licenca);

	return frame;
}


void saveImg(IplImage *img)
{
	time_t t = time(0);
	struct tm tstruct;

	char buf[80];
	tstruct = *localtime (&t);
	strftime(buf, sizeof(buf), "%m-%d-%H-%M-%S.jpg" , &tstruct);
	cvSaveImage(buf,img);
	cvShowImage("sign gray", img);
}




int main23555(int argc, char** argv) {

	IplImage* src;
	src = cvLoadImage(argv[1], 1);
	frameDeteccaoPlaca(src);
	waitKey(0);
	return 0;
}








