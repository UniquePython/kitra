#include "kitra_internal.h"

#include <stdlib.h>

static SDL_MessageBoxFlags KitraToSDLMsgBoxFlags(KitraMessageBoxType type)
{
    switch (type)
    {
    case KITRA_MSGBOX_INFO:
        return SDL_MESSAGEBOX_INFORMATION;
    case KITRA_MSGBOX_WARNING:
        return SDL_MESSAGEBOX_WARNING;
    case KITRA_MSGBOX_ERROR:
        return SDL_MESSAGEBOX_ERROR;
    default:
        return SDL_MESSAGEBOX_INFORMATION;
    }
}

KitraStatus KitraShowMessageBox(KitraMessageBoxType type, const char *title, const char *message)
{
    if (!title || !message)
    {
        KITRA_LOG(KITRA_LOG_WARNING, "title or message is NULL in KitraShowMessageBox");
        return KITRA_STATUS_OK;
    }

    if (SDL_ShowSimpleMessageBox(KitraToSDLMsgBoxFlags(type), title, message, gKitraCtx.core.window) != 0)
    {
        KITRA_LOG(KITRA_LOG_ERROR, SDL_GetError());
        return KITRA_STATUS_MSGBOX_FAILED;
    }

    return KITRA_STATUS_OK;
}

int KitraShowDialog(KitraMessageBoxType type, const char *title, const char *message, const KitraMessageBoxButton *buttons, int count)
{
    if (!title || !message || !buttons || count <= 0)
    {
        KITRA_LOG(KITRA_LOG_WARNING, "invalid arguments in KitraShowDialog");
        return -1;
    }

    SDL_MessageBoxButtonData *sdlButtons = malloc(sizeof(SDL_MessageBoxButtonData) * (size_t)count);
    if (!sdlButtons)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "failed to allocate button data in KitraShowDialog");
        return -1;
    }

    for (int i = 0; i < count; i++)
    {
        sdlButtons[i].flags = 0;
        sdlButtons[i].buttonid = buttons[i].id;
        sdlButtons[i].text = buttons[i].text;
    }

    SDL_MessageBoxData data = {
        .flags = KitraToSDLMsgBoxFlags(type),
        .window = gKitraCtx.core.window,
        .title = title,
        .message = message,
        .numbuttons = count,
        .buttons = sdlButtons,
        .colorScheme = NULL,
    };

    int buttonid = -1;
    if (SDL_ShowMessageBox(&data, &buttonid) != 0)
    {
        KITRA_LOG(KITRA_LOG_ERROR, SDL_GetError());
        free(sdlButtons);
        return -1;
    }

    free(sdlButtons);
    return buttonid;
}