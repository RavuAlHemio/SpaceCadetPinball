#include "pch.h"
#include "audio.h"
#include "options.h"
#include "Sound.h"
#include "maths.h"

int Sound::num_channels;
bool Sound::enabled_flag = false;
std::vector<ChannelInfo> Sound::Channels{};
float Sound::Volume = 1.0f;
bool Sound::MixOpen = false;

void Sound::Init(bool mixOpen, bool enableFlag, int volume)
{
	MixOpen = mixOpen;
	Volume = volume;
	Enable(enableFlag);
}

void Sound::Enable(bool enableFlag)
{
	enabled_flag = enableFlag;
	if (MixOpen && !enableFlag)
		MIX_StopTag(audio::globalMixer, audio::TAG_SOUNDS, 0);
}

void Sound::Activate()
{
	if (MixOpen)
		MIX_ResumeTag(audio::globalMixer, audio::TAG_SOUNDS);
}

void Sound::Deactivate()
{
	if (MixOpen)
		MIX_PauseTag(audio::globalMixer, audio::TAG_SOUNDS);
}

void Sound::Close()
{
	Enable(false);
	Channels.clear();
}

void Sound::PlaySound(MIX_Track* trackPtr, int time, TPinballComponent* soundSource, const char* info)
{
	if (MixOpen && trackPtr && enabled_flag)
	{
		if (options::Options.SoundStereo)
		{
			// Positional audio uses collision grid 2D coordinates normalized to [0, 1]
			// Point (0, 0) is bottom left table corner; point (1, 1) is top right table corner.
			// Z is defined as: 0 at table level, positive axis goes up from table surface.

			// Get the source sound position.
			// Sound without position are assumed to be at the center top of the table.
			vector3 soundPos{};
			if (soundSource)
			{
				auto soundPos2D = soundSource->get_coordinates();
				soundPos = {soundPos2D.X, soundPos2D.Y, 0.0f};
			}
			else
			{
				soundPos = {0.5f, 1.0f, 0.0f};
			}

			// Listener is positioned at the bottom center of the table,
			// at 0.5 height, so roughly a table half - length.
			vector3 playerPos = {0.5f, 0.0f, 0.5f};
			auto soundDir = maths::vector_sub(soundPos, playerPos);

			// Find sound angle from positive Y axis in clockwise direction with atan2
			// Remap atan2 output from (-Pi, Pi] to [0, 2 * Pi)
			auto angle = fmodf(atan2(soundDir.X, soundDir.Y) + Pi * 2, Pi * 2);

			// Distance from listener to the sound position is roughly in the [0, ~1.22] range.
			// Remap to [0, 122] by multiplying by 100 and cast to an integer.
			auto distance = 100.0f * maths::magnitude(soundDir);

			auto point = MIX_Point3D {
				SDL_cosf(angle) * distance,
				0.0f,
				SDL_sinf(angle) * distance };

			// Mix_SetPosition expects an angle in (Sint16)degrees, where
			// angle 0 is due north, and rotates clockwise as the value increases.
			// Mix_SetPosition expects a (Uint8)distance from 0 (near) to 255 (far).
			MIX_SetTrack3DPosition(trackPtr, &point);

			// Output position of each sound emitted so we can verify
			// the sanity of the implementation.
			/*printf("X: %3.3f Y: %3.3f Angle: %3.3f Distance: %3d, Object: %s\n",
					soundPos.X,
					soundPos.Y,
					angleDeg,
					distance,
					info
			);*/
		}
		MIX_PlayTrack(trackPtr, 0);
	}
}

MIX_Track* Sound::LoadWaveFile(const std::string& lpName)
{
	if (!MixOpen)
		return nullptr;

	auto wavFile = fopenu(lpName.c_str(), "r");
	if (!wavFile)
		return nullptr;
	fclose(wavFile);

	auto wavAudio = MIX_LoadAudio(audio::globalMixer, lpName.c_str(), true);
	auto wavTrack = MIX_CreateTrack(audio::globalMixer);
	MIX_SetTrackAudio(wavTrack, wavAudio);
	MIX_TagTrack(wavTrack, audio::TAG_SOUNDS);
	return wavTrack;
}

void Sound::FreeSound(MIX_Track* wave)
{
	if (MixOpen && wave)
		MIX_DestroyTrack(wave);
}

void Sound::SetVolume(float volume)
{
	Volume = volume;
	if (MixOpen)
		MIX_SetTagGain(audio::globalMixer, audio::TAG_SOUNDS, volume);
}
