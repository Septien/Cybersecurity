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

/* A contains a representation of a binary number */ 
void increment(vector<int> &A)
{
    int i = 0;
    int n = A.size();
    while (i < n && A[i] == 1)
    {
        A[i++] = 0;
    }
    if (i < n)
    {
        A.at(i) = 1;
    }
}

/* Generates all the ULBP patterns values for a veccinity of p pixels*/
void generateUlbpValues(vector<int> &bi, int p)
{
    int n = p * (p - 1) + 2;
    int m = pow(2, p);
    bi.clear();
    vector<int> A;
    A.assign(p, 0);
    int countChgs;
    for (int i = 0; i < m; i++)
    {
        // Get the number of changes 0 - 1, 1 - 0 on A
        countChgs = 0;
        for (int j = 1; j < p; j++)
        {
            if (A.at(j - 1) != A.at(j) )
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
                value += A.at(j) * pow(2, j);
            }
            bi.push_back(value);
        }
        increment(A);
    }
}

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

// Uniform LBP. Returns the number of 0-1 and 1-0 transitions on a lbp pattern
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

/* Local binary pattern. Returns the lbp-code value of the kernel.
    c -> the pixel used as threshold.
    pKernel -> Number of pixels on kernel, besides c.
*/
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

/* Transforms a gray-scale image to an lbp-code based image. */
Mat gray2lbp(Mat img, int kWidth, int kHeight)
{
    int rows = img.rows, cols = img.cols;
    Mat lbpImg = Mat(rows, cols, CV_8U, Scalar(0));
    int i, j;
    int lbpV = 0;
    for (i = 1; i < rows - 1; i++)
    {
        for (j = 1; j < cols - 1; j++)
        {
            Rect roi = Rect(i - 1, j - 1, kWidth, kHeight);
            Mat region = img(roi);
            lbpV = lbp(region, (int)((kWidth * kHeight) / 2), (kWidth * kHeight) - 1);
            lbpImg.at<uchar>(j, i) = (uchar)lbpV;
        }
    }
    return lbpImg;
}

void upgradeHistogram(vector<double> &histogram, vector<int> &bk, int lbpV)
{
    int k, n = bk.size();
    for (k = 0; k < n - 1; k++)
    {
        if (lbpV == bk.at(k))
        {
            histogram.at(k)++;
            break;
        }
    }
}

vector<double> normalizeHistogram(vector<double> histogram)
{
    vector<double> h(histogram);
    int hTotal = 0;
    int n = histogram.size(), i;
    // Compute the total value of the histogram
    for (i = 0; i < n; i++)
    {
        hTotal += histogram.at(i);
    }
    // Normalize histogram
    for (i = 0; i < n; i++)
    {
        h.at(i) /= hTotal;
    }
    return h;
}

/* Get the region ulbp histogram.
    reg -> the extended region over wich to calculate the histogram.
    kWidth, kHeight -> widht and height of the kernel.
    bk -> array with decimal values uniform lbp.
 */
vector<double> regionUlbpH(Mat reg, vector<int> &bk, int kWidth, int kHeight)
{
    // Create and initilize histogram
    int n = bk.size();
    vector<double> histogram;
    histogram.assign(n, 0);
    int rows = reg.rows, cols = reg.cols;
    // Compute the LBP value for each pixel, and check if it is uniform
    int i, j, k;
    int ulbpV = 0, lbpV = 0;
    // For each pixel of the region, compute the lbp value, and check if it is uniform or not,
    // update histogram accordingly.
    for (i = 1; i < rows - 1; i++)
    {
        for (j = 1; j < cols - 1; j++)
        {
            Rect kernelR = Rect(i - 1, j - 1, kWidth, kHeight);
            Mat kernel = reg(kernelR);
            ulbpV = ulbp(kernel, (int)((kWidth * kHeight) / 2), (kWidth * kHeight) - 1);
            lbpV = lbp(kernel, (int)((kWidth * kHeight) / 2), (kWidth * kHeight) - 1);
            // Check if pattern is uniform
            if (ulbpV <= 2)
            {
                // Pattern is uniform
                upgradeHistogram(histogram, bk, lbpV);
            }
            else
            {
                // Pattern is non-uniform
                histogram.at(n - 1)++;
            }
        }
    }
    return normalizeHistogram(histogram);
}

/* Computes the ulbp histogram per region for the img image */
void ulbpHistogram(Mat img, vector< vector<double> > &histograms, vector<int> &bk, int rWidth, int rHeight)
{
    int rows = img.rows, cols = img.cols;
    histograms.clear();
    int i, j;
    // Generate the regions
    for (i = 1; i < rows - rHeight - 1; i += rWidth)
    {
        for (j = 1; j < cols - rWidth - 1; j += rHeight)
        {
            Rect region = Rect(j - 1, i - 1, rHeight + 1, rWidth + 1);
            Mat Region = img(region);
            // Generate the region histogram and append to histogram
            vector<double> h(regionUlbpH(Region, bk, 3, 3));
            histograms.push_back(h);
        }
    }
}

/* Reads an image and returns its color version, and gray version with histogram equlization. */
void readImage(String filename, Mat &clrImg, Mat &eqImg)
{
    Mat img = imread(filename);
    if (img.empty())
    {
        cout << "Could not load image: " << filename << endl;
        exit(-1);
    }
    //  Graying
    Mat imgGray;
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    // Apply histogram equalization
    equalizeHist(imgGray, eqImg);
    clrImg = img;
}

/* Apply the Haar algorith for finding faces on the image (front-looking). */
void returnFaces(Mat eqImg, vector<Mat> &faces)
{
    // Load the trained classifiers
    CascadeClassifier face_cascade = CascadeClassifier("haarcascade_frontalface_default.xml");

    // Find faces on image
    vector<Rect> facesR;
    Mat extImg;
    face_cascade.detectMultiScale(eqImg, facesR, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
    for (size_t i = 0; i < facesR.size(); i++)
    {
        Point corner1(facesR[i].x, facesR[i].y );
        Point corner2(facesR[i].x + facesR[i].width, facesR[i].y + facesR[i].height);
        // Extract ROI of face and extend borders of image (1 pixel)
        copyMakeBorder(eqImg(facesR[i]), extImg, 1, 1, 1, 1, BORDER_REPLICATE);
        faces.push_back(extImg);
    }
}

int main(int argc, char **argv)
{
    String ext = ".jpg";
    String fullName;
    int totalImg = 41;
    int i = 0;
    Mat img, grayImg, imgEq;
    String name;
    for (i = 1; i <= totalImg; i++)
    {
        name = "faces/image_0";
        char buffer[4];
        if (i < 10)
        {
            sprintf(buffer, "00%d", i);
        }
        else if (i < 100)
        {
            sprintf(buffer, "0%d", i);
        }
        Mat img, eqImg;
        name += buffer;
        name += ext;
        readImage(name, img, eqImg);
        vector<Mat> faces;
        returnFaces(eqImg, faces);
        Mat lbpImg;
        lbpImg = gray2lbp(faces[0], 3, 3);
        vector<int> bk;
        int p = 8;
        generateUlbpValues(bk, p);
        vector< vector<double> > histograms;
        ulbpHistogram(eqImg, histograms, bk, 16, 16);

        // Write results to file
        string file = "histograms/histograms" + to_string(i) + ".txt";
        ofstream out(file, ofstream::out);
        for (size_t i = 0; i < histograms.size(); i++)
        {
            size_t n = histograms[i].size();
            for (size_t j = 0; j < n - 1; j++)
            {
                out << histograms[i][j] << ",";
            }
            out << histograms[i][n - 1] << endl;
        }   
    }

    waitKey(0);
    destroyAllWindows();
    return 0;
}
