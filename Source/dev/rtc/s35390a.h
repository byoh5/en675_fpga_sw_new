#ifndef __S35390A__H__
#define __S35390A__H__

#ifdef __RTC_S35390A__

/* I2C bus slave code */
#define S35390A_I2C_READ            0x61
#define S35390A_I2C_WRITE           0x60

/* Command */
#define S35390A_CMD_STATUS1         0x00
#define S35390A_CMD_STATUS2         0x01
#define S35390A_CMD_TIME1           0x02
#define S35390A_CMD_TIME2           0x03
#define S35390A_CMD_INT2_REG1       0x05

/* BCD encoded clock registers */
#define S35390A_BYTE_YEAR           0x00
#define S35390A_BYTE_MONTH          0x01
#define S35390A_BYTE_DAY            0x02
#define S35390A_BYTE_WDAY           0x03
#define S35390A_BYTE_HOURS          0x04
#define S35390A_BYTE_MINS           0x05
#define S35390A_BYTE_SECS           0x06

/* Alrm */
#define S35390A_ALRM_BYTE_WDAY      0x00
#define S35390A_ALRM_BYTE_HOURS     0x01
#define S35390A_ALRM_BYTE_MINS      0x02

/* Flag */
#define S35390A_FLAG_TEST           0x01
#define S35390A_FLAG_POC            0x01
#define S35390A_FLAG_BLD            0x02
#define S35390A_FLAG_24H            0x40
#define S35390A_FLAG_RESET          0x80

/* INT2 */
#define S35390A_INT2_MODE_NOINTR    0x00
#define S35390A_INT2_MODE_FREQ      0x10
#define S35390A_INT2_MODE_PMIN_EDG  0x20
#define S35390A_INT2_MODE_ALARM     0x40
#define S35390A_INT2_MODE_MASK      0xF0

typedef struct {
    int exits;
    int twentyfourhour;
} s35390a_t;

int s35390a_init(void);

#endif
#endif
