#pragma once
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/img_hash.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <cstdio>
#include <cppconn/driver.h> 
#include <cppconn/exception.h> 
#include <cppconn/statement.h> 
#include <cppconn/prepared_statement.h>
#include <mysql_connection.h> 
#include <mysql_driver.h> 

using namespace std;
using namespace cv;

//Menu Functions
void DisplayMenu();
void ExecuteChoice(int choice);

//Image manipulation functions
void TakeImage();
void ImageManipulation(Mat& frame);
void ExtractCardImage(Mat& image_copy, const vector<std::vector<cv::Point>> contours);


//Hash Generation/Storage Functions
class MyPhash : public cv::img_hash::PHash {
public:
	using cv::img_hash::PHash::compare;
};

void PerceptHashInput(Mat& image);
void CompareHashes(const Mat& inputHash);
void PerceptHashForDB();
void AddtoDB();
void AddtoFile(string cardnum, Mat hash);
int HammingDistance(vector<int>& inputHash, vector<int>& DBHash);