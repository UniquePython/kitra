#include "cinder_internal.h"

const CinderColor CINDER_BLACK = CINDER_COLOR_RGB(0, 0, 0);
const CinderColor CINDER_WHITE = CINDER_COLOR_RGB(255, 255, 255);
const CinderColor CINDER_RED = CINDER_COLOR_RGB(255, 0, 0);
const CinderColor CINDER_GREEN = CINDER_COLOR_RGB(0, 255, 0);
const CinderColor CINDER_BLUE = CINDER_COLOR_RGB(0, 0, 255);
const CinderColor CINDER_YELLOW = CINDER_COLOR_RGB(255, 255, 0);
const CinderColor CINDER_MAGENTA = CINDER_COLOR_RGB(255, 0, 255);
const CinderColor CINDER_CYAN = CINDER_COLOR_RGB(0, 255, 255);

#include <math.h>

CinderColor CinderColorLerp(CinderColor a, CinderColor b, float t)
{
    if (t <= 0.0f)
        return a;
    if (t >= 1.0f)
        return b;

    return (CinderColor){
        .r = (uint8_t)(a.r + (b.r - a.r) * t),
        .g = (uint8_t)(a.g + (b.g - a.g) * t),
        .b = (uint8_t)(a.b + (b.b - a.b) * t),
        .a = (uint8_t)(a.a + (b.a - a.a) * t),
    };
}

CinderColor CinderColorFade(CinderColor color, float alpha)
{
    if (alpha <= 0.0f)
        color.a = 0;
    else if (alpha >= 1.0f)
        color.a = 255;
    else
        color.a = (uint8_t)(alpha * 255.0f);
    return color;
}

CinderColor CinderColorFromHSV(float h, float s, float v)
{
    if (s <= 0.0f)
    {
        uint8_t c = (uint8_t)(v * 255.0f);
        return CINDER_COLOR_RGB(c, c, c);
    }

    h = fmodf(h, 360.0f);
    if (h < 0.0f)
        h += 360.0f;

    float sector = h / 60.0f;
    int i = (int)sector;
    float f = sector - (float)i;

    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float w = v * (1.0f - s * (1.0f - f));

    float r, g, b;
    switch (i)
    {
    case 0:
        r = v;
        g = w;
        b = p;
        break;
    case 1:
        r = q;
        g = v;
        b = p;
        break;
    case 2:
        r = p;
        g = v;
        b = w;
        break;
    case 3:
        r = p;
        g = q;
        b = v;
        break;
    case 4:
        r = w;
        g = p;
        b = v;
        break;
    default:
        r = v;
        g = p;
        b = q;
        break;
    }

    return CINDER_COLOR_RGB(
        (uint8_t)(r * 255.0f),
        (uint8_t)(g * 255.0f),
        (uint8_t)(b * 255.0f));
}

void CinderColorToHSV(CinderColor color, float *h, float *s, float *v)
{
    float r = color.r / 255.0f;
    float g = color.g / 255.0f;
    float b = color.b / 255.0f;

    float max = r > g ? (r > b ? r : b) : (g > b ? g : b);
    float min = r < g ? (r < b ? r : b) : (g < b ? g : b);
    float delta = max - min;

    *v = max;
    *s = (max > 0.0f) ? (delta / max) : 0.0f;

    if (delta <= 0.0f)
        *h = 0.0f;
    else if (max == r)
        *h = 60.0f * fmodf((g - b) / delta, 6.0f);
    else if (max == g)
        *h = 60.0f * ((b - r) / delta + 2.0f);
    else
        *h = 60.0f * ((r - g) / delta + 4.0f);

    if (*h < 0.0f)
        *h += 360.0f;
}

CinderColor CinderColorBrighten(CinderColor color, float amount)
{
    int r = (int)(color.r + amount * 255.0f);
    int g = (int)(color.g + amount * 255.0f);
    int b = (int)(color.b + amount * 255.0f);
    color.r = (uint8_t)(r > 255 ? 255 : r);
    color.g = (uint8_t)(g > 255 ? 255 : g);
    color.b = (uint8_t)(b > 255 ? 255 : b);
    return color;
}

CinderColor CinderColorDarken(CinderColor color, float amount)
{
    int r = (int)(color.r - amount * 255.0f);
    int g = (int)(color.g - amount * 255.0f);
    int b = (int)(color.b - amount * 255.0f);
    color.r = (uint8_t)(r < 0 ? 0 : r);
    color.g = (uint8_t)(g < 0 ? 0 : g);
    color.b = (uint8_t)(b < 0 ? 0 : b);
    return color;
}

CinderColor CinderColorInvert(CinderColor color)
{
    color.r = 255 - color.r;
    color.g = 255 - color.g;
    color.b = 255 - color.b;
    return color;
}

bool CinderColorIsEqual(CinderColor a, CinderColor b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}