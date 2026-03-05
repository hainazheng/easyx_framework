#pragma once
#include <mutex>

/// 帧率配置管理类
class FrameRateConfig {
public:

	static FrameRateConfig* instance();

	/// 是否开启帧率限制
	/// @remark 默认为 true，推荐用户不要关闭，否则可能导致不稳定
	bool IsFrameLimitOn() {
		return bFrameLimitOn;
	}

	///是否开启帧率限制
	/// @remark !!! 一般用户都要进行开启 否则不稳定
	void SetFrameLimit(bool enable) {
		bFrameLimitOn = enable;
	}

	/// 获取最大帧率
	/// @remark 默认值为 100，推荐用户根据实际需求调整，但不要超过合理范围
	int GetMaxFrame() {
		return maxFrame;
	}

	///最大帧率(每秒执行多少次)
	///  @remark !!! 一般用户都要进行设置 但是太大每一帧时间太短会极大影响框架
	void SetMaxFrame(int maxFrame_) {
		maxFrame = maxFrame_;
	}

private:
	FrameRateConfig() {};
	~FrameRateConfig();
	bool bFrameLimitOn=true; ///< 内部存储是否开启帧率限制
	int maxFrame = 100;       ///< 内部存储最大帧率

	static FrameRateConfig* Frc;

	static std::mutex m_Mutex;
};