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
	cct_setconsoletitle("五子连珠");  // 设置cmd窗口的标题
	cct_setconsoleborder(SET_COLS, SET_LINES, SET_BUFFER_COLS, SET_BUFFER_LINES);  // 设置cmd窗口的大小及缓冲区的大小
	srand((unsigned int)(time(0)));  // 初始化随机种子
	scoreboard sb;  // 创建记分板
	bool game_end = false;  // 结束游戏标记

	cct_setcursor(CURSOR_INVISIBLE);  // 光标不可见

	while (1)  // 进行一轮游戏
	{
		if (game_end)  // 结束游戏
			break;

		char menu_return = menu();  // 进入菜单界面
		int score_this_turn = 0;  // 本轮游戏得分

		char ch = 0;

		switch (menu_return)
		{
			
			case '0':  // 结束游戏
				game_end = true;
				break;
			case '1':  // 鼠标操作游戏
				score_this_turn = game(true);
				sb.add_score(score_this_turn);
				break;
			case '2':  // 键盘操作游戏
				score_this_turn = game();
				sb.add_score(score_this_turn);
				break;
			case '3':  // 查看得分记录
				sb.show_scores();
				cout << "按任意键继续..." << endl;
				ch = _getch();
				break;
			case '4':  // 游戏说明
				cct_cls();
				game_discription();
				ch = _getch();
				break;
		}
	}

	cct_setcursor(CURSOR_VISIBLE_FULL);  // 光标可见

	return 0;
}