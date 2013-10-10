#if !defined(INCLUDED_RUNMODE_H_)
#define INCLUDED_RUNMODE_H_

#include "kernel.h"

typedef enum RunModeName{
	MAIMAI, LINE_TRACE,
} RunModeName;

// ������ێ����邽�߂̍\���̂̒�`
typedef struct RunMode
{
	BOOL run_mode;
} RunMode;

// ���J����
void RunMode_init(RunMode* this);
void RunMode_change(RunMode* this, int runModeName);

#endif /*!defined(INCLUDED_RUNMODE_H_)*/
