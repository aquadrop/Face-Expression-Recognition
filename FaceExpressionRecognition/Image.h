//
//  Image.h
//  FaceExpressionRecognition
//
//  Created by Jianwei Xu on 30/04/2013.
//  Copyright (c) 2013 Jianwei Xu. All rights reserved.
//

#ifndef __FaceExpressionRecognition__Image__
#define __FaceExpressionRecognition__Image__

#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

class Image {
    int argc;
    string filename;
    vector<Mat> images;
    vector<int> labels;
    
    int nfile;
public:
    cv::Size size;
    vector<string> filepath;
    enum Expressions {ANGRY = 0, DISGUST = 1, FEAR = 2, HAPPY = 3, NEUTRAL = 4, SAD = 5, SURPRISE = 6};
    const int nexpression = 7;
    Image(int, string);
    Image();
    void run();
    
    std::pair<Image*, Image*> split();//split data into trainpacakge and testpackage with ratio 4:1
    
    vector<Mat> returnImages();
    int returnFileNum();
    vector<int> returnLabels();
    
    void displayimage(Mat&);
    Mat returnimage();
    
    int intLabel(string&);
    string stringLabel(int);
private:
    void read_csv(const string&);
    void read_csv(const string&, Image*, Image*);
    
};

#endif /* defined(__FaceExpressionRecognition__Image__) */
