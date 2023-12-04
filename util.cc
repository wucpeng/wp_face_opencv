#include "util.h"
#include <time.h>
#include <vector>
#include <curl/curl.h>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


//时间戳转换为日期格式
void timestampToTime(char* timeStamp, char* dateTime, int dateTimeSize)
{
	time_t tTimeStamp = atoll(timeStamp);
	struct tm* pTm = gmtime(&tTimeStamp);
	strftime(dateTime, dateTimeSize, "%Y-%m-%d %H:%M:%S", pTm);
}


UnixTime getUnixTime() {
    struct timeval tv;
    gettimeofday (&tv , NULL);
    UnixTime currentTimeStamp = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return currentTimeStamp;
}


//curl writefunction to be passed as a parameter
// we can't ever expect to get the whole image in one piece,
// every router / hub is entitled to fragment it into parts
// (like 1-8k at a time),
// so insert the part at the end of our stream.
size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    vector<uchar> *stream = (vector<uchar>*)userdata;
    //cout<<ptr<< size<<endl;
    size_t count = size * nmemb;
    stream->insert(stream->end(), ptr, ptr + count);
    //cout<<count<<endl;
    return count;
}


void modifyImageFormat(cv::Mat& out, cv::Mat& in, int channel, int depth) 
{
    float factor = 1;
    if (depth == 2) {
        factor = 1.0 / 255;
    } else if (depth == 4) {
        factor = 1.0 / (255 * 255);
    }
    if (channel == 4) {
        cv::Mat tmp;
        cv::cvtColor(in, tmp, cv::COLOR_BGRA2BGR);
        tmp.convertTo(out, CV_8UC3, factor);
        tmp.release();
    } else if (channel == 3) {
        in.convertTo(out, CV_8UC3, factor);
    } else if (channel == 2) {
        in.convertTo(out, CV_8UC2, factor);
    } else {
        in.convertTo(out, CV_8U, factor);
    }
}


//function to retrieve the image as cv::Mat data type
cv::Mat curlImg(const char *img_url, int timeout)
{
    //cout<<img_url<<endl;
    char *pCaPath = "/opt/ssl_csr/*.haizitong.com.chained.crt";
    vector<uchar> stream;
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, img_url); //the img url
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // pass the writefunction
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream); // pass the stream ptr to the writefunction
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout); // timeout if curl_easy hangs, 
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); //重定向一次

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); //表示启用了验证访问的服务器合法性，且必须设置 CURLOPT_CAINFO 或 CURLOPT_CAPATH 其中一个
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L); //表示验证 CA 证书中的 common name 是否与访问的服务器域名是否一致
    curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
    CURLcode res = curl_easy_perform(curl); // start curl    
    curl_easy_cleanup(curl); // cleanup
    if (res != CURLE_OK) {
        cout << "curl errcode=" << res << " " << img_url <<endl;
        cv::Mat dest(0, 0, 1);
        return dest;
    }
    //cout<<img_url<<" download: " << stream.size() << " " << stream.max_size()<<endl;
    cv::Mat src = imdecode(stream, -1); // 'keep-as-is'
    //int Height = src.rows;
    int depth = src.depth();
    int channel = src.channels();
    //cout << "Width:" << Width << ",Height:"<<Height <<endl;
    // cout << "dims: " << src.dims << endl;
    // cout << "channels: " << channel << endl;
    // cout << "depth: " << cv::depthToString(src.depth()) << " , " << src.depth() << endl;
    // cout << "type: " << cv::typeToString(src.type()) << " , value=" << src.type() << endl;
    //cout << "depth:" << depth << ", channel" << channel << endl;
    if (depth != 0) { //bit数仅支持8
        cv::Mat mid; //(src.cols, src.rows, CV_8UC3);
        modifyImageFormat(mid, src, channel, depth);
        src.release();
        return mid;
    } else {
        return src;
    }
}

std::vector<std::string> stringSplit(const std::string& str, char delim) {
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}

std::string getImageUrlId(const std::string& str) {
    std::vector<std::string> vecList = stringSplit(str, '/');
    return vecList[vecList.size() - 1];
}


void imageBackgroundBlack(cv::Mat& src, const std::string key) {
    int Width = src.cols;
	int Height = src.rows;
    int dims = src.channels();
    for(int j = 0; j < Height; ++j){
        for(int i = 0; i < Width; ++i) {
            if (dims == 4){
				Vec4b bgr=src.at<Vec4b>(j, i);//返回4个值哦	可以把bgr这个结构看成数组
                //cout << (int)bgr[3] << " "; 
                if ((int)bgr[3] <= 10) { //bgr[0] == 255 && bgr[1] == 255 && bgr[2] ==255
                    src.at<Vec4b>(j, i)[0] = 0; //255 - bgr[0];
				    src.at<Vec4b>(j, i)[1] = 0; //255 - bgr[1];
				    src.at<Vec4b>(j, i)[2] = 0; //255 - bgr[2];
                }
			}
        }
        //cout << endl; 
    }
    cv::Mat dst;
    cv::cvtColor(src, dst, cv::COLOR_BGRA2BGR);
    cv::imwrite("back-"+ key + ".png", dst);
    dst.release();
}