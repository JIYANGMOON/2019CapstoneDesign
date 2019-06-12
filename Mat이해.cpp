#include<iostream>
#include"opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main()
{
	Mat A(2, 3, CV_32FC1, Scalar(0)); // Mat 클래스의 생성자를 이용해 CV_8UC1 자료형의 2x3행렬 A를 생성하고 0으로 초기화한다.
	cout << "A = " << A << endl;

	A.create(2, 3, CV_32FC1); // Mat::create() 메서드로 생성하려는 행렬의 크기 및 자료형이 기존의 행렬 A와 동일하므로 새로 데이터를 위한 메모리를 할당하지 않는다.
	cout << "A = " << A << endl;

	A.create(3, 3, CV_32FC1); // 기존의 행렬과 크기가 다르므로 자동으로 이전에 할당된 메모리를 해제하고 메모리를 재할당한다. 
	cout << "A = " << A << endl; // 초기화 되어있지않아서 쓰레기값 출력.

	A.create(Size(3, 3), CV_8UC1); // 기존의 행렬과 자료형이 다르므로 자동으로 메모리를 해제하고 메모리를 재할당.
	cout << "A = " << A << endl; // 초기화 되지않아서 쓰레기값 출력

	Mat B;
	int sizes[] = { 3,3 }; // 2차원
	B.create(2, sizes, CV_8UC1); // 자료형이 CV_8UC1 이고 크기가 3x3인 행렬  B 생성.
	cout << "B = " << B << endl;
	return 0;
}