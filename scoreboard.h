#pragma once

#define SCOREBOARD_SIZE 100

class scoreboard {
public:
	scoreboard();  // 初始化记分板
	void add_score(const int x);  // 添加分数
	void sort_scores();  // 分数排序
	void show_scores();  // 输出分数

private:
	int p;
	int scores[SCOREBOARD_SIZE];
};