// AMS_Lib.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//


#include <cmath>
#include <iostream>

using namespace std;

// TODO: 라이브러리 함수의 예제입니다.

class AMSDynamics
{
public:

	void NextPos(double _pos_x, double _pos_y, double _angle) // 현재 위치를 받아 다음 위치를 리턴하는 함수
	{
		double pos_x = _pos_x;
		double pos_y = _pos_y;
		double angle = _angle;

		double speed = 340.0 * 5.0; // 마하 5 를 초당 속도(미터)로 변환
		double timestep = 0.1;

		double next_pos_x;
		double next_pos_y;

		next_pos_x = pos_x + (speed * cos(angle) * timestep);
		next_pos_y = pos_y + (speed * sin(angle) * timestep);
		cout << "curr_x : " << pos_x << "curr_y : " << pos_y << endl;
		cout << "next_x : " << next_pos_x << "next_y : " << next_pos_y << endl;
	}

	void FireAngle(double _ATSPox_y, double _ATSPos_y, double ATSspeed)
	{

	}

};
void fnAMSLib()
{

}

int main()
{
	
	AMSDynamics ams = AMSDynamics();
	ams.NextPos(10.1, 10.1, 3.14 / 6);

};