#include "utils.h"

Color operator+ (const Color& c0, const Color& c1)
{
    unsigned char _r = ((int)c0.r + c1.r) > 255 ? 255 : c0.r + c1.r;
    unsigned char _g = ((int)c0.g + c1.g) > 255 ? 255 : c0.g + c1.g;
    unsigned char _b = ((int)c0.b + c1.b) > 255 ? 255 : c0.b + c1.b;
   unsigned char _a = ((int)c0.a + c1.a) > 255 ? 255 : c0.a + c1.a;

    return { _r, _g, _b, _a };
}

