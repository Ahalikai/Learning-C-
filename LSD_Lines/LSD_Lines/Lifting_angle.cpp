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

//ȫ�ֲ�������
ofstream outFile;
int line_minX = 10; //���ֱ�ߣ�Ϊ��ǰͼ���ȵ�1/10
float angle_max = 5.0; //ֱ��ɸѡ��������angle_max
float line_where = 2.0; //ֱ��ɸѡ����ͼ����ϰ벿�֣�Eq��2.0Ϊһ�룬3.0Ϊ����֮һ


//�ж��ļ����Ƿ���ڣ��������򴴽�
void isDirExist(const char* dir); 

//����λ�˼��������
//image_dir������ͼ���ļ�·����ͼ���ļ�����ʽΪ '%05d' 000XX.jpg������󣬿ɰ��ļ���˳�����ͼ��
//save_dir�� ��������ļ�·�������result.txt���ļ��л�txt�����ڣ����Զ����ɡ�
void LSD_angle(const char* image_dir, const char* save_dir);

//����ͼ��ĵ���λ�˼��
void LSD_img(Mat image);

//����ֱ�ߵĽǶȼ���
float cal_angle(float x1, float y1, float x2, float y2); 


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

//����ֱ�ߵĽǶȼ���
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
	int lines_num = lines_std.size();

	float x_size = image.cols / line_minX; //��̳���
	float angle_result = 0.0;
	int angle_num = 0;

	for (int i = 0; i < lines_num; i++) {
		float angle = cal_angle(lines_std[i][0], lines_std[i][1]
			, lines_std[i][2], lines_std[i][3]);

		float lines_x = abs(lines_std[i][2] - lines_std[i][0]); //ֱ��x����
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
		
	outFile.close();//��txt�ر�
	cout << "Ahalk������λ�˽Ƕ��Ѽ����ϣ������Ѵ��� -> " << save_txt << endl;
	return;
}