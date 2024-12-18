/*****************************************************************//**
 * \file   Macro.h
 * \brief  �������궨��
 *
 * \author zhn
 *********************************************************************/
#pragma once


 //���ڿ��
constexpr int WIN_WIDTH = 800;		///< ���ڿ�
constexpr int WIN_HEIGHT = 600;		///< ���ڸ�

//����
constexpr int RangeNumX = int((WIN_WIDTH + 100 - 1) / 100);
constexpr int RangeNumY = int((WIN_HEIGHT + 100 - 1) / 100);

//����
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef long long int64;
typedef int int32;
typedef short int16;
typedef char int8;