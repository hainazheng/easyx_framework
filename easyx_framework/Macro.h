/*****************************************************************//**
 * \file   Macro.h
 * \brief  别名、宏定义
 *
 * \author zhn
 *********************************************************************/
#pragma once


 //窗口宽高
constexpr int WIN_WIDTH = 800;		///< 窗口宽
constexpr int WIN_HEIGHT = 600;		///< 窗口高

//区域
constexpr int RangeNumX = int((WIN_WIDTH + 100 - 1) / 100);
constexpr int RangeNumY = int((WIN_HEIGHT + 100 - 1) / 100);

//别名
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef long long int64;
typedef int int32;
typedef short int16;
typedef char int8;