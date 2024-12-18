#include "VisualInterface.h"
#include"CoreMinimal.h"
#include"ResourceManager.h"
#include"Math.h"
#include "FileManager.h"
#include"World.h"


///����ͼ��˳��ӿ�
LayerInterface::LayerInterface()
{
	mainWorld.GameRenderers.insert(this);
}

///����ͼ��˳��ӿڣ���worldͼ��˳��������ɾȥ�����
LayerInterface::~LayerInterface()
{
	mainWorld.GameRenderers.erase(this);
}

///����ͼ��㼶
/// Ҫʵ�ֱ����Ȱ��ź����������ɾ���Ѿ��ź���Ķ������²����Ѿ��ı�õĶ���
/// @remark�����ڹ����BeginPlay������ʹ�ã���ֹ�󶨵�����ӳ��
void LayerInterface::SetLayer(
	int _layer			///<[in]ͼ��㼶
)
{
	mainWorld.GameRenderers.erase(this);
	layer = _layer;
	mainWorld.GameRenderers.insert(this);
}

///��ȡͼ��㼶
/// @return ͼ��㼶
int LayerInterface::GetLayer() const
{
	return layer;
}

///��תͼƬ
///sprite����������
void ImageInterface::RotateImage(double radian)
{
	if (!sprite)return;
	
	#define min(a,b) ((a)<(b)?(a):(b))
	#define max(a,b) ((a)>(b)?(a):(b))
		float fSin = float(sin(radian));
		float fCos = float(cos(radian));
		int w = spriteinfo.EndLoc.x; int h = spriteinfo.EndLoc.y;
		POINT point[4] = { {0,0},{w,0} ,{0,h} ,{w,h} };
		//����ԭ����ת
		int max_x = 0; int min_x = 0;
		int min_y = 0, max_y = 0;
		for (int i = 0; i < 4; i++)
		{
			//��ʱ����ת��ĵ�
			point[i] = {
				int(fCos * point[i].x - fSin * point[i].y)
				,int(fSin * point[i].x + fCos * point[i].y)
			};

			//��ת��ͼ��Ĵ�С
			min_x = min(min_x, point[i].x);
			max_x = max(max_x, point[i].x);
			min_y = min(min_y, point[i].y);
			max_y = max(max_y, point[i].y);
		}

		//������ת��ͼ��Ŀ��
		int nw = max_x - min_x;
		int nh = max_y - min_y;
		int nSize = nw * nh;

		//û�����ʹ����ռ䣬�����˾͸ı��С�Զ������ڴ�
		if (!copy)copy = new IMAGE(nw, nh);
		copy->Resize(nw, nh);
		spriteinfo.EndLoc = { nw, nh };

		DWORD* pNewBuf = GetImageBuffer(copy);
		DWORD* pBuf = GetImageBuffer(sprite);

		////��ʼԭͼ�����ͼ��ӳ��
		//for (int i = 0; i < h; i++)
		//{
		//	for (int j = 0; j < w; j++)
		//	{
		//		//��[min_x,min_y]��Ϊ[0,0]�����ƫ��
		//		int nx = int(fCos * j - fSin * i - min_x);
		//		int ny = int(fSin * j + fCos * i - min_y);
		//		if (nx * nw + ny >= 0 && nx * nw + ny < nSize)
		//			pNewBuf[nx * nw + ny] = pBuf[i * w + j];
		//	}
		//}

		//��ʼ��ͼ���ԭͼ���ӳ��
		for (int i=min_x,ni = 0; ni < nh; ++ni,++i)
		{
			for (int j=min_y,nj = 0; nj < nw; ++nj,++j)
			{
				pNewBuf[ni * nw + nj] = 0;		//��ͼƬ�������ȳ�ʼ��

				//˳ʱ��
				int nx = int(fCos * j - fSin * i );			//��ת֮��x��λ��
				int ny = int(fSin * j + fCos * i );			//��ת֮��y��λ��

				/*std::cout << '(' << nx << ',' << ny << std::endl;*/
				if (nx >= 0 && nx < w && ny >= 0 && ny < h)			//��Ŀ��ӳ����ԭ�з�Χ���򿽱�ɫֵ
				{
					pNewBuf[ni * nw + nj] = pBuf[ny * w + nx];
					/*std::cout << '(' << nx << ',' << ny << std::endl;*/
				}
			}
		}

	
}

///��ͼƬʩ���˾�
///�˾�ʵ��
void ImageInterface::FilterImage()
{
	IMAGE* img = copy ? copy : sprite;
	if (!img)return;
	if (!filter)filter = new IMAGE(img->getwidth(), img->getheight());
	filter->Resize(img->getwidth(), img->getheight());

	const DWORD *pBuf = GetImageBuffer(img);
	DWORD* pNewBuf = GetImageBuffer(filter);
	int nSize = img->getwidth() * img->getheight();		//ͼƬ���ظ���

	
	for(int i = 0; i < nSize; i++)
	{
		if (pBuf[i] >> 24)
		{
			//��ȡ��������RGB ��getimagebuffer��ȡ������ָ��ÿ���ֽ�˳����ARGB
			int pNewBufB = pBuf[i] & 0xFF;			//ȡ�����λ ��Ŀ�� 0ȫ��0 1���� 64λint��4���ֽ�
			int pNewBufG = (pBuf[i] & 0xFF00) >> 8;		//����8λ
			int pNewBufR = (pBuf[i] & 0xFF0000) >> 16;

			for (auto& filterInfo : FilterLayer)
			{
				if (filterInfo.enable)
				{
					int level = filterInfo.level;
					if ((pBuf[i] >> 24 )< 250)level = (pBuf[i] >> 24) * level >> 8;		//ʹ��͸���ȴӵ�͸���ȵ���͸����ƽ������
					//����ɫֵ����Ȩֵ����
					pNewBufB = (pNewBufB * (128 - filterInfo.level) + GetBValue(filterInfo.color) * filterInfo.level) >> 7;	//getbvalue�Ƕ���ɫ����ȡֵ
					pNewBufG = (pNewBufG * (128 - filterInfo.level) + GetGValue(filterInfo.color) * filterInfo.level) >> 7;
					pNewBufR = (pNewBufR * (128 - filterInfo.level) + GetRValue(filterInfo.color) * filterInfo.level) >> 7;
				}
			}

			//����͸�����˾�
			int pNewBufA = (pBuf[i] & 0xFF000000) >> 24;
			if (pNewBufA < 100)
			{
				pNewBufA = 0;
			}

			//������ϳ�һ���µ���ɫֵ��BGR)
			/*pNewBuf[i] = (pBuf[i] & 0xFF000000) | (pNewBufR << 16) | (pNewBufG << 8) | (pNewBufB);*/
			pNewBuf[i] = (pNewBufA << 24) | (pNewBufR << 16) | (pNewBufG << 8) | (pNewBufB);
			/*std::cout << 1;*/
		}
		else
			pNewBuf[i] = 0;
	}
}

bool ImageInterface::IsWave()
{
	return bIsWave;
}

///���õ�һ���˾�
void ImageInterface::SetFilter(bool enable, COLORREF col, int level)
{
	FilterLayer.clear();

	level = Math::Clamp(level, 0, 100);
	FilterLayer.insert({ col, level,enable });
	if (enable)FilterImage();
}

//����˾�
void ImageInterface::AddFilter(FilterInfo filterInfo)
{
	filterInfo.layer = Math::Clamp(filterInfo.layer, 1, 5);
	FilterLayer.erase({ filterInfo.color,10,true,filterInfo.layer });
	
	FilterLayer.insert(filterInfo);
	if(filterInfo.enable) FilterImage();
}

//����˾�
void ImageInterface::AddFilter(bool enable, COLORREF col, int level,int layer)
{
	layer = Math::Clamp(layer, 1, 4);
	FilterLayer.erase({ col,10,true,layer });
	FilterLayer.insert({ col, level,enable,layer });
	if (enable)FilterImage();
}

//�Ƴ�����˾�
void ImageInterface::RemoveFilter()
{
	//��Ϊ����ͼ����������ֻ�ܴ���һ��layer��ͬ���˾���ֻ��Ѱ��layer������Ҫɾȥ����ͬ��Ԫ�أ�����ֵ����ν��ʲô
	FilterLayer.erase({ BLACK, 50,true, 5 });
}

///�Ƴ���������˾� 0~4
void ImageInterface::RemoveFilter(int layer)
{
	layer = Math::Clamp(layer, 0, 4);
	FilterLayer.erase({ BLACK, 50,true, layer });
}

void ImageInterface::MeanBlur(unsigned level)
{
	if (!sprite || blur || level <= 0)return;
	blur = new IMAGE(sprite->getwidth(), sprite->getheight());
	ImageToolkit::MeanFilter(sprite, blur, level);
	sprite = blur;
}

void ImageInterface::WaveFrom(int x, int y, int stonesize, int stoneweight)
{
	if (!sprite || wave )return;
	bIsWave = true;
	wave = new IMAGE(sprite->getwidth(), sprite->getheight());
	ImageToolkit::putstone(sprite, wave, x, y, stonesize, stoneweight);
	//sprite = wave;
}

//���ؾ�̬ͼƬ
void ImageInterface::LoadSprite(std::string name)
{
	sprite = mainWorld.resourcePool->Fetch(name);
	if (sprite)spriteinfo.EndLoc = { (sprite->getwidth()),(sprite->getheight()) };
}

void ImageInterface::SetStartAndEndLoc(Pair start, Pair end)
{
	start.x = Math::Clamp(start.x, 0, spriteinfo.size.x);
	start.y = Math::Clamp(start.y, 0, spriteinfo.size.y);
	end.x = Math::Clamp(end.x, 0, spriteinfo.size.x);
	end.y = Math::Clamp(end.y, 0, spriteinfo.size.y);
	spriteinfo.StartLoc = start;
	spriteinfo.EndLoc = end;

}



//bool ImageToolkit::bIsGaussianFilterOn = false;
//int ImageToolkit::GaussianFilterLevel = 2;

std::vector<short> ImageToolkit::wave1 = { 0 };

std::vector<short> ImageToolkit::wave2 = {0};



DWORD ImageToolkit::GetPixel(IMAGE* img, int i, int j)
{
	DWORD* pBuf = GetImageBuffer(img);
	j = Math::Clamp(j, 0, img->getwidth() - 1);
	i = Math::Clamp(i, 0, img->getheight() - 1);
	return pBuf[i * img->getwidth() + j];
}

void ImageToolkit::GetScreenShot()
{
	static int i = 0;
	std::string path = "SaveGame/ScreenShot/" + GetRealTime() + "_" + std::to_string(i++) + ".png";

	FileManager::CreateFolder("SaveGame");
	FileManager::CreateFolder("SaveGame/ScreenShot");
	IMAGE buf;
	getimage(&buf, 0, 0, WIN_WIDTH, WIN_HEIGHT);
	saveimage(path.c_str(), &buf);
}

void ImageToolkit::putstone(IMAGE* srcImg, IMAGE* dstImg, int x, int y, int stonesize, int stoneweight)
{
	/*std::vector<short>wave1;
	std::vector<short>wave2;*/
	

	const DWORD* pBuf = GetImageBuffer(srcImg);
	DWORD* pNewBuf = GetImageBuffer(dstImg);
	int width = srcImg->getwidth(), height = srcImg->getheight();
	
	wave1.resize(width * height);
	wave2.resize(width * height);

	x = Math::Clamp(x, width - stonesize, stonesize);
	y = Math::Clamp(y, height - stonesize, stonesize);
	/*if (x >= width - stonesize || x < stonesize || y >= height - stonesize || y < stonesize)
		return;*/

	auto GetIndex = [=](int i, int j)->int
		{
			j = Math::Clamp(j, 0, width - 1);
			i = Math::Clamp(i, 0, height - 1);
			return i * width + j;
		};

	for (int posx = x - stonesize; posx < x + stonesize; posx++)
	{
		for (int posy = y - stonesize; posy < y + stonesize; posy++)
		{
			if ((x - posx) * (x - posx) + (y - posy) * (y - posy) < stonesize * stonesize) {
				wave1[GetIndex(posy,posx)] += stoneweight;
			}

		}
	}
	calcnextframewave(srcImg, dstImg);
	calcimage(srcImg, dstImg);

}

//bIsHorizontal����ture��ˮƽ��ת
void ImageToolkit::FlipImage(IMAGE* srcImg, IMAGE* dstImg, bool bIsHorizontal)
{
	if (!srcImg || !dstImg)return;

	const DWORD* pBuf = GetImageBuffer(srcImg);
	DWORD* pNewBuf = GetImageBuffer(dstImg);
	int width = srcImg->getwidth(), height = srcImg->getheight();

	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			pNewBuf[bIsHorizontal ? ((i + 1) * width - j - 1) : ((height - i - 1) * width + j)] = pBuf[i * width + j];
}

//��ֵ�˲�
void ImageToolkit::MeanFilter(IMAGE* srcImg, IMAGE* dstImg, int radius)
{
	if (!srcImg || !dstImg || radius == 0)return;

	const DWORD* pBuf = GetImageBuffer(srcImg);
	DWORD* pNewBuf = GetImageBuffer(dstImg);
	int width = srcImg->getwidth(), height = srcImg->getheight();

	auto GetIndex = [=](int i, int j)->int
		{
			j = Math::Clamp(j, 0, width - 1);
			i = Math::Clamp(i, 0, height - 1);
			return i * width + j;
		};

	struct rgb {
		int r, g, b;
		rgb operator+(const rgb& another) { return { r + another.r,g + another.g,b + another.b }; }
		rgb operator-(const rgb& another) { return { r - another.r,g - another.g,b - another.b }; }
	};

	/* ����ͼ���� */
	std::vector<std::vector<rgb>>integralImage;

	integralImage.resize(height + 1, std::vector<rgb>(width + 1));

	for (int i = 1; i <= height; i++) {
		for (int j = 1; j <= width; j++) {
			int color = pBuf[GetIndex(i - 1, j - 1)];
			integralImage[i][j] = integralImage[i - 1][j] + integralImage[i][j - 1] - integralImage[i - 1][j - 1]
				+ rgb{ (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF };
		}
	}

	/* ��ֵ�˲� */
	int w = radius;

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int x = i + 1, y = j + 1;
			int x1 = max(x - w - 1, 1), y1 = max(y - w - 1, 1);
			int x2 = min(x + w, height), y2 = min(y + w, width);
			rgb center = integralImage[x2][y2] + integralImage[x1][y1] - integralImage[x2][y1] - integralImage[x1][y2];
			int validArea = (x2 - x1) * (y2 - y1);
			int r = Math::Clamp(center.r / validArea, 0, 255), g = Math::Clamp(center.g / validArea, 0, 255), b = Math::Clamp(center.b / validArea, 0, 255);

			pNewBuf[GetIndex(i, j)] = (pBuf[GetIndex(i, j)] & 0xFF000000) | (r << 16) | (g << 8) | b;
		}
	}
}

void ImageToolkit::calcnextframewave(IMAGE* srcImg, IMAGE* dstImg)
{
	int width = srcImg->getwidth(), height = srcImg->getheight();
	auto GetIndex = [=](int i, int j)->int
		{
			j = Math::Clamp(j, 0, width - 1);
			i = Math::Clamp(i, 0, height - 1);
			return i * width + j;
		};
	for (int i = 1; i < WIN_HEIGHT - 1; i++)
	{
		for (int j = 1; j < WIN_WIDTH - 1; j++)
		{
			wave1[GetIndex(i, j)] = (wave1[GetIndex(i, j-1)] + wave1[GetIndex(i, j+1)] + wave1[GetIndex(i-1, j)] + wave1[GetIndex(i+1, j)]) / 2 - wave1[GetIndex(i, j)];
			wave2[GetIndex(i, j)] = wave1[GetIndex(i, j)] >> 5;
		}
	}
	std::vector<short> tmp = wave1;
	wave1 = wave2;
	wave2 = tmp;
}

void ImageToolkit::calcimage(IMAGE* srcImg, IMAGE* dstImg)
{
	DWORD* p1 = GetImageBuffer(srcImg);
	DWORD* p2 = GetImageBuffer(dstImg);

	int width = srcImg->getwidth(), height = srcImg->getheight();
	auto GetIndex = [=](int i, int j)->int
		{
			j = Math::Clamp(j, 0, width - 1);
			i = Math::Clamp(i, 0, height - 1);
			return i * width + j;
		};
	int i = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int a = (x - 0.5 * width) * (1 - wave1[i] / 1024.0) + 0.5 * width;
			int b = (y - 0.5 * height) * (1 - wave1[i] / 1024.0) + 0.5 * height;

			if (a >= width)
				a = width - 1;
			if (a < 0)
				a = 0;
			if (b >= height)
				b = height - 1;
			if (b < 0)
				b = 0;
			p2[i] = p1[GetIndex(b, a)]; 
			i++;
		}
	}
}

// ��������
void ImageToolkit::drawnoise(Perlin p1)
{
	int color;
	bool odortd = false;
	int colordeep = 100;

	clearcliprgn;
	
	double tx = 0;
	double ty = 0;
	//16*16�ľ�����
	for (int i = 0; i < WIN_WIDTH; i++)
	{

		for (int j = 0; j < WIN_HEIGHT; j++)
		{
			color = p1.perlin(tx, ty, 0) * colordeep;
			/*std::cout << p1.perlin(i / microxy, j / microxy, 0)<<std::endl;*/
			putpixel(i, j, RGB(color, color, color));
			//ty = ((j + 1.0) / 500.0) * 16.0;		//�൱��ӳ��0~499��0~16
			ty = Math::mapping(j, 0, 499, 0, 16);
		}
		/*tx = ((i + 1.0) / 500.0) * 16.0;*/
		tx = Math::mapping(i, 0, 499, 0, 16);
		ty = 0;
	}
}


