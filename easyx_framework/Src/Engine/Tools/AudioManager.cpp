#include"ZHNpch.h"
#include "AudioManager.h"
#include"ZHN_Math.h"
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

AudioManager::AudioManager()
{
	// 启动命令处理线程
	running = true;
	commandProcessorThread = std::thread(&AudioManager::CommandProcessor, this);

	// 设置线程优先级
	SetThreadPriority(commandProcessorThread.native_handle(), THREAD_PRIORITY_TIME_CRITICAL);
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

void AudioManager::CommandProcessor()
{
	while (running) {
		auto start = std::chrono::high_resolution_clock::now();

		std::function<void()> cmd;
		{
			std::unique_lock<std::mutex> lock(stateMutex);
			playCV.wait(lock, [this] { return !commandQueue.empty() || !running; });

			if (!running) break;

			cmd = commandQueue.front();
			commandQueue.pop();
		}

		try {
			cmd();
		}
		catch (const std::exception& e) {
			//记录一般错误日记
			LOG(Log::LEVEL_ERR,
				"Command execution error:  \"%s\"", e.what());
		}

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

		if (duration > std::chrono::microseconds(500)) {
			LOG(Log::LEVEL_WAR,
				"Command processing took %lld us", duration.count());
		}
	}
}

void AudioManager::EnqueueCommand(std::function<void()> cmd)
{
	{
		std::lock_guard<std::mutex> lock(stateMutex);
		commandQueue.push(cmd);
	}
	playCV.notify_one();
}

void AudioManager::Shutdown()
{
	// 停止所有音频
	EnqueueCommand([this] {
		InternalStop();
		});
}

/**
 * 设置背影音量.
 * 受全局音量影响
 * \param 设置音量
 */
void AudioManager::SetBGVolume(int vol)
{
	EnqueueCommand([this, vol] {
		InternalSetBGVolume(vol);
		});
	
}

/**
 * 加载音频.
 * 
 * \param 音频名称
 */
void AudioManager::Load(std::string name)
{
	EnqueueCommand([this, name] {
		InternalLoad(name);
		});
}

/**
 * 播放音乐.
 * 
 * \param 音频名称
 * \param 是否重复播放 默认不重复
 */
void AudioManager::Play(std::string name, bool repeat)
{
	EnqueueCommand([this, name, repeat] {
		InternalPlay(name, repeat);
		});
	//mciSendString((std::string("seek ") + paths[currentpath] + std::string(" to start")).c_str(), 0, 0, 0);
	
}

/**
 * 播放音乐.
 * 播放下一首
 * \param 是否重复 默认不重复
 */
void AudioManager::Play(bool repeat)
{
	EnqueueCommand([this, repeat] {
		InternalPlay(repeat);
		});
	
	///*mciSendString((std::string("close ") + currentpath).c_str(), 0, 0, 0); 
	//mainWorld.resourcePool->AudioPool.erase(currentpath);*/
	//Pause();
	//Next();

	//this->repeat = repeat;
	//if (currentpath == "null")return;
	//mciSendString((std::string("seek ") + paths[currentpath] + std::string(" to start")).c_str(), 0, 0, 0);

	///*char now[100];
	//mciSendString((std::string("status ") + currentpath + std::string(" position")).c_str(), now, 100, 0);
	//int t = atoi(now);
	//std::cout << "number" << t << std::endl;*/
	//mciSendString((std::string("play ") + paths[currentpath] + (repeat ? std::string(" repeat") : "")).c_str(), 0, 0, 0);
}

/**
 * 停止播放.
 * 
 */
void AudioManager::Stop()
{
	EnqueueCommand([this] {
		InternalStop();
		});
	
}

/**
 * 移除音乐资源.
 * 
 * \param 音频名字
 */
void AudioManager::Remove(std::string name)
{
	EnqueueCommand([this, name] {
		InternalRemove(name);
		});

}

/**
 * 设置是否随机播放.
 * 
 * \param 是否随机播放	ture是 false否
 */
void AudioManager::SetRandom(bool random)
{
	EnqueueCommand([this, random] {
		this->random = random;
		});
}

/**
 * 只有调用这个函数才会切换到下一首并播放.
 * 
 */
void AudioManager::NextSound()
{
	EnqueueCommand([this] {
		next = true;
		InternalPlay(repeat);
		});
}

/**
 * 关闭所有音频并且从资源池中移除.
 * 
 */
AudioManager::~AudioManager()
{
	// 停止命令处理线程
	running = false;
	playCV.notify_all();
	if (commandProcessorThread.joinable()) {
		commandProcessorThread.join();
	}

	// 释放所有音频资源
	for (auto& obj : paths)
	{
		mciSendString((std::string("close ") + obj.second).c_str(), 0, 0, 0);
		mainWorld.resourcePool->AudioPool.erase(obj.second);
	}
}

/**
 * 关闭当前正在播放的音频.
 * 并且释放当前资源.
 */
void AudioManager::Close()
{
	if(currentpath!="null")
	refCounts.erase(currentpath);
	mciSendString((std::string("close ") + paths[currentpath]).c_str(), 0, 0, 0);
	/*UpdateIndex();*/
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
	if (refCounts.empty())return;
	
	next = false;

	int index = -1;
	if (currentpath != "null")
	{
		index = refCounts[currentpath];
	}
	
	//随机播放
	random ? index = int(Math::RandInt(0, refCounts.size() - 1)) : index = (index + 1) % refCounts.size();
	index = int(Math::Clamp(index, 0, int(refCounts.size() - 1)));
	std::cout << index << std::endl;
	
	
	for (auto& a : refCounts)
	{
		if (index == a.second) { currentpath = a.first;
		
			return; }
	}
	
}

///暂停音乐
void AudioManager::Pause()
{
	EnqueueCommand([this] {
		std::lock_guard<std::mutex> lock(stateMutex);
		if (currentpath.empty() || playState != PLAYING) return;

		if (mciSendString((std::string("pause ") + paths[currentpath]).c_str(), 0, 0, 0) != 0) {
			HANDLE_MCI_ERROR("Pause command failed");
		}

		playState = PAUSED;
		});
}

///继续音乐
void AudioManager::Resume()
{
	EnqueueCommand([this] {
		std::lock_guard<std::mutex> lock(stateMutex);
		if (currentpath.empty() || playState != PAUSED) return;

		if (mciSendString((std::string("resume ") + paths[currentpath]).c_str(), 0, 0, 0) != 0) {
			HANDLE_MCI_ERROR("Resume command failed");
		}

		playState = PLAYING;
		});
}

void AudioManager::PreloadAsync(const std::vector<std::string>& names)
{
	std::thread([this, names] {
		for (const auto& name : names) {
			Load(name);
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
		}).detach();
}

void AudioManager::InternalPlay(std::string name, bool repeat)
{
	/*std::lock_guard<std::mutex> lock(stateMutex);*/

	// 确保资源存在
	if (paths.find(name) == paths.end()) {
		if (!mainWorld.resourcePool->IsAudio(name)) return;

	// 首次加载
	if (paths.find(name) == paths.end()) {
		paths[name] = mainWorld.resourcePool->AudioPool[name];
		playlist.push_back(name);
		refCounts[name] = 0;
	}

	refCounts[name]++;
	}

	// 停止当前播放
	if (playState != STOPPED) {
		if (currentpath.empty() || playState == STOPPED) return;

		if (mciSendString((std::string("stop ") + paths[currentpath]).c_str(), 0, 0, 0) != 0) {
			HANDLE_MCI_ERROR("Stop command failed");
		}

		playState = STOPPED;
	}

	// 更新状态
	currentpath = name;
	this->repeat = repeat;
	playState = PLAYING;

	// 定位并播放
	auto cmd = std::string("play ") + paths[name] + (repeat ? " repeat" : "");
	if (mciSendString(cmd.c_str(), 0, 0, 0) != 0) {
		HANDLE_MCI_ERROR("Play command failed");
	}

	// 更新索引
	auto it = std::find(playlist.begin(), playlist.end(), name);
	if (it != playlist.end()) {
		currentIndex = std::distance(playlist.begin(), it);
	}
}

void AudioManager::InternalPlay(bool repeat)
{
	/*std::lock_guard<std::mutex> lock(stateMutex);*/

	if (playlist.empty()) return;

	// 计算下一首索引
	size_t nextIndex = 0;
	if (currentIndex != -1) {
		nextIndex = random ? Math::RandInt(0, playlist.size() - 1)
			: (currentIndex + 1) % playlist.size();
	}

	InternalPlay(playlist[nextIndex], repeat);
}

void AudioManager::InternalStop()
{
	/*std::lock_guard<std::mutex> lock(stateMutex);*/

	if (currentpath.empty() || playState == STOPPED) return;

	if (mciSendString((std::string("stop ") + paths[currentpath]).c_str(), 0, 0, 0) != 0) {
		HANDLE_MCI_ERROR("Stop command failed");
	}

	playState = STOPPED;
}

void AudioManager::InternalRemove(std::string name)
{
	/*std::lock_guard<std::mutex> lock(stateMutex);*/

	if (refCounts.find(name) == refCounts.end()) return;

	refCounts[name]--;
	if (refCounts[name] > 0) return; // 仍有引用

	// 安全移除
	auto it = std::find(playlist.begin(), playlist.end(), name);
	if (it != playlist.end()) {
		// 调整当前索引
		if (currentIndex != -1 && currentIndex >= std::distance(playlist.begin(), it)) {
			currentIndex--;
		}
		playlist.erase(it);
	}

	if (currentpath == name) {
		InternalStop();
		currentpath = "";
	}

	mciSendString((std::string("close ") + paths[name]).c_str(), 0, 0, 0);
	mainWorld.resourcePool->AudioPool.erase(name);
	paths.erase(name);
	refCounts.erase(name);
}

void AudioManager::InternalSetBGVolume(int vol)
{
	/*std::lock_guard<std::mutex> lock(stateMutex);*/

	vol = Math::Clamp(vol, 0, 1000);
	BGVol = int(Math::mapping(vol, 0, 1000, 0, SettingVol));

	if (currentpath.empty() || paths.find(currentpath) == paths.end()) return;

	std::string file = std::string("setaudio ") + paths[currentpath] +
		std::string(" volume to ") + std::to_string(BGVol);

	if (mciSendString(file.c_str(), NULL, 0, NULL) != 0) {
		HANDLE_MCI_ERROR("Set volume failed");
	}
}

/**
 * 加载音频.
 *
 * \param 音频名称
 */
void AudioManager::InternalLoad(std::string name)
{
	/*std::lock_guard<std::mutex> lock(stateMutex);*/

	if (!mainWorld.resourcePool->IsAudio(name)) return;

	// 首次加载
	if (paths.find(name) == paths.end()) {
		paths[name] = mainWorld.resourcePool->AudioPool[name];
		playlist.push_back(name);
		refCounts[name] = 0;
	}

	refCounts[name]++;
}

void AudioManager::HandleMCIError(const std::string& context, const std::string& file, int line)
{
	ZHN::Log::instance()->printf(ZHN::Log::LEVEL_ERR, file.c_str(), line,
		"AudioManager Error[\"  %s  \"]: ", context.c_str());

	// 自动恢复机制
	if (playState != STOPPED) {
		playState = STOPPED;
		currentpath = "";
	}
}

//bool IndexSort::operator()(const std::pair<std::string, int> a, const std::pair<std::string, int> b) const
//{
//	if (a.second == b.second)return a < b;
//	else
//	{
//		return a.second < b.second;
//	}
//}

/**
 * 更新次序 返回之后的下一个下标
 *
 */
 //int AudioManager::UpdateIndex()
 //{
 //	if (indexmap.empty())return 0;
 //	int newindex=0;
 //	for (auto& obj:indexmap)
 //	{
 //		if (obj.second != newindex)
 //		{
 //			obj.second = newindex;
 //		}
 //		newindex++;
 //	}
 //	return newindex;
 //}