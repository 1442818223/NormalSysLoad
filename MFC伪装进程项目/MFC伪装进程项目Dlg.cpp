
// MFC伪装进程项目Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFC伪装进程项目.h"
#include "MFC伪装进程项目Dlg.h"
#include "afxdialogex.h"

#include <iostream>
#include "gmane.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFC伪装进程项目Dlg 对话框



CMFC伪装进程项目Dlg::CMFC伪装进程项目Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC伪装进程项目Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFC伪装进程项目Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CMFC伪装进程项目Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFC伪装进程项目Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMFC伪装进程项目Dlg 消息处理程序

BOOL CMFC伪装进程项目Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 获取编辑框控件并设置默认文本////////////////////////////////////////////////////////////////////////////////////////////
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	if (pEdit != NULL)
	{
		pEdit->SetWindowText(_T("PID"));
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFC伪装进程项目Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFC伪装进程项目Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFC伪装进程项目Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC伪装进程项目Dlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	 // 获取编辑框控件
   // 获取编辑框控件
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);





}


void CMFC伪装进程项目Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	

	DWORD dwValue = 0;
	
	 // 获取编辑框控件
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
	if (pEdit != NULL)
	{
		CString strText;
		pEdit->GetWindowText(strText);  // 获取编辑框中的文本

		// 将 CString 转换为 DWORD
		
		if (!strText.IsEmpty())
		{
			// 尝试将文本转换为 DWORD（十进制数值）
			if (_stscanf_s(strText, _T("%d"), &dwValue) != 1)
			{
				AfxMessageBox(_T("输入无效，请输入有效的PID"));
				return;  // 如果转换失败，则弹出错误消息并返回
			}
		}


		// 显示保存成功的消息
		//CString msg;
		//msg.Format(_T("保存的 DWORD 值为: %d"), dwValue);  // 使用 %d 格式说明符显示十进制
		//AfxMessageBox(msg);


	}

	BOOLEAN result = LOADENTRY((UINT)dwValue);





}
