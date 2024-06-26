/*******************************************************************************
* �^�C�g��:		
* �v���O������:	timer.h
* �쐬��:		GP11A132 21 �쎛�Č�
* �쐬��:		2023/07/28
*******************************************************************************/

#pragma once	// �C���N���[�h�K�[�h�i���d�C���N���[�h�h�~�j

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "main.h"

/*******************************************************************************
* �}�N����`
*******************************************************************************/
//#ifdef _DEBUG
    #define TIMER_MAX			(60)		// �^�C�}�[�̍ő�l
//#else
//    #define TIMER_MAX			(60)		// �^�C�}�[�̍ő�l
//#endif
#define TIMER_DIGIT			(2)			// ����


/*******************************************************************************
* �\���̒�`
*******************************************************************************/


/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);

int GetTimer(void);


void PassTime(int time);