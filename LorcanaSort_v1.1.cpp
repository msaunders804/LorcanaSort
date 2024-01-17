// LorcanaSort_v1.1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/opencv.hpp>
//#include <opencv2/img_hash.hpp>
#include <string>
#include <C:\Users\msaun\source\repos\LorcanaSort_v1.1\LorcanaSort_v1.1\LorcanaSort.h>

using namespace std;
using namespace cv;
//using namespace cv::img_hash;

int main(int argc, char* argv[])
{
	Mat img = imread("C:\\Users\\msaun\\Pictures\\ariel.jpg", IMREAD_COLOR);
	//PerceptHash(img);
	//capture image and save as capture.jpg
	cv::VideoCapture cap(1);
	cout << cv::getBuildInformation() << endl;

	if (!cap.isOpened()) {
		std::cerr << "Could not open webcam" << std::endl;
		return -1;
	}

	cv::Mat frame;
	cv::Mat greyImage;

	int n = 10;
	for (int i = 0; i < n; i++)
	{
		cap.read(frame);
	}

	if (frame.empty()) {
		std::cerr << "Could not capture frame" << std::endl;
		return -1;
	}

	const auto clahe = cv::createCLAHE(4);
	// convert to lab color space
	cv::Mat labImg;
	cv::cvtColor(frame, labImg, cv::ColorConversionCodes::COLOR_BGR2Lab);

	// Extract the L channel
	std::vector<cv::Mat> channels(3);
	cv::split(labImg, channels);  // now we have the L image in channels[0]

	// apply the CLAHE algorithm to the L channel
	cv::Mat dst;
	clahe->apply(channels[0], dst);

	// Merge the the color planes back into an Lab image
	dst.copyTo(channels[0]);
	cv::merge(channels, labImg);

	// convert back to BGR
	cv::Mat normalizedImg;
	cv::cvtColor(labImg, normalizedImg, cv::ColorConversionCodes::COLOR_Lab2BGR);

	//Process to find contors 
	cv::cvtColor(normalizedImg, greyImage, cv::COLOR_BGR2GRAY);
	cv::Mat thresh;
	cv::threshold(greyImage, thresh, 100, 255, cv::THRESH_BINARY);
	waitKey(0);
	cv::imwrite("THresh.jpg", thresh);
	destroyAllWindows();

	cap.release();
	//Find borders of the cards
	std::vector<std::vector<cv::Point>> contours;
	std::vector<Vec4i> hierarchy;
	thresh = 255 - thresh;
	cv::findContours(thresh, contours, hierarchy, cv::RetrievalModes::RETR_EXTERNAL, cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE);

	//draw on OG
	Mat image_copy = frame.clone();
	contours.erase(std::remove_if(contours.begin(), contours.end(), [](const auto& contour) {
		return std::fabs(cv::contourArea(cv::Mat(contour))) < 100;
		}), contours.end());
	cv::drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);
	cv::imshow("None Approx.", image_copy);
	waitKey(0);
	cv::imwrite("countours_image_1.jpg", image_copy);
	destroyAllWindows();


	//Rect boundingRect = cv::boundingRect(contours[0]);
	const auto boundingRect = cv::minAreaRect(contours[0]);
	std::vector<cv::Point2f> src(4);
	boundingRect.points(src.data());
	cv::drawContours(image_copy, src, 0, (0, 0, 255), 2);


	const auto width = std::min(boundingRect.size.height, boundingRect.size.width);
	const auto height = std::max(boundingRect.size.height, boundingRect.size.width);

	std::vector<cv::Point2f> dest(4);
	dest[0] = { 0, height - 1 };
	dest[1] = { 0, 0 };
	dest[2] = { width - 1, 0 };
	dest[3] = { width - 1, height - 1 };

	if (width == height) {// horizontally orientated
		std::rotate(src.begin(), src.begin() + 1, src.end());
	}

	const auto M = cv::getPerspectiveTransform(src.data(), dest.data());
	cv::Mat warped;
	cv::warpPerspective(image_copy, warped, M, { static_cast<int>(width), static_cast<int>(height) });
	cv::imshow("J", warped);

	//use capture.jpg to extract information
	/*
	string outText, imPath = "C:\\Users\\msaun\\source\\repos\\LorcanaSort_v1.1\\LorcanaSort_v1.1\\capture.jpg";
	cv::Mat im = cv::imread(imPath, IMREAD_COLOR);
	tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI();

	ocr->Init(NULL, "eng", tesseract::OEM_DEFAULT);
	ocr->SetPageSegMode(tesseract::PSM_AUTO);
	ocr->SetImage(im.data, im.cols, im.rows, 3, im.step);

	outText = string(ocr->GetUTF8Text());
	cout << outText;

	ocr->End();

	return 0;

	//save relevant numbers from image
	//store information to update xls
	//update xls
	//save xls, delete capture.jpg
}

cv::Mat PerceptHash(cv::Mat image)
{
	Mat img = image;
	Mat gray;
	cv::cvtColor(img, gray, COLOR_BGR2GRAY);
	Mat resized;
	cv::resize(gray, resized, Size(128, 128));
	Mat hash;
	img_hash::pHash(resized, hash);
	cout << hash << endl;
}*/

}
