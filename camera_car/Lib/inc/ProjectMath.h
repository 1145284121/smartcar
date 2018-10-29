#ifndef PROJECTMATH_H
#define PROJECTMATH_H


#include "common.h"

void UnZip(uint8 *dst, uint8 *src, uint32 srclen);/////图像解压缩
uint32 Float2U32(float data);                     ///////浮点转32位
float U322Float(uint32 * data);                   /////32位转浮点
int ABS(int data);                          ////////整型绝对值
float ABSF(float data);                  ///////浮点绝对值
int ABSDet(int datai, int dataj);               ///////差值绝对值
float ABSFDet(float datai, float dataj);

#endif