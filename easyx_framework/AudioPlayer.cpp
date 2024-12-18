#include "AudioPlayer.h"
#include "Camera.h"
#include "Math.h"
#include"World.h"
#include "easyx.h"


using std::string;


AudioPlayer::~AudioPlayer()
{
	for (auto& obj : paths)
	{
		mciSendString((string("close ") + obj).c_str(), 0, 0, 0);
	}
}

///ʵ�ֿռ��
void AudioPlayer::Update(float deltaTime)
{
	SceneComponent::Update(deltaTime);

	if (!bIsEnabled)return;

	if (bSpacial)
	{
		int vol = volume;
		float dis = Vector2D::Distance(mainWorld.mainCamera->GetWorldPosition(), GetWorldPosition());
		if (dis <= innerRadius) { ; }
		else if (dis > outerRadius)vol = 0;
		else vol = (outerRadius - dis) * float(volume) / (outerRadius - innerRadius);
		vol = Math::Clamp(vol, 0, 1000);
		if (currentVol == vol)return;
		currentVol = vol;
		for (auto& obj : paths)
		{
			string file = string("setaudio ") + obj + string(" volume to ") + std::to_string(vol);
			mciSendString(file.c_str(), NULL, 0, NULL);
		}
	}
}

void AudioPlayer::SetVolume(int vol)
{
	volume = Math::Clamp(vol, 0, 1000);
	if (bSpacial)return;
	for (auto& obj : paths)
	{
		string file = string("setaudio ") + obj + string(" volume to ") + std::to_string(vol);
		mciSendString(file.c_str(), NULL, 0, NULL);
	}
}

void AudioPlayer::Play(string name, bool repeat)
{
	paths.insert(name);
	mciSendString((string("seek ") + name + string(" to start")).c_str(), 0, 0, 0);
	mciSendString((string("play ") + name + (repeat ? string(" repeat") : "")).c_str(), 0, 0, 0);

	if (bSpacial)return;
	string file = string("setaudio ") + name + string(" volume to ") + std::to_string(volume);
	mciSendString(file.c_str(), NULL, 0, NULL);
}

void AudioPlayer::Stop(string name)
{
	mciSendString((string("stop ") + name).c_str(), 0, 0, 0);
}

void AudioPlayer::Close(string name)
{
	paths.erase(name);
	mciSendString((string("close ") + name).c_str(), 0, 0, 0);
}

void AudioPlayer::Pause(string name)
{
	mciSendString((string("pause ") + name).c_str(), 0, 0, 0);
}

void AudioPlayer::Resume(string name)
{
	mciSendString((string("resume ") + name).c_str(), 0, 0, 0);
}