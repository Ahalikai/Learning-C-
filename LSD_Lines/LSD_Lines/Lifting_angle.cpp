#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include <direct.h>
#include <io.h>
#include <string>

using namespace cv;
using namespace std;


void isDirExist(const char* dir); //判断文件夹是否存在，不存在则创建
void LSD_angle(const char* image_dir, const char* save_dir);


int main() {
	const char* image_dir = "D:/pycharm/yolov7-main/Task_img/Test/box/*.jpg";
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

void LSD_angle(const char* image_dir, const char* save_dir) {
	isDirExist(save_dir);
	Mat temp;


	Mat image = imread("D:/pycharm/yolov7-main/Task_img/Test/box/237.jpg");
	Mat grayImage;
	cvtColor(image, grayImage, COLOR_BGR2GRAY);
	// Create and LSD detector with standard or no refinement.
	Canny(grayImage, grayImage, 50, 200, 3); // Apply canny edge//可选canny算子
#if 1
	Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_ADV);//或者两种LSD算法，这边用的是standard的
#else
	Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_NONE);
#endif
	vector<Vec4f> lines_std;
	// Detect the lines
	ls->detect(grayImage, lines_std);//这里把检测到的直线线段都存入了lines_std中，4个float的值，分别为起止点的坐标
	/*   
	for (int i = 0; i < lines_std.size(); i++)
	{
		cout << lines_std[i][0] << " " << lines_std[i][1] << " "
			<< lines_std[i][2] << " " << lines_std[i][3] << endl;
	}
	*/  //检测点的坐标输出

	// Show found lines
	Mat drawnLines(image);
	ls->drawSegments(drawnLines, lines_std);
	imshow("Standard refinement", drawnLines);
	waitKey(0);

	return;
}