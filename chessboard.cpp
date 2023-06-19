#include "chessboard.h"
#include "cmd_console_tools.h"
#include "game.h"
#include <iostream>
#include <windows.h>
#include <iomanip>
using namespace std;

typedef pair<int, int> pii;

chessboard::chessboard()
{
	// 初始化空白棋盘
	row = CHESSBOARD_ROW;  // 初始化棋盘行数
	col = CHESSBOARD_COL;  // 初始化棋盘列数

	for (int i = 0; i < row; i++)  // 初始化空白棋盘
		for (int j = 0; j < col; j++)
		{
			st[i][j] = 0;
			step[i][j] = -1;
			serial[i][j] = false;
		}
}

bool chessboard::is_filled()
{
	// 判断棋盘是否是满的
	for (int i = 0; i < row; i++)  // 遍历每一行
		for (int j = 0; j < col; j++)  // 遍历每一列
			if (st[i][j] == 0)  // 有空位
				return false;  // 不是满的
	return true;  // 是满的
}
bool chessboard::is_empty()
{
	// 判断棋盘是否是空的
	for (int i = 0; i < row; i++)  // 遍历每一行
		for (int j = 0; j < col; j++)  // 遍历每一列
			if (st[i][j] != 0)  // 有珠子
				return false;  // 不是空的
	return true;  // 是空的
}

void chessboard::rand_bead(const int num)
{
	// 随机产生num个珠子
	for (int i = 0; i < num; i++)
	{
		if (is_filled())  // 如果棋盘已经放满了
			return;  // 结束

		int x = rand() % row;  // 随机生成行标（0--row-1）
		int y = rand() % col;  // 随机生成列标（0--col-1）
		if (st[x][y] == 0)  // 该位置为空
		{
			int color = rand() % 6 + 1;  // 随机生成颜色（1--6）
			st[x][y] = color;  // 放入珠子
		}
		else  // 该位置非空
			i--;  // 继续找位置
	}
}
bool chessboard::move_bead(const int sx, const int sy, const int fx, const int fy)
{
	// 从(sx, sy)移动到(fx, fy)

	clear_step();  // 清空距离

	if (st[sx][sy] == 0 || st[fx][fy] != 0)  // 起始位置为空或目标位置非空
		return false;  // 移动失败

	pii q[CHESSBOARD_COL * CHESSBOARD_ROW];  // 定义队列
	int head = 0, tail = -1;  // 队头和队尾
	q[++tail] = { sx, sy };  // 队列初始化

	
	step[sx][sy] = 0;  // 起点为第0步

	int nx[4] = { 0, 0, 1, -1 };  // 四个方向
	int ny[4] = { 1, -1, 0, 0 };  // 四个方向

	while (head <= tail)
	{
		pii t = q[head++];  // 取出队头
		for (int i = 0; i < 4; i++)  // 遍历四个方向
		{
			int n_x = t.first + nx[i];  // 下一个点的坐标
			int n_y = t.second + ny[i];  // 下一个点的坐标

			bool inside = (n_x >= 0 && n_x < CHESSBOARD_ROW&& n_y >= 0 && n_y < CHESSBOARD_COL);  // 是否在棋盘内部

			if (inside && step[n_x][n_y] == -1 && st[n_x][n_y] == 0)  // 不越界 没走过 是空位
			{
				step[n_x][n_y] = step[t.first][t.second] + 1;  // 确定为下一步
				q[++tail] = { n_x, n_y };  // 入队
			}
		}
	}

	if (step[fx][fy] == -1)  // 没有走到目标位置
		return false;  // 移动出错

	pii q_back[CHESSBOARD_ROW * CHESSBOARD_COL];  // 最短路径回溯
	int p = -1;  // 指针
	q_back[++p] = {fx, fy};  // 放入终点位置
	// 最短路回溯
	int xx = fx, yy = fy;
	while (1)
	{
		for (int i = 0; i < 4; i++)
		{
			int n_x = xx + nx[i];  // 下一个点的坐标
			int n_y = yy + ny[i];  // 下一个点的坐标

			bool inside = (n_x >= 0 && n_x < CHESSBOARD_ROW&& n_y >= 0 && n_y < CHESSBOARD_COL);  // 是否在棋盘内部

			if (inside && step[n_x][n_y] == step[xx][yy] - 1)  // 未越界 步数少1
			{
				q_back[++p] = { n_x, n_y };  // 加入数组
				xx = n_x;  // 更新
				yy = n_y;  // 更新
				break;
			}
		}
		if (xx == sx && yy == sy)  // 找到起点
			break;  // 结束回溯
	}
	// 绘制最短路
	pii last_bead = q_back[p--];  // 设置参考位置
	while (p >= 0)
	{
		pii now_bead = q_back[p--];  // 获取新位置

		st[now_bead.first][now_bead.second] = st[last_bead.first][last_bead.second];  // 复制颜色数值

		del_bead(last_bead.first, last_bead.second);  // 删掉原始位置
		erase_bead(last_bead.first, last_bead.second);  // 擦掉原位置珠子
		Sleep(100);  // 暂停一会

		draw_bead(now_bead.first, now_bead.second);  // 画出新珠子
		Sleep(100);  // 暂停一会

		last_bead = now_bead;  // 更新参考位置
	}

	return true;  // 移动成功
}
bool chessboard::add_bead(const int x, const int y, const int c)
{
	// 在指定位置添加指定颜色的珠子
	if (st[x][y] != 0)  // 指定位置非空
		return false;  // 添加出错
	else  // 指定位置为空
	{
		st[x][y] = c;  // 确定颜色
		return true;  // 添加成功
	}
}
bool chessboard::del_bead(const int x, const int y)
{
	// 删除指定位置的珠子
	if (st[x][y] == 0)  // 位置为空
		return false;  // 删除失败
	else  // 位置非空
	{
		st[x][y] = 0;  // 删除珠子
		return true;  // 删除成功
	}
}

void chessboard::horizontal(const int x, const int y)
{
	int sx = x, sy = y, fx = x, fy = y;  // 记录连续区域两端的坐标
	if (st[x][y] == 0)  // 如果是空位
		return;  // 没有连续同色珠子
	int cnt = 1;  // (x, y)本身算一个
	while (1)
	{
		sy -= 1;  // 向左找珠子
		if (sy < 0)  // 越界
		{
			sy++;  // 回溯
			break;  // 结束查找
		}
		if (st[sx][sy] == st[x][y])  // 同色
			cnt++;  // 数量增加
		else  // 不同色
		{
			sy++;  // 回溯
			break;  // 结束查找
		}
	}
	while (1)
	{
		fy += 1;  // 向右找珠子
		if (fy >= col)  // 越界
		{
			fy--;  // 回溯
			break;  // 结束查找
		}
		if (st[fx][fy] == st[x][y])  // 同色
			cnt++;  // 数量增加
		else  // 不同色
		{
			fy--;  // 回溯
			break;  // 结束查找
		}
	}
	if (cnt >= 5)
	{
		for (int i = sy; i <= fy; i++)
			serial[sx][i] = true;
	}
}
void chessboard::vertical(const int x, const int y)
{
	int sx = x, sy = y, fx = x, fy = y;  // 记录连续区域两端的坐标
	if (st[x][y] == 0)  // 如果是空位
		return;  // 没有连续同色珠子
	int cnt = 1;  // (x, y)本身算一个
	while (1)
	{
		sx -= 1;  // 向上找珠子
		if (sx < 0)  // 越界
		{
			sx++;  // 回溯
			break;  // 结束查找
		}
		if (st[sx][sy] == st[x][y])  // 同色
			cnt++;  // 数量增加
		else  // 不同色
		{
			sx++;  // 回溯
			break;  // 结束查找
		}
	}
	while (1)
	{
		fx += 1;  // 向下找珠子
		if (fx >= row)  // 越界
		{
			fx--;  // 回溯
			break;  // 结束查找
		}
		if (st[fx][fy] == st[x][y])  // 同色
			cnt++;  // 数量增加
		else  // 不同色
		{
			fx--;  // 回溯
			break;  // 结束查找
		}
	}
	if (cnt >= 5)
	{
		for (int i = sx; i <= fx; i++)
			serial[i][sy] = true;
	}
}
void chessboard::lean_right(const int x, const int y)
{
	int sx = x, sy = y, fx = x, fy = y;  // 记录连续区域两端的坐标
	if (st[x][y] == 0)  // 如果是空位
		return;  // 没有连续同色珠子
	int cnt = 1;  // (x, y)本身算一个
	while (1)
	{
		sx -= 1, sy += 1;  // 向右上方找珠子
		if (sx < 0 || sy >= col)  // 越界
		{
			sx++, sy--;  // 回溯
			break;  // 结束查找
		}
		if (st[sx][sy] == st[x][y])  // 同色
			cnt++;  // 数量增加
		else  // 不同色
		{
			sx++, sy--;  // 回溯
			break;  // 结束查找
		}
	}
	while (1)
	{
		fx += 1, fy -= 1;  // 向左下方找珠子
		if (fx >= row || fy < 0)  // 越界
		{
			fx--, fy++;  // 回溯
			break;  // 结束查找
		}
		if (st[fx][fy] == st[x][y])  // 同色
			cnt++;  // 数量增加
		else  // 不同色
		{
			fx--, fy++;  // 回溯
			break;  // 结束查找
		}
	}
	if (cnt >= 5)
	{
		for (int i = sx, j = sy; i <= fx && j >= fy; i++, j--)
			serial[i][j] = true;
	}
}
void chessboard::lean_left(const int x, const int y)
{
	int sx = x, sy = y, fx = x, fy = y;  // 记录连续区域两端的坐标
	if (st[x][y] == 0)  // 如果是空位
		return;  // 没有连续同色珠子
	int cnt = 1;  // (x, y)本身算一个
	while (1)
	{
		sx -= 1, sy -= 1;  // 向左上方找珠子
		if (sx < 0 || sy < 0)  // 越界
		{
			sx++, sy++;  // 回溯
			break;  // 结束查找
		}
		if (st[sx][sy] == st[x][y])  // 同色
			cnt++;  // 数量增加
		else  // 不同色
		{
			sx++, sy++;  // 回溯
			break;  // 结束查找
		}
	}
	while (1)
	{
		fx += 1, fy += 1;  // 向右下方找珠子
		if (fx >= row || fy >= col)  // 越界
		{
			fx--, fy--;  // 回溯
			break;  // 结束查找
		}
		if (st[fx][fy] == st[x][y])  // 同色
			cnt++;  // 数量增加
		else  // 不同色
		{
			fx--, fy--;  // 回溯
			break;  // 结束查找
		}
	}
	if (cnt >= 5)
	{
		for (int i = sx, j = sy; i <= fx && j <= fy; i++, j++)
			serial[i][j] = true;
	}
}

void chessboard::clear_step()
{
	// 清空步数标记的数组
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			step[i][j] = -1;
}
void chessboard::clear_serial()
{
	// 清空标记连续珠子位置的数组
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			serial[i][j] = false;
}
int chessboard::count_serial()
{
	// 统计连续珠子的数量
	int cnt = 0;
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			if (serial[i][j])
				cnt++;
	return cnt;
}

void chessboard::print_chessboard()
{
	// 输出棋盘数组
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			cout << st[i][j] << ' ';
		cout << "             " << endl;
	}
}
void chessboard::print_serial()
{
	// 输出连续珠子位置
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			cout << serial[i][j] << ' ';
		cout << endl;
	}
}

void chessboard::draw_chessboard()
{
	// 绘制空白棋盘┏━┓┃┗━┛┳┣╋┫┻
	cct_cls();
	int x = 1, y = 0;

	// 画上边框
	cct_gotoxy(y, x++);  // 光标移动到0列1行
	cout << "┏";
	for (int i = 0; i < row - 1; i++)
	{
		for (int j = 0; j < BEAD_SIZE; j++)
			cout << "━";
		cout << "┳";
	}
	for (int i = 0; i < BEAD_SIZE; i++)
		cout << "━";
	cout << "┓";

	// 画中间部分
	cct_gotoxy(y, x++);
	for (int i = 0; i < row - 1; i++)
	{
		for (int l = 0; l < BEAD_SIZE; l++)
		{
			cout << "┃";
			for (int j = 0; j < col - 1; j++)
			{
				for (int k = 0; k < BEAD_SIZE; k++)
					cout << "  ";
				cout << "┃";
			}
			for (int j = 0; j < BEAD_SIZE; j++)
				cout << "  ";
			cout << "┃" << i + 1;
			cct_gotoxy(y, x++);
		}

		cout << "┣";
		for (int j = 0; j < col - 1; j++)
		{
			for (int k = 0; k < BEAD_SIZE; k++)
				cout << "━";
			cout << "╋";
		}
		for (int j = 0; j < BEAD_SIZE; j++)
			cout << "━";
		cout << "┫";
		cct_gotoxy(y, x++);
	}
	for (int l = 0; l < BEAD_SIZE; l++)
	{
		cout << "┃";
		for (int j = 0; j < col - 1; j++)
		{
			for (int k = 0; k < BEAD_SIZE; k++)
				cout << "  ";
			cout << "┃";
		}
		for (int j = 0; j < BEAD_SIZE; j++)
			cout << "  ";
		cout << "┃" << row;
		cct_gotoxy(y, x++);
	}

	// 画下边框
	cout << "┗";
	for (int i = 0; i < row - 1; i++)
	{
		for (int j = 0; j < BEAD_SIZE; j++)
			cout << "━";
		cout << "┻";
	}
	for (int i = 0; i < BEAD_SIZE; i++)
		cout << "━";
	cout << "┛";

	// 列标
	cct_gotoxy(y, x);
	cout << "  ";
	for (int i = 0; i < row - 1; i++)
	{
		cout << setw(2) << i + 1;
		cout << "  ";
	}
	cout << setw(2) << col;
	cout << "  ";


	// 移动到棋盘之外的位置 防止后续输出覆盖棋盘
	goto_safe_area();
}
bool chessboard::draw_bead(const int x, const int y, bool highlight)
{
	// 绘制(x, y)处的珠子

	if (st[x][y] == 0)  // 如果位置为空
		return false;  // 绘制失败

	// 移动到棋盘图形对应的位置
	int xx = 1 + x * (BEAD_SIZE + 1) + 1;
	int yy = 2 + y * (2 * BEAD_SIZE + 2);
	cct_gotoxy(yy, xx);

	cct_setcolor(COLOR_BLACK, st[x][y] + 8 * highlight);  // 设置珠子颜色
	cout << "●";  // 绘制珠子
	cct_setcolor();  // 恢复颜色

	// 移动到棋盘之外的位置 防止后续输出覆盖棋盘
	goto_safe_area();

	return true;  // 绘制成功
}
bool chessboard::erase_bead(const int x, const int y)
{
	// 擦除(x, y)处的珠子

	if (st[x][y] != 0)  // 如果位置非空
		return false;  // 擦除失败

	// 移动到棋盘图形对应的位置
	int xx = 1 + x * (BEAD_SIZE + 1) + 1;
	int yy = 2 + y * (2 * BEAD_SIZE + 2);
	cct_gotoxy(yy, xx);

	cout << "  ";  // 擦除珠子

	// 移动到棋盘之外的位置 防止后续输出覆盖棋盘
	goto_safe_area();

	return true;  // 擦除成功
}
void chessboard::draw_blingbling(const int interval, const int time)
{
	// 闪烁显示要删除的珠子
	for (int k = 0; k < time; k++)
	{
		// 高亮
		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++)
				if (serial[i][j])
					draw_bead(i, j, true);
		// 暂停
		Sleep(interval);
		// 还原
		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++)
				if (serial[i][j])
					draw_bead(i, j);
		// 暂停
		Sleep(interval);
	}

	// 移动到棋盘之外的位置 防止后续输出覆盖棋盘
	goto_safe_area();
}