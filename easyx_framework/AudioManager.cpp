#include "AudioManager.h"
#include"Math.h"
#include "easyx.h"
#include"ResourceManager.h"
#include"World.h"


//debug mcisendstring ����ֻ��һ��ʹ�� ֻҪ�ر��������ͷ��ʲ�����
AudioManager* AudioManager::manager = nullptr;

std::mutex AudioManager::m_Mutex;

/**
 * ��ȡʵ��.
 * 
 * \return ʵ��ָ��
 */
AudioManager* AudioManager::instance()
{
	if (!manager)
	{
		std::unique_lock<std::mutex> lock(m_Mutex); // ���� һ�����ö�����ͷ� ��ʱ����
		if (!manager)
		{
			manager = new AudioManager;
		}
	}/*std::cout << manager;*/
	return manager;
}

/**
 * ��ʼ��.
 * ��ǰ��������
 */
void AudioManager::Initialize()
{
	Load("boss");
	Load("secer");
	Load("Awake");
}

/**
 * ���ñ�Ӱ����.
 * ��ȫ������Ӱ��
 * \param ��������
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
 * ������Ƶ.
 * 
 * \param ��Ƶ����
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
 * ��������.
 * 
 * \param ��Ƶ����
 * \param �Ƿ��ظ����� Ĭ�ϲ��ظ�
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
 * ��������.
 * ������һ��
 * \param �Ƿ��ظ� Ĭ�ϲ��ظ�
 */
void AudioManager::Play(bool repeat)
{
	
	
	/*mciSendString((std::string("close ") + currentpath).c_str(), 0, 0, 0); 
	mainWorld.resourcePool->AudioPool.erase(currentpath);*/
	Pause();
	Next();

	//if (!mainWorld.resourcePool->IsAudio(currentpath))
	//{
	//	//˵����Ƶ�Ѿ����ʹ�
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
 * ֹͣ����.
 * 
 */
void AudioManager::Stop()
{
	if (currentpath == "null")return;
	mciSendString((std::string("stop ") + paths[currentpath]).c_str(), 0, 0, 0);
	//UpdateIndex();
}

/**
 * �Ƴ�������Դ.
 * 
 * \param ��Ƶ����
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
 * �����Ƿ��������.
 * 
 * \param �Ƿ��������	ture�� false��
 */
void AudioManager::SetRandom(bool random)
{
	this->random = random;
}

/**
 * ֻ�е�����������Ż��л�����һ�ײ�����.
 * 
 */
void AudioManager::NextSound()
{
	next = true;
	Play();
}
/**
 * �ر�������Ƶ���Ҵ���Դ�����Ƴ�.
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
 * ���´��� ����֮�����һ���±�
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
 * �رյ�ǰ���ڲ��ŵ���Ƶ.
 * �����ͷŵ�ǰ��Դ.
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
 * ��ȡ��һ�β��ŵĴ���.
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
	
	//�������
	random ? index = int(Math::RandInt(0, indexmap.size() - 1)) : index = (index + 1) % indexmap.size();
	index = int(Math::Clamp(index, 0, int(indexmap.size() - 1)));
	std::cout << index << std::endl;
	
	
	for (auto& a : indexmap)
	{
		if (index == a.second) { currentpath = a.first;
		
			return; }
	}
	
}

///��ͣ����
void AudioManager::Pause()
{
	if (currentpath == "null")return;
	mciSendString((std::string("pause ") + paths[currentpath]).c_str(), 0, 0, 0);
}
///��������
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
