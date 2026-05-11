#include "audio.h"

MIX_Mixer* audio::globalMixer = nullptr;

bool audio::audio_init()
{
	MIX_Init();
	globalMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
	return globalMixer != nullptr;
}

void audio::audio_shutdown()
{
	if (globalMixer != nullptr)
	{
		MIX_DestroyMixer(globalMixer);
		globalMixer = nullptr;
	}
	MIX_Quit();
}
