#include "AudioManager.h"
#include"Math.h"
#include "easyx.h"
#include"ResourceManager.h"
#include"World.h"


//debug mcisendstring 命名只能一次使用 只要关闭用命名就访问不到了
AudioManager* AudioManager::manager = nullptr;

std::mutex AudioManager::m_Mutex;

/**
 * 获取实例.
 * 
 * \return 实例指针
 */
AudioManager* AudioManager::instance()
{
	if (!manager)
	{
		std::unique_lock<std::mutex> lock(m_Mutex); // 加锁 一创建好对象就释放 耗时操作
		if (!manager)
		{
			manager = new AudioManager;
		}
	}/*std::cout << manager;*/
	return manager;
}

/**
 * 初始化.
 * 提前加载音乐
 */
void AudioManager::Initialize()
{
	Load("boss");
	Load("secer");
	Load("Awake");
}

/**
 * 设置背影音量.
 * 受全局音量影响
 * \param 设置音量
 */
void AudioManager::SetBGVolume(int vol)
{
	vol = Math::Clamp(vol, 0, 1000);/* int(Math::Clamp(vol, 0, 1000) * SettingVol / 1000.f);*/
	BGVol = int(Math::mapping(vol, 0, 1000, 0, SettingVol));
	if (currentpath == "null")return;
		std::string file = std::string("setaudio ") + paths[currentpath] + std::string(" volume to ") + std::to_string(BGVol);
		mciSendString(file.c_str(), NULL, 0, NULL);
	
}
/**
 * 加载音频.
 * 
 * \param 音频名称
 */
void AudioManager::Load(std::string name)
{
	if (mainWorld.resourcePool->IsAudio(name))
	{
		paths.insert({ name,mainWorld.resourcePool->AudioPool[name]});
		indexmap.insert({ name,UpdateIndex() });
		/*std::cout << 123;*/
	}
}
/**
 * 播放音乐.
 * 
 * \param 音频名称
 * \param 是否重复播放 默认不重复
 */
void AudioManager::Play(std::string name, bool repeat)
{
	
	if (indexmap.find(name) == indexmap.end())return;/*std::cout << 123;*/
	Pause();
	this->repeat = repeat;
	currentpath = name;
	mciSendString((std::string("seek ") + paths[currentpath] + std::string(" to start")).c_str(), 0, 0, 0);
	mciSendString((std::string("play ") + paths[currentpath] + (repeat ? std::string(" repeat") : "")).c_str(), 0, 0, 0);
	
}

/**
 * 播放音乐.
 * 播放下一首
 * \param 是否重复 默认不重复
 */
void AudioManager::Play(bool repeat)
{
	
	
	/*mciSendString((std::string("close ") + currentpath).c_str(), 0, 0, 0); 
	mainWorld.resourcePool->AudioPool.erase(currentpath);*/
	Pause();
	Next();

	//if (!mainWorld.resourcePool->IsAudio(currentpath))
	//{
	//	//说明音频已经访问过
	//	mainWorld.resourcePool->Load(currentpath, currentpath);
	//}

	this->repeat = repeat;
	if (currentpath == "null")return;
	mciSendString((std::string("seek ") + paths[currentpath] + std::string(" to start")).c_str(), 0, 0, 0);

	/*char now[100];
	mciSendString((std::string("status ") + currentpath + std::string(" position")).c_str(), now, 100, 0);
	int t = atoi(now);
	std::cout << "number" << t << std::endl;*/
	mciSendString((std::string("play ") + paths[currentpath] + (repeat ? std::string(" repeat") : "")).c_str(), 0, 0, 0);
}

/**
 * 停止播放.
 * 
 */
void AudioManager::Stop()
{
	if (currentpath == "null")return;
	mciSendString((std::string("stop ") + paths[currentpath]).c_str(), 0, 0, 0);
	//UpdateIndex();
}

/**
 * 移除音乐资源.
 * 
 * \param 音频名字
 */
void AudioManager::Remove(std::string name)
{
	indexmap.erase(name);mainWorld.resourcePool->AudioPool.erase(name);
	mciSendString((std::string("close ") + name).c_str(), 0, 0, 0);
	if (currentpath == name)
	{	
		currentpath = "null";
		
	}
	UpdateIndex();
}

/**
 * 设置是否随机播放.
 * 
 * \param 是否随机播放	ture是 false否
 */
void AudioManager::SetRandom(bool random)
{
	this->random = random;
}

/**
 * 只有调用这个函数才会切换到下一首并播放.
 * 
 */
void AudioManager::NextSound()
{
	next = true;
	Play();
}
/**
 * 关闭所有音频并且从资源池中移除.
 * 
 */
AudioManager::~AudioManager()
{
	for (auto& obj : paths)
	{
		mciSendString((std::string("close ") + obj.second).c_str(), 0, 0, 0);
		mainWorld.resourcePool->AudioPool.erase(obj.second);
	}
}
/**
 * 更新次序 返回之后的下一个下标
 * 
 */
int AudioManager::UpdateIndex()
{
	if (indexmap.empty())return 0;
	int newindex=0;
	for (auto& obj:indexmap)
	{
		if (obj.second != newindex)
		{
			obj.second = newindex;
		}
		newindex++;
	}
	return newindex;
}

/**
 * 关闭当前正在播放的音频.
 * 并且释放当前资源.
 */
void AudioManager::Close()
{
	if(currentpath!="null")
	indexmap.erase(currentpath);
	mciSendString((std::string("close ") + paths[currentpath]).c_str(), 0, 0, 0);
	UpdateIndex();
	mainWorld.resourcePool->AudioPool.erase(currentpath);
	currentpath = "null";
}

/**
 * 获取下一次播放的次序.
 * 
 */
void AudioManager::Next()
{
	if (!next)return;
	if (indexmap.empty())return;
	
	next = false;

	int index = -1;
	if (currentpath != "null")
	{
		index = indexmap[currentpath];
	}
	
	//随机播放
	random ? index = int(Math::RandInt(0, indexmap.size() - 1)) : index = (index + 1) % indexmap.size();
	index = int(Math::Clamp(index, 0, int(indexmap.size() - 1)));
	std::cout << index << std::endl;
	
	
	for (auto& a : indexmap)
	{
		if (index == a.second) { currentpath = a.first;
		
			return; }
	}
	
}

///暂停音乐
void AudioManager::Pause()
{
	if (currentpath == "null")return;
	mciSendString((std::string("pause ") + paths[currentpath]).c_str(), 0, 0, 0);
}
///继续音乐
void AudioManager::Resume()
{
	if (currentpath == "null")return;
	mciSendString((std::string("resume ") + paths[currentpath]).c_str(), 0, 0, 0);
}

//bool IndexSort::operator()(const std::pair<std::string, int> a, const std::pair<std::string, int> b) const
//{
//	if (a.second == b.second)return a < b;
//	else
//	{
//		return a.second < b.second;
//	}
//}
