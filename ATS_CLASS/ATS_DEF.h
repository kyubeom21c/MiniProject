#pragma once
#include <string>

using namespace std;

struct MissionPos
{
	double x;
	double y;
};

struct MissionData
{
	MissionPos startPos;
	MissionPos endPos;
	MissionPos curPos;
	double velocity;
	int threat;
	string sysCommand;
};

struct SendData
{
	MissionPos curPos;
	string sysState;
};
