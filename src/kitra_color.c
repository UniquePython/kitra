#include "kitra_internal.h"

#include <math.h>

const KitraColor KITRA_BLACK = KITRA_COLOR_RGB(0, 0, 0);
const KitraColor KITRA_WHITE = KITRA_COLOR_RGB(255, 255, 255);
const KitraColor KITRA_RED = KITRA_COLOR_RGB(255, 0, 0);
const KitraColor KITRA_GREEN = KITRA_COLOR_RGB(0, 255, 0);
const KitraColor KITRA_BLUE = KITRA_COLOR_RGB(0, 0, 255);
const KitraColor KITRA_YELLOW = KITRA_COLOR_RGB(255, 255, 0);
const KitraColor KITRA_MAGENTA = KITRA_COLOR_RGB(255, 0, 255);
const KitraColor KITRA_CYAN = KITRA_COLOR_RGB(0, 255, 255);
const KitraColor KITRA_ORANGE = KITRA_COLOR_RGB(255, 165, 0);
const KitraColor KITRA_PURPLE = KITRA_COLOR_RGB(128, 0, 128);
const KitraColor KITRA_GRAY = KITRA_COLOR_RGB(128, 128, 128);
const KitraColor KITRA_DARK_GRAY = KITRA_COLOR_RGB(64, 64, 64);
const KitraColor KITRA_LIGHT_GRAY = KITRA_COLOR_RGB(192, 192, 192);
const KitraColor KITRA_TRANSPARENT = KITRA_COLOR_RGBA(0, 0, 0, 0);

uint32_t KitraColorToU32(KitraColor color)
{
    return ((uint32_t)color.r << 24) |
           ((uint32_t)color.g << 16) |
           ((uint32_t)color.b << 8) |
           (uint32_t)color.a;
}

KitraColor KitraColorFromU32(uint32_t value)
{
    return (KitraColor){
        .r = (uint8_t)(value >> 24),
        .g = (uint8_t)(value >> 16),
        .b = (uint8_t)(value >> 8),
        .a = (uint8_t)(value),
    };
}

KitraColor KitraColorLerp(KitraColor a, KitraColor b, float t)
{
    if (t <= 0.0f)
        return a;
    if (t >= 1.0f)
        return b;

    return (KitraColor){
        .r = (uint8_t)((int)a.r + ((int)b.r - (int)a.r) * t),
        .g = (uint8_t)((int)a.g + ((int)b.g - (int)a.g) * t),
        .b = (uint8_t)((int)a.b + ((int)b.b - (int)a.b) * t),
        .a = (uint8_t)((int)a.a + ((int)b.a - (int)a.a) * t),
    };
}

KitraColor KitraColorFade(KitraColor color, float alpha)
{
    if (alpha <= 0.0f)
        color.a = 0;
    else if (alpha >= 1.0f)
        color.a = 255;
    else
        color.a = (uint8_t)(alpha * 255.0f);
    return color;
}

KitraColor KitraColorFromHSV(float h, float s, float v)
{
    if (s <= 0.0f)
    {
        uint8_t c = (uint8_t)(v * 255.0f);
        return KITRA_COLOR_RGB(c, c, c);
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

    return KITRA_COLOR_RGB(
        (uint8_t)(r * 255.0f),
        (uint8_t)(g * 255.0f),
        (uint8_t)(b * 255.0f));
}

void KitraColorToHSV(KitraColor color, float *h, float *s, float *v)
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

KitraColor KitraColorBrighten(KitraColor color, float amount)
{
    int r = (int)(color.r + amount * 255.0f);
    int g = (int)(color.g + amount * 255.0f);
    int b = (int)(color.b + amount * 255.0f);
    color.r = (uint8_t)(r > 255 ? 255 : r);
    color.g = (uint8_t)(g > 255 ? 255 : g);
    color.b = (uint8_t)(b > 255 ? 255 : b);
    return color;
}

KitraColor KitraColorDarken(KitraColor color, float amount)
{
    int r = (int)(color.r - amount * 255.0f);
    int g = (int)(color.g - amount * 255.0f);
    int b = (int)(color.b - amount * 255.0f);
    color.r = (uint8_t)(r < 0 ? 0 : r);
    color.g = (uint8_t)(g < 0 ? 0 : g);
    color.b = (uint8_t)(b < 0 ? 0 : b);
    return color;
}

KitraColor KitraColorInvert(KitraColor color)
{
    color.r = 255 - color.r;
    color.g = 255 - color.g;
    color.b = 255 - color.b;
    return color;
}

bool KitraColorIsEqual(KitraColor a, KitraColor b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}