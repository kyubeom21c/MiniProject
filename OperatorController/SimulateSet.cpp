#include "SimulateSet.h"
#include "pch.h"

ATS_ScenarioData::ATS_ScenarioData()
{
	setStartXY(0.0, 0.0);
	setTargetXY(0.0, 0.0);
	setAttackType(0);
	setSpeed(0.1);
}

void ATS_ScenarioData::setStartXY(double x, double y)
{
	if ((x < X_LIMIT) && (x >= 0.0))
	{
		start_x = x;
		now_x = x;
	}
	if ((y < Y_LIMIT) && (y >= 0))
	{
		start_y = y;
		now_y = y;
	}
}

void ATS_ScenarioData::setTargetXY(double x, double y)
{
	if ((x < X_LIMIT) && (x >= 0.0)) target_x = x;
	if ((y < Y_LIMIT) && (y >= 0.0)) target_y = y;
}

void ATS_ScenarioData::setAttackType(bool att_type)
{
	attack_type = att_type;
}

void ATS_ScenarioData::setSpeed(double speed)
{
	if (attack_type == 0)
	{
		if ((speed >= 0.1) && (speed <= 1.0)) attack_speed = speed;
	}

	if (attack_type == 1)
	{
		if ((speed >= 1.0) && (speed <= 10.0)) attack_speed = speed;
	}
}

void ATS_ScenarioData::move(double x, double y)
{
	now_x = x;
	now_y = y;
}

double ATS_ScenarioData::getStartX()
{
	return start_x;
}

double ATS_ScenarioData::getStartY()
{
	return start_y;
}

double ATS_ScenarioData::getTargetX()
{
	return target_x;
}

double ATS_ScenarioData::getTargetY()
{
	return target_y;
}

double ATS_ScenarioData::getNowX()
{
	return now_x;
}

double ATS_ScenarioData::getNowY()
{
	return now_y;
}

bool ATS_ScenarioData::getAttackType()
{
	return attack_type;
}

double ATS_ScenarioData::getSpeed()
{
	return attack_speed;
}

void ATS_ScenarioData::userATS_Input(double sx, double sy, double tx, double ty, bool at, double speed)
{
	setStartXY(sx, sy);
	setTargetXY(tx, ty);
	setAttackType(at);
	setSpeed(speed);
}

SMS_ScenarioData::SMS_ScenarioData()
{
	setXY(0.0, 0.0);
}

void SMS_ScenarioData::setXY(double x, double y)
{
	pos_x = x;
	pos_y = y;
	now_x = x;
	now_y = y;
}

void SMS_ScenarioData::move(double x, double y)
{
	now_x = x;
	now_y = y;
}

double SMS_ScenarioData::getX()
{
	return pos_x;
}

double SMS_ScenarioData::getY()
{
	return pos_y;
}

double SMS_ScenarioData::getNowX()
{
	return now_x;
}

double SMS_ScenarioData::getNowY()
{
	return now_y;
}

double SMS_ScenarioData::getDistance()
{
	return distance;
}

double SMS_ScenarioData::getSpeed()
{
	return speed;
}

/* 운용통제기 시스템 객체 구현부분 */
void SimulateSet::initOperator()
{
	ats_socket.Bind(OPC_ATS_PORT);
	sms_socket.Bind(OPC_SMS_PORT);
}

void SimulateSet::setAts_Scenirio(ATS_ScenarioData scenario)
{
	ats_scenirio = scenario;
	isReadyATS = true;
}

void SimulateSet::setSms_Scenirio(SMS_ScenarioData scenario)
{
	sms_scenirio = scenario;
	isReadySMS = true;
}

ATS_ScenarioData SimulateSet::getAts_Scenirio()
{
	return ats_scenirio;
}

SMS_ScenarioData SimulateSet::getSms_Scenirio()
{
	return sms_scenirio;
}

/* 모의기 시나리오 배포 기능 */
bool SimulateSet::sendATSScenario()
{
	if (isReadyATS)
	{
		string sendMsg = to_string(ats_scenirio.getStartX()) + ","
			+ to_string(ats_scenirio.getStartY()) + ","
			+ to_string(ats_scenirio.getTargetX()) + ","
			+ to_string(ats_scenirio.getTargetY()) + ","
			+ to_string(ats_scenirio.getAttackType()) + ","
			+ to_string(ats_scenirio.getSpeed());

		ats_socket.SendTo(ATS_IP_ADDRESS, ATS_PORT, sendMsg.c_str(), sendMsg.size());
		atsStatus = 1;
		return true;
	}
	else return false;
}

bool SimulateSet::sendSMSScenario()
{
	if (isReadySMS)
	{
		string sendMsg = to_string(sms_scenirio.getX()) + ","
			+ to_string(sms_scenirio.getY()) + ","
			+ to_string(sms_scenirio.getDistance()) + ","
			+ to_string(sms_scenirio.getSpeed());

		sms_socket.SendTo(SMS_IP_ADDRESS, SMS_PORT, sendMsg.c_str(), sendMsg.size());
		smsStatus = 1;
		return true;
	}
	else return false;
}

/* 모의기 현재 좌표값 수신 */

void SimulateSet::receiveATSPosition(int process_num)
{
	char buffer[100];
	char separator = ',';

	while (true)
	{
		ats_socket.RecvFrom(buffer, 100);
		string rcvmsg(buffer);

		istringstream iss(rcvmsg);
		string str_buf;
		double position[2] = { 0.0, 0.0 };

		int i = 0;
		while (getline(iss, str_buf, separator))
		{
			position[i] = stod(str_buf);
			i++;
		}
		ats_scenirio.move(position[0], position[1]);
		if (isEnermyDown()) break;
	}
}

void SimulateSet::receiveSMSPosition(int process_num)
{
	char buffer[100];
	char separator = ',';

	while (true)
	{
		sms_socket.RecvFrom(buffer, 100);
		string rcvmsg(buffer);

		istringstream iss(rcvmsg);
		string str_buf;
		double position[2] = { 0.0,0.0 };

		int i = 0;
		while (getline(iss, str_buf, separator))
		{
			position[i] = stod(str_buf);
			i++;
		}
		sms_scenirio.move(position[0], position[1]);
		if (isEnermyDown()) break;
	}
}

void SimulateSet::threadPos()
{
	vector<thread>threads;
	threads.emplace_back(thread(&SimulateSet::receiveATSPosition, this, process_number));
	process_number++;
	threads.emplace_back(thread(&SimulateSet::receiveSMSPosition, this, process_number));
	process_number++;

	for (auto& thread : threads)
		thread.join();
}

/* 각 모의기 시나리오 설정 함수 */
void SimulateSet::setATSScenario(double sx, double sy, double tx, double ty, bool att_type, double speed)
{
	ats_scenirio.userATS_Input(sx, sy, tx, ty, att_type, speed);
	isReadyATS = true;
}

void SimulateSet::setSMSScenario(double mx, double my)
{
	sms_scenirio.setXY(mx, my);
	isReadySMS = true;
	smsStatus = 1;
}

/* 각 모의기 생존 여부 체크 함수 */
void SimulateSet::lifeCheckATS(bool& checkpoint)
{
	char buffer[100];
	ats_life_socket.Bind(OPC_ATS_LIFE_PORT);

	while (true)
	{
		if (ats_life_socket.RecvFromForLife(buffer, sizeof(buffer))) checkpoint = true;
		else checkpoint = false;
	}
}

void SimulateSet::lifeCheckSMS(bool& checkpoint)
{
	char buffer[100];

	sms_life_socket.Bind(OPC_SMS_LIFE_PORT);
	while (true)
	{
		if (sms_life_socket.RecvFromForLife(buffer, sizeof(buffer))) checkpoint = true;
		else checkpoint = false;
	}
}

void SimulateSet::threadLifeCheck()
{
	vector<thread>threads;
	threads.emplace_back(thread(&SimulateSet::receiveATSPosition, this, process_number));
	process_number++;
	threads.emplace_back(thread(&SimulateSet::receiveSMSPosition, this, process_number));
	process_number++;

	for (auto& thread : threads)
		thread.join();
}

/* 공중위협 기동 */
void SimulateSet::startSimulate()
{
	if (isReadyATS && isReadySMS)
	{
		string sendMsg = "start";
		ats_socket.SendTo(ATS_IP_ADDRESS, ATS_PORT, sendMsg.c_str(), sendMsg.size());
		atsStatus = 2;
	}
}

/* 대공유도탄 발사 */
void SimulateSet::launch()
{
	string sendMsg = to_string(ats_scenirio.getNowX()) + ","
		+ to_string(ats_scenirio.getNowY()) + ","
		+ to_string(ats_scenirio.getTargetX()) + ","
		+ to_string(ats_scenirio.getTargetY()) + ","
		+ to_string(ats_scenirio.getSpeed());

	sms_socket.SendTo(SMS_IP_ADDRESS, SMS_PORT, sendMsg.c_str(), sendMsg.size());
	smsStatus = 2;
}

/* 격추 이벤트 */
bool SimulateSet::isEnermyDown()
{
	double ats_sms_dist = sqrt(pow(ats_scenirio.getNowX() - sms_scenirio.getNowX(), 2) + pow(ats_scenirio.getNowY() - sms_scenirio.getNowY(), 2));
	if (ats_sms_dist < DOWN_DISTANCE)
	{
		string sendMsg = to_string(ats_sms_dist);
		ats_socket.SendTo(ATS_IP_ADDRESS, ATS_PORT, sendMsg.c_str(), sendMsg.size());
		sms_socket.SendTo(SMS_IP_ADDRESS, SMS_PORT, sendMsg.c_str(), sendMsg.size());
		atsStatus = 3;
		smsStatus = 3;
		return true;
	}
	return false;
}

int SimulateSet::getATSStatus()
{
	return atsStatus;
}

int SimulateSet::getSMSStatus()
{
	return smsStatus;
}

double SimulateSet::getATSNowX()
{
	return ats_scenirio.getNowX();
}

double SimulateSet::getATSNowY()
{
	return ats_scenirio.getNowY();
}

double SimulateSet::getSMSNowX()
{
	return sms_scenirio.getNowX();
}

double SimulateSet::getSMSNowY()
{
	return sms_scenirio.getNowY();
}
