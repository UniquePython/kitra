#ifndef CINDER_TEXT_H_
#define CINDER_TEXT_H_

#include "cinder_types.h"

typedef struct CinderFont CinderFont;
typedef struct CinderText CinderText;

CinderFont *CinderLoadFont(const char *path, int size);
void CinderDestroyFont(CinderFont **font);

void CinderDrawText(CinderFont *font, const char *text, int x, int y, CinderColor color);
void CinderDrawTextP(CinderFont *font, const char *text, CinderPoint pos, CinderColor color);

CinderText *CinderCreateText(CinderFont *font, const char *text, CinderColor color);

void CinderDrawCachedText(CinderText *t, int x, int y);
void CinderDrawCachedTextP(CinderText *t, CinderPoint pos);

void CinderDestroyCachedText(CinderText **t);

CinderSize CinderMeasureText(CinderFont *font, const char *text);

#endif /* CINDER_TEXT_H_ */