#include "AMS.h"
#include "UDPSocket.h"

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

int main(void)
{
    //
    // �ý��� �ʱ�ȭ ��� -> ������ 
    // �ó����� ����( �������ź ��ġ ����) -> Set ��
    // �������� ��Ȳ ���� -> �߻� �������� �Ǵ�
    // �߻� ���� ���� ����
    // �߻� ��� -> �߻� �� ��ġ ���������� �۽�
    //
    cout << "Hello World";

    WSASession session;
    AMSDynamics ams;
    ams.initAMS();

    // �ʱ� �ó����� ����
    ams.receiveScenario();

    thread _t1(thread_function_state, "okay");
    //�߻���
    ams.receiveScenario();
    ams.FireAngle();
    thread t{ &AMSDynamics::send_pos , &ams };

    ams.receiveScenario();

    _t1.join();
    t.join();
}