#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include<math.h>
#include "UDPSocket.h"

#define X_LIMIT 200.0
#define Y_LIMIT 200.0
#define DOWN_DISTANCE 5.0
#define ATS_IP_ADDRESS "127.0.0.1"
#define ATS_PORT 1000
#define SMS_IP_ADDRESS "127.0.0.1"
#define SMS_PORT 1000
#define OPC_ATS_PORT 1001
#define OPC_ATS_LIFE_PORT 1002
#define OPC_SMS_PORT 1101
#define OPC_SMS_LIFE_PORT 1102

using namespace std;

class ATS_ScenarioData
{
public:
	ATS_ScenarioData();

	void setStartXY(double x, double y);
	void setTargetXY(double x, double y);
	void setAttackType(bool att_type);
	void setSpeed(double speed);
	void move(double x, double y);
	double getStartX();
	double getStartY();
	double getTargetX();
	double getTargetY();
	double getNowX();
	double getNowY();
	bool getAttackType();
	double getSpeed();
	void userATS_Input(double sx, double sy, double tx, double ty, bool at, double speed);

private:
	double start_x;
	double start_y;
	double target_x;
	double target_y;
	bool attack_type;
	double attack_speed;
	double now_x;
	double now_y;
};

class SMS_ScenarioData
{
public:
	SMS_ScenarioData();

	void setXY(double x, double y);
	void move(double x, double y);
	double getX();
	double getY();
	double getNowX();
	double getNowY();
	double getDistance();
	double getSpeed();

private:
	double pos_x;
	double pos_y;
	double now_x;
	double now_y;
	double distance = 50.0;
	double speed = 5.0;
};

class SimulateSet
{
public:
	void initOperator();

	void setAts_Scenirio(ATS_ScenarioData scenario);
	void setSms_Scenirio(SMS_ScenarioData scenario);
	ATS_ScenarioData getAts_Scenirio();
	SMS_ScenarioData getSms_Scenirio();

	bool sendATSScenario();
	bool sendSMSScenario();

	void receiveATSPosition(int process_num);
	void receiveSMSPosition(int process_num);
	void threadPos();

	void setATSScenario(double sx, double sy, double tx, double ty, bool att_type, double speed);
	void lifeCheckATS(bool& checkpoint);
	void setSMSScenario(double mx, double my);
	void lifeCheckSMS(bool& checkpoint);
	void threadLifeCheck();

	void startSimulate();
	void launch();
	bool isEnermyDown();

	int getATSStatus();
	int getSMSStatus();

	double getATSNowX();
	double getATSNowY();
	double getSMSNowX();
	double getSMSNowY();

private:
	bool isReadyATS = false;
	bool isReadySMS = false;
	ATS_ScenarioData ats_scenirio;
	SMS_ScenarioData sms_scenirio;
	UDPSocket ats_socket;
	UDPSocket ats_life_socket;
	UDPSocket sms_socket;
	UDPSocket sms_life_socket;
	sockaddr_in ats_address;
	sockaddr_in ats_life_address;
	sockaddr_in sms_address;
	sockaddr_in sms_life_address;
	int atsStatus;	// 0: 대기, 1: 모의 배포, 2: 모의 실행 중, 3: 모의 종료
	int smsStatus;
	int process_number = 0;
};
