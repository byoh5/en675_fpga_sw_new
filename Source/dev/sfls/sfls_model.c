#include "dev.h"
#include "sfls_model.h"

SFLScontrol* SflsModelEON(BYTE u8Type, BYTE u8Capa)
{
#ifdef __SFLS_EON__
	switch (u8Type) {
	case 0x30:	// QUAD
		switch (u8Capa) {
		case 0x16:	// EN25Q32
			printf("EN25Q32-noTEST\n");
			break;

		case 0x17:	// EN25Q64
			printf("EN25Q64-noTEST\n");
			break;

		case 0x18:	// EN25Q128
			printf("EN25Q128-noTEST\n");
			break;

		default:
			printf("EON Etc...\n");
			break;
	}
	break;

	case 0x70:	// new QUAD
		switch (u8Capa) {
		case 0x16:	// EN25QH32A
			printf("EN25QH32A-noTEST\n");
			break;

		case 0x17:	// EN25QH64A
			printf("EN25QH64A-noTEST\n");
			break;

		case 0x18:	// EN25QH128A
			printf("EN25QH128A-TESTing...\n");
#ifdef __SFLS_EN25QH128A__
			return SflsEn25qh128a_GetControl();
#else
			break;
#endif

		case 0x19:	// EN25QH256
			printf("EN25QH256-TESTing...\n");
#ifdef __SFLS_EN25QH256__
			return SflsEn25qh256_GetControl();
#else
			break;
#endif

		default:
			printf("EON Etc...\n");
			break;
		}
		break;

	default:
		printf("EON Etc...\n");
		break;
	}
#else
	ENX_ASSERT(0);
#endif
	return NULL;
}

SFLScontrol* SflsModelGigaDevice(BYTE u8Type, BYTE u8Capa)
{
#ifdef __SFLS_GIGADEVICE__
	switch (u8Type) {
	case 0x40:	//	QUAD/QPI
		switch (u8Capa) {
		case 0x18:	// GD25Q128C
			printf("GD25Q128C-TESTing...\n");
#ifdef __SFLS_GD25Q128C__
			return SflsGd25q128c_GetControl();
#else
			break;
#endif
		default:
			printf("Gigadevice Etc...\n");
			break;
		}
		break;
	default:
		printf("Gigadevice Etc...\n");
		break;
	}
#else
	ENX_ASSERT(0);
#endif
	return NULL;
}

SFLScontrol* SflsModelISSI(BYTE u8Type, BYTE u8Capa)
{
#ifdef __SFLS_ISSI__
	switch (u8Type) {
	case 0x60:	//	QUAD
		switch (u8Capa) {
		case 0x18:	// IS25LP128
			printf("IS25LP128-TESTing...\n");
#ifdef __SFLS_IS25LP128__
			return SflsIs25lp128_GetControl();
#else
			break;
#endif
		default:
			printf("ISSI Etc...\n");
			break;
		}
		break;
	default:
		printf("ISSI Etc...\n");
		break;
	}
#else
	ENX_ASSERT(0);
#endif
	return NULL;
}

SFLScontrol* SflsModelMacronix(BYTE u8Type, BYTE u8Capa)
{
#ifdef __SFLS_MACRONIX__
	printf("Macronix Etc...\n");
#else
	ENX_ASSERT(0);
#endif
	return NULL;
}

SFLScontrol* SflsModelMicron(BYTE u8Type, BYTE u8Capa)
{
#ifdef __SFLS_MICRON__
	switch (u8Type) {
	case 0xBA:	//	QUAD
		switch (u8Capa) {
		case 0x18:	// N25Q128A
			printf("N25Q128A-TESTing...\n");
#ifdef __SFLS_N25Q128A__
			return SflsN25q128a_GetControl();
#else
			break;
#endif
		default:
			printf("Micron Etc...\n");
			break;
		}
		break;
	default:
		printf("Micron Etc...\n");
		break;
	}
#else
	ENX_ASSERT(0);
#endif
	return NULL;
}

SFLScontrol* SflsModelWinbond(BYTE u8Type, BYTE u8Capa)
{
#ifdef __SFLS_WINBOND__
	switch (u8Type) {
	case 0x40:	//	QUAD
		switch (u8Capa) {
		case 0x17:	// W25Q64JV-IQ
			printf("W25Q64JVIQ-TESTing...\n");
#ifdef __SFLS_W25Q64__
			return SflsW25q64_GetControl();
#else
			break;
#endif
		case 0x18:	// W25Q128JV-IQ/JQ
			printf("W25Q128JVIQ/JQ-noTEST\n");
			break;
		default:
			printf("Winbond Etc...\n");
			break;
		}
		break;

	case 0x70:
		switch (u8Capa) {
		case 0x17:	// W25Q64JV-IM
			printf("W25Q64JVIM-noTEST\n");
			break;
		case 0x18:	// W25Q128JV-IM/JM
			printf("W25Q128JVIM/JM-noTEST\n");
			break;
		default:
			printf("Winbond Etc...\n");
			break;
		}
		break;

	default:
		printf("Winbond Etc...\n");
		break;
	}
#else
	ENX_ASSERT(0);
#endif
	return NULL;
}
