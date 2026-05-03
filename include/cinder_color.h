#ifndef CINDER_COLOR_H_
#define CINDER_COLOR_H_

#include "cinder_types.h"

#include <stdint.h>

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

#endif /* CINDER_COLOR_H_ */