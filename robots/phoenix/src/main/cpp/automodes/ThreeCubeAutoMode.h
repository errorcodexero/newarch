#pragma once

#include "PhoenixAutoMode.h"

namespace xero
{
    namespace phoenix
    {
        class ThreeCubeAutoMode : public PhoenixAutoMode
        {
        public:
            enum class Side
            {
                Left,
                Right
            };

        public:
            ThreeCubeAutoMode(xero::base::Robot &robot, Side side);
            virtual ~ThreeCubeAutoMode();

        private:
            static std::string genName(Side side);
            static std::string genDesc(Side side);

            static std::string left_name_;
            static std::string right_name_;
            static std::string left_desc_;
            static std::string right_desc_;

        private:
            Side side_;
        };
    }
}