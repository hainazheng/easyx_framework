/*****************************************************************//**
 * \file   VisualInterface.h
 * \brief  ����һ����2Dͼ��������Ⱦ�йصĽӿ�
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

///ʵ��ͼ��˳��ӿ�  
/// @remark��Ҫʵ��ͼ��˳��ֻҪ�̳������ ͬһ�㼶��˳���ڲ�ʵ��
/// ʵ�ַ���:�Զ���set���������򷽷�
class LayerInterface
{
	int layer = 0;				///<ͼ��㼶 Ĭ��ͬһ�㼶
public:
	LayerInterface();
	~LayerInterface();
	void SetLayer(int _layer);	
	int GetLayer()const;

	///���麯�����̳е��඼������ʵ����Ⱦ��worldĬ�ϱ�������ʵ����Ⱦ
	/// @see  SpriteRenderer::Render()
	virtual void Render() = 0;					
};

///�˾���Ϣ
struct FilterInfo
{
	COLORREF color = BLACK;			///<�˾���ɫ
	int level = 5;					///<��ɫ���˲㼶1~100   Ȩ��
	bool enable=true;				///<�Ƿ�����һ���˾�
	int layer = 0;					///<�˾��㼶0~5
	
};

/**
 * �˾�ͼ���������.
 */
struct FilterSort
{
	bool operator()(const FilterInfo& a,const FilterInfo& b)const
	{
		return a.layer < b.layer;
	}
};

/**
 * ͼ�νӿ�.
 */
class ImageInterface
{
protected:			//�̳ж�����Է���
	IMAGE* sprite = nullptr;			///<�洢ָ���ʡ�ռ� ����ָ�����ǵ�ǰ��Ҫ�Ĵ���Դ����ҵ��ĵ�ַ

	///��Ⱦ������Ϣ
	struct SpriteInfo
	{
		Pair StartLoc;					///<ͼƬ�����Ͻ�����
		Pair EndLoc ;					///<ͼƬ�����½ǵ�����
		Pair offset = { 0,0 };			///<ƫ����
		Pair size = Pair(0, 0);			///<ͼƬ�Ĵ�С
	}spriteinfo;
	BYTE alpha = 255;					///<����͸����

	//��ת��Ϣ
	float angle = 0;							///<��ʱ����ת�Ƕ�
	IMAGE* copy = nullptr;						///<��תͼ����
	void RotateImage(
		double radian							///<[in]�Ƕ�
	);			///<��ת��Ƭ

	//�˾���Ϣ
	IMAGE* filter = nullptr;							///<�˾�ͼ����
	bool bFilterEnable = false;							///<�˾�����
	std::set<FilterInfo, FilterSort> FilterLayer;		///<�˾��㼶���� ����ָ���㼶�������˾� 
	void FilterImage();									

	IMAGE* blur{};			///<�˲�֮��ľ��

	//������Ϣ
	IMAGE* wave{};			///<�������
	bool bIsWave=false;		///<�Ƿ�������
public:
	///�������� 
	/// �ͷ����п��ٵ�ͼ������Դ
	~ImageInterface() { if(copy)delete copy; if (filter)delete filter; if (blur)delete blur; if (wave)delete wave;
	};

	///��ȡ͸����
	BYTE GetTransparency()const { return alpha; }			
	///����͸����
	void SetTransparency(
		BYTE transparency			///[in]͸����
	) { alpha = transparency; }			

	///�Ƿ��Ѿ������˲���
	bool IsWave();

	///���������˾�
	/// �����֮ǰ�����˾�
	///@remark	��������ײ�ֻ�����������(��һ������˾���
	void SetFilter(
		bool enable					///<[in]��һ�˾��Ƿ���
		, COLORREF col = BLACK		///<[in]��ɫ
		, int level = 100			///<[in]��ɫ���˲㼶
	);

	///����˾�
	///@remark	���������������������� ���Ǿ�����ʹ��
	void AddFilter(
		FilterInfo filterInfo		///<[in]�˾���Ϣ
	);

	///���˾������������˾� �����д�㼶�ͻ��滻��һ��
	void AddFilter(
		bool enable=true			///<[in]��һ�˾��Ƿ���
		, COLORREF col = BLACK		///<[in]��ɫ
		, int level = 100			///<[in]��ɫ���˲㼶
		,int layer=1				///<[in]�˾��㼶1~4
	);

	///�Ƴ��㼶5�˾�
	void RemoveFilter();

	///�Ƴ��˾�
	void RemoveFilter(
		int layer		///<[in]�˾��㼶0~4
	);

	///���������˾�
	void EnableFilter() { bFilterEnable=bFilterEnable?false:true; }

	///����ͼ���˾� 
	/// @remark ֻ�ܿ���0~4��
	void EnableFilLayer(
		int layer	///<[in]�˾��㼶0~4
	) 
	{
		
		if (FilterLayer.find({ 0,0,0,layer }) != FilterLayer.end())
		{
			/*std::cout << 1 << std::endl;*/
			auto& fil = *FilterLayer.find({ 0,0,0,layer });
			bool enable = fil.enable ? false : true;
			/*AddFilter(enable, fil.color, fil.level, layer);*/			//����1~4��
			layer = Math::Clamp(layer, 0, 4);
			AddFilter({ fil.color, fil.level,enable, layer});	//����0~4ͼ��
			if (enable)FilterImage();
		}
		 
	}

	///�رղ���
	void ExitWave() {
		if (!bIsWave)return;
		delete wave;
		bIsWave = false;
	}

	/**
	 * @brief ��ֵģ����������Gameplay��ʼǰԤ����ʹ�ã�
	 * @param[in] level			    ģ���̶ȣ����С������������Ӱ��
	 **/
	void MeanBlur(unsigned level);

	/**
	 * @brief ����
	 * @brief �൱��Ͷһ��ʯͷ���������
	 * @param[in] x,y				  λ��
	 * @param[in] stonesize		    ʯͷ��С
	 * @param[in] stoneweight		ʯͷ����
	 **/
	void WaveFrom(int x, int y, int stonesize, int stoneweight);

	/**
	 * ���ؾ�̬ͼ��.
	 * 
	 * \param [in] name ͼ������
	 */
	void LoadSprite(std::string name);

	/**
	 * ��ȡ�Ƕ�����Դָ������.
	 * \param [in] start		���Ͻ�����
	 * \param [in] end		���½�����
	 */
	void SetStartAndEndLoc(Pair start, Pair end);
};

/**
 * ͼ������.
 */
class ImageToolkit
{
	friend class World;
	friend class GameplayStatics;

public:
	/**
	 * ��ȡ��ά���괦����ֵ.
	 * 
	 * \param [in] img ͼƬ���
	 * \param [in] i	����������i
	 * \param [in] j	����������j
	 * \return  ��ȡ����[i][j]����
	 */
	static DWORD GetPixel(IMAGE* img, int i, int j);

	/**
	 * ��ȡ��ǰ��Ϸ��ͼ����ֹ���lʹ�ã���.
	 * 
	 */
	static void GetScreenShot();

	

	/**
	 * ����ģ��.
	 * @see ImageInterface::WaveFrom()
	 * @see ImageInterface::ExitWave()
	 * \param [in] srcImg		   ԭͼ����
	 * \param [in] dstImg		  Ŀ��ͼ�ξ��
	 * \param [in] x��y				  λ��
	 * \param [in] stonesize	    ʯͷ��С
	 * \param [in] stoneweight		ʯͷ����
	 */
	static void putstone(IMAGE* srcImg, IMAGE* dstImg,int x, int y, int stonesize, int stoneweight);

	/**
	 * ������һ֡�����.
	 * @see ImageInterface::WaveFrom()
	 * @see ImageInterface::ExitWave()
	 * \param [in] srcImg		   ԭͼ����
	 * \param [in] dstImg		  Ŀ��ͼ�ξ��
	 */
	static void calcnextframewave(IMAGE* srcImg, IMAGE* dstImg);

	/**
	 * ��wave1�������µ�ͼ��.
	 * @see ImageInterface::WaveFrom()
	 * @see ImageInterface::ExitWave()
	 * \param [in] srcImg	   ԭͼ����
	 * \param [in]  dstImg	  Ŀ��ͼ�ξ��
	 */
	static void calcimage(IMAGE* srcImg, IMAGE* dstImg);

	/**
	 * ����perlin���� ���Ľϴ�.
	 * @see Perlin::perlin()
	 * \param [in] p1		�������������
	 */
	static void drawnoise(Perlin p1);

	

	/** �任 **/

	/**
	 * ����ת�任.
	 * 
	 * \param [in] srcImg	   ԭͼ����
	 * \param [in] dstImg	  Ŀ��ͼ�ξ��
	 * \param [in] bIsHorizontal �Ƿ�ˮƽ��ת Ĭ��ˮƽ
	 */
	static void FlipImage(IMAGE* srcImg, IMAGE* dstImg, bool bIsHorizontal = true);


	/** �˲� **/

	/**
	 * ���پ�ֵ�˲�(ģ��).
	 * ĳһ��rgb�ܰ뾶������������rgbӰ�� 
	 * ��������r���ܺ�/�뾶�����ص�����
	 * @remark radiusԽ��ģ���̶�Խ��
	 * @see  ImageInterface::MeanBlur()
	 * \param [in] srcImg	   ԭͼ����
	 * \param [in] dstImg 	  Ŀ��ͼ�ξ��
	 * \param [in] radius	  �ڲ������˴�С
	 */
	static void MeanFilter(IMAGE* srcImg, IMAGE* dstImg, int radius);

	////��˹�˲�(ģ���ҿ���ݡ�����Ϊ�ĕr������)
	//static void GaussianFilter(IMAGE* srcImg, IMAGE* dstImg, int radius);

private:
	////��������Ļʩ�Ӹ�˹�˲�
	//static void ApplyGaussianFilterOnScreen();

	//static bool bIsGaussianFilterOn;

	//static int GaussianFilterLevel;

	

	static std::vector<short>wave1;		///<��ǰʱ�̵��������
	static std::vector<short>wave2;		///<��һʱ�̵��������

	
};


