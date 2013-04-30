//
//  CameraModule.h
//  FaceExpressionRecognition
//
//  Created by Jianwei Xu on 30/04/2013.
//  Copyright (c) 2013 Jianwei Xu. All rights reserved.
//

#ifndef __FaceExpressionRecognition__CameraModule__
#define __FaceExpressionRecognition__CameraModule__

#include <iostream>
//#include "SupportVectorMachines.h"
#include "NeuralNetwork.h"

class CameraModule
{
public:
    CameraModule();
    //void run(CvSVM&, cv::Size&, bool);
    void run(NeuralNetwork*);
};

#endif /* defined(__FaceExpressionRecognition__CameraModule__) */
