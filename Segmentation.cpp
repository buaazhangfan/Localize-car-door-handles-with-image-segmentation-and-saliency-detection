//
// Created by apple on 2018/9/7.
//

#include <cstdio>
#include "util.h"


Mat segmentation(const Mat input, string save_address)
{
    Mat img_ori, img_gray, img_blur, img_thre, img_erode;

    img_ori = input;
    Mat background = input;
    imshow("Original Input Image", img_ori);
    waitKey(0);

    cvtColor(img_ori, img_gray, COLOR_BGR2GRAY);
    blur(img_gray, img_blur, Size(3,3));

    vector<Vec3f>circles;
    HoughCircles(img_blur, circles, HOUGH_GRADIENT, 1, 10, 200, 100, 0, 0);

    for (int i = 0; i < circles.size(); i++)
    {
        Point center (cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle (background, center, 3, Scalar(0, 255, 0), -1, 8, 0);
        circle (background, center, radius, Scalar(0, 255, 0), -1, 8, 0);
    }

    int highest_wheel = 0;

    if (circles.size() == 1)
    {
        highest_wheel = cvRound(circles[0][1]) - cvRound(circles[0][2]) - 20;
        cout << "Only one wheel is detected and the location is: " << highest_wheel << endl;
    }
    else if (circles.size() == 2)
    {
        cout << "Two wheels are detected" << endl;
        if (circles[0][1] >= circles[1][1])
            highest_wheel = cvRound(circles[0][1]) - cvRound(circles[0][2]) - 20;
        else
            highest_wheel = cvRound(circles[1][1]) - cvRound(circles[1][2]) - 20;
        cout<<"The highest location coordinate of wheels is "<<highest_wheel<<endl;

    }
    else
    {
        cout << "This image can't be detected" << endl;
        exit(0);
    }

    DrawLine(background, Point(0, highest_wheel), Point(background.cols, highest_wheel));

    int x, y;
    Mat image_nowheels = input;

    for (x = highest_wheel; x < background.rows; x++)
    {
        for (y = 0; y < background.cols; y++)
        {
            image_nowheels.at<Vec3b>(x,y)[0] = 0;
            image_nowheels.at<Vec3b>(x,y)[1] = 0;
            image_nowheels.at<Vec3b>(x,y)[2] = 0;
        }
    }

    Mat image_noedges = image_nowheels.clone();

    if(circles.size() == 1)
    {


        if(circles[0][0] < cvRound(0.5*background.cols))
        {
            cout<<"The detected wheel is left wheel"<<endl;
            int x0,y0,left_edge;
            left_edge = circles[0][0]-circles[0][2];
            for(x0 = 0; x0 < background.rows; x0++)
            {
                for(y0 = 0; y0 < left_edge; y0++)
                {
                    image_noedges.at<Vec3b>(x0,y0)[0] = 0;
                    image_noedges.at<Vec3b>(x0,y0)[1] = 0;
                    image_noedges.at<Vec3b>(x0,y0)[2] = 0;
                }
                for(y0 = background.cols-left_edge; y0 < background.cols; y0++)
                {
                    image_noedges.at<Vec3b>(x0,y0)[0] = 0;
                    image_noedges.at<Vec3b>(x0,y0)[1] = 0;
                    image_noedges.at<Vec3b>(x0,y0)[2] = 0;
                }
            }
        }
        else
        {
            cout<<"The detected wheel is right wheel"<<endl;
            int x0,y0,right_edge;
            right_edge = circles[0][0]+circles[0][2];
            for(x0 = 0; x0 < background.rows; x0++)
            {
                for(y0 =right_edge ; y0 < background.cols; y0++)
                {
                    image_noedges.at<Vec3b>(x0,y0)[0] = 0;
                    image_noedges.at<Vec3b>(x0,y0)[1] = 0;
                    image_noedges.at<Vec3b>(x0,y0)[2] = 0;
                }
                for(y0 = 0; y0 < background.cols-right_edge; y0++)
                {
                    image_noedges.at<Vec3b>(x0,y0)[0] = 0;
                    image_noedges.at<Vec3b>(x0,y0)[1] = 0;
                    image_noedges.at<Vec3b>(x0,y0)[2] = 0;
                }
            }

        }
    }

    else
    {
        int x0,y0,right_edge,left_edge;
        if(circles[0][0] < circles[1][0])
        {
            left_edge = circles[0][0]-circles[0][2];
            right_edge = circles[1][0]+circles[1][2];
        }
        else
        {
            left_edge = circles[1][0]-circles[1][2];
            right_edge = circles[0][0]+circles[0][2];
        }

        for(x0 = 0; x0 < background.rows; x0++)
        {
            for(y0 =right_edge ; y0 < background.cols; y0++)
            {
                image_noedges.at<Vec3b>(x0,y0)[0] = 0;
                image_noedges.at<Vec3b>(x0,y0)[1] = 0;
                image_noedges.at<Vec3b>(x0,y0)[2] = 0;
            }
            for(y0 = 0; y0 < left_edge; y0++)
            {
                image_noedges.at<Vec3b>(x0,y0)[0] = 0;
                image_noedges.at<Vec3b>(x0,y0)[1] = 0;
                image_noedges.at<Vec3b>(x0,y0)[2] = 0;
            }
        }



    }

    Mat image_nowheels_gray,image_nowheels_gray_contrast;
    Mat image_no_wheelglasses;
    image_no_wheelglasses = image_noedges.clone();
    cvtColor(image_nowheels, image_nowheels_gray, CV_BGR2GRAY);
    image_nowheels_gray_contrast = image_nowheels_gray;

    imshow("wheels",background);
    waitKey(0);

    imshow("no wheels",image_nowheels);
    waitKey(0);

    int num_row,num_col;
    double contrast;
    contrast = 1.5;
    for(num_row = 0;num_row <= image_nowheels_gray.rows; num_row++)
    {
        for(num_col = 0; num_col <= image_nowheels_gray.cols; num_col++ )
        {
            if(image_nowheels_gray.at<uchar>(num_row,num_col) == 0)
            {
                continue;
            }
            else
            {
                image_nowheels_gray_contrast.at<uchar>(num_row,num_col) = saturate_cast<uchar>( contrast * image_nowheels_gray.at<uchar>(num_row,num_col));
            }
        }
    }


    Mat image_sobel, image_thre;
    Sobel(image_nowheels_gray_contrast, image_sobel, CV_8U, 0, 1);
    threshold(image_sobel, image_thre, 240, 255, 0);
    vector<Vec2f> lines;
    HoughLines(image_thre, lines,1, CV_PI/180, 150);
    vector<Vec2f> glass;

    for(size_t i =0;i < lines.size();i++)
    {
        if(lines[i][1] < 1.5 || lines[i][1]>1.7)
        {
            lines[i][0] = 0;
            lines[i][1] = 0;
            continue;
        }
        else
        {

            float rho = lines[i][0],theta = lines[i][1];
            Point pt1,pt2;
            double a =cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            if(highest_wheel-y0 < 20 || highest_wheel-y0 > 100 )
            {
                lines[i][0] = 0;
                lines[i][1] = 0;
                continue;
            }
            else
            {

                pt1.x = cvRound(x0 + 1000*(-b));
                pt1.y = cvRound(y0 + 1000*a);
                pt2.x = cvRound(x0 - 10000*(-b));
                pt2.y = cvRound(y0 - 10000*a);
                line(image_nowheels, pt1, pt2, Scalar(55,100,196),1,LINE_AA);
            }

        }

    }

    for(size_t i =0;i < lines.size();i++)
    {
        if(lines[i][0] == 0 && lines[i][1] == 0)
        {
            continue;
        }
        else
        {
            float theta = lines[i][1],rho = lines[i][0];
            double a =cos(theta), b = sin(theta);
            int num2_row,num2_col;
            for(num2_row = 0; num2_row < image_nowheels.rows; num2_row++)
            {
                for(num2_col = 0; num2_col < image_nowheels.cols; num2_col++)
                {
                    if(num2_row <cvRound((-a/b)*num2_col + rho/b))
                    {
                        image_no_wheelglasses.at<Vec3b>(num2_row,num2_col)[0] = 0;
                        image_no_wheelglasses.at<Vec3b>(num2_row,num2_col)[1] = 0;
                        image_no_wheelglasses.at<Vec3b>(num2_row,num2_col)[2] = 0;
                    }
                    else
                    {
                        continue;
                    }
                }
            }
        }
    }
    imshow("sobel",image_sobel);
    waitKey(0);

    imshow("lines",image_nowheels);
    waitKey(0);

    imshow("no wheels and edges",image_noedges);
    waitKey(0);

    imshow("no everything", image_no_wheelglasses);
    imwrite(save_address, image_no_wheelglasses);
    waitKey(0);

    return image_no_wheelglasses;

}

