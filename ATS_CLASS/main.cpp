#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include "ATS_DEF.h"
#include "ATS_SIM.h"
#include "ATS_COM.h"

ATS_COM* sndPosCom;
ATS_COM* sndStsCom;
ATS_COM* rcvCom;
ATS_SIM* atsSim;

thread* thMission;
thread* thSend;

MissionData* mData;
MissionPos mPos;
SendData sData;

int startFlag;
int setFlag;
int stateFlag;

void mission()
{
	cout << "���� ��� :: ���߸��� ���� ����" << endl;
	
	while (1)
	{
		if (startFlag == 1)
		{
			mPos = atsSim->startMove();

			if ((mPos.x >= mData->endPos.x) && (mPos.y >= mData->endPos.y))
			{
				mPos.x = mData->endPos.x;
				mPos.y = mData->endPos.y;

				sData.curPos = mPos;

				sndPosCom->sendInformation(sData);

				cout << "Pos_X : " << to_string(mPos.x) << " Pos_Y : " << to_string(mPos.y) << endl;

				atsSim->stopMission();
				atsSim->initialMission(mData);

				startFlag = 0;
				setFlag = 0;

				cout << "��ǥ���� ���� :: ���߸��� ���� ����" << endl;

				break;
			}

			cout << "Pos_X : " << to_string(mPos.x) << " Pos_Y : " << to_string(mPos.y) << endl;

			sData.curPos = mPos;

			sndPosCom->sendInformation(sData);

			this_thread::sleep_for(chrono::milliseconds(100));
		}
		else
		{
			atsSim->stopMission();
			atsSim->initialMission(mData);

			startFlag = 0;
			setFlag = 0;

			cout << "��� �̺�Ʈ :: ���߸��� ���� ����" << endl;

			break;
		}
	}
}

void sendSts()
{
	SendData stsData;

	cout << "���Ǳ� ���� :: �����������Ǳ� ���� ����" << endl;

	while (1)
	{
		stsData.sysState = "good";

		sndStsCom->sendInformation(stsData);

		this_thread::sleep_for(chrono::seconds(5));
	}
}

int main()
{
	sndPosCom = new ATS_COM(1001);
	sndStsCom = new ATS_COM(1002);
	rcvCom = new ATS_COM(1000);
	atsSim = new ATS_SIM();

	mData = new MissionData;

	startFlag = 0;
	setFlag = 0;
	stateFlag = 0;
	
	while (1)
	{
		if (stateFlag == 0)
		{
			thSend = new thread(sendSts);

			stateFlag = 1;
		}
		if (startFlag == 0)
		{
			if (setFlag == 0)
			{
				mData = rcvCom->receiveInformation();  // �ó����� ���� �� ����
				atsSim->setMission(mData);

				setFlag = 1;

				cout << "�ó����� ���� :: ���߸��� �ó����� ���� �Ϸ�" << endl;
			}

			if (setFlag == 1)
			{
				cout << "�ó����� ���� :: ���߸��� �ó����� ���� ����" << endl;

				mData = rcvCom->receiveInformation();  // �ó����� ���� - ����
				if (mData->sysCommand == "start")
				{
					thMission = new thread(mission);

					startFlag = 1;
				}
			}
		}
		else
		{
			mData = rcvCom->receiveInformation();  // �ó����� ���� - ���
			if (mData->sysCommand == "down")
			{
				cout << "��� �̺�Ʈ :: ���߸��� �ó����� ���� ����" << endl;

				startFlag = 0;

				atsSim->stopMission();
				atsSim->initialMission(mData);
				
				if (thMission)
				{
					thMission->join();

					delete thMission;
					thMission = nullptr;
				}
			}
		}
	}

	if (thMission->joinable()) thMission->join();
	if (thSend->joinable()) thSend->join();

	if(sndPosCom)
	{
		delete sndPosCom;
		sndPosCom = nullptr;
	}
	if (sndStsCom)
	{
		delete sndStsCom;
		sndStsCom = nullptr;
	}
	if (rcvCom)
	{
		delete rcvCom;
		rcvCom = nullptr;
	}
	if (atsSim)
	{
		delete atsSim;
		atsSim = nullptr;
	}
	if (thMission)
	{
		delete thMission;
		thMission = nullptr;
	}
	if (thSend)
	{
		delete thSend;
		thSend = nullptr;
	}
	if (mData)
	{
		delete mData;
		mData = nullptr;
	}

	return 0;
}