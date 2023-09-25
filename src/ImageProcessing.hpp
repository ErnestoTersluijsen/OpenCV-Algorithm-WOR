//
// Created by ernesto on 21-9-23.
//

#ifndef OPENCV_ALGORITHM_WOR_IMAGEPROCESSING_HPP
#define OPENCV_ALGORITHM_WOR_IMAGEPROCESSING_HPP

#include <opencv2/imgproc.hpp>

/**
 * @brief enum that contains the different colors that can be detected
 * 
 */
enum Colour {
    GREEN,
    PINK,
    YELLOW,
    ORANGE,
    NONE
};

/**
 * @brief function that uses different openCV filters to convert the image to a binary image with an specific color filter
 * 
 * @param img input image
 * @param dst colour converted and blured image
 * @param mask output image (binary image with filtered colours)
 * @param color color that gets filtered
 */
void processImage(cv::Mat &img, cv::Mat &dst, cv::Mat &mask, Colour color);

#endif //OPENCV_ALGORITHM_WOR_IMAGEPROCESSING_HPP
