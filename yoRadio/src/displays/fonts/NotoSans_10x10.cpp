#include "NotoSans_10x10.h"

// 查找字符映射
const ChineseCharMapping_10* findChineseChar_10(uint32_t unicode) {
    for (int i = 0; i < chineseCharMapSize_10; i++) {
        if (chineseCharMap_10[i].unicode == unicode) {
            return &chineseCharMap_10[i];
        }
    }
    return nullptr;  // 未找到
}
