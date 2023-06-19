#include <iostream>
#include <windows.h>
#include <conio.h>
#include "game.h"
#include "cmd_console_tools.h"
#include "chessboard.h"
using namespace std;

char menu()
{
	// 显示菜单

	cct_cls();

	cout << "    ┏━━━━━┓    " << endl;
	cout << "    ┃#五子连珠#┃    " << endl;
	cout << "    ┗━━━━━┛    " << endl;

	cout << "----------------------" << endl;
	cout << "1.开始游戏（鼠标操作）" << endl;
	cout << "----------------------" << endl;
	cout << "2.开始游戏（键盘操作）" << endl;
	cout << "----------------------" << endl;
	cout << "3.查看得分记录        " << endl;
	cout << "----------------------" << endl;
	cout << "4.查看游戏说明        " << endl;
	cout << "----------------------" << endl;
	cout << "0.退出                " << endl;
	cout << "----------------------" << endl;
	cout << "[请选择:] ";

	char ch;
	while (1)
	{
		ch = _getch();
		if (ch >= '0' && ch <= '4')
			break;
	}
	cout << ch;

	Sleep(300);

	return ch;
}
bool get_input(int &sx, int &sy, int &fx, int &fy)
{
	//输入起始行
	goto_safe_area();
	while (1)
	{
		cout << "请输入起始行数(" << 1 << '-' << CHESSBOARD_ROW << ")：";
		cin >> sx;
		if (cin.good() == 0)
		{
			cin.clear();
			cin.ignore(65536, '\n');
		}
		else if ((sx >= 1) && (sx <= CHESSBOARD_ROW))
			break;
	}
	goto_safe_area();
	cout << "                                      ";

	//输入起始列
	goto_safe_area();
	while (1)
	{
		cout << "请输入起始列数(" << 1 << '-' << CHESSBOARD_COL << ")：";
		cin >> sy;
		if (cin.good() == 0)
		{
			cin.clear();
			cin.ignore(65536, '\n');
		}
		else if ((sy >= 1) && (sy <= CHESSBOARD_COL))
			break;
	}
	goto_safe_area();
	cout << "                                      ";

	//输入目标行
	goto_safe_area();
	while (1)
	{
		cout << "请输入目标行数(" << 1 << '-' << CHESSBOARD_ROW << ")：";
		cin >> fx;
		if (cin.good() == 0)
		{
			cin.clear();
			cin.ignore(65536, '\n');
		}
		else if ((fx >= 1) && (fx <= CHESSBOARD_ROW))
			break;
	}
	goto_safe_area();
	cout << "                                      ";

	//输入目标列
	goto_safe_area();
	while (1)
	{
		cout << "请输入目标列数(" << 1 << '-' << CHESSBOARD_COL << ")：";
		cin >> fy;
		if (cin.good() == 0)
		{
			cin.clear();
			cin.ignore(65536, '\n');
		}
		else if ((fy >= 1) && (fy <= CHESSBOARD_COL))
			break;
	}
	goto_safe_area();
	cout << "                                      ";

	sx--;
	sy--;
	fx--;
	fy--;

	return false;  // 不退出游戏
}
bool my_comp(int a, int b)
{
	return a > b;
}
void goto_safe_area()
{
	// 移动到棋盘之外的位置 防止后续输出覆盖棋盘
	cct_gotoxy(0, 1 + CHESSBOARD_ROW * (BEAD_SIZE + 1) + 2);
}
void clear_extra()
{
	// 键盘输入时 由于清空换行输入在屏幕的回显
	goto_safe_area();
	int x, y;
	cct_getxy(y, x);
	for (int i = 1; i <= 3; i++)
	{
		cct_gotoxy(y, ++x);
		cout << "                    ";
	}
}
void show_score_now(const int score)
{
	cct_gotoxy(0, 0);
	cout << "当前得分：" << score;
}
void game_discription()
{
	cout << "--------------" << endl;
	cout << "本游戏的说明：" << endl;
	cout << "-------------------------------------------------" << endl;
	cout << "简介：" << endl;
	cout << "    棋盘大小是9X9，一共会出现6种颜色的珠子。初始状态棋盘上7个随机位置分布着7个随机颜色的珠子。通过移动珠子将同色的珠子连在一起来消除后得分。当无珠子可以移动时程序结束。游戏记录所有游戏者得分并排序。" << endl << endl;
	cout << "规则：" << endl;
	cout << "    1.一次只允许移动一个珠子，只要棋盘上有空位置就可以移动珠子。" << endl;
	cout << "    2.每移动一颗珠子以后，如果不满足同色的5颗珠子相连，将会出现3个随机颜色珠子分布到棋盘任意空置的位置上。" << endl;
	cout << "    3.如果同色的珠子能有5颗及以上连在一起排成横向、纵向或者斜向时，这些珠子从棋盘上消失，不产生3颗珠子。同时游戏者可以得分，每个珠子2分。" << endl;
	cout << "    4.当棋盘被珠子占满时游戏结束。" << endl;
	cout << "-------------------------------------------------" << endl;
	cout << "按任意键继续..." << endl;
	cout << "---------------" << endl;
}

int game(bool use_mouse)
{
	chessboard cb;  // 创建棋盘
	cb.draw_chessboard();  // 画出棋盘

	int player_score = 0;  // 本次游戏玩家分数
	show_score_now(player_score);  // 显示得分

	bool game_end = false;  // 是否要结束游戏

	// 开局
	while (1)
	{
		cb.rand_bead(7);  // 游戏开局随机生成7个珠子
		for (int i = 0; i < cb.row; i++)  // 遍历每一行
			for (int j = 0; j < cb.col; j++)  // 遍历每一列
				if (cb.st[i][j] != 0)  // 位置非空
					cb.draw_bead(i, j);  // 画出这个珠子

		// 对着7个珠子记分
		for (int i = 0; i < cb.row; i++)  // 遍历每一行
			for (int j = 0; j < cb.col; j++)  // 遍历每一列
				if (cb.st[i][j] != 0)  // 找到珠子
				{
					cb.clear_serial();  // 清空可消除位置标记

					// 查找可消除位置
					cb.horizontal(i, j);
					cb.vertical(i, j);
					cb.lean_left(i, j);
					cb.lean_right(i, j);

					int num_bead = cb.count_serial();  // 可消除的珠子总数

					if (num_bead != 0)  // 可以消除
					{
						player_score += 2 * num_bead;  // 记分
						show_score_now(player_score);  // 显示得分

						cb.draw_blingbling();  // 闪烁显示要消除的珠子

						for (int i = 0; i < cb.row; i++)
							for (int j = 0; j < cb.col; j++)
								if (cb.serial[i][j])  // 找到可消除珠子
								{
									cb.del_bead(i, j);  // 在棋盘数组中删除
									cb.erase_bead(i, j);  // 在界面上擦除
								}

						cb.clear_serial();  // 清空可消除位置标记
					}
				}

		if (cb.is_empty())  // 特殊情况：初始的7个珠子全都自动消除（概率极低）
		{
			player_score = 0;  // 分数作废 从新开始
			continue;  // 重新生成7个珠子
		}
		else
			break;
	}

	// 玩家操作
	while (!cb.is_filled())  // 棋盘没有放满则继续游戏
	{
		int sx = 0, sy = 0, fx = 0, fy = 0;  // 起点和终点的坐标

		if (use_mouse)  // 使用鼠标
		{
			cct_enable_mouse();

			while (1)  // 获取起点坐标
			{
				int MX, MY, MAction, keycode1, keycode2, cct_return;
				cct_return = cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2);

				if (cct_return == CCT_MOUSE_EVENT)  // 是鼠标事件
				{
					if (MAction == MOUSE_LEFT_BUTTON_CLICK)  // 单击左键
					{
						sx = (MY - 2) / (BEAD_SIZE + 1);
						sy = (MX - 2) / (BEAD_SIZE + 1) / 2;
						cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2);  // 读取松开鼠标
						break;
					}
					else if (MAction == MOUSE_RIGHT_BUTTON_CLICK)  // 单击右键
					{
						game_end = true;//结束游戏
						break;
					}
					else  // 其他鼠标操作
						continue;  // 无效操作 继续读取鼠标
				}
			}

			if (game_end)
				break;  // 结束游戏

			while (1)  // 获取终点坐标
			{
				int MX, MY, MAction, keycode1, keycode2, cct_return;
				cct_return = cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2);

				if (cct_return == CCT_MOUSE_EVENT)  // 是鼠标事件
				{
					if (MAction == MOUSE_LEFT_BUTTON_CLICK)  // 单击左键
					{
						fx = (MY - 2) / (BEAD_SIZE + 1);
						fy = (MX - 2) / (BEAD_SIZE + 1) / 2;
						cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2);  // 读取松开鼠标
						break;
					}
					else if (MAction == MOUSE_RIGHT_BUTTON_CLICK)  // 单击右键
					{
						game_end = true;//结束游戏
						break;
					}
					else  // 其他鼠标操作
						continue;  // 无效操作 继续读取鼠标
				}
			}

			if (game_end)
				break;  // 结束游戏

			cct_disable_mouse();
		}
		else  // 使用键盘
		{
			get_input(sx, sy, fx, fy);
			clear_extra();
		}

		if (cb.move_bead(sx, sy, fx, fy))  // 移动合法
		{
			//cb.erase_bead(sx, sy);  // 把起始位置珠子擦除
			//cb.draw_bead(fx, fy);  // 在目标位置画上珠子

			// 对这次移动记分
			cb.clear_serial();  // 清空可消除位置标记

			// 查找可消除位置
			cb.horizontal(fx, fy);
			cb.vertical(fx, fy);
			cb.lean_left(fx, fy);
			cb.lean_right(fx, fy);

			int num_bead = cb.count_serial();  // 可消除的珠子总数

			if (num_bead != 0)  // 可以消除
			{
				player_score += 2 * num_bead;  // 记分
				show_score_now(player_score);  // 显示得分

				cb.draw_blingbling();  // 闪烁显示要消除的珠子

				for (int i = 0; i < cb.row; i++)
					for (int j = 0; j < cb.col; j++)
						if (cb.serial[i][j])  // 找到要消除的珠子
						{
							cb.del_bead(i, j);  // 在棋盘数组中删除
							cb.erase_bead(i, j);  // 在界面上擦除
						}

				cb.clear_serial();  // 清空可消除位置标记
			}
			else  // 不能消除
			{
				cb.rand_bead(3);  // 随机产生3个珠子

				// 画出这3个珠子（实际是画出所有珠子 间接画出这3个）
				for (int i = 0; i < cb.row; i++)
					for (int j = 0; j < cb.col; j++)
						if (cb.st[i][j])
							cb.draw_bead(i, j);

				// 对这3个珠子记分
				for (int i = 0; i < cb.row; i++)  // 遍历每一行
					for (int j = 0; j < cb.col; j++)  // 遍历每一列
						if (cb.st[i][j] != 0)  // 找到珠子
						{
							cb.clear_serial();  // 清空可消除位置标记

							// 查找可消除位置
							cb.horizontal(i, j);
							cb.vertical(i, j);
							cb.lean_left(i, j);
							cb.lean_right(i, j);

							int num_bead = cb.count_serial();  // 可消除的珠子总数

							if (num_bead != 0)  // 可以消除
							{
								player_score += 2 * num_bead;  // 记分
								show_score_now(player_score);  // 显示得分

								cb.draw_blingbling();  // 闪烁显示要消除的珠子

								for (int i = 0; i < cb.row; i++)
									for (int j = 0; j < cb.col; j++)
										if (cb.serial[i][j])  // 找到可消除珠子
										{
											cb.del_bead(i, j);  // 在棋盘数组中删除
											cb.erase_bead(i, j);  // 在界面上擦除
										}

								cb.clear_serial();  // 清空可消除位置标记
							}
						}
			}
		}
		else  // 移动不合法
		{
			goto_safe_area();  // 光标移动到安全区域
			cout << "移动不合法！" << endl;  // 非法移动文字说明
			Sleep(1000);  // 暂停一会
			goto_safe_area();  // 光标移动到文字开头
			cout << "            " << endl;  // 覆盖文字说明
		}
	}

	// 结束
	return player_score;  // 棋盘放满 游戏结束 传递得分
}