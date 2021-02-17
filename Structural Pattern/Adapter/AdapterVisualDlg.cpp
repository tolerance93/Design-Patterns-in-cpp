
// AdapterVisualDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AdapterVisual.h"
#include "AdapterVisualDlg.h"
#include "afxdialogex.h"

#include <memory>
#include <map>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

vector<shared_ptr<VectorObject>> vectorObjects {
  make_shared<VectorRectangle>(10,10,100,100),
  make_shared<VectorRectangle>(30,30,60,60)
};

/**
 * 선분 하나를 점의 집합으로 만들어 저장하고, 각 점들을 순회할 수 있도록 반복자로 노출한다.
 * 화면이 업데이트 될 때마다 DrawPoints가 불린다면 같은 것을 계속 새로 생성하는 문제 -> 캐싱!
 */
struct LineToPointAdapter
{
  typedef vector<Point> Points;

  LineToPointAdapter(Line& line)
  {
    static int count = 0;
    TRACE("%d: Generating points for line (no caching)\n", count++);

    // no interpolation
    int left = min(line.start.x, line.end.x);
    int right = max(line.start.x, line.end.x);
    int top = min(line.start.y, line.end.y);
    int bottom = max(line.start.y, line.end.y);
    int dx = right - left;
    int dy = line.end.y - line.start.y;

    // only vertical or horizontal lines
      /**
       * 사각형을 그리기때문에 수평/수직선만 표현한다.
       * 선분 -> 점 변환이 생성자에서 일어난다. 성급한 접근법!
       * 어댑터 생성 즉시 변환 작업을 하는 것이 아니라, 실 사용 시점에 변환 작업이 수행되도록(lazy) 접근법도 생각해 보자.
       */
    if (dx == 0)
    {
      // vertical
      for (int y = top; y <= bottom; ++y)
      {
        points.emplace_back(Point{ left,y });
      }
    }
    else if (dy == 0)
    {
      for (int x = left; x <= right; ++x)
      {
        points.emplace_back(Point{ x, top });
      }
    }
  }

  virtual Points::iterator begin() { return points.begin(); }
  virtual Points::iterator end() { return points.end(); }
private:
  Points points;
};

/**
 * 캐싱 추가
 */
struct LineToPointCachingAdapter
{
  typedef vector<Point> Points;

  LineToPointCachingAdapter(Line& line)
  {
    boost::hash<Line> hash;
    line_hash = hash(line);
      /**
       * 캐시에 존재하면 변환하지 않음
       */
    if (cache.find(line_hash) != cache.end()) return;

    static int count = 0;
    TRACE("%d: Generating points for line (with caching)\n", count++);

    // no interpolation
    Points points;

    int left = min(line.start.x, line.end.x);
    int right = max(line.start.x, line.end.x);
    int top = min(line.start.y, line.end.y);
    int bottom = max(line.start.y, line.end.y);
    int dx = right - left;
    int dy = line.end.y - line.start.y;

    // only vertical or horizontal lines
    if (dx == 0)
    {
      // vertical
      for (int y = top; y <= bottom; ++y)
      {
        points.emplace_back(Point{ left,y });
      }
    }
    else if (dy == 0)
    {
      for (int x = left; x <= right; ++x)
      {
        points.emplace_back(Point{ x, top });
      }
    }
/**
 * 캐시에 삽입
 * 더이상 필요 없어진 오래된 점은 어떻게 처리할까?
 */
    cache[line_hash] = points;
  }

  virtual Points::iterator begin() { return cache[line_hash].begin(); }
  virtual Points::iterator end() { return cache[line_hash].end(); }
private:
  size_t line_hash;
    /**
     * 캐시를 들고 있음
     */
  static map<size_t, Points> cache;
};

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAdapterVisualDlg dialog



CAdapterVisualDlg::CAdapterVisualDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADAPTERVISUAL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAdapterVisualDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAdapterVisualDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CAdapterVisualDlg message handlers

BOOL CAdapterVisualDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAdapterVisualDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAdapterVisualDlg::OnPaint()
{
  CPaintDC dc(this); // device context for painting

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
    for (auto& o : vectorObjects)
    {
      for (auto& l : *o)
      {
          /**
           * 그림을 그리기위한 인터페이스는 DrawPoints밖에 없어서 Adapter가 필요!
           * 기하 도형에서 선분 집합을 정의하면 그 선분들로 lpo를 생성하여 점들의 집합으로 변환하고, 그 점들을 순회할 수 있는
           * 시작 반복자와 끝  반복자를 DrawPoints에 넘겨주어 그림을 그린다.
           * 스택에 어댑터를 생성하면 함수가 종료될때 사라지지 않을까? 캐시가 무의미할텐데... => cache가 static variable!!
           */
        LineToPointCachingAdapter lpo{ l };
        DrawPoints(dc, lpo.begin(), lpo.end());
      }
    }
    
    CDialogEx::OnPaint();
	}

}

map<size_t, vector<Point>> LineToPointCachingAdapter::cache;

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAdapterVisualDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

