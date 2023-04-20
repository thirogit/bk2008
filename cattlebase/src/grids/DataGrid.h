#ifndef __DATAGRID_H__
#define __DATAGRID_H__

#include "BaseDataGrid.h"
#include "../classes/forallactions/UnmarkAllAction.h"
#include "../printdocs/printdocs.h"
#include "../dlgs/propdlg.h"

#include < Prof-UIS.h >


template<class ARRAY,class DATA>
class DataGrid : public BaseDataGrid
{
public:
	DataGrid(ARRAY &dataArray,const GRIDHEADERS *ppHeaders, int pConfigItemId) :
		BaseDataGrid(ppHeaders,pConfigItemId),m_dataArray(dataArray)
	{
	};
	
	virtual int GetSelectedRows(CPtrArray &selDataArray)
	{
		CListCtrl &m_list = GetListCtrl();
		UINT i, uSelectedCount = m_list.GetSelectedCount();
		int  nItem = -1;
		if (uSelectedCount > 0)
		{
			for (i=0;i < uSelectedCount;i++)
			{
				nItem = m_list.GetNextItem(nItem, LVNI_SELECTED);
				ASSERT(nItem != -1);
				selDataArray.Add(GetItemPtr(nItem)); 
			}
		}
		return uSelectedCount;
	}

	virtual void UpdateItemCount()
	{
		GetListCtrl().SetItemCount(m_dataArray.GetSize());
	}


	virtual void MarkRow(int nRow)
	{
		if(nRow >= 0 && nRow < m_dataArray.GetSize())
			m_dataArray[nRow]->bMark = true;	
	}

	virtual bool IsItemMarked(int iItem)
	{
		return m_dataArray[iItem]->bMark;
	}

	
	virtual void SelectMarked()
	{
		CListCtrl &m_list = GetListCtrl();
		for(int i = 0;i < m_dataArray.GetSize();i++)
		{
			if(m_dataArray[i]->bMark) 
				m_list.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
			else m_list.SetItemState(i,0,LVIS_SELECTED);
		}
	}

	virtual RawData *GetItemPtr(int iItem)
	{
		return m_dataArray[iItem];
	}

	virtual bool GetDataCellColor(int iItem,int nCol,COLORREF &cr) { return false; };
	
	virtual void GridSortBy(DataMemberSortParamsArray &sortList)
	{
		m_dataArray.SortBy(sortList);
	}

	virtual UINT GetGridDataTypeId() = 0;

	virtual void AddPopMenuItem(CPopMenuNodeList &popmenulist)  
	{
	}

	virtual void OnRightClick(int nItem,int nSubItem)
	{
		CMenu _menu;
		if( !_menu.LoadMenu(IDR_GRIDPOPMENU) )
		{
			ASSERT( FALSE );
			return;
		}
		
		CWinApp * pApp = ::AfxGetApp();
		HWND mainWnd = pApp->m_pMainWnd->GetSafeHwnd();
		CExtPopupMenuWnd * pPopup = new CExtPopupMenuWnd;
		pPopup->SetLeftAreaWidth(20);
		pPopup->UpdateFromMenu(	mainWnd,&_menu);

			
		INT nReplacePos =	pPopup->ItemFindPosForCmdID(IDM_GRID_PRINT);
		if( nReplacePos >= 0 )
		{
			CString replaceItemText = pPopup->ItemGetText(nReplacePos);

			VERIFY( pPopup->ItemRemove(nReplacePos) );

			CMenu buildMenu,printMenu;
			VERIFY( buildMenu.CreatePopupMenu() );
			ASSERT( buildMenu.GetSafeHmenu() != NULL );		
							
			UINT nCmdID = ID_PRINTDOC_FIRST;
			
			CExtCmdItem * pCmdItem = g_CmdManager->CmdGetPtr(pApp->m_pszProfileName ,nCmdID	);
			if( pCmdItem == NULL ) 	pCmdItem = g_CmdManager->CmdAllocPtr( pApp->m_pszProfileName, nCmdID );			
			ASSERT( pCmdItem != NULL );

			printMenu.CreatePopupMenu();

			UINT dataType =  GetGridDataTypeId();
			bool noPrintDocs = true;
			
			CString popupMenuText;
			if(dataType)
			{
				
				int itemsAdded = 0;
				const PRINTDOCS *pDoc = g_PrintDocs;
				CString docName;

				while(pDoc->nID != NULL_DOC_ID)
				{
					if(dataType & pDoc->iDataType)
					{
						docName.LoadString(pDoc->nResDocName);
						if(printMenu.InsertMenu(0,MF_BYPOSITION,pDoc->nID,(LPCSTR)docName))
							itemsAdded++;							
					}
					pDoc++;
				}
				if(itemsAdded) noPrintDocs = false;
			}
				
			UINT mf_flag;
			UINT nID;

			if(noPrintDocs)
			{
				mf_flag = MF_DISABLED;
				nID = ID_PRINTDOC_FIRST;
				popupMenuText.LoadString(IDS_NOPRINTDOCS);
			}
			else
			{
				mf_flag = MF_POPUP;
				nID = (UINT)printMenu.GetSafeHmenu();

			}

			buildMenu.InsertMenu(0, MF_BYPOSITION | mf_flag,nID,popupMenuText);

			VERIFY(	pPopup->ItemInsert(	(UINT)CExtPopupMenuWnd::TYPE_POPUP,nReplacePos,(LPCTSTR)replaceItemText));
			pPopup->ItemSetDisplayed( nReplacePos, true );
			
			CExtPopupMenuWnd * pPrintPopup = pPopup->ItemGetPopup(nReplacePos);

			ASSERT( pPrintPopup != NULL );
			if( pPrintPopup != NULL )
				VERIFY(pPrintPopup->UpdateFromMenu(mainWnd,	&buildMenu,	!noPrintDocs,false));
				
			} // if( nReplacePos >= 0 )
			
			CPopMenuNodeList popmenulist;
			AddPopMenuItem(popmenulist);
			
			CPopMenuNode *popItem = NULL;

			UINT nCmdID;
			CExtCmdItem * pCmdItem;
			CString sMenuText;	

			for(int item = 0;item < popmenulist.GetSize();item++)
			{
				popItem = popmenulist[item];
				if(popItem->IsSpearator())
					pPopup->ItemInsert();
				else
				{
					if(nCmdID = popItem->getID())
					{
						pCmdItem = g_CmdManager->CmdGetPtr(pApp->m_pszProfileName ,nCmdID);
						if( pCmdItem == NULL )
							pCmdItem = g_CmdManager->CmdAllocPtr(pApp->m_pszProfileName,nCmdID);

						ASSERT( pCmdItem != NULL );
						if( pCmdItem != NULL )
						{
							sMenuText.LoadString(popItem->getResMenuText());
							pCmdItem->m_sMenuText = sMenuText;
							pCmdItem->StateSetBasic( true );			
							pPopup->ItemInsert(nCmdID,-1,sMenuText);
						}
					}
					else
					{

						CMenu subMenu;
						subMenu.LoadMenu(popItem->getResSubMenu());

						UINT newPos = pPopup->ItemGetCount();
						sMenuText.LoadString(popItem->getResMenuText());
						VERIFY(	pPopup->ItemInsert(	(UINT)CExtPopupMenuWnd::TYPE_POPUP,-1,(LPCTSTR)sMenuText));
						pPopup->ItemSetDisplayed(newPos , true );
				
						CExtPopupMenuWnd * pPrintPopup = pPopup->ItemGetPopup(newPos);
						ASSERT( pPrintPopup != NULL );
						if( pPrintPopup != NULL )
							VERIFY(pPrintPopup->UpdateFromMenu(mainWnd,	&subMenu,	true,false));

					}

				}
			}

			popmenulist.FreeAndFlush();
			CPoint point;
			GetCursorPos(&point);
			

			if( !pPopup->TrackPopupMenu(
				TPMX_OWNERDRAW_FIXED,
				point.x,
				point.y
				)
				)

				delete pPopup;
		
	}

	DATA* GetRowData(int nRow)
	{
		return m_dataArray[nRow];
	}

	DATA* GetSelectedRow()
	{
		int nSelItem = GetCurSel();
		if(nSelItem >= 0)
			return GetRowData(nSelItem);

		return NULL;
	}

	virtual void OnDoubleClick(int iItem,int iSubItem) 
	{
		UINT message = IDM_GRID_EDITROW;
		if(GetAsyncKeyState(VK_SHIFT) & 0x8000) message = IDM_GRID_ROWPROPERTIES;
		AfxGetApp()->m_pMainWnd->PostMessage(WM_COMMAND,message);
	
	}

	virtual void PostInit()
	{
		UpdateItemCount();
	}

	virtual void UnMarkAllRows()
	{
		m_dataArray.ForAllAction(CUnmarkAllAction<ARRAY::ArrayAction,DATA>());
	}

	virtual bool HandleMessage(UINT message)
	{

		switch(message)
		{
			case IDM_GRID_SORTMARKED:
				m_dataArray.SortBy(RAWDATA_MARK,FALSE);
				break;
			case IDM_GRID_SELECTALL:
				SelectAll();
				break;			
			case IDM_GRID_INVERTSELECTION:
				InvertSelection();
				break;;
			case IDM_GRID_UNMARK:
			{
				ARRAY selectedRowsData;
				GetSelectedRows(selectedRowsData);
				selectedRowsData.ForAllAction(CUnmarkAllAction<ARRAY::ArrayAction,DATA>());		
				break;
			}			
			case IDM_GRID_SELECTMARKED:
				SelectMarked();
				break;
			case IDM_GRID_ROWPROPERTIES:
			{
				CPropDlg propDlg(GetItemPtr(GetCurSel()));
				propDlg.DoModal();				
				break;		
			}
			default:
				return false;
		}

		Invalidate(FALSE);
		return true;
	}
	virtual bool HandleUpdateMessage(CCmdUI *pCmdUI)
	{
		return false;
	}


protected:
	ARRAY &m_dataArray;
};

#endif