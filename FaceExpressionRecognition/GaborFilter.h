//
//  GaborFilter.h
//  FaceExpressionRecognition
//
//  Created by Jianwei Xu on 30/04/2013.
//  Copyright (c) 2013 Jianwei Xu. All rights reserved.
//

#ifndef __FaceExpressionRecognition__GaborFilter__
#define __FaceExpressionRecognition__GaborFilter__

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>

#include "Image.h"

using namespace::std;
using namespace::cv;

class GaborFilter
{
    int kernel_size=21;
    float pos_sigma= 3.14;
    float pos_lm = 50;
    float pos_th = 0;
    float pos_psi = 90;
    
    float orientations[6] = {0,30,60,90,120,150};
    int norientation = 6;
    float frequencies[5] = {90,45,22.5,11.25,5.625};
    int nfrequency = 5;
    
public:
    GaborFilter();
    cv::Mat Process(cv::Mat&, float, float);
    
    //return an 1D vector of 7*7*5*6;
    cv::Mat labeledVector(cv::Mat&, cv::Size);
    
private:
    cv::Mat mkKernel(int ks, double sig, double th, double lm, double ps);
};

#endif /* defined(__FaceExpressionRecognition__GaborFilter__) */
