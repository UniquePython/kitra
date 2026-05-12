#ifndef CINDER_AUDIO_H_
#define CINDER_AUDIO_H_

#include <stdbool.h>

typedef struct CinderSound CinderSound;
typedef struct CinderMusic CinderMusic;

/**
 * @brief Loads a sound effect from a WAV file.
 *
 * Allocates and returns a new @p CinderSound backed by the given WAV file.
 * The audio subsystem must have been initialized via @p CINDER_SUBSYSTEM_AUDIO
 * passed to @p CinderInit before calling this function.
 *
 * @param path  Path to the WAV file to load.
 * @return      Pointer to a newly allocated @p CinderSound on success, or
 *              @p NULL if the audio subsystem is not initialized, the file
 *              could not be loaded, or memory allocation failed.
 *
 * @note The caller is responsible for freeing the returned sound with
 *       @p CinderFreeSound (or equivalent) when it is no longer needed.
 *
 * @see CinderInit, CinderFreeSound
 */
CinderSound *CinderLoadSound(const char *path);

/**
 * @brief Destroys a sound effect and releases all associated resources.
 *
 * Frees the underlying @p Mix_Chunk and the @p CinderSound allocation, then
 * sets the caller's pointer to @p NULL to prevent dangling references. Safe
 * to call with @p NULL or a pointer to @p NULL.
 *
 * @param sound  Address of the @p CinderSound pointer to destroy.
 *
 * @see CinderLoadSound
 */
void CinderDestroySound(CinderSound **sound);

/**
 * @brief Plays a sound effect on any available channel.
 *
 * Plays @p sound once on the first available mixing channel. If no channel
 * is free the call is silently ignored by the underlying mixer. Does nothing
 * if @p sound is @p NULL.
 *
 * @param sound  Sound to play.
 *
 * @see CinderSetSoundVolume
 */
void CinderPlaySound(CinderSound *sound);

/**
 * @brief Sets the playback volume of a sound effect.
 *
 * @p volume is clamped to [0.0, 1.0] and mapped to the mixer's internal
 * volume range before being applied. Does nothing if @p sound is @p NULL.
 *
 * @param sound   Sound to adjust.
 * @param volume  Desired volume in the range [0.0, 1.0], where 0.0 is
 *                silent and 1.0 is full volume.
 *
 * @see CinderPlaySound
 */
void CinderSetSoundVolume(CinderSound *sound, float volume);

/**
 * @brief Loads a music track from a file.
 *
 * Allocates and returns a new @p CinderMusic backed by the given file.
 * Unlike sound effects, music is streamed rather than loaded entirely into
 * memory, making this suitable for longer tracks. The audio subsystem must
 * have been initialized via @p CINDER_SUBSYSTEM_AUDIO passed to @p CinderInit
 * before calling this function.
 *
 * @param path  Path to the music file to load.
 * @return      Pointer to a newly allocated @p CinderMusic on success, or
 *              @p NULL if the audio subsystem is not initialized, the file
 *              could not be loaded, or memory allocation failed.
 *
 * @note The caller is responsible for freeing the returned music with
 *       @p CinderDestroyMusic when it is no longer needed.
 *
 * @see CinderInit, CinderDestroyMusic, CinderLoadSound
 */
CinderMusic *CinderLoadMusic(const char *path);

/**
 * @brief Destroys a music track and releases all associated resources.
 *
 * Frees the underlying @p Mix_Music handle and the @p CinderMusic allocation,
 * then sets the caller's pointer to @p NULL to prevent dangling references.
 * Safe to call with @p NULL or a pointer to @p NULL.
 *
 * @param music  Address of the @p CinderMusic pointer to destroy.
 *
 * @see CinderLoadMusic
 */
void CinderDestroyMusic(CinderMusic **music);

/**
 * @brief Plays a music track, optionally looping.
 *
 * Begins playback of @p music from the start. Only one music track may play
 * at a time — calling this while another track is playing will replace it.
 * Does nothing if @p music is @p NULL.
 *
 * @param music  Music track to play.
 * @param loop   If @p true, the track loops indefinitely until stopped;
 *               if @p false, it plays once and stops.
 *
 * @see CinderStopMusic, CinderPauseMusic, CinderSetMusicVolume
 */
void CinderPlayMusic(CinderMusic *music, bool loop);

/**
 * @brief Stops music playback immediately.
 *
 * Halts the currently playing music track. Has no effect if no music
 * is playing.
 *
 * @see CinderPlayMusic, CinderPauseMusic
 */
void CinderStopMusic(void);

/**
 * @brief Pauses the currently playing music track.
 *
 * Pauses playback without resetting the position, allowing it to be resumed
 * from the same point. Has no effect if no music is playing or if music is
 * already paused.
 *
 * @see CinderResumeMusic, CinderStopMusic
 */
void CinderPauseMusic(void);

/**
 * @brief Resumes a paused music track.
 *
 * Continues playback from the position at which the track was paused.
 * Has no effect if the music is not paused.
 *
 * @see CinderPauseMusic, CinderPlayMusic
 */
void CinderResumeMusic(void);

/**
 * @brief Returns whether music is currently playing.
 *
 * @return @p true if a music track is actively playing and not paused,
 *         @p false otherwise.
 *
 * @see CinderPlayMusic, CinderPauseMusic
 */
bool CinderIsMusicPlaying(void);

/**
 * @brief Sets the global music playback volume.
 *
 * @p volume is clamped to [0.0, 1.0] and mapped to the mixer's internal
 * volume range before being applied. Affects all music playback globally
 * rather than a specific track.
 *
 * @param volume  Desired volume in the range [0.0, 1.0], where 0.0 is
 *                silent and 1.0 is full volume.
 *
 * @see CinderPlayMusic, CinderSetSoundVolume
 */
void CinderSetMusicVolume(float volume);

#endif /* CINDER_AUDIO_H_ */