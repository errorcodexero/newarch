#pragma once

//
// This file defines subsystem IDs for subsystems in the base library.  The first
// 32 bits of the 64 bit subsystem code are reserved for the base library
//

#define MSG_GROUP_PARSER                (1ull << 0)
#define MSG_GROUP_SUBSYSTEMS            (1ull << 1)
#define MSG_GROUP_TANKDRIVE             (1ull << 2)