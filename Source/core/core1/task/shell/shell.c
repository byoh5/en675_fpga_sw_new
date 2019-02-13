//*************************************************************************************************
// Shell task
//*************************************************************************************************
#include <string.h>

#include "dev.h"

#include "shell.h"
#include "shell_cmd.h"

#include "enx_freertos.h"

static char gcShellCmdBuf[100];
static char	gcHistoryBuf[100];

#define DEBUG_UART_NUM 7

//*************************************************************************************************
// Shell functions
//-------------------------------------------------------------------------------------------------
// Take the incoming string and create an argv[] array from that.
// The incoming string is assumed to be writeable.
// The argv[] array is simple a set of pointers into that string, where the whitespace delimited character sets are each NULL terminated.
int _tokenize(char *string, char *argv[])
{
	int	argc, done;

	/* Null out the incoming argv array. */
	for(argc=0; argc<ARGCNT; argc++)	argv[argc] = (char *)0;

	argc = 0;

	while(1){
		while( (*string==' ') || (*string=='\t') )		string++;

		if (*string==0)			break;

		argv[argc] = string;

		while( (*string!=' ') && (*string!='\t') ){
			if( (*string=='\\') && (*(string+1)=='"') ){
				strcpy(string,string+1);
			}
			else if(*string == '"'){
				strcpy(string,string+1);
				while(*string != '"'){
					if( (*string=='\\') && (*(string+1)=='"') )		strcpy(string,string+1);
					if(*string==0)		return(-1);
					string++;
				}

				strcpy(string,string+1);
				continue;
			}

			if(*string==0)		break;
			string++;
		}

		if(*string == 0){	done = 1;					}
		else			{	done = 0;	*string++ = 0;	}

		argc++;
		if(done)			break;

		if(argc >= ARGCNT){
			argc = -1;
			break;
		}
	}
	return(argc);
}

int _DoCommand(char *cmdline)
{
	int ret, argc;
	tMonCmd	*cmdptr;
	char *argv[ARGCNT], cmdcpy[CMDLINESIZE];

	if(cmdline[0]=='_')		cmdline++;
	if(cmdline[0]=='!')		strcpy(cmdline, gcHistoryBuf);

	strcpy(cmdcpy, cmdline);

	if(cmdline[0]!='!')		strcpy(gcHistoryBuf, cmdline);

	/* Build argc/argv structure based on incoming command line. */
	argc = _tokenize(cmdcpy, argv);

	if( (argc==0) && (argc<0) )		return (CMD_FAILURE);

	/* Step through the command table looking for a match between the first token of the command line and a command name. */
	cmdptr = gCmdList;

	if(argv[0]==NULL)				return (CMD_SUCCESS);

	while(cmdptr->name)
	{
		if(strcmp(argv[0], cmdptr->name) == 0)	break;
		cmdptr++;

		if(cmdptr->name == 0)		return (CMD_LINE_ERROR);
	}

//	if(strcmp("md", cmdptr->name) != 0)		_printf("%s\n", *cmdptr->helphelp);		// for fast memory display

	ret = cmdptr->func(argc, argv);

	_printf("\n");
	return(ret);
}

//-------------------------------------------------------------------------------------------------
//
int _getline(char *buf, int max, int timeout)
{
	static char crlf;
	int total, idx;
	char *base;
	unsigned char ck;
	
	total = idx = 0;
	base = buf;
	max -= 1;												// Make sure there is space for the null terminator

	for(idx=0; idx<max; idx++){

		//while(1) { vTaskDelay(1); }

		while(UartRxExist(DEBUG_UART_NUM) == 0)			vTaskDelay(10);

//		*buf = (char)UartGetByte();
		ck = UartGetByte(DEBUG_UART_NUM);
		*buf = ck;
		//_printf("[%X]", ck);
		if(!*buf){
			idx--;
			return 0;
		}

		if ( (*buf=='\r') || (*buf=='\n') ){
			if(crlf && (*buf != crlf))
				crlf = 0;
			UartTx(DEBUG_UART_NUM, '\r');
			UartTx(DEBUG_UART_NUM, '\n');
		    crlf = *buf;
		    *buf = 0;
		    break;
		}

		if (*buf=='\b'){
			if(total){
				idx -=2;
				buf--;
				total--;
				UartTx(DEBUG_UART_NUM, '\b');
				UartTx(DEBUG_UART_NUM, ' ');
				UartTx(DEBUG_UART_NUM, '\b');
			}
		}
		else if(*buf==CTLC){
			UartTx(DEBUG_UART_NUM, '^');
			UartTx(DEBUG_UART_NUM, 'C');
			UartTx(DEBUG_UART_NUM, '\r');
			UartTx(DEBUG_UART_NUM, '\n');
			*base = 0;
			return 0;
		}
		else{
			UartTx(DEBUG_UART_NUM, *buf);
			total++;
			buf++;
		}

		crlf = 0;

		if (idx>=max){
			_printf("\nInput too long (exceeds %d bytes).\n",max+1);
			*buf = 0;
			return 0;
		}
	}

	return(strlen(base));
	UNUSED(timeout);
}

//*************************************************************************************************
// Shell task
//-------------------------------------------------------------------------------------------------
//
void ShellTask(void* pvParameters)
{
	vTaskDelay(500);

	while(1)
	{
		_printf(PROMPT);
		_getline(gcShellCmdBuf, CMDLINESIZE, 0); // input IDLE...
		if(_DoCommand(gcShellCmdBuf) == CMD_LINE_ERROR)
		{
			_printf("Bad or not command! : [%s]\n", gcShellCmdBuf);
		}
	}

	UNUSED(pvParameters);
}

