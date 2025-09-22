#include "NotoSans_20x20.h"

// 查找字符映射
const ChineseCharMapping_20* findChineseChar_20(uint32_t unicode) {
    for (int i = 0; i < chineseCharMapSize_20; i++) {
        if (chineseCharMap_20[i].unicode == unicode) {
            return &chineseCharMap_20[i];
        }
    }
    return nullptr;  // 未找到
}
