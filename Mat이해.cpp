#include<iostream>
#include"opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main()
{
	Mat A(2, 3, CV_32FC1, Scalar(0)); // Mat Ŭ������ �����ڸ� �̿��� CV_8UC1 �ڷ����� 2x3��� A�� �����ϰ� 0���� �ʱ�ȭ�Ѵ�.
	cout << "A = " << A << endl;

	A.create(2, 3, CV_32FC1); // Mat::create() �޼���� �����Ϸ��� ����� ũ�� �� �ڷ����� ������ ��� A�� �����ϹǷ� ���� �����͸� ���� �޸𸮸� �Ҵ����� �ʴ´�.
	cout << "A = " << A << endl;

	A.create(3, 3, CV_32FC1); // ������ ��İ� ũ�Ⱑ �ٸ��Ƿ� �ڵ����� ������ �Ҵ�� �޸𸮸� �����ϰ� �޸𸮸� ���Ҵ��Ѵ�. 
	cout << "A = " << A << endl; // �ʱ�ȭ �Ǿ������ʾƼ� �����Ⱚ ���.

	A.create(Size(3, 3), CV_8UC1); // ������ ��İ� �ڷ����� �ٸ��Ƿ� �ڵ����� �޸𸮸� �����ϰ� �޸𸮸� ���Ҵ�.
	cout << "A = " << A << endl; // �ʱ�ȭ �����ʾƼ� �����Ⱚ ���

	Mat B;
	int sizes[] = { 3,3 }; // 2����
	B.create(2, sizes, CV_8UC1); // �ڷ����� CV_8UC1 �̰� ũ�Ⱑ 3x3�� ���  B ����.
	cout << "B = " << B << endl;
	return 0;
}