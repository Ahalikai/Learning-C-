#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int hello()

{
	Mat img = imread("E:\\证件照\\头像.jpg");
	namedWindow("测试");
	imshow("测试", img);
	waitKey(0);
	return(0);

}