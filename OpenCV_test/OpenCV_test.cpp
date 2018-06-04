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

using namespace cv;
using namespace std;

// "ab12" のような16進数文字列を整数値に変換
// '0'～'9', 'A'～'F', 'a'～'f' 以外の文字は無視する
// オーバーフローチェックは行わない
unsigned int hexToUInt(const uchar *ptr)
{
	unsigned int val = 0;
	unsigned int v;
	char ch;
	while ((ch = *ptr++) != '\0') {
		if (ch >= '0' && ch <= '9')
			v = ch - '0';
		else if (ch >= 'A' && ch <= 'F')
			v = ch - 'A' + 10;
		else if (ch >= 'a' && ch <= 'f')
			v = ch - 'a' + 10;
		else
			continue;    // 16進数文字ではない場合
		val = val * 16 + v;
	}
	return val;
}

int main(int argc, char *argv[]) {

	String imageName("../data/354.jpg");

	if (argc > 1)
	{
		imageName = argv[1];
		cout << "画像を読み込みました！" << endl;
	}
	cv::Mat image = cv::imread(imageName, IMREAD_COLOR);
	cv::Mat3b img_bgr = cv::imread(imageName);
	
	const int max_image_pics = img_bgr.rows*img_bgr.cols;

	int     max1 = img_bgr.rows; //r 行数
	int     max2 = img_bgr.cols; //c 列数

	cout << "rows: " << max1 << endl; //行数
	cout << "cols: " << max2 << endl; //列数
	cout << "total: " << max_image_pics << endl; //合計数


	std::vector<uchar>bri_vec(max_image_pics);

	for (int r = 0; r < img_bgr.rows; ++r) {
		for (int c = 0; c < img_bgr.cols; ++c) {

			cv::Vec3b *p = image.ptr<cv::Vec3b>(r); //r行目の先頭画素のポインタを取得
			p[c]; //j行目を指定
			int blue = (*p)(0); //blueを取得
			int green = (*p)(1); //greenを取得
			int red = (*p)(2); //redを取得

			int brightness = (0.299*red) + (0.587*green) + (0.114*blue); //変換公式を使って カラー -> モノクロ

			//cout << "bri_vec at";
			bri_vec.at(r * max2 + c) = brightness;

			//cout << "B--" << blue << "--G--" << green << "--R--" << red << "---" << brightness << "---" << "X--" <<c << "Y--" << r <<endl;

		}
	}
	cout << bri_vec.data() << endl;
	if (bri_vec.size() == max_image_pics) {
		Mat two_color_vec = Mat(max1, max2, CV_8UC1); //init vec
		memcpy(two_color_vec.data, bri_vec.data(), bri_vec.size() * sizeof(uchar)); //変換
	}
	cout << bri_vec.data() << endl;
	uchar hexaa = hexToUInt(bri_vec.data());
	if (image.empty())
	{
		cout << "画像が読み込めませんでした！" << endl;
		return -1;
	}
	
	cv::namedWindow("Display", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("Display", image);
	cv::namedWindow("Display 2color", CV_WINDOW_NORMAL);
	cv::imshow("Display 2color", bri_vec);
	waitKey(0);
	return 0;


}