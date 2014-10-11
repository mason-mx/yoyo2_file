#include "stdafx.h"
#include "stdio.h"
#include "yoyo.h"
#include "common.h"
#include <crtdbg.h>

extern HINSTANCE hInst;
extern int selectedLottery;

INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT_PTR CALLBACK SelDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			{
				//RECT rectChild, rectParent;  
				//int DlgWidth, DlgHeight;    // 以像素为单位的对话框宽度和高度  
				//int NewPosX, NewPosY;  
	  
				//// 设法使“关于”对话框居中显示  
				//if (GetWindowRect(hDlg, &rectChild))   
				//{  
				//	GetClientRect(GetParent(hDlg), &rectParent);  
				//	DlgWidth    = rectChild.right - rectChild.left;  
				//	DlgHeight   = rectChild.bottom - rectChild.top ;  
				//	NewPosX     = (DlgWidth) / 2;  
				//	NewPosY     = (DlgHeight) / 2;  
	   //               
				//	// 如果“关于”框比实际屏幕大  
				//	if (NewPosX < 0) NewPosX = 0;  
				//	if (NewPosY < 0) NewPosY = 0;  
				//	SetWindowPos(hDlg, 0, NewPosX, NewPosY,  
				//		0, 0, SWP_NOZORDER | SWP_NOSIZE);  
				//}
			}
			return (INT_PTR)TRUE;
		case WM_COMMAND:
			/*if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			}
			break;*/
			switch(LOWORD(wParam))
			{
				case IDOK:
					{
					//HWND hListBox = GetDlgItem(hDlg, IDC_LIST1);
					//ShowResult(hListBox);
					SendMessage(hDlg, WM_CLOSE, 0, 0);
					}
					return TRUE;
				case IDC_YOYO1:
					selectedLottery = 50;
					///SendMessage(hDlg, WM_CLOSE, 0, 0);
					DialogBox(hInst, MAKEINTRESOURCE(IDD_MAINDLG), hDlg, MainDialogProc);					
					break;
				case IDC_YOYO2:
					selectedLottery = 51;
					///SendMessage(hDlg, WM_CLOSE, 0, 0);
					DialogBox(hInst, MAKEINTRESOURCE(IDD_MAINDLG), hDlg, MainDialogProc);					
					break;
			}
			break;

		case WM_CLOSE:
			//if(MessageBox(hDlg, TEXT("Close the program?"), TEXT("Close"),
			//MB_ICONQUESTION | MB_YESNO) == IDYES)
			{
				_CrtDumpMemoryLeaks();
				DestroyWindow(hDlg);
			}
			return TRUE;
		case WM_CHAR:
			MessageBox(NULL, NULL, NULL, 0);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			return TRUE;
	}

	return FALSE;
}