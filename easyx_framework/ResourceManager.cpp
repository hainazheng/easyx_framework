#include "ResourceManager.h"

ResourceManager* ResourceManager::manager = nullptr;

std::mutex ResourceManager::m_Mutex;

///���ص���ͼ����Դ��������Դ��
void ResourceManager::Load(
	std::string name			///<[in]ͼ������
	, std::string path			///<[in]ͼ����Դ·��
	, int wid					///<[in]ͼ���
	, int hei					///<[in]ͼ���
)
{
	IMAGE* imageRes = new IMAGE;
	loadimage(imageRes, (LPCTSTR)path.c_str(), wid, hei);
	ImagePool.insert(std::pair<std::string, IMAGE*>(name, imageRes));
}

//���ض��Ŷ���ͼƬ
void ResourceManager::Load(
	std::string name			///<[in]��������
	, std::string path			///<[in]����·��
	, int wid					///<[in]ͼ���
	, int hei					///<[in]ͼ���
	, int num					///<[in]����
)
{
	IMAGE** images = new IMAGE*[num];			//������ָ�����飬�����������ͷţ��ͻ�鲻������ŵ��ǵ�ַ
	for (int i = 0; i < num; i++)
	{
		images[i] = new IMAGE;
		loadimage(images[i], LPCTSTR((path+'/'+std::to_string(i)+".png").c_str()), wid, hei);
	}
	animationPool.insert({ name,{images,num} });

}

//����Springsheet����ͼƬ
void ResourceManager::Load(
	std::string name		///<[in]��������
	, std::string path		///<[in]����·��
	, int wid				///<[in]ͼ���
	, int hei				///<[in]ͼ���
	, int num				///<[in]�ܹ�����
	, int row				///<[in]��������
	, int col				///<[in]��������
)
{
	IMAGE** images = new IMAGE * [num];
	IMAGE img;
	loadimage(&img, (LPCTSTR)path.c_str(), wid, hei);
	DWORD* src = GetImageBuffer(&img);

	int width = wid / col;		//��ȡ����ͼƬ�Ŀ��
	int height = hei / row;		//��ȡ����ͼƬ�ĸ߶�
//#define Elem(array,_row,_col)  (array[(_row)*WIN_WIDTH+(_col)])
	for (int i=0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			//��ǰҪ����ͼƬ������
			int x = j * width;
			int y = i * height;

			int index = i * col + j;			//������������ͼƬ�±�
			if (index >= num)break;		//��ֹ�����������Ϣ����
			images[index] = new IMAGE(width,height);		//���ٿ���ͼ��ռ�
			DWORD* dst = GetImageBuffer(images[index]);		//��ȡҪ����ͼ����׸����ص�ַ

			//��ȡԭͼ���˾�
			
			//���п���
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
	std::string name					///<[in]��������
	, std::string path					///<[in]����·��
	, const std::vector<Pair>& arr		///<[in]ÿһ��ͼƬ��С����
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

///��ʼ����Դ��
/// ��ǰ��ȡ��Դ
/// @remark �������û�ʹ��ͼ����Դ��Ҫ���������
void ResourceManager::Initialize()
{
	//����ͼ����Դ
	Load("bg0", "res/images/bg0.jpg", 1400, 600);
	Load("sunshine", "res/animations/bullets/sun.png", 483, 160, 12, 2, 6);
	Load("sunshine1", "res/animations/bullets/sun1.png", 483, 160, 12, 2, 6);
	
	Load("mineparticle", "res/animations/effects/PotatoMine_particles.png", 138, 26, 6, 1, 6);


	//����ֲ�ﶯ����Դ
	Load("peashooter", "res/animations/plants/peashooter.png", 483, 320, 24, 4, 6);

	Load("sunflower", "res/animations/plants/sunflower.png", 483, 320, 24, 4, 6);

	//�����ӵ���Դ
	Load("pea", "res/animations/bullets/pea.png", 28, 28);
	
	//���ؽ�ʬ��Դ
	Load("normalwalk", "res/animations/zombies/normal/walk.png", 1000, 695, 46, 5, 10);
	
	Load("normaleat", "res/animations/zombies/normal/eat.png", 1000, 556, 39, 4, 10);

	Load("zombledie", "res/animations/zombies/die.png", 166, 1440, 10, 10, 1);


	//��������
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
		std::unique_lock<std::mutex> lock(m_Mutex); // ���� һ�����ö�����ͷ� ��ʱ����
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

///����Ƶ·��һ������
void ResourceManager::Load(
	std::string name		///<[in]��Ƶ����
	, std::string path		///<[in]��Ƶ·��
)
{
	if (AudioPool.find(name) != AudioPool.end())return;
	//����Ƶ����
	std::string file = std::string("open ") + path + std::string(" alias ") + name;
	mciSendString(file.c_str(), NULL, 0, NULL);
	AudioPool.insert({ name, path});
}
