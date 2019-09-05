#include "stdafx.h"
#include "Paint.h"
#include "PaintDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPaintDlg dialog




CPaintDlg::CPaintDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPaintDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	isPressed = false;
	shapeType = 0;
}

void CPaintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_red);
	DDX_Control(pDX, IDC_SLIDER2, m_green);
	DDX_Control(pDX, IDC_SLIDER3, m_blue);
}

BEGIN_MESSAGE_MAP(CPaintDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_RADIO1, &CPaintDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CPaintDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CPaintDlg::OnBnClickedRadio3)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON1, &CPaintDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPaintDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDCANCEL, &CPaintDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON3, &CPaintDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDNEW, &CPaintDlg::OnBnClickedNew)
END_MESSAGE_MAP()


// CPaintDlg message handlers

BOOL CPaintDlg::OnInitDialog()
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

	
	CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);	// Set Shape Tipe Control => Circle, Triangle, Rectangle
	m_red.SetRangeMax(255);									// Set Color Range for R, G B
	m_red.SetRangeMin(0);									// R,G B is set in 0 ~ 255
	m_green.SetRangeMax(255);
	m_green.SetRangeMin(0);
	m_blue.SetRangeMax(255);
	m_blue.SetRangeMin(0);

	nCurShape = 0;											// Default Shape is Circle
	bSaved = false;											// is not saved yet
	bDrawing = true;										// Current Status means Drawing One
	try 
	{
		CFile file(L"FILE.$$", CFile::modeRead);			// Open Shapes with Seriallization
		CArchive ar(&file, CArchive::load);
		myShapes.Serialize(ar);
		nCurShape = myShapes.GetCount();					// Get Count of Shape saved
	} 
	catch(...)
	{
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPaintDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPaintDlg::OnPaint()
{														// Update Repaint from invalidate()
	CPaintDC dc(this);

	for (int i = 0; i < nCurShape; ++i)					// Draw all shapes
		myShapes[i]->draw(&dc);

	CBrush brush(RGB(255-m_red.GetPos(), 255-m_green.GetPos(), 255-m_blue.GetPos()));		//Set Shape Color with 3 slide bars


	CBrush *old = dc.SelectObject(&brush);
	dc.Rectangle(20,300, 100, 320);
	dc.SelectObject(old);
	CDialogEx::OnPaint();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPaintDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPaintDlg::OnMouseMove(UINT nFlags, CPoint point)			//Mouse Move event
{
	if (isPressed && bDrawing)									// When Drawing status (Circle, Rectangle, or Triangle Draw)
	{
		myShapes[nCurShape-1]->setEnd(point);					// Draw a shape
		Invalidate();
	}
	if (isPressed && !bDrawing && nSelected!=-1)				// When Moving status 
	{
		myShapes[nSelected]->move(point-start);					// move a shape
		start = point;
		Invalidate();
	}


	CDialogEx::OnMouseMove(nFlags, point);
}


void CPaintDlg::OnLButtonDown(UINT nFlags, CPoint point)		// Mouse Click Event
{
	isPressed = true;											// Set mouse pressed status
	if (bDrawing){												// When Drawing Status
		MyShape *s = 0;
		switch (shapeType)										// Check out Which shape draw
		{
			case 0: s = new MyCircle(); break;					// Circle
			case 1: s = new MyRectangle(); break;				// Rectangle
			case 2: s = new MySquare(); break;					// Square
		}	

		myShapes.InsertAt(nCurShape, s);						// Add into Shape list
		nCurShape++;
		s->setBg(RGB(255-m_red.GetPos(), 255-m_green.GetPos(), 255-m_blue.GetPos()));	// Set Shape Color
		s->setStart(point);										// Set Shape Position 
		s->setEnd(point);										// Set Shape Size

	}
	else{														// When Moving Status
		start = point;											// initialize
		nSelected = -1;
		for (int i = nCurShape - 1; i >= 0; i--){				// Get Selected Shape in all Shapes
			if (myShapes.GetAt(i)->inHit(point)){
				nSelected = i;
				break;
			}
		}
	}
	Invalidate();

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CPaintDlg::OnLButtonUp(UINT nFlags, CPoint point)		// Mouse Up Event
{
	isPressed = false;										// Set Mouse Up Flag
	if (bDrawing)	myShapes[nCurShape - 1]->setEnd(point);	// Have finished a shape drawing

	Invalidate();
		
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CPaintDlg::OnBnClickedRadio1()
{
	bDrawing = true;									// Set Drawing Status
	shapeType = 0;										// Shape type means Circle
}


void CPaintDlg::OnBnClickedRadio2()
{
	bDrawing = true;									// Set Drawing Status
	shapeType = 1;										// Shape type means Rectangle
}


void CPaintDlg::OnBnClickedRadio3()
{
	bDrawing = true;									// Set Drawing Status
	shapeType = 2;										// Shape type means Triangle
}


void CPaintDlg::OnOK()												// Save all shapes with serialization
{
	CFile file(L"FILE.$$", CFile::modeWrite | CFile::modeCreate);	
	CArchive ar(&file, CArchive::store);

	myShapes.RemoveAt(nCurShape, myShapes.GetCount() - nCurShape);
	myShapes.Serialize(ar);

	bSaved = true;
}

void CPaintDlg::OnBnClickedCancel()									// Exit Paint program without save
{
	
	if(bSaved==false)
		CFile file(L"FILE.$$", CFile::modeWrite | CFile::modeCreate);		// Create only Blank file
	CDialogEx::OnCancel();
}

void CPaintDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	Invalidate();

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPaintDlg::OnBnClickedButton1()
{
	
																			//Undo Method
	nCurShape--;					
	if (nCurShape < 0) nCurShape = 0;										// Current Shape Number decreased
	Invalidate();
}


void CPaintDlg::OnBnClickedButton2()
{
																			// Redo Method
	nCurShape++;
	if (nCurShape > myShapes.GetCount()) nCurShape = myShapes.GetCount();	// Current Shape Number increased
	Invalidate();
}




void CPaintDlg::OnBnClickedButton3()
{
																			// Set Move Status
	bDrawing = false;
}


void CPaintDlg::OnBnClickedNew()         //New
{
	myShapes.RemoveAll();
	nCurShape = 0;
	Invalidate();
}

