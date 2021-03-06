#if !defined(INCLUDED_RUNMODE_H_)
#define INCLUDED_RUNMODE_H_

#include "kernel.h"

typedef enum RunModeName{
	MAIMAI, LINE_TRACE,
} RunModeName;

// 属性を保持するための構造体の定義
typedef struct RunMode
{
	BOOL run_mode;
} RunMode;

// 公開操作
void RunMode_init(RunMode* this);
void RunMode_change(RunMode* this, int runModeName);

#endif /*!defined(INCLUDED_RUNMODE_H_)*/
