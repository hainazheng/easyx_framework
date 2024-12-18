#include "Camera.h"
#include"Math.h"
#include"easyx.h"
#include"World.h"


Vector2D Camera::Lerp(Vector2D start, Vector2D end, float t)
{
	t = Math::Clamp(t, 0.001f, 0.1f);
	return start+(end-start)*t;
}

float Camera::Lerp(float start, float end, float t)
{
	t = Math::Clamp(t, 0.001f, 0.1f);
	return start + (end - start) * t;
}

float Camera::SmoothStep(float x)
{
	x = x < 1 ? 1 : x;
	/*return powf(x,(float)Smoothness/100);*/
	return powf(x, 2);
}

///初始化虚拟参数 防止开始产生一定距离偏差进行处理
void Camera::BeginPlay()
{
	SceneComponent::BeginPlay();

	
	camera_virtual.position = GetWorldPosition();
	camera_virtual.scale = GetWorldScale();
	camera_virtual.rotation = GetWorldRotation();
	springArmLength_virtual = springArmLength;
}

//int count = 0;
///实现计算相机坐标
void Camera::Calculate()
{
	if (!bIsEnabled)return;

	Vector2D pos = GetWorldPosition();
	
	if (frame.left != 0 || frame.right != 0 || frame.bottom != 0 || frame.top != 0)
	{
		pos = { Math::Clamp(pos.x - WIN_WIDTH / 2,frame.left ,frame.right) + WIN_WIDTH / 2
		,Math::Clamp(pos.y - WIN_HEIGHT / 2, frame.top,frame.bottom  ) + WIN_HEIGHT / 2 };
	}
	
	//平滑位移
	//平滑度为0就不进行平滑效果处理
	if(Smoothness)
	{
		camera_virtual.position=Lerp(camera_virtual.position, pos,
			0.1f / Smoothness * SmoothStep(Vector2D::Distance(camera_virtual.position, pos) / distanceThreshold));
		//当之间的距离达到一个非常小的数字，相机位置就跟真实位置齐平
		if (Vector2D::Distance(camera_virtual.position, pos) < 0.1)
		{
			camera_virtual.position = pos;
		}
	}
	else camera_virtual.position = pos;
	
	//平滑视野缩放
	if (smoothnessForSpringArm)
	{
		springArmLength_virtual = Lerp(springArmLength_virtual, springArmLength,
			0.1 / smoothnessForSpringArm );
	}
	else springArmLength_virtual = springArmLength;

	/*std::cout << camera_virtual.position << std::endl;*/
	/*std::cout << GetWorldPosition() << std::endl;*/

	//相机抖动
	if (shakeFlag)
	{
		if (GetLocalPosition() == shakeCenter)
		{
			shakeSpeed.x = (float)Math::RandInt(-1, 1);
			shakeSpeed.y = (float)Math::RandInt(-1, 1);
			ShakeIndensity -= (float)shakeDecay/100.0f;
			/*std::cout << GetWorldPosition() << std::endl;*/
			
			if (ShakeIndensity <= 0) {	shakeFlag = false; return;}		//退出震动

		}

		//如果震动中心和相机相对距离是小于震动强度，是在以中心点半径为震动强度的圆内
		if (Vector2D::Distance(GetLocalPosition(), shakeCenter) < ShakeIndensity)		
		{
			/*std::cout << count++ << std::endl;*/
			
				AddPosition(shakeSpeed); camera_virtual.position += shakeSpeed;			//相当于两个位置实现了绑定
				/*std::cout << camera_virtual.position << std::endl;*/
				
		}
		else
		{
			shakeSpeed = -shakeSpeed;
			AddPosition(shakeSpeed);
			camera_virtual.position += shakeSpeed;
		}
	}


	/*std::cout << camera_virtual.position - GetWorldPosition() << std::endl; */
}

void Camera::SetMainCamera()
{

		mainWorld.mainCamera = this;

}

void Camera::SetSpringArmLength(float length)
{
	springArmLength = length;
}

void Camera::SetSmoothness(short smooth)
{
	Smoothness = Math::Clamp(smooth, 0.0, 100.0);
}

void Camera::SetDistanceThreshold(float threshold)
{
	distanceThreshold = Math::Clamp(threshold, 0.0f, 500.0f);
}

void Camera::SetRectFrame(const Rect& frame)
{
	this->frame = frame;
}

void Camera::ShakeCamera(float indensity, int decay)
{
		ShakeIndensity = Math::Clamp(indensity, 0, 100);
		shakeDecay = Math::Clamp(decay, 0, 100);
		if (!shakeFlag)shakeCenter = GetLocalPosition();			//只希望设置一次直到结束为止 震动中心是相对于当前的相机组件位置也就是屏幕位置,组件相对位置自己单独变化
		shakeFlag = true;

}
