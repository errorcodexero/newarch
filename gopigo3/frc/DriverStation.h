#pragma once

#include "UdpSender.h"
#include "UdpReceiver.h"
#include <cstdint>
#include <thread>
#include <vector>

namespace frc
{
    class DriverStation
    {
    private:
        static const uint16_t ROBOT_IN_PORT = 1110;
        static const uint16_t ROBOT_OUT_PORT = 1150;

        //
        // Meaningful bits from the driverstation protocol
        //
        static const uint8_t cTest = 0x01;
        static const uint8_t cEnabled = 0x04;
        static const uint8_t cAutonomous = 0x02;
        static const uint8_t cTeleoperated = 0x00;
        static const uint8_t cFMS_Attached = 0x08;
        static const uint8_t cEmergencyStop = 0x80;
        static const uint8_t cRequestReboot = 0x08;
        static const uint8_t cRequestNormal = 0x80;
        static const uint8_t cRequestUnconnected = 0x00;
        static const uint8_t cRequestRestartCode = 0x04;
        static const uint8_t cFMS_RadioPing = 0x10;
        static const uint8_t cFMS_RobotPing = 0x08;
        static const uint8_t cFMS_RobotComms = 0x20;
        static const uint8_t cFMS_DS_Version = 0x00;
        static const uint8_t cTagDate = 0x0f;
        static const uint8_t cTagGeneral = 0x01;
        static const uint8_t cTagJoystick = 0x0c;
        static const uint8_t cTagTimezone = 0x10;
        static const uint8_t cRed1 = 0x00;
        static const uint8_t cRed2 = 0x01;
        static const uint8_t cRed3 = 0x02;
        static const uint8_t cBlue1 = 0x03;
        static const uint8_t cBlue2 = 0x04;
        static const uint8_t cBlue3 = 0x05;
        static const uint8_t cRTagCANInfo = 0x0e;
        static const uint8_t cRTagCPUInfo = 0x05;
        static const uint8_t cRTagRAMInfo = 0x06;
        static const uint8_t cRTagDiskInfo = 0x04;
        static const uint8_t cRequestTime = 0x01;
        static const uint8_t cRobotHasCode = 0x20;

    private:
        class JoystickState
        {
        public:
            JoystickState()
            {
                m_buttons = 0;
            }

            ~JoystickState()
            {
            }
            void setButtonCount(size_t count)
            {
                m_button_count = count;
            }

            size_t getButtonCount() const
            {
                return m_button_count;
            }

            void setButtons(uint16_t value)
            {
                uint16_t bcomp = static_cast<uint16_t>(~m_buttons);
                uint16_t vcomp = static_cast<uint16_t>(value);
                uint16_t newpresses = bcomp & value;
                uint16_t newreleases = m_buttons & vcomp;

                m_presses |= newpresses;
                m_presses |= newreleases;

                m_buttons = value;
            }

            uint16_t getButtons() const
            {
                return m_buttons;
            }

            bool getButton(int button) const
            {
                return ((m_buttons & (1 << button)) != 0);
            }

            bool getButtonPressed(int button)
            {
                bool pressed = ((m_presses & (1 << button)) != 0);
                m_presses = static_cast<uint16_t>(m_presses & ~(1 << button));

                return pressed;
            }

            bool getButtonReleased(int button)
            {
                bool released = ((m_releases & (1 << button)) != 0);
                m_releases = static_cast<uint16_t>(m_releases & ~(1 << button));

                return released;
            }

            void setAxisCount(size_t cnt)
            {
                m_axis.resize(cnt);
                m_axistype.resize(cnt);
            }

            size_t getAxisCount() const
            {
                return m_axis.size();
            }


            void setAxis(size_t which, float value)
            {
                m_axis[which] = value;
            }

            float getAxis(size_t which)
            {
                return m_axis[which];
            }

            void setPOVCount(size_t cnt)
            {
                m_pov.resize(cnt);
            }

            size_t getPOVCount() const
            {
                return m_pov.size();
            }

            void setPOVValue(size_t which, uint16_t value)
            {
                m_pov[which] = value;
            }

            uint16_t getPOVValue(size_t which)
            {
                return m_pov[which];
            }

            std::string getName() const
            {
                return m_name;
            }

            void setAxisType(size_t which, int type)
            {
                m_axistype[which] = type;
            }

            int getAxisType(size_t which) const
            {
                return m_axistype[which];
            }

        private:
            size_t m_button_count;
            uint16_t m_buttons;
            uint16_t m_presses;
            uint16_t m_releases;
            std::vector<int> m_axistype;
            std::vector<float> m_axis;
            std::vector<uint16_t> m_pov;
            std::string m_name;
        };

    private:
        DriverStation();

    public:
        enum Alliance
        {
            kRed,
            kBlue,
        };

        enum MatchType {
            kNone, 
            kPractice, 
            kQualification, 
            kElimination
        } ;

    public:
        virtual ~DriverStation();
        static void initialize();

        static DriverStation &GetInstance()
        {
            if (m_ds_p == nullptr)
                initialize();

            return *m_ds_p;
        }

        int GetMatchNumber() const {
            return -1 ;
        }
        
        int GetReplayNumber() const {
            return -1 ;
        }

        MatchType GetMatchType() const {
            return kNone ;
        }

        bool GetStickButton(int stick, int button);
        bool GetStickButtonPressed(int stick, int button);
        bool GetStickButtonReleased(int stick, int button);
        double GetStickAxis(int stick, int axis);
        int GetStickPOV(int stick, int pov);
        int GetStickButtons(int stick) const;
        int GetStickAxisCount(int stick) const;
        int GetStickPOVCount(int stick) const;
        int GetStickButtonCount(int stick) const;

        bool GetJoystickIsXbox(int stick) const
        {
            return false;
        }

        int GetJoystickType() const
        {
            return 0;
        }

        std::string GetJoystickName(int stick) const;
        int GetJoystickAxisType(int stick, int axis) const;

        bool IsEnabled() const
        {
            return m_enabled;
        }

        bool IsDisabled() const
        {
            return !m_enabled;
        }

        bool IsAutonomous() const
        {
            return m_auto_mode;
        }

        bool IsOperatorControl() const
        {
            return !m_auto_mode && !m_test_mode;
        }

        bool IsTest() const
        {
            return m_test_mode;
        }

        bool IsDSAttached() const
        {
            return true;
        }

        bool IsNewControlData()
        {
            bool ret = m_newpacket;
            m_newpacket = false;

            return ret;
        }

        bool IsFMSAttached() const
        {
            return false;
        }

        bool IsSysActive() const
        {
            return true;
        }

        bool IsBrownedOut() const
        {
            return false;
        }

        std::string GetGameSpecificMessage() const
        {
            return "LLL";
        }

        std::string GetEventName() const
        {
            return "PNW District - Wilsonville Event";
        }

        Alliance GetAlliance() const
        {
            return m_alliance;
        }

        int GetLocation() const
        {
            return m_location;
        }

        void WaitForData()
        {
            m_waiting = true;
            while (m_waiting);
        }

        void WaitForData(double timeout);

        double GetMatchTime() const
        {
            return 0.0;
        }

        double GetBatteryVoltage()
        {
            return 12.0;
        }

        void ReportError(const char *msg_p)
        {
            std::cout << msg_p ;
        }

    protected:
        void waitForConnection();

    private:
        void dsRecvCommThread();
        void dsSendCommThread();
        void processBaseDSData(const std::vector<uint8_t> &data, size_t start);
        void processTimeData(const std::vector<uint8_t> &data, size_t start);
        void processTimeZoneData(const std::vector<uint8_t> &data, size_t start);
        void processJoystickData(size_t index, const std::vector<uint8_t> &data, size_t start);

        uint16_t encodeVoltage(float v);

        float byteToFloat(uint8_t value, float max)
        {
            return static_cast<float>(value) * max / 255.0f;
        }

    private:
        static DriverStation *m_ds_p;

        xero::misc::UdpBroadcastSender *m_server_out_p;
        xero::misc::UdpReceiver *m_server_in_p;
        std::thread m_ds_recv_thread;
        std::thread m_ds_send_thread;
        bool m_running;
        uint16_t m_packet_index;

        //
        // Robot characteristics
        //
        bool m_test_mode;
        bool m_auto_mode;
        bool m_enabled;

        bool m_restart_robotcode;
        bool m_reboot_pi;

        Alliance m_alliance;
        int m_location;

        bool m_newpacket;
        bool m_waiting;

        std::vector<JoystickState> m_joysticks;

        //
        // Battery voltage
        //
        float m_voltage;

        //
        // Robot code
        //
        bool m_robot_code;
    };
}
