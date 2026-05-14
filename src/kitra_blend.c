#include "kitra_internal.h"
#include "kitra_blend.h"

static SDL_BlendMode KitraToSDLBlendMode(KitraBlendMode mode)
{
    switch (mode)
    {
    case KITRA_BLEND_NONE:
        return SDL_BLENDMODE_NONE;
    case KITRA_BLEND_ALPHA:
        return SDL_BLENDMODE_BLEND;
    case KITRA_BLEND_ADDITIVE:
        return SDL_BLENDMODE_ADD;
    case KITRA_BLEND_MULTIPLY:
        return SDL_BLENDMODE_MUL;
    default:
        return SDL_BLENDMODE_NONE;
    }
}

KitraStatus KitraSetBlendMode(KitraBlendMode mode)
{
    if (!gKitraCtx.core.renderer)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Renderer is NULL in KitraSetBlendMode");
        return KITRA_STATUS_RENDERER_MISSING;
    }

    SDL_SetRenderDrawBlendMode(gKitraCtx.core.renderer, KitraToSDLBlendMode(mode));
    return KITRA_STATUS_OK;
}

KitraStatus KitraSetTextureBlendMode(KitraTexture *tex, KitraBlendMode mode)
{
    if (!tex)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Texture is NULL in KitraSetTextureBlendMode");
        return KITRA_STATUS_TEXTURE_NULL;
    }

    SDL_SetTextureBlendMode(KitraTextureGetSDL(tex), KitraToSDLBlendMode(mode));
    return KITRA_STATUS_OK;
}