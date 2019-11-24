#include <stdexcept>
#include "monitoring.h"

#define     THRD_MESSAHE_CONNECT_DEVICE   WM_APP	//첫 기기연결
#define		THRD_MESSAGE_SOMEWORK		  WM_APP + 1 //스크린락	
#define		THRD_MESSAGE_EXIT			  WM_APP + 2 //스크린락 해지
#define     THRD_MESSAGE_NOSIGNAL_DEVICE  WM_APP + 3 //블루투스 꺼져 있음
#define     THRD_MESSAGE_NODEVICE         WM_APP + 4 //연결된 기기 없음
#define     THRD_CLOSE					  WM_APP + 5  //모든쓰레드종료

using namespace std;

BLUETOOTH_DEVICE_INFO m_device_info;

namespace Monitoring
{
	bool connect_flag = true;
	int lock_state = 0;
	int MSG_repeat_control = 0;
	bool bContinue = true;
	HANDLE M_Thread;	//모니터링 진입 쓰레드핸들
	DWORD m_idThread = 0;	//메인 앱 쓰레드 ID (메세지 전송할 ID)
	DWORD bt_idThread = 0;	//블루투스 쓰레드 ID
	DWORD WINAPI ThreadProc_BT(LPVOID);

	BLUETOOTH_FIND_RADIO_PARAMS m_bt_find_radio = { sizeof(BLUETOOTH_FIND_RADIO_PARAMS) };
	BLUETOOTH_DEVICE_SEARCH_PARAMS m_search_params = {
		sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS),
		1,
		0,
		1,
		1,
		1,
		15,
		NULL
	};
	BLUETOOTH_DEVICE_INFO S_device_info = { sizeof(BLUETOOTH_DEVICE_INFO), 0, };
	BLUETOOTH_DEVICE_INFO C_device_info = { sizeof(BLUETOOTH_DEVICE_INFO), 0, };
	BLUETOOTH_RADIO_INFO m_bt_info = { sizeof(BLUETOOTH_RADIO_INFO), 0, };

	void MonitoringFuncs::checkConn(BLUETOOTH_DEVICE_INFO& master_device, BLUETOOTH_DEVICE_INFO& now_device) {
		printf("메세지 전달 쓰레드 ID : %d\n", m_idThread);

		if (master_device.Address.ullLong == 0) {	//master 결정 전
			if (now_device.fConnected) {
				master_device.Address.ullLong = now_device.Address.ullLong;
				PostThreadMessage(get_thread_id, WM_APP, 0, 0);
				printf("WM : %d\n", WM_APP);
				printf("체크문1번(첫 기기연결)\n");
				connect_flag = true;
			}
			else {
				if (MSG_repeat_control != 1) {
					printf("체크문2번(연결된 기기없음)\n");
					PostThreadMessage(get_thread_id, WM_APP + 4, 0, 0);
					printf("WM : %d\n", WM_APP);
					printf("%d\n", GetLastError());
					MSG_repeat_control = 1;
					master_device.Address.ullLong = 0;
					connect_flag = false;
				}
			}
		}
		else {	//master 결정 후
			if (master_device.Address.ullLong) {
				if (lock_state == 1) {	//잠겨있을 때
					if (master_device.Address.ullLong == now_device.Address.ullLong) {
						if (now_device.fConnected && MSG_repeat_control != 4) {
							MSG_repeat_control = 4;	//잠겼다가 풀리는 경우
							printf("%d\n", MSG_repeat_control);
						}
					}
				}
				else {	//안잠겨있을 때
					if (master_device.Address.ullLong != now_device.Address.ullLong) {
						if (MSG_repeat_control != 3 /*&& connect_flag*/) {
							MSG_repeat_control = 3;	//잠기는 경우
							printf("%d\n", MSG_repeat_control);
						}
					}
					else {
						if (now_device.fConnected) {
							MSG_repeat_control = 0;	//안잠겨있고 연결된 경우
							printf("%d\n", MSG_repeat_control);
						}
						else {
							MSG_repeat_control = 2;
							printf("%d\n", MSG_repeat_control);
						}
					}
				}
			}
			else {
				if (lock_state == 0) {
					if (MSG_repeat_control != 2) {
						MSG_repeat_control = 2;	//안잠겨있고 연결 안된 경우
						printf("%d\n", MSG_repeat_control);
					}
				}
			}
		}
	}

	void MonitoringFuncs::bthMain() {		//블루투스 메인함수
		HANDLE m_radio = NULL;
		HBLUETOOTH_RADIO_FIND m_bt = NULL;
		HBLUETOOTH_DEVICE_FIND m_bt_dev = NULL;
		int m_device_id = 0;
		DWORD mbtinfo_ret;

		printf("Btmain접근\n");
		while (bContinue)
		{
			printf("접근1(radio)\n");
			m_bt = BluetoothFindFirstRadio(&m_bt_find_radio, &m_radio);

			do {
				if (m_bt == 0) {
					if (MSG_repeat_control != 5) {
						//컴퓨터 블루투스가 꺼진 상황

						PostThreadMessage(get_thread_id, WM_APP + 3, 0, 0);
						printf("WM : %d\n", WM_APP);
						MSG_repeat_control = 5;
					}
					break;
				}
				// Then get the radio device info....
				mbtinfo_ret = BluetoothGetRadioInfo(m_radio, &m_bt_info);
				m_search_params.hRadio = m_radio;
				ZeroMemory(&S_device_info, sizeof(BLUETOOTH_DEVICE_INFO));
				S_device_info.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);

				m_bt_dev = BluetoothFindFirstDevice(&m_search_params, &S_device_info);
				m_device_id = 0;

				do
				{
					printf("접근2(device)\n");
					m_device_info = S_device_info;
					Monitoring::MonitoringFuncs::checkConn(C_device_info, S_device_info);
					m_device_id++;
					if (MSG_repeat_control == 0 || MSG_repeat_control == 4) break;
				} while (BluetoothFindNextDevice(m_bt_dev, &S_device_info));

				GetLastError();
				if (MSG_repeat_control == 0 || MSG_repeat_control == 4) break;
			} while (BluetoothFindNextRadio(&m_bt_find_radio, &m_radio));


			GetLastError();
			if (MSG_repeat_control == 4) {
				Monitoring::MonitoringFuncs::checkConn(C_device_info, S_device_info);
				if (MSG_repeat_control == 4) {
					printf("체크문3번(스크린락 해지)\n");
					PostThreadMessage(get_thread_id, WM_APP + 2, 0, 0);
					printf("WM : %d\n", WM_APP);
					lock_state = 0;
				}
			}
			else if (MSG_repeat_control == 2 || MSG_repeat_control == 3) {
				int lock_cnt = 0;
				for (int i = 0; i < 3; i++) {
					Monitoring::MonitoringFuncs::checkConn(C_device_info, S_device_info);
					if (MSG_repeat_control == 3) lock_cnt++;
					printf("다른경우 안정성 테스트 %d\n", i);
					Sleep(1000);
				}
				if (lock_cnt == 3) {
					printf("체크문4번(스크린락 실행)\n");
					PostThreadMessage(get_thread_id, WM_APP + 1, 0, 0);
					printf("WM : %d\n", WM_APP);
					lock_state = 1;
				}
					printf("체크문5번(스크린락 실행)\n");
					PostThreadMessage(get_thread_id, WM_APP + 1, 0, 0);
			}
			else if (MSG_repeat_control == 0) {
				printf("이상 없음\n");
			}

			Sleep(3000);
		}
	}

	void MonitoringFuncs::stopMon() {  //메인쓰레드에서 호출하는 함수
		printf("stopMon() 함수 실행\n");
		Monitoring::bContinue = false;
	}

	void MonitoringFuncs::InitMon(DWORD ThreadID) {   //메인쓰레드에서 메인쓰레드ID를 매개변수로 호출하는 함수
		Monitoring::m_idThread = ThreadID;	//받아온 쓰레드 ID 값
		printf("받은 메인쓰레드 ID : %d\n", m_idThread);
		startMon();
	}

	void MonitoringFuncs::UnInitMon() {   //메인스레드에서 호출하는 함수
		printf("UnInitMon()함수 실행\n");
		CloseHandle(Monitoring::M_Thread);  //핸들 닫힘
	}

	void MonitoringFuncs::startMon() {
		const char* M_Thread_Msg = "블루투스 쓰레드 생성완료 "; // 프록시파라미터

		M_Thread = CreateThread(NULL, 0, ThreadProc_BT, (LPVOID)M_Thread_Msg, 0, &bt_idThread);
		WaitForSingleObject(M_Thread, INFINITE);

		bContinue = true;	//stopMon()실행 후 다시 시작하기 위해 flag값 변환
	}

	DWORD WINAPI ThreadProc_BT(LPVOID lpParam)
	{
		char* TP_Msg = (char*)lpParam; // 프록시로 들어온 파라미터
		printf("%s :: 블루투스 체크 쓰레드 ID: %d\n", TP_Msg, bt_idThread);

		//여기서 이제 블루투스함수진입
		Monitoring::MonitoringFuncs::bthMain();

		return (DWORD)lpParam;
	}
}