#pragma once

/// \file


//
// This file defines subsystem IDs for subsystems in the base library.  The first
// 32 bits of the 64 bit subsystem code are reserved for the base library
//

/// \brief ID for the settings parser messages
#define MSG_GROUP_PARSER                (1ull << 0)

/// \brief ID for the subsystems base messages
#define MSG_GROUP_SUBSYSTEMS            (1ull << 1)

/// \brief ID for the tankdrive subsystem messages
#define MSG_GROUP_TANKDRIVE             (1ull << 2)

/// \brief ID for the OI subsystem messages
#define MSG_GROUP_OI                    (1ull << 3)

/// \brief ID for the actions related messages
#define MSG_GROUP_ACTIONS               (1ull << 4)

/// \brief ID for the path follower message logging
#define MSG_GROUP_PATHFOLLOWER			(1ull << 5)

/// \brief ID for the path manager message logging
#define MSG_GROUP_PATHMGR				(1ull << 6)

/// \brief ID for the robot loop debug messages
#define MSG_GROUP_ROBOTLOOP             (1ull << 7)

/// \brief ID to get all messages
#define MSG_GROUP_ALL                   (0xffffffffffffffffull)
