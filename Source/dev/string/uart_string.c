#include "dev.h"

#if USE_UART0 | USE_UART1 | USE_UART2 | USE_UART3 | USE_UART4 | USE_UART5 | USE_UART6 | USE_UART7 | USE_UART8

#include <string.h>		// strlen()

#if 1
//-------------------------------------------------------------------------------------------------
// Tx header
// Eyenix Protocol
#define	PC_STX		0x02			// STX

	//						gbRxStg :  0   1   CMD
	#define PC_CMD_REGW		0x10	// STX CMD ADH ADL DAH DA1 DA2 DAL ETX	: ISP Reg. Write
	#define PC_CMD_REGR		0x20	// STX CMD ADH ADL ETX					: ISP Reg. Read
	#define PC_CMD_KEY		0x30	// STX CMD KEY ETX						: KEY
	//#define PC_CMD_CHA		0x40	// STX CMD CHA ETX						: gbWinChar	- Not Used

	#define PC_CMD_PARW 	0x50 		// Cmd : Write parameter in F/W. 			2 addr, 1 data.
	#define PC_CMD_PARR 	0x58 		// Cmd : Read paramter in F/W. 				2 addr, 1 data.
	#define PC_CMD_EEPW 	0x60 		// Cmd : Write paramter from F/W to EEP.
	#define PC_CMD_EEPR 	0x61 		// Cmd : Read paramter from EEP to F/W.
	//#define PC_CMD_EEPD 	0x62 		// Cmd : Write / Read for EEPROM is done.
	#define PC_CMD_PARK		0x68		// Cmd : Enable Parameter					4 data

	//#define PC_CMD_SENW 	0x70 		// Cmd : Write register in Sensor. 			2 addr, 4 data
	//#define PC_CMD_SENR 	0x78 		// Cmd : Read register from Sensor.

#if model_I2cSlave == 1	|| model_I2cSlave == 2
	#define I2C_CMD_MENUW 	/*0x50*/	// STX CMD ADH ADL WDH WDL ETX : Menu parameter write
	#define I2C_CMD_MENUR	/*0x60*/	// STX CMD ADH ADL RDH RDL ETX : Menu parameter read
	#define I2C_CMD_CLOCK	/*0x70*/	// Cmd : Internet Standard Time
#endif

	#define PD_STG			0x80	// xFF ADD CMD CMD DAT DAT CRC			: Pelco-D
	#define PP_STG			0x90	// xA0 ADD DAT DAT DAT DAT xAF CRC		: Pelco-P

	#define PC_CMD_AWBI 	0xaa	// STX CMD ETX							: InitAwbOdmCap()

	#define	PC_CMD_STR		0xb0	// UartTxStr()
	#define	PC_CMD_GRP		0xc0	// UartTxGrp()
	#define	PC_CMD_ERR		0xd0	// UartTxErr()

	#define	PC_CMD_INI		0xe0	// STX CMD ETX							: UartRstQue()	- ?
	#define	PC_CMD_ACK		0xf0	// STX CMD ETX							: TxAck()		- ?

	#define PC_CMD_DOWN		0xf8	// STX CMD ETX							: Uart Download request Rx

#if (model_Lens==1)
		// System
	#define TMC_TABLEREAD			0x01
	#define TMC_TABLEWRITE			0x02 // Data 부분이 4Byte * 11 = 총 44Byte입니다.
	#define TMC_LENSINITIALIZATION	0x03 // Write Command로 보내며, Data는 4Byte로 TRUE(1)를 보냅니다.
	#define TMC_CALIBRATIONSTART	0x04 // Write Command로 보내며, Data는 4Byte로 TRUE(1)를 보냅니다.

		// Zoom
	#define TMC_ZOOM_TOTALSTEP		0x10
	#define TMC_ZOOM_STARTPOSITION	0x11
	#define TMC_ZOOM_ENDPOSITION	0x12
	#define TMC_ZOOM_SPEED			0x13
	#define TMC_ZOOM_MINRATIO		0x14
	#define TMC_ZOOM_MAXRATIO		0x15

		// Focus
	#define TMC_FOCUS_TOTALSTEP		0x20
	#define TMC_FOCUS_STARTPOSITION	0x21
	#define TMC_FOCUS_ENDPOSITION	0x22
	#define TMC_FOCUS_SPEED			0x23
	#define TMC_FOCUS_MODE			0x24
	#define TMC_FOCUS_DnNFILTER		0x25
#endif

#define	PC_ETX		0x03			// ETX

// Pelco D Protocol
#define PD_STX		0xFF

// Pelco P Protocol
#define PP_STX		0xA0
#define PP_ETX		0xAF

// I2C Protocol
#define	I2C_STX		PC_STX
#define I2C_ETX		PC_ETX

#define I2C_STX_SIZ		1
#define I2C_CMD_SIZ		1
#define I2C_ADR_SIZ		2
#define I2C_DAT_SIZ		2
#define I2C_ETX_SIZ		1

#define I2C_ALL_SIZ		(I2C_STX_SIZ+I2C_CMD_SIZ+I2C_ADR_SIZ+I2C_DAT_SIZ+I2C_ETX_SIZ)

//-------------------------------------------------------------------------------------------------
// Snatch logic address (Regulate with logic address .CAUTION)
#define	BASE_SENS			0x4000		// 0x300 ea 		sensor control
#define BASE_EN331			0x6000
#define BASE_EEPROM			0x7000
#define BASE_MICOM_PAR		0x8000


//-------------------------------------------------------------------------------------------------
#define Case1(CMD,VAL)\
	case (CMD) :\
		(VAL) = bIn;\
		gbRxStg++;\
		break

#define Case2(CMD,VAL)\
	case (CMD)   :\
	case (CMD)+1 :\
		(VAL) = ((VAL)<<8) | bIn;\
		gbRxStg++;\
		break

#define Case4(CMD,VAL)\
	case (CMD)   :\
	case (CMD)+1 :\
	Case2((CMD)+2,VAL)

#define SetCase0(C,E,EC, ...)			case C : if(E) { EC } else goto err_proc; SetEnd(); break
#define SetCase1(C,E,EC,D0,D0n)			Case##D0n(C,D0);  SetCase0(C+D0n,E,EC)
#define SetCase2(C,E,EC,D0,D0n,...)		Case##D0n(C,D0);  SetCase1(C+D0n,E,EC,__VA_ARGS__)
#define SetCase3(C,E,EC,D0,D0n,...)		Case##D0n(C,D0);  SetCase2(C+D0n,E,EC,__VA_ARGS__)

#define SetCase(CMD,EndChk,EndCode,VAL_NUM,...)	SetCase##VAL_NUM(CMD,EndChk,EndCode,__VA_ARGS__)

#define UNUSED_ARG(x)			((void)(x))

#define UART_TX_ECM_DLY			WaitXus(1000)	// TODO KSH> ECM에서 Read를 위 또는 아래로 연속 실행하는 경우 Uart 오동작 발생 방지, 1000us가 가장 오동작 발생률 적음

#if 0
#define IRQ_DISABLE				{ clear_csr(mstatus, MSTATUS_MIE); }
#define IRQ_ENABLE				if(gnCommOn == 0) { set_csr(mstatus, MSTATUS_MIE); }

#define UART_TX_IRQ_STA			{ IRQ_DISABLE UartTxSetIrqEn(DEBUG_UART_NUM, ENX_OFF); giTxOn = 1; }
#define UART_TX_IRQ_END			{ giTxOn = 0; UartTxSetIrqEn(DEBUG_UART_NUM, ENX_ON); IRQ_ENABLE }
#else
#define IRQ_DISABLE				{ UartTxSetIrqEn(DEBUG_UART_NUM, ENX_OFF); }
#define IRQ_ENABLE				/*if(gnCommOn == 0 && giTxIrqHold == 0)*/ { UartTxSetIrqEn(DEBUG_UART_NUM, ENX_ON); }

#define UART_TX_IRQ_STA			{ IRQ_DISABLE giTxOn = 1; }
#define UART_TX_IRQ_END			{ giTxOn = 0; /*gpbTxRegOn = gtUartQue.pbTxHead;*/ IRQ_ENABLE }
#endif

#define UART_IRQ_OSD_Y	24
#define UART_IRQ_OSD	0
#if UART_IRQ_OSD == 1
	#define UIO(A,B)	A
#else
	#define UIO(A,B)	B
#endif

//-------------------------------------------------------------------------------------------------
// Variables
//UINT	gnCommOn = 0;

BYTE	gbETX = 0;
BYTE	gbRxStg = 0;
WORD	gwRxAddr = 0;
UINT	gnRxData = 0;
BYTE	gbParOn = 0x0;

#define SetEnd() {\
		gbRxStg = 0;\
		gwRxAddr = 0;\
		gnRxData = 0;\
		gbETX = 0;\
	}

//BYTE gbWinChar = 0;													// Window character
union uFLOAT gnTxGrp[GRP_NUMBER] = { {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0} };	// Graphic data

#if USE_UP_PAR == 0
UINT gnRxPar[32] = {
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	// Parameter data (Regulate with logic address .CAUTION)
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
					};
#endif

//UINT gnDownOn = 0;										// PC_CMD_DOWNLOAD

UINT gnI2cOn = 0;

TUartQue gtUartQue;
volatile int giTxOn = 0;
//volatile int giTxCnt = 0;
//volatile BYTE* gpbTxCont = 0;
//volatile BYTE* gpbTxRegOn = 0;
//volatile int giTxIrqHold = 0;
volatile int giTxIrqRdy = 0;

//-------------------------------------------------------------------------------------------------
// Functions
void UartRstRxQue(void)
{
	gtUartQue.wRxLen = 0;
	gtUartQue.pbRxHead = gtUartQue.bRxQue;
	gtUartQue.pbRxTail = gtUartQue.bRxQue;
}

void UartRstTxQue(void)
{
	gtUartQue.wTxLen = 0;
	gtUartQue.pbTxHead = gtUartQue.bTxQue;
	gtUartQue.pbTxTail = gtUartQue.bTxQue;
}

void UartRstQue(void)
{
	UartRstRxQue();
	UartRstTxQue();
}

UIO(UINT gnRxOsdPos = 0;,)

void UartDebugRxIrq(void *ctx)
{
	volatile BYTE bBuf = UartRxGetByte(DEBUG_UART_NUM);

	if(gtUartQue.wRxLen < (UART_BUF_SIZE-1)) {

		*gtUartQue.pbRxHead++ = bBuf;

		UIO(
			FontEx(UART_IRQ_OSD_Y, (gnRxOsdPos<<1), NO_ALPHA, MN_WHITE, 2);
			gnRxOsdPos++;
			if(gnRxOsdPos==18) gnRxOsdPos = 0;
			FontHexEx(UART_IRQ_OSD_Y, (gnRxOsdPos<<1), NO_ALPHA, MN_YELLOW, *(gtUartQue.pbRxHead-1), 2);
		,)

		if(gtUartQue.pbRxHead == (gtUartQue.bRxQue + UART_BUF_SIZE)) {
			gtUartQue.pbRxHead = gtUartQue.bRxQue;
		}
		gtUartQue.wRxLen++;
	}
	else {	// UART RX FULL

	}
}

UIO(UINT gnTxOsdPos = 0;,)

#if 0
void UartTxAllBuffer(void)
{
	while(gpbTxCont != 0) {
		while(UartTxIsFull(DEBUG_UART_NUM)) WaitXus(10);

		UartTxSetByte(DEBUG_UART_NUM, *gtUartQue.pbTxTail++);

		if(	gtUartQue.pbTxTail == (gtUartQue.bTxQue + UART_BUF_SIZE)) {
			gtUartQue.pbTxTail = gtUartQue.bTxQue;
		}
		gtUartQue.wTxLen--;

		if(gpbTxCont == gtUartQue.pbTxTail) gpbTxCont = 0;
	}
}
#endif

void UartDebugTxIrq(void *ctx)
{
	if(giTxOn) { UartTxSetIrqEn(DEBUG_UART_NUM, ENX_OFF); UIO(goto TxIrqEnd, return); }
	else {

		//if(UartTxIsEmpty(DEBUG_UART_NUM) == 0) UIO(goto TxIrqEnd, return);	// UART Polling TX 대기

#if 1
		while(gtUartQue.wTxLen)
		{
			if(UartTxIsFull(DEBUG_UART_NUM)) {
				/*if(gpbTxRegOn) {
					WaitXus(10);
					continue;
				}
				else*/ break;
			}

			UIO(
				FontEx(UART_IRQ_OSD_Y+1, (gnTxOsdPos<<1), NO_ALPHA, MN_WHITE, 2);
				gnTxOsdPos++;
				if(gnTxOsdPos==18) gnTxOsdPos = 0;
				FontHexEx(UART_IRQ_OSD_Y+1, (gnTxOsdPos<<1), NO_ALPHA, MN_YELLOW, *gtUartQue.pbTxTail, 2);
			,)

			UartTxSetByte(DEBUG_UART_NUM, *gtUartQue.pbTxTail++);

			if(	gtUartQue.pbTxTail == (gtUartQue.bTxQue + UART_BUF_SIZE)) {
				gtUartQue.pbTxTail = gtUartQue.bTxQue;
			}
			gtUartQue.wTxLen--;

			giTxIrqRdy = 1;

			//if(gpbTxCont == gtUartQue.pbTxTail) gpbTxCont = 0;

			/*if(gpbTxRegOn == gtUartQue.pbTxTail) {
				gpbTxRegOn = 0;
				if(giTxIrqHold == 1) {
					UartTxSetIrqEn(DEBUG_UART_NUM, ENX_OFF);
					giTxIrqHold = 2;
				}
			}*/
		}

		if(gtUartQue.wTxLen == 0) { UartTxSetIrqEn(DEBUG_UART_NUM, ENX_OFF); }
		//else { if(gpbTxCont == 0) gpbTxCont = gtUartQue.pbTxHead; }

#else
		do {
			if(gtUartQue.wTxLen == 0) {
				UartTxSetIrqEn(DEBUG_UART_NUM, ENX_OFF);
				break;
			}
			else {
				UIO(
					FontEx(23, (gnTxOsdPos<<1), NO_ALPHA, MN_WHITE, 2);
					gnTxOsdPos++;
					if(gnTxOsdPos==18) gnTxOsdPos = 0;
					FontHexEx(23, (gnTxOsdPos<<1), NO_ALPHA, MN_YELLOW, *gtUartQue.pbTxTail, 2);
				,)

				UartTxSetByte(DEBUG_UART_NUM, *gtUartQue.pbTxTail++);

				if(	gtUartQue.pbTxTail == (gtUartQue.bTxQue + UART_BUF_SIZE)) {
					gtUartQue.pbTxTail = gtUartQue.bTxQue;
				}
				gtUartQue.wTxLen--;
			}
		} while( UartTxIsFull(DEBUG_UART_NUM) == 0 );
#endif
	}

UIO(
TxIrqEnd:
	FontHexEx(UART_IRQ_OSD_Y+2, 0, NO_ALPHA, MN_WHITE, gtUartQue.wTxLen, 3);
,)
}

void UartTxIrq(BYTE abByte)
{
	//UartTx(DEBUG_UART_NUM, abByte); return;

	if(gtUartQue.wTxLen >= UART_BUF_SIZE) return;	// The Tx data is lost.

#if 0
	UartTxSetIrqEn(DEBUG_UART_NUM, ENX_OFF);
	giTxOn = 1;
#endif

	*gtUartQue.pbTxHead++ = abByte;
	if( gtUartQue.pbTxHead == (gtUartQue.bTxQue + UART_BUF_SIZE)){
		gtUartQue.pbTxHead = gtUartQue.bTxQue;
	}
	gtUartQue.wTxLen++;

#if 0
	giTxOn = 0;
	/*if(gtUartQue.wTxLen == 1)*/	UartTxSetIrqEn(DEBUG_UART_NUM, ENX_ON);
#endif
}

void UartTxReg(UINT anAddr, UINT anData)
{
	UART_TX_IRQ_STA

	UartTxIrq(PC_STX);
	UartTxIrq(PC_CMD_REGR);

	UartTxIrq(anAddr>> 8);
	UartTxIrq(anAddr>> 0);

	UartTxIrq(anData>>24);
	UartTxIrq(anData>>16);
	UartTxIrq(anData>> 8);
	UartTxIrq(anData>> 0);

	UartTxIrq(PC_ETX);

	//gpbTxRegOn = gtUartQue.pbTxHead;

	UART_TX_IRQ_END
}

void UartTxErr(void)
{
	UART_TX_IRQ_STA

	UartTxIrq(PC_STX);
	UartTxIrq(PC_CMD_ERR);
	UartTxIrq(PC_ETX);

	UART_TX_IRQ_END
}

union uFLOAT gnTxGrp0[GRP_NUMBER] = { {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0} };	// Graphic data
int giUartTxGrpOn = 0;

void ISRT0 UartTxGrp(void)
{
	for(int i=0; i<GRP_NUMBER; i++) {
		gnTxGrp0[i].m_UINT = gnTxGrp[i].m_UINT;
	}
	giUartTxGrpOn = 1;
}

void ISRT0 UartTxGrpSet(UINT anNum, UINT anData)
{
	if(anNum >= GRP_NUMBER) return;
	gnTxGrp0[anNum].m_UINT = anData;
	giUartTxGrpOn = 1;
}

void ISRT UartTxGrpRun(void)
{
	if(!giUartTxGrpOn) return;
	giUartTxGrpOn = 0;

	int i;

	//ULONG TxGrpTimeStart = BenchTimeStart();

	UART_TX_IRQ_STA

#if 1
	UartTxIrq(PC_STX);
	UartTxIrq(PC_CMD_GRP);

	for(i=0; i<GRP_NUMBER; i++){
		UartTxIrq(gnTxGrp0[i].m_UINT>>24);
		UartTxIrq(gnTxGrp0[i].m_UINT>>16);
		UartTxIrq(gnTxGrp0[i].m_UINT>> 8);
		UartTxIrq(gnTxGrp0[i].m_UINT>> 0);
	}

	UartTxIrq(PC_ETX);
#else
	UartTx(DEBUG_UART_NUM, PC_STX);
	UartTx(DEBUG_UART_NUM, PC_CMD_GRP);

	for(i=0; i<GRP_NUMBER; i++){
		UartTx(DEBUG_UART_NUM, gnTxGrp0[i].m_UINT>>24);
		UartTx(DEBUG_UART_NUM, gnTxGrp0[i].m_UINT>>16);
		UartTx(DEBUG_UART_NUM, gnTxGrp0[i].m_UINT>> 8);
		UartTx(DEBUG_UART_NUM, gnTxGrp0[i].m_UINT>> 0);
	}

	UartTx(DEBUG_UART_NUM, PC_ETX);
#endif

	//const UINT TxGrpTime = BenchTimeStop(TxGrpTimeStart);

	UART_TX_IRQ_END
}

void UartTxStrIrq0(const char *apcStr)
{
	while(*apcStr){
		UartTxIrq(*apcStr++);
	}
}

void UartTxStr0(UINT nCH, const char *apcStr)
{
	while(*apcStr) {
		UartTx(nCH, *apcStr++);
	}
}

void UartTxStrEx(const UINT nCH, const char* apbStr0, const char* apbStr1, UINT anLen, UINT anAddNewLine)
{
	if(anAddNewLine) anLen += 2;

	if(nCH < UART_CNT) {	// Polling operation
#if 0
		//while(gtUartQue.wTxLen || UART0_TX_IRQ_EN) { NOP4 };
		if(gtUartQue.wTxLen || UART0_TX_IRQ_EN) {
			UINT nStNum;
			const UINT nSt = GetSt(&nStNum);
			SetStEnaIrq(nSt);
			while(gtUartQue.wTxLen || UART0_TX_IRQ_EN) { NOP4 };
			SetSt(nSt,&nStNum);
		}
#else
		while(giTxIrqRdy && (gtUartQue.wTxLen || UartTxGetIrqEn(DEBUG_UART_NUM))) WaitXus(10);
		/*if(gtUartQue.wTxLen && UartTxGetIrqEn(DEBUG_UART_NUM)) {
			giTxIrqHold = 1;
			while(giTxIrqHold != 2) WaitXus(10);
		}*/
#endif
		UartTx(nCH, PC_STX);
		UartTx(nCH, PC_CMD_STR);
		UartTx(nCH, anLen);

		if(apbStr0) UartTxStr0(nCH, apbStr0);
		if(apbStr1) UartTxStr0(nCH, apbStr1);

		if(anAddNewLine) {
			UartTx(nCH, '\r');
			UartTx(nCH, '\n');
		}

		UartTx(nCH, PC_ETX);

		/*if(giTxIrqHold == 2) {
			giTxIrqHold = 0;
			if(gtUartQue.wTxLen) UartTxSetIrqEn(DEBUG_UART_NUM, ENX_ON);
		}*/
	}
	else {					// Interrupt operation
		UART_TX_IRQ_STA

		UartTxIrq(PC_STX);
		UartTxIrq(PC_CMD_STR);
		UartTxIrq(anLen);

		if(apbStr0) UartTxStrIrq0(apbStr0);
		if(apbStr1) UartTxStrIrq0(apbStr1);

		if(anAddNewLine) {
			UartTxIrq('\r');
			UartTxIrq('\n');
		}

		UartTxIrq(PC_ETX);

		UART_TX_IRQ_END
	}
}

void UartTxStrCh(const UINT nCH, const char* apbStr0)
{
	UartTxStrEx(nCH, apbStr0, 0, (apbStr0) ? strlen(apbStr0) : 0, 1);
}

void UartTxStrHexCh(const UINT nCH, const char* apbStr0, UINT anVal, UINT anValLen)
{
	char bStr[9];
	if(9 <= anValLen) anValLen = 9 - 1;	// 9 = 8 + '\0'
	const BYTE bLen = ((apbStr0) ? strlen(apbStr0) : 0) + anValLen;

	if(anValLen) uint2strh(bStr, anVal, anValLen);

	UartTxStrEx(nCH, apbStr0, (anValLen) ? bStr : 0, bLen, 1);
}

void UartTxStrDecCh(const UINT nCH, const char* apbStr0, UINT anVal, UINT anValLen)
{
	char bStr[12];
	if(12 <= anValLen) anValLen = 12 - 1;	// 12 = sign + 10 + '\0'
	const BYTE bLen = ((apbStr0) ? strlen(apbStr0) : 0) + anValLen;

	if(anValLen) int2str(bStr, anVal, anValLen, 0);

	UartTxStrEx(nCH, apbStr0, (anValLen) ? bStr : 0, bLen, 1);
}

void /*ISPMu*/ UartTxPar(const UINT anAddr, BYTE abData)
{
	UART_TX_IRQ_STA

	UartTxIrq(PC_STX);
	UartTxIrq(PC_CMD_PARR);					// Cmd

	UartTxIrq(anAddr>> 8);						// Addr[15: 8]
	UartTxIrq(anAddr>> 0);						// Addr[ 7: 0]

	UartTxIrq(abData>> 0);						// Data[ 7: 0]

	UartTxIrq(PC_ETX);							// ETX

	UART_TX_IRQ_END
}

void /*ISPMu*/ UartRxPar(const int aiWrite)
{
	if(aiWrite && (gbParOn == 0xFF)) {
		if(UP_START < gwRxAddr && gwRxAddr < UP_END) {
			gbUsrParTbl[gwRxAddr] = gnRxData;
			UsrParChgEndIdx(gwRxAddr);
			menu_redraw(1,0);
		}
	}

	if(gbParOn != 0xFF) {
		UartTxPar(0xFFFF, 0);
	}
	else if(gwRxAddr == 0xFFFF) {
		UartTxPar(UP_END, gbUsrParTbl[UP_END]);
	}
	else if(gwRxAddr < USR_PAR_EA){
		UartTxPar(gwRxAddr, gbUsrParTbl[gwRxAddr]);
	}
	else {
		UartTxPar(gwRxAddr, 0);
	}

	UART_TX_ECM_DLY;
}

void /*ISPMu*/ UartTxParK(const BYTE abVal)
{
	UART_TX_IRQ_STA

	UartTxIrq(PC_STX);
	UartTxIrq(PC_CMD_PARK);					// Cmd
	UartTxIrq(abVal);
	UartTxIrq(PC_ETX);							// ETX

	UART_TX_IRQ_END
}

void /*ISPMu*/ UartRxParK(void)
{
	#define PW_CONTINUE		3

	if(gbParOn < PW_CONTINUE || gbParOn == 0xFF)
	{
		if(gnRxData == model_ParID) {
			gbParOn = 0xFF;
			UartTxParK(1);
			UART_TX_ECM_DLY;
			UartTxStr("Parameter Enable On ");
		}
		else {
			if(gbParOn == 0xFF) gbParOn = 0;
			gbParOn++;
			UartTxParK(0);
			UART_TX_ECM_DLY;
			UartTxStrHex("Parameter ID Error...", gbParOn, 1);
		}
	}

	if(gbParOn == PW_CONTINUE)
	{
		gbParOn++;

		UsrParValid(0);
		gbUsrParSaveChk = 1;
		UartTxStr("Parameter Reset !!!");
	}
	else if(gbParOn > PW_CONTINUE && gbParOn != 0xFF)
	{
		UartTxParK(0);
	}
}

void UpReset(void)
{
	gbParOn = PW_CONTINUE;
	UartRxParK();
}

void /*ISPMu*/ UartTxEepDone(void)
{
	UART_TX_IRQ_STA

	UartTxIrq(PC_STX);
	UartTxIrq(PC_CMD_EEPR);					// Cmd
	UartTxIrq(PC_ETX);							// ETX

	UART_TX_IRQ_END
}

void /*ISPMu*/ UartTxParSave(void)
{
	gbUsrParSaveChk = 1;
	//DI();
	//AppSavePar();
	//EI();
	UartTxEepDone();
}

void /*ISPMu*/ UartTxParRead(void)
{
	gbUsrParReadChk = 1;		// EN778
	//AppLoadPar();
	UartTxEepDone();
}

void UartRxAddr(const int aiWrite)
{
	const int iRxAddr = gwRxAddr;

	#define SET_ADDR(ADDR_START, ADDR_END, WRITE_CMD, READ_CMD, READ_DATA)	\
		if((int)(ADDR_START) <= iRxAddr && iRxAddr < (int)(ADDR_END)) {\
			const WORD wAddr = iRxAddr - (ADDR_START);\
			UNUSED_ARG(wAddr);\
			if(aiWrite) { WRITE_CMD; }\
			READ_CMD\
			UartTxReg(iRxAddr, (READ_DATA));}


	SET_ADDR(0x0, BASE_SENS,				// ISP : 0x0000 ~ 0x3fff
		// ISP Write Code
		SetIsp(wAddr, gnRxData);

		//GRP3 = wAddr;
		//GRP4 = gnRxData;
		//GRP7 = GetIsp(wAddr);
		,
		// ISP Read Code
		,
		GetIsp(wAddr))

#ifdef __SENSOR__
	SET_ADDR(BASE_SENS, BASE_EN331,		// Sensor : 0x4000 ~ 0x5fff

	#if model_Pana || model_Aptn

	#elif (model_Sens==SENS_IMX323)
		#define SENS_ADDR_OFS		((wAddr < 0x1000) ? 0x0 : 0x2000)
	#elif model_Sony || model_Omni
		#define SENS_ADDR_OFS		0x3000
	#else
		#define SENS_ADDR_OFS		0x3000
	#endif

		// Sensor Write Code
		UartTxStrHexNoIRQ("SetSens:", SENS_ADDR_OFS+wAddr, 2);
		UartTxStrHexNoIRQ("Data:", gnRxData, 8);
		SetSens(SENS_ADDR_OFS+wAddr, gnRxData);

	#if model_Aptn
		WaitXus(20000);
	#endif
		,
		// Sensor Read Code
		UartTxStrHexNoIRQ("GetSens:", SENS_ADDR_OFS+wAddr, 8);
		//UartTxStrHex("GetSens:", wAddr, 8);
		//const UINT nBuf = GetSens(wAddr);
		,

		GetSens(SENS_ADDR_OFS+wAddr)
		/*nBuf*//*0x0*//* =Dummy */)
#endif

	SET_ADDR(BASE_MICOM_PAR, BASE_MICOM_PAR + 0x20,	// Micom Para : 0x8000 ~ 0x8020
		// User Parameter Custom Write Code
	#if USE_UP_PAR == 0
		//gnRxPar[wAddr] = gnRxData;

		//gnRxPar[wAddr] = (/*PL_STA < wAddr &&*/ wAddr < 0x1F/*PL_END*/) ? hex2dec(gnRxData) : gnRxData ;
		gnRxPar[wAddr] = hex2dec(gnRxData);
	#else
		const UINT nUpIdx = UPi(UpPAR00)+(wAddr<<2);
		SetByte(gbUsrParTbl+nUpIdx, 4, hex2dec(gnRxData));
		UsrParChg(nUpIdx);
	#endif
		,
		// User Parameter Custom Read Code
	#if USE_UP_PAR == 0
		//UINT nRxParOut = (/*PL_STA < wAddr &&*/ wAddr < 0x1F/*PL_END*/) ? dec2hex(gnRxPar[wAddr]) : gnRxPar[wAddr] ;
		UINT nRxParOut = dec2hex(gnRxPar[wAddr]);
	#else
		UINT nRxParOut = dec2hex(GetByte(gbUsrParTbl+UPi(UpPAR00)+(wAddr<<2), 4));
	#endif
		,
		//gnRxPar[wAddr])
		nRxParOut)

	UART_TX_ECM_DLY;
}

void Comm(void)
{
	BYTE bIn;

	//gnCommOn = 1;

	while(gtUartQue.wRxLen > 0)
	{
		//if(gpbTxRegOn != 0) break;

		bIn = *gtUartQue.pbRxTail++;
		if( gtUartQue.pbRxTail == (gtUartQue.bRxQue+UART_BUF_SIZE) ) {
			gtUartQue.pbRxTail  = gtUartQue.bRxQue;
		}
		gtUartQue.wRxLen--;

		switch(gbRxStg){
		 //...............................................
		 // Start -> Command
			case 0x00 :
				switch(bIn){
					case PC_STX : gbRxStg++; break;
//		 					case PD_STX : gbRxStg = PD_STG; break;
//		 					case PP_STX : gbRxStg = PP_STG; break;
				}
				break;

			case 0x01 :
				switch(bIn){
					case PC_CMD_REGW :
					case PC_CMD_REGR :
					case PC_CMD_KEY  :
					//case PC_CMD_CHA  :

					case PC_CMD_AWBI :

					case PC_CMD_INI  :
					case PC_CMD_ACK  :

					case PC_CMD_PARW :
					case PC_CMD_PARR :
					case PC_CMD_EEPW :
					case PC_CMD_EEPR :
					case PC_CMD_PARK :

					case PC_CMD_DOWN :

					case PC_CMD_STR  :
						gbRxStg = bIn;
						break;

					default	:
						goto err_proc;
						break;
				}
				break;

 //...............................................
 // Reg write process : ISP register set
			SetCase(PC_CMD_REGW, bIn==PC_ETX, if(gnI2cOn==0) { UartRxAddr(1); }
					, 2, gwRxAddr,2, gnRxData,4);
 //...............................................
 // Reg read process : ISP register get
			SetCase(PC_CMD_REGR, bIn==PC_ETX, UartRxAddr(0);
					, 1, gwRxAddr,2);

//...............................................
// Key process : Key from PC
			SetCase(PC_CMD_KEY, bIn==PC_ETX, gbUKeyVal = gnRxData;
										#if model_TgtBd == 1
											UartTxStrHex("UKeyVal ", gbUKeyVal,4);
										#endif
					, 1, gnRxData,1);

//...............................................
// Paramter write process : Parameter set
			SetCase(PC_CMD_PARK, bIn==PC_ETX, if(gnI2cOn==0) { UartRxParK(); }
					, 1, gnRxData,4);

//...............................................
// Paramter write process : Parameter set
			SetCase(PC_CMD_PARW, bIn==PC_ETX, if(gnI2cOn==0) { UartRxPar(1); }
					, 2, gwRxAddr,2, gnRxData,1);
//...............................................
// Paramter read process : Parameter get
			SetCase(PC_CMD_PARR, bIn==PC_ETX, UartRxPar(0);
					, 1, gwRxAddr,2);

//...............................................
// Paramter write process : Save parameter to EEPROM(or flash)
			SetCase(PC_CMD_EEPW, bIn==PC_ETX, UartTxParSave();
					, 0);

//...............................................
// Paramter read process : Read parameter from EEPROM(or flash)
			SetCase(PC_CMD_EEPR, bIn==PC_ETX, UartTxParRead();
					, 0);

#ifdef __ECM_STRING__
//...............................................
// Console process : Send CPU1
			case PC_CMD_STR:
				if (gptMsgShell.index != 0 && bIn == PC_ETX) {
					//UART_TX_ECM_DLY;
					hwflush_dcache_range((ulong)(intptr_t)gptMsgShell.arg[gptMsgShell.tail], MSG_SHELL_MAXLEN + 1);
					gptMsgShell.index = 0;
					num_loop(gptMsgShell.tail, gptMsgShell.tot_num);
					enx_wake_message_to_cpu0(eCPU0_MSG_SHELL_RX);
					SetEnd();
				} else {
					if (gptMsgShell.index == MSG_SHELL_MAXLEN) {
						UartTxStrHexCh(DEBUG_UART_NUM, "OverIn", bIn, 2);
						gptMsgShell.arg[gptMsgShell.tail][0]--;
					} else {
						gptMsgShell.arg[gptMsgShell.tail][gptMsgShell.index] = bIn;
						gptMsgShell.index++;
					}
				}
				break;
#endif
err_proc:
			default :
				UartTxErr();
				gbRxStg = 0;
				break;
		 }
	 }

	//gnCommOn = 0;
	//IRQ_ENABLE

}
#else
static char * const gUartNumcode = "0123456789ABCDEF";

int UartTxStr(UINT nCH, const char *str)
{
	int len = 0;
	char *pbuf = (char *)str;
	while (*pbuf) {
		if (*pbuf == '\n') {
			UartTx(nCH, '\r');
			len++;
		}
		UartTx(nCH, *pbuf++);
		len++;
	}
	return len;
}

static inline void UartTxNum(UINT nCH, BYTE data)
{
	UartTx(nCH, gUartNumcode[data]);
}

static inline void UartTxHex(UINT nCH, ULONG data, int size)
{
	for (int i = size * 8 - 4; i >= 0; i -= 4) {
		UartTxNum(nCH, (data >> i) & 0xf);
	}
}

void UartTxHexUchar(UINT nCH, BYTE data)
{
	UartTxHex(nCH, data, sizeof(BYTE));
}

void UartTxHexUshort(UINT nCH, USHORT data)
{
	UartTxHex(nCH, data, sizeof(USHORT));
}

void UartTxHexUint(UINT nCH, UINT data)
{
	UartTxHex(nCH, data, sizeof(UINT));
}

void UartTxHexUlong(UINT nCH, ULONG data)
{
	UartTxHex(nCH, data, sizeof(ULONG));
}

void UartTxDec(UINT nCH, ULONG data)
{
	UartTxNum(nCH, (data / 1000000000000000000) % 10);		// 1000000000000000000
	UartTxNum(nCH, (data / 100000000000000000) % 10);		// 100000000000000000
	UartTxNum(nCH, (data / 10000000000000000) % 10);		// 10000000000000000
	UartTxNum(nCH, (data / 1000000000000000) % 10);			// 1000000000000000
	UartTxNum(nCH, (data / 100000000000000) % 10);			// 100000000000000
	UartTxNum(nCH, (data / 10000000000000) % 10);			// 10000000000000
	UartTxNum(nCH, (data / 1000000000000) % 10);			// 1000000000000
	UartTxNum(nCH, (data / 100000000000) % 10);				// 100000000000
	UartTxNum(nCH, (data / 10000000000) % 10);				// 10000000000
	UartTxNum(nCH, (data / 1000000000) % 10);				// 1000000000
	UartTxNum(nCH, (data / 100000000) % 10);				// 100000000
	UartTxNum(nCH, (data / 10000000) % 10);					// 10000000
	UartTxNum(nCH, (data / 1000000) % 10);					// 1000000
	UartTxNum(nCH, (data / 100000) % 10);					// 100000
	UartTxNum(nCH, (data / 10000) % 10);					// 10000
	UartTxNum(nCH, (data / 1000) % 10);						// 1000
	UartTxNum(nCH, (data / 100) % 10);						// 100
	UartTxNum(nCH, (data / 10) % 10);						// 10
	UartTxNum(nCH, (data % 10));							// 1
}
#endif
#endif
