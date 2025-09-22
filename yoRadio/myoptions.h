#ifndef myoptions_h
#define myoptions_h

/*******************************************************
ESP32-S3 Configuration - DEBUG VERSION
添加调试功能以分析崩溃原因
********************************************************/

/*        调试设置                 */
#define DEBUG_MEMORY_MONITORING false      /* 关闭内存监控减少开销 */
#define DEBUG_NETWORK_EVENTS false         /* 关闭网络调试减少开销 */
#define DEBUG_DISPLAY_INIT false           /* 关闭显示调试减少开销 */

/*        稳定性优化设置           */
#define CONFIG_AUTOCONNECT_TIMEOUT 30      /* WiFi重连超时 */
#define CONFIG_TASK_WDT_TIMEOUT 10         /* 看门狗超时增加 */

/*        网络优化设置             */
#define WIFI_RECONNECT_DELAY 5000          /* WiFi重连延迟(ms) */
#define HTTP_UPLOAD_TIMEOUT 30000          /* 文件上传超时(ms) */
#define NETSERVER_UPLOAD_TIMEOUT 60000     /* 网络服务器上传超时 */

/*        基本设置                 */
#define LED_INVERT      false               
#define L10N_LANGUAGE   CN                  /* 使用中文语言包 */
#define INTEGRATION_MODE true               /* 集成模式开关 - 设为true启用长按中心键切换系统 */

/*        时区设置 - 北京时间（东八区）  */
#define DEFAULT_TIMEZONE_HOUR   8           /* UTC+8 北京时间 */
#define DEFAULT_TIMEZONE_MIN    0           /* 时区分钟偏移 */

/*        显示设置 - 临时禁用测试        */
//##############################################################################
//####  DISPLAY - 7PIN ST7789 (CS接地)
//##############################################################################
#define DSP_MODEL          DSP_ST7789_240
//#define DSP_MODEL          DSP_DUMMY

/*        SPI显示屏引脚配置 - 7PIN ST7789 CS接地        */
#define TFT_CS            -1                /* CS接地，不使用 */
#define TFT_DC            16                /* Data Command control pin */
#define TFT_RST           17                /* Reset pin */
#define TFT_MOSI          18                /* Data out */
#define TFT_SCLK          8                 /* Clock out */
#define TFT_MISO          -1                /* ST7789单向通信，不使用 */
#define TFT_BL            15                /* Backlight pin */
#define TFT_LED           TFT_BL

/*        显示屏配置               */
#define BRIGHTNESS_PIN    15                /* 背光控制 */
#define ROTATE_90         true              /* 显示旋转 */          

/*        音频设置 - I2S直接输出到MAX98357A */
#define I2S_DOUT          5                 /* I2S数据输出到MAX98357A的DIN */
#define I2S_BCLK          6                 /* I2S位时钟到MAX98357A的BCLK */
#define I2S_LRC           7                 /* I2S帧时钟到MAX98357A的LRC */
#define I2S_INTERNAL      false             /* 使用外部I2S DAC (MAX98357A) */
#define PLAYER_FORCE_MONO false             /* MAX98357A支持立体声 */

/*        按键设置 - 3个物理按键配置    */
#define BTN_MODE          255                /* 主功能按键 - 模式切换 */
#define BTN_INTERNALPULLUP true             
#define BTN_LEFT          255                 /* 左按键 - 上一电台/音量减 */
#define BTN_RIGHT         255                /* 右按键 - 下一电台/音量加 */
#define BTN_UP            9                 /* 复用左按键作为UP功能 */
#define BTN_DOWN          21                /* 复用右按键作为DOWN功能 */
#define BTN_CENTER        14               /* 中心按键 - 禁用（无硬件） */
#define USE_CH_FONT_16   true             /* 使用16x16中文字体 */
/*        按键功能详细说明 - 优化的三键布局           */
/* 
 * BTN_CENTER (GPIO 14) - 中心按键:
 *   - 短按: 播放/暂停
 *   - 长按: 进入/退出播放列表
 *   - 双击: 静音/取消静音
 * 
 * BTN_UP (GPIO 9) - 上按键: 
 *   - 短按: 音量+
 *   - 长按: 上一曲
 *   - 双击: 进入播放列表
 * 
 * BTN_DOWN (GPIO 21) - 下按键:
 *   - 短按: 音量-
 *   - 长按: 下一曲
 *   - 双击: 进入播放列表
 */

/*        禁用所有可选功能和按键      */
#define VS1053_CS         255              /* VS1053芯片CS - 完全禁用 */
#define VS1053_DCS        255              /* VS1053芯片DCS - 完全禁用 */
#define VS1053_DREQ       255              /* VS1053芯片DREQ - 完全禁用 */
#define VS1053_RST        255              /* VS1053芯片RST - 完全禁用 */
#define ENC_BTNL          255              /* 编码器左按钮 - 完全禁用 */
#define ENC_BTNB          255              /* 编码器中按钮 - 完全禁用 */
#define ENC_BTNR          255              /* 编码器右按钮 - 完全禁用 */
#define ENC2_BTNL         255              /* 第二编码器左按钮 - 完全禁用 */
#define ENC2_BTNB         255              /* 第二编码器中按钮 - 完全禁用 */
#define ENC2_BTNR         255              /* 第二编码器右按钮 - 完全禁用 */
//#define BTN_UP            255              /* 上按键 - 使用GPIO 9 */
//#define BTN_DOWN          255              /* 下按键 - 使用GPIO 21 */
#define SDC_CS            255              /* SD卡 - 完全禁用 */
#define TS_MODEL          TS_MODEL_UNDEFINED
#define TS_CS             255              /* 触摸屏 - 完全禁用 */
#define MUTE_PIN          255              /* 静音控制 - 完全禁用 */
#define WAKE_PIN          255              /* 唤醒控制 - 完全禁用 */
#define LIGHT_SENSOR      255              /* 光感 - 完全禁用 */
#define IR_PIN            255              /* 红外 - 完全禁用 */
#define DSP_INVERT_TITLE  false             

#endif