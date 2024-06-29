#include "eink_290_bw_xy.h"
#include "pgmspace.h"
#include "spi.h"
#include "stm32f4xx_hal.h"
#include <cstdio>

#ifdef _WF0290CZ0LP_


/**
 * full screen update LUT
**/

const unsigned char EPD_2IN9D_lut_vcomDC[] = {
    0x00, 0x08, 0x00, 0x00, 0x00, 0x02,
    0x60, 0x28, 0x28, 0x00, 0x00, 0x01,
    0x00, 0x14, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x12, 0x12, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};
const unsigned char EPD_2IN9D_lut_ww[] = {
    0x40, 0x08, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
    0x40, 0x14, 0x00, 0x00, 0x00, 0x01,
    0xA0, 0x12, 0x12, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const unsigned char EPD_2IN9D_lut_bw[] = {
    0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x0F, 0x0F, 0x00, 0x00, 0x03,
    0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
    0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const unsigned char EPD_2IN9D_lut_wb[] = {
    0x80, 0x08, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
    0x80, 0x14, 0x00, 0x00, 0x00, 0x01,
    0x50, 0x12, 0x12, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const unsigned char EPD_2IN9D_lut_bb[] = {
    0x80, 0x08, 0x00, 0x00, 0x00, 0x02,
    0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
    0x80, 0x14, 0x00, 0x00, 0x00, 0x01,
    0x50, 0x12, 0x12, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/******************************************************************************
function :	LUT download
parameter:
******************************************************************************/
void Eink290BW::SetFullReg(void)
{
  SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING
  SendData(0xb7);		//WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7

  unsigned int count;
  SendCommand(0x20);
  for(count=0; count<44; count++) {
    SendData(EPD_2IN9D_lut_vcomDC[count]);
  }

  SendCommand(0x21);
  for(count=0; count<42; count++) {
    SendData(EPD_2IN9D_lut_ww[count]);
  }

  SendCommand(0x22);
  for(count=0; count<42; count++) {
    SendData(EPD_2IN9D_lut_bw[count]);
  }

  SendCommand(0x23);
  for(count=0; count<42; count++) {
    SendData(EPD_2IN9D_lut_wb[count]);
  }

  SendCommand(0x24);
  for(count=0; count<42; count++) {
    SendData(EPD_2IN9D_lut_bb[count]);
  }
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void Eink290BW::Clear(void)
{
  int Width, Height;
  Width = (EPD_2IN9D_WIDTH % 8 == 0)? (EPD_2IN9D_WIDTH / 8 ): (EPD_2IN9D_WIDTH / 8 + 1);
  Height = EPD_2IN9D_HEIGHT;

  SendCommand(0x10);
  for (int j = 0; j < Height; j++) {
    for (int i = 0; i < Width; i++) {
      SendData(0x00);
    }
  }

  SendCommand(0x13);
  for (int j = 0; j < Height; j++) {
    for (int i = 0; i < Width; i++) {
      SendData(0xFF);
    }
  }

  SetFullReg();
  Update();
}


uint8_t Eink290BW::buffer[EPD_HEIGHT * EPD_WIDTH / 8];

void Eink290BW::Init()
{
  HAL_GPIO_WritePin(EINK_RST_GPIO_Port, EINK_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(200);
  HAL_GPIO_WritePin(EINK_RST_GPIO_Port, EINK_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(200);
  HAL_GPIO_WritePin(EINK_RST_GPIO_Port, EINK_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(200);

  SendCommand(0x01); // DRIVER_OUTPUT_CONTROL
  SendData(0x03);
  SendData(0x00);
  SendData(0x2b);
  SendData(0x2b);
  SendData(0x03);

  SendCommand(0x06);	//boost soft start
  SendData(0x17);     //A
  SendData(0x17);     //B
  SendData(0x17);     //C

  SendCommand(0x04);
  WaitUntilIdle();

  SendCommand(0x00);	//panel setting
  SendData(0xbf);     //LUT from OTP，128x296
  SendData(0x0e);     //VCOM to 0V fast

  SendCommand(0x30);	//PLL setting
  SendData(0x3a);     // 3a 100HZ   29 150Hz 39 200HZ	31 171HZ

  SendCommand(0x61);	//resolution setting
  SendData(EPD_2IN9D_WIDTH);
  SendData((EPD_2IN9D_HEIGHT >> 8) & 0xff);
  SendData(EPD_2IN9D_HEIGHT & 0xff);

  SendCommand(0x82);	//vcom_DC setting
  SendData(0x28);

//    SendCommand(0x04);
//    WaitUntilIdle();
//
//    SendCommand(0x00);
//    SendData(0x1f);
//
//    SendCommand(0x61);
//    SendData (0x80);
//    SendData (0x01);
//    SendData (0x28);
//
//    SendCommand(0X50);
//    SendData (0x80);
}

void Eink290BW::DrawBitmap(const unsigned char* datas)
{
  int Width, Height;
  Width = (EPD_2IN9D_WIDTH % 8 == 0)? (EPD_2IN9D_WIDTH / 8 ): (EPD_2IN9D_WIDTH / 8 + 1);
  Height = EPD_2IN9D_HEIGHT;

  SendCommand(0x10);
  for (int j = 0; j < Height; j++) {
    for (int i = 0; i < Width; i++) {
      SendData(0x00);
    }
  }

  SendCommand(0x13);
  for (int j = 0; j < Height; j++) {
    for (int i = 0; i < Width; i++) {
      SendData(datas[i + (Height-j-1) * Width]);
    }
  }

  SetFullReg();
}

void Eink290BW::Update()
{

  SendCommand(0x12); // DISPLAY_UPDATE_CONTROL_2
  SendData(0x10);

  WaitUntilIdle();
}

void Eink290BW::DeepSleep()
{
  SendCommand(0X50);
  SendData(0xf7);
  SendCommand(0X02);  	//power off
  WaitUntilIdle();
  SendCommand(0X07);  	//deep sleep
  SendData(0xA5);

}


void Eink290BW::SendCommand(unsigned char command)
{
  HAL_GPIO_WritePin(EINK_DC_GPIO_Port, EINK_DC_Pin, GPIO_PIN_RESET);
  SpiTransfer(command);
}

void Eink290BW::SendData(unsigned char data)
{
  HAL_GPIO_WritePin(EINK_DC_GPIO_Port, EINK_DC_Pin, GPIO_PIN_SET);
  SpiTransfer(data);
}

void Eink290BW::WaitUntilIdle()
{
  HAL_Delay(100);
  while (HAL_GPIO_ReadPin(EINK_BUSY_GPIO_Port, EINK_BUSY_Pin) == GPIO_PIN_SET)
    HAL_Delay(100);
}

void Eink290BW::SpiTransfer(unsigned char data)
{
  HAL_GPIO_WritePin(EINK_CS_GPIO_Port, EINK_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi2, &data, 1, 5);
  HAL_GPIO_WritePin(EINK_CS_GPIO_Port, EINK_CS_Pin, GPIO_PIN_SET);
}


void Eink290BW::SendWhite() {
  SendCommand(WRITE_RAM);//write RAM for black(0)/white (1)
  for (int i = 0; i < EPD_WIDTH / 8 * EPD_HEIGHT; i++) {
    SendData(0xff);
  }
}

#endif
