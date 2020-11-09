
// PlanetryMotionView.h : interface of the CPlanetryMotionView class
//

#pragma once


class CPlanetryMotionView : public CView
{
	CWinThread* newThread;
	CWinThread* secThread;
protected: // create from serialization only
	CPlanetryMotionView() noexcept;
	DECLARE_DYNCREATE(CPlanetryMotionView)

// Attributes
public:
	CPlanetryMotionDoc* GetDocument() const;
	double winCenterX, winCenterY;
// Operations
public:
	int RadOrbit1;
	int RadOrbit2;
	int RadSun;
	int i,j,dx,ecc;
	LPPOINT lpPoints;
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CPlanetryMotionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	static unsigned int StartThread(LPVOID pmtr);
	afx_msg void OnAnimationStart();
	afx_msg void OnAnimationStop();
	afx_msg void OnAnimationResume();
	static unsigned int AnotherThread(LPVOID parm);
};

#ifndef _DEBUG  // debug version in PlanetryMotionView.cpp
inline CPlanetryMotionDoc* CPlanetryMotionView::GetDocument() const
   { return reinterpret_cast<CPlanetryMotionDoc*>(m_pDocument); }
#endif

