//
//  main.cpp
//
//
//  Created by Jianwei Xu on 05/01/2013.
//  Copyright (c) 2013 Jianwei Xu. All rights reserved.
//

#include "Image.h"
//#include "SupportVectorMachines.h"
#include "CameraModule.h"
#include "GaborFilter.h"
#include "NeuralNetwork.h"
#include "FaceDetector.h"

int main(int argc, const char *argv[])
{
    if (argc < 2) {
        cout << "usage: " << argv[0] << "CSV File Missing" << endl;
        exit(1);
    }
    
    string csv = string(argv[1]);
    Image *image = new Image(argc,csv);
    
    std::pair<Image*, Image*> workdata =image->split();
    Image *train = workdata.first;
    Image *test = workdata.second;
    
    cout<<"Original Images Load Complete"<<endl;
    NeuralNetwork *neu = new NeuralNetwork(train,test);
    neu->LRTrainer();
    
    //cout<<test->stringLabel(neu->LRSingleValidate(img, 7))<<endl;
    
    neu->LRPredictor();
    //    CameraModule *cam = new CameraModule();
    //    //
    //    cam->run(neu);
    
    
}
/*
 
 
 string train_csv = string(argv[1]);
 string test_csv = string(argv[2]);
 
 Image *train = new Image(argc, train_csv);
 Image *test = new Image(argc, test_csv);
 
 train->run();
 test->run();
 
 
 CameraModule *cam = new CameraModule();
 //
 cam->run(neu);
 
 vector<Mat> images = test->returnImages();
 vector<int> labels = test->returnLabels();
 int n = test->returnFileNum();
 for (int i = 0; i<n; i++)
 cout<<test->stringLabel(neu->LRSingleValidate(images[i], 7))<<" "<<test->stringLabel(labels[i])<<endl;
 
 GaborFilter *gf = new GaborFilter();
 Mat newimg = gf->labeledVector(img, Size(7,7));
 
 
 train->run();
 test->run();
 
 
 
 Mat img = imread("/Users/jianweixu/Downloads/gabor-filter-master/src/test/resources/lena.jpg",CV_LOAD_IMAGE_COLOR);
 
 Mat newimg = FD->objectDetector(img);
 //cout<<newimg.cols<<endl;
 train->displayimage(newimg);
 */



//    //Mat img = train->returnimage();
//    //
//
//    //cv::cvtColor(img, newimg, CV_BGR2GRAY);
//Mat newimg = gf->Process(img);
//train->displayimage(newimg);
////    //Mat img2;
////
//    //train->displayimage(img);
//     ImageTracker *imgtracker = new ImageTracker();
//      Mat tracker;
//    tracker = imgtracker->singleImageTracker(img, true, true);
//    //cv::cvtColor(img, tracker, CV_BGR2GRAY);
//    cout<<tracker<<endl;
//     test->displayimage(tracker);


//    Mat M(2,2, CV_32FC1,1);
//    M.col(0) = M.col(0)/2;
//    cout << "M = " << endl << " " << M << endl << endl;
//    SupportVectorMachines *svmModule = new SupportVectorMachines(train,test);
//    svmModule->SVMClassification();
//    CvSVM svm = svmModule->returnSVM();
//    CameraModule *cam = new CameraModule();
//    cam->run(svm,train->size, true);
//    svmModule->validationReport();
