//
// Created by ernesto on 21-9-23.
//

#ifndef OPENCV_ALGORITHM_WOR_SHAPEDETECTION_HPP
#define OPENCV_ALGORITHM_WOR_SHAPEDETECTION_HPP

#include <opencv2/imgproc.hpp>

/**
 * @brief enum that contains the different shapes that can be filtered
 * 
 */
enum Shape {
    TRIANGLE,
    SQUARE,
    RECTANGLE,
    CIRCLE,
    SEMI_CIRCLE,
	NO_SHAPE
};

/**
 * @brief function that detects the shape given a specific mask
 * 
 * @param mask mask used to detect the shape
 * @param img image where all effects are drawn on
 * @param shape shape that needs to be detected
 * @param isInteractive whether the program should run in interactive mode or not
 */
void detectShape(const cv::Mat& mask, cv::Mat img, Shape shape, bool isInteractive);

/**
 * @brief function that draws the contours on the image and calls for the information to be sent to std::cout / to be drawn on screen
 *
 * @param image image that the effect are drawn on
 * @param conPoly a vector containing a vector that contains all the points for the shapes
 * @param index selects the specific shape you want to draw on screen
 * @param isInteractive whether it should run in interactive mode
 */
void renderShapeInfo(cv::Mat image, std::vector<std::vector<cv::Point>> conPoly, unsigned long index, bool isInteractive);

/**
 * @brief renders a dot in interactive mode on middle of object or x & y in console if batch mode is on
 *
 * @param img image where effects are drawn on
 * @param x x coordinate
 * @param y y coordinate
 * @param isInteractive whether it should run in interactive mode
 */
void renderCenterPointInfo(cv::Mat img, double x, double y, bool isInteractive);

/**
 * @brief calculates the area of a circle based on the radius
 *
 * @param radius radius of the circle
 * @return area of the circle as a double
 */
double calcCircleArea(int radius);

/**
 * @brief calculates the area of a semi circle based on the radius
 *
 * @param radius radius of semi circle
 * @return returns the area as a double
 */
double calcSemiCircleArea(int radius);

/**
 * @brief calculates the longest line based on an vector of points
 *
 * @param conPoly vector with points
 * @return longest line between points as a double
 */
double calcLongestLine(std::vector<cv::Point> conPoly);

/**
 * @brief calculates the distance between 2 points
 *
 * @param p0 point 1
 * @param p1 point 2
 * @return distance between the 2 points
 */
double calcDistance(const cv::Point& p0, const cv::Point& p1);

/**
 * @brief returns a vector of distances between points
 * 
 * @param conPoly vector with points
 * @return vector with doubles with distance between points
 */
std::vector<double> getDistances(std::vector<cv::Point> conPoly);



#endif //OPENCV_ALGORITHM_WOR_SHAPEDETECTION_HPP
