#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

#define PI 3.1415926535
using namespace std;
using namespace cv;


// 计算当前点的LBP特征
int calcPointLBP(Mat src, int radius, int neighbors)
{
	int result = 0;

	int center_value = src.at<uchar>(radius, radius);

	double x, y;
	int x1, x2, y1, y2;
	double w1, w2, w3, w4;
	for (int i = 0; i < neighbors; i++)
	{
		// 取当前角度对应圆上的灰度值（双线性插值）
		result = result << 1;
		x = radius + radius * cos(2 * PI * i / neighbors);
		y = radius + radius * sin(2 * PI * i / neighbors);

		x1 = floor(x);
		x2 = x1 + 1;
		y1 = floor(y);
		y2 = y1 + 1;

		w1 = (x2 - x) * (y2 - y);
		w2 = (x - x1) * (y2 - y);
		w3 = (x2 - x) * (y - y1);
		w4 = (x - x1) * (y - y1);

		if (x2 > 2 * radius)	x2 = 2 * radius;
		if (y2 > 2 * radius)	y2 = 2 * radius;

		double value = w1 * src.at<uchar>(x1, y1) + w2 * src.at<uchar>(x1, y2) + w3 * src.at<uchar>(x2, y1) + w4 * src.at<uchar>(x2, y2);

		result += ((value - center_value) > -5);
	}
	result = result * 256.0 / (1 << neighbors);	// 计算二进制数并归一化到0-255
	return result;
}

// 计算最小LBP（旋转不变LBP）
int getMinLBP(int LBP_value, int length)
{
	unsigned int LBP_value_temp = LBP_value;
	int result = 1 << length;
	for (int i = 0; i < length; i++)
	{
		LBP_value_temp = (LBP_value >> (length - i)) | (LBP_value_temp << i);	// 循环左移
		if (LBP_value_temp < result)	// 保存最小值
			result = LBP_value_temp;
	}
	return result;
}

// 计算LBP特征图
void getLBPFeature(Mat src, Mat &dst, int radius, int neighbors, bool isMinLBP=true)
{
	dst = Mat::zeros(src.rows, src.cols, CV_8UC1);
	for (int i = radius; i < src.rows - radius; i++)
	{
		for (int j = radius; j < src.cols - radius; j++)
		{
			int LBP_value = calcPointLBP(src(Rect(j - radius, i - radius, 2 * radius + 1, 2 * radius + 1)), radius, neighbors);
			if(isMinLBP)
				LBP_value = getMinLBP(LBP_value, neighbors);
			LBP_value = LBP_value * 256.0 / (1 << neighbors);
			dst.at<uchar>(i, j) = LBP_value;
		}
	}
	double maxValue;
	minMaxLoc(dst, 0, &maxValue);		// 归一化
	dst = dst * 256.0 / maxValue;
	return;
}

int main()
{
	Mat image, LBP_image, LBP_image2;
	image = imread("1.jpeg", IMREAD_GRAYSCALE);
	getLBPFeature(image, LBP_image, 5, 12, true);
	getLBPFeature(image, LBP_image2, 5, 12, false);

	namedWindow("origin", WINDOW_FREERATIO);
	namedWindow("CircleLBP", WINDOW_FREERATIO);
	namedWindow("CircleLBP_min", WINDOW_FREERATIO);
	imshow("origin", image);
	imshow("CircleLBP", LBP_image);
	imshow("CircleLBP_min", LBP_image2);
	waitKey(0);
	imwrite("1_LBP.jpg", LBP_image);
	imwrite("1_LBPMin.jpg", LBP_image2);
	destroyAllWindows();
	system("Pause");
	return 0;
}