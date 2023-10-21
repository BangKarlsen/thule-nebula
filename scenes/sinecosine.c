#include "sinecosine.h"

static UBYTE sineTable[] = {
  0,   4,   9,  13,  18,  22,  27,  31,  36,  40,
 44,  49,  53,  58,  62,  66,  71,  75,  79,  83,
 88,  92,  96, 100, 104, 108, 112, 116, 120, 124,
128, 132, 136, 139, 143, 147, 150, 154, 158, 161,
165, 168, 171, 175, 178, 181, 184, 187, 190, 193,
196, 199, 202, 204, 207, 210, 212, 215, 217, 219,
222, 224, 226, 228, 230, 232, 234, 236, 237, 239,
241, 242, 243, 245, 246, 247, 248, 249, 250, 251,
252, 253, 254, 254, 255, 255, 255,   1,   1,   1,
  1
};

WORD sinFP(WORD angle)
{
    WORD sin;
    if (angle <= 90) {
        sin = sineTable[angle];
        if (sin == 1) {
            sin = 256;
        }
    }
    else if (angle <= 180) {
        sin = sineTable[180 - angle];
        if (sin == 1) {
            sin = 256;
        }
    }
    else if (angle <= 270) {
        sin = -sineTable[angle - 180];
        if (sin == -1) {
            sin = -256;
        }
    }
    else { /* angle <= 360 */
        sin = -sineTable[360 - angle];
        if (sin == -1) {
            sin = -256;
        }
    }
    return sin;
}

WORD cosFP(WORD angle)
{
    WORD cos;
    if (angle <= 90) {
        cos = sineTable[90 - angle];
        if (cos == 1) {
            cos = 256;
        }
    }
    else if (angle <= 180) {
        cos = -sineTable[angle - 90];
        if (cos == -1) {
            cos = -256;
        }
    }
    else if (angle <= 270) {
        cos = -sineTable[270 - angle];
        if (cos == -1) {
            cos = -256;
        }
    }
    else { /* angle <= 360 */
        cos = sineTable[angle - 270];
        if (cos == 1) {
            cos = 256;
        }
    }
    return cos;
}
