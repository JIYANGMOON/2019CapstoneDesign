#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat region_of_interest(Mat canny_image); // ROI 

int main()
{

		//VideoCapture inputVideo("drive.mp4");
	Mat srcImage_GRAY;
	Mat srcImage_COLOR;
	Mat srcImage;
	Mat hough_image;
	Mat canny_image;
	Mat ROI_image;
	Mat result;

	int range_count = 0;

	Scalar red(0, 0, 255);
	Scalar blue(255, 0, 0);
	Scalar yellow(0, 255, 255);

	//Scalar magenta(255, 0, 255);


	Mat rgb_color = Mat(1, 1, CV_8UC3, red);
	Mat hsv_color;

	cvtColor(rgb_color, hsv_color, COLOR_BGR2HSV);


	int hue = (int)hsv_color.at<Vec3b>(0, 0)[0];
	int saturation = (int)hsv_color.at<Vec3b>(0, 0)[1];
	int value = (int)hsv_color.at<Vec3b>(0, 0)[2];


	cout << "hue = " << hue << endl;
	cout << "saturation = " << saturation << endl;
	cout << "value = " << value << endl;


	int low_hue = hue - 10;
	int high_hue = hue + 10;

	int low_hue1 = 0, low_hue2 = 0;
	int high_hue1 = 0, high_hue2 = 0;

	if (low_hue < 10) {
		range_count = 2;

		high_hue1 = 180;
		low_hue1 = low_hue + 180;
		high_hue2 = high_hue;
		low_hue2 = 0;
	}
	else if (high_hue > 170) {
		range_count = 2;

		high_hue1 = low_hue;
		low_hue1 = 180;
		high_hue2 = high_hue - 180;
		low_hue2 = 0;
	}
	else {
		range_count = 1;

		low_hue1 = low_hue;
		high_hue1 = high_hue;
	}


	cout << low_hue1 << "  " << high_hue1 << endl;
	cout << low_hue2 << "  " << high_hue2 << endl;


	//VideoCapture cap("drive.mp4");
	Mat img_frame, img_hsv;


	//if (!cap.isOpened()) {
	//	cerr << "ERROR! Unable to open camera\n";
	//	return -1;
	//}
	VideoCapture cap;
	// open the default camera using default API
	// cap.open(0);
	// OR advance usage: select any API backend
	int deviceID = 0;             // 0 = open default camera
	int apiID = cv::CAP_ANY;      // 0 = autodetect default API
						  // open selected camera using selected API
	cap.open(deviceID + apiID);
	// check if we succeeded
	if (!cap.isOpened()) {
		cerr << "ERROR! Unable to open camera\n";
		return -1;
	}

	int fps = (int)(cap.get(CAP_PROP_FPS));
	int delay = 1000 / fps;
	int count = 0;

	for (;;)
	{
		cap >> srcImage; // frame�� �о�´�.
		srcImage_COLOR = srcImage.clone();
		cvtColor(srcImage, srcImage_GRAY, COLOR_BGR2GRAY);
		//Canny(srcImage_GRAY, canny_image, 100, 100, 3); // edge ����
		//if( count == 0) 
		ROI_image = region_of_interest(srcImage); // ROI
		//else if (count > 0) ROI_image = line_new_ROI(canny_image);
		imshow("d", ROI_image);
		//hough_image = hough_lines(ROI_image, srcImage_COLOR); //������ȯ
		//result = weight_image(hough_image, srcImage_COLOR);
		//imshow("result", result);

		int ckey = waitKey(delay);
		if (ckey == 27) break;
		count++;
		// wait for a new frame from camera and store it into 'frame'
		//cap.read(ROI_image);

		// check if we succeeded
		if (ROI_image.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
			break;
		}


		//HSV�� ��ȯ
		cvtColor(ROI_image, img_hsv, COLOR_BGR2HSV);


		//������ HSV ������ �̿��Ͽ� ������ ����ȭ
		Mat img_mask1, img_mask2;
		inRange(img_hsv, Scalar(low_hue1, 50, 50), Scalar(high_hue1, 255, 255), img_mask1);
		if (range_count == 2) {
			inRange(img_hsv, Scalar(low_hue2, 50, 50), Scalar(high_hue2, 255, 255), img_mask2);
			img_mask1 |= img_mask2;
		}


		//morphological opening ���� ������ ���� 
		erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));


		//morphological closing ������ ���� �޿�� 
		dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));


		//�󺧸� 
		Mat img_labels, stats, centroids;
		int numOfLables = connectedComponentsWithStats(img_mask1, img_labels,
			stats, centroids, 8, CV_32S);


		//�����ڽ� �׸���
		int max = -1, idx = 0;
		for (int j = 1; j < numOfLables; j++) {
			int area = stats.at<int>(j, CC_STAT_AREA);
			if (max < area)
			{
				max = area;
				idx = j;
			}
		}


		int left = stats.at<int>(idx, CC_STAT_LEFT);
		int top = stats.at<int>(idx, CC_STAT_TOP);
		int width = stats.at<int>(idx, CC_STAT_WIDTH);
		int height = stats.at<int>(idx, CC_STAT_HEIGHT);


		rectangle(ROI_image, Point(left, top), Point(left + width, top + height),
			Scalar(0, 0, 255), 1);


		imshow("binary", img_mask1);
		imshow("origin", ROI_image);
		if (waitKey(5) >= 0)
			break;

		
	}


	return 0;
}

Mat region_of_interest(Mat canny_image) // �����̹����� �ٰ��� �κ� ����
{
	int height = canny_image.size().height;
	int width = canny_image.size().width;
	Mat dst_image;

	Mat mask(canny_image.rows, canny_image.cols, CV_8UC3, Scalar(0,0,0));
	Point pts[4] = { Point(0,height/3+30),Point(0, 0), Point(width, 0), Point(width,height/3+30) };
	fillConvexPoly(mask, pts, 4, Scalar(255,255,255));
	bitwise_and(canny_image, mask, dst_image);
	return dst_image;
}