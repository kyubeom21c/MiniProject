#include "AMS.h"

int cnt = 0;

AMSDynamics::AMSDynamics()
{
    
}

void AMSDynamics::initAMS()
{
    udpSocket.Bind(SOCKET_PORT);
}



void AMSDynamics::receiveScenario()
{
    char buffer[100];
    udpSocket.RecvFrom(buffer, 100);
    string receivedMsg(buffer); //�� �Ľ� 
    int count = 0;
    for (int i = 0; i < receivedMsg.length(); i++) {
        if (receivedMsg.find(',') == -1) {
            break;
        }
        else if (receivedMsg.find(',', i) <= i) {
            count += 1;
        }
    }
    if (count > 2)
        ATS_POS = receivedMsg;
    else
        AMS_scenario = receivedMsg;
}

void AMSDynamics::sendXY()
{
    string sendMsg = to_string(curr_pos_x) + "," + to_string(curr_pos_y); //����  
    udpSocket.SendTo(OPC_IP_ADDRESS, OPC_DATA_PORT, sendMsg.c_str(), sendMsg.size());
}

void AMSDynamics::sendLife()
{
    string sendMsg = "1"; //���� 
    udpSocket.SendTo(OPC_IP_ADDRESS, OPC_DATA_PORT, sendMsg.c_str(), sendMsg.size());
}

void AMSDynamics::Set_AMS_System()
{
    AMS_scenario.insert(AMS_scenario.length(), ","); // ���ڿ� �������� ���� �߰�
    string delimiter = ",";
    vector<string> tok{}; // message�� tokenizing�� token�� ������ vector	
    size_t pos = 0;
    // pos�� string::npos�� ����ų ��, ��, ���ڿ��� ���� ������ �� ���� Ž��
    while ((pos = AMS_scenario.find(delimiter)) != string::npos) {
        // 0 ~ ������ ���� �� ���� ���ڿ��� �߶󳻾� tok vector�� push
        tok.push_back(AMS_scenario.substr(0, pos));
        // �߶� ���ڿ��� erase �Լ��� �̿��� ����
        AMS_scenario.erase(0, pos + delimiter.length());
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


void AMSDynamics::FireAngle()
// arg : �������� ������ġ(x), �������� ������ġ(y), �������� ��ǥ��ġ(x), �������� ��ǥ��ġ(y),  �������� �ӷ�
{
    // �ó������κ��� ���������� �ʱ���ġ�� ������ġ�κ��� ���������� ���� ������ ���Ѵ�.
    ATS_POS.insert(ATS_POS.length(), ","); // ���ڿ� �������� ���� �߰�
    string delimiter = ",";
    vector<string> tok{}; // message�� tokenizing�� token�� ������ vector   
    size_t pos = 0;
    // pos�� string::npos�� ����ų ��, ��, ���ڿ��� ���� ������ �� ���� Ž��
    while ((pos = ATS_POS.find(delimiter)) != string::npos) {
        // 0 ~ ������ ���� �� ���� ���ڿ��� �߶󳻾� tok vector�� push
        tok.push_back(ATS_POS.substr(0, pos));
        // �߶� ���ڿ��� erase �Լ��� �̿��� ����
        ATS_POS.erase(0, pos + delimiter.length());
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
        string msg = to_string(ATSPos_x) + "," + to_string(ATSPos_y) + "," + to_string(ATS_Target_x) + "," + to_string(ATS_Target_y) + "," + to_string(ATS_Speed);
        this->FireAngle();
        this->NextPos();
        this_thread::sleep_for(chrono::milliseconds(100));

    }
}

