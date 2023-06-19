#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "game.h"
#include "cmd_console_tools.h"
#include "chessboard.h"
#include "scoreboard.h"
using namespace std;

int main()
{
	cct_setconsoletitle("��������");  // ����cmd���ڵı���
	cct_setconsoleborder(SET_COLS, SET_LINES, SET_BUFFER_COLS, SET_BUFFER_LINES);  // ����cmd���ڵĴ�С���������Ĵ�С
	srand((unsigned int)(time(0)));  // ��ʼ���������
	scoreboard sb;  // �����Ƿְ�
	bool game_end = false;  // ������Ϸ���

	cct_setcursor(CURSOR_INVISIBLE);  // ��겻�ɼ�

	while (1)  // ����һ����Ϸ
	{
		if (game_end)  // ������Ϸ
			break;

		char menu_return = menu();  // ����˵�����
		int score_this_turn = 0;  // ������Ϸ�÷�

		char ch = 0;

		switch (menu_return)
		{
			
			case '0':  // ������Ϸ
				game_end = true;
				break;
			case '1':  // ��������Ϸ
				score_this_turn = game(true);
				sb.add_score(score_this_turn);
				break;
			case '2':  // ���̲�����Ϸ
				score_this_turn = game();
				sb.add_score(score_this_turn);
				break;
			case '3':  // �鿴�÷ּ�¼
				sb.show_scores();
				cout << "�����������..." << endl;
				ch = _getch();
				break;
			case '4':  // ��Ϸ˵��
				cct_cls();
				game_discription();
				ch = _getch();
				break;
		}
	}

	cct_setcursor(CURSOR_VISIBLE_FULL);  // ���ɼ�

	return 0;
}