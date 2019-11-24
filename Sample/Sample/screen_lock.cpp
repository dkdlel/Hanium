// screen_lock.cpp: 구현 파일
//

#include "pch.h"
#include "Sample.h"
#include "screen_lock.h"
#include "afxdialogex.h"


// screen_lock 대화 상자

IMPLEMENT_DYNAMIC(screen_lock, CDialogEx)

screen_lock::screen_lock(CWnd* pParent /*=nullptr*/)
	: CDialogEx(screen_lock_page, pParent)
{

}

int screenx;
int screeny;

int x;
int y;

BOOL screen_lock::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.
	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	int cxIcon = GetSystemMetrics(SM_CXICON);
	int cyIcon = GetSystemMetrics(SM_CYICON);
	screenx = GetSystemMetrics(SM_CXSCREEN);
	screeny = GetSystemMetrics(SM_CYSCREEN);

	// resize:
	//SetWindowPos(NULL, 0, 0, screenx , screeny, SWP_NOZORDER);
	//MoveWindow(screenx / 2 - 346, screeny / 2 - 461 / 2, 346, 461);
	//SetBackgroundColor(RGB(255, 255, 255));

	MoveWindow(0, 0, screenx, screeny, TRUE);
	SetBackgroundColor(RGB(255, 255, 255));
	CStatic* rct = (CStatic*)GetDlgItem(IDC_STATIC);
	CRect rect;
	rct->GetClientRect(rect); // Static 의 크기를 얻어옴
	rct->MoveWindow(screenx/2 - 346 / 2, screeny/2 - 461 / 2, 346, 461); // Static 의 크기 조절
	//CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

	//CDC MemDC;
	//CBitmap bmp;

	//CRect rct;
	//this->GetClientRect(&rct);


	//// Then load the bitmap to the memory.
	//BITMAP bm;
	//MemDC.CreateCompatibleDC(&dc);
	//bmp.LoadBitmap(IDB_BITMAP1);   // 3) resource에 열쇠 이미지를 bitmap resource로 추가하였습니다. (IDB_BKIMG_BITMAP)
	//MemDC.SelectObject(&bmp);
	//bmp.GetBitmap(&bm);

	//// dialog surface.
	//dc.StretchBlt(0, 0, 346 / 2, 461 / 2, &MemDC, screenx - 346 / 2, screeny - 461 / 2, bm.bmWidth, bm.bmHeight, SRCCOPY);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

//void screen_lock::OnPaint()
//{
//	if (IsIconic())
//	{
//		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
//
//		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
//
//		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
//		int cxIcon = GetSystemMetrics(SM_CXICON);
//		int cyIcon = GetSystemMetrics(SM_CYICON);
//		CRect rect;
//		GetClientRect(&rect);
//		x = (rect.Width() - cxIcon + 1) / 2;
//		y = (rect.Height() - cyIcon + 1) / 2;
//
//		// 아이콘을 그립니다.
//		dc.DrawIcon(x, y, m_hIcon);
//	}
//	// 2) 배경화면으로 이미지 출력시, 아래 부분을 추가 하시면 됩니다.
//	else
//	{
//		CStatic* test = (CStatic*)GetDlgItem(IDC_STATIC);
//		CRect rec;
//		test->GetClientRect(rec); // Static 의 크기를 얻어옴
//		test->MoveWindow(screenx / 2 - x / 2, screeny / 2 - y / 2, x / 2, y / 2); // Static 의 크기 조절
//
//		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트
//
//		CDC MemDC;
//		CBitmap bmp;
//
//		CRect rct;
//		this->GetClientRect(&rct);
//
//		// Then load the bitmap to the memory.
//		BITMAP bm;
//		MemDC.CreateCompatibleDC(&dc);
//		bmp.LoadBitmap(IDB_BITMAP1);   // 3) resource에 열쇠 이미지를 bitmap resource로 추가하였습니다. (IDB_BKIMG_BITMAP)
//		MemDC.SelectObject(&bmp);
//		bmp.GetBitmap(&bm);
//
//		// dialog surface.
//		dc.StretchBlt(screenx - x / 2, screeny - y / 2, x / 2, y / 2, &MemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
//
//		//CDialogEx::OnPaint();
//	}
//
//}


screen_lock::~screen_lock()
{
}

void screen_lock::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(screen_lock, CDialogEx)
END_MESSAGE_MAP()


// screen_lock 메시지 처리기
