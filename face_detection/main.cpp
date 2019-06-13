#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    // Load image as gray scale
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
    vector<Rect> faces;
    face_cascade.detectMultiScale(eqImg, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
    for (size_t i = 0; i < faces.size(); i++)
    {
        Point corner1(faces[i].x, faces[i].y );
        Point corner2(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
        rectangle(img, corner1, corner2, Scalar(255, 0, 0));
    }

    String window1 = "Original Image";

    namedWindow(window1, WINDOW_AUTOSIZE);

    imshow(window1, img);

    waitKey(0);
    destroyAllWindows;
    return 0;
}
