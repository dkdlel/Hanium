#pragma once


// screen_lock 대화 상자

class screen_lock : public CDialogEx
{
	DECLARE_DYNAMIC(screen_lock)

public:
	screen_lock(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~screen_lock();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = screen_lock_page };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Pic;
	afx_msg void OnPaint();
	BOOL screen_lock::OnInitDialog();
};
