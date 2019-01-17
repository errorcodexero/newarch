#include <opencv2/opencv.hpp>
#include <iostream>
#include <StringUtils.h>
 
int main(int argc, char* argv[]) {
    const std::string output_filename = "capout.avi";
    
    if (argc == 1) {
        std::cout << "Specify video source to open.\n";
        return -1;
    }
    const std::string video_source(argv[1]);
 
    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    cv::VideoCapture cap;
    if (xero::string::hasOnlyDigits(video_source)) {
        cap.open(std::stoi(video_source));
    } else {
        cap.open(video_source);
    }
    
    // Check if video source opened successfully
    if (!cap.isOpened()){
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    // Default resolution of the frame is obtained. The default resolution is system dependent. 
    int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH); 
    int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); 
   
    // Define the codec and create VideoWriter object. 
    cv::VideoWriter video(output_filename,
                          CV_FOURCC('M','J','P','G'),
                          10,
                          cv::Size(frame_width,frame_height));

    std::cout << "Recording...\n";
    cv::Mat frame;
    while (1) {
 
        // Capture frame-by-frame
        cap >> frame;
  
        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        // Write the frame into the file
        video.write(frame);

#if 0
        // Display the resulting frame
        cv::imshow("Frame", frame );
 
        // Press  ESC on keyboard to exit
        char c=(char)cv::waitKey(25);
        if (c==27) {
            break;
        }
#endif
    }
  
    // When everything done, release the video capture object
    cap.release();
    video.release();
 
    // Closes all the frames
    cv::destroyAllWindows();
     
    return 0;
}
