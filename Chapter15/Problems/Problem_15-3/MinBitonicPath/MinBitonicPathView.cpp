// MinBitonicPathView.cpp: 实现文件
//

#include "stdafx.h"
#include <string>
#include <sstream>
#include "MinBitonicPath.h"
#include "MinBitonicPathView.h"
#include "MinBitonicPathDlg.h"

using namespace std;


// CMinBitonicPathView

IMPLEMENT_DYNAMIC(CMinBitonicPathView, CWnd)

CMinBitonicPathView::CMinBitonicPathView()
{

}

CMinBitonicPathView::~CMinBitonicPathView()
{
}


BEGIN_MESSAGE_MAP(CMinBitonicPathView, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()


static int GetRange(int min, int max)
{
	if (min >= 0) return max;
	else if (max <= 0) return -min;
	else return (max-min);
}

static double GetUnit(CRect &paintRect, int xRange, int yRange)
{
	double xUnit = (xRange > 0) ? (double)paintRect.Width() / xRange : (double)paintRect.Width() / 5;
	double yUnit = (yRange > 0) ? (double)paintRect.Height() / yRange : (double)paintRect.Height() / 5;

	if (xUnit > yUnit) xUnit = yUnit;
	if (xUnit > 32) xUnit = 32;

	return xUnit;
}

static CPoint GetBasePoint(CRect &paintRect, int xMin, int xRange, int yMax, int yRange, double unit)
{
	CPoint basePoint;

	if (xRange == 0)
	{
		basePoint.x = paintRect.CenterPoint().x;
	}
	else
	{
		double xStart = paintRect.left + (paintRect.Width()-unit*xRange)/2;

		if (xMin < 0) basePoint.x = static_cast<int>(xStart - xMin*unit);
		else basePoint.x = static_cast<int>(xStart);
	}

	if (yRange == 0)
	{
		basePoint.y = paintRect.CenterPoint().y;
	}
	else
	{
		double yStart = paintRect.top + (paintRect.Height()-unit*yRange)/2;

		if (yMax > 0) basePoint.y = static_cast<int>(yStart + unit*yMax);
		else basePoint.y = static_cast<int>(yStart);
	}

	return basePoint;
}

static int GetPaintUnit(double unit)
{
	static const int paintUnitSet[] = { 1, 2, 5 };
	int paintUnit = paintUnitSet[0];
	double paintUnitLen = paintUnit*unit;

	for (int i = 1; i <= INT_MAX/paintUnitSet[sizeof(paintUnitSet)/sizeof(int)-1] && paintUnitLen < 20; i *= 10)
	{
		for (int j = 0; j < sizeof(paintUnitSet) / sizeof(int) && paintUnitLen < 20; ++j)
		{
			paintUnit = paintUnitSet[j] * i;
			paintUnitLen = paintUnit * unit;
		}
	}

	return paintUnit;
}

static void PaintBaseLine(CDC *pDC, CRect &wndRect, CPoint &basePoint, double unit, int paintUnit)
{
	static const int fontHeight = 12;

	pDC->MoveTo(wndRect.left, basePoint.y);
	pDC->LineTo(wndRect.right, basePoint.y);
	pDC->LineTo(wndRect.right-8, basePoint.y+3);
	pDC->MoveTo(wndRect.right, basePoint.y);
	pDC->LineTo(wndRect.right-8, basePoint.y-3);

	pDC->MoveTo(basePoint.x, wndRect.bottom);
	pDC->LineTo(basePoint.x, wndRect.top);
	pDC->LineTo(basePoint.x-3, wndRect.top+8);
	pDC->MoveTo(basePoint.x, wndRect.top);
	pDC->LineTo(basePoint.x+3, wndRect.top+8);

	CFont font;
	font.CreateFont(fontHeight, 0, 000, 000, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS,
					CLIP_TT_ALWAYS, PROOF_QUALITY, VARIABLE_PITCH|FF_ROMAN, CString("Times New Roman"));
	CFont *pOldFont = pDC->SelectObject(&font);
	int oldTextAlign = pDC->SetTextAlign(TA_TOP | TA_CENTER);

	pDC->SetTextAlign(TA_TOP|TA_RIGHT);
	pDC->TextOut(basePoint.x-2, basePoint.y+2, _T("0"));

	pDC->SetTextAlign(TA_TOP|TA_CENTER);

	int coordinateX = static_cast<int>(basePoint.x + paintUnit*unit);
	for (int i = 1; coordinateX <= wndRect.right-10; ++i)
	{
		pDC->MoveTo(coordinateX, basePoint.y);
		pDC->LineTo(coordinateX, basePoint.y-4);
		ostringstream is;
		is << i*paintUnit;
		pDC->TextOut(coordinateX, basePoint.y+2, CString(string(is.str()).c_str()));
		coordinateX = static_cast<int>(basePoint.x + (i+1)*paintUnit*unit);
	}

	coordinateX = static_cast<int>(basePoint.x - paintUnit*unit);
	for (int i = -1; coordinateX >= wndRect.left+10; --i)
	{
		pDC->MoveTo(coordinateX, basePoint.y);
		pDC->LineTo(coordinateX, basePoint.y-4);
		ostringstream is;
		is << i*paintUnit;
		pDC->TextOut(coordinateX, basePoint.y+2, CString(string(is.str()).c_str()));
		coordinateX = static_cast<int>(basePoint.x + (i-1)*paintUnit*unit);
	}

	pDC->SetTextAlign(TA_BOTTOM | TA_RIGHT);

	int coordinateY = static_cast<int>(basePoint.y - paintUnit*unit);
	for (int i = 1; coordinateY >= wndRect.top+10; ++i)
	{
		pDC->MoveTo(basePoint.x, coordinateY);
		pDC->LineTo(basePoint.x+4, coordinateY);
		ostringstream is;
		is << i*paintUnit;
		pDC->TextOut(basePoint.x-3, coordinateY + fontHeight/2, CString(string(is.str()).c_str()));
		coordinateY = static_cast<int>(basePoint.y - (i+1)*paintUnit*unit);
	}

	coordinateY = static_cast<int>(basePoint.y + paintUnit*unit);
	for (int i = -1; coordinateY <= wndRect.bottom-10; --i)
	{
		pDC->MoveTo(basePoint.x, coordinateY);
		pDC->LineTo(basePoint.x+4, coordinateY);
		ostringstream is;
		is << i*paintUnit;
		pDC->TextOut(basePoint.x-3, coordinateY + fontHeight/2, CString(string(is.str()).c_str()));
		coordinateY = static_cast<int>(basePoint.y - (i-1)*paintUnit*unit);
	}

	pDC->SelectObject(pOldFont);
	pDC->SetTextAlign(oldTextAlign);
}

static void PaintPoint(CDC *pDC, CPoint &basePoint, double unit, const CPoint sortedPoint[], const int pointChain[], int n)
{
	CPen pen(PS_SOLID, 1, RGB(0, 0, 255));
	CBrush brush(RGB(0, 0, 255));
	CPen *pOldPen = pDC->SelectObject(&pen);
	CBrush *pOldBrush = pDC->SelectObject(&brush);

	int pointPaintX = static_cast<int>(basePoint.x + sortedPoint[n-1].x*unit);
	int pointPaintY = static_cast<int>(basePoint.y - sortedPoint[n-1].y*unit);
	pDC->Ellipse(pointPaintX-4, pointPaintY-4, pointPaintX+3, pointPaintY+3);
	pDC->MoveTo(pointPaintX, pointPaintY);

	int endPointPaintX = pointPaintX;
	int endPointPaintY = pointPaintY;

	for (int i = 0; i < n - 1; ++i)
	{
		pointPaintX = static_cast<int>(basePoint.x + sortedPoint[pointChain[i]].x*unit);
		pointPaintY = static_cast<int>(basePoint.y - sortedPoint[pointChain[i]].y*unit);
		pDC->Ellipse(pointPaintX-4, pointPaintY-4, pointPaintX+3, pointPaintY+3);
		pDC->LineTo(pointPaintX, pointPaintY);
	}

	pDC->LineTo(endPointPaintX, endPointPaintY);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

static void PaintPathLength(CDC *pDC, CRect &wndRect, double pathLength)
{
	CFont font;
	font.CreateFont(16, 0, 000, 000, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS,
					CLIP_TT_ALWAYS, PROOF_QUALITY, VARIABLE_PITCH|FF_ROMAN, CString("Times New Roman"));
	CFont *pOldFont = pDC->SelectObject(&font);
	int oldTextAlign = pDC->SetTextAlign(TA_TOP|TA_RIGHT);

	ostringstream is;
	is << "Min bitonic path length = " << pathLength;
	pDC->TextOut(wndRect.right-16, wndRect.top+16, CString(string(is.str()).c_str()));

	pDC->SetTextAlign(oldTextAlign);
	pDC->SelectObject(pOldFont);
}

void CMinBitonicPathView::Paint(const CPoint sortedPoint[], const int pointChain[], int n)
{
	CRect wndRect;
	GetClientRect(wndRect);

	CClientDC dc(this);
	dc.SetBkMode(TRANSPARENT);

	CRgn rgn;
	rgn.CreateRectRgn(wndRect.left, wndRect.top, wndRect.right, wndRect.bottom);
	dc.SelectClipRgn(&rgn, RGN_COPY);
	dc.FillSolidRect(wndRect, RGB(255, 255, 255));

	if (wndRect.Width() <= 80 || wndRect.Height() <= 80)
		return;

	if (n < 1 || sortedPoint == NULL)
		return;

	if (n > 1 && pointChain == NULL)
		return;

	int xMin = sortedPoint[0].x;
	int xMax = sortedPoint[n-1].x;
	int yMin = sortedPoint[0].y;
	int yMax = sortedPoint[0].y;

	for (int i = 1; i < n; ++i)
	{
		if (sortedPoint[i].y < yMin) yMin = sortedPoint[i].y;
		else if (sortedPoint[i].y > yMax) yMax = sortedPoint[i].y;
	}

	CRect paintRect = wndRect;
	paintRect.top    += 40;
	paintRect.left   += 40;
	paintRect.bottom -= 40;
	paintRect.right  -= 40;

	int xRange = GetRange(xMin, xMax);
	int yRange = GetRange(yMin, yMax);
	double unit = GetUnit(paintRect, xRange, yRange);
	CPoint basePoint = GetBasePoint(paintRect, xMin, xRange, yMax, yRange, unit);
	int paintUnit = GetPaintUnit(unit);

	PaintBaseLine(&dc, wndRect, basePoint, unit, paintUnit);
	PaintPoint(&dc, basePoint, unit, sortedPoint, pointChain, n);
	PaintPathLength(&dc, wndRect, ((CMinBitonicPathDlg*)GetParent())->GetPathLength());
}


// CMinBitonicPathView 消息处理程序

BOOL CMinBitonicPathView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK
		|| pMsg->message == WM_RBUTTONDOWN || pMsg->message == WM_RBUTTONDBLCLK)
	{
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CMinBitonicPathView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CWnd::OnPaint()

	Paint(((CMinBitonicPathDlg*)GetParent())->GetPoint(),
		((CMinBitonicPathDlg*)GetParent())->GetPointChain(),
		((CMinBitonicPathDlg*)GetParent())->GetPointNum());
}
