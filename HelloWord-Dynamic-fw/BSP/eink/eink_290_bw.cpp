#include <cstdio>
#include <cstring>
#include "eink_290_bw.h"
#include "pgmspace.h"
#include "stm32f4xx_hal.h"
#include "spi.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

// GC 波形
const unsigned char lut_GC[]={  //153+6=159 bytes
        0x0A,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x01,0x2A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x0A,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x05,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x05,0x05,0x00,
        0x05,0x03,0x05,0x05,0x00,
        0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,

};

// DU 波形
const unsigned char lut_DU[]={  //153+6=159 bytes
        0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //00
        0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x03, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char  lut_R24_DU[] =
        {
                0x01,	0x40,	0x14,	0x01,	0x01,	0x00,	0x00,	0x00,
                0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
                0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
                0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
                0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
                0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
                0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,
        };

extern const unsigned char gImage_1[];
extern const unsigned char gImage_2[];

//uint8_t *Eink290BW::buffer = nullptr;

void Eink290BW::demo() {
//    auto tmpPtr = (uint8_t *) pvPortMalloc(EPD_HEIGHT * EPD_WIDTH / 8);
//    memset(tmpPtr, 0xff, EPD_HEIGHT * EPD_WIDTH / 8);
//
//    // 全刷
//    DrawBitmap(tmpPtr);
//    Update();
//    HAL_Delay(1100);

    //SetLut(lut_R24_DU, sizeof (lut_R24_DU));


    // 设置画面
//    SetFrameMemory(gImage_1,0,0,128,148);
//    HalLcd_Partial_Update();
//    SetFrameMemory(gImage_1,0,0,128,148);
//    HalLcd_Partial_Update();
//    SetFrameMemory(gImage_1,0,0,128,148);
//    HalLcd_Partial_Update();
//    HAL_Delay(1000);
//    SetFrameMemory(gImage_2,0,148,128,148);
//    HalLcd_Partial_Update();
//    HAL_Delay(1000);
//    memset(tmpPtr, 0x0, EPD_HEIGHT * EPD_WIDTH / 8);
//    SetLut(LUTDefault_full,sizeof (LUTDefault_full));
////    //SetFrameMemory(gImage_2,0,0,EPD_WIDTH,EPD_HEIGHT);
////    //ClearFrameMemory();
//    SetMemoryArea(0,0,EPD_HEIGHT,EPD_WIDTH);
//    DrawBitmap(tmpPtr);
//    Update();

}

void Eink290BW::Init() {
//    if (buffer != nullptr) {
//        vPortFree(buffer);
//    }
//    buffer = (uint8_t *) pvPortMalloc(EPD_HEIGHT * EPD_WIDTH / 8);
//    memset(buffer, 0x0, EPD_HEIGHT * EPD_WIDTH / 8);

    HAL_GPIO_WritePin(EINK_RST_GPIO_Port, EINK_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(EINK_RST_GPIO_Port, EINK_RST_Pin, GPIO_PIN_SET);

    WaitUntilIdle();
    SendCommand(SW_RESET); // soft reset
    WaitUntilIdle();

    SendCommand(0x74); //set analog block control
    SendData(0x54);
    SendCommand(0x7E); //set digital block control
    SendData(0x3B);

    SendCommand(DRIVER_OUTPUT_CONTROL); //Driver output control
    SendData(0x27);
    SendData(0x01);
    SendData(0x00);

    SendCommand(DATA_ENTRY_MODE_SETTING); //data entry mode
    SendData(0x01);

    SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION); //set Ram-X address start/end position
    SendData(0x00);
    SendData(0x0F);    //0x0F-->(15+1)*8=128

    SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION); //set Ram-Y address start/end position
    SendData(0x27);   //0x0127-->(295+1)=296
    SendData(0x01);
    SendData(0x00);
    SendData(0x00);
    SendCommand(BORDER_WAVEFORM_CONTROL); //set border
    SendData(0x01);

    SendCommand(0x18); // use the internal temperature sensor
    SendData(0x80);


    SendCommand(DISPLAY_UPDATE_CONTROL_2);
    SendData(0xB1);
    SendCommand(MASTER_ACTIVATION);

    SendCommand(SET_RAM_X_ADDRESS_COUNTER);
    SendData(0x00);
    SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
    SendData(0x27);
    SendData(0x01);
    WaitUntilIdle();
    //printf("e-Paper init OK!");
}

void Eink290BW::DrawBitmap(const unsigned char *datas) {
    SendCommand(WRITE_RAM);   //write RAM for black(0)/white (1)

    for (int i = 0; i < SCREEN_BUFFER_SIZE; i++)
        SendData(PGM_READ_BYTES(&datas[i]));
}

void Eink290BW::DeepSleep() {
    SendCommand(0x10); //enter deep sleep
    SendData(0x01);
}


void Eink290BW::SendCommand(unsigned char command) {
    HAL_GPIO_WritePin(EINK_DC_GPIO_Port, EINK_DC_Pin, GPIO_PIN_RESET);
    SpiTransfer(command);
}

void Eink290BW::SendData(unsigned char data) {
    HAL_GPIO_WritePin(EINK_DC_GPIO_Port, EINK_DC_Pin, GPIO_PIN_SET);
    SpiTransfer(data);
}

void Eink290BW::WaitUntilIdle() {
    while (HAL_GPIO_ReadPin(EINK_BUSY_GPIO_Port, EINK_BUSY_Pin) == GPIO_PIN_SET) {
        osDelay(100);
    }
}

void Eink290BW::SpiTransfer(unsigned char data) {
    HAL_GPIO_WritePin(EINK_CS_GPIO_Port, EINK_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, &data, 1, 5);
    HAL_GPIO_WritePin(EINK_CS_GPIO_Port, EINK_CS_Pin, GPIO_PIN_SET);
}

/**
 *  @brief: clear the frame memory with the specified color.
 *          this won't update the display.
 */
void Eink290BW::ClearFrameMemory(unsigned char color) {

    SendCommand(WRITE_RAM);
    /* send the color data */
    for (int i = 0; i < EPD_WIDTH / 8 * EPD_HEIGHT; i++) {
        SendData(color);
    }
}

/**
 *  @brief: update the display
 *          there are 2 memory areas embedded in the e-paper display
 *          but once this function is called,
 *          the the next action of SetFrameMemory or ClearFrame will
 *          set the other memory area.
 */
void Eink290BW::HalLcd_Partial_Update(void) {


//    SendCommand(DISPLAY_UPDATE_CONTROL_1);
//    SendData(0x40);
//
//    SendCommand(DISPLAY_UPDATE_CONTROL_2);
//    SendData(0xC7);
//    SendCommand(MASTER_ACTIVATION);
//    WaitUntilIdle();

    SendCommand(0x22);
    SendData(0xCF);
    // _writeData(0x04);
    SendCommand(0x20);

    WaitUntilIdle();

}

void Eink290BW::Update() {
    SendCommand(DISPLAY_UPDATE_CONTROL_1);
    SendData(0x40);

    SendCommand(DISPLAY_UPDATE_CONTROL_2);
    SendData(0xC7);
    SendCommand(MASTER_ACTIVATION);
    WaitUntilIdle();
}

/**
 *  @brief: put an image buffer to the frame memory.
 *          this won't update the display.
 */
void Eink290BW::SetFrameMemory(
        const unsigned char *image_buffer,
        int x,
        int y,
        int image_width,
        int image_height
) {
    int x_end;
    int y_end;

    if (
            image_buffer == NULL ||
            x < 0 || image_width < 0 ||
            y < 0 || image_height < 0
            ) {
        return;
    }
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    x &= 0xF8;
    image_width &= 0xF8;
    if (x + image_width >= EPD_WIDTH) {
        x_end = EPD_WIDTH - 1;
    } else {
        x_end = x + image_width - 1;
    }
    if (y + image_height >= EPD_HEIGHT) {
        y_end = EPD_HEIGHT - 1;
    } else {
        y_end = y + image_height - 1;
    }
    SetMemoryArea(x, y, x_end, y_end);
    SetMemoryPointer(x, y);
    SendCommand(WRITE_RAM);
    /* send the image data */
    for (int j = 0; j < y_end - y + 1; j++) {
        for (int i = 0; i < (x_end - x + 1) / 8; i++) {
            SendData(image_buffer[i + j * (image_width / 8)]);
        }
    }
}

/**
 *  @brief: private function to specify the memory area for data R/W
 */
void Eink290BW::SetMemoryArea(int x_start, int y_start, int x_end, int y_end) {
    SendCommand(DATA_ENTRY_MODE_SETTING); //data entry mode
    SendData(0x01);
    SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    SendData((x_start >> 3) & 0xFF);
    SendData((x_end >> 3) & 0xFF);
    SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);
    SendData(y_start & 0xFF);
    SendData((y_start >> 8) & 0xFF);
    SendData(y_end & 0xFF);
    SendData((y_end >> 8) & 0xFF);
}

/**
 *  @brief: private function to specify the start point for data R/W
 */
void Eink290BW::SetMemoryPointer(int x, int y) {
    SendCommand(SET_RAM_X_ADDRESS_COUNTER);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    SendData((x >> 3) & 0xFF);
    SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
    SendData(y & 0xFF);
    SendData((y >> 8) & 0xFF);

}

/**
 *  @brief: set the look-up table register
 */
void Eink290BW::SetLut(const unsigned char *lut,int size) {
    this->lut = lut;
    SendCommand(WRITE_LUT_REGISTER);
    /* the length of look-up table is 30 bytes */
    for (int i = 0; i < size; i++) {
        SendData(this->lut[i]);
    }
}
