#ifndef _oled_H
#define _oled_H


#include "sys.h"
#include "stdlib.h"	



//OLEDģʽ����
//0:4�ߴ���SPIģʽ
//1:����8080ģʽ
//2:IICģʽ
#define OLED_MODE 0


#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64


#if OLED_MODE==0
//OLDE-SPI4�߿��ƹܽŶ���
#define OLED_CS   PEout(10)		
#define OLED_RST  PEout(7)
#define OLED_DC   PEout(8)
#define OLED_SCL  PBout(0)
#define OLED_SDA  PBout(1)
#endif

//TODO: ����ģʽû��
#if OLED_MODE==1
//OLDE-8080���߿��ƹܽŶ���
#define OLED_CS   PDout(3)		
#define OLED_RST  PDout(4)
#define OLED_DC   PDout(5)
#define OLED_WR   PDout(6)
#define OLED_RD   PDout(7)

#define OLED_DATA_OUT(x) GPIO_Write(GPIOC,x);//���
#endif


#if OLED_MODE==2
//OLDE-IIC���߿��ƹܽŶ���


#endif


 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����		    						  



//OLED�����ú���
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);	   							   		    
void OLED_Init(void);
void OLED_Refresh_Gram(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size); 
void OLED_ShowFontHZ(u8 x,u8 y,u8 pos,u8 size,u8 mode);
void OLED_DrawBMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[]);
void OLED_ShowFloat(u8 x, u8 y, float val);

#endif
