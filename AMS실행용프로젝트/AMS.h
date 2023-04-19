#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include "UDPSocket.h"

#define SOCKET_PORT 1000
#define OPC_IP_ADDRESS "127.0.0.1"
#define OPC_DATA_PORT 1101
#define OPC_LIFE_PORT 1102

using namespace std;

class AMSDynamics
{
private:
    //_ATSPos_x, _ATSPos_y, _ATSPos_Target_x, _ATSPos_Target_y, collisionTime
    double ATS_dX;
    double ATS_dY;
    double ATS_ang; // ���������� ���� ����  ************************
    double ATSvX;  //�������� x�̵��ӵ�
    double ATSvY;  //�������� y�̵��ӵ�
    double ATSPos_x; //���������� ��ġ �� x ��ǥ , �������ź�� �߻����� �޾� �߻��ϱ� ������ ��ǥ
    double ATSPos_y; //���������� ��ġ �� y ��ǥ , �������ź�� �߻����� �޾� �߻��ϱ� ������ ��ǥ 
    double ATS_Target_x;
    double ATS_Target_y;
    double ATS_Speed;
    
    double AMSangle; // �������ź�� �߻� ���� // <-�� ���� ������ �ʰ� vx, vy ���� �� �״�� ���
    double AMSvX;//�������ź X�� �̵� ��
    double AMSvY;//�������ź Y�� �̵� ��
    double AMSspeed = 340.0 * 5.0; // �������ź �ӷ� (����5 �� �ʴ� �ӷ� m)
    double curr_pos_x; //�������ź�� ���� ��ġ �� x��ǥ
    double curr_pos_y; //�������ź�� ���� ��ġ �� y��ǥ
    bool AMS_State = "False";

    double samplingRate;
    double timestep = 0.1;

    string AMS_scenario="";
    string ATS_POS;
    double collisionTime;
    UDPSocket udpSocket;

public:
   
    AMSDynamics();

    void initAMS();
   
    void receiveScenario();
    void sendXY();
    void sendLife();

    void Set_AMS_System(); // AMS ������. �Ű����� : ����ź �߻� �ʱ���ġ (x, y)
    double get_Current_X();
    double get_Current_Y();
    void set_X(double x);
    void set_Y(double y);
    void NextPos(); // �̵� �� ��ǥ ���� �Լ�, ���� ��ǥ�� �޾Ƽ� timestep ��ŭ ���� ��ǥ�� ��ȯ�Ѵ�.
    void FireAngle(); // arg : �������� ������ġ(x), �������� ������ġ(y), �������� ��ǥ��ġ(x), �������� ��ǥ��ġ(y),  �������� �ӷ�
    void send_pos();
    void AMS_INFO(double _ATSPos_x, double _ATSPos_y, double _ATSPos_Target_x, double _ATSPos_Target_y, double collisionTime);
};

