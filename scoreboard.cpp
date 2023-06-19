#include "scoreboard.h"
#include "game.h"
#include "cmd_console_tools.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

scoreboard::scoreboard()
{
	for (int i = 0; i < SCOREBOARD_SIZE; i++)  // 初始化分数
		scores[i] = 0;
	p = -1;  //  初始化指针
}

void scoreboard::add_score(const int x)
{
	// 添加分数记录
	scores[++p] = x;
}
void scoreboard::sort_scores()
{
	// 给分数排序
	sort(scores, scores + p + 1, my_comp);
}
void scoreboard::show_scores()
{
	// 输出分数记录┏━┓┃┗━┛┳┣╋┫┻

	cct_cls();
	cct_gotoxy(0, 0);

	cout << "┏━━━┳━━━┓" << endl;
	cout << "┃ 排名 ┃ 分数 ┃" << endl;

	if (p == -1)
	{
		cout << "┗━━━┻━━━┛" << endl;
		return;
	}
	else
		cout << "┣━━━╋━━━┫" << endl;

	for (int i = 0; i < p; i++)
	{
		cout << "┃" << "No." << setw(3) << i + 1 << "┃" << setw(6) << scores[i] << "┃" << endl;
		cout << "┣━━━╋━━━┫" << endl;
	}
	cout << "┃" << "No." << setw(3) << p + 1 << "┃" << setw(6) << scores[p] << "┃" << endl;
	cout << "┗━━━┻━━━┛" << endl;
}

