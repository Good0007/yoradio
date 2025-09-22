#include "../core/options.h"
#if DSP_MODEL==DSP_ST7789 || DSP_MODEL==DSP_ST7789_240 || DSP_MODEL==DSP_ST7789_76 || DSP_MODEL==DSP_ST7789_170
#include "dspcore.h"
#include "../core/config.h"

// ST7789 7PIN 显示驱动 - ESP32-S3优化版本
// ST7789 命令定义
#define ST7789_SWRESET  0x01
#define ST7789_SLPOUT   0x11
#define ST7789_NORON    0x13
#define ST7789_MADCTL   0x36
#define ST7789_COLMOD   0x3A
#define ST7789_CASET    0x2A
#define ST7789_RASET    0x2B
#define ST7789_RAMWR    0x2C
#define ST7789_DISPON   0x29
#define ST7789_INVON    0x21
#define ST7789_INVOFF   0x20

// 全局SPI设置标志
static bool spiInitialized = false;

// CS接地配置 - 继承自yoDisplay，尺寸240x240
DspCore::DspCore(): yoDisplay(240, 240) {}

// 发送命令到ST7789
void writeCommand(uint8_t cmd) {
    digitalWrite(TFT_DC, LOW);   // 命令模式
    SPI.transfer(cmd);
}

// 发送数据到ST7789
void writeData(uint8_t data) {
    digitalWrite(TFT_DC, HIGH);  // 数据模式
    SPI.transfer(data);
}

// 发送16位数据 - 优化版本
void writeData16(uint16_t data) {
    digitalWrite(TFT_DC, HIGH);  // 数据模式
    uint8_t buffer[2] = {(uint8_t)(data >> 8), (uint8_t)(data & 0xFF)};
    SPI.writeBytes(buffer, 2);
}

// 批量发送16位数据
void writeData16Array(uint16_t *data, uint32_t len) {
    digitalWrite(TFT_DC, HIGH);  // 数据模式
    SPI.writePixels(data, len * 2);
}

// 设置显示窗口
void setDisplayWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    // 设置列地址
    writeCommand(ST7789_CASET);
    writeData16(x1);
    writeData16(x2);
    
    // 设置行地址
    writeCommand(ST7789_RASET);
    writeData16(y1);
    writeData16(y2);
    
    // 准备写入RAM
    writeCommand(ST7789_RAMWR);
}

void DspCore::initDisplay() {
    Serial.println("##[DISPLAY]## ST7789 initialization starting...");
    
    if (!spiInitialized) {
        // 初始化背光
        pinMode(TFT_BL, OUTPUT);
        digitalWrite(TFT_BL, HIGH);  // 打开背光
        
        // 初始化控制引脚
        pinMode(TFT_DC, OUTPUT);
        pinMode(TFT_RST, OUTPUT);
        
        // 初始化SPI
        SPI.begin(TFT_SCLK, -1, TFT_MOSI, -1);
        SPI.setFrequency(10000000);  // 10MHz - 稳定工作频率
        SPI.setDataMode(SPI_MODE3);
        SPI.setBitOrder(SPI_MSBFIRST);
        
        // 复位显示屏
        digitalWrite(TFT_RST, LOW);
        delay(100);
        digitalWrite(TFT_RST, HIGH);
        delay(100);
        
        // 初始化ST7789
        // 软件复位
        writeCommand(ST7789_SWRESET);
        delay(150);
        
        // 退出睡眠模式
        writeCommand(ST7789_SLPOUT);
        delay(10);
        
        // 设置颜色模式为16位
        writeCommand(ST7789_COLMOD);
        writeData(0x55);  // 16-bit color
        delay(10);
        
        // 设置显示方向
        writeCommand(ST7789_MADCTL);
        writeData(0x00);  // 默认方向
        
        // 正常显示模式
        writeCommand(ST7789_NORON);
        delay(10);
        
        // 打开显示
        writeCommand(ST7789_DISPON);
        delay(100);
        
        spiInitialized = true;
        Serial.println("##[DISPLAY]## ST7789 initialized successfully");
    }
    
    // 清屏为黑色，准备使用
    fillScreen(0x0000);  // 黑色
}

// 优化的drawPixel实现
void DspCore::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (x < 0 || x >= 240 || y < 0 || y >= 240) return;
    
    // 直接使用批量传输优化
    setDisplayWindow(x, y, x, y);
    
    // 使用优化的16位数据发送
    digitalWrite(TFT_DC, HIGH);
    uint8_t buffer[2] = {(uint8_t)(color >> 8), (uint8_t)(color & 0xFF)};
    SPI.writeBytes(buffer, 2);
}

void DspCore::fillScreen(uint16_t color) {
    setDisplayWindow(0, 0, 239, 239);
    
    digitalWrite(TFT_DC, HIGH);  // 数据模式
    
    // 使用批量传输优化性能
    uint8_t colorBytes[2] = {(uint8_t)(color >> 8), (uint8_t)(color & 0xFF)};
    
    // 分块传输，避免看门狗超时
    for (int block = 0; block < 240; block++) {
        for (int i = 0; i < 240; i++) {
            SPI.writeBytes(colorBytes, 2);
        }
        // 每行后给看门狗一点时间
        if (block % 10 == 0) {
            yield();  // 让系统处理其他任务
        }
    }
}

void DspCore::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (x >= 240 || y >= 240 || w <= 0 || h <= 0) return;
    
    // 裁剪到屏幕边界
    if (x + w > 240) w = 240 - x;
    if (y + h > 240) h = 240 - y;
    
    setDisplayWindow(x, y, x + w - 1, y + h - 1);
    
    digitalWrite(TFT_DC, HIGH);  // 数据模式
    
    // 优化的批量填充，添加yield避免看门狗超时
    uint8_t colorBytes[2] = {(uint8_t)(color >> 8), (uint8_t)(color & 0xFF)};
    uint32_t totalPixels = w * h;
    
    for(uint32_t i = 0; i < totalPixels; i++) {
        SPI.writeBytes(colorBytes, 2);
        // 每100个像素让出CPU时间
        if (i % 100 == 0 && i > 0) {
            yield();
        }
    }
}

void DspCore::clearDsp(bool black) { 
    fillScreen(black ? 0x0000 : config.theme.background); 
}

// 重写write方法以支持中文UTF-8
size_t DspCore::write(uint8_t c) {
    // 单字节处理，主要用于ASCII字符
    return Adafruit_GFX::write(c);
}

size_t DspCore::write(const uint8_t *buffer, size_t size) {
    // 处理UTF-8字符串
    size_t bytesProcessed = 0;
    
    for (size_t i = 0; i < size; ) {
        uint8_t byte1 = buffer[i];
        
        if (byte1 < 0x80) {
            // ASCII字符，使用原有方法
            Adafruit_GFX::write(byte1);
            bytesProcessed++;
            i++;
        } else if ((byte1 & 0xE0) == 0xC0 && i + 1 < size) {
            // 2字节UTF-8 (大部分俄语字符)
            Adafruit_GFX::write(byte1);
            Adafruit_GFX::write(buffer[i + 1]);
            bytesProcessed += 2;
            i += 2;
        } else if ((byte1 & 0xF0) == 0xE0 && i + 2 < size) {
            // 3字节UTF-8 (中文字符)
            uint32_t unicode = ((byte1 & 0x0F) << 12) | 
                              ((buffer[i + 1] & 0x3F) << 6) | 
                              (buffer[i + 2] & 0x3F);
            
            // 绘制中文字符
            //drawChineseChar(getCursorX(), getCursorY(), unicode, textcolor);
            
            bytesProcessed += 3;
            i += 3;
        } else {
            // 无法处理的字符，跳过
            i++;
        }
    }
    
    return bytesProcessed;
}


void DspCore::flip() {
    // 设置旋转方向
    writeCommand(ST7789_MADCTL);
    if (config.store.flipscreen) {
        writeData(0xC0);  // 180度旋转
    } else {
        writeData(0x00);  // 正常方向
    }
}

void DspCore::invert() { 
    if (config.store.invertdisplay) {
        writeCommand(ST7789_INVON);
    } else {
        writeCommand(ST7789_INVOFF);
    }
}

void DspCore::sleep(void) { 
    writeCommand(0x10);          // 进入睡眠模式 (SLPIN)
    digitalWrite(TFT_BL, LOW);   // 关闭背光
    delay(150);
}

void DspCore::wake(void) { 
    digitalWrite(TFT_BL, HIGH);  // 打开背光
    writeCommand(ST7789_SLPOUT); // 退出睡眠模式
    delay(150);
    writeCommand(ST7789_DISPON); // 显示开启
    delay(150);
}

// 添加Adafruit_GFX兼容的方法
void DspCore::init(uint16_t width, uint16_t height, uint8_t spiMode) {
    // 这个方法由initDisplay()替代，但为兼容性保留
    initDisplay();
}

void DspCore::setRotation(uint8_t rotation) {
    writeCommand(ST7789_MADCTL);
    switch (rotation) {
        case 0:
            writeData(0x00);
            break;
        case 1:
            writeData(0x60);
            break;
        case 2:
            writeData(0xC0);
            break;
        case 3:
            writeData(0xA0);
            break;
    }
}

void DspCore::invertDisplay(bool invert) {
    if (invert) {
        writeCommand(ST7789_INVON);
    } else {
        writeCommand(ST7789_INVOFF);
    }
}

void DspCore::enableDisplay(bool enable) {
    if (enable) {
        writeCommand(ST7789_DISPON);
    } else {
        writeCommand(0x28);  // DISPOFF
    }
}

void DspCore::enableSleep(bool enable) {
    if (enable) {
        writeCommand(0x10);  // SLPIN
    } else {
        writeCommand(ST7789_SLPOUT);  // SLPOUT
    }
}

// 兼容性方法
void DspCore::cp437(bool x) { /* 不需要实现 */ }

// Adafruit_GFX兼容方法
void DspCore::setAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h) {
    // 设置窗口区域，直接调用我们的底层方法
    uint16_t x_start = x;
    uint16_t y_start = y;
    uint16_t x_end = x + w - 1;
    uint16_t y_end = y + h - 1;
    
    writeCommand(ST7789_CASET);
    writeData16(x_start);
    writeData16(x_end);
    
    writeCommand(ST7789_RASET);
    writeData16(y_start);
    writeData16(y_end);
    
    writeCommand(ST7789_RAMWR);
}

void DspCore::writePixels(uint16_t *colors, uint32_t len) {
    digitalWrite(TFT_DC, HIGH);  // 数据模式
    
    // 批量传输优化性能
    if (len > 0) {
        // 使用SPI的writePixels方法进行批量传输
        SPI.writePixels(colors, len * 2);
    }
}

#endif