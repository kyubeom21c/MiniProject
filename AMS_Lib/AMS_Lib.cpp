// AMS_Lib.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//


#include <cmath>
#include <iostream>

using namespace std;

// TODO: 라이브러리 함수의 예제입니다.

class AMSDynamics
{
public:
double samplingRate;

    // 이동 및 좌표 갱신 함수
    void NextPos(double _pos_x, double _pos_y, double _angle)
    {
        curr_pos_x = _pos_x;
        curr_pos_y = _pos_y;
        AMSangle = _angle;

        
        double timestep = 0.1;
                
        next_pos_x = curr_pos_x + (AMSspeed * cos(AMSangle) * timestep);
        next_pos_y = curr_pos_y + (AMSspeed * sin(AMSangle) * timestep);
        cout << "curr_x : " << curr_pos_x << "curr_y : " << curr_pos_y << endl;
        cout << "next_x : " << next_pos_x << "next_y : " << next_pos_y << endl;
    }

    void FireAngle(double _ATSPos_Initial_x, double _ATSPos_Initial_y, double _ATSPos_Target_x, double _ATSPos_Target_y, double ATSspeed)
    {
        // 시나리오로부터 공중위협의 초기위치, 목적위치로부터 공중위협의 비행 각도를 구한다.
        // ATSang = (y_target - y_init) / (x_target - x_init)

        // 
        double ATS_dX =  (_ATSPos_Target_x - _ATSPos_Initial_x);
        double ATS_dY= (_ATSPos_Target_y - _ATSPos_Initial_y);

        double ATS_Rad = atan2(ATS_dY, ATS_dX);
        cout << ATS_Rad;
       
    }
private:
    double curr_pos_x; //대공유도탄의 현재 위치 중 x좌표
    double curr_pos_y; //대공유도탄의 현재 위치 중 y좌표

    double next_pos_x; //대공유도탄의 다음 위치 중 x좌표
    double next_pos_y; //대공유도탄의 다음 위치 중 y좌표
   


    double AMSangle; // 대공유도탄의 발사 각도
    double AMSspeed = 340 * 5; // 대공유도탄 속력 (마하5 의 초당 속력 m)

    double ATSPos_x; //공중위협의 위치 중 x 좌표
    double ATSPos_y; //공중위협의 위치 중 y 좌표

};
void fnAMSLib()
{


}

int main()
{

    AMSDynamics ams = AMSDynamics();
    ams.NextPos(10.1, 10.1, 3.14 / 6);

};