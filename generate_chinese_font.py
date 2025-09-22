#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
扫描yoRadio电台列表和系统文件中的中文字符
生成精简的中文字体库所需的字符集
"""

import re
import os
from collections import Counter

def extract_chinese_chars(text):
    """提取文本中的中文字符"""
    # 匹配中文字符的正则表达式
    chinese_pattern = re.compile(r'[\u4e00-\u9fff]+')
    chinese_chars = set()
    
    for match in chinese_pattern.finditer(text):
        for char in match.group():
            chinese_chars.add(char)
    
    return chinese_chars

def scan_playlist_file(filepath):
    """扫描电台列表文件"""
    chinese_chars = set()
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
            chinese_chars.update(extract_chinese_chars(content))
    except Exception as e:
        print(f"无法读取文件 {filepath}: {e}")
    
    return chinese_chars

def scan_locale_files(locale_dir):
    """扫描本地化文件"""
    chinese_chars = set()
    
    for root, dirs, files in os.walk(locale_dir):
        for file in files:
            if file.endswith('.h'):
                filepath = os.path.join(root, file)
                try:
                    with open(filepath, 'r', encoding='utf-8') as f:
                        content = f.read()
                        chinese_chars.update(extract_chinese_chars(content))
                except Exception as e:
                    print(f"无法读取文件 {filepath}: {e}")
    
    return chinese_chars

def generate_font_header(chars, output_file):
    """生成字体头文件"""
    # 按unicode排序
    sorted_chars = sorted(chars, key=lambda x: ord(x))
    
    header_content = f'''#ifndef CHINESE_FONT_H
#define CHINESE_FONT_H

#include "Arduino.h"

// 精简中文字体库 - 12x12点阵
// 包含 {len(sorted_chars)} 个常用中文字符

struct ChineseFontChar {{
    uint16_t unicode;       // Unicode码点
    uint8_t bitmap[18];     // 12x12点阵数据 (12*12/8=18字节)
}};

// 字符列表
static const uint16_t chineseCharList[] = {{
'''
    
    # 添加字符列表
    for i, char in enumerate(sorted_chars):
        if i % 8 == 0:
            header_content += '\n    '
        header_content += f'0x{ord(char):04X}, '
    
    header_content += f'''
}};

static const size_t chineseCharCount = {len(sorted_chars)};

// 字体数据声明 (需要外部提供点阵数据)
extern const ChineseFontChar chineseFontData[];

// 查找字符在字体数据中的索引
int findChineseCharIndex(uint16_t unicode);

// 绘制中文字符
void drawChineseChar(int16_t x, int16_t y, uint16_t unicode, uint16_t color, uint16_t bg);

// 绘制中文字符串
void drawChineseString(int16_t x, int16_t y, const char* str, uint16_t color, uint16_t bg);

#endif
'''
    
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(header_content)
    
    # 生成字符清单
    char_list_file = output_file.replace('.h', '_chars.txt')
    with open(char_list_file, 'w', encoding='utf-8') as f:
        f.write(f"扫描到的中文字符 ({len(sorted_chars)}个):\\n")
        f.write("=" * 50 + "\\n")
        for i, char in enumerate(sorted_chars):
            f.write(f"{char} (U+{ord(char):04X})")
            if (i + 1) % 10 == 0:
                f.write("\\n")
            else:
                f.write("  ")
    
    return sorted_chars

def main():
    base_dir = "/Users/kangkang/Workspace/yoradio/yoRadio"
    
    print("🔍 扫描yoRadio项目中的中文字符...")
    
    all_chinese_chars = set()
    
    # 1. 扫描电台列表
    playlist_file = f"{base_dir}/data/data/playlist.csv"
    if os.path.exists(playlist_file):
        chars = scan_playlist_file(playlist_file)
        all_chinese_chars.update(chars)
        print(f"📻 电台列表: 发现 {len(chars)} 个不同的中文字符")
    
    # 2. 扫描本地化文件
    locale_dir = f"{base_dir}/locale"
    if os.path.exists(locale_dir):
        chars = scan_locale_files(locale_dir)
        all_chinese_chars.update(chars)
        print(f"🌐 本地化文件: 发现 {len(chars)} 个不同的中文字符")
    
    # 3. 添加系统常用字符
    system_chars = set("设置网络音量时间日期星期一二三四五六日上下午确定取消返回播放暂停电台列表")
    all_chinese_chars.update(system_chars)
    print(f"⚙️  系统界面: 添加 {len(system_chars)} 个常用字符")
    
    print(f"\\n📊 总计发现 {len(all_chinese_chars)} 个不同的中文字符")
    
    # 生成字体头文件
    output_file = f"{base_dir}/src/displays/fonts/chineseFont.h"
    os.makedirs(os.path.dirname(output_file), exist_ok=True)
    
    chars = generate_font_header(all_chinese_chars, output_file)
    
    print(f"\\n✅ 字体头文件已生成: {output_file}")
    print(f"📝 字符清单已生成: {output_file.replace('.h', '_chars.txt')}")
    print(f"💾 预计内存占用: {len(chars)} × 18字节 = {len(chars) * 18}字节 ({len(chars) * 18 / 1024:.1f}KB)")

if __name__ == "__main__":
    main()