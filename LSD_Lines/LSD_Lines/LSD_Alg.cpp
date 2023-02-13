#include<iostream>
#include<opencv.hpp>
using namespace cv;
using namespace std;
int H_main()
{
	cout << "hello world" << endl;
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
	double start = double(getTickCount());
	vector<Vec4f> lines_std;
	// Detect the lines
	ls->detect(grayImage, lines_std);//����Ѽ�⵽��ֱ���߶ζ�������lines_std�У�4��float��ֵ���ֱ�Ϊ��ֹ�������
	for (int i = 0; i < lines_std.size(); i++)
	{
		cout << lines_std[i][0] << " " << lines_std[i][1] << " "
			<< lines_std[i][2] << " " << lines_std[i][3] << endl;
	}
	double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
	std::cout << "It took " << duration_ms << " ms." << std::endl;
	// Show found lines
	Mat drawnLines(image);
	ls->drawSegments(drawnLines, lines_std);
	imshow("Standard refinement", drawnLines);
	waitKey(0);
	return 0;
}
