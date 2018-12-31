#pragma once

/// Bits 0-31 are assigned to the base class, so bits that control this specific robot's sub-systems start with bits 32.
#define MSG_GROUP_SHOOTER              (1ull << 34)
#define MSG_GROUP_SHOOTER_VERBOSE      (1ull << 35)
#define MSG_GROUP_INTAKE               (1ull << 36)
#define MSG_GROUP_HOPPER               (1ull << 37)
#define MSG_GROUP_COLLECTOR            (1ull << 38)
