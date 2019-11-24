
// SampleDlg.h: 헤더 파일
//

#pragma once


// CSampleDlg 대화 상자
class CSampleDlg : public CDialogEx
{
	// 생성입니다.
public:
	CSampleDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.

  // 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAMPLE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   // DDX/DDV 지원입니다.


  // 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString status_edit;
	static UINT ThreadStart(LPVOID pParam);
	static DWORD Message_threadProc(LPVOID pParam);
	static UINT Key_Event(LPVOID pParam);
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnEnChangeName();
};
