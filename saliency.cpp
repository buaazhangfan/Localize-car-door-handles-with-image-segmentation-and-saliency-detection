//
// Created by apple on 2018/9/11.
//

#include "util.h"



Mat saliency (Mat input, Mat seg)
{
    Mat src,inputRe,inputIm,Fourier,Inverse,magnitude,LOGmagnitude;
    Mat Saliency, Seg;
    Mat temp1,temp2,temp3;
    src = seg;
    cvtColor(src, src, COLOR_BGR2GRAY);
    Mat image;
    image = input;
    int m = getOptimalDFTSize(src.rows);
    int n = getOptimalDFTSize(src.cols);
    Mat padded;

    copyMakeBorder(src, padded, 0, m-src.rows, 0, n-src.cols, BORDER_CONSTANT);

    Mat planes[] = {Mat_<double>(padded),Mat::zeros(padded.size(), CV_64F)};
    Mat complexI;
    merge(planes,2,complexI);
    dft(complexI, complexI);
    split(complexI, planes);
    inputRe = planes[0];
    inputIm = planes[1];
    cv:: magnitude(planes[0], planes[1], magnitude);
    log(magnitude, LOGmagnitude);
    Mat average;
    blur(LOGmagnitude, average, Size(3,3));
    Mat sub;
    sub = LOGmagnitude - average;
    Mat sin = Mat::zeros(padded.size(), CV_64F);
    Mat cos = Mat::zeros(padded.size(), CV_64F);
    sin = planes[1]/magnitude;
    cos = planes[0]/magnitude;
    exp(sub,sub);
    multiply(sub, cos, temp1);
    multiply(sub, sin, temp2);
    planes[0] = temp1;
    planes[1] = temp2;
    merge(planes, 2,complexI);
    dft(complexI, Inverse,DFT_INVERSE);
    split(Inverse, planes);
    Mat  outputRE,outputIM;
    outputRE = planes[0];
    outputIM = planes[1];

    cv::magnitude(outputRE, outputIM, Saliency);

    GaussianBlur(Saliency, Saliency,Size(31,31),0,0);

    normalize(Saliency, Saliency, 0, 1,NORM_MINMAX);

    Mat saliency_result;
    Saliency.convertTo(saliency_result, CV_8U,255,0);

    threshold(saliency_result, saliency_result, 100, 255, 0);
    imshow("1",saliency_result);

    Mat element = getStructuringElement(MORPH_RECT, Size(15,15));

    erode(saliency_result, saliency_result, element);

    return saliency_result;
    vector<vector<Point>> contours;
    findContours(saliency_result, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    drawContours(image, contours, -1, Scalar(0,0,255),3);

    waitKey(0);
}