#include "NotoSans_12x12.h"

// 查找字符映射
const ChineseCharMapping_12* findChineseChar_12(uint32_t unicode) {
    for (int i = 0; i < chineseCharMapSize_12; i++) {
        if (chineseCharMap_12[i].unicode == unicode) {
            return &chineseCharMap_12[i];
        }
    }
    return nullptr;  // 未找到
}
