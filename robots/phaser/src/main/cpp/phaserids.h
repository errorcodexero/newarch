#pragma once

#include <basegroups.h>

//
// This file contains the group numbers for message logging.  Group nubmers
// are 64 bit number with a single bit set.  The numbers with the bits 32 to
// 63 are reserved for the robot and can be used here in this file.  The numbers
// with bits 0 to 31 set are reserved for the xerobase library and cannot be used
// here
//

#define MSG_GROUP_PHASER_LIFTER                     (1ull << 32)

#define MSG_GROUP_PHASER_TURNTABLE                  (1ull << 33)

#define MSG_GROUP_VISION_DRIVING                    (1ull << 34)

#define MSG_GROUP_CARGO_INTAKE                      (1ull << 35)

