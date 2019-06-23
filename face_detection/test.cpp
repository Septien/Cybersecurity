#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <fstream>

using namespace cv;
using namespace std;

int s(int x)
{
    if (x >= 0)
        return 1;
    return 0;
}

vector<uchar> toArray(Mat m, int cth,int &gc)
{
    vector<uchar> neighbors;
    int r = m.rows, c = m.cols;
    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (i + j == cth)
            {
                gc = (int) m.at<uchar>(i, j);
                continue;
            }
            neighbors.push_back(m.at<uchar>(i, j));
        }
    }
    return neighbors;
}

// Uniform LBP
int ulbp(Mat kernel, int c, int pKernel)
{
    int gc = 0;
    // Store neighbors in array and get cth pixel
    vector<uchar> neighbors = toArray(kernel, c, gc);
    // Get the 0th pixel
    int gp = (int)neighbors[0];
    int gp_1 = (int)neighbors.at(neighbors.size() - 1);
    int a0 = fabs(s(gp_1 - gc) - s(gp - gc));
    for (int i = 2; i < neighbors.size(); i++)
    {
        gp_1 = (int)neighbors.at(i - 1);
        gp = (int)neighbors.at(i);
        a0 += fabs(s(gp - gc) - s(gp_1 - gc));
    }
    return a0;
}

// Local binary pattern
int lbp(Mat kernel, int c, int pKernel)
{
    int gc = 0;
    // Store neighbors in array and get cth pixel
    vector<uchar> neighbors = toArray(kernel, c, gc);
    int r = 0;
    int gp = 0;
    for (int i = 0; i < neighbors.size(); i++)
    {
        gp = (int)neighbors[i];
        r += s(gp - gc) * pow(2, i);
    }
    return r;
}

Mat gray2lbp(Mat img, vector<int> &histogram, int kWidth, int kHeight)
{
    int rows = img.rows, cols = img.cols;
    Mat lbpImg = Mat(rows, cols, CV_8U, Scalar(0));
    histogram.clear();
    histogram.assign(pow(2, (kWidth * kHeight) - 1), 0);
    cout << histogram.size() << endl;
    int i, j;
    int lbpV = 0;
    for (i = 1; i < rows - 1; i++)
    {
        for (j = 1; j < cols - 1; j++)
        {
            Rect roi = Rect(i - 1, j - 1, kWidth, kHeight);
            Mat region = img(roi);
            lbpV = lbp(region, (int)((kWidth * kHeight) / 2), (kWidth * kHeight) - 1);
            cout << lbpV << endl;
            histogram.at(lbpV)++;
            lbpImg.at<uchar>(j, i) = (uchar)lbpV;
        }
    }
    return lbpImg;
}

/* Get the region ulbp */
vector<int> regionUlbpH(Mat reg, int kWidth, kHeight)
{
    vector<int> histogram;
    int row = reg.rows, cols = reg.cols;
}

/* Computes the ulbp histogram per region for the img image */
void ulbpHistogram(Mat img, vector< vector<int> > &histograms, int rWidth, int rHeight)
{
    int rows = img.rows, cols = img.cols;
    histograms.clear();
    int i, j;
    for (i = 1; i < rows; i++)
    {
        for (j = 1; j < cols; j++)
        {
            Rect region = Rect(i - 1, j - 1, rWidth, rHeight);
            Mat Region = img(roi);
        }
    }
}

/* A contains a representation of a binary number */
void increment(vector<int> A)
{
    int i = 0;
    int n = A.size();
    for (i = 0; i < n; i++)
    {
        A.at(i) = 0;
        i = i + 1;
    }
    if (i < n)
    {
        A.at(i) = 1;
    }
}

/* Generates all the ULBP patterns values for a veccinity of p pixels*/
void generateUlbpValues(vector<int> &bi, int p)
{
    int n = p * (p - 1) + 3;
    bi.clear();
    bi.assign(n, 0);
    vector<int> A;
    A.assign(p, 0);
    int countChgs;
    for (int i = 0; i < n - 1; i++)
    {
        // Get the number of changes 0 - 1, 1 - 0 on A
        countChgs = 0;
        for (int j = 1; j < p; j++)
        {
            if (A.at(i - 1) != A.at(i) )
            {
                countChgs++;
            }
        }
        if (countChgs <= 2)
        {
            // Get then integer value
            int value = 0;
            for (int j = 0; j < p; j++)
            {
                value += pow(2, A.at(i));
            }
            bi.push_back(value);
        }
    }
}


int main(int argc, char **argv)
{
    Mat img = imread("faces/image_0001.jpg");
    if (img.empty())
    {
        cout << "Could not load image." << endl;
        return -1;
    }
    //  Graying
    Mat imgGray;
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    // Apply histogram equalization
    Mat eqImg;
    equalizeHist(imgGray, eqImg);

    // Load the trained classifiers
    CascadeClassifier face_cascade = CascadeClassifier("haarcascade_frontalface_default.xml");

    // Find faces on image
    vector<Rect> facesR;
    vector<Mat> faces;
    Mat extImg;
    face_cascade.detectMultiScale(eqImg, facesR, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
    for (size_t i = 0; i < facesR.size(); i++)
    {
        Point corner1(facesR[i].x, facesR[i].y );
        Point corner2(facesR[i].x + facesR[i].width, facesR[i].y + facesR[i].height);
        // Extract ROI of face and extend borders of image (1 pixel)
        copyMakeBorder(eqImg(facesR[i]), extImg, 1, 1, 1, 1, BORDER_REPLICATE);
        faces.push_back(extImg);
        rectangle(img, corner1, corner2, Scalar(255, 0, 0));
    }

    // Get the LBP image
    vector<int> histogram;
    Mat lbpImg = gray2lbp(faces[0], histogram, 3, 3);

    String window1 = "Original Image";
    String window2 = "Face";
    String window3 = "LBP face";

    namedWindow(window1, WINDOW_AUTOSIZE);
    namedWindow(window2, WINDOW_AUTOSIZE);
    namedWindow(window3, WINDOW_AUTOSIZE);

    imshow(window1, img);
    imshow(window2, faces[1]);
    imshow(window3, lbpImg);

    waitKey(0);
    destroyAllWindows();
    return 0;
}
