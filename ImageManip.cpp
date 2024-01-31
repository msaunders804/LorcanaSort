#include <C:\Users\User\source\repos\Lorcana\Lorcana\Lorcana.h>

void TakeImage() {
	//capture image and save as capture.jpg
	/*cv::VideoCapture cap(0);
	cout << cv::getBuildInformation() << endl;

	if (!cap.isOpened()) {
		std::cerr << "Could not open webcam" << std::endl;
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
	}*/
	cv::Mat frame = imread("C:\\Users\\User\\Downloads\\20240125_131000.jpg", IMREAD_COLOR);
	ImageManipulation(frame);
}

void ImageManipulation(Mat& frame) {
	cv::Mat labImg, greyImage, dst, normalizedImg, thresh;
	std::vector<cv::Mat> channels(3);
	std::vector<std::vector<cv::Point>> contours;
	std::vector<Vec4i> hierarchy;
	const auto clahe = cv::createCLAHE(4);
	// convert to lab color space
	//TEMP FOR TESTING
	cv::cvtColor(frame, labImg, cv::ColorConversionCodes::COLOR_BGR2Lab);

	// Extract the L channel
	cv::split(labImg, channels);  // now we have the L image in channels[0]

	// apply the CLAHE algorithm to the L channel
	clahe->apply(channels[0], dst);

	// Merge the the color planes back into an Lab image
	dst.copyTo(channels[0]);
	cv::merge(channels, labImg);

	// convert back to BGR
	cv::cvtColor(labImg, normalizedImg, cv::ColorConversionCodes::COLOR_Lab2BGR);
	//Process to find contors 
	cv::cvtColor(normalizedImg, greyImage, cv::COLOR_BGR2GRAY);
	cv::threshold(greyImage, thresh, 100, 255, cv::THRESH_BINARY);
	waitKey(0);
	cv::imwrite("THresh.jpg", thresh);
	destroyAllWindows();

	//cap.release();
	//Find borders of the cards
	thresh = 255 - thresh;
	cv::findContours(thresh.clone(), contours, hierarchy, cv::RetrievalModes::RETR_EXTERNAL, cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE);

	//draw on OG
	Mat image_copy = frame.clone();
	contours.erase(std::remove_if(contours.begin(), contours.end(), [](const auto& contour) {
		return std::fabs(cv::contourArea(cv::Mat(contour))) < 100;
		}), contours.end());
	for (size_t i = 0; i < contours.size(); i++)
	{
		cv::drawContours(image_copy, contours, (int)i, Scalar(0, 255, 0), 2);
	}
	waitKey(0);
	cv::imwrite("countours_image_1.jpg", image_copy);
	destroyAllWindows();
	ExtractCardImage(image_copy, contours);
}

void ExtractCardImage(Mat& input_image, const vector<std::vector<cv::Point>> contours)
{
	size_t largestContourIndex = 0;
	double largestContourArea = 0.0;

	for (size_t i = 0; i < contours.size(); ++i)
	{
		double area = contourArea(contours[i]);
		if (area > largestContourArea) {
			largestContourArea = area;
			largestContourIndex = i;
		}
	}

	Rect boundingRect = cv::boundingRect(contours[largestContourIndex]);
	Mat cardImg = input_image(boundingRect), clone();
	imshow("Card", cardImg);
	waitKey(0);
	imwrite("Card.jpg", cardImg);
	PerceptHashInput(cardImg);
}

