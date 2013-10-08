#if !defined(INCLUDED_RUNMODE_H_)
#define INCLUDED_RUNMODE_H_

#include "kernel.h"

// ‘®«‚ğ•Û‚·‚é‚½‚ß‚Ì\‘¢‘Ì‚Ì’è‹`
typedef struct RunMode
{
	BOOL run_mode;
} RunMode;

// ŒöŠJ‘€ì
void RunMode_init(RunMode* this);

#endif /*!defined(INCLUDED_RUNMODE_H_)*/
