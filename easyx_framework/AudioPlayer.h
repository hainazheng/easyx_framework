/*****************************************************************//**
 * \file   AudioPlayer.h
 * \brief  ��Ƶ���(��������������
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"SceneComponent.h"


/**
 * ��Ƶ���.
 * @brief ֧�ֻ���
 * һ�㶼�ǲ��Ŷ̵���������
 * @remark !!!Ҫ����Դ��Ԥ����
 */
class AudioPlayer :public SceneComponent
{

	std::unordered_set<std::string>paths;			///<��Ƶ�������� һֱ����ֱ������

	float innerRadius = 0;///<��Դ�ھ�
	float outerRadius = 0;///<��Դ�⾶
	bool bSpacial = false;///<֧�ֿռ�еĲ�����ʱ�俪���ϴ�����
	bool bVolume = false;///<�Ƿ����������� Ĭ��ϵͳ����
	int volume = 1000;///<ý��������С����ΧΪ0~1000

	int currentVol = 0;			///<��ǰ����

public:
	virtual ~AudioPlayer();

	virtual void Update(float deltaTime)override;

	/**
	 * �Ƿ�֧�ֿռ��.
	 *
	 * \param [in] spacial �Ƿ�֧�ֿռ��
	 */
	void SetSpacial(bool spacial) { bSpacial = spacial; }

	/**
	 * �������⾶�����ھ����⾶��Դ�ݼ�����.
	 *
	 * \param [in] inner �ھ�
	 * \param [in] outer �⾶
	 */
	void SetRadius(float inner, float outer)
	{
		innerRadius = (inner < 0 ? 0 : inner);
		outerRadius = (outer < 0 ? 0 : outer);
	}

	/**
	 * ����������С.
	 *
	 * \param [in] vol ������С
	 */
	void SetVolume(int vol);

	/**
	 * ��������.
	 *
	 * \param [in] name ��Ƶ����
	 * \param [in] repeat �Ƿ��ظ�����
	 */
	void Play(std::string name, bool repeat = false);

	/**
	 * ����������Դ����ʡ�ڴ棬���ٴβ��Ż����Ľϴ����ܣ�.
	 *
	 * \param [in] name ��Ƶ����
	 */
	void Stop(std::string name);

	/**
	 * ֹͣ����.
	 * ��֧���ٴβ���
	 * \param name ��Ƶ����
	 */
	void Close(std::string name);

	/**
	 * ��ͣ����.
	 *
	 * \param name ��Ƶ����
	 */
	void Pause(std::string name);

	/**
	 * ��������.
	 *
	 * \param name ��Ƶ����
	 */
	void Resume(std::string name);
};