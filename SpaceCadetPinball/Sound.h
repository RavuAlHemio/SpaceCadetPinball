#pragma once
#include "maths.h"
#include "TPinballComponent.h"

struct ChannelInfo
{
	int TimeStamp;
	vector2 Position;
};

class Sound
{
public:
	static std::vector<ChannelInfo> Channels;

	static void Init(bool mixOpen, bool enableFlag, int volume);
	static void Enable(bool enableFlag);
	static void Activate();
	static void Deactivate();
	static void Close();
	static void PlaySound(MIX_Track* trackPtr, int time, TPinballComponent *soundSource, const char* info);
	static MIX_Track* LoadWaveFile(const std::string& lpName);
	static void FreeSound(MIX_Track* track);
	static void SetVolume(float volume);
private:
	static int num_channels;
	static bool enabled_flag;
	static float Volume;
	static bool MixOpen;
};
