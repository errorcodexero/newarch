/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <cstdio>
#include <string>
#include <thread>
#include <vector>

#include <iostream>
#include <stdlib.h>    // For system()
#include <math.h>
#include <assert.h>

#include <networktables/NetworkTableInstance.h>
#include <vision/VisionPipeline.h>
#include <vision/VisionRunner.h>
#include <wpi/StringRef.h>
#include <wpi/json.h>
#include <wpi/raw_istream.h>
#include <wpi/raw_ostream.h>

#include <cameraserver/CameraServer.h>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

#include <FileUtils.h>

//#include "SettingsParser.h"
#include "params_parser.h"



/*
   JSON format:
   {
       "team": <team number>,
       "ntmode": <"client" or "server", "client" if unspecified>
       "cameras": [
           {
               "name": <camera name>
               "path": <path, e.g. "/dev/video0">
               "pixel format": <"MJPEG", "YUYV", etc>   // optional
               "width": <video mode width>              // optional
               "height": <video mode height>            // optional
               "fps": <video mode fps>                  // optional
               "brightness": <percentage brightness>    // optional
               "white balance": <"auto", "hold", value> // optional
               "exposure": <"auto", "hold", value>      // optional
               "properties": [                          // optional
                   {
                       "name": <property name>
                       "value": <property value>
                   }
               ],
               "stream": {                              // optional
                   "properties": [
                       {
                           "name": <stream property name>
                           "value": <stream property value>
                       }
                   ]
               }
           }
       ]
   }
*/


namespace {
    
    const char* configFile = "/boot/frc.json";
    const cv::Scalar color_blue(2550,0);
    const cv::Scalar color_green(0,255,0);
    const cv::Scalar color_red(0,0,255);
    
    paramsInput params;


    unsigned int team;
    bool nt_server = false;

    // Whether to stream the output image from the pipeline.
    // Should just be needed for debug.
    // To enable, set param in param fie.
    static bool stream_pipeline_output = false;

    // Network table entries where results from tracking will be posted.
    nt::NetworkTableEntry nt_target_dist_pixels, nt_target_dist_inches, nt_target_yaw_deg, nt_target_valid;

    
    struct CameraConfig {
        std::string name;
        std::string path;
        wpi::json config;
        wpi::json streamConfig;
    };

    std::vector<CameraConfig> cameraConfigs;

    
    wpi::raw_ostream& ParseError() {
        return wpi::errs() << "config error in '" << configFile << "': ";
    }

    
    bool ReadCameraConfig(const wpi::json& config) {
        CameraConfig c;

        // Read name from json config
        try {
            c.name = config.at("name").get<std::string>();
        } catch (const wpi::json::exception& e) {
            ParseError() << "Could not read camera name: " << e.what() << '\n';
            return false;
        }

        // Read path from json config
        try {
            c.path = config.at("path").get<std::string>();
        } catch (const wpi::json::exception& e) {
            ParseError() << "Camera '" << c.name
                         << "': Could not read path: " << e.what() << '\n';
            return false;
        }

        // Stream properties
        if (config.count("stream") != 0) c.streamConfig = config.at("stream");

        c.config = config;

        cameraConfigs.emplace_back(std::move(c));
        return true;
    }

    
    bool ReadConfig() {
        // Open config file
        std::error_code ec;
        wpi::raw_fd_istream is(configFile, ec);
        if (ec) {
            wpi::errs() << "Could not open '" << configFile << "': " << ec.message()
                        << '\n';
            return false;
        }

        // Parse file
        wpi::json j;
        try {
            j = wpi::json::parse(is);
        } catch (const wpi::json::parse_error& e) {
            ParseError() << "byte " << e.byte << ": " << e.what() << '\n';
            return false;
        }

        // Top level must be an object
        if (!j.is_object()) {
            ParseError() << "Must be JSON object\n";
            return false;
        }

        // Team number
        try {
            team = j.at("team").get<unsigned int>();
        } catch (const wpi::json::exception& e) {
            ParseError() << "Could not read team number: " << e.what() << '\n';
            return false;
        }

        // ntmode (optional)
        if (j.count("ntmode") != 0) {
            try {
                auto str = j.at("ntmode").get<std::string>();
                wpi::StringRef s(str);
                if (s.equals_lower("client")) {
                    nt_server = false;
                } else if (s.equals_lower("server")) {
                    nt_server = true;
                } else {
                    ParseError() << "could not understand ntmode value '" << str << "'\n";
                }
            } catch (const wpi::json::exception& e) {
                ParseError() << "Could not read ntmode: " << e.what() << '\n';
            }
        }

        // Cameras
        try {
            for (auto&& camera : j.at("cameras")) {
                if (!ReadCameraConfig(camera)) return false;
            }
        } catch (const wpi::json::exception& e) {
            ParseError() << "Could not read cameras: " << e.what() << '\n';
            return false;
        }

        return true;
    }

    
    cs::UsbCamera StartCamera(const CameraConfig& config) {
        wpi::outs() << "Starting camera '" << config.name << "' on " << config.path
                    << '\n';
        
        cs::UsbCamera camera{config.name, config.path};
        auto inst = frc::CameraServer::GetInstance();
        auto server = inst->StartAutomaticCapture(camera);

        camera.SetConfigJson(config.config);
        camera.SetConnectionStrategy(cs::VideoSource::kConnectionKeepOpen);

        if (config.streamConfig.is_object()) {
            server.SetConfigJson(config.streamConfig);
        }

        return camera;
    }

    // Return area of cv::RotatedRect
    double getRectArea(const cv::RotatedRect& rect) {
        cv::Size2f rect_size = rect.size;
        return (rect_size.width * rect_size.height);
    }

    // Return rectangle aspect ratio.  Always >= 1.
    double getRectAspectRatio(const cv::RotatedRect& rect) {
        cv::Size2f rect_size = rect.size;
        double aspect_ratio = rect_size.width / rect_size.height;
        if (aspect_ratio < 1.0 && aspect_ratio != 0) {
            aspect_ratio = 1.0/aspect_ratio;
        }
        return aspect_ratio;
    }

    // Check if a number is approximately equal to another, +- some tolerance (percentage/100) of the second number
    bool isApproxEqual(double num1, double num2, double tolerance /*0->1*/) {
        const double min = num2 * (1.0 - tolerance);
        const double max = num2 * (1.0 + tolerance);
        return (num1 >= min) && (num1 <= max);
    }

    // Ratio of difference between 2 numbers to the average of the two.
    // If one if 0, compare to the other.
    double ratioOfDifference(double num1, double num2) {
        double ref = (num1 + num2) / 2.0;
        if (ref == 0) {
            ref = (num1 == 0)? num2 : num1;
            if (ref == 0) {
                assert(num1 == 0);
                assert(num2 == 0);
                return 0;
            }
        }
        const double ratio = (num1 - num2) / ref;
        return fabs(ratio);
    }

    // One component of a vision pipeline.
    // Takes in an image (cv::Mat) and produces a modified one.
    // Copy the image frame rather than modify it in place in case we later need to show the different pipeline element outputs.
    // Later, optimize to avoid copies in non-debug mode if there's a visible performance impact.
    class XeroPipelineElement {
    public:
        XeroPipelineElement(std::string name) : name_(name) {}
        virtual void Process(cv::Mat& frame_in) =0;
        virtual cv::Mat& getFrameOut() {
            return frame_out_;
        }
        
    protected:
        std::string name_;
        cv::Mat frame_out_;
    };

    // Pipeline element for: HSV Threshold
    class XeroPipelineElementHsvThreshold : public XeroPipelineElement {
        
    public:
        
        XeroPipelineElementHsvThreshold(std::string name) : XeroPipelineElement(name) {
            int h_min = params.getValue("vision:pipeline:hsv_threshold:h_min");
            int h_max = params.getValue("vision:pipeline:hsv_threshold:h_max");
            int s_min = params.getValue("vision:pipeline:hsv_threshold:s_min");
            int s_max = params.getValue("vision:pipeline:hsv_threshold:s_max");
            int v_min = params.getValue("vision:pipeline:hsv_threshold:v_min");
            int v_max = params.getValue("vision:pipeline:hsv_threshold:v_max");
            
            // Set threshold to only select green
            hsv_ranges = {h_min, h_max, s_min, s_max, v_min, v_max};
        }
        
        virtual void Process(cv::Mat& frame_in) {
            //frame_out_ = frame_in;

            cv::cvtColor(frame_in, hsv_image, cv::COLOR_BGR2HSV);

            cv::inRange(hsv_image,
                        cv::Scalar(hsv_ranges[0], hsv_ranges[2], hsv_ranges[4]),
                        cv::Scalar(hsv_ranges[1], hsv_ranges[3], hsv_ranges[5]),
                        green_only_image_);

#if 0   // Make frame_out a viewable image            
            cv::cvtColor(green_only_image_, frame_out_, cv::COLOR_GRAY2BGR);
            cv::circle(frame_out_, cv::Point(100,100), 50,  color_red);
#else  // Assign binary result to output image for use by contour detection next
            frame_out_ = green_only_image_;
#endif
        }

    private:

        std::vector<int> hsv_ranges;
        cv::Mat hsv_image, green_only_image_;

    };

    
    // Pipeline element for: Contour detection
    class XeroPipelineElementFindContours : public XeroPipelineElement {
        
    public:
        
        XeroPipelineElementFindContours(std::string name) : XeroPipelineElement(name) {
        }
        
        virtual void Process(cv::Mat& frame_in) {
            // Perform contour detection
            contours.clear();
            const bool externalOnlyContours = true;
            const int mode = externalOnlyContours ? cv::RETR_EXTERNAL : cv::RETR_LIST;
            const int method = cv::CHAIN_APPROX_SIMPLE;
            cv::findContours(frame_in, contours, hierarchy, mode, method);

            // Confirm input binary image to a viewable object.
            // Further detection will be added to this image.
            cv::cvtColor(frame_in, frame_out_, cv::COLOR_GRAY2BGR);
            
            // Unless we have at least 2 contours, nothing further to do
            if (contours.size() < 2) {
                nt_target_valid.SetBoolean(false);                
                return;
            }

            // Draw contours + find rectangles meeting aspect ratio requirement
            std::vector<cv::RotatedRect> min_rects(contours.size()), filtered_min_rects;
            const double expected_aspect_of_target = 5.5/2;
            const double aspect_ratio_tolerance = 0.15;
            for (int ix=0; ix < contours.size(); ++ix) {
                std::vector<cv::Point>& contour = contours[ix];

                // Draw contour in blue
                if (1 /*cv::iscorrect(contour)*/) {  // TODO: Replace 1 by cv::iscorrect(), ensure it works
                    cv::drawContours(frame_out_,
                                     std::vector<std::vector<cv::Point> >(1,contour),  // TODO: Pass contours + actual index
                                     -1,          // Negative index ==> draw all contours
                                     color_blue,  // Color
                                     1,           // Thickness
                                     8);
                }

                // Find minimum rotated rectangle encapsulating the contour
                cv::RotatedRect min_rect = min_rects[ix];
                min_rect = cv::minAreaRect(contour);
                double aspect_ratio = getRectAspectRatio(min_rect);

                // Keep rectangles that have the expected aspect ratio
                if (isApproxEqual(aspect_ratio, expected_aspect_of_target, aspect_ratio_tolerance)) {
                    filtered_min_rects.push_back(min_rect);
                    cv::Point2f rect_points[4];
                    min_rect.points(rect_points);
                    for (int j = 0; j < 4; j++ ) {
                        cv::line(frame_out_, rect_points[j], rect_points[(j+1)%4], color_red, 2);
                    }
                }
            }

            // Only continue if we have at least 2 filtered rectangles
            if (filtered_min_rects.size() < 2) {
                nt_target_valid.SetBoolean(false);                
                return;
            }
            
            // Sort rectangles on descending area
            std::sort(filtered_min_rects.begin(),
                      filtered_min_rects.end(),
                      [](const cv::RotatedRect& a, const cv::RotatedRect& b) -> bool
                      { 
                          return (getRectArea(a) > getRectArea(b));
                      });

            // Only accept rectangles if top 2 rectangles close in area
            const double area1 = getRectArea(filtered_min_rects[0]);
            const double area2 = getRectArea(filtered_min_rects[1]);
            double relative_area = ratioOfDifference(area1, area2);
            //std::cout << "    Relative area = " << relative_area << "\n";
            if (relative_area > 0.45) {
                nt_target_valid.SetBoolean(false);                
                return;
            }

            cv::RotatedRect left_rect(filtered_min_rects[0]);
            cv::RotatedRect right_rect(filtered_min_rects[1]);
            if (left_rect.center.x > right_rect.center.x) {
                std::swap(left_rect, right_rect);
            }

            // Top 2 rectangles must have centers almost at same height.
            // Note origin (0,0) is top left corner so Y axis is reversed.
            cv::Point2f left_center = left_rect.center;
            cv::Point2f right_center = right_rect.center;
            const double delta_x = right_center.x - left_center.x;
            const double delta_y = -(right_center.y - left_center.y);
            assert(delta_x >= 0);
            double tangent = delta_y / delta_x;
            double angle_in_rad = atan(tangent);
            double angle_in_deg = angle_in_rad * 57.2958;
            if (fabs(angle_in_deg) > 15) {
                nt_target_valid.SetBoolean(false);                
                return;
            }

            cv::Point2f center_point = (left_center + right_center) * 0.5;
            double dist_between_centers = hypot(delta_x, delta_y);

            // At this point, top 2 rectangles have right aspect ratio, almost equal size, and almost same height
            // So likely a valid target.
            // Draw them in green.
            for (int ix=0; ix<2; ++ix) {
                cv::RotatedRect rect = filtered_min_rects[ix];
                cv::Point2f rect_points[4];
                rect.points(rect_points);
                for (int j = 0; j < 4; j++ ) {
                    cv::line(frame_out_, rect_points[j], rect_points[(j+1)%4], color_green, 2);
                }
            }

            // Draw line between centres
            cv::line(frame_out_, left_center, right_center, color_green, 2);

            // Draw marker through center
            cv::drawMarker(frame_out_, center_point, color_green, cv::MARKER_CROSS, 20 /*marker size*/, 2 /*thickness*/);

            // TODO: Filter on angle of rectangles?
            //       Filter on area vs. distance between centres?
            //       Measure angle and distance.

            // Publish results on network table
            nt_target_dist_pixels.SetDouble(dist_between_centers);
            //nt_target_dist_inches.SetDouble(TO BE DONE);
            //nt_target_yaw_deg.SetDouble(TO BE DONE);
            nt_target_valid.SetBoolean(true);                


#if 0
            std::cout << "Contours: Total=" << contours.size() << ", filtered=" << filtered_min_rects.size() << "\n";
            for (cv::RotatedRect& rect : filtered_min_rects) {
                cv::Size2f rect_size = rect.size;
                std::cout << "    " << (rect_size.width * rect_size.height) << "\n";
            }
#endif
            
            //frame_out_ = frame_in;
        }

    private:

        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;

    };

    
    // Example pipeline
    class XeroPipeline : public frc::VisionPipeline {
    public:
        int val = 0;

        XeroPipeline() {
            pipe_elements_.push_back(new XeroPipelineElementHsvThreshold("HSV Threshold"));
            pipe_elements_.push_back(new XeroPipelineElementFindContours("Find Contours"));
        }

        virtual ~XeroPipeline();

        virtual void Process(cv::Mat& mat) override {
            //std::cout << "Process  " << mat.cols << "   " << mat.rows << "\n";
            cv::Mat* frame_out_p = &mat;
            for (auto& elem : pipe_elements_) {
                elem->Process(*frame_out_p);
                frame_out_p = &(elem->getFrameOut());
            }
            output_frame_ = *frame_out_p;
            ++val;
        }

        cv::Mat output_frame_;

    private:
        std::vector<XeroPipelineElement*> pipe_elements_;
    };

    class VisionPipelineResultProcessor {
    public:

        VisionPipelineResultProcessor(bool stream_output) : stream_output_(stream_output) {
            if (stream_output_) {
                output_stream_ = frc::CameraServer::GetInstance()->PutVideo("Pipeline Output", 640, 480);
            }
        }
        
        void operator()(XeroPipeline& pipe) {
            if (stream_output_) {
                output_stream_.PutFrame(pipe.output_frame_);
            }
        }
        
    private:

        cs::CvSource output_stream_;
        bool stream_output_;
    };


    XeroPipeline::~XeroPipeline() {
        for (auto& elem : pipe_elements_) {
            delete elem;
        }
    }


    void runPipelineFromCamera(/*std::vector<CameraConfig>& cameraConfigs*/) {
        // Start camera streaming
        std::vector<cs::VideoSource> cameras;
        for (auto&& cameraConfig : cameraConfigs) {
            cameras.emplace_back(StartCamera(cameraConfig));
        }

        // Start image processing on last camera if present
        auto pipe = std::make_shared<XeroPipeline>();
        if (cameras.size() >= 1) {
            std::cout << "Starting vision pipeline\n";

            std::thread t([&] {
                              frc::VisionRunner<XeroPipeline> runner(cameras[/*0*/ cameras.size()-1],
                                                                     pipe.get(),
                                                                     VisionPipelineResultProcessor(stream_pipeline_output));
                              runner.RunForever();
                          });
            t.detach();
        }

        // Manually set camera controls directly using v2l4-ctl.
        // Apparently only works after starting the pipeline + small delay.
        // TODO: Don't hardcode device id.  Get it from json.
        std::this_thread::sleep_for(std::chrono::seconds(2));
        int sysret = system("v4l2-ctl -d /dev/video0 -c exposure_auto=1 -c exposure_absolute=100 -c brightness=1 -c gain=30");
        if (sysret != 0) {
            std::cout << "ERROR: Failed to call v4l2-ctl\n";
        }
        
        // Loop forever
        for (;;) std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    void runPipelineFromVideo(const std::string& video_filename) {
        if (!xero::file::exists(video_filename)) {
            std::cout << "Video file '" << video_filename << "' does not exist.\n";
            exit(-1);
        }

        VisionPipelineResultProcessor pipe_result_processor(stream_pipeline_output);
        XeroPipeline pipe;

        bool playVideo = true;
        char key;
        std::cout << "DEBUG: Just before cv::VideoCapture\n";
        cv::VideoCapture capture(video_filename);
        std::cout << "DEBUG: Just after cv::VideoCapture\n";

        cv::Mat frame;
        while (capture.get(cv::CAP_PROP_POS_FRAMES) < capture.get(cv::CAP_PROP_FRAME_COUNT)) {

            if (!capture.isOpened()) {
                std::cout << "Could not read video\n";
                exit(-1);
            }
            if (playVideo) {
                capture >> frame;
                pipe.Process(frame);
                pipe_result_processor(pipe);
            }
#if 0
            key = cv::waitKey(10);
            if (key == 'p') {
                playVideo = !playVideo;
            } else if (key == 'q') {
                break;
            }
#endif
            //sleep(10);
        }
        capture.release();
        
    }


}  // namespace



int main(int argc, char* argv[]) {
    const std::string params_filename("vision_params.txt");

    if (argc >= 2) {
        configFile = argv[1];
    }

    // Read params file
    if (!params.readFile(params_filename)) {
        return EXIT_FAILURE;
    }

    // Read configuration
    if (!ReadConfig()) {
        return EXIT_FAILURE;
    }

    // Import global settings from param file
    const std::string stream_output_param_name("vision:pipeline:stream_output");
    if (params.hasParam(stream_output_param_name)) {
        stream_pipeline_output = (params.getValue(stream_output_param_name) != 0);
    }
    

    // Start NetworkTables
    auto ntinst = nt::NetworkTableInstance::GetDefault();
    if (nt_server) {
        wpi::outs() << "Setting up NetworkTables server\n";
        ntinst.StartServer();
    } else {
        wpi::outs() << "Setting up NetworkTables client for team " << team << '\n';
        ntinst.StartClientTeam(team);
    }

    // Prepare network table variables that tracker will populate
    std::shared_ptr<NetworkTable> nt_table = ntinst.GetTable("TargetTracking");
    nt_target_dist_pixels = nt_table->GetEntry("dist_pixels");
    nt_target_dist_pixels.SetDefaultDouble(0);
    nt_target_dist_inches = nt_table->GetEntry("dist_inch");
    nt_target_dist_inches.SetDefaultDouble(0);
    nt_target_yaw_deg = nt_table->GetEntry("yaw_deg");
    nt_target_yaw_deg.SetDefaultDouble(0);
    nt_target_valid = nt_table->GetEntry("valid");
    nt_target_valid.SetDefaultBoolean(false);

    // Start camera streaming + image processing on last camera if present
    runPipelineFromCamera(/*cameraConfigs*/);

    // Start image processing from video file
    //runPipelineFromVideo("cube1.mp4");
    //runPipelineFromVideo("capout.avi");
}
