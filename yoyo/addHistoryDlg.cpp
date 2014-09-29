#include "stdafx.h"
#include "stdio.h"
#include "yoyo.h"
#include "common.h"

WNDPROC g_Edit;

LRESULT CALLBACK NewEditProc (HWND hwnd, UINT message,WPARAM wParam, LPARAM lParam)
{
	//TCHAR chCharCode;
	TCHAR   szString[2];
	switch (message)
	{
		case WM_CHAR:
			{
				//wParam=toupper(wParam);
				if((wParam < 58) && (wParam > 47))
					SetFocus(GetDlgItem(GetParent(hwnd), IDC_EDIT2));
			}
		break;
	}
	return CallWindowProc (g_Edit, hwnd, message, wParam, lParam);
}

LRESULT CALLBACK New2EditProc (HWND hwnd, UINT message,WPARAM wParam, LPARAM lParam)
{
	TCHAR   szString[2];
	switch (message)
	{
		case WM_CHAR:
			{
				if((wParam < 58) && (wParam > 47))
					SetFocus(GetDlgItem(GetParent(hwnd), IDC_EDIT3));
			}
		break;
	}
	return CallWindowProc (g_Edit, hwnd, message, wParam, lParam);
}

static int CaculateWeight(int *parray)
{
	int lotteries[LOTTERIES_ROW][3] = {0};
	int j = NUMBER_TOTAL, total = 0, pEnable[MAX-MIN+1] = {0}, pNumber[NUMBER_TOTAL] = {0};

	for(int i = 0;i<(MAX-MIN+1);i++){
		pEnable[i] = 1;
    }

	for(int i = 0;i<NUMBER_TOTAL;i++){
		pNumber[i] = 1;
    }	

	int n = getCombos();

	//YOYO data
	ULONG *numbers = (ULONG *)malloc(sizeof(ULONG)*NUMBER_TOTAL);
	for(int i = 0;i<NUMBER_TOTAL;i++){
		numbers[i] = 0;
	}

	COMBO_YOYO *combos = initCombo();

	//for(int i = 0;i<330;i++)
	{
			//combos[i].weight = pWeight[i-120];
			//printf("%d weight:%lf\n", 308, combos[307].weight);
		
    }
	
	//int parray[10][3] = {{0,5,8},{0,5,8},{0,5,8},{0,5,8},{0,5,8},{0,5,8},{0,5,8},{0,5,8},{0,5,8},{0,5,8}};
	
	staCombos(parray, pEnable, pNumber, combos, n, numbers);

	for(int i = 0; i < n;i++)
	{
		free(combos[i].combo_array);
	}
	free(combos);
	free(numbers);
	return 1;
}


int SaveEdittoFile(HWND hDlg)
{
	TCHAR   szString[2];
	int parray[3];
	GetDlgItemText(hDlg, IDC_EDIT1, szString, 2);
	parray[0] = _wtoi(szString);
	if(parray[0] >= NUMBER_TOTAL) return 0;
	GetDlgItemText(hDlg, IDC_EDIT2, szString, 2);
	parray[1] = _wtoi(szString);
	if(parray[1] >= NUMBER_TOTAL) return 0;
	GetDlgItemText(hDlg, IDC_EDIT3, szString, 2);
	parray[2] = _wtoi(szString);
	if(parray[2] >= NUMBER_TOTAL) return 0;
	/*char   *pFilePathName;  
        TCHAR   lpszFile[256];  
      _tcscpy(lpszFile,   _T("c://aa.dll"));  
   
      int   nLen   =   wcslen(wstr)+1;    
      WideCharToMultiByte(CP_ACP,   0,   lpszFile,   nLen,   pFilePathName,  
2*nLen,   NULL,   NULL);*/
	CaculateWeight(parray);
	return saveLottery(parray);
}

INT_PTR CALLBACK AddHisDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			{
				HWND hWndEdit = GetDlgItem(hDlg, IDC_EDIT1);
				SendMessage(hWndEdit, EM_LIMITTEXT, (WPARAM)1, 0);
				SetFocus(hWndEdit);
				//Subclass the Edit control
				g_Edit = (WNDPROC)SetWindowLong(hWndEdit, GWL_WNDPROC, (LONG)NewEditProc);
				hWndEdit = GetDlgItem(hDlg, IDC_EDIT2);
				SendMessage(hWndEdit, EM_LIMITTEXT, (WPARAM)1, 0);
				//Subclass the Edit control
				g_Edit = (WNDPROC)SetWindowLong(hWndEdit, GWL_WNDPROC, (LONG)New2EditProc);
				hWndEdit = GetDlgItem(hDlg, IDC_EDIT3);
				SendMessage(hWndEdit, EM_LIMITTEXT, (WPARAM)1, 0);
			}
			break;
			//return (INT_PTR)TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK:
					SaveEdittoFile(hDlg);
					EndDialog(hDlg, LOWORD(wParam));
					return (INT_PTR)TRUE;
				case IDCANCEL:
					EndDialog(hDlg, LOWORD(wParam));
					return (INT_PTR)TRUE;
				//case IDC_NUM1:
				//case IDC_NUM2:
				//	keybd_event(VK_NUMPAD1, 0, 0, 0);
				//	keybd_event(VK_NUMPAD1, 0, KEYEVENTF_KEYUP, 0);
				//	//PostMessage(hDlg,WM_KEYDOWN,'A',0);
				//	//if(GetFocus()==GetDlgItem(hDlg, IDC_EDIT1))
				//	{
				//	//	SetDlgItemText(hDlg,IDC_EDIT1,L"1");
				//	}
				//	break;
			}
			
			break;
		
	}

	return FALSE;
}