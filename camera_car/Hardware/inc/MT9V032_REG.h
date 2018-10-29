#ifndef MT9V032_REG_H
#define MT9V032_REG_H


#define  MT9V032_ADDR           (0xB8 >> 1)
#define  CHIPVERSION            0x00
#define  COLUMNSTART            0x01
#define  ROWSTART               0x02
#define  WINDOWHEIGHT           0x03
#define  WINDOWWIDTH            0x04
#define  HBLANKING              0x05
#define  VBLANKING              0x06
#define  CHIPCONTROL            0x07
#define  SHUTTERWIDTH1          0x08
#define  SHUTTERWIDTH2          0x09
#define  SHUTTERWIDTHCONTROL    0x0A
#define  TOTALSHUTTERWIDTH      0x0B
#define  RESET                  0x0C
#define  READMODE               0x0D
#define  MONITROMODE            0x0E
#define  PIXELMODE              0x0F
#define  LEDOUTCONTROL          0x1B
#define  ADCRESCONTROL          0x1C
#define  VREFADCCONTROL         0x2C
#define  V1CONTROL              0x31
#define  V2CONTROL              0x32
#define  V4CONTROL              0x34
#define  ANALOGGAIN             0x35
#define  V3CONTROL              0x33
#define  MAXANALOGGAIN          0x36
#define  FRAMEDARK              0x42
#define  DARKTHRESHOLDS         0x46
#define  BLACKLEVELCONTROL      0x47
#define  BLACKLEVELVALUE        0x48
#define  AEGENABLE              0xAF
#define  REGLOCK                0xFE



#endif
