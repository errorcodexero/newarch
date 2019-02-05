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
#include <algorithm>

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
#include <frc/Timer.h>

#include <cameraserver/CameraServer.h>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

// From xerolib
#include <FileUtils.h>
#include <StringUtils.h>
#include <GetHostIpAddresses.h>

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

    nt::NetworkTableInstance ntinst;
    const char* configFile = "/boot/frc.json";
    const cv::Scalar color_blue(255,0,0);
    const cv::Scalar color_green(0,255,0);
    const cv::Scalar color_red(0,0,255);
    const cv::Scalar color_cyan(255,255,0);
    const cv::Scalar color_orange(0,165,255);
    const cv::Scalar color_yellow(0,255,255);
    const cv::Scalar color_white(255,255,255);

    // Vision targets should be 8in apart at closest point.
    // 5.5in x 2in strips.
    // Angled about 14.5 degrees.
    double dist_bet_centers_inch = 11.5;  // APPROXIMATE.  TODO: Calculate accurately.

    paramsInput params;


    // Team number read from frc.json file
    unsigned int team;
    //bool nt_server = false;

    // Read from param file
    int width_pixels;
    int height_pixels;
    
    // Whether to stream camera's direct output
    static bool stream_camera = true;

    // Whether to stream the output image from the pipeline.
    // Should just be needed for debug.
    static bool stream_pipeline_output = false;

    // Network table entries where results from tracking will be posted.
    nt::NetworkTableEntry nt_pipe_fps;
    nt::NetworkTableEntry nt_pipe_runtime_ms;
    nt::NetworkTableEntry nt_target_dist_pixels;
    nt::NetworkTableEntry nt_target_dist_inch;
    nt::NetworkTableEntry nt_target_yaw_deg;
    nt::NetworkTableEntry nt_target_rect_ratio;
    nt::NetworkTableEntry nt_rect_l_angle_deg;
    nt::NetworkTableEntry nt_rect_r_angle_deg;
    nt::NetworkTableEntry nt_rect_l_height;
    nt::NetworkTableEntry nt_rect_r_height;
    nt::NetworkTableEntry nt_rect_l_width;
    nt::NetworkTableEntry nt_rect_r_width;
    nt::NetworkTableEntry nt_rect_l_dist_inch;
    nt::NetworkTableEntry nt_rect_r_dist_inch;
    nt::NetworkTableEntry nt_bot_x_offset_inch;
    nt::NetworkTableEntry nt_bot_z_offset_inch;
    nt::NetworkTableEntry nt_target_valid;

    
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

#if 0  // Network table server vs. client mode depends on detecting if running on robot.
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
#endif

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
        
        auto inst = frc::CameraServer::GetInstance();
        cs::UsbCamera camera{config.name, config.path};
        cs::MjpegServer server;
        if (stream_camera) {
            server = inst->StartAutomaticCapture(camera);
        }

        camera.SetConfigJson(config.config);
        camera.SetConnectionStrategy(cs::VideoSource::kConnectionKeepOpen);

        if (config.streamConfig.is_object()) {
            if (stream_camera) {
                server.SetConfigJson(config.streamConfig);
            }
        }
            
        // Force resolution from param file
        camera.SetResolution(width_pixels, height_pixels);

        return camera;
    }

    int setTrackingExposure(const std::string& device_name) {
        std::string command = std::string("v4l2-ctl -d ") + device_name + " -c exposure_auto=1 -c exposure_absolute=100 -c brightness=1 -c gain=30";
        int sysret = system(command.c_str());
        if (sysret != 0) {
            std::cout << "ERROR: Failed to call v4l2-ctl\n";
        }
        return sysret;
    }

    int setViewingExposure(const std::string& device_name) {
        std::string command = std::string("v4l2-ctl -d ") + device_name + " -c exposure_auto=3 -c brightness=128";
        int sysret = system(command.c_str());
        if (sysret != 0) {
            std::cout << "ERROR: Failed to call v4l2-ctl\n";
        }
        return sysret;
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
        double min = num2 * (1.0 - tolerance);
        double max = num2 * (1.0 + tolerance);
        if (min > max) {  // Needed to handle negative numbers
            std::swap(min, max);
        }
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

    // Start network table in client vs. server mode depending whether running on robot or not
    // Also speed up update rate.
    void startNetworkTable(nt::NetworkTableInstance& ntinst) {
        // Figure out if running on robot so network table can be started in server vs. client mode
        bool running_on_robot = false;
        std::vector<std::string> ip_addresses = xero::misc::get_host_ip_addresses();
        for (auto addr : ip_addresses) {
            if (xero::string::startsWith(addr, "10.14.25")) {
                // TODO: Don't hardcode IP address.  Derive from team number.
                running_on_robot = true;
                break;
            }
        }
    
        // Start NetworkTables
        const bool nt_server = !running_on_robot;
        ntinst = nt::NetworkTableInstance::GetDefault();
        if (nt_server) {
            wpi::outs() << "Setting up NetworkTables server\n";
            ntinst.StartServer();
        } else {
            wpi::outs() << "Setting up NetworkTables client for team " << team << '\n';
            ntinst.StartClientTeam(team);
        }

        // Change default update rate from 100ms to 10ms.
        // Will flush update anyway to try and reduce latency after posting set of results,
        // but it's rate-limited to prevent flooding newwork. Unclear whether this affects the cap.
        // Changing it anyway in case it does + in case flush() is not called.
        ntinst.SetUpdateRate(0.01);    // Allowed range per docs is 0.01 -> 1.0 (rate in seconds)
    }

    // Draw ractangle
    void drawRectangle(cv::Mat& frame,
                       const cv::RotatedRect& rect,
                       const cv::Scalar color,
                       int width) {
        cv::Point2f rect_points[4];
        rect.points(rect_points);
        for (int j = 0; j < 4; j++ ) {
            cv::line(frame, rect_points[j], rect_points[(j+1)%4], color, width);
        }
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
            if (stream_pipeline_output) {
                cv::cvtColor(frame_in, frame_out_, cv::COLOR_GRAY2BGR);
            }
            
            // Unless we have at least 2 contours, nothing further to do
            if (contours.size() < 2) {
                nt_target_valid.SetBoolean(false);
                ntinst.Flush();
                //std::cout << "FALSE: Fewer than 2 contours\n";
                return;
            }

            // Draw contours + find rectangles meeting aspect ratio requirement
            std::vector<cv::RotatedRect> min_rects(contours.size()), filtered_min_rects;
            const double expected_aspect_of_target = 5.5/2;
            const double aspect_ratio_tolerance = 0.3;   //0.15;

            // Draw all contours in blue
            /*
            if (stream_pipeline_output) {
                cv::drawContours(frame_out_,
                                 contours,
                                 -1,          // Negative index ==> draw all contours
                                 color_blue,  // Color
                                 1,           // Thickness
                                 8);
            }
            */
            
            for (int ix=0; ix < contours.size(); ++ix) {
                std::vector<cv::Point>& contour = contours[ix];

                // Find minimum rotated rectangle encapsulating the contour
                cv::RotatedRect min_rect = min_rects[ix];
                min_rect = cv::minAreaRect(contour);
                double aspect_ratio = getRectAspectRatio(min_rect);

                // Discard rectangles off vertical center.  Discard top and bottom of fov.
                const double half_height = height_pixels/2;
                const double pixels_from_vertical_middle = abs(min_rect.center.y - half_height);
                if (pixels_from_vertical_middle/half_height > 0.7) {
                    continue;
                }
                
                // Draw rectangle (cyan) after excluding those in top or bottom of frame
                if (stream_pipeline_output) {
                    drawRectangle(frame_out_, min_rect, color_cyan, 4);
                }

                // Discard rectangles that don't have the expected aspect ratio
                if (!isApproxEqual(aspect_ratio, expected_aspect_of_target, aspect_ratio_tolerance)) {
                    continue;
                }

                // Draw rectangle (orange) after filtering based on aspect ratio
                if (stream_pipeline_output) {
                    drawRectangle(frame_out_, min_rect, color_orange, 4);
                }
                
                // Discard rectangles that don't have the expected angle
                // Expected angles are -15 for one and -75 for the other.
                const double rotate_angle_tol = 0.25;
                if (!isApproxEqual(min_rect.angle+90, 15, rotate_angle_tol) &&
                    !isApproxEqual(min_rect.angle+90, 75, rotate_angle_tol)) {
                    //std::cout << "FALSE: Rect angle = " << min_rect.angle << "\n";
                    continue;
                }
                
                // Draw rectangle (red) after filtering based on angle of rotation
                if (stream_pipeline_output) {
                    drawRectangle(frame_out_, min_rect, color_red, 4);
                }

                // Add filtered rectangle to list
                filtered_min_rects.push_back(min_rect);
            }

            // Only continue if we have at least 2 filtered rectangles
            if (filtered_min_rects.size() < 2) {
                nt_target_valid.SetBoolean(false);                
                ntinst.Flush();
                //std::cout << "FALSE: Fewer than 2 filtered rect\n";
                return;
            }
            
            // Sort rectangles on descending area
            std::sort(filtered_min_rects.begin(),
                      filtered_min_rects.end(),
                      [](const cv::RotatedRect& a, const cv::RotatedRect& b) -> bool
                      { 
                          return (getRectArea(a) > getRectArea(b));
                      });

            // Focus on 2 largest rectangles
            cv::RotatedRect left_rect(filtered_min_rects[0]);
            cv::RotatedRect right_rect(filtered_min_rects[1]);
            if (left_rect.center.x > right_rect.center.x) {
                std::swap(left_rect, right_rect);
            }

            // Only accept rectangles if top 2 rectangles close in area
            const double l_area = getRectArea(left_rect);
            const double r_area = getRectArea(right_rect);
            double relative_area = ratioOfDifference(l_area, r_area);
            //std::cout << "    Relative area = " << relative_area << "\n";
            if (relative_area > 0.5) {
                nt_target_valid.SetBoolean(false);                
                ntinst.Flush();
                //std::cout << "FALSE: Relative area " << relative_area << " (" << l_area << ", " << r_area << ")\n";
                return;
            }

            // Draw 2 largest rectangles (yellow)
            if (stream_pipeline_output) {
                drawRectangle(frame_out_, left_rect, color_yellow, 4);
                drawRectangle(frame_out_, right_rect, color_yellow, 4);
            }

            // Top 2 rectangles must have centers almost at same height.
            // Note pixel origin (0,0) is top left corner so Y axis is reversed.
            cv::Point2f left_center = left_rect.center;
            cv::Point2f right_center = right_rect.center;
            const double delta_x = right_center.x - left_center.x;
            const double delta_y = -(right_center.y - left_center.y);
            assert(delta_x >= 0);
            double tangent = delta_y / delta_x;
            double angle_in_rad = atan(tangent);
            double angle_in_deg = angle_in_rad * 180.0 / M_PI;
            angle_in_deg = fabs(angle_in_deg);
            //std::cout << "    Angle (centers) in deg = " << angle_in_deg << "\n";
            if (angle_in_deg > 15) {
                nt_target_valid.SetBoolean(false);                
                ntinst.Flush();
                //std::cout << "FALSE: Angle in deg << " << angle_in_deg << "\n";
                return;
            }

            cv::Point2f center_point = (left_center + right_center) * 0.5;
            double dist_bet_centers = hypot(delta_x, delta_y);

            // Distance to target in inches
            // At 640x460 of C270 and 640x480 resolution, pixels_bet_centres * dist_to_target_in_FEET ~ 760
            double dist_to_target = (760.0/dist_bet_centers) * 12.0/*inches_per_foot*/ * static_cast<double>(width_pixels)/640.0;

            // At this point, top 2 rectangles meet all the criteria so likely have a valid target.
            // Draw them in green.
            if (stream_pipeline_output) {
                for (int ix=0; ix<2; ++ix) {
                    cv::RotatedRect rect = filtered_min_rects[ix];
                    drawRectangle(frame_out_, rect, color_green, 2);
                }

                // Draw line between centres
                cv::line(frame_out_, left_center, right_center, color_green, 2);

                // Draw marker through center
                cv::drawMarker(frame_out_, center_point, color_green, cv::MARKER_CROSS, 20 /*marker size*/, 2 /*thickness*/);
            }

            // Calculate offset = ratio right rectangle / left rectangle.
            // If ratio > 1 ==> robot right of target
            // If ratio < 1 ==> robot left of target
            double rect_ratio = getRectArea(right_rect) / getRectArea(left_rect);
            nt_target_rect_ratio.SetDouble(rect_ratio);

            // Estimate yaw.  Assume both rectangles at equal height (among other things).
            //double yaw = pixels_off_center * (camera_hfov_deg / width_pixels);
            double pixels_off_center = center_point.x - (width_pixels/2);
            double pixels_per_inch = dist_bet_centers / dist_bet_centers_inch;
            double inches_off_center = pixels_off_center / pixels_per_inch;
            double yaw_in_rad = atan(inches_off_center / dist_to_target);
            double yaw_in_deg = yaw_in_rad * 180.0 / M_PI;
            nt_target_yaw_deg.SetDouble(yaw_in_deg);

            // Estimate distance to each rectangle based on its height + coordinate of bot rel to target
            double l_rect_height = std::max(left_rect.size.height, left_rect.size.width);
            double r_rect_height = std::max(right_rect.size.height, right_rect.size.width);
            double l_rect_width = std::min(left_rect.size.height, left_rect.size.width);
            double r_rect_width = std::min(right_rect.size.height, right_rect.size.width);
            double l_rect_dist_inch = 12.0*(206.0/l_rect_height);
            double r_rect_dist_inch = 12.0*(206.0/r_rect_height);
            double bot_x_offset_inch = (pow(r_rect_dist_inch,2) - pow(l_rect_dist_inch,2))/(2*dist_bet_centers_inch);
            double bot_z_offset_inch = sqrt(pow(l_rect_dist_inch,2) - pow(bot_x_offset_inch - 0.5*dist_bet_centers_inch,2));
            bot_x_offset_inch = -bot_x_offset_inch;  // Flip X coordinate to negative if bot on left of target, not opposite.
            nt_rect_l_dist_inch.SetDouble(l_rect_dist_inch);
            nt_rect_r_dist_inch.SetDouble(r_rect_dist_inch);
            nt_bot_x_offset_inch.SetDouble(bot_x_offset_inch);
            nt_bot_z_offset_inch.SetDouble(bot_z_offset_inch);
            
            // Publish info on the 2 rectangles.
            nt_rect_l_angle_deg.SetDouble(left_rect.angle);
            nt_rect_r_angle_deg.SetDouble(right_rect.angle);
            nt_rect_l_height.SetDouble(l_rect_height);
            nt_rect_r_height.SetDouble(r_rect_height);
            nt_rect_l_width.SetDouble(l_rect_width);
            nt_rect_r_width.SetDouble(r_rect_width);


            // TODO: Filter on vertical distance of rect from center?  Only keep pairs of rectangles meeting other critria that are at similar height.
            //       Measure angle vs. perpendicular to target
            //       Measure coordinates & orientation vs. target.

            // Publish other results on network table
            nt_target_dist_pixels.SetDouble(dist_bet_centers);
            nt_target_dist_inch.SetDouble(dist_to_target);
            nt_target_valid.SetBoolean(true);

            // Flush NT updates after all data has been posted.
            ntinst.Flush();
        }

    private:

        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;

    };

    
    // Example pipeline
    class XeroPipeline : public frc::VisionPipeline {
    public:
        const int frames_to_sample_per_report = 20;
        int frames_processed = 0;
        double total_processing_time = 0;

        XeroPipeline() {
            pipe_elements_.push_back(new XeroPipelineElementHsvThreshold("HSV Threshold"));
            pipe_elements_.push_back(new XeroPipelineElementFindContours("Find Contours"));
        }

        virtual ~XeroPipeline();

        virtual void Process(cv::Mat& mat) override {
            const double start_time = frc::Timer::GetFPGATimestamp();
            //std::cout << "Process  " << mat.cols << "   " << mat.rows << "\n";
            cv::Mat* frame_out_p = &mat;
            for (auto& elem : pipe_elements_) {
                elem->Process(*frame_out_p);
                frame_out_p = &(elem->getFrameOut());
            }
            output_frame_ = *frame_out_p;
            ++frames_processed;
            const double end_time = frc::Timer::GetFPGATimestamp();
            total_processing_time += (end_time - start_time);

            // Report average processing time every X calls,
            // then reset metrics for next window to measure and report
            if (frames_processed == frames_to_sample_per_report) {
                double runtime_ms = 1000.0 * total_processing_time / frames_processed;
                nt_pipe_runtime_ms.SetDouble(runtime_ms);
                //std::cout << "Average pipe processing time per frame (ms) = " << runtime_ms << " (" << frames_processed << " frames)\n";
                frames_processed = 0;
                total_processing_time = 0;
            }
        }

        cv::Mat output_frame_;

    private:
        std::vector<XeroPipelineElement*> pipe_elements_;
    };

    class VisionPipelineResultProcessor {
    public:

        VisionPipelineResultProcessor(bool stream_output) : stream_output_(stream_output) {
            if (stream_output_) {
                output_stream_ = frc::CameraServer::GetInstance()->PutVideo("Pipeline Output", width_pixels, height_pixels);
            }
            start_time = frc::Timer::GetFPGATimestamp();
            times_called = 0;
        }
        
        void operator()(XeroPipeline& pipe) {
            ++times_called;
            if (stream_output_) {
                output_stream_.PutFrame(pipe.output_frame_);
            }
            if ((times_called % 20) == 0) {
                const double current_time = frc::Timer::GetFPGATimestamp();
                double elapsed_time = current_time - start_time;
                double fps = static_cast<double>(times_called) / elapsed_time;
                nt_pipe_fps.SetDouble(fps);
                //std::cout << "fps = " << fps << "\n";
                start_time = current_time;
                times_called = 0;
            }
        }
        
    private:

        cs::CvSource output_stream_;
        bool stream_output_;
        double start_time;
        int times_called;
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
            // Wait for camera device to be readable otherwise starting camera will fail
            while (!xero::file::is_readable(cameraConfig.path)) {
                std::cout << "Waiting for camera device " << cameraConfig.path << " to become readable\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        
            cameras.emplace_back(StartCamera(cameraConfig));
        }

        // Start image processing if present.  First one only.
        auto pipe = std::make_shared<XeroPipeline>();
        if (cameras.size() >= 1) {
            std::cout << "Starting vision pipeline\n";

            std::thread t([&] {
                              frc::VisionRunner<XeroPipeline> runner(cameras[0],
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
        for (auto&& cameraConfig : cameraConfigs) {
            (void)setTrackingExposure(cameraConfig.path);
            //(void)setViewingExposure(cameraConfig.path);
        }

        /*
        // Select one of the cameras for streaming for now and toggle.  Just for testing.
        cs::VideoSink server = frc::CameraServer::GetInstance()->GetServer();
        server.SetSource(cameras[0]);
        std::this_thread::sleep_for(std::chrono::seconds(4));
        server.SetSource(cameras[1]);
        std::this_thread::sleep_for(std::chrono::seconds(4));
        server.SetSource(cameras[0]);
        std::this_thread::sleep_for(std::chrono::seconds(4));
        server.SetSource(cameras[1]);
        */

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
    const std::string stream_camera_param_name("vision:stream_camera");
    if (params.hasParam(stream_camera_param_name)) {
        stream_camera = (params.getValue(stream_camera_param_name) != 0);
    }
    const std::string stream_pipeline_output_param_name("vision:stream_pipeline_output");
    if (params.hasParam(stream_pipeline_output_param_name)) {
        stream_pipeline_output = (params.getValue(stream_pipeline_output_param_name) != 0);
    }
    width_pixels = params.getValue("vision:camera:width_pixels");
    height_pixels = params.getValue("vision:camera:height_pixels");

    // Start network table in client or server mode + configure it
    ntinst = nt::NetworkTableInstance::GetDefault();
    startNetworkTable(ntinst);
    
    // Prepare network table variables that tracker will populate
    std::shared_ptr<NetworkTable> nt_table = ntinst.GetTable("TargetTracking");
    nt_pipe_fps = nt_table->GetEntry("pipe_fps");
    nt_pipe_fps.SetDefaultDouble(0);
    nt_pipe_runtime_ms = nt_table->GetEntry("pipe_runtime_ms");
    nt_pipe_runtime_ms.SetDefaultDouble(0);
    nt_target_dist_pixels = nt_table->GetEntry("dist_pixels");
    nt_target_dist_pixels.SetDefaultDouble(0);
    nt_target_dist_inch = nt_table->GetEntry("dist_inch");
    nt_target_dist_inch.SetDefaultDouble(0);
    nt_target_yaw_deg = nt_table->GetEntry("yaw_deg");
    nt_target_yaw_deg.SetDefaultDouble(0);
    nt_target_rect_ratio = nt_table->GetEntry("rect_ratio");
    nt_target_rect_ratio.SetDefaultDouble(0);
    nt_rect_l_angle_deg = nt_table->GetEntry("rect_l_angle_deg");
    nt_rect_l_angle_deg.SetDefaultDouble(0);
    nt_rect_r_angle_deg = nt_table->GetEntry("rect_r_angle_deg");
    nt_rect_r_angle_deg.SetDefaultDouble(0);
    nt_rect_l_height = nt_table->GetEntry("rect_l_height");
    nt_rect_l_height.SetDefaultDouble(0);
    nt_rect_r_height = nt_table->GetEntry("rect_r_height");
    nt_rect_r_height.SetDefaultDouble(0);
    nt_rect_l_width = nt_table->GetEntry("rect_l_width");
    nt_rect_l_width.SetDefaultDouble(0);
    nt_rect_r_width = nt_table->GetEntry("rect_r_width");
    nt_rect_r_width.SetDefaultDouble(0);
    nt_rect_l_dist_inch = nt_table->GetEntry("rect_l_dist_inch");
    nt_rect_l_dist_inch.SetDefaultDouble(0);
    nt_rect_r_dist_inch = nt_table->GetEntry("rect_r_dist_inch");
    nt_rect_r_dist_inch.SetDefaultDouble(0);
    nt_bot_x_offset_inch = nt_table->GetEntry("bot_x_offset_inch");
    nt_bot_x_offset_inch.SetDefaultDouble(0);
    nt_bot_z_offset_inch = nt_table->GetEntry("bot_z_offset_inch");
    nt_bot_z_offset_inch.SetDefaultDouble(0);
    nt_target_valid = nt_table->GetEntry("valid");
    nt_target_valid.SetDefaultBoolean(false);

    // Start camera streaming + image processing on last camera if present
    runPipelineFromCamera(/*cameraConfigs*/);

    // Start image processing from video file
    //runPipelineFromVideo("cube1.mp4");
    //runPipelineFromVideo("capout.avi");
}
