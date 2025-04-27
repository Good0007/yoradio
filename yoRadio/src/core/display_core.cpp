#include "display_core.h"

LGFX_Sprite sp(&dsp);
#define FONT16 &fonts::efontCN_16

DspCore::DspCore(){
  {
        // Get the structure for bus configuration.
        auto cfg = _bus_instance.config();
    
        // SPI bus settings
        cfg.spi_host = SPI2_HOST; // Select the SPI to use ESP32-S2,C3 : SPI2_HOST
                                    // or SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
        // * With the ESP-IDF version upgrade, VSPI_HOST and HSPI_HOST
        // descriptions are deprecated, so if an error occurs, use SPI2_HOST and
        // SPI3_HOST instead.
        cfg.spi_mode = 3;          // Set SPI communication mode (0 ~ 3)
        cfg.freq_write = 79000000; // SPI clock when sending (up to 80MHz, rounded
                                    // to 80MHz divided by an integer)
        cfg.freq_read = 6000000;   // SPI clock when receiving
        cfg.spi_3wire = false;     // set to true if receiving on MOSI pin
        cfg.use_lock = true;       // set to true to use transaction lock
        // Set the DMA channel to use (0=not use DMA / 1=1ch
        cfg.dma_channel = SPI_DMA_CH_AUTO;
        // / 2=ch / SPI_DMA_CH_AUTO=auto setting)
        // * With the ESP-IDF version upgrade, SPI_DMA_CH_AUTO (automatic setting)
        // is recommended for the DMA channel. Specifying 1ch and 2ch is
        // deprecated.
        cfg.pin_sclk = 8;  // set SPI SCLK pin number
        cfg.pin_mosi = 18;  // Set MOSI pin number for SPI
        cfg.pin_miso = -1; // set SPI MISO pin number (-1 = disable)
        cfg.pin_dc = 16;    // Set SPI D/C pin number (-1 = disable)
    
        _bus_instance.config(cfg);              // Apply the settings to the bus.
        _panel_instance.setBus(&_bus_instance); // Sets the bus to the panel.
        }
    
        { // Set display panel control.
        // Get the structure for display panel settings.
        auto cfg = _panel_instance.config();
    
        cfg.pin_cs = -1;    // Pin number to which CS is connected (-1 = disable)
        cfg.pin_rst = 17;  // pin number where RST is connected (-1 = disable)
        cfg.pin_busy = -1; // pin number to which BUSY is connected (-1 = disable)
    
        // * The following setting values ​​are set to general default values
        // ​​for each panel, and the pin number (-1 = disable) to which BUSY
        // is connected, so please try commenting out any unknown items.
    
        cfg.memory_width = 240;  // Maximum width supported by driver IC
        cfg.memory_height = 240; // Maximum height supported by driver IC
        cfg.panel_width = 240;   // actual displayable width
        cfg.panel_height = 240;  // actual displayable height
        cfg.offset_x = 0;        // Panel offset in X direction
        cfg.offset_y = -80;        // Panel offset in Y direction
        cfg.offset_rotation = 3;
        cfg.dummy_read_pixel = 8;
        cfg.dummy_read_bits = 1;
        cfg.readable = false;
        cfg.invert = true;
        cfg.rgb_order = false;
        cfg.dlen_16bit = false;
        cfg.bus_shared = false;
    
        _panel_instance.config(cfg);
        }
    
        { // Set backlight control. (delete if not necessary)
        // Get the structure for backlight configuration.
        auto cfg = _light_instance.config();
    
        cfg.pin_bl = 15;      // pin number to which the backlight is connected
        cfg.invert = false;  // true to invert backlight brightness
        cfg.freq = 44100;    // backlight PWM frequency
        cfg.pwm_channel = 1; // PWM channel number to use
    
        _light_instance.config(cfg);
        // Sets the backlight to the panel.
        _panel_instance.setLight(&_light_instance);
        }
    
        setPanel(&_panel_instance); // Sets the panel to use.
};

void DspCore::initDisplay() {
  dsp.init();
  dsp.setBrightness(60);
  dsp.setFont(FONT16);
  dsp.setColorDepth(8);
  dsp.fillScreen(TFT_BLACK);
  //sp.setFont(FONT16);
  //sp.setColorDepth(8);
  //this->begin();
  //this->fillScreen(TFT_RED);      // 先填充红色
  //this->setTextColor(TFT_WHITE, TFT_RED);
  //this->setTextSize(2);
  //this->setCursor(10, 10);
  //this->print("Hello, yoRadio!"); // 打印一句话
  //delay(1000);        
  //延时一段时间，便于观察
}


void DspCore::drawLogo(uint16_t top) {
   //drawRGBBitmap((width() - 99) / 2, top, bootlogo2, 99, 64); 
   // bootlogo2 必须是 RGB565 格式的 uint16_t 数组
   pushImage((width() - 99) / 2, top, 99, 64, bootlogo2);
}

void DspCore::printPLitem(uint8_t pos, const char* item, ScrollWidget& current){
  setTextSize(playlistConf.widget.textsize);
  if (pos == plCurrentPos) {
    current.setText(item);
  } else {
    uint8_t plColor = (abs(pos - plCurrentPos)-1)>4?4:abs(pos - plCurrentPos)-1;
    setTextColor(config.theme.playlist[plColor], config.theme.background);
    setCursor(TFT_FRAMEWDT, plYStart + pos * plItemHeight);
    fillRect(0, plYStart + pos * plItemHeight - 1, width(), plItemHeight - 2, config.theme.background);
    print(utf8Rus(item, true));
  }
}

void DspCore::drawPlaylist(uint16_t currentItem) {
  uint8_t lastPos = config.fillPlMenu(currentItem - plCurrentPos, plTtemsCount);
  if(lastPos<plTtemsCount){
    fillRect(0, lastPos*plItemHeight+plYStart, width(), height()/2, config.theme.background);
  }
}

void DspCore::clearDsp(bool black) { fillScreen(black?0:config.theme.background); }

GFXglyph *pgm_read_glyph_ptr(const GFXfont *gfxFont, uint8_t c) {
  return gfxFont->glyph + c;
}

uint8_t DspCore::_charWidth(unsigned char c){
  GFXglyph *glyph = pgm_read_glyph_ptr(&DS_DIGI42pt7b, c - 0x20);
  return pgm_read_byte(&glyph->xAdvance);
}

uint16_t DspCore::textWidth(const char *txt){
  uint16_t w = 0, l=strlen(txt);
  for(uint16_t c=0;c<l;c++) w+=_charWidth(txt[c]);
  return w;
}

void DspCore::_getTimeBounds() {
  _timewidth = textWidth(_timeBuf);
  char buf[4];
  strftime(buf, 4, "%H", &network.timeinfo);
  _dotsLeft=textWidth(buf);
}

const char* DspCore::utf8Rus(const char* str, bool) {
  return str; // 如需转码可自行实现
}

void DspCore::setTextSize(uint8_t size) {
  LGFX_Device::setTextSize(size);
}

void DspCore::setTextSize(uint8_t sx, uint8_t sy) {
  LGFX_Device::setTextSize(sx, sy);
}


void DspCore::_clockSeconds(){
  setTextSize(4);
  setTextColor(config.theme.seconds, config.theme.background);
  setCursor(width() - 8 - clockRightSpace - CHARWIDTH*4*2, clockTop-clockTimeHeight+1);
  sprintf(_bufforseconds, "%02d", network.timeinfo.tm_sec);
  if(!config.isScreensaver) dsp.print(_bufforseconds);
  setTextSize(1);
  setFont(&DS_DIGI42pt7b);
  setTextColor((network.timeinfo.tm_sec % 2 == 0) ? config.theme.clock : (CLOCKFONT_MONO?config.theme.clockbg:config.theme.background), config.theme.background);
  setCursor(_timeleft+_dotsLeft, clockTop);
  print(":");
  setFont(nullptr);
}

void DspCore::_clockDate(){
  if(_olddateleft>0)
  LGFX_Device::fillRect(_olddateleft,  clockTop+14, _olddatewidth, CHARHEIGHT*2, config.theme.background);
  setTextColor(config.theme.date, config.theme.background);
  setCursor(_dateleft, clockTop+15);
  setTextSize(2);
  if(!config.isScreensaver) print(_dateBuf);                                            /* print date */
  strlcpy(_oldDateBuf, _dateBuf, sizeof(_dateBuf));
  _olddatewidth = _datewidth;
  _olddateleft = _dateleft;
  setTextSize(4);
  setTextColor(config.theme.dow, config.theme.background);
  setCursor(width() - 8 - clockRightSpace - CHARWIDTH*4*2, clockTop-CHARHEIGHT*4+4);
  print(utf8Rus(dow[network.timeinfo.tm_wday], false));       /* print dow */
}

void DspCore::_clockTime(){
  if(_oldtimeleft>0 && !CLOCKFONT_MONO) fillRect(_oldtimeleft, clockTop-clockTimeHeight+1, _oldtimewidth, clockTimeHeight, config.theme.background);
  _timeleft = width()-clockRightSpace-CHARWIDTH*4*2-24-_timewidth;
  setTextSize(1);
  setFont(&DS_DIGI56pt7b);
  
  if(CLOCKFONT_MONO) {
    setCursor(_timeleft, clockTop);
    setTextColor(config.theme.clockbg, config.theme.background);
    print("88:88");
  }
  setTextColor(config.theme.clock, config.theme.background);
  setCursor(_timeleft, clockTop);
  print(_timeBuf);
  setFont(nullptr);
  strlcpy(_oldTimeBuf, _timeBuf, sizeof(_timeBuf));
  _oldtimewidth = _timewidth;
  _oldtimeleft = _timeleft;
  if(!config.isScreensaver) drawFastVLine(width()-clockRightSpace-CHARWIDTH*4*2-18, clockTop-clockTimeHeight, clockTimeHeight+4, config.theme.div);  /*divider vert*/
  if(!config.isScreensaver) drawFastHLine(width()-clockRightSpace-CHARWIDTH*4*2-18, clockTop-clockTimeHeight+37, 59, config.theme.div);              /*divider hor*/
  sprintf(_buffordate, "%2d %s %d", network.timeinfo.tm_mday,mnths[network.timeinfo.tm_mon], network.timeinfo.tm_year+1900);
  strlcpy(_dateBuf, utf8Rus(_buffordate, true), sizeof(_dateBuf));
  _datewidth = strlen(_dateBuf) * CHARWIDTH*2;
  _dateleft = width() - 10 - clockRightSpace - _datewidth;
}

void DspCore::printClock(uint16_t top, uint16_t rightspace, uint16_t timeheight, bool redraw){
  clockTop = top;
  clockRightSpace = rightspace;
  clockTimeHeight = timeheight;
  strftime(_timeBuf, sizeof(_timeBuf), "%H:%M", &network.timeinfo);
  if(strcmp(_oldTimeBuf, _timeBuf)!=0 || redraw){
    _getTimeBounds();
    _clockTime();
    if(!config.isScreensaver)
      if(strcmp(_oldDateBuf, _dateBuf)!=0 || redraw) _clockDate();
  }
  _clockSeconds();
}

void DspCore::clearClock(){
  LGFX_Device::fillRect(_timeleft,  clockTop-clockTimeHeight, _timewidth+CHARWIDTH*3*2+24, clockTimeHeight+12+CHARHEIGHT, config.theme.background);
}

void DspCore::startWrite(void) {
  LGFX_Device::startWrite();
}

void DspCore::endWrite(void) {
  LGFX_Device::endWrite();
}

void DspCore::loop(bool force) {
}

void DspCore::charSize(uint8_t textsize, uint8_t& width, uint16_t& height){
  width = textsize * CHARWIDTH;
  height = textsize * CHARHEIGHT;
}


void DspCore::flip(){
  #if DSP_MODEL==DSP_ST7789
  setRotation(config.store.flipscreen?3:1);
  #endif
  #if DSP_MODEL==DSP_ST7789_240
    if(ROTATE_90){
      setRotation(config.store.flipscreen?3:1);
    }else{
      setRotation(config.store.flipscreen?2:0);
    }
  #endif
}

void DspCore::invert(){
  invertDisplay(config.store.invertdisplay);
}


void DspCore::writePixel(int16_t x, int16_t y, uint16_t color) {
  if(_clipping){
    if ((x < _cliparea.left) || (x > _cliparea.left+_cliparea.width) || (y < _cliparea.top) || (y > _cliparea.top + _cliparea.height)) return;
  }
  //Adafruit_ST7796S_kbv::writePixel(x, y, color);
  // 调用基础类的 writePixel 方法
  LGFX_Device::writePixel(x, y, color);
}

void DspCore::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  if(_clipping){
    if ((x < _cliparea.left) || (x >= _cliparea.left+_cliparea.width) || (y < _cliparea.top) || (y > _cliparea.top + _cliparea.height))  return;
  }
  LGFX_Device::writeFillRect(x, y, w, h, color);
}

void DspCore::setClipping(clipArea ca){
  _cliparea = ca;
  _clipping = true;
}

void DspCore::clearClipping(){
  _clipping = false;
}

void DspCore::setNumFont(){
  setFont(&DS_DIGI56pt7b);
  setTextSize(1);
}