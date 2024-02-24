#ifndef __DEPG0290BxS75AFxX_BW_H__
#define __DEPG0290BxS75AFxX_BW_H__

/* EPD commands */
#define DRIVER_OUTPUT_CONTROL                       0x01 // 驱动器输出控制
#define BOOSTER_SOFT_START_CONTROL                  0x0C // 动力放大器软启动控制
#define GATE_SCAN_START_POSITION                    0x0F // 门扫描起始位置
#define DEEP_SLEEP_MODE                             0x10 // 深度休眠模式
#define DATA_ENTRY_MODE_SETTING                     0x11 // 数据输入模式设置
#define SW_RESET                                    0x12 // 软件复位
#define TEMPERATURE_SENSOR_CONTROL                  0x1A // 温度传感器控制
#define MASTER_ACTIVATION                           0x20 // 主激活
#define DISPLAY_UPDATE_CONTROL_1                    0x21 // 显示更新控制 1
#define DISPLAY_UPDATE_CONTROL_2                    0x22 // 显示更新控制 2
#define WRITE_RAM                                   0x24 // 写入 RAM
#define WRITE_VCOM_REGISTER                         0x2C // 写入 VCOM 寄存器
#define LOAD_WS_OTP                                 0x31
#define WRITE_LUT_REGISTER                          0x32 // 写入 LUT 寄存器
#define SET_DUMMY_LINE_PERIOD                       0x3A // 设置虚拟行周期
#define SET_GATE_TIME                               0x3B // 设置门时间
#define BORDER_WAVEFORM_CONTROL                     0x3C // 边框波形控制
#define SET_RAM_X_ADDRESS_START_END_POSITION        0x44 // 设置 RAM X 起始/结束地址位置
#define SET_RAM_Y_ADDRESS_START_END_POSITION        0x45 // 设置 RAM Y 起始/结束地址位置
#define SET_RAM_X_ADDRESS_COUNTER                   0x4E // 设置 RAM X 地址计数器
#define SET_RAM_Y_ADDRESS_COUNTER                   0x4F // 设置 RAM Y 地址计数器
#define TERMINATE_FRAME_READ_WRITE                  0xFF // 终止帧读写
/* Display resolution */
#define EPD_WIDTH          128
#define EPD_HEIGHT         296
#define SCREEN_BUFFER_SIZE   (EPD_WIDTH * EPD_HEIGHT / 8)


class Eink290BW {
public:
    Eink290BW()
    = default;

    void Init();

    void Update();

    void DeepSleep();

    void DrawBitmap(const unsigned char *datas, int size = SCREEN_BUFFER_SIZE);

    void SetFrameMemory(const unsigned char *image_buffer, int x, int y, int image_width, int image_height);

    void HalLcd_Partial_Update();

    void ClearFrameMemory(unsigned char color);

    void demo();

    // 刷全白
    void SendWhite();

private:
    void SendCommand(unsigned char command);

    void SendData(unsigned char data);

    void WaitUntilIdle();

    void SpiTransfer(unsigned char data);

    void SetMemoryArea(int x_start, int y_start, int x_end, int y_end);

    void ReSetMemoryArea();

    void SetMemoryPointer(int x, int y);

    void SetLut(const unsigned char *lut, int size);

    const unsigned char *lut;
};

#endif
