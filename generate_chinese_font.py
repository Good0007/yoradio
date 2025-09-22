#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
生成yoRadio项目的中文字体库
使用Noto Sans SC字体生成多尺寸bitmap数据
"""

import re
import os
from PIL import Image, ImageDraw, ImageFont
from collections import OrderedDict

# 常用中文字符常量
RADIO_STATION_CHARS = """
一三上下不世东两个中为主之乌乐也乡书了事云亚交京亲人他休会伦伴体佛你佬信健儿克兰典内军农凤凰分剧动勒
化北区千华南卫卷厅历原厦口古台史合吉后听吾呼和哈善四园国在圳坊坛城基士声夏外大天太央女好娱子宁安家对
小少尔山岛岷川州市布年广庄庆应康延建德徽心快怀急性恋息悦情想感戏成我扬技报拉控摇摩播故教数文斯新方旁
旅族无日早时昆昌明星是晚晨曲月有朝木村杭松林果校格梦楚武民气汉江汽沈沙河波津洲活流济浙浩海深清温港游
湖湘湾源滚滨潇潮火爱爵牧特猫环珠球甘生电界番疆登的知石码社神福私科秒空第籁米粤经络维综绿网羊美老肃股
肥育色艺芒苏茄草莞萤萨蒙藏虫蝠行西视角讯评语读调谣象贝财贵资越车轻辣边辽这连通速道郁郑部郭都里重野金
银锡长门闲闻阅阳阿际陕陵集青音顺频飞食香高鱼鲁鲜鹰麻黑齐龙
"""

LOCALIZATION_CHARS = """
周一二三四五六日星期月北东南西北西南东北气压湿度风速体感就绪已停止连接中音量断开更新触摸屏卡索引热点模式密码正在到设置
"""

COMMON_CHINESE_CHARS = """
年月日时分秒上下左右前后里外大小多少高低长短宽窄远近快慢好坏新旧冷热干湿明暗静闹开关启停播放暂停下一首上一首音量调节设置配置网络连接断开错误成功失败重试取消确定返回菜单主页帮助关于版本更新搜索查找添加删除编辑保存退出
"""

NUMBERS_AND_PUNCTUATION = """
一二三四五六七八九十百千万亿零第，。；：？！""''（）【】《》〈〉「」『』、·…－—
"""

# 合并所有字符并去重
ALL_CHARS = RADIO_STATION_CHARS + LOCALIZATION_CHARS + COMMON_CHINESE_CHARS + NUMBERS_AND_PUNCTUATION

def extract_unique_chars(text):
    """提取文本中的唯一字符"""
    # 只保留中文字符、数字和常用标点
    pattern = re.compile(r'[\u4e00-\u9fff\u3000-\u303f\uff00-\uffef]+')
    chars = set()
    
    for match in pattern.finditer(text):
        for char in match.group():
            # 排除空格和某些特殊字符
            if char.strip() and ord(char) >= 0x4e00:
                chars.add(char)
    
    return chars

def create_char_bitmap(char, font_size, font_path, width=None, height=None):
    """创建单个字符的点阵数据，支持非正方形，左上角对齐"""
    try:
        # 如果没有指定宽高，默认为正方形
        if width is None:
            width = font_size
        if height is None:
            height = font_size
            
        # 直接创建目标尺寸的图像
        img = Image.new('L', (width, height), 0)  # 灰度图像，黑色背景
        draw = ImageDraw.Draw(img)
        
        # 加载字体，使用实际目标尺寸
        font = ImageFont.truetype(font_path, min(width, height))
        
        # 获取字符的边界框
        bbox = draw.textbbox((0, 0), char, font=font)
        
        # 左上角对齐绘制 - 类似 Adafruit GFX 的方式
        # 使用 bbox 的负偏移来确保字符从 (0,0) 开始正确显示
        x = -bbox[0]  # 补偿左边界偏移
        y = -bbox[1]  # 补偿上边界偏移
        
        # 绘制字符 - 白色字符在黑色背景上
        draw.text((x, y), char, fill=255, font=font)
        
        # 转换为点阵数据
        bitmap = []
        if width <= 8:
            # 8x8 或更小: 每行1字节
            for y in range(height):
                byte_val = 0
                for x in range(width):
                    pixel = img.getpixel((x, y))
                    if pixel > 128:  # 阈值化
                        byte_val |= (1 << (7 - x))
                bitmap.append(byte_val)
        else:
            # 大于8宽度: 计算需要的字节数
            bytes_per_row = (width + 7) // 8  # 向上取整
            for y in range(height):
                row_bytes = [0] * bytes_per_row
                for x in range(width):
                    pixel = img.getpixel((x, y))
                    if pixel > 128:  # 阈值化
                        byte_index = x // 8
                        bit_position = 7 - (x % 8)
                        row_bytes[byte_index] |= (1 << bit_position)
                bitmap.extend(row_bytes)
        
        return bitmap
    
    except Exception as e:
        print(f"创建字符 '{char}' 点阵失败: {e}")
        # 返回空点阵
        if width <= 8:
            return [0] * height
        else:
            bytes_per_row = (width + 7) // 8
            return [0] * (height * bytes_per_row)

def generate_font_header(chars, font_size, font_path, width=None, height=None):
    """生成字体头文件，支持非正方形字体"""
    # 如果没有指定宽高，默认为正方形
    if width is None:
        width = font_size
    if height is None:
        height = font_size
        
    # 排序字符
    sorted_chars = sorted(chars, key=lambda x: ord(x))
    
    print(f"正在生成 {width}x{height} 字体，包含 {len(sorted_chars)} 个字符...")
    
    # 计算每个字符需要的字节数
    if width <= 8:
        bytes_per_char = height  # 宽度<=8: height字节
    else:
        bytes_per_row = (width + 7) // 8  # 向上取整计算每行字节数
        bytes_per_char = height * bytes_per_row  # 总字节数
    
    # 开始生成头文件内容
    size_suffix = f"{width}x{height}" if width != height else f"{font_size}x{font_size}"
    size_id = f"{width}_{height}" if width != height else str(font_size)
    
    content = f'''#ifndef CHINESE_FONT_{width}X{height}_H
#define CHINESE_FONT_{width}X{height}_H

#include <stdint.h>

// 中文字体配置 - {width}x{height}点阵
#define CHINESE_FONT_{size_id}_WIDTH  {width}
#define CHINESE_FONT_{size_id}_HEIGHT {height}
#define CHINESE_FONT_{size_id}_BYTES  {bytes_per_char}

// 字符映射结构
struct ChineseCharMapping_{size_id} {{
    uint32_t unicode;           // Unicode码点
    const uint8_t bitmap[{bytes_per_char}];  // 内嵌点阵数据
}};

'''
    
    # 生成字符映射表
    content += f"// 字符映射表 - {width}x{height}点阵\n"
    content += f"const ChineseCharMapping_{size_id} chineseCharMap_{size_id}[] = {{\n"
    
    for char in sorted_chars:
        unicode_val = ord(char)
        bitmap = create_char_bitmap(char, font_size, font_path, width, height)
        
        content += f"    // {char} (U+{unicode_val:04X})\n"
        content += f"    {{0x{unicode_val:04X}, {{"
        
        # 输出bitmap数据
        for i, byte_val in enumerate(bitmap):
            if i % 8 == 0:
                content += "\n        "
            content += f"0x{byte_val:02X}"
            if i < len(bitmap) - 1:
                content += ", "
        
        content += "\n    }},\n"
    
    content += "};\n\n"
    
    # 添加数组大小常量
    content += f"const int chineseCharMapSize_{size_id} = sizeof(chineseCharMap_{size_id}) / sizeof(ChineseCharMapping_{size_id});\n\n"
    
    # 添加查找函数声明
    content += f'''// 查找函数
const ChineseCharMapping_{size_id}* findChineseChar_{size_id}(uint32_t unicode);

#endif // CHINESE_FONT_{width}X{height}_H
'''
    
    return content

def generate_font_cpp(font_size, width=None, height=None):
    """生成字体cpp实现文件，支持非正方形字体"""
    if width is None:
        width = font_size
    if height is None:
        height = font_size
    
    size_id = f"{width}_{height}" if width != height else str(font_size)
    size_suffix = f"{width}x{height}" if width != height else f"{font_size}x{font_size}"
    
    content = f'''#include "NotoSans_{size_suffix}.h"

// 查找字符映射
const ChineseCharMapping_{size_id}* findChineseChar_{size_id}(uint32_t unicode) {{
    for (int i = 0; i < chineseCharMapSize_{size_id}; i++) {{
        if (chineseCharMap_{size_id}[i].unicode == unicode) {{
            return &chineseCharMap_{size_id}[i];
        }}
    }}
    return nullptr;  // 未找到
}}
'''
    return content

def main():
    """主函数"""
    # 检查字体文件是否存在
    font_path = "/Users/kangkang/Library/Fonts/NotoSansSC.ttf"
    if not os.path.exists(font_path):
        print(f"错误: 字体文件不存在: {font_path}")
        print("请确保已安装 Noto Sans SC 字体")
        return
    
    # 提取所有唯一字符
    unique_chars = extract_unique_chars(ALL_CHARS)
    print(f"提取到 {len(unique_chars)} 个唯一中文字符")
    
    # 输出字符列表用于检查
    char_list = sorted(unique_chars, key=lambda x: ord(x))
    print(f"字符列表: {''.join(char_list)}")
    
    # 生成多种尺寸的字体
    font_configs = [
        (10,10,10),   # 10x12 矩形
        (12, 12, 12),   # 12x16 矩形 - 匹配 playlistConf
        (20, 20, 20),  # 20x32 矩形 - 新增更大尺寸
    ]
    output_dir = "yoRadio/src/displays/fonts"
    
    # 确保输出目录存在
    os.makedirs(output_dir, exist_ok=True)
    
    for font_size, width, height in font_configs:
        size_suffix = f"{width}x{height}" if width != height else f"{font_size}x{font_size}"
        print(f"\n开始生成 {size_suffix} 字体...")
        
        # 生成头文件
        header_content = generate_font_header(unique_chars, font_size, font_path, width, height)
        header_file = os.path.join(output_dir, f"NotoSans_{size_suffix}.h")
        
        with open(header_file, 'w', encoding='utf-8') as f:
            f.write(header_content)
        
        # 生成cpp文件
        cpp_content = generate_font_cpp(font_size, width, height)
        cpp_file = os.path.join(output_dir, f"NotoSans_{size_suffix}.cpp")
        
        with open(cpp_file, 'w', encoding='utf-8') as f:
            f.write(cpp_content)
        
        print(f"已生成: {header_file}")
        print(f"已生成: {cpp_file}")
    
    print(f"\n✅ 字体生成完成！共生成 {len(font_configs)} 种尺寸的字体文件")
    print("文件位置: yoRadio/src/displays/fonts/")

if __name__ == "__main__":
    main()