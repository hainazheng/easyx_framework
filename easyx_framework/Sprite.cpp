#include "Sprite.h"
#include"Math.h"

///�ָ���˸��ʶ
void Sprite::Recover()
{
	blinkFlag = false;
}

//������������Ҫ�ֶ����룬Ҫ��ʾ�ڵ�ͼ��Ҫ�Լ����
//�¼�����Ⱦ�������
///������Ⱦ�����������������
Sprite::Sprite()
{
	renderer = ConstructComponent<SpriteRenderer>();				//renderer����Ϊ�����崴���������ָ��
	renderer->AttachTo(root);							//ʵ�����������Ļ����������԰�


}

void Sprite::BeginPlay()
{
	Object::BeginPlay();
}

///ʵ����˸����
void Sprite::Update(float deltaTime)
{
	Object::Update(deltaTime);
	
	this->deltaTime = deltaTime;

	if (IsBlinking)				
	{
		if (!blinkFlag) {			//�ں���
			if (transistionLevel > 0)transistionLevel -= transistionSpeed;
			if (transistionLevel <= 0)IsBlinking = false;						//debug 2024.9.28û�ӵȺţ����µ������ʱ��Ͳ�ִ�в���
			/*std::cout << transistionLevel << std::endl;*/
		}
		renderer->RemoveFilter();
		if (IsBlinking)renderer->AddFilter({blinkFillter.color,(int)transistionLevel,true,5});
	}
}

/**
 * ����ͼƬ��Դ.
 * 
 * \param [in] name ͼƬ����
 */
void Sprite::LoadSprite(std::string name)
{
	renderer->LoadSprite(name);
}

/**
 * ��˸Ч��.
 * 
 * \param [in] duration ��˸��ʱ��(��λ����)
 * \param [in] col ��ɫ
 * \param [in] level �˾��㼶
 */
void Sprite::Blink(float duration, COLORREF col, int level)
{
	if (!IsBlinking)
	{
		duration = Math::Clamp(duration, 0.1f, 1.f);
		IsBlinking = true; blinkFlag = true;
		blinkFillter = { col,level,true,5 };
		transistionLevel = float(level);
		transistionSpeed = 2 * level * deltaTime / duration;
		recoverTimerHandle.Bind(duration / 2.f, this, &Sprite::Recover);

	}
}
