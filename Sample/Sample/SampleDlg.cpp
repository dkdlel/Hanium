
// SampleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Sample.h"
#include "SampleDlg.h"
#include "afxdialogex.h"
#include "screen_lock.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "irprops.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSampleDlg 대화 상자
DWORD get_thread_id; // getMessage를 위한 쓰레드
DWORD Monitoring_Threadid; // 버튼 클릭시 생기는 쓰레드
DWORD lock_id;
screen_lock dlg;
bool flag_lock = false;
extern BLUETOOTH_DEVICE_INFO m_device_info;

CSampleDlg::CSampleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SAMPLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(Start, &CSampleDlg::OnBnClickedStart)
	ON_BN_CLICKED(Stop, &CSampleDlg::OnBnClickedStop)
	ON_EN_CHANGE(Name, &CSampleDlg::OnEnChangeName)
END_MESSAGE_MAP()


// CSampleDlg 메시지 처리기

BOOL CSampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Message_threadProc, 0, 0, &get_thread_id);
	CSampleDlg::OnBnClickedStart();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);         // 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);      // 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
UINT CSampleDlg::Key_Event(LPVOID pParam) // Screen Lock
{
	/*while (flag_lock) {
		screen_lock dlg;
		dlg.DoModal();
		Sleep(1000);
	}*/
	dlg.DoModal();
	return 0;
}

DWORD CSampleDlg::Message_threadProc(LPVOID lParam) // GetMessage하는 부분
{

	CSampleDlg* ptr = (CSampleDlg*)lParam;
	get_thread_id = GetCurrentThreadId();
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		if (msg.message == WM_APP) //첫 기기연결
		{
			CString tmp = m_device_info.szName;
			CWnd* pWnd = AfxGetApp()->GetMainWnd();
			HWND hWnd = pWnd->m_hWnd;
			::SetDlgItemText(hWnd, Name, tmp);
		}
		else if (msg.message == WM_APP + 1) //스크린락   
		{
			//CWinThread* event_thread = AfxBeginThread(Key_Event, ptr);
			//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ptr->Key_Event, 0, 0, &lock_id);
			

			/*if (!flag_modal) {
				dlg.DoModal();
				flag_modal = true;
			}*/

			if (!flag_lock) {
				CWinThread* event_thread = AfxBeginThread(Key_Event, ptr);
				flag_lock = true;
			}
		}
		else if (msg.message == WM_APP + 2) //스크린락 해지
		{
			flag_lock = false;
			::SendMessage(dlg, WM_CLOSE, NULL, NULL);
		}
		else if (msg.message == WM_APP + 3) //블루투스 꺼져 있음
		{
			CString tmp = (CString) "블루투스 꺼져 있음";
			CWnd* pWnd = AfxGetApp()->GetMainWnd();
			HWND hWnd = pWnd->m_hWnd;
			::SetDlgItemText(hWnd, Name, tmp);
		}
		else if (msg.message == WM_APP + 4) //연결된 기기 없음
		{
			CString tmp = (CString) "연결된 기기 없음";
			CWnd* pWnd = AfxGetApp()->GetMainWnd();
			HWND hWnd = pWnd->m_hWnd;
			::SetDlgItemText(hWnd, Name, tmp);
		}
		else if (msg.message == WM_APP + 5) //모든 쓰레드 종료
		{
			CString tmp = (CString) "모든 쓰레드 종료";
			CWnd* pWnd = AfxGetApp()->GetMainWnd();
			HWND hWnd = pWnd->m_hWnd;
			::SetDlgItemText(hWnd, Name, tmp);
		}
		DispatchMessage(&msg);
	}

	return 0;
}

void CSampleDlg::OnBnClickedStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CWinThread* main_thread = AfxBeginThread(ThreadStart, this);
	GetDlgItem(Start)->EnableWindow(FALSE);
}

UINT CSampleDlg::ThreadStart(LPVOID pParam) {

	CSampleDlg* ptr = (CSampleDlg*)pParam;
	Monitoring_Threadid = GetCurrentThreadId();
	Monitoring::MonitoringFuncs::InitMon(Monitoring_Threadid);
	return 0;
}
void CSampleDlg::OnBnClickedStop()
{
	Monitoring::MonitoringFuncs::stopMon();
	Monitoring::MonitoringFuncs::UnInitMon();
	GetDlgItem(Start)->EnableWindow(TRUE);
}

void CSampleDlg::OnEnChangeName()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
