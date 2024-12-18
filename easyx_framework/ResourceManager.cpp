#include "ResourceManager.h"

ResourceManager* ResourceManager::manager = nullptr;

std::mutex ResourceManager::m_Mutex;

///加载单张图像资源，放在资源库
void ResourceManager::Load(
	std::string name			///<[in]图像名字
	, std::string path			///<[in]图像资源路径
	, int wid					///<[in]图像宽
	, int hei					///<[in]图像高
)
{
	IMAGE* imageRes = new IMAGE;
	loadimage(imageRes, (LPCTSTR)path.c_str(), wid, hei);
	ImagePool.insert(std::pair<std::string, IMAGE*>(name, imageRes));
}

//加载多张动画图片
void ResourceManager::Load(
	std::string name			///<[in]动画名字
	, std::string path			///<[in]动画路径
	, int wid					///<[in]图像宽
	, int hei					///<[in]图像高
	, int num					///<[in]数量
)
{
	IMAGE** images = new IMAGE*[num];			//不开辟指针数组，结束函数就释放，就会查不到，存放的是地址
	for (int i = 0; i < num; i++)
	{
		images[i] = new IMAGE;
		loadimage(images[i], LPCTSTR((path+'/'+std::to_string(i)+".png").c_str()), wid, hei);
	}
	animationPool.insert({ name,{images,num} });

}

//加载Springsheet动画图片
void ResourceManager::Load(
	std::string name		///<[in]动画名字
	, std::string path		///<[in]动画路径
	, int wid				///<[in]图像宽
	, int hei				///<[in]图像高
	, int num				///<[in]总共数量
	, int row				///<[in]最大多少行
	, int col				///<[in]最大多少列
)
{
	IMAGE** images = new IMAGE * [num];
	IMAGE img;
	loadimage(&img, (LPCTSTR)path.c_str(), wid, hei);
	DWORD* src = GetImageBuffer(&img);

	int width = wid / col;		//获取单场图片的宽度
	int height = hei / row;		//获取单张图片的高度
//#define Elem(array,_row,_col)  (array[(_row)*WIN_WIDTH+(_col)])
	for (int i=0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			//当前要处理图片的坐标
			int x = j * width;
			int y = i * height;

			int index = i * col + j;			//索引拷贝到的图片下标
			if (index >= num)break;		//防止出错，给入的信息出错
			images[index] = new IMAGE(width,height);		//开辟拷贝图像空间
			DWORD* dst = GetImageBuffer(images[index]);		//获取要拷贝图像的首个像素地址

			//获取原图像滤镜
			
			//进行拷贝
			for (int x1 = 0; x1 < width; x1++)
			{
				for (int y1 = 0; y1 < height; y1++)
				{
					int pNewBufA = (src[x + x1 + (y + y1) * wid] & 0xFF000000) >> 24;
				if (pNewBufA < 50)
				{
					pNewBufA = 0;
				}
				/*if (pNewBufA < 100&&pNewBufA>60)
				{
					pNewBufA =20;
				}*/

				dst[x1 + y1 * width] = (pNewBufA << 24) | (src[x + x1 + (y + y1) * wid] & 0xFFFFFF);
				}
			}
		}
	}
	animationPool.insert({ name,{images,num} });
}


void ResourceManager::Load(
	std::string name					///<[in]动画名字
	, std::string path					///<[in]动画路径
	, const std::vector<Pair>& arr		///<[in]每一张图片大小容器
)
{
	int num = int(arr.size());
	IMAGE** images = new IMAGE * [num];
	for (int i = 0; i < num; i++)
	{
		images[i] = new IMAGE;
		loadimage(images[i], LPCTSTR((path + "/" + std::to_string(i) + ".png").c_str()), arr[i].x, arr[i].y);
	}
	animationPool.insert({ name, {images,int(arr.size())} });
}

///初始化资源库
/// 提前获取资源
/// @remark ！！！用户使用图像资源需要在这里加载
void ResourceManager::Initialize()
{
	//加载图像资源
	Load("bg0", "res/images/bg0.jpg", 1400, 600);
	Load("sunshine", "res/animations/bullets/sun.png", 483, 160, 12, 2, 6);
	Load("sunshine1", "res/animations/bullets/sun1.png", 483, 160, 12, 2, 6);
	
	Load("mineparticle", "res/animations/effects/PotatoMine_particles.png", 138, 26, 6, 1, 6);


	//加载植物动画资源
	Load("peashooter", "res/animations/plants/peashooter.png", 483, 320, 24, 4, 6);

	Load("sunflower", "res/animations/plants/sunflower.png", 483, 320, 24, 4, 6);

	//加载子弹资源
	Load("pea", "res/animations/bullets/pea.png", 28, 28);
	
	//加载僵尸资源
	Load("normalwalk", "res/animations/zombies/normal/walk.png", 1000, 695, 46, 5, 10);
	
	Load("normaleat", "res/animations/zombies/normal/eat.png", 1000, 556, 39, 4, 10);

	Load("zombledie", "res/animations/zombies/die.png", 166, 1440, 10, 10, 1);


	//种子银行
	Load("seedbank", "res/images/seedbank.png", 530, 91);


	Load("card1", "res/images/cards/card1.png", 54, 77);
	Load("card2", "res/images/cards/card2.png", 54, 77);
	Load("card3", "res/images/cards/card3.png", 54, 77);
	Load("card4", "res/images/cards/card4.png", 54, 77);
	Load("card5", "res/images/cards/card5.png", 54, 77);
	Load("card6", "res/images/cards/card6.png", 54, 77);
	Load("card7", "res/images/cards/card7.png", 54, 77);
	Load("card8", "res/images/cards/card8.png", 54, 77);
	Load("black", "res/images/cards/black.png", 54, 77);
	Load("gray", "res/images/cards/black.png", 54, 77);

	Load("boss", "res/sounds/boss.mp3");
	Load("secer", "res/secer.wav");
	Load("Awake", "res/Awake.mp3");

	Load("rain_bg", "res/Particles/rain_particle.png", 10, 1000, 3, 3, 1);
	Load("rain_bg_", "res/Particles/rain_particle_.png", 15, 800, 3, 3, 1);
	Load("rain_land", "res/animations/RainLand", 120, 20, 6);
}

ResourceManager* ResourceManager::instance()
{
	if (!manager)
	{
		std::unique_lock<std::mutex> lock(m_Mutex); // 加锁 一创建好对象就释放 耗时操作
		if (!manager)
		{
			manager = new ResourceManager;
		}
	}
	return manager;
}

IMAGE* ResourceManager::Fetch(std::string name)
{
	return (*(ImagePool.find(name))).second;
}

AnimationResource ResourceManager::AnimationFetch(std::string name)
{
	return animationPool.find(name)->second;
}

bool ResourceManager::IsAudio(std::string name)
{
	return (AudioPool.find(name)!=AudioPool.end());
}

///给音频路径一个别名
void ResourceManager::Load(
	std::string name		///<[in]音频名字
	, std::string path		///<[in]音频路径
)
{
	if (AudioPool.find(name) != AudioPool.end())return;
	//给音频别名
	std::string file = std::string("open ") + path + std::string(" alias ") + name;
	mciSendString(file.c_str(), NULL, 0, NULL);
	AudioPool.insert({ name, path});
}
