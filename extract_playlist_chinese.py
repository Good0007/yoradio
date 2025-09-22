#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
提取播放列表中的所有中文字符用于字体生成
"""

import re
import os
import sys

def extract_chinese_chars_from_playlist(playlist_file):
    """从播放列表文件中提取所有中文字符"""
    chinese_chars = set()
    
    try:
        with open(playlist_file, 'r', encoding='utf-8') as f:
            for line_num, line in enumerate(f, 1):
                line = line.strip()
                if not line or line.startswith('#'):
                    continue
                
                # 查找中文字符 (Unicode范围: 4E00-9FFF)
                chinese_in_line = re.findall(r'[\u4e00-\u9fff]', line)
                for char in chinese_in_line:
                    chinese_chars.add(char)
                    
                # 打印包含中文的行用于调试
                if chinese_in_line:
                    print(f"行 {line_num}: {line[:50]}{'...' if len(line) > 50 else ''}")
                    print(f"  找到中文字符: {''.join(chinese_in_line)}")
                    
    except FileNotFoundError:
        print(f"错误: 找不到文件 {playlist_file}")
        return set()
    except UnicodeDecodeError:
        print(f"错误: 文件编码问题，尝试其他编码...")
        # 尝试其他编码
        for encoding in ['gbk', 'gb2312', 'latin1']:
            try:
                with open(playlist_file, 'r', encoding=encoding) as f:
                    for line in f:
                        chinese_in_line = re.findall(r'[\u4e00-\u9fff]', line)
                        for char in chinese_in_line:
                            chinese_chars.add(char)
                print(f"使用 {encoding} 编码成功读取文件")
                break
            except:
                continue
        else:
            print("无法识别文件编码")
            return set()
    
    return chinese_chars

def main():
    # 检查播放列表文件
    playlist_paths = [
        './yoRadio/data/data/playlist.csv',
        './yoRadio/data/playlist.csv', 
        './playlist.csv',
        './data/playlist.csv'
    ]
    
    playlist_file = None
    for path in playlist_paths:
        if os.path.exists(path):
            playlist_file = path
            break
    
    if not playlist_file:
        print("错误: 找不到播放列表文件")
        print("请确保以下路径之一存在播放列表文件:")
        for path in playlist_paths:
            print(f"  {path}")
        sys.exit(1)
    
    print(f"正在处理播放列表文件: {playlist_file}")
    print("-" * 50)
    
    # 提取中文字符
    chinese_chars = extract_chinese_chars_from_playlist(playlist_file)
    
    if not chinese_chars:
        print("未找到中文字符")
        sys.exit(0)
    
    # 排序字符
    sorted_chars = sorted(chinese_chars)
    
    print(f"\n找到 {len(sorted_chars)} 个唯一的中文字符:")
    print("-" * 50)
    
    # 按每行20个字符显示
    for i in range(0, len(sorted_chars), 20):
        chunk = sorted_chars[i:i+20]
        print(''.join(chunk))
    
    # 保存到文件
    output_file = 'playlist_chinese_chars.txt'
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(''.join(sorted_chars))
    
    print(f"\n中文字符已保存到: {output_file}")
    
    # 生成字符统计
    print(f"\n字符统计:")
    print(f"总字符数: {len(sorted_chars)}")
    
    # 按Unicode块分类
    blocks = {
        'CJK统一汉字': (0x4E00, 0x9FFF),
        'CJK扩展A': (0x3400, 0x4DBF),
        'CJK扩展B': (0x20000, 0x2A6DF),
    }
    
    for block_name, (start, end) in blocks.items():
        count = sum(1 for char in sorted_chars if start <= ord(char) <= end)
        if count > 0:
            print(f"  {block_name}: {count} 个字符")

if __name__ == "__main__":
    main()