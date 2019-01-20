//
// Created by apple on 2018/9/7.
//

#ifndef IST2017_UTIL_H
#define IST2017_UTIL_H

#endif //IST2017_UTIL_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstdio>



using namespace cv;
using namespace std;

void DrawLine (Mat img, Point start, Point end);


Mat saliency (Mat input, Mat seg);

Mat segmentation(const Mat input, string save_address);

