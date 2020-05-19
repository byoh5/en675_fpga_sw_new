//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include "dev.h"

#include <string.h> // strcmp
#include <stdlib.h> // atoi

#include "enx_freertos.h"

#include "shell_cmd_isp.h"

const char *sIspCmd[] = {"Control ISP",          (char*)0};

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//
void ISPtoCPUcallback(void *ctx)
{
	printf("ISP to CPU!(%.32b)\n", SHREG_RSP);	// MUTEX 적용 필요
}

int atoh(const char *s, const int len)
{
	int o = 0;
	int u = 0;

	int i = strlen(s);
	if(len && i>len) i = len;

	for(i--;i>=0;i--)
	{
		if(s[i] == '\0') break;
		const int iHex = ('a' <= s[i] && s[i] <= 'f') ? s[i] - 'a' + 10 :
						 ('A' <= s[i] && s[i] <= 'F') ? s[i] - 'A' + 10 :
						 ('0' <= s[i] && s[i] <= '9') ? s[i] - '0' : 0 ;
		o |= iHex<<u;
		u += 4;
	}

	return o;
}

int cmd_isp(int argc, char *argv[])
{
	//printf("isp command\n");

	// first!
	//while (CPU_TO_ISP_MSG_MUTEX);
	//SHREG_CMD = (argc > 2) ? ((atoi(argv[1])&0xFFFF)<<16) | ((atoi(argv[2])&0xFFFF)<<0) : 0;
	//SHREG_CMD = (argc > 2) ? ((atoh(argv[1],4)&0xFFFF)<<16) | ((atoh(argv[2],4)&0xFFFF)<<0) : 0;
	//CPU_TO_ISP_MSG_MUTEX = 0;

	if(argc == 2) {
		while (CPU_TO_ISP_MSG_MUTEX);
		SHREG_CMD = atoh(argv[1],8);
		//CPU_TO_ISP_MSG_MUTEX = 0;
		AtoBIrqCall();
	}
	else if(argc == 3) {
		const WORD wAdr = atoh(argv[1],4);
		const WORD wDat = atoh(argv[2],4);

		if(UPtoMSG(UPstinv(ISP_BINARY_INFO)) < wAdr && wAdr < UPtoMSG(USR_PAR_EA)) {
			while (CPU_TO_ISP_MSG_MUTEX);
			SetByte((BYTE*)ISP_MSG_AREA+wAdr, UsrParSiz(MSGtoUP(wAdr)), wDat);
			CPU_TO_ISP_MSG_MUTEX = 0;
		}
	}

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

