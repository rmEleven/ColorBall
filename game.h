#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define SET_COLS 50
#define SET_LINES 27
#define SET_BUFFER_COLS 50
#define SET_BUFFER_LINES 27

char menu();  // 显示菜单
bool get_input(int& sx, int& sy, int& fx, int& fy);  // 读取起点终点坐标（返回值表示是否退出游戏）
bool my_comp(int a, int b);  // 比较函数 用于分数排序
void goto_safe_area();  // 移动到棋盘之外的位置 防止后续输出覆盖棋盘
void clear_extra();  // 键盘输入时 由于清空换行输入在屏幕的回显
void show_score_now(const int score);  // 显示本次游戏得分
void game_discription();  // 游戏说明

int game(bool use_mouse = false);  // 游戏过程