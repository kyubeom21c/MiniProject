#include "ATS_SIM.h"

ATS_SIM::ATS_SIM()
{
	mData = new MissionData;

	initialMission(mData);

	setFlag = 0;
	startFlag = 0;
}

ATS_SIM::~ATS_SIM()
{
	delete mData;
	mData = nullptr;
}

void ATS_SIM::setMission(MissionData* rData)
{
	double speed;

	if (setFlag == 0)
	{
		if (mData == nullptr)
		{
			mData = new MissionData;
		}

		setStartPos(rData->startPos);
		setEndPos(rData->endPos);
		setThreat(rData->threat);

		if (rData->threat == THREAT_MSL)
		{
			if (rData->velocity <= 1)
			{
				speed = 1;
			}
			else if (rData->velocity > 10)
			{
				speed = 10;
			}
			else
			{
				speed = rData->velocity;
			}
			setVelocity(speed);
		}
		if (rData->threat == THREAT_AIR)
		{
			if (rData->velocity <= 0.1)
			{
				speed = 0.1;
			}
			else if (rData->velocity > 1)
			{
				speed = 1;
			}
			else
			{
				speed = rData->velocity;
			}
			setVelocity(speed);
		}

		setFlag = 1;
	}
}

MissionPos ATS_SIM::startMove()
{
	MissionPos mPos = getCurPos(mData);

	mData->startPos.x = mPos.x;
	mData->startPos.y = mPos.y;

	return mPos;
}

void ATS_SIM::stopMission()
{
	deleteMission();

	startFlag = 0;
	setFlag = 0;
}

void ATS_SIM::deleteMission()
{
	if(!(mData == nullptr))
	{
		delete mData;
		mData = nullptr;
	}
}

void ATS_SIM::initialMission(MissionData* iData)
{
	iData->startPos.x = 0.0;
	iData->startPos.y = 0.0;
	iData->endPos.x = 0.0;
	iData->endPos.y = 0.0;
	iData->curPos.x = 0.0;
	iData->curPos.y = 0.0;
	iData->velocity = 0.0;
	iData->threat = 0;
	iData->sysCommand = "";
}

void ATS_SIM::setStartPos(MissionPos rData)
{
	mData->startPos.x = rData.x;
	mData->startPos.y = rData.y;

	cout << mData->startPos.x << " " << mData->startPos.y << endl;
}

void ATS_SIM::setEndPos(MissionPos rData)
{
	mData->endPos.x = rData.x;
	mData->endPos.y = rData.y;

	cout << mData->endPos.x << " " << mData->endPos.y << endl;
}

void ATS_SIM::setVelocity(double rData)
{
	mData->velocity = rData;

	cout << mData->velocity << endl;
}

void ATS_SIM::setThreat(int rData)
{
	mData->threat = rData;

	cout << mData->threat << endl;
}

MissionPos ATS_SIM::getCurPos(MissionData* gData)
{
	double xPosDiff = gData->endPos.x - gData->startPos.x;
	double yPosDiff = gData->endPos.y - gData->startPos.y;

	double speed = gData->velocity * METER_PER_SEC; // 마하 초속
	double angle = atan2(yPosDiff, xPosDiff);
	
	gData->curPos.x = gData->startPos.x + ((speed * cos(angle)) * 0.1); //(0.1 sec)
	gData->curPos.y = gData->startPos.y + ((speed * sin(angle)) * 0.1); //(0.1 sec)

	gData->startPos.x = gData->curPos.x;
	gData->startPos.y = gData->curPos.y;

	return gData->startPos;
}