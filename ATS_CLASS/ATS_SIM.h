#pragma once
#include <string>
#include <math.h>
#include <iostream>
#include "ATS_DEF.h"

#define THREAT_MSL		0
#define THREAT_AIR		1
#define METER_PER_SEC	340

#define UPDATE_TIME		0.1

using namespace std;

class ATS_SIM
{
public:
	ATS_SIM();
	~ATS_SIM();
	void setMission(MissionData* rData);
	MissionPos startMove();
	void stopMission();
	void initialMission(MissionData* iData);

private:
	void setStartPos(MissionPos rData);
	void setEndPos(MissionPos rData);
	void setVelocity(double rData);
	void setThreat(int rData);
	void deleteMission();
	MissionPos getCurPos(MissionData* gData);

private:
	MissionData *mData;
	int setFlag;
	int startFlag;
};