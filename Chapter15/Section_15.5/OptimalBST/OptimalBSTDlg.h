
// OptimalBSTDlg.h: 头文件
//

#pragma once

#include <string>
#include "OptimalBinarySearchTree.h"
#include "OptimalBSTView.h"
#include "afxcmn.h"
#include "afxwin.h"

using namespace std;


// COptimalBSTDlg 对话框
class COptimalBSTDlg : public CDialog
{
// 构造
public:
	COptimalBSTDlg(CWnd* pParent = NULL);	// 标准构造函数

	const OptimalBinarySearchTree& GetOptimalBST() const { return m_OptimalBST; }

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPTIMALBST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	OptimalBinarySearchTree m_OptimalBST;
	COptimalBSTView m_OptimalBSTFigure;
	CListCtrl m_ProbabilityList;
	CString m_Probability;
	int m_EditItem;
	int m_EditSubitem;

	template <typename T> void InitProbabilityList(T keyPR[], T dummyPR[], int n);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetOptimalBST();
	afx_msg void OnClear();
	afx_msg void OnDblclkProbabilityList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKillfocusProbabilityEdit();
	virtual void OnOK();
	afx_msg void OnReturnProbabilityList(NMHDR *pNMHDR, LRESULT *pResult);
};
