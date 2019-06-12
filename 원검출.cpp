#include "opencv2/opencv.hpp" 
#include <iostream>  
#include <string> 

using namespace cv;
using namespace std;



//Contour 영역 내에 텍스트 쓰기 
void setLabel(Mat& image, string str, vector<Point> contour)
{
	int fontface = FONT_HERSHEY_SIMPLEX;
	double scale = 0.5;
	int thickness = 1;
	int baseline = 0;

	Size text = getTextSize(str, fontface, scale, thickness, &baseline);
	Rect r = boundingRect(contour);

	Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	rectangle(image, pt + Point(0, baseline), pt + Point(text.width, -text.height), CV_RGB(200, 200, 200), FILLED);
	putText(image, str, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
}



int main(int, char**)
{
	Mat img_input, img_result, img_gray;

	//이미지파일을 로드하여 image에 저장  
	img_input = imread("road.jpg", IMREAD_COLOR);
	if (img_input.empty())
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}


	//그레이스케일 이미지로 변환  
	cvtColor(img_input, img_gray, COLOR_BGR2GRAY);

	//이진화 이미지로 변환
	Mat binary_image;
	threshold(img_gray, img_gray, 125, 255, THRESH_BINARY_INV | THRESH_OTSU);

	//contour를 찾는다.
	vector<vector<Point> > contours;
	findContours(img_gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

	//contour를 근사화한다.
	vector<Point2f> approx;
	img_result = img_input.clone();

	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

		if (fabs(contourArea(Mat(approx))) > 100)  //면적이 일정크기 이상이어야 한다. 
		{

			int size = approx.size();

			//Contour를 근사화한 직선을 그린다.
			if (size % 2 == 0) {
				line(img_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);

				for (int k = 0; k < size - 1; k++)
					line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);

				for (int k = 0; k < size; k++)
					circle(img_result, approx[k], 3, Scalar(0, 0, 255));
			}
			else {
				line(img_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);

				for (int k = 0; k < size - 1; k++)
					line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);

				for (int k = 0; k < size; k++)
					circle(img_result, approx[k], 3, Scalar(0, 0, 255));
			}



			//도형을 판정한다.
			if (size == 3)
				setLabel(img_result, "triangle", contours[i]); //삼각형

			//이하는 해당 꼭지점을 가진 convex라면 찾는 도형
			else if (size == 4 && isContourConvex(Mat(approx)))
				setLabel(img_result, "rectangle", contours[i]); //사각형

			else if (size == 5 && isContourConvex(Mat(approx)))
				setLabel(img_result, "pentagon", contours[i]); //오각형

			else if (size == 6 && isContourConvex(Mat(approx)))
				setLabel(img_result, "hexagon", contours[i]);  //육각형

			else if (size == 10 && isContourConvex(Mat(approx)))
				setLabel(img_result, "decagon", contours[i]);    //십각형

			//위 조건에 해당 안되는 경우는 찾아낸 꼭지점 갯수를 표시
			else setLabel(img_result, to_string(approx.size()), contours[i]);
		}

	}


	imshow("input", img_input);
	imshow("result", img_result);


	waitKey(0);


	return 0;
}

//#include"opencv2/opencv.hpp"
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
//using namespace cv;
//using namespace std;
//int main()
//{
//	//filename의 이미지를 , 컬러로 읽는다
//	Mat srcImage = imread("road.jpg", IMREAD_COLOR);
//
//	if (srcImage.empty()) { //해당 이미지 없으면 경고
//		printf(" Error opening image\n");
//		return -1;
//	}
//	Mat gray;   //Gray이미지를 담기위한 Mat 클래스 선언
//	//HoughCircle 알고리즘에는 Gray,8bit이미지 들어가야한다.
//	cvtColor(srcImage, gray, COLOR_BGR2GRAY);
//	//원 이미지 GrayScale로 변환한다.
//
//	int ksize = 7;/*커널의 사이즈 결정 - 커널=필터윈도우, 특정 지점의 주변을         어느만큼 크기로 필터링할 지 기본연산 단위 설정*/
//	Mat dstImage1, dstImage2; //2개의 필터를 각각 처리하기위해 2개의 Mat선언
//
//	//GaussianBlur(src, dstImage3, Size(ksize, ksize), 0.0);
//	/*가우시안블러 이미지의 노이즈를 제거하는 역할,영상 흐리게 만들 수 있다.
//	원 검출시 이미지가 뭉게질 수 있으므로 이미지를 더 분명하게 만들 수
//	있는 bilateralFilter사용이 좋을 수도 있다. 선택하면 된다.*/
//
//	double sigmaColor = 10.0;
//	double sigmaSpace = 10.0;
//	//bilateralFilter(srcImage, dstImage2, d, sigmaColor, sigmaSpace);
//
//	imshow("dstImage1", dstImage1);
//	imshow("dstImage2", dstImage2);
//
//	vector<Vec3f> circles; //circle정보를 담을 vector클래스 선언
//
//	/*처리할image,검출유형circles,method,minDist, dp,param1,param2,minRadius,MaxRadius*/
//	HoughCircles(gray, circles, HOUGH_GRADIENT, 2,
//		30, 200, 100, 1, 30);//맨 아래에 parameter설명
//
//
//		//검출한 원을 원본이미지에 표시
//		for (size_t i = 0; i < circles.size(); i++)
//		{
//			Vec3i c = circles[i];
//			Point center = Point(c[0], c[1]); // circle 중점
//			circle(srcImage, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
//			// circle outline
//			int radius = c[2];
//			circle(srcImage, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
//		}
//	imshow("detected circles", srcImage);
//	waitKey(0);
//	return 0;
//}
