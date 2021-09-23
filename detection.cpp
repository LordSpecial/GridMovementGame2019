#include "opencv2/opencv.hpp"
#include "lineDetection.h"
#include "lineIntersects.h"

#include <QDebug>

void DetectGrid(int * playerLocation, Mat cameraFrame) {

    //        namedWindow("Raw");
    //        imshow("Raw", cameraFrame);

    Mat processedImage = PreProcessing(cameraFrame);
    Mat isolatedGrid = FindGrid(processedImage);

    vector<Vec2f> lines = DetectLines(isolatedGrid, cameraFrame);

    Mat warpedGrid = WarpGrid(cameraFrame, isolatedGrid, lines);

    int squaresArray[10][10];
    GetCenterSquares(warpedGrid, squaresArray);
    int nPlayerLocation[2] = {99,99};
    for (int i = 0; i<10;i++) {
        for (int j = 0; j<10; j++) {
            if (squaresArray[i][j] == 1){
                if (nPlayerLocation[0] == 99) {
                    nPlayerLocation[0] = i;
                    nPlayerLocation[1] = j;
                } else {
                    nPlayerLocation[0] = 98;
                }
            }
        }
    }
    if (nPlayerLocation[0] < 10) {
        playerLocation[0] = nPlayerLocation[0];
        playerLocation[1] = nPlayerLocation[1];
    }
    //        namedWindow("squares");
//            imshow("warpedGrid", warpedGrid);
}

