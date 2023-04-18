#include "SimulateSet.h"

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
	else
	{
		cout << "���߰����������� ���� X��ǥ�� �ùٸ��� �ʽ��ϴ�.\n�ٽ� �Է����ּ���.\n";
		return;
	}
	if ((y < Y_LIMIT) && (y >= 0))
	{
		start_y = y;
		now_y = y;
	}
	else
	{
		cout << "���߰����������� ���� Y��ǥ�� �ùٸ��� �ʽ��ϴ�.\n�ٽ� �Է����ּ���.\n";
		return;
	}
}

void ATS_ScenarioData::setTargetXY(double x, double y)
{
	if ((x < X_LIMIT) && (x >= 0.0)) { target_x = x; }
	else
	{
		cout << "���߰����������� ������ X��ǥ�� �ùٸ��� �ʽ��ϴ�.\n�ٽ� �Է����ּ���.\n";
		return;
	}
	if ((y < Y_LIMIT) && (y >= 0.0)) { target_y = y; }
	else
	{
		cout << "���߰����������� ������ Y��ǥ�� �ùٸ��� �ʽ��ϴ�.\n�ٽ� �Է����ּ���.\n";
		return;
	}
}

void ATS_ScenarioData::setAttackType(bool att_type)
{
	attack_type = att_type;
}

void ATS_ScenarioData::setSpeed(double speed)
{
	if (attack_type == 0)
	{
		if ((speed >= 0.1) && (speed <= 1.0))
		{
			attack_speed = speed;
			return;
		}
	}

	if (attack_type == 1)
	{
		if ((speed >= 1.0) && (speed <= 10.0))
		{
			attack_speed = speed;
			return;
		}
	}

	cout << "���� Ÿ�Կ� ���� �ʴ� �ӵ��Դϴ�.\n�ٽ��Է����ּ���";
}

void ATS_ScenarioData::move(double x, double y)
{
	if ((x < X_LIMIT) && (x >= 0.0))
	{
		now_x = x;
	}
	else
	{
		cout << "���������� ȭ�鿡�� ������ϴ�.\n";
		return;
	}
	if ((y < Y_LIMIT) && (y >= 0))
	{
		now_y = y;
	}
	else
	{
		cout << "���������� ȭ�鿡�� ������ϴ�.\n";
		return;
	}
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
	if ((x < X_LIMIT) && (x >= 0.0))
	{
		pos_x = x;
		now_x = x;
	}
	else
	{
		cout << "�������ź �ʱ� X��ǥ�� �ùٸ��� �ʽ��ϴ�.\n�ٽ� �Է����ּ���.\n";
		return;
	}
	if ((y < Y_LIMIT) && (y >= 0.0))
	{
		pos_y = y;
		now_y = y;
	}
	else
	{
		cout << "�������ź �ʱ� Y��ǥ�� �ùٸ��� �ʽ��ϴ�.\n�ٽ� �Է����ּ���.\n";
		return;
	}
}

void SMS_ScenarioData::move(double x, double y)
{
	if ((x < X_LIMIT) && (x >= 0.0))
	{
		now_x = x;
	}
	else
	{
		cout << "�������ź�� ȭ�鿡�� ������ϴ�.\n";
		return;
	}
	if ((y < Y_LIMIT) && (y >= 0))
	{
		now_y = y;
	}
	else
	{
		cout << "�������ź�� ȭ�鿡�� ������ϴ�.\n";
		return;
	}
}

double SMS_ScenarioData::getX()
{
	return pos_x;
}

double SMS_ScenarioData::getY()
{
	return pos_y;
}

double SMS_ScenarioData::getDistance()
{
	return distance;
}

double SMS_ScenarioData::getSpeed()
{
	return speed;
}

SimulateSet::SimulateSet()
{
}

SimulateSet::SimulateSet(UDPSocket ats_udp, UDPSocket sms_udp)
{
	ats_socket = ats_udp;
	sms_socket = sms_udp;
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

void SimulateSet::sendATSScenario()
{
	if (isReadyATS)
	{
		string sendMsg = to_string(ats_scenirio.getStartX()) + ","
			+ to_string(ats_scenirio.getStartY()) + ","
			+ to_string(ats_scenirio.getTargetX()) + ","
			+ to_string(ats_scenirio.getTargetY()) + ","
			+ to_string(ats_scenirio.getAttackType()) + ","
			+ to_string(ats_scenirio.getSpeed());

		ats_socket.SendTo(ats_address, sendMsg.c_str(), sendMsg.size());
	}
	else
	{
		cout << "������������ �ó������� �غ���� �ʾҽ��ϴ�.\n";
	}
}

void SimulateSet::sendSMSScenario()
{
	if (isReadySMS)
	{
		string sendMsg = to_string(sms_scenirio.getX()) + ","
			+ to_string(sms_scenirio.getY()) + ","
			+ to_string(sms_scenirio.getDistance()) + ","
			+ to_string(sms_scenirio.getSpeed());

		sms_socket.SendTo(sms_address, sendMsg.c_str(), sendMsg.size());
	}
	else
	{
		cout << "�������ź �ó������� �غ���� �ʾҽ��ϴ�.\n";
	}
}

void SimulateSet::receiveATSPosition(UDPSocket socket)
{
	char buffer[100];
	char separator = ',';

	socket.RecvFrom(buffer, 100);
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
	ats_scenirio.move(position[0], position[1]);
}

void SimulateSet::receiveSMSPosition(UDPSocket socket)
{
	char buffer[100];
	socket.RecvFrom(buffer, 100);
	string rcvmsg(buffer);
	char separator = ',';

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
}

void SimulateSet::runScenario()
{
	cout << "UDP Socket Server Ready\n";
	vector<thread>threads;
	int porcess_number = 0;
	threads.emplace_back(thread(&SimulateSet::processATS, this, porcess_number));
	porcess_number++;

	threads.emplace_back(thread(&SimulateSet::processSMS, this, porcess_number));
	porcess_number++;

	threads.emplace_back(thread(&SimulateSet::lifeCheckATS, this, porcess_number));
	porcess_number++;

	threads.emplace_back(thread(&SimulateSet::lifeCheckSMS, this, porcess_number));
	porcess_number++;

	for (auto& thread : threads)
		thread.join();
}

void SimulateSet::processATS(int process_no)
{
	char buffer[100];

	ats_socket.Bind(1111);
	ats_address = ats_socket.RecvFrom(buffer, sizeof(buffer));
	if (string(buffer) == "ATS")
	{
		cout << "�����������Ǳ� �����" << endl;
	}

	double sx = (double)(rand() % 2000) / 10;
	double sy = (double)(rand() % 2000) / 10;
	double tx = (double)(rand() % 2000) / 10;
	double ty = (double)(rand() % 2000) / 10;
	bool att_type = 1;
	double speed = (double)(rand() % 100) / 10;

	ats_scenirio.userATS_Input(sx, sy, tx, ty, att_type, speed);
	isReadyATS = true;
	lock_guard<mutex>lock_guard(g_lock);
	printf("%f %f %f %f %d %f\n", sx, sy, tx, ty, att_type, speed);

	cout << "�������� �ó����� ����\n";
	sendATSScenario();



	cout << "�������� �ó����� ���� Test �Ϸ�" << endl;
}

void SimulateSet::lifeCheckATS(int process_no)
{
	char buffer[100];

	ats_life_socket.Bind(1112);
	while (true)
	{
		if (ats_life_socket.RecvFromForLife(buffer, sizeof(buffer)))
		{
			cout << "�����������Ǳ� �����" << endl;
		}
		else
		{
			cout << "�����������Ǳ� ������" << endl;
		}
	}
}

void SimulateSet::processSMS(int process_no)
{
	char buffer[100];

	sms_socket.Bind(2222);
	sms_address = sms_socket.RecvFrom(buffer, sizeof(buffer));
	if (string(buffer) == "SMS")
	{
		cout << "�������ź���Ǳ� �����" << endl;
	}

	double mx = (double)(rand() % 2000) / 10;
	double my = (double)(rand() % 2000) / 10;

	sms_scenirio.setXY(mx, my);
	isReadySMS = true;
	lock_guard<mutex>lock_guard(g_lock);
	printf("%f %f\n", mx, my);

	cout << "�������ź �ó����� ����\n";
	sendSMSScenario();

	launch();

	cout << "�������ź �ó����� ���� Test �Ϸ�" << endl;
}

void SimulateSet::lifeCheckSMS(int process_no)
{
	char buffer[100];

	sms_life_socket.Bind(1112);
	while (true)
	{
		if (sms_life_socket.RecvFromForLife(buffer, sizeof(buffer)))
		{
			cout << "�����������Ǳ� �����" << endl;
		}
		else
		{
			cout << "�����������Ǳ� ������" << endl;
		}
	}
}

void SimulateSet::launch()
{
	cout << "�߻縦 ���ϸ� �ƹ�Ű�� �Է����ּ���: ";
	string input;
	cin >> input;

	string sendMsg = "�������ź�߻� : " + to_string(ats_scenirio.getNowX()) + ","
		+ to_string(ats_scenirio.getNowY()) + ","
		+ to_string(ats_scenirio.getTargetX()) + ","
		+ to_string(ats_scenirio.getTargetY()) + ","
		+ to_string(ats_scenirio.getSpeed());

	sms_socket.SendTo(sms_address, sendMsg.c_str(), sendMsg.size());
}
