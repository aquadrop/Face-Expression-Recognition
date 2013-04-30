//
//  FaceDetector.h
//  FaceExpressionRecognition
//
//  Created by Jianwei Xu on 30/04/2013.
//  Copyright (c) 2013 Jianwei Xu. All rights reserved.
//

#ifndef __FaceExpressionRecognition__FaceDetector__
#define __FaceExpressionRecognition__FaceDetector__

#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/ml/ml.hpp"
#include "Image.h"

#include <iostream>

using namespace std;
using namespace cv;

class FaceDetector
{
    String face_cascade_name = "./FaceExpressionRecognition/haarcascade_frontalface_alt.xml";
    String eyes_cascade_name = "./FaceExpressionRecognition/haarcascade_eye_tree_eyeglasses.xml";
    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;
    const float minscale = 0.3;//IMPORTANT
public:
    FaceDetector();
    void loadModel();
    cv::Mat objectDetector(Mat&);
    void objectDectorDraw(Mat&);
    
};

#endif /* defined(__FaceExpressionRecognition__FaceDetector__) */
