#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define SET_COLS 50
#define SET_LINES 27
#define SET_BUFFER_COLS 50
#define SET_BUFFER_LINES 27

char menu();  // ��ʾ�˵�
bool get_input(int& sx, int& sy, int& fx, int& fy);  // ��ȡ����յ����꣨����ֵ��ʾ�Ƿ��˳���Ϸ��
bool my_comp(int a, int b);  // �ȽϺ��� ���ڷ�������
void goto_safe_area();  // �ƶ�������֮���λ�� ��ֹ���������������
void clear_extra();  // ��������ʱ ������ջ�����������Ļ�Ļ���
void show_score_now(const int score);  // ��ʾ������Ϸ�÷�
void game_discription();  // ��Ϸ˵��

int game(bool use_mouse = false);  // ��Ϸ����