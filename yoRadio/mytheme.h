#ifndef _optimized_purple_theme_h
#define _optimized_purple_theme_h

/*
    优化紫色主题 - Optimized Purple Theme
    优雅的紫色配色方案，顶部无背景色，更简洁美观
    ***********************************************************************
    *    !!! This file must be in the root directory of the sketch !!!    *
    ***********************************************************************
*/
#define ENABLE_THEME
#ifdef  ENABLE_THEME

/*----------------------------------------------------------------------------------------------------------------*/
/*       | 优化紫色主题配色    |   RGB值 (0-255)   |                                                               */
/*       | 颜色名称           |    R    G    B    |                                                               */
/*----------------------------------------------------------------------------------------------------------------*/
#define COLOR_BACKGROUND         25,  15,  35     /*  深紫色背景                                                */
#define COLOR_STATION_NAME      255, 230, 255     /*  亮白紫色电台名称（增强对比度）                            */
#define COLOR_STATION_BG         25,  15,  35     /*  电台背景与主背景相同（无背景色效果）                      */
#define COLOR_STATION_FILL       25,  15,  35     /*  电台填充背景与主背景相同（无背景色效果）                  */
#define COLOR_SNG_TITLE_1       190, 130, 255     /*  亮紫色主标题（增强亮度）                                  */
#define COLOR_SNG_TITLE_2       210, 180, 235     /*  浅紫色副标题（增强可读性）                                */
#define COLOR_WEATHER           200, 150, 255     /*  淡紫色天气信息                                            */
#define COLOR_VU_MAX            190, 130, 255     /*  VU表最大值（与主标题统一）                                */
#define COLOR_VU_MIN             80,  50, 100     /*  VU表最小值（深紫灰）                                      */
#define COLOR_CLOCK             200, 150, 255     /*  淡紫色时钟                                                */
#define COLOR_CLOCK_BG           25,  15,  35     /*  时钟背景与主背景相同                                      */
#define COLOR_SECONDS           190, 130, 255     /*  亮紫色秒数（与主标题统一）                                */
#define COLOR_DAY_OF_W          255, 230, 255     /*  亮白紫色星期（与电台名称统一）                            */
#define COLOR_DATE              220, 190, 240     /*  浅紫色日期（增强亮度）                                    */
#define COLOR_HEAP              120,  80, 140     /*  深紫灰色堆信息                                            */
#define COLOR_BUFFER            160, 110, 210     /*  紫色缓冲区线（增强亮度）                                  */
#define COLOR_IP                180, 150, 210     /*  浅紫色IP地址（增强可读性）                                */
#define COLOR_VOLUME_VALUE      210, 180, 230     /*  浅紫色音量值（增强可读性）                                */
#define COLOR_RSSI              190, 130, 255     /*  亮紫色信号强度（与主标题统一）                            */
#define COLOR_VOLBAR_OUT        140, 110, 160     /*  音量条外框（增强对比度）                                  */
#define COLOR_VOLBAR_IN         190, 130, 255     /*  音量条填充（与主标题统一）                                */
#define COLOR_DIGITS            190, 130, 255     /*  亮紫色数字显示（与主标题统一）                            */
#define COLOR_DIVIDER            80,  50, 100     /*  深紫灰分割线                                              */
#define COLOR_BITRATE           255, 180, 200     /*  粉紫色码率                                                */
#define COLOR_PL_CURRENT        255, 230, 255     /*  当前播放项（与电台名称统一，突出显示）                    */
#define COLOR_PL_CURRENT_BG      50,  35,  70     /*  当前播放项背景（稍微突出）                                */
#define COLOR_PL_CURRENT_FILL    50,  35,  70     /*  当前播放项填充背景（稍微突出）                            */
#define COLOR_PLAYLIST_0        240, 220, 255     /*  播放列表项0                                               */
#define COLOR_PLAYLIST_1        220, 200, 240     /*  播放列表项1                                               */
#define COLOR_PLAYLIST_2        200, 180, 220     /*  播放列表项2                                               */
#define COLOR_PLAYLIST_3        180, 160, 200     /*  播放列表项3                                               */
#define COLOR_PLAYLIST_4        160, 140, 180     /*  播放列表项4                                               */

#endif  /* #ifdef  ENABLE_THEME */
#endif  /* #define _optimized_purple_theme_h  */