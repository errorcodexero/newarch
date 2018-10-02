#include <iostream>
#include "FileUtils.h"
#include "StringUtils.h"
#include "Cube.h"

//#include <cstdlib>
#include <libv4l2.h>
#include <linux/videodev2.h>
//#include <sys/ioctl.h>
#include <fcntl.h>


int main(int argc, char **argv) {

    // Read param file
    if (argc < 2) {
        std::cout << "Please specify the params file.\n";
        return 1;
    }
    std::string params_file(argv[1]);
    paramsInput params;
    params.readFile(params_file);

    std::string image_source_file(params.getString("FILENAME", ""));
    if (!xero::file::exists(image_source_file)) {
        std::cout << "Input image file '" << image_source_file << "' does not exist.\n";
        return 1;
    }
    if (!xero::file::is_readable(image_source_file)) {
        std::cout << "Input image file '" << image_source_file << "' is unreadable.\n";
        return 1;
    }

    // Infer source type (image, video file, camera)
    const bool is_image = xero::string::endsWith(xero::string::downcase(image_source_file), ".jpg");
    const bool is_video = xero::string::endsWith(xero::string::downcase(image_source_file), ".mov") ||
                          xero::string::endsWith(xero::string::downcase(image_source_file), ".mp4");
    const bool is_camera = xero::string::startsWith(image_source_file, "/dev/video");
    if (!is_image && !is_video && !is_camera) {
        std::cout << "Unrecognized input source type '" << image_source_file << "'\n";
        return 1;
    }

    if (is_image) {
        // Open image
        cv::Mat frame = cv::imread(image_source_file, cv::IMREAD_COLOR);
        if (!frame.data) {
            std::cout << "Could not read image\n";
            return 1;
        }
        
        // Show original image
        cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
        cv::moveWindow("Original", 0, 0);
        cv::imshow("Original", frame);
        
        // Detect and show cubes
        Cube cube(frame, params);
        
        //std::cout << cube.getPosition() << std::endl;
        //cube.getPosition(Cube::detectionMode::CONTOURS);
        //cv::imshow("Cube", cube.showFrame());
        //cv::imshow("Camera", frame);
        //cv::imshow("Video", frame);
        
        cv::waitKey(0);
        
    } else if (is_video) {

        bool playVideo = true;
        char key;
        cv::VideoCapture capture(image_source_file);

        cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
        cv::moveWindow("Original", 0, 0);
        
        cv::Mat frame;
        while(capture.get(cv::CAP_PROP_POS_FRAMES)<capture.get(cv::CAP_PROP_FRAME_COUNT)) {

            if (!capture.isOpened()) {
                std::cout << "Could not read video\n";
                return 1;
            }
            if (playVideo) {
                capture >> frame;
                    
                cv::imshow("Original", frame);
                
                Cube cube(frame, params);
                
                //std::cout << cube.getPosition() << std::endl;
                //cube.getPosition(Cube::detectionMode::CONTOURS);
                //cv::imshow("Cube", cube.showFrame());
                //cv::imshow("Camera", frame);
                //cv::imshow("Video", frame);
            }
            key = cv::waitKey(15);
            if (key == 'p') playVideo = !playVideo;
            //sleep(10);
        }
        capture.release();

    } else if (is_camera) {

        const std::string cam_id_str = image_source_file.substr(strlen("/dev/video"));
        const int         cam_id     = std::stoi(cam_id_str);

        double exposure = params.getValue("CAMERA_EXPOSURE",-9001);
        int descriptor = v4l2_open(image_source_file.c_str(), O_RDWR);
        v4l2_control c;
        c.id = V4L2_CID_EXPOSURE_AUTO;
        c.value = int(params.getValue("CAMERA_EXPOSURE_AUTO", -9001));
        if(v4l2_ioctl(descriptor, VIDIOC_S_CTRL, &c) == 0)
            std::cout << "success";
        /*c.id = V4L2_CID_EXPOSURE_AUTO_PRIORITY;
          c.value = 0;
          if (v4l2_ioctl(descriptor, VIDIOC_S_CTRL, &c) == 0)
          std::cout << "success";*/
        
        c.id = V4L2_CID_EXPOSURE_ABSOLUTE;
        c.value = exposure;
        if (v4l2_ioctl(descriptor, VIDIOC_S_CTRL, &c) == 0)
            std::cout << "success";
        
        bool playVideo = true;
        char key;
        cv::VideoCapture capture(cam_id);

        cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
        cv::moveWindow("Original", 0, 0);
        
        cv::Mat frame;
        while (1) {
            
            if (!capture.isOpened()) {
                std::cout << "Could not read camera\n";
                return 1;
            }
            if (playVideo) {
                capture >> frame;
                
                cv::imshow("Original", frame);
                
                Cube cube(frame, params);
            }
            key = cv::waitKey(1);
            if (key == 'p') playVideo = !playVideo;
        }
        capture.release();
        
    } else {
        assert(0);
    }
      
    return 0;
}
