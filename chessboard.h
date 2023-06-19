#pragma once

#define CHESSBOARD_ROW 9
#define CHESSBOARD_COL 9
#define BEAD_SIZE 1  // 不要修改

class chessboard {
public:
	chessboard();  // 析构函数

	bool is_filled();  // 棋盘是满的
	bool is_empty();  // 棋盘是空的

	void rand_bead(const int num);  // 随机生成珠子
	bool move_bead(const int sx, const int sy, const int fx, const int fy);  // 移动珠子
	bool add_bead(const int x, const int y, const int c);  // 指定新的珠子（用于调试）
	bool del_bead(const int x, const int y);  // 删除珠子

	void horizontal(const int x, const int y);  // 标记横向连珠
	void vertical(const int x, const int y);  // 标记纵向连珠
	void lean_right(const int x, const int y);  // 标记右斜连珠
	void lean_left(const int x, const int y);  // 标记左斜连珠

	void clear_step();  // 清空步数标记 设为-1
	void clear_serial();  // 清空标记连续珠子位置的数组
	int count_serial();  // 统计连续珠子的数量

	void print_chessboard();  // 打印棋盘（用于调试）
	void print_serial();  // 打印连续珠子位置（用于调试）

	void draw_chessboard();  // 绘制空棋盘
	bool draw_bead(const int x, const int y, bool highlight = false);  // 绘制珠子
	bool erase_bead(const int x, const int y);  // 擦去珠子
	void draw_blingbling(const int interval = 300, const int times = 3);  // 闪烁显示要删除的珠子

private:
	int row, col;

	int st[CHESSBOARD_ROW][CHESSBOARD_COL];
	int step[CHESSBOARD_ROW][CHESSBOARD_COL];
	bool serial[CHESSBOARD_ROW][CHESSBOARD_COL];

	friend int game(bool use_mouse);
};