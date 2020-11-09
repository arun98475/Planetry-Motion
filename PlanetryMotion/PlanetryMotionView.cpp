
// PlanetryMotionView.cpp : implementation of the CPlanetryMotionView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PlanetryMotion.h"
#endif

#include "PlanetryMotionDoc.h"
#include "PlanetryMotionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlanetryMotionView

IMPLEMENT_DYNCREATE(CPlanetryMotionView, CView)

BEGIN_MESSAGE_MAP(CPlanetryMotionView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_ANIMATION_START, &CPlanetryMotionView::OnAnimationStart)
	ON_COMMAND(ID_ANIMATION_STOP, &CPlanetryMotionView::OnAnimationStop)
	ON_COMMAND(ID_ANIMATION_RESUME, &CPlanetryMotionView::OnAnimationResume)
//	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CPlanetryMotionView construction/destruction

CPlanetryMotionView::CPlanetryMotionView() noexcept
{
	RadOrbit1=300;
	RadOrbit2=200;
	RadSun=60;
	i = 0;
	j = 65;
	newThread = NULL;
	secThread = NULL;
	dx = -20;
	ecc = 15;

}

CPlanetryMotionView::~CPlanetryMotionView()
{
}

BOOL CPlanetryMotionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPlanetryMotionView drawing

void CPlanetryMotionView::OnDraw(CDC* pDC)
{
	CPlanetryMotionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect clientRect,orbitA,orbitB,sun;
	GetClientRect(clientRect);
	winCenterX = clientRect.CenterPoint().x;
	winCenterY = clientRect.CenterPoint().y;
	CPen redPen;
	redPen.CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&redPen);
	pDC->Ellipse(winCenterX - RadOrbit1, winCenterY - RadOrbit1, winCenterX + RadOrbit1, winCenterY + RadOrbit1);
	pDC->SelectObject(pOldPen);
	CPen greenPen;
	greenPen.CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
	CPen* pOldPen2 = pDC->SelectObject(&greenPen);
	pDC->BeginPath();
	pDC->Ellipse(winCenterX - RadOrbit1, winCenterY - RadOrbit1, winCenterX + RadOrbit1, winCenterY + RadOrbit1);
	pDC->Ellipse(winCenterX - RadOrbit2 + ecc, winCenterY+dx - RadOrbit2, winCenterX + RadOrbit2 - ecc, winCenterY + dx + RadOrbit2);
	pDC->EndPath();
	pDC->Ellipse(winCenterX - RadOrbit2+ecc, winCenterY + dx - RadOrbit2, winCenterX + RadOrbit2 - ecc, winCenterY + dx + RadOrbit2);
	pDC->SelectObject(pOldPen2);
	CRgn elps;
	elps.CreateEllipticRgn(winCenterX - RadSun, winCenterY - RadSun,winCenterX + RadSun,winCenterY + RadSun);
	CBrush brush,bplA,bplB;
	brush.CreateSolidBrush(RGB(50, 0, 50));
	pDC->FillRgn(&elps, &brush);
	CRgn plaA, plaB;
	bplA.CreateSolidBrush(RGB(70, 0, 250));
	bplB.CreateSolidBrush(RGB(100, 0, 50));
	pDC->FlattenPath();
	int nNumPts = pDC->GetPath(NULL, NULL, 0);
	if (nNumPts == 0)
		return;
	lpPoints = NULL;
	LPBYTE lpTypes = NULL;
	try
	{
		lpPoints = new POINT[nNumPts];
		lpTypes = new BYTE[nNumPts];
	}
	catch (CException* pe)
	{
		delete[] lpPoints;
		lpPoints = NULL;
		delete[] lpTypes;
		lpTypes = NULL;
		pe->Delete();
	}
	if (lpPoints == NULL || lpTypes == NULL)
		return;

	// Now that we have the memory, really get the path data.
	pDC->FlattenPath();
	nNumPts = pDC->GetPath(lpPoints, lpTypes, nNumPts);
	if (nNumPts != -1)
	{
		plaA.CreateEllipticRgn(lpPoints[i].x - 15, lpPoints[i].y - 10, lpPoints[i].x + 10, lpPoints[i].y + 10);
		plaB.CreateEllipticRgn(lpPoints[j].x - 10, lpPoints[j].y - 10, lpPoints[j].x + 10, lpPoints[j].y + 10);
	}
		pDC->FillRgn(&plaA, &bplA);
		pDC->FillRgn(&plaB, &bplB);
	delete[] lpPoints;
	delete[] lpTypes;
}	


// CPlanetryMotionView printing

BOOL CPlanetryMotionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPlanetryMotionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPlanetryMotionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CPlanetryMotionView diagnostics

#ifdef _DEBUG
void CPlanetryMotionView::AssertValid() const
{
	CView::AssertValid();
}

void CPlanetryMotionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlanetryMotionDoc* CPlanetryMotionView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlanetryMotionDoc)));
	return (CPlanetryMotionDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlanetryMotionView message handlers


unsigned int CPlanetryMotionView::StartThread(LPVOID pmtr)
{
	CPlanetryMotionView* pView = (CPlanetryMotionView*)pmtr;
	while (1)
	{
		pView->i++;
		if (pView->i >= 65)
		{
			pView->i = 0;
		}
		pView->Invalidate();
		Sleep(100);

	}
	return 0;
}


unsigned int CPlanetryMotionView::AnotherThread(LPVOID parm)
{
	CPlanetryMotionView* pView = (CPlanetryMotionView*)parm;
	while (1)
	{
		pView->j++;
		if (pView->j >= 130)
		{
			pView->j = 65;
		}
		pView->Invalidate();
		Sleep(75);

	}
	return 0;
}


void CPlanetryMotionView::OnAnimationStart()
{
	newThread = AfxBeginThread(CPlanetryMotionView::StartThread,this);
	secThread = AfxBeginThread(CPlanetryMotionView::AnotherThread, this);
}


void CPlanetryMotionView::OnAnimationStop()
{
	newThread->SuspendThread();
	secThread->SuspendThread();
}


void CPlanetryMotionView::OnAnimationResume()
{
	newThread->ResumeThread();
	secThread->ResumeThread();
}

