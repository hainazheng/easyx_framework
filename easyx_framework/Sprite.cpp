#include "Sprite.h"
#include"Math.h"

///恢复闪烁标识
void Sprite::Recover()
{
	blinkFlag = false;
}

//加入世界容器要手动加入，要显示在地图上要自己添加
//新加入渲染组件操作
///创建渲染组件，加入世界容器
Sprite::Sprite()
{
	renderer = ConstructComponent<SpriteRenderer>();				//renderer设置为用物体创建的组件的指针
	renderer->AttachTo(root);							//实现组件和物体的基本场景属性绑定


}

void Sprite::BeginPlay()
{
	Object::BeginPlay();
}

///实现闪烁功能
void Sprite::Update(float deltaTime)
{
	Object::Update(deltaTime);
	
	this->deltaTime = deltaTime;

	if (IsBlinking)				
	{
		if (!blinkFlag) {			//在后半段
			if (transistionLevel > 0)transistionLevel -= transistionSpeed;
			if (transistionLevel <= 0)IsBlinking = false;						//debug 2024.9.28没加等号，导致等于零的时候就不执行操作
			/*std::cout << transistionLevel << std::endl;*/
		}
		renderer->RemoveFilter();
		if (IsBlinking)renderer->AddFilter({blinkFillter.color,(int)transistionLevel,true,5});
	}
}

/**
 * 加载图片资源.
 * 
 * \param [in] name 图片名字
 */
void Sprite::LoadSprite(std::string name)
{
	renderer->LoadSprite(name);
}

/**
 * 闪烁效果.
 * 
 * \param [in] duration 闪烁总时长(单位：秒)
 * \param [in] col 颜色
 * \param [in] level 滤镜层级
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
