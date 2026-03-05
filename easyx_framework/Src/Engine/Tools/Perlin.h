/*****************************************************************//**
 * \file   Perlin.h
 * \brief  生成柏林噪音
 * 
 * \author zhn
 *********************************************************************/
#pragma once
#include"iostream"
#include"vector"

///Perlin类
/// @see ImageToolkit
class Perlin
{

private:

std::vector<int> p;												///两倍排列数来回避超出区间 0~255


public:
	int repeat = -1;				///<什么时候开始重复 默认-1表示不起作用

	///构造函数来生成排列数
	Perlin(int repeat=-1			///<[in]什么时候开始重复 默认-1表示不起作用
	) {
		repeat = repeat;
		//p = new int[512];
		p.resize(512);
		for (int i = 0; i < 256; ++i) p[i] = i;
		for (int i = 0; i < 256; ++i) std::swap(p[i], p[rand() % 256]);
		for (int i = 0; i < 512; ++i) p[i] = p[i % 256];
		/*for (int x = 0; x < 512; x++) {

			p[x] = permutation[x % 256];
		}*/
	}
	
	double OctavePerlin(double x, double y, double z, int octaves, double persistence) {
		double total = 0;
		double frequency = 1;
		double amplitude = 1;
		double maxValue = 0;			// Used for normalizing result to 0.0 - 1.0
		for (int i = 0; i < octaves; i++) {
			total += perlin(x * frequency, y * frequency, z * frequency) * amplitude;

			maxValue += amplitude;

			amplitude *= persistence;
			frequency *= 2;
		}

		return total / maxValue;
	}


	///获取影响值
	/// @remark 函数是三维的，二维只需要z取0 参数坐标还要映射到晶格体上 晶格体决定大小
	double perlin(
		double x			///<[in]坐标x 
		, double y			///<[in]坐标y
		, double z			///<[in]坐标z
	) {
		if (repeat > 0) {									// If we have any repeat on, change the coordinates to their "local" repetitions
			x = int(x) % repeat;
			y = int(y) % repeat;
			z = int(z) % repeat;
		}

		int xi = (int)x & 255;								// Calculate the "unit cube" that the point asked will be located in
		int yi = (int)y & 255;								// The left bound is ( |_x_|,|_y_|,|_z_| ) and the right bound is that
		int zi = (int)z & 255;								// plus 1.  Next we calculate the location (from 0.0 to 1.0) in that cube.
		double xf = x - (int)x;								// We also fade the location to smooth the result.
		double yf = y - (int)y; 
		double zf = z - (int)z;
		double u = fade(xf);
		double v = fade(yf);
		double w = fade(zf);

		int aaa, aba, aab, abb, baa, bba, bab, bbb;
		aaa = p[p[p[xi] + yi] + zi];
		aba = p[p[p[xi] + inc(yi)] + zi];
		aab = p[p[p[xi] + yi] + inc(zi)];
		abb = p[p[p[xi] + inc(yi)] + inc(zi)];
		baa = p[p[p[inc(xi)] + yi] + zi];
		bba = p[p[p[inc(xi)] + inc(yi)] + zi];
		bab = p[p[p[inc(xi)] + yi] + inc(zi)];
		bbb = p[p[p[inc(xi)] + inc(yi)] + inc(zi)];

		double x1, x2, y1, y2;
		x1 = lerp(grad(aaa, xf, yf, zf),				// The gradient function calculates the dot product between a pseudorandom
			grad(baa, xf - 1, yf, zf),				// gradient vector and the vector from the input coordinate to the 8
			u);										// surrounding points in its unit cube.
		x2 = lerp(grad(aba, xf, yf - 1, zf),				// This is all then lerped together as a sort of weighted average based on the faded (u,v,w)
			grad(bba, xf - 1, yf - 1, zf),				// values we made earlier.
			u);
		y1 = lerp(x1, x2, v);

		x1 = lerp(grad(aab, xf, yf, zf - 1),
			grad(bab, xf - 1, yf, zf - 1),
			u);
		x2 = lerp(grad(abb, xf, yf - 1, zf - 1),
			grad(bbb, xf - 1, yf - 1, zf - 1),
			u);
		y2 = lerp(x1, x2, v);

		return (lerp(y1, y2, w) + 1) / 2;						// For convenience we bound it to 0 - 1 (theoretical min/max before is -1 - 1)
	}

	///相当于+1
	int inc(int num) {
		num++;
		if (repeat > 0) num %= repeat;

		return num;
	}

	///梯度值
	static double grad(
		int hash				///<[in]哈希值 0~255
		, double x				
		, double y
		, double z
	) {
		int h = hash & 15;									// Take the hashed value and take the first 4 bits of it (15 == 0b1111)
		double u = h < 8 /* 0b1000 */ ? x : y;				// If the most significant bit (MSB) of the hash is 0 then set u = x.  Otherwise y.

		double v;											// In Ken Perlin's original implementation this was another conditional operator (?:).  I
		// expanded it for readability.

		if (h < 4 /* 0b0100 */)								// If the first and second significant bits are 0 set v = y
			v = y;
		else if (h == 12 /* 0b1100 */ || h == 14 /* 0b1110*/)// If the first and second significant bits are 1 set v = x
			v = x;
		else 												// If the first and second significant bits are not equal (0/1, 1/0) set v = z
			v = z;

		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v); // Use the last 2 bits to decide if u and v are positive or negative.  Then return their addition.
	}

	///参数方程
	/// 曲线五次
	/// 大概图像慢-块-慢 0~1
	static double fade(double t			//约束0~1
	) {
		// Fade function as defined by Ken Perlin.  This eases coordinate values
		// so that they will "ease" towards integral values.  This ends up smoothing
		// the final output.
		return t * t * t * (t * (t * 6 - 15) + 10);			// 6t^5 - 15t^4 + 10t^3
	}

	///线性插值
	static double lerp(
		double a			///<[in]左区间
		, double b			///<[in]右区间
		, double x			///约束值
	) {
		return a + x * (b - a);
	}

};

//namespace perlin { Perlin p1; }