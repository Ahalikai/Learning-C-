#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include <direct.h>
#include <io.h>
#include <string>
#include <algorithm>
#include <fstream>


using namespace cv;
using namespace std;

//全局参数变量
ofstream outFile;
int line_minX = 10; //最短直线，为当前图像宽度的1/10
float angle_max = 5.0; //直线筛选，不超过angle_max
float line_where = 2.0; //直线筛选，在图像的上半部分，Eq：2.0为一半，3.0为三分之一


//判断文件夹是否存在，不存在则创建
void isDirExist(const char* dir); 

//吊具位姿检测主函数
//image_dir：输入图像文件路径，图像文件名格式为 '%05d' 000XX.jpg。补零后，可按文件名顺序遍历图像
//save_dir： 结果保存文件路径，输出result.txt。文件夹或txt不存在，可自动生成。
void LSD_angle(const char* image_dir, const char* save_dir);

//单张图像的吊具位姿检测
void LSD_img(Mat image);

//单根直线的角度计算
float cal_angle(float x1, float y1, float x2, float y2); 


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

//单根直线的角度计算
float cal_angle(float x1, float y1, float x2, float y2){
	if (abs(x2 - x1) < 0.001) return 90.0;
	else if (abs(y2 - y1) < 0.001) return 0.0;
	else {
		float k = -(y2 - y1) / (x2 - x1);
		float result = atan(k) * 180 / 3.1415;
		return result;
	}
}


void LSD_img(Mat image) {
	//Mat image = imread(img_str);
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
	int lines_num = lines_std.size();

	float x_size = image.cols / line_minX; //最短长度
	float angle_result = 0.0;
	int angle_num = 0;

	for (int i = 0; i < lines_num; i++) {
		float angle = cal_angle(lines_std[i][0], lines_std[i][1]
			, lines_std[i][2], lines_std[i][3]);

		float lines_x = abs(lines_std[i][2] - lines_std[i][0]); //直线x长度
		if (abs(angle) < angle_max && lines_x > x_size && lines_std[i][1] < (image.rows / line_where)) {
			//cout << "angle: " << angle << endl;
			angle_num++;
			angle_result += angle;
		}
		else {
			lines_std[i][0] = lines_std[i][1] = lines_std[i][2] = lines_std[i][3] = 0;
		}
	}

	if (angle_num > 0) {
		angle_result /= angle_num;
		outFile << angle_result << endl;
		//cout << img_str << " ";
		cout << angle_result << endl;
	}
	
	// Show found lines
	Mat drawnLines(image);
	ls->drawSegments(drawnLines, lines_std);
	imshow("Standard refinement", drawnLines);
	waitKey(1);
}

void LSD_angle(const char* image_dir, const char* save_dir) {
	isDirExist(save_dir);
	
	String save_txt = save_dir;
	save_txt = save_txt + "result1.txt";
	cout << "save_txt:  " << save_txt << endl;
	outFile.open(save_txt);
	Mat temp;

	vector<cv::String> fn;
	glob(image_dir, fn, false);

	int img_num = fn.size();
	for (int i = 0; i < img_num; i++) {
		//cout << fn[i] << endl;
		Mat image = imread(fn[i]);
		LSD_img(image);
	}
		
	outFile.close();//将txt关闭
	cout << "Ahalk：吊具位姿角度已检测完毕，数据已存入 -> " << save_txt << endl;
	return;
}