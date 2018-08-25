#pragma once

#include <basegroups.h>

//
// This file contains the group numbers for message logging.  Group nubmers
// are 64 bit number with a single bit set.  The numbers with the bits 32 to
// 63 are reserved for the robot and can be used here in this file.  The numbers
// with bits 0 to 31 set are reserved for the xerobase library and cannot be used
// here
//

/// \brief the message logger group ID for the grabber subsystem
#define MSG_GROUP_GRABBER                   (1ull << 32)

/// \brief the message logger group ID for the lifter subsystem
#define MSG_GROUP_LIFTER                    (1ull << 33)

/// \brief the message lgoger group ID for the intake
#define MSG_GROUP_INTAKE					(1ull << 34)

/// \brief the message logger group ID for the collector
#define MSG_GROUP_COLLECTOR					(1ull << 35)


