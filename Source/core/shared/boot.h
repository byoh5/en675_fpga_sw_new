/*
 * boot.h
 *
 *  Created on: 2018. 11. 29.
 *      Author: BYOH
 */

#ifndef BOOT_H_
#define BOOT_H_

//******************************************************************************
// Code Memory Infomation
//------------------------------------------------------------------------------
typedef enum { 	CS_LMA=0,
				CS_RESET_LMA=1,	CS_TEXT_LMA, 	CS_ISPM_TEXT_LMA, 	CS_ISPM_DATA_LMA, 	CS_RODATA_LMA,	CS_DATA_LMA,
				CS_RESET=7, 	CS_TEXT, 		CS_ISPM_TEXT, 		CS_ISPM_DATA, 		CS_RODATA, 		CS_DATA,  		// : Code Sections
				CS_BSS,			CS_STACK
} eCodeSectionIndex;

#define	CODE_SECTION_IDX(NAME) 		CS_ ## NAME
#define	CODE_SECTION_LMA_IDX(NAME)	CS_ ## NAME ##_LMA
#define	CS_NSECTIONS				(CS_RESET-CS_RESET_LMA)
#define	VMA_TO_LMA(VMA_ID)			((VMA_ID)-CS_NSECTIONS)

typedef union {
	unsigned int info[2];
	struct {
		unsigned int s, e;
	};
} tCodeSectionInfo;

typedef union {
	tCodeSectionInfo section[1+2*CS_NSECTIONS];
	struct {
		tCodeSectionInfo	lma;
		tCodeSectionInfo 	reset_lma, 	text_lma, 	ispm_text_lma, 	ispm_data_lma, 	rodata_lma,	data_lma;
		tCodeSectionInfo	reset,		text,		ispm_text,		ispm_data,		rodata, 	data;
		tCodeSectionInfo	bss, 		stack;
	};
} tCodeMemInfo;

#define BOOT_TRACE printf

#define PRINT_CODE_STR(CODE)			BOOT_TRACE("< %8s > %8s %8s %8s",CODE,"VMA-S","VMA-E","LENGTH");
#define PRINT_CODE_INFO(NAME)			BOOT_TRACE("|%10s: %08x %08x %08x",#NAME, pCodeMemInfo->NAME.s, pCodeMemInfo->NAME.e,pCodeMemInfo->NAME.e-pCodeMemInfo->NAME.s);
#define PRINT_STACK_INFO(NAME)			BOOT_TRACE("|%10s: %08x %08x %08x",#NAME, pCodeMemInfo->NAME.s, pCodeMemInfo->NAME.e,pCodeMemInfo->NAME.s-pCodeMemInfo->NAME.e+4);

#define PRINT_CODE_LMA_STR(CODE)		PRINT_CODE_STR(CODE); 	BOOT_TRACE(" : %8s %8s","LMA-S","LMA-E");
#define PRINT_CODE_LMA_INFO(NAME)		PRINT_CODE_INFO(NAME); 	BOOT_TRACE(" : %08x %08x", pCodeMemInfo->NAME ## _lma.s, pCodeMemInfo->NAME ## _lma.e);

#define PRINT_CODE_LFMA_STR(CODE)		PRINT_CODE_LMA_STR(CODE); 	BOOT_TRACE(" : %8s %8s\n","FMA-S","FMA-E");
#define PRINT_CODE_LFMA_INFO(NAME,BASE)	PRINT_CODE_LMA_INFO(NAME); 	BOOT_TRACE(" : %08x %08x\n",  ((unsigned int)BASE) + pCodeMemInfo->NAME ## _lma.s, ((unsigned int)BASE) + pCodeMemInfo->NAME ## _lma.e);


#define	PRINT_CODE_MEM_INFO(CODE,BASE)	PRINT_CODE_LFMA_STR(CODE);					\
										PRINT_CODE_INFO(lma); BOOT_TRACE("\n");   		\
										PRINT_CODE_LFMA_INFO(reset,BASE);      		\
										PRINT_CODE_LFMA_INFO(text,BASE);       		\
										PRINT_CODE_LFMA_INFO(rodata,BASE);      	\
										PRINT_CODE_LFMA_INFO(data,BASE);       		\
										PRINT_CODE_LFMA_INFO(ispm_text,BASE);  		\
										PRINT_CODE_LFMA_INFO(ispm_data,BASE);  		\
										PRINT_CODE_INFO(bss); BOOT_TRACE("\n");   		\
										PRINT_STACK_INFO(stack); BOOT_TRACE("\n\n");




#define STRINGIFY(X) 	#X
#define	XSTRINGIFY(X) 	STRINGIFY(X)


//******************************************************************************
//
//------------------------------------------------------------------------------

void _init_bss_section(void);
void _init_text_section(void);
void _init_data_section(void);

#endif /* BOOT_H_ */
