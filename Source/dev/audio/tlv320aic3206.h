#ifndef __TLV320AIc3206_H__
#define __TLV320AIc3206_H__

#define TLV320AIC3206_I2C_WRITE	AUDIO_TLV320AIC3206_I2C_CA
#define TLV320AIC3206_I2C_READ	(TLV320AIC3206_I2C_WRITE | 1)

extern void tlv320aic3206_print_name(void);
extern void tlv320aic3206_init(void);

#endif
