/*****************************************************************//**
 * \file   VisualInterface.h
 * \brief  包含一切与2D图像处理与渲染有关的接口
 * 
 * \author zhn
 *********************************************************************/

#pragma once
#include"easyx.h"
#include<string>
#include<set>
#include"iostream"
#include"Math.h"
#include"CoreMinimal.h"
#include"Perlin.h"

///实现图层顺序接口  
/// @remark想要实现图层顺序只要继承这个类 同一层级的顺序看内部实现
/// 实现方法:自定义set容器的排序方法
class LayerInterface
{
	int layer = 0;				///<图层层级 默认同一层级
public:
	LayerInterface();
	~LayerInterface();
	void SetLayer(int _layer);	
	int GetLayer()const;

	///纯虚函数，继承的类都有它来实现渲染，world默认遍历函数实现渲染
	/// @see  SpriteRenderer::Render()
	virtual void Render() = 0;					
};

///滤镜信息
struct FilterInfo
{
	COLORREF color = BLACK;			///<滤镜颜色
	int level = 5;					///<颜色过滤层级1~100   权重
	bool enable=true;				///<是否开启这一层滤镜
	int layer = 0;					///<滤镜层级0~5
	
};

/**
 * 滤镜图层排序规则.
 */
struct FilterSort
{
	bool operator()(const FilterInfo& a,const FilterInfo& b)const
	{
		return a.layer < b.layer;
	}
};

/**
 * 图形接口.
 */
class ImageInterface
{
protected:			//继承对象可以访问
	IMAGE* sprite = nullptr;			///<存储指针节省空间 用来指向我们当前需要的从资源库查找到的地址

	///渲染补充信息
	struct SpriteInfo
	{
		Pair StartLoc;					///<图片的左上角坐标
		Pair EndLoc ;					///<图片的右下角的坐标
		Pair offset = { 0,0 };			///<偏移量
		Pair size = Pair(0, 0);			///<图片的大小
	}spriteinfo;
	BYTE alpha = 255;					///<设置透明度

	//旋转信息
	float angle = 0;							///<逆时针旋转角度
	IMAGE* copy = nullptr;						///<旋转图像句柄
	void RotateImage(
		double radian							///<[in]角度
	);			///<旋转照片

	//滤镜信息
	IMAGE* filter = nullptr;							///<滤镜图像句柄
	bool bFilterEnable = false;							///<滤镜开关
	std::set<FilterInfo, FilterSort> FilterLayer;		///<滤镜层级容器 按照指定层级来排序滤镜 
	void FilterImage();									

	IMAGE* blur{};			///<滤波之后的句柄

	//波幅信息
	IMAGE* wave{};			///<波幅句柄
	bool bIsWave=false;		///<是否开启波幅
public:
	///析构函数 
	/// 释放所有开辟的图像句柄资源
	~ImageInterface() { if(copy)delete copy; if (filter)delete filter; if (blur)delete blur; if (wave)delete wave;
	};

	///获取透明度
	BYTE GetTransparency()const { return alpha; }			
	///设置透明度
	void SetTransparency(
		BYTE transparency			///[in]透明度
	) { alpha = transparency; }			

	///是否已经开启了波幅
	bool IsWave();

	///开启设置滤镜
	/// 会清空之前所有滤镜
	///@remark	！！！最底层只能用这个设置(第一次添加滤镜）
	void SetFilter(
		bool enable					///<[in]这一滤镜是否开启
		, COLORREF col = BLACK		///<[in]颜色
		, int level = 100			///<[in]颜色过滤层级
	);

	///添加滤镜
	///@remark	！！！第五层可以用这个添加 但是尽量不使用
	void AddFilter(
		FilterInfo filterInfo		///<[in]滤镜信息
	);

	///在滤镜上面继续添加滤镜 如果不写层级就会替换第一层
	void AddFilter(
		bool enable=true			///<[in]这一滤镜是否开启
		, COLORREF col = BLACK		///<[in]颜色
		, int level = 100			///<[in]颜色过滤层级
		,int layer=1				///<[in]滤镜层级1~4
	);

	///移除层级5滤镜
	void RemoveFilter();

	///移除滤镜
	void RemoveFilter(
		int layer		///<[in]滤镜层级0~4
	);

	///开关整体滤镜
	void EnableFilter() { bFilterEnable=bFilterEnable?false:true; }

	///开关图层滤镜 
	/// @remark 只能控制0~4层
	void EnableFilLayer(
		int layer	///<[in]滤镜层级0~4
	) 
	{
		
		if (FilterLayer.find({ 0,0,0,layer }) != FilterLayer.end())
		{
			/*std::cout << 1 << std::endl;*/
			auto& fil = *FilterLayer.find({ 0,0,0,layer });
			bool enable = fil.enable ? false : true;
			/*AddFilter(enable, fil.color, fil.level, layer);*/			//控制1~4层
			layer = Math::Clamp(layer, 0, 4);
			AddFilter({ fil.color, fil.level,enable, layer});	//控制0~4图层
			if (enable)FilterImage();
		}
		 
	}

	///关闭波幅
	void ExitWave() {
		if (!bIsWave)return;
		delete wave;
		bIsWave = false;
	}

	/**
	 * @brief 均值模糊（建议在Gameplay开始前预处理使用）
	 * @param[in] level			    模糊程度，其大小对性能无显著影响
	 **/
	void MeanBlur(unsigned level);

	/**
	 * @brief 波纹
	 * @brief 相当于投一块石头产生的振幅
	 * @param[in] x,y				  位置
	 * @param[in] stonesize		    石头大小
	 * @param[in] stoneweight		石头重量
	 **/
	void WaveFrom(int x, int y, int stonesize, int stoneweight);

	/**
	 * 加载静态图像.
	 * 
	 * \param [in] name 图像名字
	 */
	void LoadSprite(std::string name);

	/**
	 * 截取非动画资源指定部分.
	 * \param [in] start		左上角坐标
	 * \param [in] end		右下角坐标
	 */
	void SetStartAndEndLoc(Pair start, Pair end);
};

/**
 * 图像处理类.
 */
class ImageToolkit
{
	friend class World;
	friend class GameplayStatics;

public:
	/**
	 * 获取二维坐标处像素值.
	 * 
	 * \param [in] img 图片句柄
	 * \param [in] i	像素行坐标i
	 * \param [in] j	像素列坐标j
	 * \return  获取到的[i][j]像素
	 */
	static DWORD GetPixel(IMAGE* img, int i, int j);

	/**
	 * 获取当前游戏截图（禁止高頻使用！）.
	 * 
	 */
	static void GetScreenShot();

	

	/**
	 * 波纹模拟.
	 * @see ImageInterface::WaveFrom()
	 * @see ImageInterface::ExitWave()
	 * \param [in] srcImg		   原图像句柄
	 * \param [in] dstImg		  目标图形句柄
	 * \param [in] x，y				  位置
	 * \param [in] stonesize	    石头大小
	 * \param [in] stoneweight		石头重量
	 */
	static void putstone(IMAGE* srcImg, IMAGE* dstImg,int x, int y, int stonesize, int stoneweight);

	/**
	 * 计算下一帧的振幅.
	 * @see ImageInterface::WaveFrom()
	 * @see ImageInterface::ExitWave()
	 * \param [in] srcImg		   原图像句柄
	 * \param [in] dstImg		  目标图形句柄
	 */
	static void calcnextframewave(IMAGE* srcImg, IMAGE* dstImg);

	/**
	 * 用wave1来生成新的图形.
	 * @see ImageInterface::WaveFrom()
	 * @see ImageInterface::ExitWave()
	 * \param [in] srcImg	   原图像句柄
	 * \param [in]  dstImg	  目标图形句柄
	 */
	static void calcimage(IMAGE* srcImg, IMAGE* dstImg);

	/**
	 * 绘制perlin噪音 消耗较大.
	 * @see Perlin::perlin()
	 * \param [in] p1		柏林噪音类对象
	 */
	static void drawnoise(Perlin p1);

	

	/** 变换 **/

	/**
	 * 镜像翻转变换.
	 * 
	 * \param [in] srcImg	   原图像句柄
	 * \param [in] dstImg	  目标图形句柄
	 * \param [in] bIsHorizontal 是否水平翻转 默认水平
	 */
	static void FlipImage(IMAGE* srcImg, IMAGE* dstImg, bool bIsHorizontal = true);


	/** 滤波 **/

	/**
	 * 快速均值滤波(模糊).
	 * 某一点rgb受半径里面所有像素rgb影响 
	 * 例：所有r的总和/半径内像素的数量
	 * @remark radius越大模糊程度越大
	 * @see  ImageInterface::MeanBlur()
	 * \param [in] srcImg	   原图像句柄
	 * \param [in] dstImg 	  目标图形句柄
	 * \param [in] radius	  内部限制了大小
	 */
	static void MeanFilter(IMAGE* srcImg, IMAGE* dstImg, int radius);

	////高斯滤波(模糊且抗锯齿。但较为耗時，慎用)
	//static void GaussianFilter(IMAGE* srcImg, IMAGE* dstImg, int radius);

private:
	////对整个屏幕施加高斯滤波
	//static void ApplyGaussianFilterOnScreen();

	//static bool bIsGaussianFilterOn;

	//static int GaussianFilterLevel;

	

	static std::vector<short>wave1;		///<当前时刻的振幅数据
	static std::vector<short>wave2;		///<上一时刻的振幅数据

	
};


