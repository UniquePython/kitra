#include "cinder_internal.h"
#include "cinder_blend.h"

static SDL_BlendMode CinderToSDLBlendMode(CinderBlendMode mode)
{
    switch (mode)
    {
    case CINDER_BLEND_NONE:
        return SDL_BLENDMODE_NONE;
    case CINDER_BLEND_ALPHA:
        return SDL_BLENDMODE_BLEND;
    case CINDER_BLEND_ADDITIVE:
        return SDL_BLENDMODE_ADD;
    case CINDER_BLEND_MULTIPLY:
        return SDL_BLENDMODE_MUL;
    default:
        return SDL_BLENDMODE_NONE;
    }
}

CinderStatus CinderSetBlendMode(CinderBlendMode mode)
{
    if (!gCinderCtx.core.renderer)
    {
        CINDER_LOG(CINDER_LOG_ERROR, "Renderer is NULL in CinderSetBlendMode");
        return CINDER_STATUS_RENDERER_MISSING;
    }

    SDL_SetRenderDrawBlendMode(gCinderCtx.core.renderer, CinderToSDLBlendMode(mode));
    return CINDER_STATUS_OK;
}

CinderStatus CinderSetTextureBlendMode(CinderTexture *tex, CinderBlendMode mode)
{
    if (!tex)
    {
        CINDER_LOG(CINDER_LOG_ERROR, "Texture is NULL in CinderSetTextureBlendMode");
        return CINDER_STATUS_TEXTURE_NULL;
    }

    SDL_SetTextureBlendMode(CinderTextureGetSDL(tex), CinderToSDLBlendMode(mode));
    return CINDER_STATUS_OK;
}