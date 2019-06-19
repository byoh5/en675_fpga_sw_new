#include "dev.h"

#if 1

ISRD static tIhnd arrBUSIrq;

typedef enum {
	axiFRC = 0x00,
	axiWDR = 0x02,
	axiCVBSEnc = 0x4,
	axiJPEGEnc = 0x6,
	axiDImg0Y = 0x8,
	axiDImg0C = 0xA,
	axiDImg1Y = 0xC,
	axiDImg1C = 0xE,
	axiDImg2Y = 0x10,
	axiDImg2C = 0x12,
	axiDImg3Y = 0x14,
	axiDImg3C = 0x16,
	axiDImg4Y = 0x18,
	axiDImg4C = 0x1A,
	axiDImg5Y = 0x1C,
	axiDImg5C = 0x1E,
	axiDImg6Y = 0x20,
	axiDImg6C = 0x22,
	axiCPU = 0x40,
	axiBDMA = 0x48,
	axiSDIO0 = 0x50,
	axiSDIO1 = 0x54,
	axiETH = 0x58,
	axiAUD = 0x60,
	axiAES = 0x68,
	axiUSB = 0x70,
} axiID;

typedef struct {
	char name[12];
	BYTE bus_rw;
	BYTE bus_id_width;
	BYTE bus_id;
} axiIDTable;



void BusOutput(void *ctx)
{
#if 1
//	MCU WR:00000MB/s, IO/s:0000000 RD:00000MB/s, IO/s:0000000
//	CPU WR:00000MB/s, IO/s:0000000 RD:00000MB/s, IO/s:0000000
//	DMA WR:00000MB/s, IO/s:0000000 RD:00000MB/s, IO/s:0000000

	//printf("mstatus(0x%16lX)\n", read_csr(mstatus));

	if (BUS_ACT_BYTE0 >= 1000000)	printf("MCU WR:%5uM",BUS_ACT_BYTE0/1000000);
	else if (BUS_ACT_BYTE0 >= 1000)	printf("MCU WR:%5uK",BUS_ACT_BYTE0/1000);
	else							printf("MCU WR:%5u ",BUS_ACT_BYTE0);
	printf("B/s, IO/s:%7u ", BUS_IO_CNT0);

	if (BUS_ACT_BYTE1 >= 1000000)	printf("RD:%5uM", BUS_ACT_BYTE1/1000000);
	else if (BUS_ACT_BYTE1 >= 1000)	printf("RD:%5uK", BUS_ACT_BYTE1/100);
	else							printf("RD:%5u ", BUS_ACT_BYTE1);
	printf("B/s, IO/s:%7u | ", BUS_IO_CNT1);

	if (BUS_ACT_BYTE2 >= 1000000)	printf("CPU WR:%5uM", BUS_ACT_BYTE2/1000000);
	else if (BUS_ACT_BYTE2 >= 1000)	printf("CPU WR:%5uK", BUS_ACT_BYTE2/100);
	else							printf("CPU WR:%5u ", BUS_ACT_BYTE2);
	printf("B/s, IO/s:%7u ", BUS_IO_CNT2);

	if (BUS_ACT_BYTE3 >= 1000000)	printf("RD:%5uM",BUS_ACT_BYTE3/1000000);
	else if (BUS_ACT_BYTE3 >= 1000)	printf("RD:%5uK",BUS_ACT_BYTE3/100);
	else							printf("RD:%5u ",BUS_ACT_BYTE3);
	printf("B/s, IO/s:%7u | ", BUS_IO_CNT3);

	if (BUS_ACT_BYTE4 >= 1000000)	printf("ETH WR:%5uM",BUS_ACT_BYTE4/1000000);
	else if (BUS_ACT_BYTE4 >= 1000)	printf("ETH WR:%5uK",BUS_ACT_BYTE4/1000);
	else							printf("ETH WR:%5u ",BUS_ACT_BYTE4);
	printf("B/s, IO/s:%7u ", BUS_IO_CNT4);

	if (BUS_ACT_BYTE5 >= 1000000)	printf("RD:%5uM",BUS_ACT_BYTE5/1000000);
	else if (BUS_ACT_BYTE5 >= 1000)	printf("RD:%5uK",BUS_ACT_BYTE5/1000);
	else							printf("RD:%5u ",BUS_ACT_BYTE5);
	printf("B/s, IO/s:%7u\n", BUS_IO_CNT5);
#else
	printf("SP[0x%08X] gptMsgShare.UPTIME: %lus\n", sp, gptMsgShare.UPTIME);
	printf("BUS_ACT_BYTE0(%u Byte) BUS_IO_CNT0(cnt: %u)\n", BUS_ACT_BYTE0, BUS_IO_CNT0);
	printf("BUS_ACT_BYTE1(%u Byte) BUS_IO_CNT1(cnt: %u)\n", BUS_ACT_BYTE1, BUS_IO_CNT1);
#endif
}

void BusInit(void)
{
#if 1
	BUS_EN = 1;

	BUS_RW0 = 1;
	BUS_ID_WIDTH0 = 0;
	BUS_ID0 = 0x00;
	BUS_EN0 = 1;

	BUS_RW1 = 0;
	BUS_ID_WIDTH1 = 0;
	BUS_ID1 = 0x00;
	BUS_EN1 = 1;

	BUS_RW2 = 1;
	BUS_ID_WIDTH2 = 4;
	BUS_ID2 = 0x40;
	BUS_EN2 = 1;

	BUS_RW3 = 0;
	BUS_ID_WIDTH3 = 4;
	BUS_ID3 = 0x40;
	BUS_EN3 = 1;

	UINT id = 0x58;

	BUS_RW4 = 1;
	BUS_ID_WIDTH4 = 4;
	BUS_ID4 = id;
	BUS_EN4 = 1;

	BUS_RW5 = 0;
	BUS_ID_WIDTH5 = 4;
	BUS_ID5 = id;
	BUS_EN5 = 1;

#else
	BUS_EN = 1;
	BUS_IRQ_EN = 1;
	BUS_IRQ_PERIOD = MCK_FREQ;

	BUS_RW0 = 0; // read check
	BUS_ID_WIDTH0 = 0;
	BUS_ID0 = 0x00;
	BUS_EN0 = 1;

	BUS_RW1 = 1; // write check
	BUS_ID_WIDTH1 = 0;
	BUS_ID1 = 0x00;
	BUS_EN1 = 1;
#endif

	BusSetIrqPeriod(1000);
	BusIrqCallback(BusOutput, NULL);
	BusSetIrqEn(ENX_ON);
}

void BusSetIrqPeriod(UINT msec)
{
	if (msec == 0) {
		msec = 1000;
	}
	BUS_IRQ_PERIOD = (MCK_FREQ / 1000) * msec;
}

UINT BusGetIrqPeriod(void)
{
	return BUS_IRQ_PERIOD / (MCK_FREQ / 1000);
}

void BusIrqCallback(irq_fn irqfn, void *arg)
{
	arrBUSIrq.irqfn = irqfn;
	arrBUSIrq.arg = arg;
}

void BusSetIrqEn(ENX_SWITCH sw)
{
	BUS_IRQ_EN = sw;
}

ENX_SWITCH BusGetIrqEn(void)
{
	return BUS_IRQ_EN;
}

void BusIrqClear(void)
{
	BUS_IRQ_CLR = 1;
}

UINT BusIsIrq(void)
{
	return BUS_IRQ;
}

void IrqBus(void)
{
	if (BusIsIrq()) {
		printf("BUS IRQ Get\n");
		if (arrBUSIrq.irqfn) {
			arrBUSIrq.irqfn(arrBUSIrq.arg);
		}
		BusIrqClear();
	}
}
#else
void IrqBus(void)
{
	_Rprintf("Bus IRQ Get! Bus is inactive.\n");
	ENX_ASSERT(0);
}
#endif
