#include <opencv2/opencv.hpp>
// #include <opencv2/core/core.hpp>
// #include <opencv2/imgproc/imgproc.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/calib3d.hpp>
#include <iostream>
using namespace cv;
using namespace std;

void Demo(Mat &image){
    vector<Mat> mv;
    split(image, mv);

    double minv, maxv;
    Point minLoc, maxLoc;
    Mat meanv, stdv;

    for(int i=0;i<3;i++)
    {
        minMaxLoc(mv[i], &minv, &maxv, &minLoc, &maxLoc, Mat());
    }
    meanStdDev(image, meanv, stdv);


    Mat image_b, image_mixrb;
    mv[1] = 0; 
    mv[2] = 0;
    merge(image, image_b);

    Mat hsv,mask;
    cvtColor(image, hsv, COLOR_RGB2HSV);
    inRange(hsv,Scalar(35,43,46),Scalar(77,255,255),mask);
    bitwise_not(mask, mask);
    
    Mat redback =Mat::zeros(Size(400,400), CV_8UC3);
    redback = Scalar(40,40,200);
    image.copyTo(redback, mask); //only masked part of image will be copied to redback


    int from_to[] = {0, 2, 1, 1, 2, 0};
    mixChannels(&image, 1, &image_mixrb, 1, from_to, 3);

    Mat m1=Mat::zeros(Size(400,400), CV_8UC3);
    m1=Scalar(0,0,255);
    cout<<m1.cols<<m1.rows<<m1.channels()<<endl;

    Mat mm, dd, dst;
    multiply(m1, image, mm);
    divide(m1, image, mm);

    for (int r = 0; r<m1.rows; r++){
        uchar* curr_r = image.ptr<uchar>(r);
        for (int c = 0; c<m1.cols; r++){
            if (m1.channels() == 1){
                int pv= *curr_r;
                *curr_r++ = 255-pv;
                //int pix = m1.at<uchar>(r,c); 
                //image.at<uchar>(2,2) = 255-pix;
            }
            else{
                /*
                Vec3b pix = m1.at<Vec3b>(r,c); 
                image.at<Vec3b>(r,c)[0] = 255-pix[0];
                image.at<Vec3b>(r,c)[1] = 255-pix[1];
                image.at<Vec3b>(r,c)[2] = 255-pix[2];
                */
                *curr_r++ = 255 - *curr_r;
                *curr_r++ = 255 - *curr_r;
                *curr_r++ = 255 - *curr_r;

                Vec3b p1 = image.at<Vec3b>(r, c);
                Vec3b p2 = m1.at<Vec3b>(r, c);
                dst.at<Vec3b>(r, c)[0] = saturate_cast<uchar>(p1[0] + p2[0]);
                dst.at<Vec3b>(r, c)[1] = saturate_cast<uchar>(p1[1] + p2[1]);
                dst.at<Vec3b>(r, c)[2] = saturate_cast<uchar>(p1[2] + p2[2]);
            }
        }
    }
    Mat r1=Mat::zeros(Size(400,400), CV_8UC3);
    Mat r2=Mat::zeros(Size(400,400), CV_8UC3);
    Mat r3, rn;
    rectangle(r1, Rect(0,0,2,2),Scalar(255,255,0),-1,LINE_8,0);
    rectangle(r2, Rect(0,0,2,2),Scalar(255,255,0),-1,LINE_8,0);
    bitwise_and(r1,r2,r3);
    bitwise_not(r1,rn);

    Rect rect;
    rect.x = 200;
    rect.y = 200;
    rect.width = 20;
    rect.width = 20;

    rectangle(image, rect ,Scalar(255,255,0),-1,LINE_8,0);
    circle(image, Point(35,35), 15, Scalar(255,0,0), -1, 8, 0);
    line(image, Point(35,35), Point(32,39), Scalar(255,0,0), 1, 8, 0);
    
    RotatedRect rrt;
    rrt.center=Point(20, 35);
    rrt.size = Size(50,50);
    rrt.angle = 0.0;
    ellipse(image, rrt, Scalar(9, 25, 51), 1, 8);

    addWeighted(image, 0.7, m1, 0.3, 0, image);

    Mat canvas=Mat::zeros(Size(400,400), CV_8UC3);
    RNG rng(54157);
    int x1 = rng.uniform(0, canvas.cols);
    int y1 = rng.uniform(0, canvas.rows);
    int x2 = rng.uniform(0, canvas.cols);
    int y2 = rng.uniform(0, canvas.rows);
    int b = rng.uniform(0, 255);
    int g = rng.uniform(0, 255);
    int r = rng.uniform(0, 255);
    line(canvas, Point(x1,y1), Point(x2,y2), Scalar(b, g, r), 1, LINE_AA, 0);
    
    Mat zoomin, ud, lr, f_180;
    resize(image, zoomin, Size(20,20), 0, 0, INTER_LINEAR);
    flip(image, ud, 0);
    flip(image, lr, 1);
    flip(image, f_180, -1);


    Mat M=getRotationMatrix2D(Point(image.cols/2, image.rows/2), 45, 1.0);
    double cos= abs(M.at<double>(0,0));
    double sin= abs(M.at<double>(0,1));
    int nw = cos*image.cols + sin*image.rows;
    int nh = sin*image.cols + cos*image.rows;
    M.at <double>(0,2) = M.at <double>(0,2) + (nw/2-image.cols/2);
    M.at <double>(1,2) = M.at <double>(1,2) + (nh/2-image.rows/2);
    warpAffine(image, dst, M, image.size(), INTER_LINEAR, 0, Scalar(24,24,24));
}


