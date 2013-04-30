//
//  CameraModule.cpp
//  //
//  Created by Jianwei Xu on 10/01/2013.
//  Copyright (c) 2013 Jianwei Xu. All rights reserved.
//

#include "CameraModule.h"


CameraModule::CameraModule()
{
    
}


void CameraModule::run(NeuralNetwork* neu)
{
    Image *img = new Image();
    cv::Mat frame,gray;
    cv::Mat testimage;
    string text;
    CvCapture* camera = cvCreateCameraCapture(CV_CAP_ANY);
    //    cvSetCaptureProperty(camera,CV_CAP_PROP_FRAME_WIDTH,256);
    //    cvSetCaptureProperty(camera,CV_CAP_PROP_FRAME_HEIGHT,256);
    
    cvNamedWindow("Face Tracker",CV_WINDOW_AUTOSIZE);
    
    const char *predictlabel = "NULL";
    
    
    
    CvFont font;
    double hScale=1.0;
    double vScale=1.0;
    int    lineWidth=1;
    cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);
    while(1)
    {
        //cout<<++k<<endl;
        //grab image, resize and flip
        IplImage* I = cvQueryFrame(camera);
        if(!I)continue;
        Mat im(I);
        //cout<<im.rows<<endl;
        //cout<<im.cols<<endl;
        cv::cvtColor(im,gray,CV_BGR2GRAY);
        //img->displayimage(gray);
        int label = neu->LRSingleValidate(gray, img->nexpression);
        predictlabel = (img->stringLabel(label)).c_str();
        //cout<<predictlabel<<endl;
        //cout<<predictlabel<<endl;
        
        if (im.rows == 0) continue;
        //resize(im, im, cv::Size(800,800));
        putText (im,predictlabel,cvPoint(200,400), FONT_HERSHEY_COMPLEX_SMALL, 6, cvScalar(50,50,50), 1, CV_AA);
        
        imshow("Face Tracker",im);
        int c = cvWaitKey(10);
        if(c == 27)
            break;
    }
    
}



