#ifndef psframebuffer_h
#define psframebuffer_h

#ifdef PSFBUFFER
#include <Adafruit_GFX.h>
#include <string.h>
// 中文位图字体（10x10 与 20x20）
#include "../fonts/NotoSans_10x10.h"
#define CH_FONT_W_10_10 CHINESE_FONT_10_WIDTH
#define CH_FONT_H_10_10 CHINESE_FONT_10_HEIGHT
#define FIND_CH_10   findChineseChar_10

#include "../fonts/NotoSans_20x20.h"
#define CH_FONT_W CHINESE_FONT_20_WIDTH
#define CH_FONT_H CHINESE_FONT_20_HEIGHT
#define FIND_CH   findChineseChar_20

class  psFrameBuffer : public Adafruit_GFX {
  public:
    psFrameBuffer(int16_t w, int16_t h):Adafruit_GFX(w, h){ setTextWrap(false); cp437(true); }
    ~psFrameBuffer(){ freeBuffer(); }
    bool ready() { return _ready; }
    
    // 覆盖 write 以支持 UTF-8（ASCII 仍走基类）
    virtual size_t write(uint8_t c){
      return Adafruit_GFX::write(c);
    }

    virtual size_t write(const uint8_t *buffer, size_t size){
      size_t bytesProcessed = 0;
      for (size_t i = 0; i < size; ) {
        uint8_t byte1 = buffer[i];
        if (byte1 < 0x80) {
          Adafruit_GFX::write(byte1);
          bytesProcessed++; i++;
        } else if ((byte1 & 0xE0) == 0xC0 && i + 1 < size) {
          // 2字节：保持基类逐字写（用于西里尔等）
          Adafruit_GFX::write(byte1);
          Adafruit_GFX::write(buffer[i + 1]);
          bytesProcessed += 2; i += 2;
        } else if ((byte1 & 0xF0) == 0xE0 && i + 2 < size) {
          // 3字节（中文）
          uint8_t byte2 = buffer[i + 1];
          uint8_t byte3 = buffer[i + 2];
          if ((byte2 & 0xC0) == 0x80 && (byte3 & 0xC0) == 0x80) {
            uint32_t unicode = ((byte1 & 0x0F) << 12) | ((byte2 & 0x3F) << 6) | (byte3 & 0x3F);
            drawChineseChar(cursor_x, cursor_y, unicode, textcolor);
            bytesProcessed += 3; i += 3;
          } else {
            // 无效 UTF-8 序列，跳过
            i++;
          }
        } else {
          // 其他情况跳过一个字节
          i++;
        }
      }
      return bytesProcessed;
    }
    
    void freeBuffer(){
      _ready = false;
      if(buffer) {
        _dspl->fillRect(_ll, _tt, _ww, _hh, _bgcolor);
        free(buffer);
      }
      buffer = nullptr;
    }
    bool begin(yoDisplay *dspl, int16_t l, int16_t t, int16_t w, int16_t h, uint16_t bgcolor = 0){
      _dspl = dspl; _ll = l; _tt = t; _ww = w; _hh = h; _bgcolor = bgcolor;
      _createBuffer();
      return _ready;
    }
    void move(int16_t l, int16_t t, int16_t w, int16_t h){
      _ll = l; _tt = t; _ww = w; _hh = h;
      freeBuffer();
      _createBuffer();
    }
    void drawPixel( int16_t x, int16_t y, uint16_t color){
      if (x < 0 || x >= _ww || y < 0 || y >= _hh) return;
      if(!buffer) return;
      buffer[x + y * _ww] = color;
    }
    
    // 在帧缓冲中按位图绘制中文字符
    void drawChineseChar(int16_t x, int16_t y, uint32_t unicode, uint16_t color){
      uint8_t current_textsize = textsize_x ? textsize_x : 1;
      const uint8_t* bitmap = nullptr;
      int16_t glyphW = 0, glyphH = 0;
      volatile uint32_t safe_unicode = unicode;

      if (current_textsize == 1) {
        const ChineseCharMapping_10* charData = FIND_CH_10(safe_unicode);
        if (!charData) { drawRect(x, y, 10, 10, color); cursor_x += 10; return; }
        bitmap = charData->bitmap; glyphW = CH_FONT_W_10_10; glyphH = CH_FONT_H_10_10; y -= 1;
      } else {
        const ChineseCharMapping_20* charData = FIND_CH(safe_unicode);
        if (!charData) { drawRect(x, y, 20, 20, color); cursor_x += 20; return; }
        bitmap = charData->bitmap; glyphW = CH_FONT_W; glyphH = CH_FONT_H; y -= 1;
      }

      if (!bitmap) { drawRect(x, y, glyphW, glyphH, color); cursor_x += glyphW; return; }

      const int bytesPerRow = (glyphW + 7) >> 3;
      const int totalBytes  = bytesPerRow * glyphH;
      uint8_t localBuf[64];
      int copyBytes = totalBytes <= (int)sizeof(localBuf) ? totalBytes : (int)sizeof(localBuf);
      memcpy(localBuf, bitmap, copyBytes);

      if (textbgcolor != textcolor) {
        fillRect(x, y, glyphW, glyphH, textbgcolor);
      }
      for (int row = 0; row < glyphH; ++row) {
        const uint8_t* rowPtr = &localBuf[row * bytesPerRow];
        uint8_t byte = 0;
        for (int col = 0; col < glyphW; ++col) {
          if ((col & 7) == 0) byte = rowPtr[col >> 3];
          if (byte & (0x80 >> (col & 7))) {
            drawPixel(x + col, y + row, color);
          }
        }
        if ((row & 7) == 7) { yield(); }
      }

      cursor_x += glyphW;
      // 使用帧缓冲的宽度判断换行，避免使用 Adafruit_GFX 基类构造时的全屏宽度
      if (wrap && (cursor_x + glyphW > _ww)) { cursor_x = 0; cursor_y += glyphH; }
    }
    void display(){
      if(!buffer) return;
      _dspl->startWrite();
      _dspl->setAddrWindow(_ll, _tt, _ww, _hh);
      _dspl->writePixels((uint16_t*)buffer,  _ww * _hh);
      _dspl->endWrite();
    }
    void clear(){
      if(!buffer) return;
      fillRect(0, 0, _ww, _hh, _bgcolor);
    }
    int16_t width(){ return _ww; }
    int16_t height(){ return _hh; }
  private:
    int16_t _ll, _tt, _ww, _hh;
    yoDisplay *_dspl;
    uint16_t *buffer=nullptr;
    bool _ready = false;
    uint16_t _bgcolor;
    void _createBuffer(){
      if(USE_FBUFFER){
        if(psramInit())
          buffer = (uint16_t*) ps_calloc(_hh * _ww, sizeof(uint16_t));
        #ifdef SFBUFFER
        else
          buffer = (uint16_t*) calloc(_hh * _ww, sizeof(uint16_t));
        #endif
      }
      if(buffer){
        for (int i = 0; i < _hh * _ww; i++)
          buffer[i] = _bgcolor;
        _ready = true;
      }
    }
};
#else
struct psFrameBuffer {
  bool ready() { return false; }
  void display() {}
};
#endif //#ifdef PSFBUFFER


#endif
