#pragma once

#define SCOREBOARD_SIZE 100

class scoreboard {
public:
	scoreboard();  // ��ʼ���Ƿְ�
	void add_score(const int x);  // ��ӷ���
	void sort_scores();  // ��������
	void show_scores();  // �������

private:
	int p;
	int scores[SCOREBOARD_SIZE];
};