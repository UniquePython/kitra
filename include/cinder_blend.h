#ifndef CINDER_BLEND_H_
#define CINDER_BLEND_H_

#include "cinder_types.h"
#include "cinder_texture.h"
#include "cinder_status.h"

typedef enum CinderBlendMode
{
    CINDER_BLEND_NONE,
    CINDER_BLEND_ALPHA,
    CINDER_BLEND_ADDITIVE,
    CINDER_BLEND_MULTIPLY,
} CinderBlendMode;

CinderStatus CinderSetBlendMode(CinderBlendMode mode);
CinderStatus CinderSetTextureBlendMode(CinderTexture *tex, CinderBlendMode mode);

#endif /* CINDER_BLEND_H_ */