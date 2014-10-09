#include "stdafx.h"
#include "stdio.h"
#include "yoyo.h"
#include "common.h"
#include <crtdbg.h>
#include <commdlg.h>

extern HINSTANCE hInst;
extern int selectedLottery;
int combosFileImported;
int hisFileImported;
COMBO_YOYO *combosF;
int icombosF = 0;

INT_PTR CALLBACK AddHisDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);


static int ImportCombos(HWND hDlg)
{
	OPENFILENAMEA ofn = {0};
	char Buffer[300] = {0};
	int combosInBeforeImported = 0;

	HWND hListBox = GetDlgItem(hDlg, IDC_LIST1);	
	
	memset(Buffer, '\0', 300);
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = hDlg;
    ofn.lpstrFile = Buffer;
    ofn.nMaxFile = 300;
    ofn.Flags = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST|OFN_NOCHANGEDIR;
    ofn.lpstrFilter = "Text File\0*.txt\0";
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
    if(0 == GetOpenFileNameA(&ofn))
		return 0;

	icombosF = getCombos(Buffer);

	if(icombosF < 1) return 0;
	
	char hisf[MAX_PATH] = {0};
	if(getImportedCombosFile(hisf))
		combosInBeforeImported = getCombos(hisf);
	saveImportedCombosFile(Buffer);
	if(combosFileImported)
	{
		for(int i = 0; i < combosInBeforeImported;i++)
		{
			free(combosF[i].combo_array);
		}
		free(combosF);
	}
	else
	{
			combosFileImported = 1;
	}

	TCHAR combosn[MAX_PATH] = {0};
	swprintf(combosn,TEXT("已導入%d個4碼組合"),icombosF);
	SetWindowText(GetDlgItem(hDlg, IDC_IMPORTCFILE), combosn);
	
	//Reset all weight
	char filename[MAX_PATH];

	sprintf(filename, "%d_w.txt", selectedLottery);
	remove(filename);
	sprintf(filename, "%d_h.txt", selectedLottery);
	remove(filename);
	sprintf(filename, "%d_mnh.txt", selectedLottery);
	remove(filename);
	prepareWeight(icombosF);
	prepareHit(icombosF);
	prepareMaxNoHit();
	SetWindowText(GetDlgItem(hDlg, IDC_HISN), L"");
	SetWindowText(GetDlgItem(hDlg, IDC_ALERT), L"");

	SendMessage( hListBox, LB_RESETCONTENT, 0, 0);

	combosF = initCombo(icombosF, Buffer);	

	MessageBox(hDlg, TEXT("請重新導入歷史出獎號，或者手動添加！"), TEXT("警告"), MB_ICONWARNING | MB_OK);
	SetWindowText(GetDlgItem(hDlg, IDC_IMPORTHFILE), TEXT("無記錄"));

	return icombosF;
}

static int ImportHis(HWND hDlg)
{
	int lotteries[MAX_HISTORY_NUM][3] = {0};
	TCHAR pLotteries[MAX_HISTORY_NUM][HISTORY_PATH] = {0};
	int pEnable[MAX-MIN+1] = {0}, pNumber[NUMBER_TOTAL] = {0};
	
	OPENFILENAMEA ofn = {0};
	char Buffer[300] = {0};

	HWND hListBox = GetDlgItem(hDlg, IDC_LIST1);

	memset(Buffer, '\0', 300);
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = hDlg;
    ofn.lpstrFile = Buffer;
    ofn.nMaxFile = 300;
    ofn.Flags = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST|OFN_NOCHANGEDIR;
    ofn.lpstrFilter = "Text File\0*.txt\0";
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
	//ofn.lpstrDefExt = "txt";
    if(0 == GetOpenFileNameA(&ofn))
		return 0;
	
	saveImportedHisFile(Buffer);
	int rel = parseLottery(Buffer, lotteries, pLotteries);
	if(rel > 0)
	{
		SendMessage( hListBox, LB_RESETCONTENT, 0, 0);

		for (int i = 0 ; i < rel ; i++)
		{
			SendMessage( hListBox, LB_INSERTSTRING, i, (LPARAM)pLotteries[i]);
		}
	}
	else
	{
		return 0;
	}

	hisFileImported = 1;
	TCHAR hisn[MAX_PATH] = {0};
	swprintf(hisn,TEXT("已導入%d個歷史出獎號"),rel);
	SetWindowText(GetDlgItem(hDlg, IDC_IMPORTHFILE), hisn);
	for(int i = 0;i<(MAX-MIN+1);i++){
		pEnable[i] = 1;
    }

	for(int i = 0;i<NUMBER_TOTAL;i++){
		pNumber[i] = 1;
    }	
	//Reset all weight
	char filename[MAX_PATH];

	sprintf(filename, "%d_w.txt", selectedLottery);
	remove(filename);
	sprintf(filename, "%d_h.txt", selectedLottery);
	remove(filename);
	sprintf(filename, "%d_mnh.txt", selectedLottery);
	remove(filename);
	prepareWeight(icombosF);
	prepareHit(icombosF);
	prepareMaxNoHit();
	SetWindowText(GetDlgItem(hDlg, IDC_HISN), L"");
	SetWindowText(GetDlgItem(hDlg, IDC_ALERT), L"");

	for(int i = 0; i < icombosF;i++)
	{
		combosF[i].weight = 1.0;
		combosF[i].nohit = 0;
	}

	//YOYO data
	ULONG *numbers = (ULONG *)malloc(sizeof(ULONG)*NUMBER_TOTAL);
	for(int i = 0;i<NUMBER_TOTAL;i++){
		numbers[i] = 0;
	}

	for(int i = (rel - 1); i >= 0;i--)
	{
		for(int i = 0;i<NUMBER_TOTAL;i++){
			numbers[i] = 0;
		}
		staCombos(lotteries[i], pEnable, pNumber, combosF, icombosF, numbers);
		int noHit = staHit(combosF, icombosF);
		if(parseMaxNoHit() < noHit) saveMaxNoHit(noHit);
		if(ALERT_TH <= noHit)
		{	
			TCHAR combosn[MAX_PATH] = {0};
			swprintf(combosn,TEXT("警告！已有最大%d期不中的情況"),noHit);
			SetWindowText(GetDlgItem(hDlg, IDC_ALERT), combosn);
		}
	}
	free(numbers);

	return 1;
}

static int ShowResult(HWND hDlg)
{
	TCHAR pRel[NUMBER_TOTAL][RESULT_PATH] = {0};
	int lotteries[MAX_HISTORY_NUM][3] = {0};
	int j = NUMBER_TOTAL, total = 0, pEnable[MAX-MIN+1] = {0}, pNumber[NUMBER_TOTAL] = {0};

	HWND hListBox = GetDlgItem(hDlg, IDC_LIST2);

	SendMessage( hListBox, LB_RESETCONTENT, 0, 0);

	for(int i = 0;i<(MAX-MIN+1);i++){
		//HWND hCheck = GetDlgItem(hDlg, IDC_CHECK3 + MIN - 3 + i);
		//int nChecked = SendMessage(hCheck, BM_GETCHECK, 0, 0);
		//if(nChecked == BST_CHECKED)
		{
			pEnable[i] = 0;
			//total ++;
		}
		//printf("%d : %d\r\n",IDC_CHECK3 + i,pEnable[i]);
    }
	pEnable[1] = 1;
	total = 1;

	if(total == 0)
	{
		MessageBox(hDlg, TEXT("請輸入統計條件！"), TEXT("警告"), MB_ICONWARNING | MB_OK);
		return 0;
	}

	for(int i = 0;i<NUMBER_TOTAL;i++){
		HWND hCheck = GetDlgItem(hDlg, IDC_CHECK1 + i);
		int nChecked = SendMessage(hCheck, BM_GETCHECK, 0, 0);
		if(nChecked == BST_CHECKED)
		{
			pNumber[i] = 1;
		}
		//printf("%d : %d\r\n",IDC_CHECK3 + i,pEnable[i]);
    }	

	//int n = getCombos();

	//YOYO data
	ULONG *numbers = (ULONG *)malloc(sizeof(ULONG)*NUMBER_TOTAL);
	for(int i = 0;i<NUMBER_TOTAL;i++){
		numbers[i] = 0;
	}

	//COMBO_YOYO *combos = initCombo();

	staWeight( pEnable, pNumber, combosF, icombosF, numbers);


	while(j >= 0)
	{
		j --;
		if(pNumber[j] == 1)
		{
			wsprintf(pRel[j], L"%d : %u", j, numbers[j]);
			SendMessage( hListBox, LB_INSERTSTRING, 0, (LPARAM)pRel[j]);
		}
	}

	free(numbers);
	return 1;
}

static int ShowLotteryHistory(HWND hDlg)
{
	int lotteries[MAX_HISTORY_NUM][3] = {0};
	TCHAR pLotteries[MAX_HISTORY_NUM][HISTORY_PATH] = {0};
	int rel = 0;
	HWND hListBox = GetDlgItem(hDlg, IDC_LIST1);
	if(hisFileImported)
	{
		char hisf[MAX_PATH] = {0};
		if(getImportedHisFile(hisf))
		rel = parseLottery(hisf, lotteries, pLotteries);
	}
	else
	{
		rel = parseLottery(lotteries, pLotteries);
	}
	if(rel > 0)
	{
		TCHAR hisn[MAX_PATH] = {0};
		swprintf(hisn,TEXT("當前共%d期："),rel);
		SetWindowText(GetDlgItem(hDlg, IDC_HISN), hisn);
		SendMessage( hListBox, LB_RESETCONTENT, 0, 0);

		for (int i = 0 ; i < rel ; i++)
		{
			SendMessage( hListBox, LB_INSERTSTRING, i, (LPARAM)pLotteries[i]);
		}
	}
	return rel;
}

static void ClearLotteryHistory(HWND hDlg)
{
	char filename[MAX_PATH];
	HWND hListBox = GetDlgItem(hDlg, IDC_LIST1);
	sprintf(filename, "%d_w.txt", selectedLottery);
	remove(filename);
	sprintf(filename, "%d_h.txt", selectedLottery);
	remove(filename);
	sprintf(filename, "%d_mnh.txt", selectedLottery);
	remove(filename);
	sprintf(filename, "lotteries%d.txt", selectedLottery);
	remove(filename);
	/*if(hisFileImported)
	{
		char hisf[MAX_PATH] = {0};
		if(getImportedHisFile(hisf)) remove(hisf);
	}*/
	for(int i = 0; i < icombosF;i++)
	{
		combosF[i].weight = 1.0;
		combosF[i].nohit = 0;
	}
	hisFileImported = 0;
	prepareLotteries();
	remove("importh.txt");
	prepareWeight(icombosF);
	prepareHit(icombosF);
	prepareMaxNoHit();
	SetWindowText(GetDlgItem(hDlg, IDC_HISN), L"");
	SetWindowText(GetDlgItem(hDlg, IDC_ALERT), L"");
	SendMessage( hListBox, LB_RESETCONTENT, 0, 0);
	MessageBox(hDlg, TEXT("請重新導入歷史出獎號，或者手動添加！"), TEXT("警告"), MB_ICONWARNING | MB_OK);
	SetWindowText(GetDlgItem(hDlg, IDC_IMPORTHFILE), TEXT("無記錄"));
}

INT_PTR CALLBACK MainDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			{
				RECT rectChild, rectParent;  
				int DlgWidth, DlgHeight;    // 以像素为单位的对话框宽度和高度  
				int NewPosX, NewPosY;  
	  
				// 设法使“关于”对话框居中显示  
				if (GetWindowRect(hDlg, &rectChild))   
				{  
					GetClientRect(GetParent(hDlg), &rectParent);  
					DlgWidth    = rectChild.right - rectChild.left;  
					DlgHeight   = rectChild.bottom - rectChild.top ;  
					NewPosX     = (DlgWidth) / 2;  
					NewPosY     = (DlgHeight) / 2;  
	                  
					// 如果“关于”框比实际屏幕大  
					if (NewPosX < 0) NewPosX = 0;  
					if (NewPosY < 0) NewPosY = 0;  
					SetWindowPos(hDlg, 0, NewPosX, NewPosY,  
						0, 0, SWP_NOZORDER | SWP_NOSIZE);  
				}
				HWND hCheck = GetDlgItem(hDlg, IDC_CHECK4);
				SendMessage(hCheck, BM_SETCHECK, 1, 0);
				for(int i = 0;i<10;i++){
					HWND hCheck = GetDlgItem(hDlg, IDC_CHECK1 + i);
					SendMessage(hCheck, BM_SETCHECK, 1, 0);
				}
				//printf("%d\n",INT_MAX); %d up to 2147483647
				//printf("%d\n",INT_MIN);
				//printf("%u\n",UINT_MAX);
				//printf("%u\n",ULONG_MAX); %u up to 4294967295
				char combosf[MAX_PATH] = {0};
				char hisf[MAX_PATH] = {0};
				TCHAR combosn[MAX_PATH] = {0};
				combosFileImported = 0;
				hisFileImported = 0;
				if(getImportedCombosFile(combosf))
				{
					combosFileImported = 1;
					icombosF = getCombos(combosf);
					prepareWeight(icombosF);
					prepareHit(icombosF);
					prepareMaxNoHit();
					combosF = initCombo(icombosF, combosf);

					swprintf(combosn,TEXT("已導入%d個4碼組合"),icombosF);
					SetWindowText(GetDlgItem(hDlg, IDC_IMPORTCFILE), combosn);
				}
				else
				{
					SetWindowText(GetDlgItem(hDlg, IDC_IMPORTCFILE), TEXT("無記錄"));
				}
				if(getImportedHisFile(hisf))
				{
					int lotteries[MAX_HISTORY_NUM][3] = {0};
					int rel = parseLottery(hisf, lotteries, NULL);
					if(rel > 0)
					{
						hisFileImported = 1;
						TCHAR hisn[MAX_PATH] = {0};
						swprintf(hisn,TEXT("已導入%d個歷史出獎號"),rel);
						SetWindowText(GetDlgItem(hDlg, IDC_IMPORTHFILE), hisn);
					}
				}
				else
				{
					SetWindowText(GetDlgItem(hDlg, IDC_IMPORTHFILE), TEXT("無記錄"));
				}
				ShowLotteryHistory(hDlg);
				
				int maxNoHit = parseMaxNoHit();
				if(ALERT_TH <= maxNoHit)
				{	
					//TCHAR combosn[MAX_PATH] = {0};
					swprintf(combosn,TEXT("警告！已有最大%d期不中的情況！"), maxNoHit);
					SetWindowText(GetDlgItem(hDlg, IDC_ALERT), combosn);
					
				}
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
				case IDC_ADDHIS:
					if(combosFileImported)
					{
						if ( IDOK == DialogBox(hInst, MAKEINTRESOURCE(IDD_ADDHIS), hDlg, AddHisDlgProc))
						{
							ShowLotteryHistory(hDlg);
							int noHit = staHit(combosF, icombosF);
							if(parseMaxNoHit() < noHit) saveMaxNoHit(noHit);
							if(ALERT_TH <= noHit)
							{	
								TCHAR combosn[MAX_PATH] = {0};
								swprintf(combosn,TEXT("警告！已有最大%d期不中的情況"),noHit);
								SetWindowText(GetDlgItem(hDlg, IDC_ALERT), combosn);
								
							}
						}
					}
					else
					{
						MessageBox(hDlg, TEXT("請導入4碼組合後再操作！"), TEXT("警告"), MB_ICONWARNING | MB_OK);
					}
					break;
				case IDC_STAT:
					if(combosFileImported)
					{
						ShowResult(hDlg);
					}
					else
					{
						MessageBox(hDlg, TEXT("請導入4碼組合後再操作！"), TEXT("警告"), MB_ICONWARNING | MB_OK);
					}
					break;
				case IDC_CLEAR:
					ClearLotteryHistory(hDlg);						
					break;
				case IDC_IMPORTC:
					ImportCombos(hDlg);
					break;
				case IDC_IMPORTH:
					ImportHis(hDlg);
					break;
			}
			break;

		case WM_CLOSE:
			//if(MessageBox(hDlg, TEXT("Close the program?"), TEXT("Close"),
			//MB_ICONQUESTION | MB_YESNO) == IDYES)
			{
				if(combosFileImported)
				{
					for(int i = 0; i < icombosF;i++)
					{
						free(combosF[i].combo_array);
					}
					free(combosF);
				}
				_CrtDumpMemoryLeaks();
				DestroyWindow(hDlg);
			}
			return TRUE;

		case WM_DESTROY:
			PostQuitMessage(0);
			return TRUE;
	}

	return FALSE;
}