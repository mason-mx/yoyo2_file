#include "yoyo.h"

int getCombos();
COMBO_YOYO * initCombo();
int staCombos(int *parray, int *pEnable,int *pNumber, COMBO_YOYO *pcombos, int n, ULONG *numbers);
int parseLottery(int pArray[10][3], TCHAR pLotteries[LOTTERIES_ROW][HISTORY_PATH]);
int saveLottery(int* pArray);
int prepareWeight(int combo_n);
int prepareLotteries();
int parseWeight(double pArray[COMBOS_N]);
int staWeight(int *pEnable, int *pNumber, COMBO_YOYO *pcombos, int n, ULONG *numbers);