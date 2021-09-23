#ifndef LINEINTERSECTS_H
#define LINEINTERSECTS_H

#include<opencv2\opencv.hpp>
#include<opencv2\imgcodecs.hpp>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\imgcodecs.hpp>

using namespace std;
using namespace cv;

void VecToLine(Mat image, Vec2f line) {
    cout << line << endl;
    // Lines: a(x) + b = c
    float a = -1 / tan(line[1]);
    float b = line[0] / sin(line[1]);

    cv::line(image, Point(0, b), Point(image.size().width, a*image.size().width + b), Scalar(255, 0, 0), 8, 5);
}

void VecToIntercept(Mat image, vector<Vec2f> lines) {
    for (uint i = 0; i < lines.size(); i++) {
        float a = -1 / tan(lines[i][1]);
        float b = lines[i][0] / sin(lines[i][1]);

        for (uint j = 0; j < lines.size(); j++) {
            float c = -1 / tan(lines[j][1]);
            float d = lines[j][0] / sin(lines[j][1]);

            if (i == j) continue;
            int x = (d - b) / (a - c);
            int y = (a*x) + b;

            circle(image, Point(x, y), 1, CV_RGB(255, 0, 0), 1);

        }
    }
}

void GetCenterSquares(Mat inputGrid, int outputArray[10][10]) {
    bool squares[10][10];
    uint width = inputGrid.size().width;
    uint height = inputGrid.size().height;
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 10; j++) {
            int x = (width * 19 / 20) - (i * width / 10);
            int y = (width * 19 / 20) - (j * width / 10);
            Vec3b colour = inputGrid.at<Vec3b>(Point(x, y));
            int threshold = 100;
            if (colour.val[0] < threshold && colour.val[1] < threshold && colour.val[2] < threshold) {
                squares[i][j] = true;
                circle(inputGrid, Point(x, y), 3, CV_RGB(0, 255, 0), -1);
                outputArray[i][j] = 1;
            }
            else {
                squares[i][j] = false;
                circle(inputGrid, Point(x, y), 3, CV_RGB(0, 0, 255), -1);
                outputArray[i][j] = 0;
            }

        }
    }
}

Mat WarpGrid(Mat rawImage, Mat processedImage, vector<Vec2f> lines) {
    //if (lines.size() < 4) return rawImage; // If the majority of the grid is not recognised then just skip this  as otherwise there is a huge memory usage.

    Vec2f topEdge = Vec2f(1000, 1000);
    double topYIntercept = 100000, topXIntercept = 0;
    Vec2f bottomEdge = Vec2f(-1000, -1000);
    double bottomYIntercept = 0, bottomXIntercept = 0;
    Vec2f leftEdge = Vec2f(1000, 1000);
    double leftXIntercept = 100000, leftYIntercept = 0;
    Vec2f rightEdge = Vec2f(-1000, -1000);
    double rightXIntercept = 0, rightYIntercept = 0;

    for (uint i = 0; i < lines.size(); i++) {
        Vec2f currentLine = lines[i];
        float rho = currentLine[0];
        float theta = currentLine[1];

        if (rho == 0 && theta == -100) //if the line has been marked as a duplicate: skip
            continue;

        double currentXInt, currentYInt;
        currentXInt = rho / cos(theta);
        currentYInt = rho / (cos(theta)*sin(theta));

        if (theta > CV_PI * 50 / 180 && theta < CV_PI * 130 / 180) {
            if (rho < topEdge[0])
                topEdge = currentLine;

            if (rho > bottomEdge[0])
                bottomEdge = currentLine;
        }
        else if (theta<CV_PI * 40 / 180 || theta>CV_PI * 140 / 180) {
            if (currentXInt > rightXIntercept) {
                rightEdge = currentLine;
                rightXIntercept = currentXInt;
            }
            else if (currentXInt <= leftXIntercept) {
                leftEdge = currentLine;
                leftXIntercept = currentXInt;
            }
        }
    }


    Point left1, left2, right1, right2, bottom1, bottom2, top1, top2;

    int height = processedImage.size().height;

    int width = processedImage.size().width;


    if (leftEdge[1] != 0) {
        left1.x = 0;
        left1.y = leftEdge[0] / sin(leftEdge[1]);
        left2.x = width;
        left2.y = -left2.x / tan(leftEdge[1]) + left1.y;
    }
    else {
        left1.y = 0;
        left1.x = leftEdge[0] / cos(leftEdge[1]);
        left2.y = height;
        left2.x = left1.x - height * tan(leftEdge[1]);

    }

    if (rightEdge[1] != 0) {
        right1.x = 0;
        right1.y = rightEdge[0] / sin(rightEdge[1]);
        right2.x = width;
        right2.y = -right2.x / tan(rightEdge[1]) + right1.y;
    }
    else {
        right1.y = 0;
        right1.x = rightEdge[0] / cos(rightEdge[1]);
        right2.y = height;
        right2.x = right1.x - height * tan(rightEdge[1]);

    }

    bottom1.x = 0;
    bottom1.y = bottomEdge[0] / sin(bottomEdge[1]);
    bottom2.x = width;
    bottom2.y = -bottom2.x / tan(bottomEdge[1]) + bottom1.y;

    top1.x = 0;
    top1.y = topEdge[0] / sin(topEdge[1]);
    top2.x = width;
    top2.y = -top2.x / tan(topEdge[1]) + top1.y;


    double leftA = left2.y - left1.y;
    double leftB = left1.x - left2.x;

    double leftC = leftA * left1.x + leftB * left1.y;

    double rightA = right2.y - right1.y;
    double rightB = right1.x - right2.x;

    double rightC = rightA * right1.x + rightB * right1.y;

    double topA = top2.y - top1.y;
    double topB = top1.x - top2.x;

    double topC = topA * top1.x + topB * top1.y;

    double bottomA = bottom2.y - bottom1.y;
    double bottomB = bottom1.x - bottom2.x;

    double bottomC = bottomA * bottom1.x + bottomB * bottom1.y;

    // Intersection of left and top
    double detTopLeft = leftA * topB - leftB * topA;

    Point ptTopLeft = Point((topB*leftC - leftB * topC) / detTopLeft, (leftA*topC - topA * leftC) / detTopLeft);

    // Intersection of top and right
    double detTopRight = rightA * topB - rightB * topA;

    Point ptTopRight = Point((topB*rightC - rightB * topC) / detTopRight, (rightA*topC - topA * rightC) / detTopRight);

    // Intersection of right and bottom
    double detBottomRight = rightA * bottomB - rightB * bottomA;
    Point ptBottomRight = Point((bottomB*rightC - rightB * bottomC) / detBottomRight, (rightA*bottomC - bottomA * rightC) / detBottomRight);// Intersection of bottom and left
    double detBottomLeft = leftA * bottomB - leftB * bottomA;
    Point ptBottomLeft = Point((bottomB*leftC - leftB * bottomC) / detBottomLeft, (leftA*bottomC - bottomA * leftC) / detBottomLeft);

    int maxLength = (ptBottomLeft.x - ptBottomRight.x)*(ptBottomLeft.x - ptBottomRight.x) + (ptBottomLeft.y - ptBottomRight.y)*(ptBottomLeft.y - ptBottomRight.y);

    int temp = (ptTopRight.x - ptBottomRight.x)*(ptTopRight.x - ptBottomRight.x) + (ptTopRight.y - ptBottomRight.y)*(ptTopRight.y - ptBottomRight.y);
    if (temp > maxLength) maxLength = temp;

    temp = (ptTopRight.x - ptTopLeft.x)*(ptTopRight.x - ptTopLeft.x) + (ptTopRight.y - ptTopLeft.y)*(ptTopRight.y - ptTopLeft.y);
    if (temp > maxLength) maxLength = temp;

    temp = (ptBottomLeft.x - ptTopLeft.x)*(ptBottomLeft.x - ptTopLeft.x) + (ptBottomLeft.y - ptTopLeft.y)*(ptBottomLeft.y - ptTopLeft.y);
    if (temp > maxLength) maxLength = temp;

    maxLength = (int)sqrt((double)maxLength);

    Point2f src[4], dst[4];
    src[0] = ptTopLeft;
    dst[0] = Point2f(0, 0);
    src[1] = ptTopRight;
    dst[1] = Point2f(maxLength - 1, 0);
    src[2] = ptBottomRight;
    dst[2] = Point2f(maxLength - 1, maxLength - 1);
    src[3] = ptBottomLeft;
    dst[3] = Point2f(0, maxLength - 1);

    Mat warped = rawImage;
    cv::warpPerspective(warped, warped, cv::getPerspectiveTransform(src, dst), Size(maxLength, maxLength));

    return warped;

}

#endif // LINEINTERSECTS_H
