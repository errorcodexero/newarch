#include "DIJoystickManager.h"
#include <stdexcept>
#include <iostream>

#ifdef __CYGWIN__

namespace xero {
    namespace sim {

        static BOOL CALLBACK callback(const DIDEVICEINSTANCE *inst, void *context) {
            DIJoystickManager *mgr = reinterpret_cast<DIJoystickManager *>(context) ;
            return mgr->enumCallback(inst) ;
        }

        DIJoystickManager::DIJoystickManager() {
            init() ;
        }

        DIJoystickManager::~DIJoystickManager() {
        }

        bool DIJoystickManager::enumCallback(const DIDEVICEINSTANCE *inst) {
            LPDIRECTINPUTDEVICE8 device ;
            DIDEVCAPS caps ;
            DIJOYSTATE2 state;

            if (FAILED(lpDinput_->CreateDevice(inst->guidInstance, (IDirectInputDeviceA**)&device, NULL)))
                return DIENUM_CONTINUE ;

            if (FAILED(device->SetDataFormat(&c_dfDIJoystick2)))
                return DIENUM_CONTINUE ;

            if (FAILED(device->Acquire()))
                return DIENUM_CONTINUE ;

            caps.dwSize = sizeof(caps) ;
            if (FAILED(device->GetCapabilities(&caps)))
                return DIENUM_CONTINUE ;

            devices_.push_back(device) ;
            caps_.push_back(caps) ;
            state_.push_back(state) ;

            return DIENUM_CONTINUE ;
        }

        void DIJoystickManager::init() {
            HINSTANCE hinst = GetModuleHandle(NULL) ;

            if (FAILED(DirectInput8Create(hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&lpDinput_, NULL))) {
                std::runtime_error ex("cannot initialize DirectInput") ;
                throw ex ;
            }

            if (FAILED(lpDinput_->EnumDevices(DI8DEVCLASS_GAMECTRL, callback, this, DIEDFL_ATTACHEDONLY))) {
                std::runtime_error ex("cannot initialize DirectInput") ;
                throw ex ;              
            }
        }

        void DIJoystickManager::poll() {
            for(size_t i = 0 ; i < devices_.size() ; i++) {
                auto dev = devices_[i] ;
                dev->Poll() ;
                dev->GetDeviceState(sizeof(DIJOYSTATE2), &state_[i]) ;

#ifdef PRINT_RAW_DATA
                std::cout << "Device " << i + 1 ;
                std::cout << " axis" ;
                std::cout << " lX " << state_[i].lX ;
                std::cout << " lY " << state_[i].lY ;
                std::cout << " lZ " << state_[i].lZ ;
                std::cout << " lRx " << state_[i].lRx ;
                std::cout << " lRy " << state_[i].lRy ;
                std::cout << " lRz " << state_[i].lRz ;                                                                             
                std::cout << std::endl ;
#endif
            }
        }

        size_t DIJoystickManager::getCount() {
            return devices_.size() ;
        }

        size_t DIJoystickManager::getButtonCount(size_t index) {
            if (index >= devices_.size())
                return 0 ;

            return caps_[index].dwButtons ;
        }

        bool DIJoystickManager::getButtonValue(size_t index, size_t button) {
            if (index >= devices_.size())
                return false ;

            if (button >= getButtonCount(index))
                return false ;

            return (state_[index].rgbButtons[button] & 0x80) ? true : false ;
        }

        size_t DIJoystickManager::getAxisCount(size_t index) {
            if (index >= devices_.size())
                return 0 ;

            return caps_[index].dwAxes ;
        }

        double DIJoystickManager::scaleAxisValue(LONG v) {
            return -static_cast<double>(v - 32767) / 32767.0 ;
        }

        double DIJoystickManager::getAxisValue(size_t index, size_t axis) {
            double ret = 0.0 ;

            if (index >= devices_.size())
                return 0.0 ;            

            if (axis >= getAxisCount(index))
                return 0.0 ;

            switch(axis) {
            case 0:
                ret = scaleAxisValue(state_[index].lX) ;
                break ;

            case 1:
                ret = scaleAxisValue(state_[index].lY) ;
                break ;

            case 2:
                ret = scaleAxisValue(state_[index].lZ) ;
                break ;

            case 3:
                ret = scaleAxisValue(state_[index].lRx) ;
                break ; 

            case 4:
                ret = scaleAxisValue(state_[index].lRy) ;
                break ;                         
            }

            return ret ;
        }

        size_t DIJoystickManager::getPOVCount(size_t index) {
            if (index >= devices_.size())
                return 0 ;

            return caps_[index].dwPOVs ;
        }

        int DIJoystickManager::getPOVValue(size_t index, size_t pov) {
            if (index >= devices_.size())
                return 0 ;

            if (pov >= getPOVCount(index))
                return 0 ;


            auto value = state_[index].rgdwPOV[pov] ;
            if (value == 0xFFFFFFFF)
                return -1 ;

            return value / 100 ;
        }
    }
}

#endif
