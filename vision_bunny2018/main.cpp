#include <iostream>
#include <string>
#include <vector>
#include "FileUtils.h"
#include "StringUtils.h"

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"

#include "params_parser.h"

//#include <cstdlib>
#include <libv4l2.h>
#include <linux/videodev2.h>
//#include <sys/ioctl.h>
#include <fcntl.h>


double getImageScaleFactor(const cv::Mat& frame,
                           const int      max_width,
                           const int      max_height)
{
    double width_scale = double(max_width) / frame.cols;
    double height_scale = double(max_width) / frame.rows;

    // Calculate overall scale factor, maintaining aspect ratio.
    // // If image already smaller, don't want to make it larger.
    double scale = std::min(width_scale, height_scale);
    scale = std::min(scale, 1.0);
    
    return scale;
}


void displayImage(const std::string& title,
                  const cv::Mat&     frame,
                  const int          x_coordinate,
                  const int          y_coordinate)
{
    cv::namedWindow(title.c_str(), cv::WINDOW_AUTOSIZE);
    cv::moveWindow(title.c_str(), x_coordinate, y_coordinate);
    cv::imshow(title.c_str(), frame);
}


std::vector<cv::KeyPoint> detectCrate(const paramsInput& params,
                                      const cv::Mat& frame_orig)
{
    if (!frame_orig.data) {
        std::cout << "Could not read image\n";
        exit(1);
    }

    // Resize original image
    double scale = getImageScaleFactor(frame_orig,
                                       params.getValue("MAX_WIDTH"),
                                       params.getValue("MAX_HEIGHT"));
    double width = scale * frame_orig.cols;
    double height = scale * frame_orig.rows;
    cv::Mat frame_orig_resized;
    cv::resize(frame_orig, frame_orig_resized, cv::Size(), scale, scale);
        
    // Show original image
    displayImage("Original", frame_orig_resized, 0, 0);

    // Apply: Blur
    enum BlurType {
        BOX, GAUSSIAN, MEDIAN, BILATERAL
    };
    cv::Mat frame_blur;
    BlurType blurType = BlurType::MEDIAN;
    double blurRadius = 15;
    int kernelSize = 2 * blurRadius + 1;
    cv::medianBlur(frame_orig_resized, frame_blur, kernelSize);
    displayImage("Blur", frame_blur, width, 0);

    // Apply: HSV threshold
    // Convert from BGR to HSV colorspace
    cv::Mat frame_HSV, frame_HSV_threshold;
    cvtColor(frame_blur, frame_HSV, cv::COLOR_BGR2HSV);
    // Filter based on HSV Range Values
    const int H_low  = params.getValue("HSV_H_LOW");
    const int H_high = params.getValue("HSV_H_HIGH");
    const int S_low  = params.getValue("HSV_S_LOW");
    const int S_high = params.getValue("HSV_S_HIGH");
    const int V_low  = params.getValue("HSV_V_LOW");
    const int V_high = params.getValue("HSV_V_HIGH");
    inRange(frame_HSV, cv::Scalar(H_low, S_low, V_low), cv::Scalar(H_high, S_high, V_high), frame_HSV_threshold);
    displayImage("HSV Threshold", frame_HSV_threshold, width*2, 0);

    // Apply: Erode
    cv::Mat frame_erode;
    cv::Mat cvErodeKernel;
    cv::Point cvErodeAnchor(-1, -1);
    int cvErodeIterations = 1;
    int cvErodeBorderType = cv::BORDER_DEFAULT;
    cv::Scalar cvErodeBorderValue(-1);
    cv::erode(frame_HSV_threshold, frame_erode, cvErodeKernel, cvErodeAnchor, cvErodeIterations, cvErodeBorderType, cvErodeBorderValue);
    displayImage("Erode", frame_erode, width*3, 0);
        
    // Apply: Dilate
    cv::Mat frame_dilate;
    cv::Mat cvDilateKernel;
    cv::Point cvDilateAnchor(-1, -1);
    int cvDilateIterations = 27;  // default Double
    int cvDilateBorderType = cv::BORDER_CONSTANT;
    cv::Scalar cvDilateBorderValue(-1);
    cv::dilate(frame_erode, frame_dilate, cvDilateKernel, cvDilateAnchor, cvDilateIterations, cvDilateBorderType, cvDilateBorderValue);
    displayImage("Dilate", frame_dilate, width*4, 0);

    // Blob detection
    bool findBlobsDarkBlobs = false;
    cv::SimpleBlobDetector::Params blob_params;
    blob_params.filterByColor = params.getValue("BLOB_FILTER_BY_COLOR");;
    blob_params.blobColor = (findBlobsDarkBlobs ? 0 : 255);
    blob_params.minThreshold = params.getValue("BLOB_MIN_THRESHOLD");
    blob_params.maxThreshold = params.getValue("BLOB_MAX_THRESHOLD");
    blob_params.filterByArea = params.getValue("BLOB_FILTER_BY_AREA");
    blob_params.minArea = params.getValue("BLOB_MIN_AREA");
    blob_params.maxArea = 100000;  // Set to very large no. For some reason, it defaults to arbitrary limit that's too small.
    blob_params.filterByCircularity = params.getValue("BLOB_FILTER_BY_CIRCULARITY");
    blob_params.minCircularity = params.getValue("BLOB_MIN_CIRCULARITY");
    blob_params.maxCircularity = params.getValue("BLOB_MAX_CIRCULARITY");
    blob_params.filterByConvexity = params.getValue("BLOB_FILTER_BY_CONVEXITY");
    blob_params.filterByInertia = params.getValue("BLOB_FILTER_BY_INERTIA");;
    std::vector<cv::KeyPoint> keypoints;
    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(blob_params);
    detector->detect(frame_dilate, keypoints);

    // Draw keypoints
    if (!keypoints.empty()) {
        cv::Mat frame_with_keypoints;
        cv::drawKeypoints(frame_dilate, keypoints, frame_with_keypoints, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
        displayImage("Dilate", frame_with_keypoints, width*4, 0);
    }
        
    return keypoints;
}


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
        cv::Mat frame_orig = cv::imread(image_source_file, cv::IMREAD_COLOR);
        std::vector<cv::KeyPoint> keypoints = detectCrate(params, frame_orig);        
        std::cout << "No. of detected blobs: " << keypoints.size() << std::endl;

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
                
                //Cube cube(frame, params);
                
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
                
                //Cube cube(frame, params);
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
