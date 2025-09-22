#ifndef displayST7789_h
#define displayST7789_h

#include "Arduino.h"
#include <Adafruit_GFX.h>

#if DSP_MODEL==DSP_ST7789_76
  #include "fonts/bootlogo62x40.h"
  #include "fonts/dsfont35.h"
#else
  #include "fonts/bootlogo99x64.h"
  #include "fonts/dsfont52.h"
#endif

typedef GFXcanvas16 Canvas;

// 自定义 yoDisplay 基类，继承自 Adafruit_GFX
class yoDisplay : public Adafruit_GFX {
  public:
    yoDisplay(int16_t w, int16_t h) : Adafruit_GFX(w, h) {}
    virtual void setAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h) {}
    virtual void writePixels(uint16_t *colors, uint32_t len) {}
};

#include "tools/commongfx.h"

#if __has_include("conf/displayST7789conf_custom.h")
  #include "conf/displayST7789conf_custom.h"
#else
  #if DSP_MODEL==DSP_ST7789
    #include "conf/displayST7789conf.h"
  #elif DSP_MODEL==DSP_ST7789_170
    #include "conf/displayST7789_170conf.h"
  #elif DSP_MODEL==DSP_ST7789_76
    #include "conf/displayST7789_76conf.h"
  #else
    #include "conf/displayST7789_240conf.h"
  #endif
#endif

#endif
