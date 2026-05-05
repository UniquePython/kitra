#include "cinder_internal.h"

struct CinderSound
{
    Mix_Chunk *chunk;
};

struct CinderMusic
{
    Mix_Music *handle;
};

// --------------------------------------- SOUND ---------------------------------------

CinderSound *CinderLoadSound(const char *path)
{
    if (!gCinderCtx.core.audioInitialized)
    {
        CINDER_LOG(CINDER_LOG_ERROR, "Audio not initialized — pass CINDER_SUBSYSTEM_AUDIO to CinderInit");
        return NULL;
    }

    Mix_Chunk *chunk = Mix_LoadWAV(path);
    if (!chunk)
    {
        CINDER_LOG(CINDER_LOG_ERROR, Mix_GetError());
        return NULL;
    }

    CinderSound *sound = malloc(sizeof(CinderSound));
    if (!sound)
    {
        CINDER_LOG(CINDER_LOG_ERROR, "Failed to allocate CinderSound");
        Mix_FreeChunk(chunk);
        return NULL;
    }

    sound->chunk = chunk;
    return sound;
}

void CinderDestroySound(CinderSound **sound)
{
    if (!sound || !*sound)
        return;

    Mix_FreeChunk((*sound)->chunk);
    free(*sound);
    *sound = NULL;
}

void CinderPlaySound(CinderSound *sound)
{
    if (!sound)
        return;

    Mix_PlayChannel(-1, sound->chunk, 0);
}

void CinderSetSoundVolume(CinderSound *sound, float volume)
{
    if (!sound)
        return;

    if (volume < 0.0f)
        volume = 0.0f;
    if (volume > 1.0f)
        volume = 1.0f;

    Mix_VolumeChunk(sound->chunk, (int)(volume * MIX_MAX_VOLUME));
}

// --------------------------------------- MUSIC ---------------------------------------

CinderMusic *CinderLoadMusic(const char *path)
{
    if (!gCinderCtx.core.audioInitialized)
    {
        CINDER_LOG(CINDER_LOG_ERROR, "Audio not initialized — pass CINDER_SUBSYSTEM_AUDIO to CinderInit");
        return NULL;
    }

    Mix_Music *music = Mix_LoadMUS(path);
    if (!music)
    {
        CINDER_LOG(CINDER_LOG_ERROR, Mix_GetError());
        return NULL;
    }

    CinderMusic *cmusic = malloc(sizeof(CinderMusic));
    if (!cmusic)
    {
        CINDER_LOG(CINDER_LOG_ERROR, "Failed to allocate CinderMusic");
        Mix_FreeMusic(music);
        return NULL;
    }

    cmusic->handle = music;
    return cmusic;
}

void CinderDestroyMusic(CinderMusic **music)
{
    if (!music || !*music)
        return;

    Mix_FreeMusic((*music)->handle);
    free(*music);
    *music = NULL;
}

void CinderPlayMusic(CinderMusic *music, bool loop)
{
    if (!music)
        return;

    Mix_PlayMusic(music->handle, loop ? -1 : 1);
}

void CinderStopMusic(void)
{
    Mix_HaltMusic();
}

void CinderPauseMusic(void)
{
    Mix_PauseMusic();
}

void CinderResumeMusic(void)
{
    Mix_ResumeMusic();
}

bool CinderIsMusicPlaying(void)
{
    return Mix_PlayingMusic() && !Mix_PausedMusic();
}

void CinderSetMusicVolume(float volume)
{
    if (volume < 0.0f)
        volume = 0.0f;
    if (volume > 1.0f)
        volume = 1.0f;

    Mix_VolumeMusic((int)(volume * MIX_MAX_VOLUME));
}