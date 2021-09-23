#ifndef LINEDETECTION_H
#define LINEDETECTION_H

#include<opencv2/opencv.hpp>
#include<opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;



Mat PreProcessing(Mat rawCameraFrame) {
    Mat ProcessedCameraFrame = rawCameraFrame; // Create second Mat to write adaptive thresholding on.

    cvtColor(rawCameraFrame, rawCameraFrame, COLOR_BGR2GRAY);
    GaussianBlur(rawCameraFrame, rawCameraFrame, Size(11, 11), 0);

    adaptiveThreshold(rawCameraFrame, ProcessedCameraFrame, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);
    bitwise_not(ProcessedCameraFrame, ProcessedCameraFrame);

    return ProcessedCameraFrame;
}


Mat FindGrid(Mat processedImage) {
    // Find Biggest Blob
    int max = -1;
    Point maxPt;

    for (int y = 0; y < processedImage.size().height; y++) {
        uchar *row = processedImage.ptr(y);
        for (int x = 0; x < processedImage.size().width; x++) {
            if (row[x] >= 128) {
                int area = floodFill(processedImage, Point(x, y), CV_RGB(0, 0, 64));
                if (area > max)
                {
                    maxPt = Point(x, y);
                    max = area;
                }
            }
        }
    }

    // Flood biggest blob with white
    floodFill(processedImage, maxPt, CV_RGB(255, 255, 255));

    // Remove all other blobs
    for (int y = 0; y < processedImage.size().height; y++) {
        uchar *row = processedImage.ptr(y);
        for (int x = 0; x < processedImage.size().width; x++) {
            if (row[x] == 64 && x != maxPt.x && y != maxPt.y) {
                floodFill(processedImage, Point(x, y), CV_RGB(0, 0, 0));
            }
        }
    }
    erode(processedImage, processedImage, Mat());

    return processedImage;
}

void mergeRelatedLines(vector<Vec2f> *lines, Mat &img) {
    vector<Vec2f>::iterator currentLine;

    for (currentLine = lines->begin(); currentLine != lines->end(); currentLine++) {
        if ((*currentLine)[0] == 0 && (*currentLine)[1] == -100) continue;

        float currentRho = (*currentLine)[0];
        float currentTheta = (*currentLine)[1];

        Point currentPt1, currentPt2;

        if (currentTheta > CV_PI * 45 / 180 && currentTheta < CV_PI * 135 / 180) { // Horizontal Line
            currentPt1.x = 0; //pt1 x = far left
            currentPt1.y = currentRho / sin(currentTheta);

            currentPt2.x = img.size().width; //pt2 x = far right
            currentPt2.y = -currentPt2.x / tan(currentTheta) + currentRho / sin(currentTheta);
        }
        else { // Vertical Line

            currentPt1.y = 0; // pt1 y = top
            currentPt1.x = currentRho / cos(currentTheta);

            currentPt2.y = img.size().height; //pt2 y = bottom
            currentPt2.x = -currentPt2.y * tan(currentTheta) + currentRho / cos(currentTheta);

        }

        vector<Vec2f>::iterator compareLine;
        for (compareLine = lines->begin(); compareLine != lines->end(); compareLine++)
        {
            if (*currentLine == *compareLine) continue; // If comparing the same line continue
            if (fabs((*compareLine)[0] - (*currentLine)[0]) < 20 && fabs((*compareLine)[1] - (*currentLine)[1]) < CV_PI * 10 / 180)
            {
                float compareRho = (*compareLine)[0];
                float compareTheta = (*compareLine)[1];
                Point comparePt1, comparePt2;
                if ((*compareLine)[1] > CV_PI * 45 / 180 && (*compareLine)[1] < CV_PI * 135 / 180) // Horizontal
                {
                    comparePt1.x = 0;
                    comparePt1.y = compareRho / sin(compareTheta);
                    comparePt2.x = img.size().width;
                    comparePt2.y = -comparePt2.x / tan(compareTheta) + compareRho / sin(compareTheta);
                }
                else //Vertical
                {
                    comparePt1.y = 0;
                    comparePt1.x = compareRho / cos(compareTheta);
                    comparePt2.y = img.size().height;
                    comparePt2.x = -comparePt2.y * tan(compareTheta) + compareRho / cos(compareTheta);
                }
                int sensitivity = 64;
                if (((double)(comparePt1.x - currentPt1.x)*(comparePt1.x - currentPt1.x) + (comparePt1.y - currentPt1.y)*(comparePt1.y - currentPt1.y) < sensitivity * sensitivity) && ((double)(comparePt2.x - currentPt2.x)*(comparePt2.x - currentPt2.x) + (comparePt2.y - currentPt2.y)*(comparePt2.y - currentPt2.y) < sensitivity * sensitivity))
                {
                    // Merge the two
                    (*currentLine)[0] = ((*currentLine)[0] + (*compareLine)[0]) / 2;

                    (*currentLine)[1] = ((*currentLine)[1] + (*compareLine)[1]) / 2;

                    (*compareLine)[0] = 0;
                    (*compareLine)[1] = -100;
                }
            }
        }
    }
}


vector<Vec2f> DetectLines(Mat isolatedGrid, Mat rawImage) {


    vector<Vec2f> lines;
    HoughLines(isolatedGrid, lines, 1, CV_PI / 180, 200);

    Mat outputImage = rawImage; //allows lines to be drawn on the original image without overriding it

    mergeRelatedLines(&lines, outputImage);

    return lines;
}
#endif // LINEDETECTION_H
