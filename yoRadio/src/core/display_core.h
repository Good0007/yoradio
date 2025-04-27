#include <LovyanGFX.hpp>
#include "options.h"
typedef LGFX_Sprite* Canvas;
#include "displays/widgets/widgets.h"
#include "displays/widgets/pages.h"

#include "displays/fonts/bootlogo.h"
#include "displays/conf/displayST7789_240conf.h"
#include "displays/fonts/DS_DIGI42pt7b_mono.h"
#include "displays/fonts/DS_DIGI56pt7b_mono.h"
#include "displays/tools/l10n.h"
#include "config.h"
#include "network.h"

#define BOOT_PRG_COLOR    0xE68B
#define BOOT_TXT_COLOR    0xFFFF
#define PINK              0xF97F


#define CHARWIDTH   6
#define CHARHEIGHT  8

class DspCore : public lgfx::LGFX_Device {
    lgfx::Panel_ST7789 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    lgfx::Light_PWM _light_instance;

    public:
    uint16_t plItemHeight, plTtemsCount, plCurrentPos;
    int plYStart;

public:
DspCore();

void initDisplay();

void drawLogo(uint16_t top);
void clearDsp(bool black=false);
void printClock(){}
void printClock(uint16_t top, uint16_t rightspace, uint16_t timeheight, bool redraw);
void clearClock();
const char* utf8Rus(const char* str, bool uppercase);

void drawPlaylist(uint16_t currentItem);
void loop(bool force=false);
void charSize(uint8_t textsize, uint8_t& width, uint16_t& height);

uint16_t width() {
    return _panel_instance.height();
}
uint16_t height() {
    return _panel_instance.width();
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (_clipping) {
        if (x < _cliparea.left) x = _cliparea.left;
        if (y < _cliparea.top) y = _cliparea.top;
        if (x + w > _cliparea.left + _cliparea.width) w = _cliparea.left + _cliparea.width - x;
        if (y + h > _cliparea.top + _cliparea.height) h = _cliparea.top + _cliparea.height - y;
    }
    LGFX_Device::fillRect(x, y, w, h, color);
}
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){}
void setTextSize(uint8_t s);
void setTextSize(uint8_t sx, uint8_t sy);
void setTextColor(uint16_t c, uint16_t bg){}
void apScreen();

void startWrite();
void endWrite();

void flip();
void invert();
void sleep() {
    //调用父类的sleep函数
    lgfx::LGFX_Device::sleep();
    //关闭背光
    _light_instance.setBrightness(0);
}
void wake() {
    //调用父类的wakeup函数
    lgfx::LGFX_Device::wakeup();
    //打开背光
    _light_instance.setBrightness(255);
}
void writePixel(int16_t x, int16_t y, uint16_t color);
void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void setClipping(clipArea ca);
void clearClipping();
void setScrollId(void * scrollid) { _scrollid = scrollid; }
void * getScrollId() { return _scrollid; }
void setNumFont();
uint16_t textWidth(const char *txt);

void printPLitem(uint8_t pos, const char* item, ScrollWidget& current);
private:
char  _timeBuf[20], _dateBuf[20], _oldTimeBuf[20], _oldDateBuf[20], _bufforseconds[4], _buffordate[40];
uint16_t _timewidth, _timeleft, _datewidth, _dateleft, _oldtimeleft, _oldtimewidth, _olddateleft, _olddatewidth, clockTop, clockRightSpace, clockTimeHeight, _dotsLeft;
bool _clipping, _printdots;
clipArea _cliparea;
void * _scrollid;
void _getTimeBounds();
void _clockSeconds();
void _clockDate();
void _clockTime();
uint8_t _charWidth(unsigned char c);
 
};
    
extern DspCore dsp;
    