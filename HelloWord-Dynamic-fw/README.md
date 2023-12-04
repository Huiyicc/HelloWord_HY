# HelloWord-Dynamic

---

## 介绍
[HelloWord-Dynamic](https://github.com/Huiyicc/HelloWord_HY/tree/master/HelloWord-Dynamic-fw)是基于稚晖君([@peng-zhihui](https://github.com/peng-zhihui))原版 [Dynamic](https://github.com/peng-zhihui/HelloWord-Keyboard/tree/main/2.Firmware/HelloWord-Dynamic-fw) 代码上进行大量补充,修复之后的版本

## 说明

## 更新计划

- [x] 电机驱动
  - [x] 力反馈模式
    - [x] 弹簧编码器模式
    - [x] 编码器模式
    - [x] 惯性模式
    - [x] 摇杆模式
    - [x] 限位模式
    - [x] 智能模式
      - > 低速情况下为编码器模式,高速情况下为惯性模式
  - [ ] 电机双向通信
    - > 上位机发送指令操作电机/电机反馈位置给上位机
  - [ ] 电机位置反馈
  - [ ] 修复偶尔唤醒后角度控制模式会有电音的问题
    - > 已经尽力压倒最小了,但是由于我没有设备测量数据,无法调整到完美.
  - [x] 修复电机在低速时会有抖动的问题
- [x] RGB灯效
- [x] 低功耗休眠模式
- [x] 内置APP架构
  - [x] 亮度调节
  - [x] 音量调节
  - [x] 横向/纵向滚动模式
  - [x] 剪辑/视频进度调节
  - [x] 视窗模式
  - [ ] APP应用市场
- [x] 上位机(网页/APP)控制
- [x] 双按钮短按/长按事件
  - [x] 双按钮双击/多击事件
- [x] 墨水屏驱动
  - [x] 全刷
  - [x] 局刷
  - [ ] 快刷
  - [ ] 三色屏
- [ ] 修复UART
  - > 由于更换了HID通信模式,需要改为复合设备,emmmm,谁爱该谁改,太麻烦了

# 自定义图标字库
- 首先安装 [fontforge](https://github.com/fontforge/fontforge)
> fontforge 汉化方法:  
> - 进入fontforge安装目录,编辑fontforge.bat  
> - 找到set LANGUAGE=en,删除前面的::  
> - 修改为set LANGUAGE=zh_CN  
> - 保存

- 使用fontforge打开`hwd_hy.sfd`
- 导出ttf
- 打开[guitool](./scripts/guitooldswda.zip),使用刚刚的ttf导出bpf文件(像素设置24,导出00-FF字符)
- 制作导出map
- 命令行进入scripts文件夹
- `"./u8g2/tools/font/bdfconv/bdfconv.exe"  -v -b 0 -f 1 <bdf文件>  -M "../font/hwd_hy.map" -n hwd_hy_U24 -o  -o "../font/hwd_hy_U24.c" -d <bdf文件>`

# 相关链接

---

- [peng-zhihui/HelloWord-Keyboard](https://github.com/peng-zhihui/HelloWord-Keyboard)  
- [fontforge](https://github.com/fontforge/fontforge)

# 协议

---

MIT License