#pragma once
#include <vector>
using namespace std;

class CJudge
{
private:
	int flag[17][17];
public:
	CJudge(void);
	~CJudge(void);
	void changeFlag(int x, int y, int color);
	int is_Win();
	int arrayLine(vector<int>a);
};

