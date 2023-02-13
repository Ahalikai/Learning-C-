#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include <direct.h>
#include <io.h>
#include <string>

using namespace cv;
using namespace std;


void isDirExist(const char* dir); //�ж��ļ����Ƿ���ڣ��������򴴽�
void LSD_angle(const char* image_dir, const char* save_dir);


int main() {
	const char* image_dir = "D:/pycharm/yolov7-main/Task_img/Test/box/*.jpg";
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

void LSD_angle(const char* image_dir, const char* save_dir) {
	isDirExist(save_dir);
	Mat temp;


	Mat image = imread("D:/pycharm/yolov7-main/Task_img/Test/box/237.jpg");
	Mat grayImage;
	cvtColor(image, grayImage, COLOR_BGR2GRAY);
	// Create and LSD detector with standard or no refinement.
	Canny(grayImage, grayImage, 50, 200, 3); // Apply canny edge//��ѡcanny����
#if 1
	Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_ADV);//��������LSD�㷨������õ���standard��
#else
	Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_NONE);
#endif
	vector<Vec4f> lines_std;
	// Detect the lines
	ls->detect(grayImage, lines_std);//����Ѽ�⵽��ֱ���߶ζ�������lines_std�У�4��float��ֵ���ֱ�Ϊ��ֹ�������
	/*   
	for (int i = 0; i < lines_std.size(); i++)
	{
		cout << lines_std[i][0] << " " << lines_std[i][1] << " "
			<< lines_std[i][2] << " " << lines_std[i][3] << endl;
	}
	*/  //������������

	// Show found lines
	Mat drawnLines(image);
	ls->drawSegments(drawnLines, lines_std);
	imshow("Standard refinement", drawnLines);
	waitKey(0);

	return;
}