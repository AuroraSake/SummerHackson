/*******************************************************************************
* タイトル:		
* プログラム名:	timer.h
* 作成者:		GP11A132 21 野寺翔悟
* 作成日:		2023/07/28
*******************************************************************************/

#pragma once	// インクルードガード（多重インクルード防止）

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "main.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
//#ifdef _DEBUG
    #define TIMER_MAX			(60)		// タイマーの最大値
//#else
//    #define TIMER_MAX			(60)		// タイマーの最大値
//#endif
#define TIMER_DIGIT			(2)			// 桁数


/*******************************************************************************
* 構造体定義
*******************************************************************************/


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
HRESULT InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);

int GetTimer(void);


void PassTime(int time);