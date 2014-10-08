#include "yoyo.h"

int getCombos();
int getCombos(char *filename);
COMBO_YOYO * initCombo();
COMBO_YOYO * initCombo(int icombos, char *filename);
int staCombos(int *parray, int *pEnable,int *pNumber, COMBO_YOYO *pcombos, int n, ULONG *numbers);
int parseLottery(int pArray[MAX_HISTORY_NUM][3], TCHAR pLotteries[MAX_HISTORY_NUM][HISTORY_PATH]);
int parseLottery(char *filename, int pArray[MAX_HISTORY_NUM][3], TCHAR pLotteries[MAX_HISTORY_NUM][HISTORY_PATH]);
int saveLottery(int* pArray);
int saveLottery(int* pArray, char *filename);
int prepareWeight(int combo_n);
int prepareHit(int combo_n);
int prepareLotteries();
int parseWeight(double pArray[COMBOS_N]);
int staWeight(int *pEnable, int *pNumber, COMBO_YOYO *pcombos, int n, ULONG *numbers);
int saveImportedCombosFile(char *filename);
int getImportedCombosFile(char *filename);
int saveImportedHisFile(char *filename);
int getImportedHisFile(char *filename);
int staHit(COMBO_YOYO *pcombos, int n);
int prepareMaxNoHit();
int saveMaxNoHit(int MaxNoHit);
int parseMaxNoHit();
