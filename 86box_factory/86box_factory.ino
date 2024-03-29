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
#include "lvgl_port.h"

#if LVGL_PORT_AVOID_TEAR
IRAM_ATTR bool onRefreshFinishCallback(void *user_data)
{
    return lvgl_port_notify_rgb_vsync();
}
#endif

void setup()
{
  Serial.begin(115200);

  // while (!Serial);
  Serial.println("LVGL Widgets Demo");

  ESP_Panel *panel = new ESP_Panel();
  panel->init();
  ESP_PanelBus_RGB *rgb_bus = static_cast<ESP_PanelBus_RGB *>(panel->getLcd()->getBus());
  rgb_bus->configRgbFrameBufferNumber(LVGL_PORT_DISP_BUFFER_NUM);
  panel->begin();

  Serial.println("Initialize LVGL");
  lvgl_port_init(panel->getLcd(), panel->getTouch());
#if LVGL_PORT_AVOID_TEAR
  panel->getLcd()->attachRefreshFinishCallback(onRefreshFinishCallback, NULL);
#endif

  lvgl_port_lock(-1);

  // lv_demo_widgets();
  // lv_demo_benchmark();
  ui_main();

  lvgl_port_unlock();
}

void loop()
{
  delay(1000);
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
