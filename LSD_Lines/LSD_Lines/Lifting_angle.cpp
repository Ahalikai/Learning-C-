#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include <direct.h>
#include <io.h>
#include <string>
#include <algorithm>

using namespace cv;
using namespace std;


void isDirExist(const char* dir); //判断文件夹是否存在，不存在则创建
void LSD_angle(const char* image_dir, const char* save_dir);
// 图像文件名格式 '%05d' 000XX.jpg。补零后，可按文件名顺序遍历图像
void LSD_img(const String);
float cal_angle(float x1, float y1, float x2, float y2); //计算角度

int main() {
	const char* image_dir = "D:/pycharm/yolov7-main/Task_img/Test/box1/*.jpg";
	const char* save_dir = "D:/pycharm/yolov7-main/Task_img/Test/result_2/";

	LSD_angle(image_dir, save_dir);

	return 0;
}

//判断文件夹是否存在，不存在则创建
void isDirExist(const char* dir) {
	if (_access(dir, 0) == -1){
		_mkdir(dir);
		cout << "该文件夹不存在，已自动创建:" << dir << endl;
	}
	else
		cout << "文件夹已经存在" << endl;
}

float cal_angle(float x1, float y1, float x2, float y2){
	if (abs(x2 - x1) < 0.001) return 90.0;
	else if (abs(y2 - y1) < 0.001) return 0.0;
	else {
		float k = -(y2 - y1) / (x2 - x1);
		float result = atan(k) * 180 / 3.1415;
		return result;
	}
}

void LSD_img(const String img_str) {
	Mat image = imread(img_str);
	Mat grayImage;
	cvtColor(image, grayImage, COLOR_BGR2GRAY);
	// Create and LSD detector with standard or no refinement.
	Canny(grayImage, grayImage, 50, 200, 3); // Apply canny edge//可选canny算子
#if 1
	Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_ADV);//或者两种LSD算法，这边用的是standard的
#else
	Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_NONE);
#endif
	vector<Vec4f> lines_std, lines_new;
	// Detect the lines
	ls->detect(grayImage, lines_std);//这里把检测到的直线线段都存入了lines_std中，4个float的值，分别为起止点的坐标
	/*
	for (int i = 0; i < lines_std.size(); i++)
	{
		cout << lines_std[i][0] << " " << lines_std[i][1] << " "
			<< lines_std[i][2] << " " << lines_std[i][3] << endl;
	}
	*/  //检测点的坐标输出
	int lines_num = lines_std.size();
	for (int i = 0; i < lines_num; i++) {
		float angle = cal_angle(lines_std[i][0], lines_std[i][1]
			, lines_std[i][2], lines_std[i][3]);

		if(abs(angle) < 5)
		cout << "angle: " << angle << endl;
	}

	// Show found lines
	Mat drawnLines(image);
	ls->drawSegments(drawnLines, lines_std);
	imshow("Standard refinement", drawnLines);
	waitKey(0);
}

void LSD_angle(const char* image_dir, const char* save_dir) {
	isDirExist(save_dir);
	Mat temp;

	vector<cv::String> fn;
	glob(image_dir, fn, false);

	int img_num = fn.size();
	for (int i = 0; i < 1; i++) {
		cout << fn[i] << endl;
		LSD_img(fn[i]);
	}
		
	//LSD_img("D:/pycharm/yolov7-main/Task_img/Test/box/237.jpg");
	return;
}