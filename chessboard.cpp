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
	// ��ʼ���հ�����
	row = CHESSBOARD_ROW;  // ��ʼ����������
	col = CHESSBOARD_COL;  // ��ʼ����������

	for (int i = 0; i < row; i++)  // ��ʼ���հ�����
		for (int j = 0; j < col; j++)
		{
			st[i][j] = 0;
			step[i][j] = -1;
			serial[i][j] = false;
		}
}

bool chessboard::is_filled()
{
	// �ж������Ƿ�������
	for (int i = 0; i < row; i++)  // ����ÿһ��
		for (int j = 0; j < col; j++)  // ����ÿһ��
			if (st[i][j] == 0)  // �п�λ
				return false;  // ��������
	return true;  // ������
}
bool chessboard::is_empty()
{
	// �ж������Ƿ��ǿյ�
	for (int i = 0; i < row; i++)  // ����ÿһ��
		for (int j = 0; j < col; j++)  // ����ÿһ��
			if (st[i][j] != 0)  // ������
				return false;  // ���ǿյ�
	return true;  // �ǿյ�
}

void chessboard::rand_bead(const int num)
{
	// �������num������
	for (int i = 0; i < num; i++)
	{
		if (is_filled())  // ��������Ѿ�������
			return;  // ����

		int x = rand() % row;  // ��������б꣨0--row-1��
		int y = rand() % col;  // ��������б꣨0--col-1��
		if (st[x][y] == 0)  // ��λ��Ϊ��
		{
			int color = rand() % 6 + 1;  // ���������ɫ��1--6��
			st[x][y] = color;  // ��������
		}
		else  // ��λ�÷ǿ�
			i--;  // ������λ��
	}
}
bool chessboard::move_bead(const int sx, const int sy, const int fx, const int fy)
{
	// ��(sx, sy)�ƶ���(fx, fy)

	clear_step();  // ��վ���

	if (st[sx][sy] == 0 || st[fx][fy] != 0)  // ��ʼλ��Ϊ�ջ�Ŀ��λ�÷ǿ�
		return false;  // �ƶ�ʧ��

	pii q[CHESSBOARD_COL * CHESSBOARD_ROW];  // �������
	int head = 0, tail = -1;  // ��ͷ�Ͷ�β
	q[++tail] = { sx, sy };  // ���г�ʼ��

	
	step[sx][sy] = 0;  // ���Ϊ��0��

	int nx[4] = { 0, 0, 1, -1 };  // �ĸ�����
	int ny[4] = { 1, -1, 0, 0 };  // �ĸ�����

	while (head <= tail)
	{
		pii t = q[head++];  // ȡ����ͷ
		for (int i = 0; i < 4; i++)  // �����ĸ�����
		{
			int n_x = t.first + nx[i];  // ��һ���������
			int n_y = t.second + ny[i];  // ��һ���������

			bool inside = (n_x >= 0 && n_x < CHESSBOARD_ROW&& n_y >= 0 && n_y < CHESSBOARD_COL);  // �Ƿ��������ڲ�

			if (inside && step[n_x][n_y] == -1 && st[n_x][n_y] == 0)  // ��Խ�� û�߹� �ǿ�λ
			{
				step[n_x][n_y] = step[t.first][t.second] + 1;  // ȷ��Ϊ��һ��
				q[++tail] = { n_x, n_y };  // ���
			}
		}
	}

	if (step[fx][fy] == -1)  // û���ߵ�Ŀ��λ��
		return false;  // �ƶ�����

	pii q_back[CHESSBOARD_ROW * CHESSBOARD_COL];  // ���·������
	int p = -1;  // ָ��
	q_back[++p] = {fx, fy};  // �����յ�λ��
	// ���·����
	int xx = fx, yy = fy;
	while (1)
	{
		for (int i = 0; i < 4; i++)
		{
			int n_x = xx + nx[i];  // ��һ���������
			int n_y = yy + ny[i];  // ��һ���������

			bool inside = (n_x >= 0 && n_x < CHESSBOARD_ROW&& n_y >= 0 && n_y < CHESSBOARD_COL);  // �Ƿ��������ڲ�

			if (inside && step[n_x][n_y] == step[xx][yy] - 1)  // δԽ�� ������1
			{
				q_back[++p] = { n_x, n_y };  // ��������
				xx = n_x;  // ����
				yy = n_y;  // ����
				break;
			}
		}
		if (xx == sx && yy == sy)  // �ҵ����
			break;  // ��������
	}
	// �������·
	pii last_bead = q_back[p--];  // ���òο�λ��
	while (p >= 0)
	{
		pii now_bead = q_back[p--];  // ��ȡ��λ��

		st[now_bead.first][now_bead.second] = st[last_bead.first][last_bead.second];  // ������ɫ��ֵ

		del_bead(last_bead.first, last_bead.second);  // ɾ��ԭʼλ��
		erase_bead(last_bead.first, last_bead.second);  // ����ԭλ������
		Sleep(100);  // ��ͣһ��

		draw_bead(now_bead.first, now_bead.second);  // ����������
		Sleep(100);  // ��ͣһ��

		last_bead = now_bead;  // ���²ο�λ��
	}

	return true;  // �ƶ��ɹ�
}
bool chessboard::add_bead(const int x, const int y, const int c)
{
	// ��ָ��λ�����ָ����ɫ������
	if (st[x][y] != 0)  // ָ��λ�÷ǿ�
		return false;  // ��ӳ���
	else  // ָ��λ��Ϊ��
	{
		st[x][y] = c;  // ȷ����ɫ
		return true;  // ��ӳɹ�
	}
}
bool chessboard::del_bead(const int x, const int y)
{
	// ɾ��ָ��λ�õ�����
	if (st[x][y] == 0)  // λ��Ϊ��
		return false;  // ɾ��ʧ��
	else  // λ�÷ǿ�
	{
		st[x][y] = 0;  // ɾ������
		return true;  // ɾ���ɹ�
	}
}

void chessboard::horizontal(const int x, const int y)
{
	int sx = x, sy = y, fx = x, fy = y;  // ��¼�����������˵�����
	if (st[x][y] == 0)  // ����ǿ�λ
		return;  // û������ͬɫ����
	int cnt = 1;  // (x, y)������һ��
	while (1)
	{
		sy -= 1;  // ����������
		if (sy < 0)  // Խ��
		{
			sy++;  // ����
			break;  // ��������
		}
		if (st[sx][sy] == st[x][y])  // ͬɫ
			cnt++;  // ��������
		else  // ��ͬɫ
		{
			sy++;  // ����
			break;  // ��������
		}
	}
	while (1)
	{
		fy += 1;  // ����������
		if (fy >= col)  // Խ��
		{
			fy--;  // ����
			break;  // ��������
		}
		if (st[fx][fy] == st[x][y])  // ͬɫ
			cnt++;  // ��������
		else  // ��ͬɫ
		{
			fy--;  // ����
			break;  // ��������
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
	int sx = x, sy = y, fx = x, fy = y;  // ��¼�����������˵�����
	if (st[x][y] == 0)  // ����ǿ�λ
		return;  // û������ͬɫ����
	int cnt = 1;  // (x, y)������һ��
	while (1)
	{
		sx -= 1;  // ����������
		if (sx < 0)  // Խ��
		{
			sx++;  // ����
			break;  // ��������
		}
		if (st[sx][sy] == st[x][y])  // ͬɫ
			cnt++;  // ��������
		else  // ��ͬɫ
		{
			sx++;  // ����
			break;  // ��������
		}
	}
	while (1)
	{
		fx += 1;  // ����������
		if (fx >= row)  // Խ��
		{
			fx--;  // ����
			break;  // ��������
		}
		if (st[fx][fy] == st[x][y])  // ͬɫ
			cnt++;  // ��������
		else  // ��ͬɫ
		{
			fx--;  // ����
			break;  // ��������
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
	int sx = x, sy = y, fx = x, fy = y;  // ��¼�����������˵�����
	if (st[x][y] == 0)  // ����ǿ�λ
		return;  // û������ͬɫ����
	int cnt = 1;  // (x, y)������һ��
	while (1)
	{
		sx -= 1, sy += 1;  // �����Ϸ�������
		if (sx < 0 || sy >= col)  // Խ��
		{
			sx++, sy--;  // ����
			break;  // ��������
		}
		if (st[sx][sy] == st[x][y])  // ͬɫ
			cnt++;  // ��������
		else  // ��ͬɫ
		{
			sx++, sy--;  // ����
			break;  // ��������
		}
	}
	while (1)
	{
		fx += 1, fy -= 1;  // �����·�������
		if (fx >= row || fy < 0)  // Խ��
		{
			fx--, fy++;  // ����
			break;  // ��������
		}
		if (st[fx][fy] == st[x][y])  // ͬɫ
			cnt++;  // ��������
		else  // ��ͬɫ
		{
			fx--, fy++;  // ����
			break;  // ��������
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
	int sx = x, sy = y, fx = x, fy = y;  // ��¼�����������˵�����
	if (st[x][y] == 0)  // ����ǿ�λ
		return;  // û������ͬɫ����
	int cnt = 1;  // (x, y)������һ��
	while (1)
	{
		sx -= 1, sy -= 1;  // �����Ϸ�������
		if (sx < 0 || sy < 0)  // Խ��
		{
			sx++, sy++;  // ����
			break;  // ��������
		}
		if (st[sx][sy] == st[x][y])  // ͬɫ
			cnt++;  // ��������
		else  // ��ͬɫ
		{
			sx++, sy++;  // ����
			break;  // ��������
		}
	}
	while (1)
	{
		fx += 1, fy += 1;  // �����·�������
		if (fx >= row || fy >= col)  // Խ��
		{
			fx--, fy--;  // ����
			break;  // ��������
		}
		if (st[fx][fy] == st[x][y])  // ͬɫ
			cnt++;  // ��������
		else  // ��ͬɫ
		{
			fx--, fy--;  // ����
			break;  // ��������
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
	// ��ղ�����ǵ�����
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			step[i][j] = -1;
}
void chessboard::clear_serial()
{
	// ��ձ����������λ�õ�����
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			serial[i][j] = false;
}
int chessboard::count_serial()
{
	// ͳ���������ӵ�����
	int cnt = 0;
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			if (serial[i][j])
				cnt++;
	return cnt;
}

void chessboard::print_chessboard()
{
	// �����������
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			cout << st[i][j] << ' ';
		cout << "             " << endl;
	}
}
void chessboard::print_serial()
{
	// �����������λ��
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			cout << serial[i][j] << ' ';
		cout << endl;
	}
}

void chessboard::draw_chessboard()
{
	// ���ƿհ����̩��������������שǩ�ϩ�
	cct_cls();
	int x = 1, y = 0;

	// ���ϱ߿�
	cct_gotoxy(y, x++);  // ����ƶ���0��1��
	cout << "��";
	for (int i = 0; i < row - 1; i++)
	{
		for (int j = 0; j < BEAD_SIZE; j++)
			cout << "��";
		cout << "��";
	}
	for (int i = 0; i < BEAD_SIZE; i++)
		cout << "��";
	cout << "��";

	// ���м䲿��
	cct_gotoxy(y, x++);
	for (int i = 0; i < row - 1; i++)
	{
		for (int l = 0; l < BEAD_SIZE; l++)
		{
			cout << "��";
			for (int j = 0; j < col - 1; j++)
			{
				for (int k = 0; k < BEAD_SIZE; k++)
					cout << "  ";
				cout << "��";
			}
			for (int j = 0; j < BEAD_SIZE; j++)
				cout << "  ";
			cout << "��" << i + 1;
			cct_gotoxy(y, x++);
		}

		cout << "��";
		for (int j = 0; j < col - 1; j++)
		{
			for (int k = 0; k < BEAD_SIZE; k++)
				cout << "��";
			cout << "��";
		}
		for (int j = 0; j < BEAD_SIZE; j++)
			cout << "��";
		cout << "��";
		cct_gotoxy(y, x++);
	}
	for (int l = 0; l < BEAD_SIZE; l++)
	{
		cout << "��";
		for (int j = 0; j < col - 1; j++)
		{
			for (int k = 0; k < BEAD_SIZE; k++)
				cout << "  ";
			cout << "��";
		}
		for (int j = 0; j < BEAD_SIZE; j++)
			cout << "  ";
		cout << "��" << row;
		cct_gotoxy(y, x++);
	}

	// ���±߿�
	cout << "��";
	for (int i = 0; i < row - 1; i++)
	{
		for (int j = 0; j < BEAD_SIZE; j++)
			cout << "��";
		cout << "��";
	}
	for (int i = 0; i < BEAD_SIZE; i++)
		cout << "��";
	cout << "��";

	// �б�
	cct_gotoxy(y, x);
	cout << "  ";
	for (int i = 0; i < row - 1; i++)
	{
		cout << setw(2) << i + 1;
		cout << "  ";
	}
	cout << setw(2) << col;
	cout << "  ";


	// �ƶ�������֮���λ�� ��ֹ���������������
	goto_safe_area();
}
bool chessboard::draw_bead(const int x, const int y, bool highlight)
{
	// ����(x, y)��������

	if (st[x][y] == 0)  // ���λ��Ϊ��
		return false;  // ����ʧ��

	// �ƶ�������ͼ�ζ�Ӧ��λ��
	int xx = 1 + x * (BEAD_SIZE + 1) + 1;
	int yy = 2 + y * (2 * BEAD_SIZE + 2);
	cct_gotoxy(yy, xx);

	cct_setcolor(COLOR_BLACK, st[x][y] + 8 * highlight);  // ����������ɫ
	cout << "��";  // ��������
	cct_setcolor();  // �ָ���ɫ

	// �ƶ�������֮���λ�� ��ֹ���������������
	goto_safe_area();

	return true;  // ���Ƴɹ�
}
bool chessboard::erase_bead(const int x, const int y)
{
	// ����(x, y)��������

	if (st[x][y] != 0)  // ���λ�÷ǿ�
		return false;  // ����ʧ��

	// �ƶ�������ͼ�ζ�Ӧ��λ��
	int xx = 1 + x * (BEAD_SIZE + 1) + 1;
	int yy = 2 + y * (2 * BEAD_SIZE + 2);
	cct_gotoxy(yy, xx);

	cout << "  ";  // ��������

	// �ƶ�������֮���λ�� ��ֹ���������������
	goto_safe_area();

	return true;  // �����ɹ�
}
void chessboard::draw_blingbling(const int interval, const int time)
{
	// ��˸��ʾҪɾ��������
	for (int k = 0; k < time; k++)
	{
		// ����
		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++)
				if (serial[i][j])
					draw_bead(i, j, true);
		// ��ͣ
		Sleep(interval);
		// ��ԭ
		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++)
				if (serial[i][j])
					draw_bead(i, j);
		// ��ͣ
		Sleep(interval);
	}

	// �ƶ�������֮���λ�� ��ֹ���������������
	goto_safe_area();
}