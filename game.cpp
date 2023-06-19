#include <iostream>
#include <windows.h>
#include <conio.h>
#include "game.h"
#include "cmd_console_tools.h"
#include "chessboard.h"
using namespace std;

char menu()
{
	// ��ʾ�˵�

	cct_cls();

	cout << "    ��������������    " << endl;
	cout << "    ��#��������#��    " << endl;
	cout << "    ��������������    " << endl;

	cout << "----------------------" << endl;
	cout << "1.��ʼ��Ϸ����������" << endl;
	cout << "----------------------" << endl;
	cout << "2.��ʼ��Ϸ�����̲�����" << endl;
	cout << "----------------------" << endl;
	cout << "3.�鿴�÷ּ�¼        " << endl;
	cout << "----------------------" << endl;
	cout << "4.�鿴��Ϸ˵��        " << endl;
	cout << "----------------------" << endl;
	cout << "0.�˳�                " << endl;
	cout << "----------------------" << endl;
	cout << "[��ѡ��:] ";

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
	//������ʼ��
	goto_safe_area();
	while (1)
	{
		cout << "��������ʼ����(" << 1 << '-' << CHESSBOARD_ROW << ")��";
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

	//������ʼ��
	goto_safe_area();
	while (1)
	{
		cout << "��������ʼ����(" << 1 << '-' << CHESSBOARD_COL << ")��";
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

	//����Ŀ����
	goto_safe_area();
	while (1)
	{
		cout << "������Ŀ������(" << 1 << '-' << CHESSBOARD_ROW << ")��";
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

	//����Ŀ����
	goto_safe_area();
	while (1)
	{
		cout << "������Ŀ������(" << 1 << '-' << CHESSBOARD_COL << ")��";
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

	return false;  // ���˳���Ϸ
}
bool my_comp(int a, int b)
{
	return a > b;
}
void goto_safe_area()
{
	// �ƶ�������֮���λ�� ��ֹ���������������
	cct_gotoxy(0, 1 + CHESSBOARD_ROW * (BEAD_SIZE + 1) + 2);
}
void clear_extra()
{
	// ��������ʱ ������ջ�����������Ļ�Ļ���
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
	cout << "��ǰ�÷֣�" << score;
}
void game_discription()
{
	cout << "--------------" << endl;
	cout << "����Ϸ��˵����" << endl;
	cout << "-------------------------------------------------" << endl;
	cout << "��飺" << endl;
	cout << "    ���̴�С��9X9��һ�������6����ɫ�����ӡ���ʼ״̬������7�����λ�÷ֲ���7�������ɫ�����ӡ�ͨ���ƶ����ӽ�ͬɫ����������һ����������÷֡��������ӿ����ƶ�ʱ�����������Ϸ��¼������Ϸ�ߵ÷ֲ�����" << endl << endl;
	cout << "����" << endl;
	cout << "    1.һ��ֻ�����ƶ�һ�����ӣ�ֻҪ�������п�λ�þͿ����ƶ����ӡ�" << endl;
	cout << "    2.ÿ�ƶ�һ�������Ժ����������ͬɫ��5�������������������3�������ɫ���ӷֲ�������������õ�λ���ϡ�" << endl;
	cout << "    3.���ͬɫ����������5�ż���������һ���ųɺ����������б��ʱ����Щ���Ӵ���������ʧ��������3�����ӡ�ͬʱ��Ϸ�߿��Ե÷֣�ÿ������2�֡�" << endl;
	cout << "    4.�����̱�����ռ��ʱ��Ϸ������" << endl;
	cout << "-------------------------------------------------" << endl;
	cout << "�����������..." << endl;
	cout << "---------------" << endl;
}

int game(bool use_mouse)
{
	chessboard cb;  // ��������
	cb.draw_chessboard();  // ��������

	int player_score = 0;  // ������Ϸ��ҷ���
	show_score_now(player_score);  // ��ʾ�÷�

	bool game_end = false;  // �Ƿ�Ҫ������Ϸ

	// ����
	while (1)
	{
		cb.rand_bead(7);  // ��Ϸ�����������7������
		for (int i = 0; i < cb.row; i++)  // ����ÿһ��
			for (int j = 0; j < cb.col; j++)  // ����ÿһ��
				if (cb.st[i][j] != 0)  // λ�÷ǿ�
					cb.draw_bead(i, j);  // �����������

		// ����7�����ӼǷ�
		for (int i = 0; i < cb.row; i++)  // ����ÿһ��
			for (int j = 0; j < cb.col; j++)  // ����ÿһ��
				if (cb.st[i][j] != 0)  // �ҵ�����
				{
					cb.clear_serial();  // ��տ�����λ�ñ��

					// ���ҿ�����λ��
					cb.horizontal(i, j);
					cb.vertical(i, j);
					cb.lean_left(i, j);
					cb.lean_right(i, j);

					int num_bead = cb.count_serial();  // ����������������

					if (num_bead != 0)  // ��������
					{
						player_score += 2 * num_bead;  // �Ƿ�
						show_score_now(player_score);  // ��ʾ�÷�

						cb.draw_blingbling();  // ��˸��ʾҪ����������

						for (int i = 0; i < cb.row; i++)
							for (int j = 0; j < cb.col; j++)
								if (cb.serial[i][j])  // �ҵ�����������
								{
									cb.del_bead(i, j);  // ������������ɾ��
									cb.erase_bead(i, j);  // �ڽ����ϲ���
								}

						cb.clear_serial();  // ��տ�����λ�ñ��
					}
				}

		if (cb.is_empty())  // �����������ʼ��7������ȫ���Զ����������ʼ��ͣ�
		{
			player_score = 0;  // �������� ���¿�ʼ
			continue;  // ��������7������
		}
		else
			break;
	}

	// ��Ҳ���
	while (!cb.is_filled())  // ����û�з����������Ϸ
	{
		int sx = 0, sy = 0, fx = 0, fy = 0;  // �����յ������

		if (use_mouse)  // ʹ�����
		{
			cct_enable_mouse();

			while (1)  // ��ȡ�������
			{
				int MX, MY, MAction, keycode1, keycode2, cct_return;
				cct_return = cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2);

				if (cct_return == CCT_MOUSE_EVENT)  // ������¼�
				{
					if (MAction == MOUSE_LEFT_BUTTON_CLICK)  // �������
					{
						sx = (MY - 2) / (BEAD_SIZE + 1);
						sy = (MX - 2) / (BEAD_SIZE + 1) / 2;
						cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2);  // ��ȡ�ɿ����
						break;
					}
					else if (MAction == MOUSE_RIGHT_BUTTON_CLICK)  // �����Ҽ�
					{
						game_end = true;//������Ϸ
						break;
					}
					else  // ����������
						continue;  // ��Ч���� ������ȡ���
				}
			}

			if (game_end)
				break;  // ������Ϸ

			while (1)  // ��ȡ�յ�����
			{
				int MX, MY, MAction, keycode1, keycode2, cct_return;
				cct_return = cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2);

				if (cct_return == CCT_MOUSE_EVENT)  // ������¼�
				{
					if (MAction == MOUSE_LEFT_BUTTON_CLICK)  // �������
					{
						fx = (MY - 2) / (BEAD_SIZE + 1);
						fy = (MX - 2) / (BEAD_SIZE + 1) / 2;
						cct_read_keyboard_and_mouse(MX, MY, MAction, keycode1, keycode2);  // ��ȡ�ɿ����
						break;
					}
					else if (MAction == MOUSE_RIGHT_BUTTON_CLICK)  // �����Ҽ�
					{
						game_end = true;//������Ϸ
						break;
					}
					else  // ����������
						continue;  // ��Ч���� ������ȡ���
				}
			}

			if (game_end)
				break;  // ������Ϸ

			cct_disable_mouse();
		}
		else  // ʹ�ü���
		{
			get_input(sx, sy, fx, fy);
			clear_extra();
		}

		if (cb.move_bead(sx, sy, fx, fy))  // �ƶ��Ϸ�
		{
			//cb.erase_bead(sx, sy);  // ����ʼλ�����Ӳ���
			//cb.draw_bead(fx, fy);  // ��Ŀ��λ�û�������

			// ������ƶ��Ƿ�
			cb.clear_serial();  // ��տ�����λ�ñ��

			// ���ҿ�����λ��
			cb.horizontal(fx, fy);
			cb.vertical(fx, fy);
			cb.lean_left(fx, fy);
			cb.lean_right(fx, fy);

			int num_bead = cb.count_serial();  // ����������������

			if (num_bead != 0)  // ��������
			{
				player_score += 2 * num_bead;  // �Ƿ�
				show_score_now(player_score);  // ��ʾ�÷�

				cb.draw_blingbling();  // ��˸��ʾҪ����������

				for (int i = 0; i < cb.row; i++)
					for (int j = 0; j < cb.col; j++)
						if (cb.serial[i][j])  // �ҵ�Ҫ����������
						{
							cb.del_bead(i, j);  // ������������ɾ��
							cb.erase_bead(i, j);  // �ڽ����ϲ���
						}

				cb.clear_serial();  // ��տ�����λ�ñ��
			}
			else  // ��������
			{
				cb.rand_bead(3);  // �������3������

				// ������3�����ӣ�ʵ���ǻ����������� ��ӻ�����3����
				for (int i = 0; i < cb.row; i++)
					for (int j = 0; j < cb.col; j++)
						if (cb.st[i][j])
							cb.draw_bead(i, j);

				// ����3�����ӼǷ�
				for (int i = 0; i < cb.row; i++)  // ����ÿһ��
					for (int j = 0; j < cb.col; j++)  // ����ÿһ��
						if (cb.st[i][j] != 0)  // �ҵ�����
						{
							cb.clear_serial();  // ��տ�����λ�ñ��

							// ���ҿ�����λ��
							cb.horizontal(i, j);
							cb.vertical(i, j);
							cb.lean_left(i, j);
							cb.lean_right(i, j);

							int num_bead = cb.count_serial();  // ����������������

							if (num_bead != 0)  // ��������
							{
								player_score += 2 * num_bead;  // �Ƿ�
								show_score_now(player_score);  // ��ʾ�÷�

								cb.draw_blingbling();  // ��˸��ʾҪ����������

								for (int i = 0; i < cb.row; i++)
									for (int j = 0; j < cb.col; j++)
										if (cb.serial[i][j])  // �ҵ�����������
										{
											cb.del_bead(i, j);  // ������������ɾ��
											cb.erase_bead(i, j);  // �ڽ����ϲ���
										}

								cb.clear_serial();  // ��տ�����λ�ñ��
							}
						}
			}
		}
		else  // �ƶ����Ϸ�
		{
			goto_safe_area();  // ����ƶ�����ȫ����
			cout << "�ƶ����Ϸ���" << endl;  // �Ƿ��ƶ�����˵��
			Sleep(1000);  // ��ͣһ��
			goto_safe_area();  // ����ƶ������ֿ�ͷ
			cout << "            " << endl;  // ��������˵��
		}
	}

	// ����
	return player_score;  // ���̷��� ��Ϸ���� ���ݵ÷�
}