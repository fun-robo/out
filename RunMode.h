#if !defined(INCLUDED_RUNMODE_H_)
#define INCLUDED_RUNMODE_H_

#include "kernel.h"

// ������ێ����邽�߂̍\���̂̒�`
typedef struct RunMode
{
	BOOL run_mode;
} RunMode;

// ���J����
void RunMode_init(RunMode* this);

#endif /*!defined(INCLUDED_RUNMODE_H_)*/
