// AMS_Lib.cpp : ���� ���̺귯���� ���� �Լ��� �����մϴ�.
//


#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
using namespace std;

// AMSDynamics ������ : AMSDynamics(����ź �ʱ���ġx, ����ź �ʱ���ġy)


void thread_function(double x, double y)
{
    // �Ķ���ͷ� ���� count ��ġ��ŭ �ݺ����� �����Ѵ�.
    while (1)
    {
        // �ܼ� ���
        cout << "���� ��ġ x : " << x << "���� ��ġ y : " << y << endl;
        // �������� ��Ȱ�� ���� ���, 10ms��..
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

class AMSDynamics
{
private:
    double curr_pos_x; //�������ź�� ���� ��ġ �� x��ǥ
    double curr_pos_y; //�������ź�� ���� ��ġ �� y��ǥ

    double next_pos_x; //�������ź�� ���� ��ġ �� x��ǥ
    double next_pos_y; //�������ź�� ���� ��ġ �� y��ǥ

    double ATS_dX;
    double ATS_dY;

    double ATS_ang; // ���������� ���� ����

    double ATSvX;  //�������� x�̵��ӵ�
    double ATSvY;  //�������� y�̵��ӵ�

    double AMSangle; // �������ź�� �߻� ���� // <-�� ���� ������ �ʰ� vx, vy ���� �� �״�� ���
    double AMSvX;//�������ź X�� �̵� ��
    double AMSvY;//�������ź Y�� �̵� ��
    double AMSspeed = 340.0 * 5.0; // �������ź �ӷ� (����5 �� �ʴ� �ӷ� m)

    //double ATSPos_x; //���������� ��ġ �� x ��ǥ , �������ź�� �߻����� �޾� �߻��ϱ� ������ ��ǥ
    //double ATSPos_y; //���������� ��ġ �� y ��ǥ , �������ź�� �߻����� �޾� �߻��ϱ� ������ ��ǥ

    bool AMS_State = "False";
public:
    double samplingRate;
    double timestep = 0.1;
    double AMSPos_initial_x;
    double AMSPos_initial_y;

    AMSDynamics(string message); // AMS ������. �Ű����� : ����ź �߻� �ʱ���ġ (x, y)
    double get_Current_X();
    double get_Current_Y();
    void NextPos(double _pos_x, double _pos_y, double _angle); // �̵� �� ��ǥ ���� �Լ�, ���� ��ǥ�� �޾Ƽ� timestep ��ŭ ���� ��ǥ�� ��ȯ�Ѵ�.
    void FireAngle(string message); // arg : �������� ������ġ(x), �������� ������ġ(y), �������� ��ǥ��ġ(x), �������� ��ǥ��ġ(y),  �������� �ӷ�
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
    AMSPos_initial_x = stod(tok[0]);
    AMSPos_initial_y = stod(tok[1]);
    AMS_State = "True";
    cout << "��� ����ź �ʱ� x : " << AMSPos_initial_x << "   �������ź �ʱ� y : " << AMSPos_initial_y << endl;
}

double AMSDynamics::get_Current_X()
{
    return curr_pos_x;
}
double AMSDynamics::get_Current_Y()
{
    return curr_pos_y;
}

void AMSDynamics::NextPos(double _pos_x, double _pos_y, double _angle)
{
    curr_pos_x = _pos_x;
    curr_pos_y = _pos_y;
    AMSangle = _angle;

    curr_pos_x = curr_pos_x + (AMSspeed * cos(AMSangle) * timestep);
    curr_pos_y = curr_pos_y + (AMSspeed * sin(AMSangle) * timestep);
    //cout << "curr_x : " << curr_pos_x << endl;
    //cout << "curr_y : " << curr_pos_y << endl;
    cout << "curr_pos_x : " << curr_pos_x << endl;
    cout << "curr_pos_y : " << curr_pos_y << endl;
}

void AMSDynamics::FireAngle(string message)
// arg : �������� ������ġ(x), �������� ������ġ(y), �������� ��ǥ��ġ(x), �������� ��ǥ��ġ(y),  �������� �ӷ�
{
    // �ó������κ��� ���������� �ʱ���ġ�� ������ġ�κ��� ���������� ���� ������ ���Ѵ�.
    // ATSang = (y_target - y_init) / (x_target - x_init)
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
    double _ATSPos_x = stod(tok[0]);
    double _ATSPos_y = stod(tok[1]);
    double _ATSPos_Target_x = stod(tok[2]);
    double _ATSPos_Target_y = stod(tok[3]);
    double ATSspeed = stod(tok[4]);
    ATS_dX = (_ATSPos_Target_x - _ATSPos_x);
    ATS_dY = (_ATSPos_Target_y - _ATSPos_y);

    ATS_ang = atan2(ATS_dY, ATS_dX); // ���������� ���� ����

    ATSvX = cos(ATS_ang) * ATSspeed;  //�������� x�̵��ӵ�
    ATSvY = sin(ATS_ang) * ATSspeed;  //�������� y�̵��ӵ�

    double a = (_ATSPos_y - AMSPos_initial_y) / (_ATSPos_y - AMSPos_initial_x);
    double b = a * ATSvX + ATSvY;

    // ���������� ax**2 + bx + c = 0�� ���
    double quad_a = pow(a, 2) + 1;
    double quad_b = -2 * a * b;
    double quad_c = pow(b, 2) - pow(AMSspeed, 2);

    double sol1 = (-quad_b + sqrt(pow(quad_b, 2) - 4 * quad_a * quad_c)) / (2 * quad_a);
    double sol2 = (-quad_b - sqrt(pow(quad_b, 2) - 4 * quad_a * quad_c)) / (2 * quad_a);

    double collisionTime; // �浹���� �ҿ�ð�

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

    cout << "������� �ʱ� ��ġ (x,y) : " << _ATSPos_x << " , " << _ATSPos_y << endl;
    cout << "������� ��ǥ ��ġ (x,y) : " << _ATSPos_Target_x << " , " << _ATSPos_Target_y << endl;
    cout << "����ź �ʱ� ��ġ (x,y) : " << AMSPos_initial_x << " , " << AMSPos_initial_y << endl;
    cout << "����ź ��� �ӷ� (Vx, Vy) : " << AMSvX << " , " << AMSvY << endl;
    cout << "Collision Time : " << collisionTime << " �� �� ���" << endl;
    cout << "Collision Time �� ������� ��ġ (x, y) : " << _ATSPos_x + ATSvX * collisionTime << " , " << _ATSPos_y + ATSvY * collisionTime << endl;
    cout << "Collision Time �� �������ź ��ġ (x, y) : " << AMSPos_initial_x + AMSvX * collisionTime << " , " << AMSPos_initial_y + AMSvY * collisionTime << endl;

}
void fnAMSLib()
{


}

int main()
{

    AMSDynamics ams = AMSDynamics("0    0"); // ������ : ����ź �߻� �ʱ���ġ 
    ams.FireAngle("-2000    -2000    4000    4000    340");
    thread _t1(thread_function);
    //ams.NextPos(10.1, 10.1, 3.14 / 6);
    //ams.FireAngle("-2000, -2000,  4000, 4000,  340");
    // arg :  �������� ������ġ(x), �������� ������ġ(y) , �������� ��ǥ��ġ(x), �������� ��ǥ��ġ(y), , �������� �ӷ�
    _t1.join();
}