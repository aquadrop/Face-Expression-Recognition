//
//  NeuralNetwork.cpp
//
//  Created by Jianwei Xu on 12/01/2013.
//  Copyright (c) 2013 Jianwei Xu. All rights reserved.
//

#include "NeuralNetwork.h"
#include <math.h>
NeuralNetwork::NeuralNetwork()
{
    
}

NeuralNetwork::NeuralNetwork(Image* trainpackage, Image* testpackage)
{
    this->trainpackage = trainpackage;
    this->testpackage = testpackage;
}

cv::Mat NeuralNetwork::labeledVector(cv::Mat& img)
{
    GaborFilter *gaborfilter = new GaborFilter();
    cv::Mat vector = gaborfilter->labeledVector(img, this->ngrid);
    return vector;
}

void NeuralNetwork::createPCA(cv::Mat& matrix)
{
    this->pca = new PCA(matrix,Mat(),CV_PCA_DATA_AS_ROW,npca);
}

cv::Mat NeuralNetwork::PCAlabeledVector(cv::Mat& matrix, bool docreatepca)//stored in rows
{
    if (docreatepca)
    {this->createPCA(matrix);cout<<"Create PCA... "<<endl;}
    Mat pcamatrix,X;
    //cout<<" cols "<<matrix.cols<<endl;
    for (int i = 0; i<matrix.rows; i++)
    {
        
        //matrix.row(i).copyTo(X);
        //cout<<X.at<float>(0,10)<<endl;
        pcamatrix.push_back(this->pca->project(matrix.row(i)));
        
    }
    //cout<<"Hi "<<pcamatrix.row(0)<<endl;
    return pcamatrix;
}




void NeuralNetwork::svmTrainer()
{
    cv::Mat traindata;
    cv::Mat trainlabels;
    std::pair<cv::Mat, cv::Mat> pacakge = unpackage(this->trainpackage,true);
    traindata = pacakge.first;
    trainlabels = pacakge.second;
    //cout<<traindata<<endl;
    //cout<<trainlabels<<endl;
    //now everything is ready
    
    cout << "Starting training process" << endl;
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 2000, 1e-6);
    this->svm->train(traindata, trainlabels, Mat(), Mat(), params);
    cout << "Finished training process" << endl;
    
    
}

void NeuralNetwork::validationReport()
{
    cv::Mat testdata;
    cv::Mat testlabels;
    std::pair<cv::Mat, cv::Mat> pacakge = unpackage(this->testpackage,false);
    testdata = pacakge.first;
    testlabels = pacakge.second;
    cout<<"Start Predicting "<<endl;
    //cout<<testdata.row(2);
    //cout<<testlabels<<endl;
    int sample_per_class[7] = {0};
    float right_per_class[7] = {0};
    
    int nfile = testlabels.rows;
    
    //cout<<testdata.row(0);
    
    for (int i = 0; i<nfile; i++)
    {
        //cout<<i<<" "<<testlabels.at<float>(i)<<endl;
        int current_label = testlabels.at<int>(i);
        //cout<<current_label<<endl;
        sample_per_class[current_label]++;
        //cout<<testdata.row(i)<<endl;
        //cout<<sample<<endl;
        float r = this->svm->predict(testdata.row(i));
        cout<<r<<" "<<current_label<<endl;
        int response = (int)r;
        //cout<<response<<" "<<current_label<<endl;
        if (response == current_label)
        {
            right_per_class[current_label]++;
        }
    }
    
    cout<<"Prediction Accracy "<<endl;
    for (int i = 0; i<7; i++)
    {
        if (sample_per_class[i] == 0) continue;
        //right_per_class[i] = right_per_class[i]/sample_per_class[i];
        cout<<i<<" "<<right_per_class[i]<<" "<<sample_per_class[i]<<endl;
    }
    
}



void NeuralNetwork::LRTrainer()
{
    Image *img = new Image();
    cv::Mat traindata;
    cv::Mat trainlabels;
    std::pair<cv::Mat, cv::Mat> pacakge = unpackage(this->trainpackage,true);
    traindata = pacakge.first;
    trainlabels = pacakge.second;
    
    int nsample = traindata.rows;
    int nexpression = img->nexpression;
    int lengthvector = traindata.cols;
    Mat weight(nexpression, lengthvector, CV_32F);
    //weight = weight;
    
    float labelmatrix[nsample][nexpression];
    
    for (int n = 0; n<nsample; n++)
    {
        for (int j = 0; j<nexpression; j++)
        {
            labelmatrix[n][j] = 0.0;
        }
        labelmatrix[n][trainlabels.at<int>(n)] = 1;
    }
    
    float learn_rate = 0.05;
    int iteration = 1000;
    
    RNG rng(0xFFFFFFFF);
    //initiate weights
    float rn;
    for (int i = 0; i<nexpression; i++)
    {
        for (int j = 0; j<lengthvector; j++)
        {
            rn = (float)rng.gaussian(0.01);
            //cout<<rn<<endl;
            weight.at<float>(i,j) = rn;
        }
    }
    //cout<<weight.at<float>(0,0)<<endl;
    //learn
    
    //cout<<gradient<<endl;
    //Mat gradientline(1,lengthvector,CV_32F);
    for (int i = 0; i<iteration; i++)
    {
        //calculate gradient
        Mat pred;
        Mat gradient(nexpression,lengthvector,CV_32F,Scalar(0));
        for (int n = 0; n<nsample; n++)
        {
            Mat datavector = traindata.row(n);
            pred.push_back(softmax(weight, datavector, nexpression));
        }
        //cout<<pred.at<float>(0,0)<<endl;
        for (int k = 0; k<nexpression; k++)
        {
            for (int n = 0; n<nsample; n++)
            {
                gradient.row(k) = gradient.row(k) + (pred.at<float>(n,k)-
                                                     (float)labelmatrix[n][k])
                *traindata.row(n);
            }
            
        }
        
        
        weight = weight - learn_rate*gradient;
        Mat rowsum;
        reduce(gradient, rowsum, 1, CV_REDUCE_MAX);
        cout<<i<<" "<<rowsum.at<float>(0)<<endl;
        //cout<<weight<<endl;
        pred.release();
        gradient.release();
    }
    
    this->neuweight = weight;
}

void NeuralNetwork::LRPredictor()
{
    Image *img = new Image();
    cv::Mat testdata;
    cv::Mat testlabels;
    //    std::pair<cv::Mat, cv::Mat> pacakge = unpackage(this->testpackage,false);
    std::pair<cv::Mat, cv::Mat> pacakge = unpackage(this->testpackage,false);
    testdata = pacakge.first;
    testlabels = pacakge.second;
    cout<<"Start Predicting "<<endl;
    //cout<<testdata.row(2);
    //cout<<testlabels<<endl;
    int nsample = testdata.rows;
    cout<<"number "<<nsample<<endl;
    int nexpression = img->nexpression;
    //int lengthvector = testdata.cols;
    int sample_per_class[nexpression];
    float right_per_class[nexpression];
    
    for (int k = 0; k<nexpression; k++)
    {
        sample_per_class[k] = 0;
        right_per_class[k] = 0;
    }
    
    Mat soft;
    for (int n = 0; n<nsample; n++)
    {
        cout<<testlabels.at<int>(n)<<endl;
        sample_per_class[testlabels.at<int>(n)]++;
        Mat test = testdata.row(n);
        soft = softmax(this->neuweight, test, nexpression);
        //cout<<testlabels.at<int>(n)<<" "<<soft<<endl;
        int pred = 0;
        float softmax = -1;
        float softtemp;
        for (int k = 0; k<nexpression; k++)
        {
            if ((softtemp=soft.at<float>(k))>softmax)
            {
                softmax = softtemp;
                pred = k;
            }
        }
        
        int currentlabel = testlabels.at<int>(n);
        if (pred==currentlabel)
        {
            right_per_class[pred]++;
        }
    }
    
    for (int k = 0; k<nexpression; k++)
    {
        cout<<k<<" "<<right_per_class[k]<<" "<<sample_per_class[k]<<endl;
    }
}



int NeuralNetwork::LRSingleValidate(Mat& img, int nexpression)
{
    FaceDetector *facedetect = new FaceDetector();
    Mat face = facedetect->objectDetector(img);
    
    Mat labelvector;
    Mat labelmatrix;
    Mat pcalabelvector;
    Mat pcamatrix;
    if (face.empty()) {return -1;}
    else
    {
        labelvector = this->labeledVector(face);
        this->zeroMean(labelvector);
        labelmatrix.push_back(labelvector);
    }
    //cout<<labelvector<<endl;
    //pcalabelvector = this->pca->project(labelvector);
    pcalabelvector = this->PCAlabeledVector(labelmatrix,false);
    //cout<<pcalabelvector<<endl;
    
    this->zeroMean(pcalabelvector);
    
    Mat soft = softmax(this->neuweight, pcalabelvector, nexpression);
    int pred = 0;
    float softmax = -1;
    float softtemp;
    for (int k = 0; k<nexpression; k++)
    {
        if ((softtemp=soft.at<float>(k))>softmax)
        {
            softmax = softtemp;
            pred = k;
        }
    }
    pcalabelvector.release();
    labelvector.release();
    return pred;
    
}



cv::Mat NeuralNetwork::softmax(cv::Mat &weight, cv::Mat &datavector, int nexpression)
{
    Mat pred(1, nexpression, CV_32F);
    float activation[nexpression];
    float summary = 0;
    int length = datavector.cols;
    for (int k = 0; k<nexpression; k++)
    {
        float act = 0;
        for (int v = 0; v<length; v++)
        {
            act+=weight.at<float>(k,v)*datavector.at<float>(v);
        }
        activation[k] = (float)exp(act);
        summary+=activation[k];
    }
    for (int k = 0; k<nexpression; k++)
        pred.at<float>(k) = activation[k]/summary;
    
    return pred;
}


std::pair<cv::Mat, cv::Mat> NeuralNetwork::unpackage(Image* package, bool docreatepca)
{
    FaceDetector *facedetect = new FaceDetector();
    vector<cv::Mat> images = package->returnImages();
    vector<int> labels = package->returnLabels();
    cout<<package->returnFileNum()<<endl;
    vector<cv::Mat>::iterator it;
    vector<int>::iterator labelit = labels.begin();
    cv::Mat data;
    cv::Mat label;
    
    cv::Mat face;
    Mat labeledmatrix;
    Mat labelvector;
    Mat pcalabeledvector;
    int count = 0;
    
    for (it=images.begin(); it!=images.end();++it)
    {
        
        if ((*it).empty())
        {continue;}
        else
        {
            
            face = facedetect->objectDetector((*it));
            //package->displayimage(face);
            if (face.empty()) {continue;}
            else
            {
                cout<<"File "<<++count<<endl;
                labelvector = this->labeledVector(face);
                //cout<<labelvector.cols<<endl;
                this->zeroMean(labelvector);
                //scout<<labelvector<<endl;
                //this->zeroMean(labelvector);
                labeledmatrix.push_back(labelvector);
                //cout<<"File "<<++count<<" "<<labelvector.at<float>(0)<<endl;
                label.push_back(*labelit);
            }
        }
        labelit++;
    }
    
    
    data = this->PCAlabeledVector(labeledmatrix,docreatepca);
    cout<<"Hi "<<data.at<float>(0,0)<<endl;
    this->zeroMean(data);
    return std::make_pair(data, label);
    
    
}

std::pair<cv::Mat, cv::Mat> NeuralNetwork::unpackage(Image* package)
{
    vector<cv::Mat> images = package->returnImages();
    vector<int> labels = package->returnLabels();
    cout<<package->returnFileNum()<<endl;
    vector<cv::Mat>::iterator it;
    vector<int>::iterator labelit = labels.begin();
    cv::Mat data;
    cv::Mat label;
    
    cv::Mat face;
    Mat labeledmatrix;
    Mat labelvector;
    Mat pcalabeledvector;
    int count = 0;
    
    for (it=images.begin(); it!=images.end();++it)
    {
        
        if ((*it).empty())
        {continue;}
        else
        {
            
            face = *it;//facedetect->objectDetector((*it));
            //package->displayimage(face);
            if (face.empty()) {continue;}
            else
            {
                cout<<"File "<<++count<<endl;
                labelvector = this->labeledVector(face);
                //cout<<labelvector.cols<<endl;
                this->zeroMean(labelvector);
                //scout<<labelvector<<endl;
                //this->zeroMean(labelvector);
                labeledmatrix.push_back(labelvector);
                //cout<<"File "<<++count<<" "<<labelvector.at<float>(0)<<endl;
                label.push_back(*labelit);
            }
        }
        labelit++;
    }
    
    
    data = this->PCAlabeledVector(labeledmatrix,false);
    //cout<<"Hi "<<data.at<float>(0,0)<<endl;
    this->zeroMean(data);
    return std::make_pair(data, label);
    
    
}

void NeuralNetwork::zeroMean(Mat& data)
{
    float mean = 0;
    float stddev = 0;
    for (int i = 0; i<data.rows; i++)
    {
        for (int j = 0; j<data.cols; j++)
        {
            mean+=data.at<float>(i,j);
        }
        mean/=data.cols;
        for (int j = 0; j<data.cols; j++)
        {
            float cut = data.at<float>(i,j)-mean;
            stddev+= cut*cut;
        }
        stddev = sqrt(stddev/data.cols);
        
        
        data.row(i) = data.row(i) - mean;
        data.row(i) = data.row(i)/stddev;
        
    }
    //cout<<stddev<<endl;
}
