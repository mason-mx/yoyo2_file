// yoyo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "stdio.h"
#include "yoyo.h"
#include "common.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
int selectedLottery = 0;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	SelDialogProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
/*	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_YOYO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_YOYO));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
	*/
	HWND hDlg;
	MSG msg;
	BOOL ret;

	hDlg = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_SELDLG), 0, SelDialogProc, 0);
	ShowWindow(hDlg, nCmdShow);

	while((ret = GetMessage(&msg, 0, 0, 0)) != 0) {
		if(ret == -1)
		return -1;

		if(!IsDialogMessage(hDlg, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

  return 0;
}

int main()
{
	//int n;
	//n = initCombo();
	//int parray[10][3] = {{0,5,8},{0,5,8},{0,5,8},{0,5,8},{0,5,8},{0,5,8},{0,5,8},{0,5,8},{0,5,8},{0,5,8}};
	//for(int i = 0; i < 10;i++)
	//{
	//	staCombos(parray[i], combos, n);
	//}
	/*int rel = parseLottery(lotteries);
	for(int i=0; i<rel; i++)
	 {
		 for(int j=0; j<3; j++)
			printf("%d ", lotteries[i][j]);
		 printf("\n");
	 }
	printf("%d history", rel);
	int parray[3] = {0,5,8};
	saveLottery(parray);*/
	return 0;
    //return _tWinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOW);
}