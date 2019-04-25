#include "Cube.h"
#include <iostream>

// Added from branch for streaming from camera instead of file.  Most likely not needed, to be cleaned up.
//#include <cstdlib>
#include <libv4l2.h>
#include <linux/videodev2.h>
//#include <sys/ioctl.h>
#include <fcntl.h>
//#include <sstream>


int main(int argc, char **argv) {
    std::string params_file(argv[1]);
    paramsInput params;
    params.readFile(params_file);

#if 0  // Stream from file    
    //auto debug = params.getValue("CAMERA_EXPOSURE", -9001);
    std::string video_file(params.getString("FILENAME", ""));
    bool playVideo = true;
    char key;
    while (1) {
      cv::VideoCapture capture(video_file);
      while(capture.get(CV_CAP_PROP_POS_FRAMES)<capture.get(CV_CAP_PROP_FRAME_COUNT)-1) {
        //we can loop the video by re-opening the capture every time the video reaches its last frame

        if (!capture.isOpened()) {
          std::cout << "ERROR ACQUIRING VIDEO FEED\n";
          getchar();
          return -1;
        }
        if (playVideo) {
          //check if the video has reach its last frame.
          //we add '-1' because we are reading two frames from the video at a time.
          //if this is not included, we get a memory error!
          cv::Mat frame(cv::Scalar(capture.get(cv::CAP_PROP_FRAME_WIDTH), capture.get(cv::CAP_PROP_FRAME_HEIGHT)));
          //read first frame
          capture >> frame;
          //std::cout << frame.cols;
          //std::cout << frame.rows << std::endl;
          /*if (frame.empty()) {
            capture.release();
            capture = cv::VideoCapture(video_file);    
          }*/
            
          Cube cube(frame, params);
          //std::cout << cube.getPosition() << std::endl;
          cube.getPosition(Cube::detectionMode::CONTOURS);
          //cv::imshow("Cube", cube.showFrame());
          //cv::imshow("Camera", frame);
          //cv::imshow("Video", frame);
        }
        key = cv::waitKey(15);
        if (key == 'p') playVideo = !playVideo;
        
      }
      capture.release();
    }

#else // Stream from camera
      double exposure = params.getValue("CAMERA_EXPOSURE",-9001);
      std::stringstream ss;
      ss << "/dev/video" << std::to_string(int(params.getValue("CAMID", -9001)));
      int descriptor = v4l2_open(ss.str().c_str(), O_RDWR);
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
      cv::VideoCapture capture(params.getValue("CAMID", 0));
      while (1) {
	if (!capture.isOpened()) {
	  std::cout << "ERROR ACQUIRING VIDEO FEED\n";
	  getchar();
	  return -1;
	}
	cv::Mat frame;
	//read first frame
	capture >> frame;
	Cube cube(frame, params);
	cube.getPosition(Cube::detectionMode::BLOB);
	cv::imshow("Cube", cube.showFrame());
	//cv::imshow("Camera", frame);
	//cv::imshow("Video", frame);
	cv::waitKey(10);
      };
      capture.release();
      
#endif
  
    return 0;
}
