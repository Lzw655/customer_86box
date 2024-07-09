/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <ESP_Panel_Library.h>
#include <lvgl.h>

/* Display related parameters */
#define LVGL_PORT_DISP_WIDTH                    (ESP_PANEL_LCD_WIDTH)
#define LVGL_PORT_DISP_HEIGHT                   (ESP_PANEL_LCD_HEIGHT)

/* LVGL related parameters */
#define LVGL_PORT_TICK_PERIOD_MS                (2)
#define LVGL_PORT_BUFFER_MALLOC_CAPS            (MALLOC_CAP_SPIRAM)
// #define LVGL_PORT_BUFFER_MALLOC_CAPS            (MALLOC_CAP_INTERNAL)
#define LVGL_PORT_BUFFER_SIZE                   (LVGL_PORT_DISP_WIDTH * LVGL_PORT_DISP_HEIGHT)
#define LVGL_PORT_TASK_MAX_DELAY_MS             (500)
#define LVGL_PORT_TASK_MIN_DELAY_MS             (2)
#define LVGL_PORT_TASK_STACK_SIZE               (6 * 1024)
#define LVGL_PORT_TASK_PRIORITY                 (2)
#define LVGL_PORT_TASK_CORE                     (1)

/* Avoid tering related configurations */
// Mode Type, don't change these
#define LVGL_PORT_AVOID_TEARING_MODE_NONE       (0)     // No avoid tearing
#define LVGL_PORT_AVOID_TEARING_MODE_0          (1)     // LCD double-buffer + LVGL full-refresh
#define LVGL_PORT_AVOID_TEARING_MODE_1          (2)     // LCD double-buffer + LVGL direct-mode (recommended)
#define LVGL_PORT_AVOID_TEARING_MODE_2          (3)     // LCD triple-buffer + LVGL full-refresh
// Set the avoid tearing mode
#define LVGL_PORT_AVOID_TEARING_MODE            (LVGL_PORT_AVOID_TEARING_MODE_1)
#if LVGL_PORT_AVOID_TEARING_MODE != LVGL_PORT_AVOID_TEARING_MODE_NONE
// Set the rotation degree(0/90/180/270) when avoid tearing is enabled
#define LVGL_PORT_ROTATION_DEGREE               (0)
#endif

/**
 * Here, some important configurations will be set based on different anti-tearing modes and rotation angles.
 * No modification is required here.
 *
 * Users should use `lcd_bus->configRgbFrameBufferNumber(LVGL_PORT_DISP_BUFFER_NUM);` to set the buffer number before
 * initializing the LCD bus
 *
 */
// *INDENT-OFF*
#if LVGL_PORT_AVOID_TEARING_MODE == LVGL_PORT_AVOID_TEARING_MODE_NONE
    #define LVGL_PORT_DISP_BUFFER_NUM           (1)
#else
    #define LVGL_PORT_AVOID_TEAR                (1)
    // Set the buffer number and refresh mode according to the different modes
    #if LVGL_PORT_AVOID_TEARING_MODE == LVGL_PORT_AVOID_TEARING_MODE_0
        #define LVGL_PORT_DISP_BUFFER_NUM       (2)
        #define LVGL_PORT_FULL_REFRESH          (1)
    #elif LVGL_PORT_AVOID_TEARING_MODE == LVGL_PORT_AVOID_TEARING_MODE_1
        #define LVGL_PORT_DISP_BUFFER_NUM       (2)
        #define LVGL_PORT_DIRECT_MODE           (1)
    #elif LVGL_PORT_AVOID_TEARING_MODE == LVGL_PORT_AVOID_TEARING_MODE_2
        #define LVGL_PORT_DISP_BUFFER_NUM       (3)
        #define LVGL_PORT_FULL_REFRESH          (1)
    #else
        #error "Invalid avoid tearing mode, please set macro `LVGL_PORT_AVOID_TEARING_MODE` to one of `LVGL_PORT_AVOID_TEARING_MODE_*`"
    #endif
    // Check rotation
    #if (LVGL_PORT_ROTATION_DEGREE != 0) && (LVGL_PORT_ROTATION_DEGREE != 90) && (LVGL_PORT_ROTATION_DEGREE != 180) && \
        (LVGL_PORT_ROTATION_DEGREE != 270)
        #error "Invalid rotation degree, please set to 0, 90, 180 or 270"
    #elif LVGL_PORT_ROTATION_DEGREE != 0
        #ifdef LVGL_PORT_DISP_BUFFER_NUM
        #undef LVGL_PORT_DISP_BUFFER_NUM
        #define LVGL_PORT_DISP_BUFFER_NUM       (3)
        #endif
    #endif
    #define LVGL_PORT_ROTATION_DEGREE_TYPE       ((lv_disp_rot_t)(LVGL_PORT_ROTATION_DEGREE / 90))
#endif /* LVGL_PORT_AVOID_TEARING_MODE */
// *INDENT-OFF*

#ifdef __cplusplus
extern "C" {
#endif

bool lvgl_port_init(ESP_PanelLcd *lcd, ESP_PanelTouch *tp);

bool lvgl_port_lock(int timeout_ms);

bool lvgl_port_unlock(void);

#if LVGL_PORT_AVOID_TEAR
bool lvgl_port_notify_rgb_vsync(void);
#endif

#ifdef __cplusplus
}
#endif
