//
//  Image.cpp
//
//
//  Created by Jianwei Xu on 05/01/2013.
//  Copyright (c) 2013 Jianwei Xu. All rights reserved.
//

#include "Image.h"

Image::Image()
{
    
}

Image::Image(int argc, string filename)
{
    this->argc = argc;
    this->filename = filename;
}

void Image::run()
{
    string fn_csv = this->filename;
    try
    {
        read_csv(fn_csv);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    // Quit if there are not enough images for this demo.
    if(images.size() <= 1)
    {
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }
}






void Image::read_csv(const string& filename)
{
    char separator = ';';
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    this->nfile = 0;
    string line, path, classlabel;
    Mat src;
    int count = 0;
    while (getline(file, line))
    {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty())
        {
            src = imread(path,0);
            if (src.channels() != 1)
                cv::cvtColor(src, src, CV_BGR2GRAY);
            //cv::resize(src, src, cv::Size(1280,720);
            //imread(path, 0).convertTo(src, CV_64FC1, 1.0/255, 0);
            if (count == 0)
            {
                count++;
                size = cv::Size(src.rows,src.cols);
            }
            this->images.push_back(src);
            this->labels.push_back(this->intLabel(classlabel));
            //cout<<classlabel<<" "<<this->intLabel(classlabel)<<endl;
            this->nfile++;
            this->filepath.push_back(path);
            //cout<<path<<endl;
        }
    }
    //    std::vector<string>::iterator iter;
    //    for (iter = this->labels.begin();iter!=this->labels.end();iter++)
    //    {
    //        cout<<*iter<<endl;
    //    }
}


std::pair<Image*, Image*> Image::split()
{
    Image* train = new Image();
    Image* test = new Image();
    
    string fn_csv = this->filename;
    try
    {
        read_csv(fn_csv, train, test);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    // Quit if there are not enough images for this demo.
    if(train->images.size() <= 1)
    {
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }
    
    return make_pair(train, test);
}

void Image::read_csv(const string &filename, Image *train, Image *test)
{
    char separator = ';';
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "Split：No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    train->nfile = 0;
    test->nfile = 0;
    string line, path, classlabel;
    Mat src;
    int count = 0;
    int splitcount = 0;
    int splitratio = 3;
    while (getline(file, line))
    {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty())
        {
            src = imread(path,0);
            if (src.channels() != 1)
                cv::cvtColor(src, src, CV_BGR2GRAY);
            
            if (count == 0)
            {
                count++;
                train->size = cv::Size(src.rows,src.cols);
                test->size = train->size;
            }
            
            if (splitcount < splitratio)
            {
                train->images.push_back(src);
                train->labels.push_back(this->intLabel(classlabel));
                //cout<<classlabel<<" "<<this->intLabel(classlabel)<<endl;
                train->nfile++;
                train->filepath.push_back(path);
                //cout<<path<<endl;
                splitcount++;
            }
            else
            {
                test->images.push_back(src);
                test->labels.push_back(this->intLabel(classlabel));
                //cout<<classlabel<<" "<<this->intLabel(classlabel)<<endl;
                test->nfile++;
                test->filepath.push_back(path);
                //cout<<path<<endl;
                splitcount = 0;
                
            }
            
        }
    }
    
}







//display image in a window
void Image::displayimage(Mat &img)
{
    namedWindow( "Display window", CV_WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", img);
    waitKey(0);
}

//randomly return image back
Mat Image::returnimage()
{
    std::srand((unsigned int)time(NULL));
    int index = rand()%this->nfile;
    return this->images[index];
}


vector<Mat> Image::returnImages()
{
    return this->images;
}

vector<int> Image::returnLabels()
{
    return this->labels;
}

int Image::returnFileNum()
{
    return nfile;
}


int Image::intLabel(string &label)
{
    if (label =="ANGERY")
        return this->ANGRY;
    else if (label == "DISGUST")
        return this->DISGUST;
    else if (label == "FEAR")
        return this->FEAR;
    else if (label == "HAPPY")
    {return this->HAPPY;cout<<HAPPY<<endl;}
    else if (label == "NEUTRAL")
        return this->NEUTRAL;
    else if (label == "SAD")
        return this->SAD;
    else if (label == "SURPRISE")
        return this->SURPRISE;
    else
        return -1;
}

string Image::stringLabel(int label)
{
    if (label ==0)
        return "ANGERY";
    else if (label == 1)
        return "DISGUST";
    else if (label == 2)
        return "FEAR";
    else if (label == 3)
        return "HAPPY";
    else if (label == 4)
        return "NEUTRAL";
    else if (label == 5)
        return "SAD";
    else if (label == 6)
        return "SURPRISE";
    else
        return "UNRECOGNISABLE";
}

























