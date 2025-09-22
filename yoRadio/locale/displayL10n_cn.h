#ifndef dsp_full_loc
#define dsp_full_loc
#include <pgmspace.h>
/*************************************************************************************
    中文语言包 - Chinese Language Pack
    包含常用中文字符，需要配合中文字体使用
*************************************************************************************/
const char mon[] PROGMEM = "一";
const char tue[] PROGMEM = "二";
const char wed[] PROGMEM = "三";
const char thu[] PROGMEM = "四";
const char fri[] PROGMEM = "五";
const char sat[] PROGMEM = "六";
const char sun[] PROGMEM = "日";

const char monf[] PROGMEM = "周一";
const char tuef[] PROGMEM = "周二";
const char wedf[] PROGMEM = "周三";
const char thuf[] PROGMEM = "周四";
const char frif[] PROGMEM = "周五";
const char satf[] PROGMEM = "周六";
const char sunf[] PROGMEM = "周日";

const char jan[] PROGMEM = "1月";
const char feb[] PROGMEM = "2月";
const char mar[] PROGMEM = "3月";
const char apr[] PROGMEM = "4月";
const char may[] PROGMEM = "5月";
const char jun[] PROGMEM = "6月";
const char jul[] PROGMEM = "7月";
const char aug[] PROGMEM = "8月";
const char sep[] PROGMEM = "9月";
const char octt[] PROGMEM = "10月";
const char nov[] PROGMEM = "11月";
const char decc[] PROGMEM = "12月";

const char wn_N[]      PROGMEM = "北";
const char wn_NNE[]    PROGMEM = "北北东";
const char wn_NE[]     PROGMEM = "东北";
const char wn_ENE[]    PROGMEM = "东北东";
const char wn_E[]      PROGMEM = "东";
const char wn_ESE[]    PROGMEM = "东南东";
const char wn_SE[]     PROGMEM = "东南";
const char wn_SSE[]    PROGMEM = "南南东";
const char wn_S[]      PROGMEM = "南";
const char wn_SSW[]    PROGMEM = "南南西";
const char wn_SW[]     PROGMEM = "西南";
const char wn_WSW[]    PROGMEM = "西南西";
const char wn_W[]      PROGMEM = "西";
const char wn_WNW[]    PROGMEM = "西北西";
const char wn_NW[]     PROGMEM = "西北";
const char wn_NNW[]    PROGMEM = "北北西";

const char* const dow[]     PROGMEM = { sun, mon, tue, wed, thu, fri, sat };
const char* const dowf[]    PROGMEM = { sunf, monf, tuef, wedf, thuf, frif, satf };
const char* const mnths[]   PROGMEM = { jan, feb, mar, apr, may, jun, jul, aug, sep, octt, nov, decc };
const char* const wind[]    PROGMEM = { wn_N, wn_NNE, wn_NE, wn_ENE, wn_E, wn_ESE, wn_SE, wn_SSE, wn_S, wn_SSW, wn_SW, wn_WSW, wn_W, wn_WNW, wn_NW, wn_NNW, wn_N };

const char    const_PlReady[]    PROGMEM = "就绪";
const char  const_PlStopped[]    PROGMEM = "暂停";
const char  const_PlConnect[]    PROGMEM = "播放中";
const char  const_DlgVolume[]    PROGMEM = "音量";
const char    const_DlgLost[]    PROGMEM = "连接断开";
const char  const_DlgUpdate[]    PROGMEM = "更新中";
const char const_DlgNextion[]    PROGMEM = "触摸屏";
const char const_getWeather[]    PROGMEM = "";
const char  const_waitForSD[]    PROGMEM = "SD卡索引中";

const char        apNameTxt[]    PROGMEM = "热点模式";
const char        apPassTxt[]    PROGMEM = "密码";
const char       bootstrFmt[]    PROGMEM = "正在连接到 %s";
const char        apSettFmt[]    PROGMEM = "设置: HTTP://%s/";
#if EXT_WEATHER
const char       weatherFmt[]    PROGMEM = "%s, %.1f°C 体感: %.1f°C 气压: %d mmHg 湿度: %d%% 风速: %.1f m/s [%s]";
#else
const char       weatherFmt[]    PROGMEM = "%s, %.1f°C 气压: %d mmHg 湿度: %d%%";
#endif
const char     weatherUnits[]    PROGMEM = "metric";   /* standard, metric, imperial */
const char      weatherLang[]    PROGMEM = "zh_cn";    /* https://openweathermap.org/current#multi */

#endif