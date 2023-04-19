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
    double ATS_ang; // 공중위협의 비행 각도  ************************
    double ATSvX;  //공중위협 x이동속도
    double ATSvY;  //공중위협 y이동속도
    double ATSPos_x; //공중위협의 위치 중 x 좌표 , 대공유도탄이 발사명령을 받아 발사하기 직전의 좌표
    double ATSPos_y; //공중위협의 위치 중 y 좌표 , 대공유도탄이 발사명령을 받아 발사하기 직전의 좌표 
    double ATS_Target_x;
    double ATS_Target_y;
    double ATS_Speed;
    
    double AMSangle; // 대공유도탄의 발사 각도 // <-를 딱히 구하지 않고 vx, vy 구한 후 그대로 사용
    double AMSvX;//대공유도탄 X축 이동 값
    double AMSvY;//대공유도탄 Y축 이동 값
    double AMSspeed = 340.0 * 5.0; // 대공유도탄 속력 (마하5 의 초당 속력 m)
    double curr_pos_x; //대공유도탄의 현재 위치 중 x좌표
    double curr_pos_y; //대공유도탄의 현재 위치 중 y좌표
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

    void Set_AMS_System(); // AMS 생성자. 매개변수 : 유도탄 발사 초기위치 (x, y)
    double get_Current_X();
    double get_Current_Y();
    void set_X(double x);
    void set_Y(double y);
    void NextPos(); // 이동 및 좌표 갱신 함수, 현재 좌표를 받아서 timestep 만큼 후의 좌표를 반환한다.
    void FireAngle(); // arg : 공중위협 현재위치(x), 공중위협 현재위치(y), 공중위협 목표위치(x), 공중위협 목표위치(y),  공중위협 속력
    void send_pos();
    void AMS_INFO(double _ATSPos_x, double _ATSPos_y, double _ATSPos_Target_x, double _ATSPos_Target_y, double collisionTime);
};

