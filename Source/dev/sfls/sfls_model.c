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
#ifdef __SFLS_EN25QH128A__
			printf("EN25QH128A-RTL(200130_1405)-TEST\n");
			return SflsEn25qh128a_GetControl();
#else
			printf("EN25QH128A-Disabled\n");
			break;
#endif

		case 0x19:	// EN25QH256
#ifdef __SFLS_EN25QH256__
			printf("EN25QH256-RTL(200130_1405)-TEST\n");
			return SflsEn25qh256_GetControl();
#else
			printf("EN25QH256-Disabled\n");
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
#ifdef __SFLS_GD25Q128C__
			printf("GD25Q128C-RTL(200130_1405)-TEST\n");
			return SflsGd25q128c_GetControl();
#else
			printf("GD25Q128C-Disabled\n");
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
		case 0x17:	// IS25LP064
#ifdef __SFLS_IS25LP064__
			printf("IS25LP064-RTL(200130_1405)-TEST\n");
			return SflsIs25lp064_GetControl();
#else
			printf("IS25LP064-Disabled\n");
			break;
#endif
		case 0x18:	// IS25LP128
#ifdef __SFLS_IS25LP128__
			printf("IS25LP128-RTL(200130_1405)-TEST\n");
			return SflsIs25lp128_GetControl();
#else
			printf("IS25LP128-Disabled\n");
			break;
#endif
		case 0x19:	// IS25LP256
#ifdef __SFLS_IS25LP256__
			printf("IS25LP256-RTL(200130_1405)-TEST\n");
			return SflsIs25lp256_GetControl();
#else
			printf("IS25LP256-Disabled\n");
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
	switch (u8Type) {
	case 0x25:	//	QPI(1.8V)
		switch (u8Capa) {
		case 0x38:	// MX25U12832F
#ifdef __SFLS_MX25U12832F__
			printf("MX25U12832F-RTL(200118_1937)-TEST\n");
			return SflsMx25u12832f_GetControl();
#else
			printf("MX25U12832F-Disabled\n");
			break;
#endif
		default:
			printf("Macronix Etc...\n");
			break;
		}
		break;

	case 0x20:	//	QPI(3.3V)
		switch (u8Capa) {
		case 0x18:	// MX25L12835F
#ifdef __SFLS_MX25L12835F__
			printf("MX25L12835F-RTL(200130_1405)-TEST\n");
			return SflsMx25l12835f_GetControl();
#else
			printf("MX25L12835F-Disabled\n");
			break;
#endif
		default:
			printf("Macronix Etc...\n");
			break;
		}
		break;

	default:
		printf("Macronix Etc...\n");
		break;
	}
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
#ifdef __SFLS_N25Q128A__
			printf("N25Q128A-RTL(200130_1405)-TEST\n");
			return SflsN25q128a_GetControl();
#else
			printf("N25Q128A-Disabled\n");
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
#ifdef __SFLS_W25Q64JVIQ__
			printf("W25Q64JVIQ-RTL(200130_1405)-TEST\n");
			return SflsW25q64jviq_GetControl();
#else
			printf("W25Q64JVIQ-Disabled\n");
			break;
#endif
		case 0x18:	// W25Q128JV-IQ/JQ
			printf("W25Q128JVIQ/JQ-noTEST\n");
			break;
		case 0x19:	// W25Q128JV-IQ
#ifdef __SFLS_W25Q256JVQ__
			printf("W25Q256JVQ-RTL(200130_1405)-TEST\n");
			return SflsW25q256jvq_GetControl();
#else
			printf("W25Q256JVQ-Disabled\n");
			break;
#endif
		default:
			printf("Winbond Etc...\n");
			break;
		}
		break;

	case 0x70:	//	QPI
		switch (u8Capa) {
		case 0x17:	// W25Q64JV-IM
			printf("W25Q64JVIM-noTEST\n");
			break;
		case 0x18:	// W25Q128JV-IM/JM
			printf("W25Q128JVIM/JM-noTEST\n");
			break;
		case 0x19:	// W25Q256JV-IM
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
