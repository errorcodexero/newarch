#include "opencv2/opencv.hpp"
#include <iostream>
 
int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Specify video source to open.\n";
        return -1;
    }
    const std::string video_source(argv[1]);
 
    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    cv::VideoCapture cap(video_source); 
    
    // Check if video source opened successfully
    if (!cap.isOpened()){
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }
     
    cv::Mat frame;
    while (1) {
 
        // Capture frame-by-frame
        cap >> frame;
  
        // If the frame is empty, break immediately
        if (frame.empty())
            break;
 
        // Display the resulting frame
        cv::imshow("Frame", frame );
 
        // Press  ESC on keyboard to exit
        char c=(char)cv::waitKey(25);
        if (c==27) {
            break;
        }
    }
  
    // When everything done, release the video capture object
    cap.release();
 
    // Closes all the frames
    cv::destroyAllWindows();
     
    return 0;
}
