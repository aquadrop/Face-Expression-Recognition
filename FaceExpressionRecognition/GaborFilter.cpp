//
//  GaborFilter.cpp
//
//
//  Created by Jianwei Xu on 10/01/2013.
//  Copyright (c) 2013 Jianwei Xu. All rights reserved.
//

#include "GaborFilter.h"

GaborFilter::GaborFilter()
{
    
}



cv::Mat GaborFilter::mkKernel(int ks, double sig, double th, double lm, double ps)
{
    int hks = (ks-1)/2;
    float theta = th*CV_PI/180;
    float psi = ps*CV_PI/180;
    float del = 2.0/(ks-1);
    float lmbd = lm;
    float sigma = sig/ks;
    float x_theta;
    float y_theta;
    cv::Mat kernel(ks,ks, CV_32F);
    for (int y=-hks; y<=hks; y++)
    {
        for (int x=-hks; x<=hks; x++)
        {
            x_theta = x*del*cos(theta)+y*del*sin(theta);
            y_theta = -x*del*sin(theta)+y*del*cos(theta);
            kernel.at<float>(hks+y,hks+x) = (float)exp(-0.5*(pow(x_theta,2)+pow(y_theta,2))/pow(sigma,2))* cos(2*CV_PI*x_theta/lmbd + psi);
            //cout<<exp(-0.5*(pow(x_theta,2)+pow(y_theta,2))/pow(sigma,2))<<endl;
        }
    }
    return kernel;
}

cv::Mat GaborFilter::Process(cv::Mat &image, float frequency, float orientation)
{
    //cv::Mat image = cv::imread("cat.tiff",1);
    cv::Mat src, src_f;
    //src = image;
    //cv::cvtColor(image, src, CV_BGR2GRAY);
    image.convertTo(src_f, CV_32F, 1.0/255, 0);
    cv::Mat dest, mag;
    //cv::namedWindow("Process window", 1);
    double sig = pos_sigma;
    double lm = 0.5+pos_lm/100.0;
    double th = orientation;
    double ps = frequency;
    cv::Mat kernel = mkKernel(kernel_size, sig, th, lm, ps);
    //cout<<src_f<<endl;
    cv::filter2D(src_f
                 , dest
                 , CV_32F, kernel);
    
    
    cv::pow(dest, 2.0, mag);
    return mag;
    //    cv::imshow("Mag", mag);
    //cv::waitKey(0);
}


cv::Mat GaborFilter::labeledVector(cv::Mat& img, cv::Size gridsize)
{
    //Image *IMG = new Image();
cv:Mat vector;
    cv::Mat filteredimg;
    
    int stepcol = max((int)img.cols/gridsize.width,1);
    int steprow = max((int)img.rows/gridsize.height,1);
    //cout<<"Grid "<<img.cols<<" "<<stepcol<<endl;
    int startcol = max((int)(img.cols+1)/2-gridsize.width/2*stepcol-1,0);
    int startrow = max((int)(img.rows+1)/2-gridsize.height/2*steprow -1,0);
    
    int endcol = min(startcol+(gridsize.width-1)*stepcol,img.cols);
    int endrow = min(startrow+(gridsize.height-1)*steprow,img.cols);
    
    int count = 0;
    
    for (int i = 0; i< this->nfrequency; i++)
    {
        for (int j = 0; j<this->norientation; j++)
        {
            filteredimg = Process(img, frequencies[i], orientations[j]);
            //IMG->displayimage(filteredimg);
            for (int m = startrow; m<endrow; m+=steprow)
            {
                //cout<<startcol<<" "<<stepcol<<" "<<m<<" "<<img.rows<<endl;
                
                for (int n = startcol; n<endcol; n+=stepcol)
                {
                    vector.push_back(filteredimg.at<float>(m,n));
                    //                    cout<<count<<" "<<filteredimg.at<float>(m,n)<<endl;
                    count++;
                }
            }
        }
        
    }
    //cout<<count<<endl;
    vector = vector.t();//return in row;
    return vector;
    
}


















