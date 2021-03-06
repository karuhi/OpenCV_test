// OpenCV_test.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

// リンカーの入力を設定
// デバッグとリリースで入力するファイルが異なるのでこのようにしています。
#ifdef _DEBUG
#define CV_EXT "d.lib"
#else
#define CV_EXT ".lib"
#endif

#pragma comment(lib, "opencv_world341" CV_EXT)

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <chrono>

#ifdef _OEPNMP
#include <omp.h>
#endif

using namespace cv;
using namespace std;


int main(int argc, char *argv[]) {
	String imageName("../data/354.jpg");

	if (argc > 1)
	{
		imageName = argv[1];
		std::cout << "画像を読み込みました！" << endl;
	}
	cv::Mat image = cv::imread(imageName, IMREAD_COLOR);
	if (image.empty())
	{
		cout << "画像が読み込めませんでした！" << endl;
		return -1;
	}
	cv::Mat3b img_bgr = cv::imread(imageName);
	
	const int max_image_pics = img_bgr.rows*img_bgr.cols;

	int     max1 = img_bgr.rows; //r 行数
	int     max2 = img_bgr.cols; //c 列数

	cout << "rows: " << max1 << endl; //行数
	cout << "cols: " << max2 << endl; //列数
	cout << "total: " << max_image_pics << endl; //合計数


	std::vector<uchar>bri_vec(max_image_pics);

	Mat gray = Mat(Size(max2, max1), CV_8UC1);

	// aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

	std::chrono::system_clock::time_point  start, end; // 型は auto で可
	start = std::chrono::system_clock::now(); // 計測開始時間
	for (int n = 0; n < 1000; n++) {
		for (int r = 0; r < max1; r++) {
			for (int c = 0; c < max2; c++) {

				cv::Vec3b *p = img_bgr.ptr<cv::Vec3b>(r,c); //r行目の先頭画素のポインタを取得
				uchar blue = (*p)(0); //blueを取得
				uchar green = (*p)(1); //greenを取得
				uchar red = (*p)(2); //redを取得

				uchar brightness = (0.299*red) + (0.587*green) + (0.114*blue); //変換公式を使って カラー -> グレースケール

				gray.at<uchar>(r, c) = brightness;

			}
		}
	}

	end = std::chrono::system_clock::now();  // 計測終了時間
	double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); //処理に要した時間をミリ秒に変換

	cout << elapsed << endl;

	//bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb

	std::chrono::system_clock::time_point  start2, end2; // 型は auto で可
	start2 = std::chrono::system_clock::now(); // 計測開始時間
	int num_el = max1 * max2;
	int bgr_len = num_el * img_bgr.elemSize1();

	for (int n = 0; n < 1000; n++) {
		uchar *q = gray.data;
		uchar *p = img_bgr.data;
		for (int i = 0; i < bgr_len;i+=3) {
			*q++ = (0.299 * *(p + 2)) + (0.587 * *(p + 1)) + (0.114 * (*p));
			p += 3;
		}
	}

	end2 = std::chrono::system_clock::now();  // 計測終了時間
	double elapsed2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count(); //処理に要した時間をミリ秒に変換

	cout << elapsed2;

	uchar threshold_value = (255 / 2);
	int threshold_total;
	for (int i = 0; i < max1; i++) {
		for (int j = 0; j < max2; j++) {
			threshold_total += gray.at<int>(i, j);
		}
	}

	Mat threshold_image = Mat(Size(max2, max1), CV_8UC1);
	uchar thres;
	uchar thres_val;
	threshold_value = (threshold_total / (max1*max2));
	for (int x = 0; x < max1; x++) {
		for (int y = 0; y < max2; y++) {
			thres = gray.at<uchar>(x, y);
			if (thres < threshold_value) {
				thres_val = 0;
			} else if (thres > threshold_value) {
				thres_val = 255;
			}
			threshold_image.at<uchar>(x, y) = thres_val;
		}
	}
	
	/*cv::namedWindow("Display", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("Display", image);*/
	cv::namedWindow("Display gray", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("Display gray", gray); 
	/*cv::namedWindow("Display 2color", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("Display 2color", threshold_image);*/
	waitKey(0);
	return 0;

}