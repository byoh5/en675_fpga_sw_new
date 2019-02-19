#ifndef __PCF8563__H__
#define __PCF8563__H__

#ifdef __RTC_PCF8563__

/* I2C bus slave registers. */
#define PCF8563_I2C_READ        0xa3
#define PCF8563_I2C_WRITE       0xa2

/* Phillips PCF8563 registers. */
#define PCF8563_CONTROL1        0x00            /* Control/Status register 1. */
#define PCF8563_CONTROL2        0x01            /* Control/Status register 2. */
#define PCF8563_CLOCKOUT_FREQ   0x0d            /* CLKOUT frequency. */
#define PCF8563_TIMER_CONTROL   0x0e            /* Timer control. */
#define PCF8563_TIMER_CNTDOWN   0x0f            /* Timer countdown. */

/* BCD encoded clock registers. */
#define PCF8563_SECONDS         0x02
#define PCF8563_MINUTES         0x03
#define PCF8563_HOURS           0x04
#define PCF8563_DAY_OF_MONTH    0x05
#define PCF8563_WEEKDAY         0x06           /* Not coded in BCD! */
#define PCF8563_MONTH           0x07
#define PCF8563_YEAR            0x08
#define PCF8563_MINUTE_ALARM    0x09
#define PCF8563_HOUR_ALARM      0x0a
#define PCF8563_DAY_ALARM       0x0b
#define PCF8563_WEEKDAY_ALARM   0x0c

typedef struct {
    int exits;
    int twentyfourhour;
} pcf8563_t;

int pcf8563_init(void);

#endif
#endif