#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/utility.hpp>
#include <iostream>
#include <cstdlib>
using namespace cv;
using namespace std;

vector<vector<Point> > contours;
vector<Vec4i> hierarchy;
RNG rng(12345);
int lowThreshold = 60;
int buddingcell, roundcell = 0;

int main()
{
	Mat src = imread("C:/Users/Asus/Desktop/1c4-0079_c1.TIF",IMREAD_UNCHANGED);
	Mat srcgray;
	if (src.empty())
	{
		cout << "Cannot load image!" << endl;
		return -1;
	}
	resize(src, src, Size(), 0.75, 0.75);
	cvtColor(src, srcgray, 7);
	Mat src1;
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(20,20));
	morphologyEx(srcgray, src1, 2, kernel);
	//src1 = srcgray;
	imshow("morpOutput", src1);

	Canny(src1, src1, lowThreshold, lowThreshold*2, 3);

	Mat kernel2 = getStructuringElement(MORPH_ELLIPSE, Size(15,15));
	imshow("edge", src1);
	morphologyEx(src1, src1, 3, kernel2);

	//adaptiveThreshold(src1, src1, 255,ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,15,6);
	imshow("canvasOutput", src1);
	findContours(src1, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	for (size_t i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(256,256,256);
		drawContours(src1, contours, (int)i, color,FILLED, LINE_8, hierarchy, 2);
	}
	//Mat kernel3 = getStructuringElement(MORPH_ELLIPSE, Size(15, 15));
	//imshow("edge", src1);
	//morphologyEx(src1, src1, 3, kernel2);
	imshow("canvasOutput", src1);

	findContours(src1, contours,hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	for (size_t i = 0; i< contours.size(); i++)
	{
		if (contourArea(contours[i]) > 200)
		{
			Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
			drawContours(src, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
		}
	}

	char str[50];
	for (unsigned int i = 0; i < contours.size(); i++)
	{
		if(contourArea(contours[i]) > 200)
		{
			int arc = arcLength(contours[i], true);
			if (abs(arc*arc / (4 * contourArea(contours[i]))) > 4.5)
			{
				sprintf_s(str, "#%d area %d bud",
					i,
					int(contourArea(contours[i])));
				buddingcell++;
			}
			else
			{
				sprintf_s(str, "#%d area %d round",
					i,
					int(contourArea(contours[i])));
				roundcell++;
			}
			
				Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
				putText(src,
					str,
					Point(contours[i][contours[i].size()/2]),//Middle Point
					FONT_HERSHEY_COMPLEX_SMALL, 
					0.6,//scalar
					color,
					2);
	      }
     }
        
	}
	imshow("Contours", src);
	cout << "There are total " << buddingcell + roundcell << "number of yeast cell detected" << endl;
	cout << "BuddingCell: " << buddingcell << " RoundCell: " << roundcell << endl;
	waitKey(0);
	return 0;
}
