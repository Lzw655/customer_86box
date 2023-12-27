/*******************************************************************************
   LVGL Widgets
   This is a widgets demo for LVGL - Light and Versatile Graphics Library
   import from: https://github.com/lvgl/lv_demos.git

   Dependent libraries:
   LVGL: https://github.com/lvgl/lvgl.git

   Touch libraries:
   FT6X36: https://github.com/strange-v/FT6X36.git
   GT911: https://github.com/TAMCTec/gt911-arduino.git
   XPT2046: https://github.com/PaulStoffregen/XPT2046_Touchscreen.git

   LVGL Configuration file:
   Copy your_arduino_path/libraries/lvgl/lv_conf_template.h
   to your_arduino_path/libraries/lv_conf.h
   Then find and set:
   #define LV_COLOR_DEPTH     16
   #define LV_TICK_CUSTOM     1

   For SPI display set color swap can be faster, parallel screen don't set!
   #define LV_COLOR_16_SWAP   1

   Optional: Show CPU usage and FPS count
   #define LV_USE_PERF_MONITOR 1
 ******************************************************************************/
#include <esp_heap_caps.h>
#include "lv_demo_widgets.h"
#include <Ticker.h>
//#include "demos/lv_demos.h"
#include "doMain.h"

#include <ESP_Panel_Library.h>

ESP_Panel *panel = new ESP_Panel();

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    panel->getLcd()->drawBitmap(area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_p);

    lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    panel->getLcdTouch()->readData();

    bool touched = panel->getLcdTouch()->getLcdTouchState();
    if(!touched) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        TouchPoint point = panel->getLcdTouch()->getPoint();

        data->state = LV_INDEV_STATE_PR;
        /*Set the coordinates*/
        data->point.x = point.x;
        data->point.y = point.y;

        // Serial.printf("Touch point: x %d, y %d\n", point.x, point.y);
    }
}

Ticker ticker;

void tcr1s()
{
  Serial.printf("SRAM free size: %d\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
  Serial.printf("PSRAM free size: %d\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
}

void setup()
{
  Serial.begin(115200);

  vTaskDelay(pdMS_TO_TICKS(2000));

  Serial.printf("SRAM free size: %d\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
  Serial.printf("PSRAM free size: %d\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));

  // while (!Serial);
  Serial.println("LVGL Widgets Demo");

  lv_init();

  panel->init();
  panel->begin();

  screenWidth = ESP_PANEL_LCD_H_RES;
  screenHeight = ESP_PANEL_LCD_V_RES;

  // screenWidth = gfx->width();
  // screenHeight = gfx->height();
  // #ifdef ESP32
  // disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * 480, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  // #else
  //   disp_draw_buf = (lv_color_t *)malloc(sizeof(lv_color_t) * screenWidth * 100);
  // #endif
  //  disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * 480, MALLOC_CAP_SPIRAM);

  lv_color_t *buf_3_1 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * 480, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  // lv_color_t *buf_3_2 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * 480, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

  //  lv_color_t *buf_3_1 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * 100, MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
  // lv_color_t *buf_3_2 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * 100, MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);

  // if (!disp_draw_buf)
  // {
  //   Serial.println("LVGL disp_draw_buf allocate failed!");
  // }
  // if (!buf_3_1 && !buf_3_2)
  // {
  //   Serial.println("LVGL disp_draw_buf allocate failed!");
  // }

    if (!buf_3_1 )
  {
    Serial.println("LVGL disp_draw_buf allocate failed!");
  }


  else
  {
    lv_disp_draw_buf_init(&draw_buf, buf_3_1, NULL, screenWidth * 480);

    // lv_disp_draw_buf_init(&draw_buf, buf_3_1, buf_3_2, screenWidth * 480);
    // lv_disp_draw_buf_init(&draw_buf, buf_3_1, buf_3_2, screenWidth * 100);

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);
    /* Change the following line to your display resolution */
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /* Initialize the (dummy) input device driver */
    // static lv_indev_drv_t indev_drv;
    // lv_indev_drv_init(&indev_drv);
    // indev_drv.type = LV_INDEV_TYPE_POINTER;
    // // indev_drv.read_cb = my_touchpad_read;
    // lv_indev_drv_register(&indev_drv);

    // lv_demo_widgets();
    // lv_demo_benchmark();
    ui_main();
    Serial.println("Setup done");

    Serial.printf("SRAM free size: %d\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
    Serial.printf("PSRAM free size: %d\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));

    ticker.attach(1, tcr1s);
    lv_timer_handler(); /* let the GUI do its work */


  }
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
  // digitalWrite(40, HIGH);
  // digitalWrite(2, HIGH);
  // digitalWrite(1, HIGH);
  // Serial.println("HIGH");
  // delay(5000);

  // digitalWrite(40, LOW);
  // digitalWrite(2, LOW);
  // digitalWrite(1, LOW);
  // Serial.println("LOW");
  // delay(5000);
}
