#include "ProjectMath.h"

uint8 BmpHead[14 + 40 + 1024] = {0};

//////8位bmp格式文件头，直接放于图像数组之前则形成一个BMP文件
void BmpHeadinit(uint16 H, uint16 W)
{
    uint16 size = H * W;
    uint16 m_lenH = 14 + 40 + 1024;
    BmpHead[0] = 0x42;
    BmpHead[1] = 0x4D;
    BmpHead[2] = (size & 0x00FF);           /////小端存储
    BmpHead[3] = (size & 0xFF00) >> 8;
    BmpHead[4] = 0;
    BmpHead[5] = 0;
    BmpHead[7] = 0x00;  
    BmpHead[8] = 0x00;                       //WORD 保留字，不考虑  
    BmpHead[9] = 0x00;  
    BmpHead[10] = (m_lenH & 0x00FF);      //DWORD 实际位图数据的偏移字  
    BmpHead[11] = (m_lenH & 0xFF00) >> 8;      //节数，即所有三个头（文件头、  
    BmpHead[12] = 0;                       //文件信息头、颜色表）之和    
    BmpHead[13] = 0;                       //14 + 40 + 1024 = 1078 
    
    ///////////////////////以上为文件头
    
    BmpHead[14] = 40;                      ////文件信息头大小
    BmpHead[15] = 0;
    BmpHead[16] = 0;
    BmpHead[17] = 0;
    BmpHead[18] = (W & 0x00FF);            ////图片宽度
    BmpHead[19] = (W & 0xFF00) >> 8;
    BmpHead[20] = 0;
    BmpHead[21] = 0;
    BmpHead[22] = (H & 0x00FF);           /////图片高度
    BmpHead[23] = (H & 0xFF00) >> 8;
    BmpHead[24] = 0;
    BmpHead[25] = 0;
    
    BmpHead[26] = 1;
    BmpHead[27] = 0;
    BmpHead[28] = 8;                      /////图像位数
    BmpHead[29] = 0;
    BmpHead[30] = 0;
    BmpHead[31] = 0;
    BmpHead[32] = 0;
    BmpHead[33] = 0;
    BmpHead[34] = 0;
    BmpHead[35] = 0;
    BmpHead[36] = 0;
    BmpHead[37] = 0;
    BmpHead[38] = 0x20;          ///X分辨率
    BmpHead[39] = 0x4E;
    BmpHead[40] = 0;
    BmpHead[41] = 0;
    BmpHead[42] = 0x20;         ////Y分辨率
    BmpHead[43] = 0x4E;
    BmpHead[44] = 0;
    BmpHead[45] = 0;
    
    BmpHead[46] = 0;
    BmpHead[47] = 0;
    BmpHead[48] = 0;
    BmpHead[49] = 0;
    BmpHead[50] = 0;
    BmpHead[51] = 0;
    BmpHead[52] = 0;
    BmpHead[53] = 0;
    ////////以上为文件信息头
    for(int16 i = 0; i < 256; i++)
    {
      BmpHead[i * 4 + 54] = i;
      BmpHead[i * 4 + 55] = i;
      BmpHead[i * 4 + 56] = i;
      BmpHead[i * 4 + 57] = 0xFF;
    }
    //////以上为调色板
}


void UnZip(uint8 *dst, uint8 *src, uint32 srclen)/////图像解压缩
{
    uint8_t colour[2] = { 0xff, 0x00 }; //0 和 1 分别对应的颜色///山外摄像头，原数据为0为白
    uint8_t * mdst = dst;
    uint8_t * msrc = src;
    uint8_t tmpsrc;
    while (srclen--)
    {
        tmpsrc = *msrc++;
        *mdst++ = colour[(tmpsrc >> 7) & 0x01];
        *mdst++ = colour[(tmpsrc >> 6) & 0x01];
        *mdst++ = colour[(tmpsrc >> 5) & 0x01];
        *mdst++ = colour[(tmpsrc >> 4) & 0x01];
        *mdst++ = colour[(tmpsrc >> 3) & 0x01];
        *mdst++ = colour[(tmpsrc >> 2) & 0x01];
        *mdst++ = colour[(tmpsrc >> 1) & 0x01];
        *mdst++ = colour[(tmpsrc >> 0) & 0x01];
    }
  }

uint32 Float2U32(float data)    ///////浮点转32位
{
    uint8 * point;
    point = (uint8*)(&data);
    return (((uint32)point[3] << 24) | ((uint32)point[2] << 16) | ((uint32)point[1] << 8) | ((uint32)point[0]));
}

float U322Float(uint32 * data)   /////32位转浮点
{
    return (*((float *)data));
} 

int ABS(int data)   ////////整型绝对值
{
    return ((data >= 0) ? data : (-data));
} 

float ABSF(float data)  ///////浮点绝对值
{
    return ((data >= 0) ? data : (-data));
} 

int ABSDet(int datai, int dataj)   ///////整型差值绝对值
{
    int temp = datai - dataj;
    return ((temp >= 0) ? temp : (-temp));
}

float ABSFDet(float datai, float dataj) //////浮点差值绝对值
{
    float temp = datai - dataj;
    return ((temp >= 0) ? temp : (-temp));
}
