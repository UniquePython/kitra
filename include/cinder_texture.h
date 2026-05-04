#ifndef CINDER_TEXTURE_H_
#define CINDER_TEXTURE_H_

#include "cinder_types.h"

typedef struct CinderTexture CinderTexture;

CinderTexture *CinderLoadTexture(const char *path);
void CinderDestroyTexture(CinderTexture **tex);

CinderSize CinderGetTextureSize(const CinderTexture *tex);

void CinderDrawTextureEx(CinderTexture *tex, const CinderRect *src, const CinderRect *dst);
void CinderDrawTexture(CinderTexture *tex, int x, int y);
void CinderDrawTextureP(CinderTexture *tex, CinderPoint pos);

#endif /* CINDER_TEXTURE_H_ */