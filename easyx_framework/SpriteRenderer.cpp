#include "SpriteRenderer.h"
#include"Windows.h"
#pragma comment(lib,"Msimg32.lib")
#include"Camera.h"
#include"Animator.h"
#include"FileManager.h"



///处理滤镜和旋转
void SpriteRenderer::DealImage()
{
	if (GetWorldRotation() != 0)
	{
		if (sprite)RotateImage(angle * PI / 180.f);

	}
	if (sprite && bFilterEnable && FilterLayer.size() > 0)FilterImage();
}

SpriteRenderer::~SpriteRenderer()
{
  //LayerInterface::~LayerInterface(); ImageInterface::~ImageInterface(); 
  if (animatorAttached)animatorAttached->rendererAttached = nullptr; 
}

void SpriteRenderer::BeginPlay()
{
	SceneComponent::BeginPlay();

	//控制更新振幅的速度
	WaveUpdate.Bind(0.3, [this]() {
		if (!wave)return;
		if (bIsWave)
	{
		ImageToolkit::calcnextframewave(sprite, wave);
		ImageToolkit::calcimage(sprite, wave);
	}
	
		}
	);
}

/**
 * 渲染图片到句柄上.
 * @brief 核心函数
 */
void SpriteRenderer::Render()
{
	//debug 2024.9.20 不进行判定sprite为空会出错，一旦sprite还没开始渲染
	if (sprite)
	{
		
		if(!bmove)
		{
			bmove = true;
			//parent->AddPosition(Vector2D(sprite->getwidth(), sprite->getheight()));
		if(parent)parent->AddPosition(Vector2D(spriteinfo.EndLoc.x/2.0, spriteinfo.EndLoc.y/2.0));			//设置中心坐标
		std::cout << GetWorldPosition() << std::endl;
		}
	
		/*std::cout << mainWorld.mainCamera-> GetWorldPosition() << std::endl;*/
		
		//debug 2024.9.23 物体位置没有随缩放大小进行偏移 导致绕着物体自己的中心点进行缩放 修改后会随着相机位置进行偏移
		Vector2D pos = (GetWorldPosition() - mainWorld.mainCamera->GetCameraPosition())
			* (20.f / mainWorld.mainCamera->springArmLength_virtual) + Vector2D(getwidth() / 2, getheight() / 2);		//获取当前位置 相机往右移动，图片往左移动相同位置
		
		//判断是否有对图形进行旋转操作
		IMAGE* img = copy ? copy:sprite;

		HDC dstDC = GetImageHDC();				//工作平面的图片句柄
		HDC srcDC = GetImageHDC(img);			//要传递的图片句柄
		int w = img->getwidth();			//IMAGE类型自带获取对象的宽度
		int h = img->getheight();		//IMAGE类型自带获取对象的高度

		//实现全局缩放和局部缩放
		//跟物体规模成正比实现局部缩放
		//跟弹簧长度成反比实现全局缩放
		//物体实际大小
		int dw = int(w * GetWorldScale().x * 20.f / mainWorld.mainCamera->springArmLength_virtual);
		int dh = int(h * GetWorldScale().y * 20.f / mainWorld.mainCamera->springArmLength_virtual);
		/*std::cout << (int)pos.x  << std::endl;*/
		BLENDFUNCTION bf = { AC_SRC_OVER,0,alpha,AC_SRC_ALPHA };				//设置透明度，成员全是BYTE类型
		//是否添加了滤镜
		if (bFilterEnable && filter&&FilterLayer.size()>0)
		{
			HDC srcDC_filter = GetImageHDC(filter);
			AlphaBlend(dstDC, (int)pos.x - dw / 2, (int)pos.y - dh / 2, dw, dh, srcDC_filter, 0, 0, w, h, bf);			//相当于putimage 粘贴到相应的工作平面

		}
		else if(IsWave()&&wave)
		{
			HDC srcDC_wave = GetImageHDC(wave);
			AlphaBlend(dstDC, (int)pos.x - dw / 2, (int)pos.y - dh / 2, dw, dh, srcDC_wave, 0, 0, w, h, bf);
		}
		else
		AlphaBlend(dstDC, (int)pos.x-dw/2, (int)pos.y-dh/2, dw, dh, srcDC, 0, 0, w, h, bf);			//相当于putimage 粘贴到相应的工作平面
	}
}

///执行支持的旋转滤镜等功能
void SpriteRenderer::Update(float deltaTime)
{
	SceneComponent::Update(deltaTime);
	if (!sprite )return;

	if (copy)spriteinfo.size = Pair(copy->getwidth(), copy->getheight());
	else spriteinfo.size = Pair(sprite->getwidth(), sprite->getheight());

	if (GetWorldRotation() != angle)
	{
		angle = GetWorldRotation();
		RotateImage(angle * PI / 180.f);
		if ( bFilterEnable&&FilterLayer.size()>0)FilterImage();
	}
	/*if (!wave)return;
	if (bIsWave)
	{
		ImageToolkit::calcnextframewave(sprite, wave);
		ImageToolkit::calcimage(sprite, wave);
	}*/
	

}

///@remark图像路径:"SaveGame/ProcessedImage/" 
/// @remark 图像名: GetName() + ".png"
void SpriteRenderer::SaveImage()
{
	std::string path = "SaveGame/ProcessedImage/" + GetName() + ".png";

	FileManager::CreateFolder("SaveGame");
	FileManager::CreateFolder("SaveGame/ProcessedImage");

	saveimage(path.c_str(), sprite);
}

void SpriteRenderer::SaveImage(IMAGE* img)
{
	if (!img)return;
	std::string path = "SaveGame/ProcessedImage/" + GetName() + ".png";

	FileManager::CreateFolder("SaveGame");
	FileManager::CreateFolder("SaveGame/ProcessedImage");

	saveimage(path.c_str(), img);
}