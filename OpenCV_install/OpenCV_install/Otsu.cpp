#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//****Otsu****
int OtsuAlgThreshold(const Mat image);

int main(int argc, char *argv[]) {
	Mat image = imread("Otsu.jpg");
	imshow("SouceImage", image);
	cvtColor(image, image, COLOR_RGB2GRAY);
	Mat imageOut, imageOtsu;
	int thresholdValue = OtsuAlgThreshold(image);
	cout << "��䷽��g=w0*w1��u0-u1��^2 = " << thresholdValue << endl;
	threshold(image, imageOut, thresholdValue, 255, THRESH_BINARY);
	threshold(image, imageOtsu, 0, 255, THRESH_OTSU);

	imshow("imageOut", imageOut);
	imshow("imageOtsu", imageOtsu);

	waitKey();
	return 0;
}


int OtsuAlgThreshold(const Mat image) {
	if (image.channels() != 1) {
		cout << "Please input Gray-image!" << endl;
		return 0;
	}
	int T = 0; //Otsu�㷨��ֵ
	double varValue = 0;//�м���ֵ�ݴ�
	double w0 = 0, w1 = 0; //ǰ�������ص�����ռ����
	double u0 = 0, u1 = 0; //ǰ����ƽ���Ҷ�

	double Histogram[256] = { 0 }; // �Ҷ�ֱ��ͼ��������Ҷ�ֵ��Ӧ�����ص�����
	int Histogram1[256] = { 0 };
	uchar *data = image.data;
	double totalNum = image.rows * image.cols;

	//*****����ֱ��ͼ�ֲ�
	for(int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++) {
			Histogram[data[i * image.step + j]]++;
			Histogram1[data[i * image.step + j]]++;
		}

	//*****����ֱ��ͼ
	Mat image1(255, 255, CV_8UC3); //8bit U�޷������� C3ͼ��4ͨ��
	for (int i = 0; i < 255; i++) {
		Histogram1[i] = Histogram1[i] % 200;
		line(image1, Point(i, 235), Point(i, 235 - Histogram1[i]), Scalar(255, 0, 0));
		if (i % 50 == 0) {
			char ch[255];
			sprintf(ch, "%d", i);
			string str = ch;
			putText(image1, str, Point(i, 250), 1, 1, Scalar(0, 0, 255));
		}

	}
	//*****����ֱ��ͼ

	for (int i = 0; i < 255; i++) {
		w1 = w0 = u1 = u0 = 0; //��ʼ��
		//�������ָ�ֵ����
		for (int j = 0; j <= i; j++) { 
			w1 += Histogram[j];  //�����������ص��ܺ�
			u1 += j * Histogram[j];  //�������������ܻҶȺ�
		}
		if (w1 == 0) break;
		u1 = u1 / w1;
		w1 = w1 / totalNum;

		//ǰ�����ָ�ֵ����
		for (int k = i + 1; k < 255; k++) {
			w0 += Histogram[k];
			u0 += k * Histogram[k];
		}
		if (w0 == 0) break;
		u0 = u0 / w0;
		w0 = w0 / totalNum;

		//��䷽�����
		double varValue1 = w0 * w1 * (u1 - u0) * (u1 - u0);
		if (varValue1 > varValue) {
			varValue = varValue1;
			T = i;
		}
	}

	//����T��ֵ�ķָ���
	line(image1, Point(T, 235), Point(T, 0), Scalar(0, 0, 255), 2);
	imshow("result", image1);
	return T;
}
