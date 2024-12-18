/*****************************************************************//**
 * \file   AudioPlayer.h
 * \brief  音频组件(例如人物语音）
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"SceneComponent.h"


/**
 * 音频组件.
 * @brief 支持混音
 * 一般都是播放短的人物语音
 * @remark !!!要在资源池预加载
 */
class AudioPlayer :public SceneComponent
{

	std::unordered_set<std::string>paths;			///<音频名字容器 一直存着直到析构

	float innerRadius = 0;///<音源内径
	float outerRadius = 0;///<音源外径
	bool bSpacial = false;///<支持空间感的播放器时间开销较大，慎用
	bool bVolume = false;///<是否设置了音量 默认系统音量
	int volume = 1000;///<媒体音量大小，范围为0~1000

	int currentVol = 0;			///<当前音量

public:
	virtual ~AudioPlayer();

	virtual void Update(float deltaTime)override;

	/**
	 * 是否支持空间感.
	 *
	 * \param [in] spacial 是否支持空间感
	 */
	void SetSpacial(bool spacial) { bSpacial = spacial; }

	/**
	 * 设置内外径，从内径到外径声源递减至零.
	 *
	 * \param [in] inner 内径
	 * \param [in] outer 外径
	 */
	void SetRadius(float inner, float outer)
	{
		innerRadius = (inner < 0 ? 0 : inner);
		outerRadius = (outer < 0 ? 0 : outer);
	}

	/**
	 * 设置音量大小.
	 *
	 * \param [in] vol 音量大小
	 */
	void SetVolume(int vol);

	/**
	 * 播放音乐.
	 *
	 * \param [in] name 音频名字
	 * \param [in] repeat 是否重复播放
	 */
	void Play(std::string name, bool repeat = false);

	/**
	 * 回收音乐资源（节省内存，但再次播放会消耗较大性能）.
	 *
	 * \param [in] name 音频名字
	 */
	void Stop(std::string name);

	/**
	 * 停止音乐.
	 * 不支持再次播放
	 * \param name 音频名字
	 */
	void Close(std::string name);

	/**
	 * 暂停音乐.
	 *
	 * \param name 音频名字
	 */
	void Pause(std::string name);

	/**
	 * 继续音乐.
	 *
	 * \param name 音频名字
	 */
	void Resume(std::string name);
};