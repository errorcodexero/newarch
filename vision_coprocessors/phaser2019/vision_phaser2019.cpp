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

#include <networktables/NetworkTableInstance.h>
#include <vision/VisionPipeline.h>
#include <vision/VisionRunner.h>
#include <wpi/StringRef.h>
#include <wpi/json.h>
#include <wpi/raw_istream.h>
#include <wpi/raw_ostream.h>

#include "cameraserver/CameraServer.h"

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"



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


static const char* configFile = "/boot/frc.json";

namespace {

    unsigned int team;
    bool nt_server = false;

    
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
            // Set threshold to only select green
            //hsv_ranges = {40, 70, 40, 255, 40, 255};
            //hsv_ranges = {0, 180, 100, 255, 50, 255};
            //hsv_ranges = {75, 85, 200, 255, 200, 255};
            //hsv_ranges = {65, 95, 200, 255, 200, 255};
            //hsv_ranges = {55, 105, 200, 255, 100, 255};
            
            hsv_ranges = {0, 255, 0, 255, 0, 255};
        }
        
        virtual void Process(cv::Mat& frame_in) {
            //frame_out_ = frame_in;

            cv::cvtColor(frame_in, hsv_image, cv::COLOR_BGR2HSV);

            cv::inRange(hsv_image, cv::Scalar(hsv_ranges[0], hsv_ranges[2], hsv_ranges[4]), cv::Scalar(hsv_ranges[1], hsv_ranges[3], hsv_ranges[5]), green_only_image_);
            //cv::inRange(frame_in, cv::Scalar(0, 0, 0), cv::Scalar(0, 255, 0), green_only_image_);

            
            cv::cvtColor(green_only_image_, frame_out_, cv::COLOR_GRAY2BGR);
            //frame_out_ = green_only_image_;
            //std::cout << frame_in.channels() << "    " << frame_out_.channels() << "\n";
            cv::circle(frame_out_, cv::Point(100,100), 50,  cv::Scalar(0,0,255));
        }

    private:

        std::vector<int> hsv_ranges;
        cv::Mat hsv_image, green_only_image_;

    };

    
    // Example pipeline
    class XeroPipeline : public frc::VisionPipeline {
    public:
        int val = 0;

        XeroPipeline() {
            pipe_elements_.push_back(new XeroPipelineElementHsvThreshold("test element"));
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


}  // namespace



int main(int argc, char* argv[]) {
    const bool stream_pipeline_output = true;

    if (argc >= 2) {
        configFile = argv[1];
    }

    // Read configuration
    if (!ReadConfig()) {
        return EXIT_FAILURE;
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

    // Send sample parameter to network table
    std::shared_ptr<NetworkTable> nt_table = ntinst.GetTable("ObjectDetection");
    nt::NetworkTableEntry nt_table_entry = nt_table->GetEntry("MyEntry");
    nt_table_entry.SetDouble(1.5);

    // Start cameras
    std::vector<cs::VideoSource> cameras;
    for (auto&& cameraConfig : cameraConfigs) {
        cameras.emplace_back(StartCamera(cameraConfig));
    }

    // Start image processing on camera 0 if present
    auto pipe = std::make_shared<XeroPipeline>();
    if (cameras.size() >= 1) {

        std::thread t([&] {
                frc::VisionRunner<XeroPipeline> runner(cameras[0],
                                                       pipe.get(),
                                                       VisionPipelineResultProcessor(stream_pipeline_output));
                runner.RunForever();
            });
        t.detach();
    }

    // Loop forever
    for (;;) std::this_thread::sleep_for(std::chrono::seconds(10));
}
