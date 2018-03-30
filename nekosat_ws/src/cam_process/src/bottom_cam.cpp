//
//  main.cpp
//  Opencv_test
//
//  Created by Ler Wilson on 5/11/17.
//  Copyright © 2017 Ler Wilson. All rights reserved.
//

//
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <cam_process/BoundingBox.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include "ros/ros.h"


using namespace cv;
using namespace std;

ros::Publisher my_msg_pub_;

Mat grayWorld_2 (Mat imgIn) {
    Mat imgOut, BGR[3];
    split(imgIn, BGR);
    Scalar sum_r = sum(BGR[0]);
    Scalar sum_g = sum(BGR[1]);
    Scalar sum_b = sum(BGR[2]);
    
    // Obtain mean
    sum_r /= BGR[0].rows*BGR[0].cols;
    sum_g /= BGR[1].rows*BGR[1].cols;
    sum_b /= BGR[2].rows*BGR[2].cols;
    
    // Find the max value, something similar to np.max
    double_t max_val = sum_r[0];
    if (sum_g[0] > max_val) {
        max_val = sum_g[0];
    }
    if (sum_b[0] > max_val) {
        max_val = sum_b[0];
    }
    //cout << max_val << endl;
    BGR[0] = max_val/sum_r[0] * BGR[0];
    BGR[1] = max_val/sum_g[0] * BGR[1];
    BGR[2] = max_val/sum_b[0] * BGR[2];
    
    merge(BGR, 3, imgOut);
    return imgOut;
}


Mat bounding_box (Mat imgIn, int *x, int *y, int *w, int *h) {
    Mat bounding_box, hsv, dilation_closing;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    cvtColor(imgIn, hsv, COLOR_BGR2HSV);
    
    Mat1b mask1, mask2;
    inRange(hsv, Scalar(160, 30, 30), Scalar(179, 255, 255), mask1); // This needs adjustment
    //inRange(hsv, Scalar(0,30,30) ,Scalar(15,255,255), mask2);
    
    //Mat1b masked_output;

    //bitwise_or(mask1, mask2, masked_output);
    //bitwise_and(imgIn, imgIn , imgIn ,masked_output);

    findContours(mask1, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    
    
    Rect box;
    /*
    for (int i = 0; i < contours.size() ; i++) {
        box = boundingRect(contours[i]);
        if (box.width > 10 && box.height > 10) {
            *x = box.x;
            *y = box.y;
            *h = box.height;
            *w = box.width;
            Scalar color_box = Scalar( 0, 255, 255);
            rectangle(imgIn, box, color_box,2);

        }
    }
    */
    *x = -1; *y = -1; *w = -1; *h = -1;
    int max_area = 0;
    for (int i = 0; i < contours.size() ; i++) {
        if (contourArea(contours[i]) > max_area) {
            // Find largest contour
        
            box = boundingRect(contours[i]);
            if (box.width > 10 && box.height > 10) {

                max_area = contourArea(contours[i]);
                *x = box.x;
                *y = box.y;
                *h = box.height;
                *w = box.width;
            
                Scalar color_box = Scalar( 0, 255, 255);
                rectangle(imgIn, box, color_box,2);
            }

        }
    }

    return imgIn;
}



Mat displayImg(Mat source){
    int x, y, w, h;

    cam_process::BoundingBox msg;
    
    source = bounding_box (source, &x, &y, &w, &h);

    msg.x = x;
    msg.y = y;
    msg.width = w;
    msg.height = h;
    msg.source_id = "frontcam";

    my_msg_pub_.publish(msg);
    //waitKey(1);
    return source;
}


int main(int argc, char **argv){
    Mat source;
    VideoCapture cap(0);

    ros::init(argc, argv, "bottom_cam");

    ros::NodeHandle n;
    //cv::startWindowThread();
    image_transport::ImageTransport it(n);

    my_msg_pub_ = n.advertise<cam_process::BoundingBox>("/center_box_topic", 1); 

    image_transport::Publisher image_pub = it.advertise("Image_Stream_bottom", 1);




    while (n.ok()) { 
        cap.read(source);
        source = grayWorld_2 (source);
        //source = displayImg (source);
        
        sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", source).toImageMsg();
        waitKey(1);
        image_pub.publish(msg);
        //ros::spin();
    }
    cap.release();
    //image_transport::Subscriber cam_image_sub_ = it.subscribe("/hornet/bottomcam", 1, displayImg); // front or bottom, change topic as you wish

}


/**
int k_nearest (Mat input, int * x_send, int * y_send, int * width_send, int * height_send) {
    vector<vector<Point> > contours;
    Mat dilation_closing, bounding_box;
    // Controls for auto tuning
    int allowance = 10; // Decides the range of differences allowed from min +x
    int setpoint = 170; // Decides minimum difference required to pass as an object (anything above this would not be recognised)
    int tuning_val = 3; // Decides the minimum allowed size of bucket (needs more tuning)
    
    
    int x = 0, y = 0, w = 0, h = 0;
    vector<Vec4i> hierarchy;
    Mat lab_image, Lab[3];
    double feature[3] = {53.23, 80.11, 67.22}; // For pure red
    //double feature[3] = {97.13, -21.5559, 94.482}; // For yellow
    
    cvtColor(input, lab_image, CV_RGB2Lab);
    split(lab_image, Lab);
    
    Mat img(lab_image.rows, lab_image.cols, CV_8UC1, Scalar(0)), processed(lab_image.rows, lab_image.cols, CV_8UC1);
    double min = 255;
    
    for (int d = 0 ; d < lab_image.rows; d++) {
        for (int c = 0; c < lab_image.cols; c++) {
            double difference_total = 0;
            for (int i = 1; i < 3; i++) {
                double difference = Lab[i].at<unsigned char>(d, c) - feature[i];
                difference = pow(difference, 2);
                difference_total += difference;
            }
            difference_total = sqrt(difference_total);
            // Convert this into an image
            if (difference_total < min) {
                min = difference_total;
            }
            img.at<unsigned char>(d,c) = difference_total;
        }
    }
    
    
    //printf ("min: %f\n", min);
    if (min < setpoint) {
        inRange(img, Scalar(0), Scalar(min+allowance), processed);
    }else {
        inRange(img, Scalar(0), Scalar(0), processed);
    }
    // Try to ditch dilation and closing
    dilation_closing = Erosion(processed);
    dilation_closing = Dilation(dilation_closing);
    findContours( dilation_closing, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    Rect box, best_contour;
    int contour_thresh = (255-min)*tuning_val; // Adaptive bounding box threshold control
    //int contour_thresh = 0;
    Scalar pure_red(53.23, 80.11, 67.22);
    double mindiff = 255*255*255;
    // Theory: when the obj is further away, the more decolourised it becomes
    for (int i = 0; i < contours.size() ; i++) {
        box = boundingRect(contours[i]);
        if (contourArea(contours[i]) > contour_thresh) {
            x = box.x;
            y = box.y;
            h = box.height;
            w = box.width;
            Mat cropped_contour = input(box);
            Scalar color =sum(cropped_contour)/(cropped_contour.rows*cropped_contour.cols);
            Scalar diff = color - pure_red;
            if(mindiff > (abs(diff[2]))) {
                mindiff = (abs(diff[2]));
                best_contour = box;
            }
            Scalar color_box = Scalar( 0, 255, 255);
            rectangle(input, best_contour, color_box,2);

            *x_send = x;
            *y_send = y;
            *width_send = w;
            *height_send = h;
        }else{
            return 0;
        }
    }
    return 1;
}


Mat Erosion(Mat imgIn)
{
    Mat erosion_dst;
    int erosion_size = 5;
    int erosion_type;
    erosion_type = MORPH_RECT;
    Mat element = getStructuringElement( erosion_type,
                                        Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                        Point( erosion_size, erosion_size ) );
    
    /// Apply the erosion operation
    erode( imgIn, erosion_dst, element );
    return erosion_dst;
}

Mat Dilation( Mat imgIn )
{
    Mat dilation_dst;
    int dilation_size = 5;
    int dilation_type;
    dilation_type = MORPH_RECT;
    Mat element = getStructuringElement( dilation_type,
                                        Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                        Point( dilation_size, dilation_size ) );
    /// Apply the dilation operation
    dilate( imgIn, dilation_dst, element );
    return  dilation_dst;
}
*/
