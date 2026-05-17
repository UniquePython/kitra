#include "kitra_internal.h"

struct KitraSound
{
    Mix_Chunk *chunk;
    int channel;
};

struct KitraMusic
{
    Mix_Music *handle;
};

// --------------------------------------- SOUND ---------------------------------------

KitraSound *KitraLoadSound(const char *path)
{
    if (!gKitraCtx.core.audioInitialized)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Audio not initialized — pass KITRA_SUBSYSTEM_AUDIO to KitraInit");
        return NULL;
    }

    Mix_Chunk *chunk = Mix_LoadWAV(path);
    if (!chunk)
    {
        KITRA_LOG(KITRA_LOG_ERROR, Mix_GetError());
        return NULL;
    }

    KitraSound *sound = malloc(sizeof(KitraSound));
    if (!sound)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Failed to allocate KitraSound");
        Mix_FreeChunk(chunk);
        return NULL;
    }

    sound->chunk = chunk;
    sound->channel = -1;
    return sound;
}

void KitraDestroySound(KitraSound **sound)
{
    if (!sound || !*sound)
        return;

    Mix_FreeChunk((*sound)->chunk);
    free(*sound);
    *sound = NULL;
}

void KitraPlaySound(KitraSound *sound)
{
    if (!sound)
        return;

    sound->channel = Mix_PlayChannel(-1, sound->chunk, 0);
}

void KitraStopSound(KitraSound *sound)
{
    if (!sound || sound->channel == -1)
        return;

    if (Mix_GetChunk(sound->channel) == sound->chunk)
        Mix_HaltChannel(sound->channel);
}

void KitraSetSoundVolume(KitraSound *sound, float volume)
{
    if (!sound)
        return;

    if (volume < 0.0f)
        volume = 0.0f;
    if (volume > 1.0f)
        volume = 1.0f;

    Mix_VolumeChunk(sound->chunk, (int)(volume * MIX_MAX_VOLUME));
}

void KitraPlaySoundVolume(KitraSound *sound, float volume)
{
    if (!sound)
        return;

    if (volume < 0.0f)
        volume = 0.0f;
    if (volume > 1.0f)
        volume = 1.0f;

    int saved = Mix_VolumeChunk(sound->chunk, -1);
    Mix_VolumeChunk(sound->chunk, (int)(volume * MIX_MAX_VOLUME));
    sound->channel = Mix_PlayChannel(-1, sound->chunk, 0);
    Mix_VolumeChunk(sound->chunk, saved);
}

void KitraSetMasterSoundVolume(float volume)
{
    if (volume < 0.0f)
        volume = 0.0f;
    if (volume > 1.0f)
        volume = 1.0f;

    Mix_Volume(-1, (int)(volume * MIX_MAX_VOLUME));
}

// --------------------------------------- MUSIC ---------------------------------------

KitraMusic *KitraLoadMusic(const char *path)
{
    if (!gKitraCtx.core.audioInitialized)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Audio not initialized — pass KITRA_SUBSYSTEM_AUDIO to KitraInit");
        return NULL;
    }

    Mix_Music *music = Mix_LoadMUS(path);
    if (!music)
    {
        KITRA_LOG(KITRA_LOG_ERROR, Mix_GetError());
        return NULL;
    }

    KitraMusic *cmusic = malloc(sizeof(KitraMusic));
    if (!cmusic)
    {
        KITRA_LOG(KITRA_LOG_ERROR, "Failed to allocate KitraMusic");
        Mix_FreeMusic(music);
        return NULL;
    }

    cmusic->handle = music;
    return cmusic;
}

void KitraDestroyMusic(KitraMusic **music)
{
    if (!music || !*music)
        return;

    Mix_FreeMusic((*music)->handle);
    free(*music);
    *music = NULL;
}

void KitraPlayMusic(KitraMusic *music, bool loop)
{
    if (!music)
        return;

    Mix_PlayMusic(music->handle, loop ? -1 : 1);
}

void KitraStopMusic(void)
{
    Mix_HaltMusic();
}

void KitraPauseMusic(void)
{
    Mix_PauseMusic();
}

void KitraResumeMusic(void)
{
    Mix_ResumeMusic();
}

bool KitraIsMusicPlaying(void)
{
    return Mix_PlayingMusic() && !Mix_PausedMusic();
}

bool KitraIsMusicPaused(void)
{
    return Mix_PausedMusic() == 1;
}

void KitraSetMusicVolume(float volume)
{
    if (volume < 0.0f)
        volume = 0.0f;
    if (volume > 1.0f)
        volume = 1.0f;

    Mix_VolumeMusic((int)(volume * MIX_MAX_VOLUME));
}