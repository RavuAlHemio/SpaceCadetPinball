#pragma once

#include <SDL3_mixer/SDL_mixer.h>


/// Global audio objects common to sound and music playback.

class audio
{
public:
	static bool audio_init();
	static void audio_shutdown();

	/// @brief The global mixer object that mixes sounds and music.
	static MIX_Mixer *globalMixer;

	static constexpr const char *TAG_SOUNDS = "sounds";
	static constexpr const char *TAG_MUSIC = "music";
};
