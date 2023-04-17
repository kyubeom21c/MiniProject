// AMS_Lib.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include <cmath>
#include <iostream>

using namespace std;

// TODO: 라이브러리 함수의 예제입니다.

class AMSDynamics
{
public:

<<<<<<< HEAD
    void NextPos(double _pos_x, double _pos_y, double _angle)
    {
        double pos_x = _pos_x;
        double pos_y = _pos_y;
        double angle = _angle;

        double speed = 340.0 * 5.0; // 마하 5
        double timestep = 0.1;

        double next_pos_x;
        double next_pos_y;

        next_pos_x = pos_x + (speed * cos(angle));
        next_pos_y = pos_y + (speed * sin(angle));
        cout << "curr_x : " << pos_x << "curr_y : " << pos_y << endl;
        cout << "next_x : " << next_pos_x << "next_y : " << next_pos_y << endl;
    }
=======
	void NextPos(double _pos_x, double _pos_y, double _angle)
	{
		double pos_x = _pos_x;
		double pos_y = _pos_y;
		double angle = _angle;

		double speed = 340.0 * 5.0; // 마하 5
		double timestep = 0.1;

		double next_pos_x;
		double next_pos_y;

		next_pos_x = pos_x + (speed * cos(angle));
		next_pos_y = pos_y + (speed * sin(angle));
		cout << "curr_x : " << pos_x << "curr_y : " << pos_y << endl;
		cout << "next_x : " << next_pos_x << "next_y : " << next_pos_y << endl;
	}
>>>>>>> 30c9875e687ec4d14796dd26af2b97f820cf0485

};
void fnAMSLib()
{

}

int main()
{
<<<<<<< HEAD
    cout << "hello world" << endl;
    AMSDynamics ams = AMSDynamics();
    ams.NextPos(10.1, 10.1, 3.14 / 6);

=======
	cout << "hello world" << endl;
//	AMSDynamics ams = AMSDynamics();
	//ams.NextPos(10.1, 10.1, 3.14 / 6);
	
>>>>>>> 30c9875e687ec4d14796dd26af2b97f820cf0485
};