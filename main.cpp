#include <iostream>
#include "opencv2/opencv.hpp"
#include "util.h"



int main() {

    string inputpath = "/Users/apple/Desktop/Ist2017/1.jpg";

    string segpath = "/Users/apple/Desktop/Ist2017/seg.jpg";

    Mat input = imread(inputpath);

    Mat seg = segmentation(input, segpath);

    Mat sal = saliency(input, seg);

    vector<vector<Point>> contours;
    findContours(sal, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    drawContours(input, contours, -1, Scalar(0,0,255),3);

    imshow("result", input);
    waitKey(0);

}