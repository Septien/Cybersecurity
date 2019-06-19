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

vector<uchar> toArray(Mat m)
{
    vector<uchar> neighbors;
    if (m.isContinuous())
    {
        neighbors.assign(m.data, m.data + m.total());
    }
    else
    {
        for (int i = 0; i < m.rows; ++i)
        {
            neighbors.insert(neighbors.end(), m.ptr<uchar>(i), m.ptr<uchar>(i) + m.cols);
        }
    }
}

// Local binary pattern
int lbp(Mat kernel, int c, int pKernel)
{
    // Store neighbors in array
    vector<uchar> neighbors = toArray(kernel);
    // Get the cth pixel
    int gc = (int)neighbors[c];
    int r = 0;
    int gp = 0;
    for (int i = 0; i < pKernel; i++)
    {
        gp = (int)neighbors[i];
        r += s(gp - gc) * pow(2, i);
    }
}

// Uniform LBP
int ulbp(Mat kernel, int c, int pKernel)
{
    // Store neighbors in array
    vector<uchar> neighbors = toArray(kernel);
    
    // Get the cth pixel
    int gc = (int)neighbors[c];
    // Get the 0th pixel
    int gp = (int)neighbors[0];
    int gp_1 = (int)neighbors[pKernel - 1];
    int a0 = fabs(s(gp_1 - gc) - s(gp - gc));
    for (int i = i; i < pKernel; i++)
    {
        gp_1 = (int)neighbors[i - 1];
        if (i == c)
        {
            continue;
        }
        if (i - 1 == c)
        {
            gp_1 = (int)neighbors[i - 2];
        }
        gp = (int)neighbors[i];
        a0 += fabs(s(gp - gc) - s(gp_1 - gc));
    }
    return a0;
}

vector<double> constructHistogram(Mat img, Rect Region, int P)
{
    int n = (P * (P - 1)) + 3;
    vector<double> histogram;
    histogram.assign(n, 0.0f);
    // Get the size of the region
    int rows = Region.width;
    int cols = Region.height;

    int i, j;
    // Divide the region into 3x3 kernels
    // Number of rows and columns of the kernel
    int kerWidth = 3, kerHeight = 3;
    Mat kernel;
    Rect roiR;
    int lbpV = 0, ulbpV;
    // Get a kernel and compute the LBP pattern
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            // Get the rectangle of the ROI
            roiR = Rect(Region.x + i - 1, Region.y + j - 1 , kerWidth, kerHeight);
            // Get the ROI
            kernel = img(roiR);
            // Compute the ULBP value (number of transitions)
            ulbpV = ulbp(kernel, int((kerWidth * kerHeight) / 2), (kerWidth * kerHeight) - 1);
            // Compute the LBP value
            lbpV = lbp(kernel, int((kerWidth * kerHeight) / 2), (kerWidth * kerHeight) - 1);
            if (ulbpV <= 2)
            {
                // Uniform pattern
                histogram[lbpV]++;
            }
            else
            {
                // Non-uniform pattern
                histogram[n - 1]++;
            }
        }
    }
    // Normalize histogram
    int total = 0;
    for (i = 0; i < histogram.size(); i++)
    {
        total += histogram[i];
    }
    for (i = 0; i < histogram.size(); i++)
    {
        histogram[i] /= total;
    }
    return histogram;
}

// m -> number of regions. w -> width of region
vector< vector<double> > totalHistogram(Mat img, int w, int h)
{
    vector< vector<double> > histograms;
    vector<double> histogram;
    int i, j;
    Rect Region;
    for (i = 1; i < img.rows - 1; i += w)
    {
        for (j = 1; j < img.cols - 1; j += h)
        {
            // Construct the histogram for each region
            Region = Rect(i, j , w, h);
            histograms.push_back(constructHistogram(img, Region, w * h));
        }
    }
    return histograms;
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

    // Compute the histogram
    vector< vector<double> > histograms;
    histograms = totalHistogram(faces[0], 8, 8);

    // Write results to file
    ofstream out("histograms.txt", ofstream::out);
    for (size_t i = 0; i < histograms.size(); i++)
    {
        size_t n = histograms[i].size();
        for (size_t j = 0; j < n - 1; j++)
        {
            out << histograms[i][j] << ", ";
        }
        out << histograms[i][n - 1] << endl;
    }

    String window1 = "Original Image";
    String window2 = "Face";

    namedWindow(window1, WINDOW_AUTOSIZE);
    namedWindow(window2, WINDOW_AUTOSIZE);

    imshow(window1, img);
    imshow(window2, faces[0]);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
