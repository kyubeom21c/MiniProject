// AMS_Lib.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//


#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
using namespace std;
int cnt = 0;
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

    double collisionTime;

public:
    AMSDynamics(string message); // AMS 생성자. 매개변수 : 유도탄 발사 초기위치 (x, y)
    double get_Current_X();
    double get_Current_Y();
    void set_X(double x);
    void set_Y(double y);
    void NextPos(); // 이동 및 좌표 갱신 함수, 현재 좌표를 받아서 timestep 만큼 후의 좌표를 반환한다.
    void FireAngle(string message); // arg : 공중위협 현재위치(x), 공중위협 현재위치(y), 공중위협 목표위치(x), 공중위협 목표위치(y),  공중위협 속력
    void send_pos();
    void AMS_INFO(double _ATSPos_x, double _ATSPos_y, double _ATSPos_Target_x, double _ATSPos_Target_y, double collisionTime);
};

AMSDynamics::AMSDynamics(string message)
{
    message.insert(message.length(), "    "); // 문자열 마지막에 공백 추가
    string delimiter = "    ";
    vector<string> tok{}; // message을 tokenizing한 token을 저장할 vector	
    size_t pos = 0;
    // pos가 string::npos를 가리킬 때, 즉, 문자열의 끝에 도달할 때 까지 탐색
    while ((pos = message.find(delimiter)) != string::npos) {
        // 0 ~ 공백이 나올 때 까지 문자열을 잘라내어 tok vector에 push
        tok.push_back(message.substr(0, pos));
        // 잘라낸 문자열은 erase 함수를 이용해 삭제
        message.erase(0, pos + delimiter.length());
    }

    //for (const auto& s : tok)
      //  cout << s << endl;
    curr_pos_x = stod(tok[0]);
    curr_pos_y = stod(tok[1]);
    AMS_State = "True";
    cout << "대공 유도탄 초기 x : " << curr_pos_x << "   대공유도탄 초기 y : " << curr_pos_y << endl;
}

double AMSDynamics::get_Current_X()
{
    return curr_pos_x;
}
double AMSDynamics::get_Current_Y()
{
    return curr_pos_y;
}
void AMSDynamics::set_X(double x)
{
    curr_pos_x = x;
}
void AMSDynamics::set_Y(double y)
{
    curr_pos_y = y;
}
void AMSDynamics::NextPos()
{
    curr_pos_x = curr_pos_x + (AMSvX * timestep);
    curr_pos_y = curr_pos_y + (AMSvY * timestep);
    cout << "curr_pos_x : " << curr_pos_x << "    curr_pos_y : " << curr_pos_y << endl;
}


void AMSDynamics::FireAngle(string message)
// arg : 공중위협 현재위치(x), 공중위협 현재위치(y), 공중위협 목표위치(x), 공중위협 목표위치(y),  공중위협 속력
{
    // 시나리오로부터 공중위협의 초기위치와 목적위치로부터 공중위협의 비행 각도를 구한다.
    message.insert(message.length(), "    "); // 문자열 마지막에 공백 추가
    string delimiter = "    ";
    vector<string> tok{}; // message을 tokenizing한 token을 저장할 vector   
    size_t pos = 0;
    // pos가 string::npos를 가리킬 때, 즉, 문자열의 끝에 도달할 때 까지 탐색
    while ((pos = message.find(delimiter)) != string::npos) {
        // 0 ~ 공백이 나올 때 까지 문자열을 잘라내어 tok vector에 push
        tok.push_back(message.substr(0, pos));
        // 잘라낸 문자열은 erase 함수를 이용해 삭제
        message.erase(0, pos + delimiter.length());
    }
    ATSPos_x = stod(tok[0]);
    ATSPos_y = stod(tok[1]);
    ATS_Target_x = stod(tok[2]);
    ATS_Target_y = stod(tok[3]);
    ATS_Speed = stod(tok[4]);
    ATS_dX = (ATS_Target_x - ATSPos_x);
    ATS_dY = (ATS_Target_x - ATSPos_y);

    ATS_ang = atan2(ATS_dY, ATS_dX); // 공중위협의 비행 각도

    ATSvX = cos(ATS_ang) * ATS_Speed;  //공중위협 x이동속도
    ATSvY = sin(ATS_ang) * ATS_Speed;  //공중위협 y이동속도

    double a = (ATSPos_y - curr_pos_y) / (ATSPos_y - curr_pos_x);
    double b = a * ATSvX + ATSvY;

    // 이차방정식 ax**2 + bx + c = 0의 계수
    double quad_a = pow(a, 2) + 1;
    double quad_b = -2 * a * b;
    double quad_c = pow(b, 2) - pow(AMSspeed, 2);

    double sol1 = (-quad_b + sqrt(pow(quad_b, 2) - 4 * quad_a * quad_c)) / (2 * quad_a);
    double sol2 = (-quad_b - sqrt(pow(quad_b, 2) - 4 * quad_a * quad_c)) / (2 * quad_a);

    double collisionTime; // 충돌까지 소요시간

    if ((ATSPos_x - curr_pos_x) / (sol1 - ATSvX) > 0)
    {
        AMSvX = sol1;
        collisionTime = (ATSPos_x - curr_pos_x) / (sol1 - ATSvX);
    }
    else
    {
        AMSvX = sol2;
        collisionTime = (ATSPos_x - curr_pos_x) / (sol2 - ATSvX);
    }

    AMSvY = a * (AMSvX - ATSvX) + ATSvY;

    if (cnt % 10 == 0)
        AMS_INFO(ATSPos_x, ATSPos_y, ATS_Target_x, ATS_Target_x, collisionTime);
    cnt++;


}
void AMSDynamics::AMS_INFO(double _ATSPos_x, double _ATSPos_y, double _ATSPos_Target_x, double _ATSPos_Target_y, double collisionTime)
{

    cout << "대공위협 위치 (x,y) : " << _ATSPos_x << " , " << _ATSPos_y << endl;
    cout << "유도탄 위치 (x,y) : " << curr_pos_x << " , " << curr_pos_y << endl;
    cout << "Collision Time : " << collisionTime << " 초 후 요격" << endl;
    cout << "Distance AMS - ATS : " << sqrt(pow((_ATSPos_x - curr_pos_x), 2) + pow((_ATSPos_y - curr_pos_y), 2)) << endl;
    //cout << "Collision Time 후 대공위협 위치 (x, y) : " << _ATSPos_x + ATSvX * collisionTime << " , " << _ATSPos_y + ATSvY * collisionTime << endl;
    //cout << "Collision Time 후 대공유도탄 위치 (x, y) : " << curr_pos_x + AMSvX * collisionTime << " , " << curr_pos_x + AMSvY * collisionTime << endl;
}

void AMSDynamics::send_pos()
{
    //this->FireAngle("-2000    -2000    4000    4000    340");
    while (1) {
        string msg = to_string(ATSPos_x) + "    " + to_string(ATSPos_y) + "    " + to_string(ATS_Target_x) + "    " + to_string(ATS_Target_y) + "    " + to_string(ATS_Speed);
        this->FireAngle(msg);
        this->NextPos();
        this_thread::sleep_for(chrono::milliseconds(100));

    }
}

void thread_function_state(string msg)
{
    // 파라미터로 받은 count 수치만큼 반복문을 실행한다.
    if (msg == "okay")
    {
        while (1)
        {
            // 콘솔 출력
            cout << "System is Connected!!" << endl;
            // 쓰레드의 비활성 상태 대기, 100ms간..
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }

}



int main()
{
    AMSDynamics ams = AMSDynamics("0    0"); // 생성자 : 유도탄 발사 초기위치 
    thread _t1(thread_function_state, "okay");
    ams.FireAngle("-2000    -2000    4000    4000    340");
    thread t{ &AMSDynamics::send_pos , &ams };
    _t1.join();
    t.join();
}