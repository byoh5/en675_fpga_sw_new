#include "dev.h"

#include "lwip/apps/httpd_opts.h"

#if LWIP_HTTPD_SUPPORT_AUTHORIZATION
#include <string.h>
#include "httpd_authorization.h"

#if (LWIP_HTTPD_SUPPORT_AUTHORIZATION==LWIP_HTTPD_AUTH_BASIC)
#include "Base64.h"
int http_auth_process(int isPost, char *data, int data_len)
{
	int nRes = DEF_FAIL;
	char *datacheck = strnstr(data, HTTP11_AUTORIZATION_TEXT, data_len);
	if (datacheck == NULL) {
		return nRes;
	}
	datacheck += sizeof(HTTP11_AUTORIZATION_TEXT) - 1;

	int base64len = 0, index = strcspn(datacheck, CRLF);
	char *strHeaderfield = (char *)pvPortMalloc((size_t)index + 1);
	if (strHeaderfield == NULL) {
		flprintf("Malloc Fail\n");
	} else {
		strncpy(strHeaderfield, datacheck, index);
		strHeaderfield[index] = 0;

		// decode Base64 => ID:PW
		char *baseDecodeString = (char *)base64_decode(strHeaderfield, index, (size_t *)&base64len);
		if (baseDecodeString == NULL) {
			flprintf("Malloc fail\n");
		} else {
			char *id = baseDecodeString;
			char *colon = strchr(baseDecodeString, ':');
			char *pw = colon + 1;
			colon[0] = '\0';
			nRes = UserPasswordCheck(id, pw);

			vPortFree(baseDecodeString);
		}

		vPortFree(strHeaderfield);
	}

	return nRes;

	UNUSED(isPost);
}
#elif (LWIP_HTTPD_SUPPORT_AUTHORIZATION==LWIP_HTTPD_AUTH_DIGEST)
#include "md5.h"

#define ENX_HTTP_AUTH_strUSERNAME	"username"
#define ENX_HTTP_AUTH_strREALM		"realm"
#define ENX_HTTP_AUTH_strNONCE		"nonce"
#define ENX_HTTP_AUTH_strURI		"uri"
#define ENX_HTTP_AUTH_strRESPONSE	"response"
#define ENX_HTTP_AUTH_strALGORITHM	"algorithm"
#define ENX_HTTP_AUTH_strQOP		"qop"
#define ENX_HTTP_AUTH_strCNONCE		"cnonce"
#define ENX_HTTP_AUTH_strNC			"nc"
#define ENX_HTTP_AUTH_strOPAQUE		"opaque"

enum {
	ENX_HTTP_AUTH_numUSERNAME,	// 0
	ENX_HTTP_AUTH_numREALM,		// 1
	ENX_HTTP_AUTH_numNONCE,		// 2
	ENX_HTTP_AUTH_numURI,		// 3
	ENX_HTTP_AUTH_numRESPONSE,	// 4
	ENX_HTTP_AUTH_numALGORITHM,	// 5
	ENX_HTTP_AUTH_numQOP,		// 6
	ENX_HTTP_AUTH_numCNONCE,	// 7
	ENX_HTTP_AUTH_numNC,		// 8
	ENX_HTTP_AUTH_numOPAQUE,	// 9
	ENX_HTTP_AUTH_numNONE,
};

typedef struct {
	int checked;
	char *data;
} http_auth_header;

typedef struct {
	char *strHeaderfield;
	http_auth_header rahList[ENX_HTTP_AUTH_numNONE];
} http_auth_headerlist;

static char *http_auth_del_quotes(char *temp)
{
	temp = strchr(temp, '=') + 1;
	if (temp) {
		char *temp2 = strchr(temp, '\"');
		if (temp2) {
			temp = temp2 + 1;
			temp2 = strchr(temp2 + 1, '\"');
			temp2[0] = '\0';
		}
	}
	return temp;
}

int http_auth_process(int isPost, char *data, int data_len)
{
	int nRes = DEF_FAIL;
	char *datacheck = strnstr(data, HTTP11_AUTORIZATION_TEXT, data_len);
	if (datacheck == NULL) {
		return nRes;
	}

	http_auth_headerlist hahItem;
	memset(&hahItem, 0, sizeof(http_auth_headerlist));
	int index = strcspn(datacheck, CRLF);
	hahItem.strHeaderfield = (char *)pvPortMalloc((size_t)index + 1);
	if (hahItem.strHeaderfield == NULL) {
		flprintf("Malloc Fail\n");
	} else {
		strncpy(hahItem.strHeaderfield, datacheck, index);
		hahItem.strHeaderfield[index] = 0;

		char *temp = NULL, *strHeader = strtok(hahItem.strHeaderfield, ",");
		while (strHeader != NULL) {
			if ((hahItem.rahList[ENX_HTTP_AUTH_numUSERNAME].checked == 0) && (temp = strstr(strHeader, ENX_HTTP_AUTH_strUSERNAME))) {
				hahItem.rahList[ENX_HTTP_AUTH_numUSERNAME].checked = 1;
				hahItem.rahList[ENX_HTTP_AUTH_numUSERNAME].data = http_auth_del_quotes(temp);
			} else if ((hahItem.rahList[ENX_HTTP_AUTH_numREALM].checked == 0) && (temp = strstr(strHeader, ENX_HTTP_AUTH_strREALM))) {
				hahItem.rahList[ENX_HTTP_AUTH_numREALM].checked = 1;
				hahItem.rahList[ENX_HTTP_AUTH_numREALM].data = http_auth_del_quotes(temp);
			} else if ((hahItem.rahList[ENX_HTTP_AUTH_numNONCE].checked == 0) && (temp = strstr(strHeader, ENX_HTTP_AUTH_strNONCE))) {
				hahItem.rahList[ENX_HTTP_AUTH_numNONCE].checked = 1;
				hahItem.rahList[ENX_HTTP_AUTH_numNONCE].data = http_auth_del_quotes(temp);
			} else if ((hahItem.rahList[ENX_HTTP_AUTH_numURI].checked == 0) && (temp = strstr(strHeader, ENX_HTTP_AUTH_strURI))) {
				hahItem.rahList[ENX_HTTP_AUTH_numURI].checked = 1;
				hahItem.rahList[ENX_HTTP_AUTH_numURI].data = http_auth_del_quotes(temp);
			} else if ((hahItem.rahList[ENX_HTTP_AUTH_numRESPONSE].checked == 0) && (temp = strstr(strHeader, ENX_HTTP_AUTH_strRESPONSE))) {
				hahItem.rahList[ENX_HTTP_AUTH_numRESPONSE].checked = 1;
				hahItem.rahList[ENX_HTTP_AUTH_numRESPONSE].data = http_auth_del_quotes(temp);
			} else if ((hahItem.rahList[ENX_HTTP_AUTH_numALGORITHM].checked == 0) && (temp = strstr(strHeader, ENX_HTTP_AUTH_strALGORITHM))) {
				hahItem.rahList[ENX_HTTP_AUTH_numALGORITHM].checked = 1;
				hahItem.rahList[ENX_HTTP_AUTH_numALGORITHM].data = http_auth_del_quotes(temp);
			} else if ((hahItem.rahList[ENX_HTTP_AUTH_numQOP].checked == 0) && (temp = strstr(strHeader, ENX_HTTP_AUTH_strQOP))) {
				hahItem.rahList[ENX_HTTP_AUTH_numQOP].checked = 1;
				hahItem.rahList[ENX_HTTP_AUTH_numQOP].data = http_auth_del_quotes(temp);
			} else if ((hahItem.rahList[ENX_HTTP_AUTH_numCNONCE].checked == 0) && (temp = strstr(strHeader, ENX_HTTP_AUTH_strCNONCE))) {
				hahItem.rahList[ENX_HTTP_AUTH_numCNONCE].checked = 1;
				hahItem.rahList[ENX_HTTP_AUTH_numCNONCE].data = http_auth_del_quotes(temp);
			} else if ((hahItem.rahList[ENX_HTTP_AUTH_numNC].checked == 0) && (temp = strstr(strHeader, ENX_HTTP_AUTH_strNC))) {
				hahItem.rahList[ENX_HTTP_AUTH_numNC].checked = 1;
				hahItem.rahList[ENX_HTTP_AUTH_numNC].data = http_auth_del_quotes(temp);
			} else if ((hahItem.rahList[ENX_HTTP_AUTH_numOPAQUE].checked == 0) && (temp = strstr(strHeader, ENX_HTTP_AUTH_strOPAQUE))) {
				hahItem.rahList[ENX_HTTP_AUTH_numOPAQUE].checked = 1;
				hahItem.rahList[ENX_HTTP_AUTH_numOPAQUE].data = http_auth_del_quotes(temp);
			} else {
				flprintf("unknown header [%s]\n", strHeader);
			}
			strHeader = strtok(NULL, ",");
		}

#if 0
		int i;
		for (i = 0; i < ENX_HTTP_AUTH_numNONE; i++) {
			if (i == ENX_HTTP_AUTH_numUSERNAME) printf("%10s", ENX_HTTP_AUTH_strUSERNAME);
			if (i == ENX_HTTP_AUTH_numREALM) printf("%10s", ENX_HTTP_AUTH_strREALM);
			if (i == ENX_HTTP_AUTH_numNONCE) printf("%10s", ENX_HTTP_AUTH_strNONCE);
			if (i == ENX_HTTP_AUTH_numURI) printf("%10s", ENX_HTTP_AUTH_strURI);
			if (i == ENX_HTTP_AUTH_numRESPONSE) printf("%10s", ENX_HTTP_AUTH_strRESPONSE);
			if (i == ENX_HTTP_AUTH_numALGORITHM) printf("%10s", ENX_HTTP_AUTH_strALGORITHM);
			if (i == ENX_HTTP_AUTH_numCNONCE) printf("%10s", ENX_HTTP_AUTH_strCNONCE);
			if (i == ENX_HTTP_AUTH_numNC) printf("%10s", ENX_HTTP_AUTH_strNC);
			if (i == ENX_HTTP_AUTH_numQOP) printf("%10s", ENX_HTTP_AUTH_strQOP);
			if (i == ENX_HTTP_AUTH_numOPAQUE) printf("%10s", ENX_HTTP_AUTH_strOPAQUE);
			printf(" Idx:%d Ck:%d Txt:[%s]\n", i, hahItem.rahList[i].checked, hahItem.rahList[i].data);
		}
#endif

		char HA1[33] = {0}, HA2[33] = {0}, HRes[33] = {0};
		BYTE hash[16] = {0};
		do {
			// Hash A1 = hash(id:REALM:pw)
			memcpy(HA1, (void *)gtNetwork.hashUserPw, 32);

			// Hash A2 = hash(METHOD:uri)
			MD5_CTX mdStep2;
			MD5Init(&mdStep2);
			if (isPost == 1) {
				MD5Update(&mdStep2, (unsigned char *)"POST:", 5);
			} else {
				MD5Update(&mdStep2, (unsigned char *)"GET:", 4);
			}
			MD5Update(&mdStep2, (unsigned char *)hahItem.rahList[ENX_HTTP_AUTH_numURI].data, strlen(hahItem.rahList[ENX_HTTP_AUTH_numURI].data));
			MD5Final(hash, &mdStep2);
			Hex2Str32B(mdStep2.digest, HA2);

			// Hash Response = hash(Hash A1:NONCE:NC:CNONCE:QOP:Hash A2)
			MD5_CTX mdStep3;
			MD5Init(&mdStep3);
			MD5Update(&mdStep3, (unsigned char *)HA1, sizeof(HA1) - 1);
			MD5Update(&mdStep3, (unsigned char *)":", 1);
			MD5Update(&mdStep3, (unsigned char *)hahItem.rahList[ENX_HTTP_AUTH_numNONCE].data, strlen(hahItem.rahList[ENX_HTTP_AUTH_numNONCE].data));
			MD5Update(&mdStep3, (unsigned char *)":", 1);
			MD5Update(&mdStep3, (unsigned char *)hahItem.rahList[ENX_HTTP_AUTH_numNC].data, strlen(hahItem.rahList[ENX_HTTP_AUTH_numNC].data));
			MD5Update(&mdStep3, (unsigned char *)":", 1);
			MD5Update(&mdStep3, (unsigned char *)hahItem.rahList[ENX_HTTP_AUTH_numCNONCE].data, strlen(hahItem.rahList[ENX_HTTP_AUTH_numCNONCE].data));
			MD5Update(&mdStep3, (unsigned char *)":", 1);
			MD5Update(&mdStep3, (unsigned char *)hahItem.rahList[ENX_HTTP_AUTH_numQOP].data, strlen(hahItem.rahList[ENX_HTTP_AUTH_numQOP].data));
			MD5Update(&mdStep3, (unsigned char *)":", 1);
			MD5Update(&mdStep3, (unsigned char *)HA2, sizeof(HA2) - 1);
			MD5Final(hash, &mdStep3);
			Hex2Str32B(mdStep3.digest, HRes);
		} while(0);

		if (memcmp(HRes, hahItem.rahList[ENX_HTTP_AUTH_numRESPONSE].data, sizeof(HRes)) == 0) {
			nRes = DEF_OK;
		}

		vPortFree(hahItem.strHeaderfield);
	}

	return nRes;
}
#endif
#endif // LWIP_HTTPD_SUPPORT_AUTHORIZATION
