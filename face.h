#ifndef FACE_H
#define FACE_H

#include <opencv2/opencv.hpp>
#include <vector>
bool MixImage(cv::Mat& srcImage, cv::Mat mixImage, cv::Point startPoint);

std::vector<cv::Rect> cvFindFacesAndDrawSave(const std::string& str, const std::string& key, const std::string& output);

std::vector<cv::Rect> cnnFindFacesAndDrawSave(const std::string& str, const std::string& key, const std::string& output);

void drawFaceAndSave(std::vector<cv::Rect>& faces, cv::Mat& src, const std::string& str, const std::string& key, const std::string& output);

#endif