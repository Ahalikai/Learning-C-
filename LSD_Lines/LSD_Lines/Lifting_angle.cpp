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


void isDirExist(const char* dir); //�ж��ļ����Ƿ���ڣ��������򴴽�
void LSD_angle(const char* image_dir, const char* save_dir);
// ͼ���ļ�����ʽ '%05d' 000XX.jpg������󣬿ɰ��ļ���˳�����ͼ��
void LSD_img(const String);
float cal_angle(float x1, float y1, float x2, float y2); //����Ƕ�

int main() {
	const char* image_dir = "D:/pycharm/yolov7-main/Task_img/Test/box1/*.jpg";
	const char* save_dir = "D:/pycharm/yolov7-main/Task_img/Test/result_2/";

	LSD_angle(image_dir, save_dir);

	return 0;
}

//�ж��ļ����Ƿ���ڣ��������򴴽�
void isDirExist(const char* dir) {
	if (_access(dir, 0) == -1){
		_mkdir(dir);
		cout << "���ļ��в����ڣ����Զ�����:" << dir << endl;
	}
	else
		cout << "�ļ����Ѿ�����" << endl;
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
	Canny(grayImage, grayImage, 50, 200, 3); // Apply canny edge//��ѡcanny����
#if 1
	Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_ADV);//��������LSD�㷨������õ���standard��
#else
	Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_NONE);
#endif
	vector<Vec4f> lines_std, lines_new;
	// Detect the lines
	ls->detect(grayImage, lines_std);//����Ѽ�⵽��ֱ���߶ζ�������lines_std�У�4��float��ֵ���ֱ�Ϊ��ֹ�������
	/*
	for (int i = 0; i < lines_std.size(); i++)
	{
		cout << lines_std[i][0] << " " << lines_std[i][1] << " "
			<< lines_std[i][2] << " " << lines_std[i][3] << endl;
	}
	*/  //������������
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