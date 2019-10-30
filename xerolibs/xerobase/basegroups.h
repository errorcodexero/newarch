#pragma once

/// \file


//
// This file defines subsystem IDs for subsystems in the base library.  The first
// 32 bits of the 64 bit subsystem code are reserved for the base library
//

/// \brief ID for the settings parser messages
#define MSG_GROUP_PARSER                    (1ull << 0)

/// \brief ID for the subsystems base messages
#define MSG_GROUP_SUBSYSTEMS                (1ull << 1)

/// \brief ID for the tankdrive subsystem messages
#define MSG_GROUP_TANKDRIVE                 (1ull << 2)

/// \brief ID for the OI subsystem messages
#define MSG_GROUP_OI                        (1ull << 3)

/// \brief ID for the actions related messages
#define MSG_GROUP_ACTIONS                   (1ull << 4)

/// \brief ID for the path follower message logging
#define MSG_GROUP_PATHFOLLOWER              (1ull << 5)

/// \brief ID for the path manager message logging
#define MSG_GROUP_PATHMGR                   (1ull << 6)

/// \brief ID for the robot loop debug messages
#define MSG_GROUP_ROBOTLOOP                 (1ull << 7)

/// \brief ID for the tank drive subsystem message.  
/// This specific ID gets verbose messages including drivebase status
/// for each robot loop.  This produces a lot of data and should be
/// used cautiously and never during a competition.
#define MSG_GROUP_TANKDRIVE_VERBOSE         (1ull << 8)

#define MSG_GROUP_LINE_FOLLOWER             (1ull << 9)

#define MSG_GROUP_CAMERA_TRACKER            (1ull << 10)

#define MSG_GROUP_ACTIONS_VERBOSE           (1ull << 11)

#define MSG_GROUP_POWER                     (1ull << 12)

#define MSG_GROUP_CAMERA_TRACKER_VERBOSE    (1ull << 13)

#define MSG_GROUP_SIMULATOR                 (1ull << 14)

#define MSG_GROUP_LINE_FOLLOWER_VERBOSE     (1ull << 15)

#define MSG_GROUP_PATHWATCHER               (1ull << 16)

#define MSG_GROUP_FLAGS                     (1ull << 17)

/// \brief ID to get all messages
#define MSG_GROUP_ALL                   (0xffffffffffffffffull)
