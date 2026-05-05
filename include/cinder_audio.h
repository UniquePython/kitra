#ifndef CINDER_AUDIO_H_
#define CINDER_AUDIO_H_

#include <stdbool.h>

typedef struct CinderSound CinderSound;
typedef struct CinderMusic CinderMusic;

CinderSound *CinderLoadSound(const char *path);
void CinderDestroySound(CinderSound **sound);
void CinderPlaySound(CinderSound *sound);
void CinderSetSoundVolume(CinderSound *sound, float volume); // 0.0 - 1.0

CinderMusic *CinderLoadMusic(const char *path);
void CinderDestroyMusic(CinderMusic **music);
void CinderPlayMusic(CinderMusic *music, bool loop);
void CinderStopMusic(void);
void CinderPauseMusic(void);
void CinderResumeMusic(void);
bool CinderIsMusicPlaying(void);
void CinderSetMusicVolume(float volume); // 0.0 - 1.0

#endif /* CINDER_AUDIO_H_ */