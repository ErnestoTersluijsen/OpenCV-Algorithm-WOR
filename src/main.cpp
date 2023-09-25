#include "ImageProcessing.hpp"
#include "ShapeDetection.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>

const long WEBCAM_DEVICE_ID = 0;
bool isInteractive = true;

bool run = true;
Colour colour = YELLOW;
Shape shape = SQUARE;

std::mutex runBooleanMutex;
std::mutex colourMutex;
std::mutex shapeMutex;

void parseCommands(const std::string& filename);

Shape getShape();
void setShape(Shape s);

Colour getColour();
void setColour(Colour c);

void setRun(bool b);
bool getRun();

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    std::string filename;
    if(argc > 1)
    {
        isInteractive = false;
        std::string fileLocation(argv[1]);
        filename = fileLocation;
    }

    std::thread inputThread(parseCommands, filename);

    cv::VideoCapture cap(WEBCAM_DEVICE_ID, cv::CAP_V4L2);

    cv::Mat img, processedImg, mask;



    while (getRun())
    {
        cap.read(img);

        int64 clockTickStart = cv::getTickCount();
        processImage(img, processedImg, mask, getColour());

        detectShape(mask, img, getShape(), isInteractive);
        std::cout << "Clock Ticks: " << cv::getTickCount() - clockTickStart << std::endl << std::endl;

        cv::imshow("Shape & Colour detection", img);
        // cv::imshow("Processed Image", processedImg);
        cv::imshow("Mask", mask);

        cv::waitKey(1);
    }

    inputThread.join();

    return 0;
}

void parseCommands(const std::string& filename) {
    std::stringstream buffer;
    if(!filename.empty())
    {
        std::fstream file(filename);
        buffer << file.rdbuf();
        file.close();
    }

    while (getRun())
    {
        std::string string;
        std::vector<std::string> tokens;

        Shape shape1;
        Colour colour1;


        if(filename.empty())
        {
            std::getline(std::cin, string);
        }
        else
        {
            std::getline(buffer, string);
        }

        if (string == "exit")
        {
            setRun(false);
            break;
        }

        unsigned long commentPosition = string.find('#');
        if (commentPosition != std::string::npos)
        {
            string.erase(commentPosition);
        }

        std::istringstream iss(string);
        std::string word;

        while (iss >> word)
        {
            tokens.push_back(word);
        }

        if (tokens.size() > 1) {
            if (tokens.front() == "driehoek") {
                shape1 = TRIANGLE;
            } else if (tokens.front() == "vierkant") {
                shape1 = SQUARE;
            } else if (tokens.front() == "rechthoek") {
                shape1 = RECTANGLE;
            } else if (tokens.front() == "cirkel") {
                shape1 = CIRCLE;
            } else if (tokens.front() == "halve") {
                shape1 = SEMI_CIRCLE;
            } else {
                std::cout << "incorrecte invoer" << std::endl;
                continue;
            }

            if (tokens.back() == "groen") {
                colour1 = GREEN;
            } else if (tokens.back() == "roze") {
                colour1 = PINK;
            } else if (tokens.back() == "geel") {
                colour1 = YELLOW;
            } else if (tokens.back() == "oranje") {
                colour1 = ORANGE;
            } else {
                std::cout << "incorrecte invoer" << std::endl;
                continue;
            }

            setShape(shape1);
            setColour(colour1);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}


Shape getShape()
{
    std::lock_guard<std::mutex> lock(shapeMutex);
    return shape;
}

void setShape(Shape s)
{
    std::lock_guard<std::mutex> lock(shapeMutex);
    shape = s;
}

Colour getColour()
{
    std::lock_guard<std::mutex> lock(colourMutex);
    return colour;
}

void setColour(Colour c)
{
    std::lock_guard<std::mutex> lock(colourMutex);
    colour = c;
}

void setRun(bool b)
{
    std::lock_guard<std::mutex> lock(runBooleanMutex);
    run = b;
}

bool getRun()
{
    std::lock_guard<std::mutex> lock(runBooleanMutex);
    return run;
}

