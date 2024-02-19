//
// Created by 19254 on 2023/9/4.
//

#ifndef HELLOWORD_DYNAMIC_FW_USBD_H
#define HELLOWORD_DYNAMIC_FW_USBD_H

#include <cstdint>

namespace HYSDK::USB {

void SendMouseUP(int num = 1);

void SendMouseDOWN(int num = 1);

enum KeyCode_t : int16_t {
    /*------------------------- HID report data -------------------------*/
    LEFT_CTRL = -8, LEFT_SHIFT = -7, LEFT_ALT = -6, LEFT_GUI = -5,
    RIGHT_CTRL = -4, RIGHT_SHIFT = -3, RIGHT_ALT = -2, RIGHT_GUI = -1,

    RESERVED = 0, ERROR_ROLL_OVER, POST_FAIL, ERROR_UNDEFINED,
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    NUM_1/*1!*/, NUM_2/*2@*/, NUM_3/*3#*/, NUM_4/*4$*/, NUM_5/*5%*/,
    NUM_6/*6^*/, NUM_7/*7&*/, NUM_8/*8**/, NUM_9/*9(*/, NUM_0/*0)*/,
    ENTER, ESC, BACKSPACE, TAB, SPACE,
    MINUS/*-_*/, EQUAL/*=+*/, LEFT_U_BRACE/*[{*/, RIGHT_U_BRACE/*]}*/,
    BACKSLASH/*\|*/, NONE_US/**/, SEMI_COLON/*;:*/, QUOTE/*'"*/,
    GRAVE_ACCENT/*`~*/, COMMA/*,<*/, PERIOD/*.>*/, SLASH/*/?*/,
    CAP_LOCK, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    PRINT, SCROLL_LOCK, PAUSE, INSERT, HOME, PAGE_UP, DELETE, END, PAGE_DOWN,
    RIGHT_ARROW, LEFT_ARROW, DOWN_ARROW, UP_ARROW, PAD_NUM_LOCK,
    PAD_SLASH, PAD_ASTERISK, PAD_MINUS, PAD_PLUS, PAD_ENTER,
    PAD_NUM_1, PAD_NUM_2, PAD_NUM_3, PAD_NUM_4, PAD_NUM_5,
    PAD_NUM_6, PAD_NUM_7, PAD_NUM_8, PAD_NUM_9, PAD_NUM_0,
    PAD_PERIOD, NONUS_BACKSLASH, APPLICATION, POWER, PAD_EQUAL,
    F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, EXECUTE,
    HELP, MENU, SELECT, STOP, AGAIN, UNDO, CUT, COPY, PASTE, FIND, MUTE, VOLUME_UP, VOLUME_DOWN,
    FN = 1000
    /*------------------------- HID report data -------------------------*/
};

void SendKeyBoardCode(int ctrl, KeyCode_t code);

uint8_t USBDHIDSendReport(void *pdev,
                          uint8_t *report, uint16_t len,uint8_t delay);


}


#endif //HELLOWORD_DYNAMIC_FW_USBD_H
