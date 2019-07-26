#include "includes.h"
#include "common.h"
#include "config.h"
#include "wpa.h"

//const char *con_ssid = "iptime_enx";
//const char *con_ssid = "netgear123";
//const char *con_psk  = "42b4e8a53b202f8ee7965fc51e037e48dc6b403ee3290aebcb049f0c020240a2";
const char *con_ssid = "iptime_enx";
const char *con_psk  = "0906804203548335cd350825905f2ab2c94a8936b44c35654c374d4b6f47c63d";

/*
const char *cfg_ssid = "update_config=1\n"
                       "network={\n"
                        "        ssid=\"netgear33\" \n"
                        "        key_mgmt=NONE\n"
                        "}\n";
 


const char *cfg_ssid = "update_config=1\n"
                      "network={\n"
                       "        ssid=\"iptime_enx\" \n"
                       "        key_mgmt=WPA-PSK\n"
                       "        psk=\"sysfo_eyenix\"\n"
                       "}\n";  

const char *cfg_ssid = "update_config=1\n"
                       "network={\n"
                        "        ssid=\"netgear33\" \n"
                        "        key_mgmt=NONE\n"
                        "        wep_key0=759478362c\n"
                        "        wep_tx_keyidx=0\n"
                        "        priority=1\n"
                        "        auth_alg=SHARED\n"
                        "}\n";                     
const char *cfg_ssid = "update_config=1\n"
                      "network={\n"
                       "        ssid=\"iptime_enx\" \n"
                       "        key_mgmt=WPA-PSK\n"
                       "        psk=\"sysfo_eyenix\"\n"
                       "}\n";    


*/
 

const char *cfg_ssid = "update_config=1\n"
                       "network={\n"
                        "        ssid=\"netgear33\" \n"
                        "        key_mgmt=NONE\n"
                        "        wep_key0=759478362c\n"
                        "        wep_tx_keyidx=0\n"
                        "        priority=1\n"
                        "        auth_alg=SHARED\n"
                        "}\n"; 




struct ssid_keyword {
	char keyword[14];
	char def[30];
};

struct ssid_network
{
    struct ssid_keyword keywords[7];
};


#if 0
struct ssid_network config_ssid =
{
    .ssid = {ssid.
};
#endif

struct buf_stream{
    int   code; /*maic code */
    int   size;
    int   pos;
    char *buf;
};

/*
    read a line from buffer
    
    @s    : buffer to be read, and null terminated
    @length : length of s
    @stream : stream struct. 
*/

static int 
bgets(char *s, int size, struct buf_stream *stream)
{
    int i;
    
retry:    
    for(i=0; i<size; i++)
    {
        if(stream->buf[stream->pos] == '\n')
        {
            stream->pos++;
            break;
        }    
            
        s[i] = stream->buf[stream->pos++];
        if(stream->pos >= stream->size) /* is end of config ? */
            break;
    }
    s[i] = 0;
    if(i == 0 && stream->pos < stream->size) 
        goto retry;
    return i; 
}

static struct buf_stream *
alloc_buf_stream(int size)
{
    struct buf_stream  *b_stream;

    
    b_stream = os_zalloc(sizeof(*b_stream)+size);

    if(b_stream == NULL) 
        return NULL;
    b_stream->buf = (char*)b_stream+sizeof(*b_stream);
    b_stream->size = size;
    b_stream->code = 0x11223344; /*wpa supplicant cfg code */
    return b_stream;
}

static void free_buf_stream(struct buf_stream *b)
{
    os_free(b);
}

static 
int bprintf(struct buf_stream *f, const char *fmt, ...)
{
    va_list args;
    int n;
    
    va_start(args, fmt);
    n = vsprintf(&(f->buf[f->pos]), fmt, args);
    va_end(args);
    f->pos += n;
    f->size = f->pos;
    return n;
}


static void 
write_str(struct buf_stream *f, const char *field, struct wpa_ssid *ssid)
{
	char *value = wpa_config_get(ssid, field);
	if (value == NULL)
		return;
	bprintf(f, "\t%s=%s\n", field, value);
	os_free(value);
}

int buf_stream_flash_write(struct buf_stream *f)
{
#if 0
    struct buf_stream *b=alloc_buf_stream(1024);
#endif    
    FlsErase(FLS_BASE_WIFI_CFG);
    FlsWrite(FLS_BASE_WIFI_CFG,   &(f->code), 4);
    FlsWrite(FLS_BASE_WIFI_CFG+4, &(f->size), 4);
    if(f->size)
        FlsWrite(FLS_BASE_WIFI_CFG+8, f->buf, f->size);
#if 0
    
    FlsRead(&b->code,FLS_BASE_WIFI_CFG, 4);
    FlsRead(&b->size,FLS_BASE_WIFI_CFG+4, 4);
    FlsRead(b->buf,FLS_BASE_WIFI_CFG+8, b->size);
    _hexdump(__func__,b->buf,b->size,1);
    printf("b->code %x, f->code %x \n", b->code, f->code); 
    free_buf_stream(b);
#endif
    return f->size;
}

/* 
    @return value
    -1 : faile, else read size
*/
int buf_stream_flash_read(struct buf_stream *f)
{
    FlsRead(&f->code,FLS_BASE_WIFI_CFG, 4);

    if(f->code != 0x11223344)
        return -1;
        
    FlsRead(&f->size,FLS_BASE_WIFI_CFG+4, 4);
    FlsRead(f->buf,FLS_BASE_WIFI_CFG+8, f->size);

    return f->size;
}


/**
 * wpa_config_get_line - Read the next configuration file line
 * @s: Buffer for the line
 * @size: The buffer length
 * @stream: buffer stream to read from
 * @line: Pointer to a variable storing the file line number
 * @_pos: Buffer for the pointer to the beginning of data on the text line or
 * %NULL if not needed (returned value used instead)
 * Returns: Pointer to the beginning of data on the text line or %NULL if no
 * more text lines are available.
 *
 * This function reads the next non-empty line from the configuration file and
 * removes comments. The returned string is guaranteed to be null-terminated.
 */
static char * 
wpa_config_get_line(char *s, int size, struct buf_stream *stream, int *line,
				  char **_pos)
{
	char *pos, *end, *sstart;

	while (bgets(s, size, stream)) {
		(*line)++;
		s[size - 1] = '\0';
		pos = s;

		/* Skip white space from the beginning of line. */
		while (*pos == ' ' || *pos == '\t' || *pos == '\r')
			pos++;

		/* Skip comment lines and empty lines */
		if (*pos == '#' || *pos == '\n' || *pos == '\0')
			continue;

		/*
		 * Remove # comments unless they are within a double quoted
		 * string.
		 */
		sstart = os_strchr(pos, '"');
		if (sstart)
			sstart = os_strrchr(sstart + 1, '"');
		if (!sstart)
			sstart = pos;
		end = os_strchr(sstart, '#');
		if (end)
			*end-- = '\0';
		else
			end = pos + os_strlen(pos) - 1;

		/* Remove trailing white space. */
		while (end > pos &&
		       (*end == '\n' || *end == ' ' || *end == '\t' ||
			*end == '\r'))
			*end-- = '\0';

		if (*pos == '\0')
			continue;

		if (_pos)
			*_pos = pos;
		return pos;
	}

	if (_pos)
		*_pos = NULL;
	return NULL;
}

#if 0
/* 
	ssid : ssid string, length variable 
	psk  : psk string,  64+1 fixed length
*/
static struct wpa_ssid 
*wpa_config_read_network(const char *pcon_ssid, const char *pcon_psk, int id)
{
	struct wpa_ssid *ssid;
	char buff[256];
	int config_size =0, pos=0, len;
	int ssid_len = strlen(pcon_ssid);


	ssid = os_zalloc(sizeof(*ssid));
	if (ssid == NULL)
		return NULL;
	ssid->id = id;


    pos =0;

	wpa_config_set_network_defaults(ssid);

	//UPDATE PSK 
	if (hexstr2bin(pcon_psk, ssid->psk, PMK_LEN))	
		goto err;
	ssid->psk_set = 1;
	
	//UPDATAE SSID
	ssid->ssid_len = ssid_len;
	ssid->ssid = os_zalloc(ssid_len);
	os_memcpy(ssid->ssid, pcon_ssid, ssid_len);

	if(ssid->psk_set)
	    wpa_hexdump(MSG_MSGDUMP, "psk", ssid->psk, 32);

    wpa_hexdump(MSG_MSGDUMP, "ssid",ssid->ssid, ssid->ssid_len);

    if(ssid->bssid_set)
        wpa_hexdump(MSG_MSGDUMP, "bssid",ssid->bssid, ETH_ALEN);      
	
	return ssid;
	
err:	
	return NULL;		
}
#endif


static int wpa_config_validate_network(struct wpa_ssid *ssid, int line)
{
	int errors = 0;

	if (ssid->passphrase) {
		if (ssid->psk_set) {
			wpa_printf(MSG_ERROR, "Line %d: both PSK and "
				   "passphrase configured.", line);
			errors++;
		}
		wpa_config_update_psk(ssid);
	}

	if ((ssid->key_mgmt & WPA_KEY_MGMT_PSK) && !ssid->psk_set) {
		wpa_printf(MSG_ERROR, "Line %d: WPA-PSK accepted for key "
			   "management, but no PSK configured.", line);
		errors++;
	}

	if ((ssid->group_cipher & WPA_CIPHER_CCMP) &&
	    !(ssid->pairwise_cipher & WPA_CIPHER_CCMP) &&
	    !(ssid->pairwise_cipher & WPA_CIPHER_NONE)) {
		/* Group cipher cannot be stronger than the pairwise cipher. */
		wpa_printf(MSG_DEBUG, "Line %d: removed CCMP from group cipher"
			   " list since it was not allowed for pairwise "
			   "cipher", line);
		ssid->group_cipher &= ~WPA_CIPHER_CCMP;
	}

	return errors;
}

static struct wpa_ssid * wpa_config_read_network(struct buf_stream *f, int *line, int id)
{
	struct wpa_ssid *ssid;
	int errors = 0, end = 0;
	char buf[256], *pos, *pos2;

	wpa_printf(MSG_MSGDUMP, "Line: %d - start of a new network block",
		   *line);
	ssid = os_zalloc(sizeof(*ssid));
	if (ssid == NULL)
		return NULL;
	ssid->id = id;

	wpa_config_set_network_defaults(ssid);

	while (wpa_config_get_line(buf, sizeof(buf), f, line, &pos)) {        
		if (os_strcmp(pos, "}") == 0) {
			end = 1;
			break;
		}
		

		pos2 = os_strchr(pos, '=');
		if (pos2 == NULL) {
			wpa_printf(MSG_ERROR, "Line %d: Invalid SSID line "
				   "'%s'.", *line, pos);
			errors++;
			continue;
		}

		*pos2++ = '\0';
		if (*pos2 == '"') {
			if (os_strchr(pos2 + 1, '"') == NULL) {
				wpa_printf(MSG_ERROR, "Line %d: invalid "
					   "quotation '%s'.", *line, pos2);
				errors++;
				continue;
			}
		}
        printf("%s, pos %s, pos2 %s , line %d \n",__func__,pos,pos2,line);
		if (wpa_config_set(ssid, pos, pos2, *line) < 0)
			errors++;
	}

	if (!end) {
		wpa_printf(MSG_ERROR, "Line %d: network block was not "
			   "terminated properly.", *line);
		errors++;
	}

	errors += wpa_config_validate_network(ssid, *line);

	if (errors) {
		wpa_config_free_ssid(ssid);
		ssid = NULL;
	}
	return ssid;
}

/*
    user input ssid change into wpa_ssid .
    wpa_ssid'format "ssid"
*/
static char*
change_to_wpa_sring(char *ssid, char *temp)
{
    int len = strlen(ssid);

    temp[0]='"';
    os_memcpy(temp+1, ssid, len);
    temp[len+1]='"';
    temp[len+2] = 0;
}

void 
printf_config_ssd(struct wpa_ssid *ssid)
{
    printf("id              %d \n",     ssid->id);
    printf("priority        %d \n",     ssid->priority);
    printf("ssid            %s\n",      ssid->ssid);
    printf("passphrase      %s \n",     ssid->passphrase);
    printf("pairwise_cipher %x \n",     ssid->pairwise_cipher);
    printf("group_cipher    %x \n",     ssid->group_cipher);
    printf("key_mgmt        %x \n",     ssid->key_mgmt);
    printf("proto           %x \n",     ssid->proto);
    printf("auth_alg        %x \n",     ssid->auth_alg);
    printf("scan_ssid       %d \n",     ssid->scan_ssid);
    wpa_hexdump(1,"wep key",ssid->wep_key[0],ssid->wep_key_len[0]);
    printf("wep_tx_keyindx  %d \n",     ssid->wep_tx_keyidx);
    
}

/*
    This function WPA-network regiters to supplicant. 
*/

struct wpa_ssid *
wpa_add_wep_netowrk(struct wpa_config *config, char* ssid, char *wep_key)
{    
    struct wpa_ssid *net_ssid;
    char new_ssid[33];
    
    net_ssid = wpa_config_add_network(config);
    if (ssid == NULL)
		goto out;
		
	net_ssid->disabled = 1;
	wpa_config_set_network_defaults(net_ssid);
    
    change_to_wpa_sring(ssid, new_ssid);
    wpa_config_set(net_ssid,"key_mgmt" ,"NONE",0);
    wpa_config_set(net_ssid,"ssid" ,new_ssid,0);
    wpa_config_set(net_ssid,"wep_key0", wep_key,0);
    wpa_config_set(net_ssid,"wep_tx_keyindex",0,0);
    wpa_config_set(net_ssid,"scan_ssid",1,0);
    
out:
	return net_ssid;
}



#if 0
const char *cfg_ssid = "update_config=1\n"
                      "network={\n"
                       "        ssid=\"iptime_enx\" \n"
                       "        key_mgmt=WPA-PSK\n"
                       "        psk=\"sysfo_eyenix\"\n"
                       "}\n";


struct wpa_ssid *
wpa_add_psk_network(struct wpa_config *config, char* ssid, char *passphrase)
{
    struct wpa_ssid *net_ssid;
    char temp[33];
    
    net_ssid = wpa_config_add_network(config);
    if (ssid == NULL)
		goto out;
		
	net_ssid->disabled = 1;
	wpa_config_set_network_defaults(net_ssid);
    
    
    wpa_config_set(net_ssid,  "key_mgmt" ,"WPA-PSK",0);
    change_to_wpa_sring(ssid, temp);
    wpa_config_set(net_ssid,  "ssid",temp,0);
    change_to_wpa_sring(passphrase, temp);
    wpa_config_set(net_ssid,  "psk",  temp,0);
    wpa_config_set(net_ssid,  "scan_ssid",1,0);      
out:
	return net_ssid;    
}

#endif

struct wpa_config * wpa_config_read(void)
{

#if 0
    struct buf_stream *b_stream;
	int errors = 0,line=0,ret;
	char buf[256],*pos;
	struct wpa_ssid *ssid, *tail = NULL, *head = NULL;
	struct wpa_config *config;
	int id = 0;
	int config_size = strlen(cfg_ssid);
	
	config = wpa_config_alloc_empty(NULL, NULL);
	if (config == NULL)
		return NULL;

    b_stream = alloc_buf_stream(1024);
    
    ret = buf_stream_flash_read(b_stream);

    if(ret == -1)
    {
        os_memcpy(b_stream->buf, cfg_ssid, config_size);
        b_stream->size = config_size;
        b_stream->pos  = 0;
    }
    else
        printf("flash wifi cfg read is ok\n");
     

	while(wpa_config_get_line(buf, sizeof(buf), b_stream, &line, &pos)) {
        if (os_strcmp(pos, "network={") == 0) {
            ssid = wpa_config_read_network(b_stream, &line, id++);
            if (ssid == NULL) {
                wpa_printf(MSG_ERROR, "Line %d: failed to "
            	       "parse network block.", line);
                errors++;
                continue;
            }
            if (head == NULL) {
                head = tail = ssid;
            } else {
                tail->next = ssid;
                tail = ssid;
            }
            if (wpa_config_add_prio_network(config, ssid)) {
                wpa_printf(MSG_ERROR, "Line %d: failed to add "
            	       "network block to priority list.",
            	    line);
                errors++;
                continue;
            }
        } 
        else if (os_strncmp(pos, "update_config=", 14) == 0) {
			config->update_config = atoi(pos + 14);
			wpa_printf(MSG_DEBUG, "update_config=%d",
				   config->update_config);
		}   	
	}

	config->ssid = head;
	printf_config_ssd(config->ssid);
	if (errors) {
		wpa_config_free(config);
		config = NULL;
		head = NULL;
	}
#else
    struct wpa_config *config;
    config = wpa_config_alloc_empty(NULL, NULL);

	if (config == NULL)
		return NULL;
	config->ssid = NULL;	
//    config->ssid=wpa_add_wep_netowrk(config,"netgear33","759478362c");    
    //config->ssid = wpa_add_psk_network(config,"iptime_enx","sysfo_eyenix");
//    config->ssid->disabled = 0;
//    if(config->ssid)
//	printf_config_ssd(config->ssid);
#endif
	return config;
} 


static void write_int(struct buf_stream *f, const char *field, int value, int def)
{
	if (value == def)
		return;
	bprintf(f,"\t%s=%d\n", field, value);
}


static void write_bssid(struct buf_stream *f, struct wpa_ssid *ssid)
{
	char *value = wpa_config_get(ssid, "bssid");
	if (value == NULL)
		return;
	bprintf(f, "\tbssid=%s\n", value);
	os_free(value);
}


static void write_psk(struct buf_stream *f, struct wpa_ssid *ssid)
{
	char *value = wpa_config_get(ssid, "psk");
	if (value == NULL)
		return;
	bprintf(f, "\tpsk=%s\n", value);
	os_free(value);
}


static void write_proto(struct buf_stream *f, struct wpa_ssid *ssid)
{
	char *value;
    
	if (ssid->proto == DEFAULT_PROTO)
		return;

	value = wpa_config_get(ssid, "proto");
	if (value == NULL)
		return;
	if (value[0])
		bprintf(f, "\tproto=%s\n", value);
	os_free(value);
}


static void write_key_mgmt(struct buf_stream *f, struct wpa_ssid *ssid)
{
	char *value;

	if (ssid->key_mgmt == DEFAULT_KEY_MGMT)
		return;

	value = wpa_config_get(ssid, "key_mgmt");
	if (value == NULL)
		return;
	if (value[0])
		bprintf(f, "\tkey_mgmt=%s\n", value);
 	os_free(value);
}


static void write_pairwise(struct buf_stream *f, struct wpa_ssid *ssid)
{
 	char *value;

	if (ssid->pairwise_cipher == DEFAULT_PAIRWISE)
		return;

	value = wpa_config_get(ssid, "pairwise");
	if (value == NULL)
		return;
		
	if (value[0])
		bprintf(f, "\tpairwise=%s\n", value);

	os_free(value);
}


static void write_group(struct buf_stream *f, struct wpa_ssid *ssid)
{
	char *value;

	if (ssid->group_cipher == DEFAULT_GROUP)
		return;

	value = wpa_config_get(ssid, "group");
	if (value == NULL)
		return;
	if (value[0])
		bprintf(f, "\tgroup=%s\n", value);
	os_free(value);
}


static void write_auth_alg(struct buf_stream *f, struct wpa_ssid *ssid)
{
	char *value;

	if (ssid->auth_alg == 0)
		return;

	value = wpa_config_get(ssid, "auth_alg");
	if (value == NULL)
		return;
		
	if (value[0])
		bprintf(f, "\tauth_alg=%s\n", value);
 
	os_free(value);
}

static void write_wep_key(struct buf_stream *f, int idx, struct wpa_ssid *ssid)
{
	char field[20], *value;

	os_snprintf(field, sizeof(field), "wep_key%d", idx);
	value = wpa_config_get(ssid, field);
	if (value) {
		bprintf(f, "\t%s=%s\n", field, value);
		os_free(value);
	}
}


static void wpa_config_write_network(struct buf_stream *f, struct wpa_ssid *ssid)
{
	int i;
#if 0
#define STR(t) write_str(f, #t, ssid)
#define INT(t) write_int(f, #t, ssid->t, 0)
#define INT_DEF(t, def) write_int(f, #t, ssid->t, def)

	STR(ssid);
	INT(scan_ssid);
	write_bssid(f, ssid);
	write_psk(f, ssid);
	write_proto(f, ssid);
	write_key_mgmt(f, ssid);
	write_pairwise(f, ssid);
	write_group(f, ssid);
	write_auth_alg(f, ssid);

	for (i = 0; i < 4; i++)
		write_wep_key(f, i, ssid);
	INT(wep_tx_keyidx);
	INT(priority);
	INT(mode);
	INT(proactive_key_caching);
	INT(disabled);
//	INT(peerkey);
	STR(id_str);

#undef STR
#undef INT
#undef INT_DEF
#endif
}

/*
    @len    : returned ssid info length
    @return : buffer point to print ssid info. 
*/

char* 
printf_config_ssid(struct wpa_ssid *ssid, int *len)
{
    struct buf_stream *f;
    f = alloc_buf_stream(0);
    f->buf = os_malloc(256);
    f->size = 0;
    bprintf(f, "\nnetwork={\n");
	wpa_config_write_network(f, ssid);
	bprintf(f, "}\n");
    free_buf_stream(f);
    *len = f->size;
    return f->buf;
}



int wpa_config_write(const char *name, struct wpa_config *config)
{
	struct buf_stream *f;
	struct wpa_ssid *ssid;
	int ret = 0,cnt=0;

	wpa_printf(MSG_DEBUG, "Writing configuration file '%s'", name);

    f = alloc_buf_stream(2048);

	for (ssid = config->ssid; ssid; ssid = ssid->next) {
		bprintf(f, "\nnetwork={\n");
		wpa_config_write_network(f, ssid);
		bprintf(f, "}\n");
		cnt++;
	}
	
	wpa_printf(MSG_DEBUG, "Configuration file '%s' written %ssuccessfully",
		   name, ret ? "un" : "");

    if(cnt == 0)
    {
        f->code = 0;
        f->size = 0;
    }
    buf_stream_flash_write(f);
    free_buf_stream(f);
#if 0		   
    //_hexdump(__func__,f->buf,f->pos,1);

    {
        char buf[256],*pos;
        int line=0;
        f->pos = 0;
	    while(wpa_config_get_line(buf, sizeof(buf), f, &line, &pos)) {	   
            printf("buf %s \n", buf);
           // _hexdump(__func__,buf,f->pos,1);
        }
    }
#endif    
	return ret;
}
