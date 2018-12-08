#pragma once

/// Bits 0-31 are assigned to the base class, so bits that control this specific robot's sub-systems start with bits 32.
#define MSG_GROUP_SORTER               (1ull << 32)
#define MSG_GROUP_SORTER_VERBOSE       (1ull << 33)
#define MSG_GROUP_SHOOTER              (1ull << 34)
#define MSG_GROUP_SHOOTER_VERBOSE      (1ull << 35)
