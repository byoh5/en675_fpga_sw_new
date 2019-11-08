#include "dev.h"
#include "msg.h"

shell_msg_t gptMsgShell;
share_msg_t gptMsgShare;
debug_msg_t gptMsgDebug;

//*************************************************************************************************
// Console message(CPU0->CPU1)
//-------------------------------------------------------------------------------------------------
void enx_msgshell_init(volatile shell_msg_t *p)
{
	p->tot_num = MSG_SHELL_NUM;
}
