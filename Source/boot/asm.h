/*
 * asm.h
 *
 *  Created on: 2018. 11. 30.
 *      Author: BYOH
 */

#ifndef ASM_H_
#define ASM_H_

#define	ENTRY(symbol)					 \
	.global	symbol						;\
symbol:

#define	WORD_ENTRY(symbol)				 \
	ENTRY(symbol)						;\
	.word	_##symbol



#endif /* ASM_H_ */
