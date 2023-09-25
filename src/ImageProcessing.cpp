//
// Created by ernesto on 21-9-23.
//

#include "ImageProcessing.hpp"

#include <opencv2/imgproc.hpp>

cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

cv::Size blurStrength(5, 5);

cv::Point anchorPoint(-1, -1);

cv::Scalar lowerGreen(70, 50, 50);
cv::Scalar upperGreen(150, 255, 255);

cv::Scalar lowerPink(150, 50, 50);
cv::Scalar upperPink(300, 255, 255);

cv::Scalar lowerYellow(25, 50, 50);
cv::Scalar upperYellow(50, 255, 255);

cv::Scalar lowerOrange(0, 100, 100);
cv::Scalar upperOrange(25, 255, 255);



void processImage(cv::Mat& img, cv::Mat& dst, cv::Mat& mask, Colour color)
{
    cv::cvtColor(img, dst, cv::COLOR_BGR2HSV_FULL);
    cv::GaussianBlur(dst, dst, blurStrength, -1, -1);

    switch (color) {
        case GREEN:
        {
            cv::inRange(dst, lowerGreen, upperGreen, mask);
            break;
        }
        case PINK:
        {
            cv::inRange(dst, lowerPink, upperPink, mask);
            break;
        }
        case YELLOW:
        {
            cv::inRange(dst, lowerYellow, upperYellow, mask);
            break;
        }
        case ORANGE:
        {
            cv::inRange(dst, lowerOrange, upperOrange, mask);
            break;
        }
        case NONE:
            cv::inRange(dst, cv::Scalar(0,0,0), cv::Scalar(0,0,0), mask);
            break;
    }

    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel, anchorPoint, 1);
}