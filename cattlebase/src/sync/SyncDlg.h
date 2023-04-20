#ifndef __SYNCDLG_H__
#define __SYNCDLG_H__

#include "SyncSocket.h"
#include "../ctrls/ConEdit/ConEdit.h"
#include "SyncThread.h"
#include "imgbutton\ImageButton.h"
#include "global_arrays.h"
#include "../ctrls/XColorStatic.h"

// CSyncDlg dialog
class CSyncDlg : public CDialog,public CSyncSocket::ISocketCallback
{
	DECLARE_DYNAMIC(CSyncDlg)

public:
	CSyncDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSyncDlg();

// Dialog Data
	enum { IDD = IDD_SYNCDLG };
private:

	CSyncSocket m_syncSocket;
	CSyncSocket m_syncClient;
	CSyncTransceiver m_syncTrans;
	CSyncThread m_syncThread;
	CConEdit m_syncLog;
	CProgressCtrl m_syncProgress;
	
	
	bool m_bClientConected;


	CSyncDocDataArray m_newInDocArray;
	CSyncDocDataArray m_updInDocArray;
    CSyncDocDataArray m_newOutDocArray;
	CSyncCowDataArray m_newCowArray;
	CSyncCowDataArray m_updCowArray;


	CImageButton  m_SendDataBtn;
	CImageButton  m_GetDataBtn;
	CXColorStatic m_hostNameLabel;

protected:
	void OnOK();
	void EnableButtons(BOOL bEnable);
	void SetStatus(UINT nStatusId);
	
	virtual void OnAccept();
	virtual void OnDisconnect();
	virtual void OnConnect();
	virtual void OnOutOfBandData();
	virtual void OnReceive();
	virtual void OnSend();

	bool UpdateCows(CSyncCowDataArray &cows2update);
	bool UpdateInDocs(CSyncDocDataArray & indocs2update);
	bool InsertOutDocs(CSyncDocDataArray & docs2insert);
	bool InsertInDocs(CSyncDocDataArray & docs2insert);
	bool InsertLonlyCows(CSyncCowDataArray & cows2insert);

	void PrintErrors(CPtrArray& syncRecords);
	void SyncCleanup(bool bCleanupAfterError);
	void RollbackCowSyncChanges(CSyncCowDataArray& syncCows);
	void CommitCowSyncChanges(CSyncCowDataArray& syncCows);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnDestroy();
	LRESULT OnSyncStatus(WPARAM wParam,LPARAM lParam);
	
	afx_msg void OnGetData();
	afx_msg void OnSendData();
	
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()
};
#endif