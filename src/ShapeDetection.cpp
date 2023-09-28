//
// Created by ernesto on 21-9-23.
//

#include "ShapeDetection.hpp"

#include <cmath>
#include <iostream>

double lowerMarginSquare = 0.65;
double upperMarginSquare = 1.35;

double circleLowerMargin = 1.0;
double circleUpperMargin = 1.3;

double semiCircleLowerMargin = 0.6;
double semiCircleUpperMargin = 1.3;

double squareMarginFactor = 0.4;

long middlePointRadius = 3;

double areaSizeFilter = 1000;
double polyAproximation = 0.03;

cv::Scalar outlineColour = cv::Scalar(255, 0, 255);

void detectShape(const cv::Mat& mask, cv::Mat img, Shape shape, bool isInteractive)
{
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<std::vector<cv::Point>> conPoly(contours.size());
    std::vector<cv::Rect> boundRect(contours.size());

    if(shape == NO_SHAPE)
    {
    	return;
    }

    bool shapeFound = false;

    for (unsigned long i = 0; i < contours.size(); ++i)
    {
        double area = cv::contourArea(contours.at(i));

        if(area > areaSizeFilter)
        {
            double peri = cv::arcLength(contours.at(i), true);
            cv::approxPolyDP(contours.at(i), conPoly.at(i), polyAproximation * peri, true);

            boundRect.at(i) = cv::boundingRect(conPoly.at(i));

            unsigned long objCor = conPoly.at(i).size();


            switch (shape)
            {
                case TRIANGLE:
                {
                    if(objCor == 3)
                    {
                        cv::drawContours(img, conPoly, static_cast<int>(i), outlineColour, 2);
                        renderShapeInfo(img, conPoly, i, isInteractive);
                        std::cout << "Area: " << area << std::endl;
                        shapeFound = true;
                    }
                    break;
                }
                case SQUARE:
                {
                    std::vector<double> distances = getDistances(conPoly.at(i));
//                    if(objCor == 4 && ((std::abs(distances.front() - distances.back()) < squareMargin) || std::abs(distances.front() - distances.back()) < (distances.front() * squareMarginFactor)))
                    if(objCor == 4 &&
                        std::abs(distances.front() - distances.back()) < (distances.front() * squareMarginFactor) &&
                        ((distances.front() * distances.front()) > area * lowerMarginSquare && (distances.front() * distances.front()) < area * upperMarginSquare))
                    {
                        cv::drawContours(img, conPoly, static_cast<int>(i), outlineColour, 2);
                        renderShapeInfo(img, conPoly, i, isInteractive);
                        std::cout << "Area: " << area << std::endl;
                        shapeFound = true;
                    }
                    break;
                }
                case RECTANGLE:
                {
                    std::vector<double> distances = getDistances(conPoly.at(i));
                    if(objCor == 4 && !(std::abs(distances.front() - distances.back()) < (distances.front() * squareMarginFactor) &&
                                      ((distances.front() * distances.front()) > area * lowerMarginSquare && (distances.front() * distances.front()) < area * upperMarginSquare)))
                    {
                        cv::drawContours(img, conPoly, static_cast<int>(i), outlineColour, 2);
                        renderShapeInfo(img, conPoly, i, isInteractive);
                        std::cout << "Area: " << area << std::endl;
                        shapeFound = true;
                    }
                    break;
                }
                case CIRCLE:
                {
                    int diameter = (boundRect.at(i).width) / 2;
                    if (objCor > 4 && ((calcCircleArea(diameter) > area * circleLowerMargin) && (calcCircleArea(diameter) < area * circleUpperMargin)))
                    {
                        cv::drawContours(img, conPoly, static_cast<int>(i), outlineColour, 2);
                        renderShapeInfo(img, conPoly, i, isInteractive);
                        std::cout << "Area: " << area << std::endl;
                        shapeFound = true;
                    }
                    break;
                }
                case SEMI_CIRCLE:
                {
                    if (objCor > 4 &&
                    (calcSemiCircleArea(static_cast<int>(calcLongestLine(conPoly.at(i)) / 2)) > area * semiCircleLowerMargin &&
                    calcSemiCircleArea(static_cast<int>(calcLongestLine(conPoly.at(i)) / 2)) < area * semiCircleUpperMargin))
                    {
                        cv::drawContours(img, conPoly, static_cast<int>(i), outlineColour, 2);
                        renderShapeInfo(img, conPoly, i, isInteractive);
                        std::cout << "Area: " << area << std::endl;
                        shapeFound = true;
                    }
                    break;
                }
                case NO_SHAPE:
                {
                	break;
                }
            }
        }
    }

    if(!shapeFound)
    {
        std::cout << "No Shape found" << std::endl;
    }
}

void renderShapeInfo(cv::Mat image, std::vector<std::vector<cv::Point>> conPoly, unsigned long index, bool isInteractive)
{
    cv::drawContours(image, conPoly, static_cast<int>(index) ,outlineColour, 2);
    cv::Moments m = cv::moments(conPoly.at(index));
    double cx = m.m10 / m.m00;
    double cy = m.m01 / m.m00;

    renderCenterPointInfo(image, cx, cy, isInteractive);
}

void renderCenterPointInfo(cv::Mat img, double x, double y, bool isInteractive)
{
    if(isInteractive)
    {
        cv::circle(img, cv::Point(static_cast<int>(x), static_cast<int>(y)), static_cast<int>(middlePointRadius), outlineColour, cv::FILLED);
    }
    else
    {
        std::cout << "Coordinates:\nx: " << x << "\ty: " << y << std::endl;
    }
}

double calcCircleArea(int radius)
{
    return (pow(radius, 2) * M_PI);
}

double calcSemiCircleArea(int radius)
{
    return (calcCircleArea(radius) / 2);
}

double calcLongestLine(std::vector<cv::Point> conPoly)
{
    double length = 0;
    for (unsigned long i = 0; i < conPoly.size() - 1; ++i)
    {
        if (calcDistance(conPoly.at(i), conPoly.at(i + 1)) > length)
        {
            length = calcDistance(conPoly.at(i), conPoly.at(i + 1));
        }
    }
    if (calcDistance(conPoly.front(), conPoly.back()) > length)
    {
        length = calcDistance(conPoly.front(), conPoly.back());
    }
    return (length);
}

double calcDistance(const cv::Point& p0, const cv::Point& p1)
{
    return (sqrt(pow(p1.x - p0.x, 2) + pow(p1.y - p0.y, 2)));
}

std::vector<double> getDistances(std::vector<cv::Point> conPoly)
{
    std::vector<double> distances(conPoly.size());
    for (unsigned long j = 0; j < distances.size() - 1; ++j)
    {
        distances.at(j) = calcDistance(conPoly.at(j), conPoly.at(j + 1));
    }
    if(!distances.empty())
    {
        distances.push_back(calcDistance(conPoly.front(), conPoly.back()));
    }

    return (distances);
}
