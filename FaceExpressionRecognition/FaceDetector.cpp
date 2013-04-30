//
//  FaceDetector.cpp
//
//
//  Created by Jianwei Xu on 12/01/2013.
//  Copyright (c) 2013 Jianwei Xu. All rights reserved.
//

#include "FaceDetector.h"

FaceDetector::FaceDetector()
{
    loadModel();
}

void FaceDetector::loadModel()
{
    if( !face_cascade.load( face_cascade_name ) )
    {
        printf("--(!)Error Face Detection Model Loading\n");
        exit(-1);
    }
    //    if( !eyes_cascade.load( eyes_cascade_name ) )
    //    {
    //        printf("--(!)Error loading\n");
    //        exit(-1);
    //    }
}

cv::Mat FaceDetector::objectDetector(cv::Mat& img)
{
    
    std::vector<Rect> faces;
    Mat img_gray;
    
    if (img.channels() == 1)
        img_gray = img;
    else
        cvtColor( img, img_gray, CV_BGR2GRAY );
    equalizeHist( img_gray, img_gray );
    
    //-- Detect faces
    Size minSize = Size((int)img.cols*minscale,(int)img.rows*minscale);
    //cout<<minSize.width<<endl;
    face_cascade.detectMultiScale( img_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, minSize );
    //    CvScalar color = CV_RGB(255, 225, 225);
    //    for( int i = 0; i < faces.size(); i++ )
    //    {
    //        Point corner(faces[i].x, faces[i].y);
    //        Point branch(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
    //
    //        rectangle(img,
    //                  corner,
    //                  branch,
    //                  color,
    //                  1, 8, 0
    //                  );
    //    }
    
    //get rectangle part of image;
    if(faces.size()!=0)
    {
        IplImage iplimg = img;
        //cout<<faces[0].width<<endl;
        cvSetImageROI(&iplimg, cvRect(faces[0].x, faces[0].y, faces[0].width, faces[0].height));
        Mat face(&iplimg);
        //cout<<face.rows<<endl;
        return face;
    }
    else
    {
        return Mat();
        
    }
    
}

void FaceDetector::objectDectorDraw(cv::Mat& img)
{
    Image *IMG = new Image();
    std::vector<Rect> faces;
    Mat img_gray;
    
    if (img.channels() == 1)
        img_gray = img;
    else
        cvtColor( img, img_gray, CV_BGR2GRAY );
    equalizeHist( img_gray, img_gray );
    
    //-- Detect faces
    Size minSize = Size((int)img.cols*minscale,(int)img.rows*minscale);
    face_cascade.detectMultiScale( img_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, minSize );
    CvScalar color = CV_RGB(255, 225, 225);
    for( int i = 0; i < faces.size(); i++ )
    {
        Point corner(faces[i].x, faces[i].y);
        Point branch(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
        
        rectangle(img,
                  corner,
                  branch,
                  color,
                  1, 8, 0
                  );
    }
    IplImage iplimg = img;
    cvSetImageROI(&iplimg, cvRect(10, 15, 150, 250));
    Mat newimg(&iplimg);
    IMG->displayimage(newimg);
    
}






















