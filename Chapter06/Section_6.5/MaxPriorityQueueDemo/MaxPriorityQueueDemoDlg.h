
// MaxPriorityQueueDemoDlg.h: 头文件
//

#pragma once

#include "MaxPriorityQueue.h"
#include "BinaryHeapView.h"


// CMaxPriorityQueueDemoDlg 对话框
class CMaxPriorityQueueDemoDlg : public CDialogEx
{
// 构造
public:
	CMaxPriorityQueueDemoDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAXPRIORITYQUEUEDEMO_DIALOG };
#endif

	const MaxPriorityQueue<int>& GetMaxPriorityQueue() const { return m_MaxPriorityQueue; }

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;
	CBinaryHeapView m_MaxPriorityQueueFigure;
	MaxPriorityQueue<int> m_MaxPriorityQueue;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInsert();
	afx_msg void OnExtractMax();
	afx_msg void OnIncreaseKey();
	CString m_InsertElements;
	int m_IncreaseElemIndex;
	int m_IncreaseToElem;
	afx_msg void OnClear();
};
