#include "scoreboard.h"
#include "game.h"
#include "cmd_console_tools.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

scoreboard::scoreboard()
{
	for (int i = 0; i < SCOREBOARD_SIZE; i++)  // ��ʼ������
		scores[i] = 0;
	p = -1;  //  ��ʼ��ָ��
}

void scoreboard::add_score(const int x)
{
	// ��ӷ�����¼
	scores[++p] = x;
}
void scoreboard::sort_scores()
{
	// ����������
	sort(scores, scores + p + 1, my_comp);
}
void scoreboard::show_scores()
{
	// ���������¼���������������שǩ�ϩ�

	cct_cls();
	cct_gotoxy(0, 0);

	cout << "���������ש�������" << endl;
	cout << "�� ���� �� ���� ��" << endl;

	if (p == -1)
	{
		cout << "���������ߩ�������" << endl;
		return;
	}
	else
		cout << "�ǩ������贈������" << endl;

	for (int i = 0; i < p; i++)
	{
		cout << "��" << "No." << setw(3) << i + 1 << "��" << setw(6) << scores[i] << "��" << endl;
		cout << "�ǩ������贈������" << endl;
	}
	cout << "��" << "No." << setw(3) << p + 1 << "��" << setw(6) << scores[p] << "��" << endl;
	cout << "���������ߩ�������" << endl;
}

