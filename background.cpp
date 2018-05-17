// g++ -o background background.cpp `pkg-config --libs opencv`

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/bgsegm.hpp>

int main() {

    // load video file
    cv::VideoCapture capture("video1.mov");
    // cv::VideoCapture stream(0);

	// ensure video can be opened
	if (!capture.isOpened())
		return 0;

    // create frames
    cv::Mat frame;
    cv::Mat background;
    cv::Mat foreground;
    cv::Mat contours;

    cv::Mat rframe;
    cv::Mat rbackground;
    cv::Mat rforeground;
    cv::Mat rcontours;

    // create windows
    cv::namedWindow("Video");
    cv::namedWindow("Background");
    cv::namedWindow("Foreground");
    cv::namedWindow("Canny");

    cv::Ptr<cv::BackgroundSubtractor> ptrMOG2 = cv::createBackgroundSubtractorMOG2();

    int count = 0;

    while(true) {

        // Print current frame
        std::cout << "Frame: " << count << std::endl;

        // if no more frame, quit
        if (!capture.read(frame))
			break;

        // read frame
        capture.read(frame);

        // apply MOG2 & extract background
        ptrMOG2->apply(frame,foreground,0.01);
        ptrMOG2->getBackgroundImage(background);

        cv::resize(frame, rframe, cv::Size(688, 387));
        cv::resize(foreground, rforeground, cv::Size(688, 387));
        cv::resize(background, rbackground, cv::Size(688, 387));

        // display results
        cv::imshow("Video",rframe);
        cv::imshow("Foreground",rforeground);
        cv::imshow("Background",rbackground);

        cv::moveWindow("Video", 20, 20);
        cv::moveWindow("Foreground", 728, 20);
        cv::moveWindow("Background", 728, 440);

        if(count%100 == 0){
            // Apply Canny algorithm
            cv::Canny(background,contours,125,350);
            cv::resize(contours, rcontours, cv::Size(688, 387));

            // Display the image
            cv::imshow("Canny", rcontours);
            cv::moveWindow("Canny", 20, 440);
        }

        cv::waitKey(40);

        count++;
    }

    cv::waitKey();
}