#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>

#define LOG_EMERG	"EMERGENCY!"
#define LOG_ALERT	"ALERT!"
#define LOG_CRIT	"critical!"
#define LOG_WARNING	"warning"
#define LOG_ERR	"error"
#define LOG_INFO	"info"
#define LOG_DEBUG	"debug"

#define OPEN_LOG(name) do {;} while(0)
#define CLOSE_LOG() do {;} while(0)

//#define DHCPD_DEBUG

#ifdef DHCPD_DEBUG
#define LOG(level, str, args...) do { printf("%s, ", level); \
				printf(str, ## args); \
				printf("\r\n"); } while(0)
# undef DHCPD_DEBUG
# define DEBUG(level, str, args...) LOG(level, str, ## args)
# define DEBUGGING
#else
# define LOG(level, str, args...)	do {;} while(0)
# define DEBUG(level, str, args...) do {;} while(0)
#endif

#endif
