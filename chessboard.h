#pragma once

#define CHESSBOARD_ROW 9
#define CHESSBOARD_COL 9
#define BEAD_SIZE 1  // ��Ҫ�޸�

class chessboard {
public:
	chessboard();  // ��������

	bool is_filled();  // ����������
	bool is_empty();  // �����ǿյ�

	void rand_bead(const int num);  // �����������
	bool move_bead(const int sx, const int sy, const int fx, const int fy);  // �ƶ�����
	bool add_bead(const int x, const int y, const int c);  // ָ���µ����ӣ����ڵ��ԣ�
	bool del_bead(const int x, const int y);  // ɾ������

	void horizontal(const int x, const int y);  // ��Ǻ�������
	void vertical(const int x, const int y);  // �����������
	void lean_right(const int x, const int y);  // �����б����
	void lean_left(const int x, const int y);  // �����б����

	void clear_step();  // ��ղ������ ��Ϊ-1
	void clear_serial();  // ��ձ����������λ�õ�����
	int count_serial();  // ͳ���������ӵ�����

	void print_chessboard();  // ��ӡ���̣����ڵ��ԣ�
	void print_serial();  // ��ӡ��������λ�ã����ڵ��ԣ�

	void draw_chessboard();  // ���ƿ�����
	bool draw_bead(const int x, const int y, bool highlight = false);  // ��������
	bool erase_bead(const int x, const int y);  // ��ȥ����
	void draw_blingbling(const int interval = 300, const int times = 3);  // ��˸��ʾҪɾ��������

private:
	int row, col;

	int st[CHESSBOARD_ROW][CHESSBOARD_COL];
	int step[CHESSBOARD_ROW][CHESSBOARD_COL];
	bool serial[CHESSBOARD_ROW][CHESSBOARD_COL];

	friend int game(bool use_mouse);
};