#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include<mutex>
#include "UDPSocket.h"

#define X_LIMIT 200.0
#define Y_LIMIT 200.0

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
	SimulateSet();
	SimulateSet(UDPSocket ats_socket, UDPSocket sms_socket);
	void setAts_Scenirio(ATS_ScenarioData scenario);
	void setSms_Scenirio(SMS_ScenarioData scenario);
	ATS_ScenarioData getAts_Scenirio();
	SMS_ScenarioData getSms_Scenirio();

	void sendATSScenario();
	void sendSMSScenario();

	void receiveATSPosition(UDPSocket socket);
	void receiveSMSPosition(UDPSocket socket);

	void runScenario();
	void processATS(int process_no);
	void lifeCheckATS(int process_no);
	void processSMS(int process_no);
	void lifeCheckSMS(int process_no);

	void launch();

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
	mutex g_lock;
	bool isATSTimerStart = false;
	bool isSMSTimerStart = false;
};
