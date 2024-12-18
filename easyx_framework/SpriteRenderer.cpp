#include "SpriteRenderer.h"
#include"Windows.h"
#pragma comment(lib,"Msimg32.lib")
#include"Camera.h"
#include"Animator.h"
#include"FileManager.h"



///�����˾�����ת
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

	//���Ƹ���������ٶ�
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
 * ��ȾͼƬ�������.
 * @brief ���ĺ���
 */
void SpriteRenderer::Render()
{
	//debug 2024.9.20 �������ж�spriteΪ�ջ����һ��sprite��û��ʼ��Ⱦ
	if (sprite)
	{
		
		if(!bmove)
		{
			bmove = true;
			//parent->AddPosition(Vector2D(sprite->getwidth(), sprite->getheight()));
		if(parent)parent->AddPosition(Vector2D(spriteinfo.EndLoc.x/2.0, spriteinfo.EndLoc.y/2.0));			//������������
		std::cout << GetWorldPosition() << std::endl;
		}
	
		/*std::cout << mainWorld.mainCamera-> GetWorldPosition() << std::endl;*/
		
		//debug 2024.9.23 ����λ��û�������Ŵ�С����ƫ�� �������������Լ������ĵ�������� �޸ĺ���������λ�ý���ƫ��
		Vector2D pos = (GetWorldPosition() - mainWorld.mainCamera->GetCameraPosition())
			* (20.f / mainWorld.mainCamera->springArmLength_virtual) + Vector2D(getwidth() / 2, getheight() / 2);		//��ȡ��ǰλ�� ��������ƶ���ͼƬ�����ƶ���ͬλ��
		
		//�ж��Ƿ��ж�ͼ�ν�����ת����
		IMAGE* img = copy ? copy:sprite;

		HDC dstDC = GetImageHDC();				//����ƽ���ͼƬ���
		HDC srcDC = GetImageHDC(img);			//Ҫ���ݵ�ͼƬ���
		int w = img->getwidth();			//IMAGE�����Դ���ȡ����Ŀ��
		int h = img->getheight();		//IMAGE�����Դ���ȡ����ĸ߶�

		//ʵ��ȫ�����ź;ֲ�����
		//�������ģ������ʵ�־ֲ�����
		//�����ɳ��ȳɷ���ʵ��ȫ������
		//����ʵ�ʴ�С
		int dw = int(w * GetWorldScale().x * 20.f / mainWorld.mainCamera->springArmLength_virtual);
		int dh = int(h * GetWorldScale().y * 20.f / mainWorld.mainCamera->springArmLength_virtual);
		/*std::cout << (int)pos.x  << std::endl;*/
		BLENDFUNCTION bf = { AC_SRC_OVER,0,alpha,AC_SRC_ALPHA };				//����͸���ȣ���Աȫ��BYTE����
		//�Ƿ�������˾�
		if (bFilterEnable && filter&&FilterLayer.size()>0)
		{
			HDC srcDC_filter = GetImageHDC(filter);
			AlphaBlend(dstDC, (int)pos.x - dw / 2, (int)pos.y - dh / 2, dw, dh, srcDC_filter, 0, 0, w, h, bf);			//�൱��putimage ճ������Ӧ�Ĺ���ƽ��

		}
		else if(IsWave()&&wave)
		{
			HDC srcDC_wave = GetImageHDC(wave);
			AlphaBlend(dstDC, (int)pos.x - dw / 2, (int)pos.y - dh / 2, dw, dh, srcDC_wave, 0, 0, w, h, bf);
		}
		else
		AlphaBlend(dstDC, (int)pos.x-dw/2, (int)pos.y-dh/2, dw, dh, srcDC, 0, 0, w, h, bf);			//�൱��putimage ճ������Ӧ�Ĺ���ƽ��
	}
}

///ִ��֧�ֵ���ת�˾��ȹ���
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

///@remarkͼ��·��:"SaveGame/ProcessedImage/" 
/// @remark ͼ����: GetName() + ".png"
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