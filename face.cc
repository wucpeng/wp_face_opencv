#include "face.h"
#include <stdio.h>
#include "util.h"
#include <time.h>
#include "facedetectcnn.h"

using namespace std;
using namespace cv;

const string mixFlower = "./images/flower.png";
const string mixHead = "./images/head.png";
const string mixRabbit = "./images/rabbit.png";
const string xmlPath = "./xml/haarcascade_frontalface_alt2.xml";

//define the buffer size. Do not change the size!
//0x9000 = 1024 * (16 * 2 + 4), detect 1024 face at most
#define DETECT_BUFFER_SIZE 0x9000

//图片混合
bool MixImage(cv::Mat& srcImage, cv::Mat mixImage, cv::Point startPoint){
	//检查图片数据
	if (!srcImage.data || !mixImage.data)
	{
		cout << "输入图片 数据错误！" << endl ;
		return false;
	}
	//检查行列是否越界
	int addCols = startPoint.x + mixImage.cols > srcImage.cols ? 0 : mixImage.cols;
	int addRows = startPoint.y + mixImage.rows > srcImage.rows ? 0 : mixImage.rows;
	if (addCols ==0 || addRows ==0)
	{
		cout << "添加图片超出" << endl;
		return false;
	}

	//ROI 混合区域
	Mat roiImage = srcImage(Rect(startPoint.x, startPoint.y, addCols, addRows));
	//图片类型一致
	if (srcImage.type() == mixImage.type()){
		mixImage.copyTo(roiImage, mixImage);
		return true;
	}

	Mat maskImage;
	//原始图片：灰度  贴图：彩色
	if (srcImage.type() == CV_8U && mixImage.type() == CV_8UC3)
	{
		cvtColor(mixImage, maskImage, COLOR_BGR2GRAY);
		maskImage.copyTo(roiImage, maskImage);
		return true;
	}

	//原始图片：彩色  贴图：灰色
	if (srcImage.type() == CV_8UC3 && mixImage.type() == CV_8U)
	{
		cvtColor(mixImage, maskImage, COLOR_GRAY2BGR);
		maskImage.copyTo(roiImage, maskImage);
		return true;
	}

	return false;
}

void drawFaceAndSave(std::vector<cv::Rect>& faces, cv::Mat& src, const std::string& str, const std::string& key, const std::string& output) {
	string id = getImageUrlId(str);
    cv::Mat flower; // = cv::imread(mixFlower.c_str()); // 0 bgr, 1 grey, -1 aplha
	if (key == "flower") {
		flower = cv::imread(mixFlower.c_str());
	} else if (key == "head") {
		flower = cv::imread(mixHead.c_str());
	} else {
		flower = cv::imread(mixRabbit.c_str());
	}
	// cv::Mat test12 = cv::imread("12.png", -1);
	// imageBackgroundBlack(test12, "test12");
	// test12.release();
	// cv::Mat test13 = cv::imread("13.png", -1);
	// imageBackgroundBlack(test13, "test13");
	// test13.release();
	// cout << key << ": " << endl;
	// imwrite("test.png", flower);
	// int depth = flower.depth();
    // int channel = flower.channels();
	// cout << "dims: " << flower.dims << endl;
    // cout << "channels: " << channel << endl;
    // cout << "depth: " << cv::depthToString(flower.depth()) << " , " << flower.depth() << endl;
    // cout << "type: " << cv::typeToString(flower.type()) << " , value=" << flower.type() << endl; //alpha
    // cout << "depth:" << depth << ", channel" << channel << endl;

	// cv::Mat tmp;
    // cv::cvtColor(flower, tmp, cv::COLOR_BGRA2BGR);
	// imwrite( key + ".png", tmp);

	// int Width = flower.cols;
	// int Height = flower.rows;
	// cout << "Height=" << Height << " , Width=" << Width << endl;
	
	// if (flower.type() != src.type()) {
	// 	cv::Mat tmp;
	// 	cv::cvtColor(src, tmp, cv::COLOR_BGR2BGRA);
	// 	src.release();
	// 	src = tmp;
	// }

	int Width = flower.cols;
	int Height = flower.rows;
	//cout << "Height=" << Height << " , Width=" << Width << endl;
	for(int i = 0; i < faces.size(); ++i) {
        cv::Rect rc = faces[i];
		// float factor = (float)rc.width / Width;
		// int height = (int)(factor * Height);
		// cout << "rcHeight=" << rc.height << " , rcWidth=" << rc.width << endl;
		// cout << "factor: " << factor << " , height=" << height << endl;
		cv::Size dsize;
		if (rc.width > rc.height) {
			float factor = (float)rc.width / Width;
			int height = (int)(factor * Height);
			// cout << "rcHeight=" << rc.height << " , rcWidth=" << rc.width << endl;
			// cout << "factor: " << factor << " , height=" << height << endl;
			dsize = cv::Size(rc.width, height);
		} else {
			float factor = (float)rc.height / Height;
			int width = (int)(factor * Width);
			// cout << "rcHeight=" << rc.height << " , rcWidth=" << rc.width << endl;
			// cout << "factor: " << factor << " , width=" << width << endl;
			dsize = cv::Size(width, rc.height);
		}
        //cv::Size dsize = cv::Size(rc.width, rc.height);
        cv::Mat flowerMode = cv::Mat(dsize, CV_8U);
        cv::resize(flower, flowerMode, dsize);
        cv::Point startPoint(rc.x, rc.y);
        bool isDone = MixImage(src, flowerMode, startPoint);
        flowerMode.release();
    }
	string fileName = output + "/" + id + ".png";
    imwrite(fileName.c_str(), src);
	flower.release();
}


std::vector<cv::Rect> cvFindFacesAndDrawSave(const std::string& str, const std::string& key, const std::string& output){
    cv::Mat src = curlImg(str.c_str(), 10000);
    int Width = src.cols;
    std::vector<cv::Rect> faces;
    if (Width == 0) {
        src.release();
        return faces;
    }
    cv::CascadeClassifier cascade;
    //haarcascade_frontalface_alt2.xml
    if (!cascade.load(xmlPath)){
		cout << "cascade load failed!\n";
        return faces;
	}
    double t = 0;
	t = (double)getTickCount();
    //cout << str << " getTickCount: " << t << endl;

    
    cascade.detectMultiScale(src, faces, 1.1, 4, 0, Size(30, 30));
    //cout << str <<" detect face number is :" << faces.size() << endl;
    if (faces.size() == 0) {
        return faces;
    }
	drawFaceAndSave(faces, src, str, key, output);
    src.release();
    return faces;
}


std::vector<cv::Rect> cnnFindFacesAndDrawSave(const std::string& str, const std::string& key, const std::string& output){
    cv::Mat src = curlImg(str.c_str(), 10000);
    int Width = src.cols;
    std::vector<cv::Rect> faces;
    if (Width == 0) {
        src.release();
        return faces;
    }

	int * pResults = NULL; 
    //pBuffer is used in the detection functions.
    //If you call functions in multiple threads, please create one buffer for each thread!
    unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
    if(!pBuffer)
    {
        fprintf(stderr, "Can not alloc buffer.\n");
        return faces;
    }
	// cout << "depth: " << cv::depthToString(src.depth()) << " , " << src.depth() << endl;
    // cout << "type: " << cv::typeToString(src.type()) << " , value=" << src.type() << endl;

	TickMeter cvtm;
    cvtm.start();
	pResults = facedetect_cnn(pBuffer, (unsigned char*)(src.ptr(0)), (int)src.cols, (int)src.rows, (int)src.step);
    cvtm.stop();    
    // printf("time = %gms\n", cvtm.getTimeMilli());
	// printf("%d faces detected.\n", (pResults ? *pResults : 0));

	for(int i = 0; i < (pResults ? *pResults : 0); i++)
	{
        short * p = ((short*)(pResults + 1)) + 16*i;
		int confidence = p[0];
		if (confidence < 70) break;
		int x = p[1];
		int y = p[2];
		int w = p[3];
		int h = p[4];
		//show the score of the face. Its range is [0-100]
        // char sScore[256];
        // snprintf(sScore, 256, "%d", confidence);
		// printf("face %d: confidence=%d, [%d, %d, %d, %d] (%d,%d) (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n", 
        //         i, confidence, x, y, w, h, 
        //         p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13],p[14]);

		int addWidth = w / 10;
		if (addWidth) {
			x -= addWidth;
			if (x < 0 ) x = 0;
			//w += addWidth;
		}
		int addHeight = h / 10;
		if (addHeight) {
			y -= addHeight;
			if (y < 0 ) y = 0;
			//h += addHeight;
		}

		cv::Rect face(x, y, w, h);
		faces.push_back(face);
	}
	free(pBuffer);
	if (faces.size() > 0) {
		drawFaceAndSave(faces, src, str, key, output);
	} 
	src.release();
    return faces;
}
