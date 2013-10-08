#if !defined(INCLUDED_RUNMODE_H_)
#define INCLUDED_RUNMODE_H_

#include "kernel.h"

// 属性を保持するための構造体の定義
typedef struct RunMode
{
	BOOL run_mode;
} RunMode;

// 公開操作
void RunMode_init(RunMode* this);

#endif /*!defined(INCLUDED_RUNMODE_H_)*/
