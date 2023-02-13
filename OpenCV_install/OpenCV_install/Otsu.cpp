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
	cout << "类间方差g=w0*w1（u0-u1）^2 = " << thresholdValue << endl;
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
	int T = 0; //Otsu算法阈值
	double varValue = 0;//中间阈值暂存
	double w0 = 0, w1 = 0; //前、后景像素点数所占比例
	double u0 = 0, u1 = 0; //前、后景平均灰度

	double Histogram[256] = { 0 }; // 灰度直方图，保存各灰度值对应的像素点总数
	int Histogram1[256] = { 0 };
	uchar *data = image.data;
	double totalNum = image.rows * image.cols;

	//*****计算直方图分布
	for(int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++) {
			Histogram[data[i * image.step + j]]++;
			Histogram1[data[i * image.step + j]]++;
		}

	//*****画出直方图
	Mat image1(255, 255, CV_8UC3); //8bit U无符号整形 C3图像4通道
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
	//*****画出直方图

	for (int i = 0; i < 255; i++) {
		w1 = w0 = u1 = u0 = 0; //初始化
		//背景部分各值计算
		for (int j = 0; j <= i; j++) { 
			w1 += Histogram[j];  //背景部分像素点总和
			u1 += j * Histogram[j];  //背景部分像素总灰度和
		}
		if (w1 == 0) break;
		u1 = u1 / w1;
		w1 = w1 / totalNum;

		//前景部分各值计算
		for (int k = i + 1; k < 255; k++) {
			w0 += Histogram[k];
			u0 += k * Histogram[k];
		}
		if (w0 == 0) break;
		u0 = u0 / w0;
		w0 = w0 / totalNum;

		//类间方差计算
		double varValue1 = w0 * w1 * (u1 - u0) * (u1 - u0);
		if (varValue1 > varValue) {
			varValue = varValue1;
			T = i;
		}
	}

	//画出T阈值的分割线
	line(image1, Point(T, 235), Point(T, 0), Scalar(0, 0, 255), 2);
	imshow("result", image1);
	return T;
}
