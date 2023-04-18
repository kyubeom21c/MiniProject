// AMS_Lib.cpp : ���� ���̺귯���� ���� �Լ��� �����մϴ�.
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

    double collisionTime;

public:
    AMSDynamics(string message); // AMS ������. �Ű����� : ����ź �߻� �ʱ���ġ (x, y)
    double get_Current_X();
    double get_Current_Y();
    void set_X(double x);
    void set_Y(double y);
    void NextPos(); // �̵� �� ��ǥ ���� �Լ�, ���� ��ǥ�� �޾Ƽ� timestep ��ŭ ���� ��ǥ�� ��ȯ�Ѵ�.
    void FireAngle(string message); // arg : �������� ������ġ(x), �������� ������ġ(y), �������� ��ǥ��ġ(x), �������� ��ǥ��ġ(y),  �������� �ӷ�
    void send_pos();
    void AMS_INFO(double _ATSPos_x, double _ATSPos_y, double _ATSPos_Target_x, double _ATSPos_Target_y, double collisionTime);
};

AMSDynamics::AMSDynamics(string message)
{
    message.insert(message.length(), "    "); // ���ڿ� �������� ���� �߰�
    string delimiter = "    ";
    vector<string> tok{}; // message�� tokenizing�� token�� ������ vector	
    size_t pos = 0;
    // pos�� string::npos�� ����ų ��, ��, ���ڿ��� ���� ������ �� ���� Ž��
    while ((pos = message.find(delimiter)) != string::npos) {
        // 0 ~ ������ ���� �� ���� ���ڿ��� �߶󳻾� tok vector�� push
        tok.push_back(message.substr(0, pos));
        // �߶� ���ڿ��� erase �Լ��� �̿��� ����
        message.erase(0, pos + delimiter.length());
    }

    //for (const auto& s : tok)
      //  cout << s << endl;
    curr_pos_x = stod(tok[0]);
    curr_pos_y = stod(tok[1]);
    AMS_State = "True";
    cout << "��� ����ź �ʱ� x : " << curr_pos_x << "   �������ź �ʱ� y : " << curr_pos_y << endl;
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
// arg : �������� ������ġ(x), �������� ������ġ(y), �������� ��ǥ��ġ(x), �������� ��ǥ��ġ(y),  �������� �ӷ�
{
    // �ó������κ��� ���������� �ʱ���ġ�� ������ġ�κ��� ���������� ���� ������ ���Ѵ�.
    message.insert(message.length(), "    "); // ���ڿ� �������� ���� �߰�
    string delimiter = "    ";
    vector<string> tok{}; // message�� tokenizing�� token�� ������ vector   
    size_t pos = 0;
    // pos�� string::npos�� ����ų ��, ��, ���ڿ��� ���� ������ �� ���� Ž��
    while ((pos = message.find(delimiter)) != string::npos) {
        // 0 ~ ������ ���� �� ���� ���ڿ��� �߶󳻾� tok vector�� push
        tok.push_back(message.substr(0, pos));
        // �߶� ���ڿ��� erase �Լ��� �̿��� ����
        message.erase(0, pos + delimiter.length());
    }
    ATSPos_x = stod(tok[0]);
    ATSPos_y = stod(tok[1]);
    ATS_Target_x = stod(tok[2]);
    ATS_Target_y = stod(tok[3]);
    ATS_Speed = stod(tok[4]);
    ATS_dX = (ATS_Target_x - ATSPos_x);
    ATS_dY = (ATS_Target_x - ATSPos_y);

    ATS_ang = atan2(ATS_dY, ATS_dX); // ���������� ���� ����

    ATSvX = cos(ATS_ang) * ATS_Speed;  //�������� x�̵��ӵ�
    ATSvY = sin(ATS_ang) * ATS_Speed;  //�������� y�̵��ӵ�

    double a = (ATSPos_y - curr_pos_y) / (ATSPos_y - curr_pos_x);
    double b = a * ATSvX + ATSvY;

    // ���������� ax**2 + bx + c = 0�� ���
    double quad_a = pow(a, 2) + 1;
    double quad_b = -2 * a * b;
    double quad_c = pow(b, 2) - pow(AMSspeed, 2);

    double sol1 = (-quad_b + sqrt(pow(quad_b, 2) - 4 * quad_a * quad_c)) / (2 * quad_a);
    double sol2 = (-quad_b - sqrt(pow(quad_b, 2) - 4 * quad_a * quad_c)) / (2 * quad_a);

    double collisionTime; // �浹���� �ҿ�ð�

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

    cout << "������� ��ġ (x,y) : " << _ATSPos_x << " , " << _ATSPos_y << endl;
    cout << "����ź ��ġ (x,y) : " << curr_pos_x << " , " << curr_pos_y << endl;
    cout << "Collision Time : " << collisionTime << " �� �� ���" << endl;
    cout << "Distance AMS - ATS : " << sqrt(pow((_ATSPos_x - curr_pos_x), 2) + pow((_ATSPos_y - curr_pos_y), 2)) << endl;
    //cout << "Collision Time �� ������� ��ġ (x, y) : " << _ATSPos_x + ATSvX * collisionTime << " , " << _ATSPos_y + ATSvY * collisionTime << endl;
    //cout << "Collision Time �� �������ź ��ġ (x, y) : " << curr_pos_x + AMSvX * collisionTime << " , " << curr_pos_x + AMSvY * collisionTime << endl;
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
    // �Ķ���ͷ� ���� count ��ġ��ŭ �ݺ����� �����Ѵ�.
    if (msg == "okay")
    {
        while (1)
        {
            // �ܼ� ���
            cout << "System is Connected!!" << endl;
            // �������� ��Ȱ�� ���� ���, 100ms��..
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }

}



int main()
{
    AMSDynamics ams = AMSDynamics("0    0"); // ������ : ����ź �߻� �ʱ���ġ 
    thread _t1(thread_function_state, "okay");
    ams.FireAngle("-2000    -2000    4000    4000    340");
    thread t{ &AMSDynamics::send_pos , &ams };
    _t1.join();
    t.join();
}