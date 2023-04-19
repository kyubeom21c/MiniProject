#include "AMS.h"
#include "UDPSocket.h"

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

int main(void)
{
    //
    // 시스템 초기화 명령 -> 생성자 
    // 시나리오 배포( 대공유도탄 위치 설정) -> Set 값
    // 공중위협 상황 수신 -> 발사 가능한지 판단
    // 발사 가능 여부 전송
    // 발사 명령 -> 발사 후 위치 지속적으로 송신
    //
    cout << "Hello World";

    WSASession session;
    AMSDynamics ams;
    ams.initAMS();
    
    ams.Set_AMS_System();
    thread _t1(thread_function_state, "okay");
    //발사명령
    ams.FireAngle();
    thread t{ &AMSDynamics::send_pos , &ams };
    _t1.join();
    t.join();

}