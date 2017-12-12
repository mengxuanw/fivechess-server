#include "StdAfx.h"
#include "Judge.h"


CJudge::CJudge(void)
{
	for(int i = 0; i < 17; i ++)
		for(int j = 0; j < 17; j ++)
			flag[i][j] = -1;
}


CJudge::~CJudge(void)
{
}

void CJudge::changeFlag(int x, int y, int color)
{
	flag[x][y] = color;
	//color = 0, white
	//color = 1, black
}

int CJudge::arrayLine(vector<int>a)
{
	int begin = -1;
	int num = 0;
	for( int i = 0; i < a.size() - 5; i ++)
	{
		if(a[i] != -1)
		{
			begin = i;
			break;
		}
	}
	if(begin == -1)
		return -1;
	for( int j = begin; j < a.size() - 1; j++)
	{
		if(a[j] == a[j+1] && a[j] != -1)
			num ++;
		else
			num = 0;
		if(num == 4)
			return a[j];
	}
	return -1;
}

int CJudge::is_Win()
{
	int num = 0;
	int begin = 0;
	vector<int> a;
	for(int i = 0; i < 17; i ++)
	{
		//按行检测
		a.clear();
		for(int j = 0; j < 17; j++)
			a.push_back(flag[i][j]);
		int p = arrayLine(a);
		if(p != -1)
			return p;
	}
	for(int i = 0; i < 17; i ++)
	{
		//按列检测
		a.clear();
		for(int j = 0; j < 17; j++)
			a.push_back(flag[j][i]);
		int p = arrayLine(a);
		if(p != -1)
			return p;
	}
	//斜对角1
	for(int i = 0; i < 12; i ++)
	{
		a.clear();
		for(int j = 0; j < 16 - i; j ++)
			a.push_back(flag[i+j][j]);
		int p = arrayLine(a);
		if(p != -1)
			return p;
	}
	//斜对角2
	for(int i = 0; i < 12; i ++)
	{
		a.clear();
		for(int j = 0; j < 16 - i; j ++)
			a.push_back(flag[j][i+j]);
		int p = arrayLine(a);
		if(p != -1)
			return p;
	}
	//斜对角3
	for(int i = 0; i < 12; i ++)
	{
		a.clear();
		for(int j = 0; j < 16 - i; j ++)
			a.push_back(flag[i+j][16-j]);
		int p = arrayLine(a);
		if(p != -1)
			return p;
	}
	//斜对角4
	for(int i = 0; i < 12; i ++)
	{
		a.clear();
		for(int j = 0; j < 16 - i; j++)
			a.push_back(flag[j][16-i -j]);
		int p = arrayLine(a);
		if(p != -1)
			return p;
	}
	return -1;
}
