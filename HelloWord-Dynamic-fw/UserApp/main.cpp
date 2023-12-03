#include "common_inc.h"
#include "os/apps.hpp"
#include "os/app_desktop.hpp"
#include "os/app_volume.hpp"
#include "os/app_updown.hpp"
#include "os/app_light.hpp"
#include "os/app_windows.hpp"
#include "os/app_leftright.hpp"
#include "os/app_hid.hpp"
#include "os/ctrl.hpp"
#include "os/sleep.hpp"
#include "os/ButtonPin.hpp"
#include "os/RGB.hpp"
#include "SDK/utils.hpp"
#include "os/storage.hpp"

SysContext *g_sysCtx = nullptr;

void SysTask(TimerHandle_t t) {
  StorageTask(t);
  RGBTask();
  ButtonTask(t);
  timerSleepCallback(t);
}

template<class T>
void regApp() {
  auto lAppPtr = (T *) pvPortMalloc(sizeof(T));
  lAppPtr = new(lAppPtr) T();
  AppRegister(lAppPtr);
}

void helloWord() {
  g_sysCtx->Device.eink->Init();
  g_sysCtx->Device.eink->demo();

  //g_sysCtx->Device.eink->DeepSleep();
  CtrlInit();
  ButtonPinInit();
  //SleepTimerInit();
  xTimerStart(xTimerCreate("SysTimer", pdMS_TO_TICKS(100), pdTRUE, nullptr, SysTask), 0);
  if (!g_sysCtx->Device.ctrl.Action) {
    OLED_CLEAR_BUFFER();
    // logo区域
    OLED_DEVICES()->SetDrawColor(1);
    OLED_DEVICES()->SetFont(u8g2_font_wqy12_t_gb2312a);
    OLED_DEVICES()->DrawBox(6, 11, 20, 20);
    OLED_DEVICES()->DrawUTF8(9, 41, "校");
    OLED_DEVICES()->DrawUTF8(9, 55, "准");
    OLED_DEVICES()->DrawUTF8(9, 69, "失");
    OLED_DEVICES()->DrawUTF8(9, 83, "败");
    OLED_DEVICES()->SetDrawColor(0);
    OLED_DEVICES()->DrawStr(6 + 3, 15, "HY");
    OLED_SEND_BUFFER();
    exit(1);
  }


}

void InitOs() {
  //初始化所有设备
  g_sysCtx = (SysContext *) pvPortMalloc(sizeof(SysContext));
  g_sysCtx = new(g_sysCtx) SysContext();

  g_sysCtx->Device.boardConfig = (BoardConfig_t *) pvPortMalloc(sizeof(BoardConfig_t));
  g_sysCtx->Device.boardConfig = new(g_sysCtx->Device.boardConfig) BoardConfig_t();

  g_sysCtx->Device.oled = (SSD1306 *) pvPortMalloc(sizeof(SSD1306));
  g_sysCtx->Device.oled = new(g_sysCtx->Device.oled) SSD1306(&hi2c1);
  g_sysCtx->Device.oled->Init();

  OLED_CLEAR_BUFFER();
  // logo区域
  g_sysCtx->Device.oled->SetDrawColor(1);
  g_sysCtx->Device.oled->DrawBox(6, 11, 20, 20);
  // 文字区域
  OLED_DEVICES()->DrawUTF8(9, 41, "系");
  OLED_DEVICES()->DrawUTF8(9, 55, "统");
  OLED_DEVICES()->DrawUTF8(9, 69, "启");
  OLED_DEVICES()->DrawUTF8(9, 83, "动");
  OLED_DEVICES()->DrawUTF8(9, 97, "中");
  g_sysCtx->Device.oled->SetDrawColor(0);
  g_sysCtx->Device.oled->DrawStr(6 + 3, 15, "HY");
  OLED_SEND_BUFFER();
  osDelay(1500);
  // 初始化存储
  InitStorage();

  RGBInit();

  g_sysCtx->Device.eink = (Eink290BW *) pvPortMalloc(sizeof(Eink290BW));
  g_sysCtx->Device.eink = new(g_sysCtx->Device.eink) Eink290BW();

  // APP在这注册
  regApp<AppDesktop>();
  regApp<AppWindows>();
  regApp<AppVolume>();
  regApp<AppUpDown>();
  regApp<AppLight>();
  regApp<AppLeftRight>();
  regApp<AppHID>();
  // 此处初始化所有APP
  for (auto &iter: g_sysCtx->Apps.AppsMap) {
    iter.second->Init();
  }

  //启动事件
  // helloWord();
  AppChange(APPID_DESKTOP);
}

// =======

extern RGB rgb;


#define CODE_1       (0xfc)       //1码定时器计数次数
#define CODE_0       (0xc0)       //0码定时器计数次数

/*建立一个定义单个LED三原色值大小的结构体*/
typedef struct
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
}RGB_Color_TypeDef;

#define Pixel_NUM 4

void RGB_SetColor(uint8_t LedId,RGB_Color_TypeDef Color);//给一个LED装载24个颜色数据码（0码和1码）
void Reset_Load(void); //该函数用于将数组最后24个数据变为0，代表RESET_code
void RGB_SendArray(void);          //发送最终数组
void RGB_RED(uint16_t Pixel_Len);  //显示红灯
void RGB_GREEN(uint16_t Pixel_Len);//显示绿灯
void RGB_BLUE(uint16_t Pixel_Len); //显示蓝灯
void RGB_WHITE(uint16_t Pixel_Len);//显示白灯

/*Some Static Colors------------------------------*/
const RGB_Color_TypeDef RED      = {255,0,0};   //显示红色RGB数据
const RGB_Color_TypeDef GREEN    = {0,255,0};
const RGB_Color_TypeDef BLUE     = {0,0,255};
const RGB_Color_TypeDef SKY      = {0,255,255};
const RGB_Color_TypeDef MAGENTA  = {255,0,220};
const RGB_Color_TypeDef YELLOW   = {127,216,0};
const RGB_Color_TypeDef OEANGE   = {127,106,0};
const RGB_Color_TypeDef BLACK    = {0,0,0};
const RGB_Color_TypeDef WHITE    = {255,255,255};

/*二维数组存放最终PWM输出数组，每一行24个
数据代表一个LED，最后一行24个0代表RESET码*/
uint32_t Pixel_Buf[Pixel_NUM+6][24];

/*
功能：设定单个RGB LED的颜色，把结构体中RGB的24BIT转换为0码和1码
参数：LedId为LED序号，Color：定义的颜色结构体
*/
void RGB_SetColor(uint8_t LedId,RGB_Color_TypeDef Color)
{
  uint8_t i;
  if(LedId > Pixel_NUM)return; //avoid overflow 防止写入ID大于LED总数

  for(i=0;i<8;i++) Pixel_Buf[LedId][i]   = ( (Color.G & (1 << (7 -i)))? (CODE_1):CODE_0 );//数组某一行0~7转化存放G
  for(i=8;i<16;i++) Pixel_Buf[LedId][i]  = ( (Color.R & (1 << (15-i)))? (CODE_1):CODE_0 );//数组某一行8~15转化存放R
  for(i=16;i<24;i++) Pixel_Buf[LedId][i] = ( (Color.B & (1 << (23-i)))? (CODE_1):CODE_0 );//数组某一行16~23转化存放B
}

/*
功能：最后一行装在24个0，输出24个周期占空比为0的PWM波，作为最后reset延时，这里总时长为24*1.2=30us > 24us(要求大于24us)
*/
void Reset_Load(void)
{
  uint8_t i;
  for(i=0;i<24;i++)
  {
    Pixel_Buf[Pixel_NUM][i] = 0;
  }
}
//bool isRgbTxBusy=false;
//extern "C"
//void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
//  isRgbTxBusy = false;
//}

void RGB_SendArray(void)
{
//  while (isRgbTxBusy);
//  isRgbTxBusy = true;
//  HAL_SPI_Transmit_DMA(&hspi3, (uint8_t *) Pixel_Buf, (Pixel_NUM+6)*24);
//  while (isRgbTxBusy);
//  isRgbTxBusy = true;
//  uint8_t wsc[64]{0};
//  HAL_SPI_Transmit_DMA(&hspi3, wsc, 64);
}

/*
功能：显示红色
参数：Pixel_Len为显示LED个数
*/
void RGB_RED(uint16_t Pixel_Len)
{
  uint16_t i;
  for(i=0;i<Pixel_Len;i++)//给对应个数LED写入红色
  {
    RGB_SetColor(i,RED);
  }
  Reset_Load();
  RGB_SendArray();
}

/*
功能：显示绿色
参数：Pixel_Len为显示LED个数
*/
void RGB_GREEN(uint16_t Pixel_Len)
{
  uint16_t i;
  for(i=0;i<Pixel_Len;i++)//给对应个数LED写入绿色
  {
    RGB_SetColor(i,GREEN);
  }
  Reset_Load();
  RGB_SendArray();
}

/*
功能：显示蓝色
参数：Pixel_Len为显示LED个数
*/
void RGB_BLUE(uint16_t Pixel_Len)
{
  uint16_t i;
  for(i=0;i<Pixel_Len;i++)//给对应个数LED写入蓝色
  {
    RGB_SetColor(i,BLUE);
  }
  Reset_Load();
  RGB_SendArray();
}

/*
功能：显示白色
参数：Pixel_Len为显示LED个数
*/
void RGB_WHITE(uint16_t Pixel_Len)
{
  uint16_t i;
  for(i=0;i<Pixel_Len;i++)//给对应个数LED写入白色
  {
    RGB_SetColor(i,WHITE);
  }
  Reset_Load();
  RGB_SendArray();
}

void testrgb() {
  char i=0;
  while (1) {
//    RGB_GREEN(4);
//    HAL_Delay(1000);
//    RGB_BLUE(4);
//    HAL_Delay(1000);
//    RGB_RED(4);
//    HAL_Delay(1000);
    if (i>250) {
      i=1;
    }
    rgb.SetRgbBuffer({0,255,0},0.2);
    rgb.SyncLights();
    HAL_Delay(100);
    i++;
  }

}

// ===========

void Main(void) {
  // InitOs();
  testrgb();
}

