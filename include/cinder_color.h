#ifndef CINDER_COLOR_H_
#define CINDER_COLOR_H_

#include "cinder_types.h"

#include <stdint.h>
#include <stdbool.h>

#define CINDER_COLOR_RGBA(r, g, b, a) \
    (CinderColor) { r, g, b, a }
#define CINDER_COLOR_RGB(r, g, b) (CinderColor){r, g, b, 255}

extern const CinderColor CINDER_BLACK;
extern const CinderColor CINDER_WHITE;
extern const CinderColor CINDER_RED;
extern const CinderColor CINDER_GREEN;
extern const CinderColor CINDER_BLUE;
extern const CinderColor CINDER_YELLOW;
extern const CinderColor CINDER_MAGENTA;
extern const CinderColor CINDER_CYAN;

CinderColor CinderColorLerp(CinderColor a, CinderColor b, float t);
CinderColor CinderColorFade(CinderColor color, float alpha);
CinderColor CinderColorFromHSV(float h, float s, float v);
void CinderColorToHSV(CinderColor color, float *h, float *s, float *v);
CinderColor CinderColorBrighten(CinderColor color, float amount);
CinderColor CinderColorDarken(CinderColor color, float amount);
CinderColor CinderColorInvert(CinderColor color);
bool CinderColorIsEqual(CinderColor a, CinderColor b);

#endif /* CINDER_COLOR_H_ */