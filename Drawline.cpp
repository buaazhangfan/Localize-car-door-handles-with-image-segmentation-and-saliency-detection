//
// Created by apple on 2018/9/7.
//
#include <iostream>
#include <opencv2/opencv.hpp>


using namespace cv;

void DrawLine (Mat img, Point start, Point end)
{
    int thickness = 3;
    int linetype = 8;
    line (img, start, end, Scalar(255, 0, 0), thickness, linetype);
}

