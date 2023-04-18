// AMS_Lib.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//


#include <cmath>
#include <iostream>

using namespace std;

// AMSDynamics 생성자 : AMSDynamics(유도탄 초기위치x, 유도탄 초기위치y)

class AMSDynamics
{
public:
    double samplingRate;
    double timestep = 0.1;
    double AMSPos_initial_x;
    double AMSPos_initial_y;

    AMSDynamics(double x, double y) // AMS 생성자. 매개변수 : 유도탄 발사 초기위치 (x, y)
    {
        AMSPos_initial_x = x;
        AMSPos_initial_y = y;
    }

    // 이동 및 좌표 갱신 함수, 현재 좌표를 받아서 timestep 만큼 후의 좌표를 반환한다.
    void NextPos(double _pos_x, double _pos_y, double _angle)
    {
        curr_pos_x = _pos_x;
        curr_pos_y = _pos_y;
        AMSangle = _angle;

        next_pos_x = curr_pos_x + (AMSspeed * cos(AMSangle) * timestep);
        next_pos_y = curr_pos_y + (AMSspeed * sin(AMSangle) * timestep);
        cout << "curr_x : " << curr_pos_x << endl;
        cout << "curr_y : " << curr_pos_y << endl;
        cout << "next_x : " << next_pos_x << endl;
        cout << "next_y : " << next_pos_y << endl;
    }

    void FireAngle(double _ATSPos_Initial_x, double _ATSPos_Initial_y, double _ATSPos_Target_x, double _ATSPos_Target_y, double _ATSPos_x, double _ATSPos_y, double ATSspeed)
        // arg : 공중위협 초기위치(x), 공중위협 초기위치(y), 공중위협 목표위치(x), 공중위협 목표위치(y), 공중위협 현재위치(x), 공중위협 현재위치(y), 공중위협 속력
    {
        // 시나리오로부터 공중위협의 초기위치와 목적위치로부터 공중위협의 비행 각도를 구한다.
        // ATSang = (y_target - y_init) / (x_target - x_init)

        // 
        ATS_dX = (_ATSPos_Target_x - _ATSPos_Initial_x);
        ATS_dY = (_ATSPos_Target_y - _ATSPos_Initial_y);

        ATS_ang = atan2(ATS_dY, ATS_dX); // 공중위협의 비행 각도
        
        ATSvX = cos(ATS_ang) * ATSspeed;  //공중위협 x이동속도
        ATSvY = sin(ATS_ang) * ATSspeed;  //공중위협 y이동속도

        double a = (_ATSPos_y - AMSPos_initial_y) / (_ATSPos_y - AMSPos_initial_x);
        double b = a * ATSvX + ATSvY;

        // 이차방정식 ax**2 + bx + c = 0의 계수
        double quad_a = pow(a, 2) + 1;
        double quad_b = -2 * a * b;
        double quad_c = pow(b, 2) - pow(AMSspeed, 2);

        double sol1 = (-quad_b + sqrt(pow(quad_b, 2) - 4 * quad_a * quad_c)) / (2 * quad_a);
        double sol2 = (-quad_b - sqrt(pow(quad_b, 2) - 4 * quad_a * quad_c)) / (2 * quad_a);

        double collisionTime; // 충돌까지 소요시간

        if ((_ATSPos_x - AMSPos_initial_x) / (sol1 - ATSvX) > 0)
        {
            AMSvX = sol1;
            collisionTime = (_ATSPos_x - AMSPos_initial_x) / (sol1 - ATSvX);
        }
        else
        {
            AMSvX = sol2;
            collisionTime = (_ATSPos_x - AMSPos_initial_x) / (sol2 - ATSvX);
        }

        AMSvY = a * (AMSvX - ATSvX) + ATSvY;

        cout << "대공위협 초기 위치 (x,y) : " << _ATSPos_x << " , " << _ATSPos_y << endl;
        cout << "대공위협 목표 위치 (x,y) : " << _ATSPos_Target_x << " , " << _ATSPos_Target_y << endl;
        cout << "유도탄 초기 위치 (x,y) : " << AMSPos_initial_x << " , " << AMSPos_initial_y << endl;
        cout << "유도탄 요격 속력 (Vx, Vy) : " << AMSvX << " , " << AMSvY << endl;
        cout << "Collision Time : " << collisionTime << " 초 후 요격" << endl;
        cout << "Collision Time 후 대공위협 위치 (x, y) : " << _ATSPos_x + ATSvX * collisionTime << " , " << _ATSPos_y + ATSvY * collisionTime << endl;
        cout << "Collision Time 후 대공유도탄 위치 (x, y) : " << AMSPos_initial_x + AMSvX * collisionTime << " , " << AMSPos_initial_y + AMSvY * collisionTime << endl;

    }
private:
    double curr_pos_x; //대공유도탄의 현재 위치 중 x좌표
    double curr_pos_y; //대공유도탄의 현재 위치 중 y좌표

    double next_pos_x; //대공유도탄의 다음 위치 중 x좌표
    double next_pos_y; //대공유도탄의 다음 위치 중 y좌표

    double ATS_dX;
    double ATS_dY;

    double ATS_ang; // 공중위협의 비행 각도

    double ATSvX;  //공중위협 x이동속도
    double ATSvY;  //공중위협 y이동속도

    double AMSangle; // 대공유도탄의 발사 각도 // <-를 딱히 구하지 않고 vx, vy 구한 후 그대로 사용
    double AMSvX;//대공유도탄 X축 이동 값
    double AMSvY;//대공유도탄 Y축 이동 값
    double AMSspeed = 340 * 5; // 대공유도탄 속력 (마하5 의 초당 속력 m)

    //double ATSPos_x; //공중위협의 위치 중 x 좌표 , 대공유도탄이 발사명령을 받아 발사하기 직전의 좌표
    //double ATSPos_y; //공중위협의 위치 중 y 좌표 , 대공유도탄이 발사명령을 받아 발사하기 직전의 좌표
   

};
void fnAMSLib()
{


}

int main()
{

    AMSDynamics ams = AMSDynamics(5000, 0); // 생성자 : 유도탄 발사 초기위치 
    ams.NextPos(10.1, 10.1, 3.14 / 6);
    ams.FireAngle(-2000, -2000, 4000, 4000, -2000, -2000, 340);
    // arg : 공중위협 초기위치(x), 공중위협 초기위치(y), 공중위협 목표위치(x), 공중위협 목표위치(y), 공중위협 현재위치(x), 공중위협 현재위치(y), 공중위협 속력

};