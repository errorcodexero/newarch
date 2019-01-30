#include "Turntable.h"
using namespace xero::misc;
using namespace xero::base;

namespace xero{
    namespace phaser{
        Turntable::Turntable(xero::base::Robot &robot, uint64_t id){
            int motor = robot.getSettingsParser().getInteger("hw:turntable:motor") ;
            int enc1 = robot.getSettingsParser().getInteger("hw:turntable:encoder1") ;
            int enc2 = robot.getSettingsParser().getInteger("hw:turntable:encoder2") ;
            
            motor_ = std::make_shared<frc::VictorSP>(motor) ;
            encoder_ = std::make_shared<frc::Encoder>(enc1, enc2) ;

            min_angle_ = robot.getSettingsParser().getDouble("grabber:turntable:minimum") ;
            max_angle_ = robot.getSettingsParser().getDouble("grabber:turntable:maximum") ;
            degrees_per_tick_ = robot.getSettingsParser().getDouble("turntable:degrees_per_tick") ;

        }

        Turntable::~Turntable() {
        }

        bool Turntable::canAcceptAction(xero::base::ActionPtr action) {
        }

        void Turntable::computeState(){
            angle_ = (encoder_->Get()) * degrees_per_tick_ ;
        }
    }
}