/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "dev.h"

#if defined(__NETWORK__)

#if defined(__WIFI__)
#include "fsaplist.h"
#endif
#if defined(__FILESYSTEM__)
#include "enx_file.h"
#if (LOAD_FS_SDCARD==1)
#include "fsfilelist.h"
#endif
#endif
//#include "fslivestream.h"

#include "lwip/apps/httpd_opts.h"
#include "lwip/apps/fs.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include <string.h>


#include HTTPD_FSDATA_FILE

/*-----------------------------------------------------------------------------------*/

#if LWIP_HTTPD_CUSTOM_FILES
int fs_open_custom(struct fs_file *file, const char *name);
void fs_close_custom(struct fs_file *file);
#if LWIP_HTTPD_FS_ASYNC_READ
u8_t fs_canread_custom(struct fs_file *file);
u8_t fs_wait_read_custom(struct fs_file *file, fs_wait_cb callback_fn, void *callback_arg);
int fs_read_async_custom(struct fs_file *file, char *buffer, int count, fs_wait_cb callback_fn, void *callback_arg);
#else /* LWIP_HTTPD_FS_ASYNC_READ */
int fs_read_custom(struct fs_file *file, char *buffer, int count);
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
#endif /* LWIP_HTTPD_CUSTOM_FILES */

/*-----------------------------------------------------------------------------------*/

#if LWIP_HTTPD_EXAMPLE_CUSTOMFILES_DELAYED

#if !LWIP_HTTPD_CUSTOM_FILES
#error This needs LWIP_HTTPD_CUSTOM_FILES
#endif
#if !LWIP_HTTPD_DYNAMIC_HEADERS
#error This needs LWIP_HTTPD_DYNAMIC_HEADERS
#endif
#if !LWIP_HTTPD_DYNAMIC_FILE_READ
#error This wants to demonstrate read-after-open, so LWIP_HTTPD_DYNAMIC_FILE_READ is required!
#endif
#if !LWIP_HTTPD_FS_ASYNC_READ
#error This needs LWIP_HTTPD_FS_ASYNC_READ
#endif

#if LWIP_HTTPD_EXAMPLE_CUSTOMFILES_DELAYED
#include "lwip/tcpip.h"
#endif

#endif

/*-----------------------------------------------------------------------------------*/
err_t
fs_open(struct fs_file *file, const char *name)
{
  const struct fsdata_file *f;

  if ((file == NULL) || (name == NULL)) {
    return ERR_ARG;
  }

#if LWIP_HTTPD_CUSTOM_FILES
  if (fs_open_custom(file, name)) {
    file->is_custom_file = 1;
    return ERR_OK;
  }
  file->is_custom_file = 0;
#endif /* LWIP_HTTPD_CUSTOM_FILES */

  for (f = FS_ROOT; f != NULL; f = f->next) {
    if (!strcmp(name, (const char *)f->name)) {
      file->data = (const char *)f->data;
      file->len = f->len;
      file->index = f->len;
      file->pextension = NULL;
      file->flags = f->flags;
#if HTTPD_PRECALCULATED_CHECKSUM
      file->chksum_count = f->chksum_count;
      file->chksum = f->chksum;
#endif /* HTTPD_PRECALCULATED_CHECKSUM */
#if LWIP_HTTPD_FILE_STATE
      file->state = fs_state_init(file, name);
#endif /* #if LWIP_HTTPD_FILE_STATE */
      return ERR_OK;
    }
  }
  /* file not found */
  return ERR_VAL;
}

/*-----------------------------------------------------------------------------------*/
void
fs_close(struct fs_file *file)
{
#if LWIP_HTTPD_CUSTOM_FILES
  if (file->is_custom_file) {
    fs_close_custom(file);
  }
#endif /* LWIP_HTTPD_CUSTOM_FILES */
#if LWIP_HTTPD_FILE_STATE
  fs_state_free(file, file->state);
#endif /* #if LWIP_HTTPD_FILE_STATE */
  LWIP_UNUSED_ARG(file);
}
/*-----------------------------------------------------------------------------------*/
#if LWIP_HTTPD_DYNAMIC_FILE_READ
#if LWIP_HTTPD_FS_ASYNC_READ
int
fs_read_async(struct fs_file *file, char *buffer, int count, fs_wait_cb callback_fn, void *callback_arg)
#else /* LWIP_HTTPD_FS_ASYNC_READ */
int
fs_read(struct fs_file *file, char *buffer, int count)
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
{
  int read;
  if (file->index == file->len) {
    return FS_READ_EOF;
  }
#if LWIP_HTTPD_FS_ASYNC_READ
  LWIP_UNUSED_ARG(callback_fn);
  LWIP_UNUSED_ARG(callback_arg);
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
#if LWIP_HTTPD_CUSTOM_FILES
  if (file->is_custom_file) {
#if LWIP_HTTPD_FS_ASYNC_READ
    return fs_read_async_custom(file, buffer, count, callback_fn, callback_arg);
#else /* LWIP_HTTPD_FS_ASYNC_READ */
    return fs_read_custom(file, buffer, count);
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
  }
#endif /* LWIP_HTTPD_CUSTOM_FILES */

  read = file->len - file->index;
  if (read > count) {
    read = count;
  }

  MEMCPY(buffer, (file->data + file->index), read);
  file->index += read;

  return (read);
}
#endif /* LWIP_HTTPD_DYNAMIC_FILE_READ */
/*-----------------------------------------------------------------------------------*/
#if LWIP_HTTPD_FS_ASYNC_READ
int
fs_is_file_ready(struct fs_file *file, fs_wait_cb callback_fn, void *callback_arg)
{
  if (file != NULL) {
#if LWIP_HTTPD_FS_ASYNC_READ
#if LWIP_HTTPD_CUSTOM_FILES
    if (!fs_canread_custom(file)) {
      if (fs_wait_read_custom(file, callback_fn, callback_arg)) {
        return 0;
      }
    }
#else /* LWIP_HTTPD_CUSTOM_FILES */
    LWIP_UNUSED_ARG(callback_fn);
    LWIP_UNUSED_ARG(callback_arg);
#endif /* LWIP_HTTPD_CUSTOM_FILES */
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
  }
  return 1;
}

u8_t
fs_canread_custom(struct fs_file *file)
{
  LWIP_ASSERT("file != NULL", file != NULL);
  struct fs_custom_data *data = (struct fs_custom_data *)file->pextension;
  if (data == NULL) {
    /* file transfer has been completed already */
    LWIP_ASSERT("transfer complete", file->index == file->len);
    return 1;
  }
  LWIP_ASSERT("data != NULL", data != NULL);
#if 0
  if (data->type == fftVidStream) {
    if (data->stream_first == 1) {
      while (http_queue_video_get(&data->stream_data) != -2) {
        if (data->stream_data.flags == 1) {
          file->len = (data->stream_data.size + 1 + 674 + 104); // 1은 종료를 막기위한 flag...
          data->stream_ready = 1;
          return 1;
        }
      }
	  return 0;
    }

    if (data->stream_ready > 0) {
	  if (data->stream_pos == (int)data->stream_data.size) {
        data->stream_nal_ready = 0;
        data->stream_ready = 0;
        data->stream_pos = 0;
      } else {
        return 1;
      }
    }

    int data_size = http_queue_video_get(&data->stream_data);
    if (data_size == -2) {
      return 0;
    } else {
      file->len += (data_size + 104);
	  data->stream_ready = 1;
      return 1;
    }
  } else {
    return 1;
  }
#endif
  LWIP_UNUSED_ARG(file);
  return 1;
}

#if LWIP_HTTPD_EXAMPLE_CUSTOMFILES_DELAYED
static void
fs_example_read_cb(void *arg)
{
  struct fs_custom_data *data = (struct fs_custom_data *)arg;
  fs_wait_cb callback_fn = data->callback_fn;
  void *callback_arg = data->callback_arg;
  data->callback_fn = NULL;
  data->callback_arg = NULL;

  LWIP_ASSERT("no callback_fn", callback_fn != NULL);

  callback_fn(callback_arg);
}
#endif

u8_t
fs_wait_read_custom(struct fs_file *file, fs_wait_cb callback_fn, void *callback_arg)
{
#if LWIP_HTTPD_EXAMPLE_CUSTOMFILES_DELAYED
#if 0
  err_t err;
  struct fs_custom_data *data = (struct fs_custom_data *)file->pextension;
  LWIP_ASSERT("data not set", data != NULL);
  flprintf("insert CALLBACK\r\n");
  data->callback_fn = callback_fn;
  data->callback_arg = callback_arg;
  err = tcpip_try_callback(fs_example_read_cb, data);
  LWIP_ASSERT("out of queue elements?", err == ERR_OK);
  LWIP_UNUSED_ARG(err);
#endif
#else
//LWIP_ASSERT("not implemented in this example configuration", 0);
#endif
  LWIP_UNUSED_ARG(file);
  LWIP_UNUSED_ARG(callback_fn);
  LWIP_UNUSED_ARG(callback_arg);
  /* Return
     - 1 if ready to read (at least one byte)
     - 0 if reading should be delayed (call 'tcpip_callback(callback_fn, callback_arg)' when ready) */
  /* - 읽을 준비가 되면 1 (적어도 1 바이트).
     - 읽기가 지연되어야하는 경우 0 (준비가 되면 'tcpip_callback (callback_fn, callback_arg)' */
  return 1;
}

int
fs_read_async_custom(struct fs_file *file, char *buffer, int count, fs_wait_cb callback_fn, void *callback_arg)
{
  struct fs_custom_data *data = (struct fs_custom_data *)file->pextension;
  UINT br = 0;
  LWIP_ASSERT("data not set", data != NULL);

#if 0
  if (data->type == fftVidStream) {
// MP4 전송 시작부 =============================================================================================
    BYTE *buf = (BYTE *)buffer;
    UINT src = 0; //, offset = 0, orl_size = count;
//	flprintf("START: count(%d) len(%d)\r\n", count, file->len);
	if (data->stream_first == 1) { // MP4 Header를 전송한다.
		data->stream_first = 0;
		data->sequence_number = 0;

		// 각 NAL(SPS/PPS/I)의 크기확인
		http_livestream_nal_start_remake(data->stream_data.addr, data->stream_data.flags, data->stream_data.size, data->stream_nal, 1);
		data->stream_nal_ready = 1;

		// MP4 header 생성(임시버퍼) 674byte(Video only)
		int head_len = http_livestream_header(data->tbuffer, data->stream_nal);
		data->tbuf_size = head_len;

		if ((int)data->tbuf_size > count) { // http buffer가 부족할 경우...
			br = count;
			flprintf("pos(%d) copy(%d)\r\n", buf - (BYTE*)buffer, br);
			DmaMemCpy_ip((BYTE *)buf, (BYTE *)data->tbuffer, br);
			invalidate_dcache_range((uint)(buf), ((uint)buf+br));

			data->tbuf_offset = br;
			file->index += br;
			return br;
		} else {
			// http buffer가 넉넉함
			br = data->tbuf_size;
//			flprintf("pos(%d) copy(%d)\r\n", buf - (BYTE*)buffer, br);
			DmaMemCpy_ip((BYTE *)buf, (BYTE *)data->tbuffer, br);
			invalidate_dcache_range((uint)(buf), ((uint)buf+br));

			data->tbuf_size = 0;
			data->tbuf_offset = 0;
			file->index += br;
			buf += br;
			count -= br;
		}
    } else if (data->tbuf_offset) {
		// 임시 buffer에 데이터가 남아있다.
		UINT tsend_size = data->tbuf_size - data->tbuf_offset;
//		flprintf("offset(%d)\r\n", data->tbuf_offset);
		if ((int)tsend_size > count) {
			// http buffer가 부족함
			br = count;
			flprintf("pos(%d) copy(%d)\r\n", buf - (BYTE*)buffer, br);
			DmaMemCpy_ip((BYTE *)buf, (BYTE *)&(data->tbuffer[data->tbuf_offset]), br);
			invalidate_dcache_range((uint)(buf), ((uint)buf+br));

			data->tbuf_offset += br;
			file->index += br;
			return br;
		} else {
			// http buffer가 넉넉함
			br = tsend_size;
//			flprintf("pos(%d) copy(%d)\r\n", buf - (BYTE*)buffer, br);
			DmaMemCpy_ip((BYTE *)buf, (BYTE *)&(data->tbuffer[data->tbuf_offset]), br);
			invalidate_dcache_range((uint)(buf), ((uint)buf+br));

			data->tbuf_size = 0;
			data->tbuf_offset = 0;
			file->index += br;
			buf += br;
			count -= br;
		}
    }

	if (data->stream_ready == 1) {
		// MOOF Header + NAL start를 전송한다.
//		flprintf("stream nal ready: %d\r\n", data->stream_nal_ready);
		if (data->stream_nal_ready == 0) {
			// 각 NAL(SPS/PPS/I/P)의 크기확인
			http_livestream_nal_start_remake(data->stream_data.addr, data->stream_data.flags, data->stream_data.size, data->stream_nal, 1);
			data->stream_nal_ready = 1;
		}

		// MOOF header 생성(임시버퍼) 104byte
		data->sequence_number++;
//		flprintf("sequence number: %d\r\n", data->sequence_number);
		int moof_len = http_livestream_moof(data->tbuffer, data->stream_data.size, data->sequence_number, data->stream_nal, data->stream_data.flags);
		data->tbuf_size = moof_len;
//		flprintf("frame(%c), moof_len(%d)\r\n", data->stream_data.flags ? 'I' : 'P', moof_len);
		if (data->stream_data.flags) {
			// SPS nal(4?) + SPS(11?) + PPS nal(4?) + PPS(5?) + I nal(4?)
//			flprintf("\r\n");
			data->stream_pos = 28;
		} else {
			// P nal(4?)
//			flprintf("\r\n");
			data->stream_pos = 4;
		}

		data->stream_ready = 2;
		if ((int)data->tbuf_size > count) {
			// http buffer가 부족함
			br = count;
			flprintf("pos(%d) copy(%d)\r\n", buf - (BYTE*)buffer, br);
			DmaMemCpy_ip((BYTE *)buf, (BYTE *)data->tbuffer, br);
			invalidate_dcache_range((uint)(buf), ((uint)buf+br));

			data->tbuf_offset = br;
			file->index += br;
			return br;
		} else {
			// http buffer가 넉넉함
			br = data->tbuf_size;
//			flprintf("pos(%d) copy(%d)\r\n", buf - (BYTE*)buffer, br);
			DmaMemCpy_ip((BYTE *)buf, (BYTE *)data->tbuffer, br);
			invalidate_dcache_range((uint)(buf), ((uint)buf+br));

			data->tbuf_size = 0;
			data->tbuf_offset = 0;
			file->index += br;
			buf += br;
			count -= br;
		}
	}

	// NAL을 전송한다.

	// 영상의 중간부분 ~ 끝부분을 전송하게 된다.
//	flprintf("size(%d) pos(%d)\r\n", data->stream_data.size, data->stream_pos);
	if ((int)(data->stream_data.size - data->stream_pos) > count) {
		br = count;
//		flprintf("copy(%d)\r\n", br);
	} else {
		br = (data->stream_data.size - data->stream_pos);
//		flprintf("copy(%d)\r\n", br);
	}

	if (br == 0) {
		data->stream_nal_ready = 0;
		data->stream_ready = 0;
		data->stream_pos = 0;
		flprintf("Delayed\r\n");
		return FS_READ_DELAYED;
	}

	if (((int)br) < 0) {
//		flprintf("br Error!!!!!\r\n");
		while (1) {
			vTaskDelay(1);
		}
	}

	src = data->stream_data.addr + data->stream_pos;

//	flprintf("pos(%d) copy(%d)\r\n", buf - (BYTE*)buffer, br);
	DmaMemCpy_ip((BYTE *)buf, (BYTE *)src, br);
	invalidate_dcache_range((uint)(buf), ((uint)buf+br));

    data->stream_pos += br;
//    flprintf("pos(%d) size(%d)\r\n", data->stream_pos, data->stream_data.size);
    if (data->stream_pos == (int)data->stream_data.size) {
      data->stream_nal_ready = 0;
      data->stream_ready = 0;
      data->stream_pos = 0;
//	  flprintf("ready/pos clear\r\n");
    }
    file->index += br;
	buf += br;
	count -= br;

//	flprintf("END: idx(%d)len(%d)pos(%d)cnt(%d)br(%d):(%d)\r\n", file->index, file->len, buf - (BYTE*)buffer, count, br, file->len - file->index);

//	printf("ori size:%d\r\n", orl_size);
//	hexDump("TEST", buffer, orl_size);
//	while(1)
//		vTaskDelay(1);

    return (buf - (BYTE*)buffer);
// MP4 전송 종료부 =============================================================================================
  }
#endif
#if LWIP_HTTPD_EXAMPLE_CUSTOMFILES_DELAYED
  /* This just simulates a delay. This delay would normally come e.g. from SPI transfer */
  /* 이것은 단지 지연을 시뮬레이션합니다. 이 지연은 일반적으로 예를 들어 SPI 전송에서. */
  LWIP_ASSERT("invalid state", data->delay_read >= 0 && data->delay_read <= 2);
  if (data->delay_read == 2) {
    /* no delay next time */
    /* 다음 번엔 아무런 지연도 없습니다. */
    data->delay_read = 0;
    return FS_READ_DELAYED;
  } else if (data->delay_read == 1) {
    err_t err;
    /* execute requested delay */
    /* 요청 된 지연을 실행하십시오. */
    data->delay_read = 2;
    LWIP_ASSERT("duplicate callback request", data->callback_fn == NULL);
    data->callback_fn = callback_fn;
    data->callback_arg = callback_arg;
    err = tcpip_try_callback(fs_example_read_cb, data);
    LWIP_ASSERT("out of queue elements?", err == ERR_OK);
    LWIP_UNUSED_ARG(err);
    return FS_READ_DELAYED;
  }
  /* execute this read but delay the next one */
  /* 이 읽기를 실행하지만 다음 것을 지연하십시오. */
  data->delay_read = 1;
#endif

#if defined(__FILESYSTEM__)
  if (data->type == fftBigfile) {
    FRESULT fres;
    UINT read = file->len - file->index;
    if(read > (UINT)count) {
      read = count;
    }
    fres = f_read(&data->f, (void *)buffer, read, &br);
    if (fres != FR_OK) {
      printf("%s, f_read error(%d)\r\n", __func__, fres);
    }
  } else {
     printf("%s, type(%d)\r\n", __func__, data->type);
  }
#endif

  LWIP_UNUSED_ARG(callback_fn);
  LWIP_UNUSED_ARG(callback_arg);

  file->index += br;

  /* Return
     - FS_READ_EOF if all bytes have been read
     - FS_READ_DELAYED if reading is delayed (call 'tcpip_callback(callback_fn, callback_arg)' when done) */
  if (br == 0) {
    /* all bytes read already */
    return FS_READ_EOF;
  }
  return br;
}
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
/*-----------------------------------------------------------------------------------*/
int
fs_bytes_left(struct fs_file *file)
{
  return file->len - file->index;
}

#if LWIP_HTTPD_CUSTOM_FILES
extern char _q_x2c(char hex_up, char hex_low);
extern size_t _q_urldecode(char *str);

#if 0
static int fs_open_custom_stream_h264_mp4(struct fs_file *file)
{
  struct fs_custom_data *data = (struct fs_custom_data *)file->pextension;
  data->type = fftVidStream;
//data->buffer = (BYTE *)mem_malloc(2048);
//data->buf_size = 2048;
  data->stream_first = 1;
  data->stream_ready = 0;
  data->stream_pos = 0;
  file->data = NULL;
  file->index = 0;
  file->len = 0;
  http_queue_init();
#if LWIP_HTTPD_SUPPORT_11_RANGE
  data->total = 0x7fffffff; // size 정보
  UINT gettime = get_fattime();
  data->last_modified_date = ((gettime >> 16) & 0xffff);
  data->last_modified_time = (gettime & 0xffff);

  int code_flag = 0;
  if (file->range_start == -1) {
    file->range_start = 0;
  } else if (file->range_start >= data->total) {
    file->range_start = -2;
  } else {
    code_flag = 1;
  }

  if (file->range_end == -1) {
    file->range_end = 0x7fffffff - 1; // size 정보
  } else if (file->range_end >= data->total) {
    file->range_end = -2;
  } else {
    code_flag = 1;
  }

  if (code_flag == 1) {
    data->code = 206;
  } else {
    data->code = 200;
  }

  if (file->range_start == -2 || file->range_end == -2) {
    data->code = 416;
  }

#endif
  return 1;
}
#endif

#if defined(__WIFI__)
static int fs_open_custom_wifi_scanap_js(struct fs_file *file)
{
  struct fs_custom_data *data = (struct fs_custom_data *)file->pextension;
  UINT realsize = 0;

  file->data = (const char *)enx_web_get_aplist(&realsize);
  if (file->data) { // malloc ok
    data->type = fftMalloc;
    file->len = realsize;
    file->index = realsize;
    return 1;
  } else {
    printf("%s(%d) : file malloc fail : scanap.js\r\n", __func__, __LINE__);
    return 0;
  }
}
#endif // #if defined(__WIFI__)

#if JPEG_SNAPSHOT
static int fs_open_custom_still_jpg(struct fs_file *file)
{
  struct fs_custom_data *data = (struct fs_custom_data *)file->pextension;
  UINT realsize = 0;

  while (gptMsgShare.JPEG_STILL_FLAG == JPEG_SNAP_PROC) {
    WaitXus(10);
  }
  gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_STR;
  gptMsgShare.JPEG_STILL_REF++;
  file->data = (char *)gptMsgShare.JPEG_STILL_ADDR;
  if (file->data) { // malloc ok
    realsize = gptMsgShare.JPEG_STILL_SIZE;
    data->type = fftJpeg;
    file->len = realsize;
    file->index = realsize;
    return 1;
  } else {
    gptMsgShare.JPEG_STILL_REF--;
    if (gptMsgShare.JPEG_STILL_REF == 0) {
      gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_IDE;
    } else if (gptMsgShare.JPEG_STILL_REF < 0) {
      printf("%s(%d) : JPEG_STILL_REF(%d) Error!\r\n", __func__, __LINE__, gptMsgShare.JPEG_STILL_REF);
      gptMsgShare.JPEG_STILL_REF = 0;
    }
    printf("%s(%d) : JPEG STILL ADDR NULL\r\n", __func__, __LINE__);
    return 0;
  }
}
#endif // #if JPEG_SNAPSHOT

#if defined(__FILESYSTEM__)
#if (LOAD_FS_SDCARD==1)
static int fs_open_custom_sdcard(struct fs_file *file)
{
  struct fs_custom_data *data = (struct fs_custom_data *)file->pextension;
  UINT realsize = 0;

  // SD card File list
  char filepath[FF_MAX_LFN+1] = {0};
  char filename[512] = {0};
  strlcpy(filename, &data->name[8], 512);
  _q_urldecode(filename);

  char *strlsxml = strstr(filename, SD_DIR_LS_XML);
  char *strlshtml = strstr(filename, SD_DIR_LS_HTML);
  if ((strlsxml != NULL) || (strlshtml != NULL)) {
    if (gtUser.bSdVidSave == ENX_ON) {
      gtUser.bSdVidSave = ENX_OFF;
      flprintf("Web connect - SD File list - SDcard save Stop!\r\n");
      vTaskDelay(10);
    }
    char fileVpath[FF_MAX_LFN*2] = {0};
    int strlens = 0;
    if (strlsxml) {
      strlens = strlsxml - filename; // [../sdsave/] ~ [ls.xml] inside char count
    } else {
      strlens = strlshtml - filename; // [../sdsave/] ~ [ls.html] inside char count
  	}
    strncpy(fileVpath, filename, strlens);
    if (fileVpath[strlens-1] == '/') {
      fileVpath[strlens-1] = '\0';
    }
    sprintf(filepath, "%d:/%s", DEV_SD, fileVpath);

    if (strlsxml) {
      file->data = (const char *)enx_webls_get_xml_sdcheck(&realsize);
      if (file->data == NULL) {
        file->data = (const char *)enx_webls_get_xml(filepath, &realsize);
      }
    } else {
      file->data = (const char *)enx_webls_get_html_sdcheck(&realsize);
      if (file->data == NULL) {
        file->data = (const char *)enx_webls_get_html(filepath, &realsize);
      }
    }
    if (file->data) { // malloc ok
      data->type = fftMalloc;
      file->len = realsize;
      file->index = realsize;
      return 1;
    } else {
      printf("%s(%d) : file malloc fail : %s\r\n", __func__, __LINE__, filepath);
      return 0;
    }
  } else {
    snprintf(filepath, FF_MAX_LFN, "%d:/%s", DEV_SD, &data->name[8]);
#if LWIP_HTTPD_SUPPORT_11_RANGE
    FILINFO finfo;
    if (f_stat(filepath, &finfo) != FR_OK) {
      printf("%s(%d) : file stat fail : %s\r\n", __func__, __LINE__, filepath);
      return 0;
    }
#endif
    if (f_open(&data->f, filepath, FA_READ) != FR_OK) { // $CMT-hjlee-181022 -
      printf("%s(%d) : file open fail : %s\r\n", __func__, __LINE__, filepath);
      return 0;
    }
#if LWIP_HTTPD_SUPPORT_11_RANGE
    data->total = f_size(&data->f);
    data->last_modified_date = finfo.fdate;
    data->last_modified_time = finfo.ftime;
    int code_flag = 0;
    if (file->range_start == -1) {
      file->range_start = 0;
    } else if (file->range_start >= data->total) {
      file->range_start = -2;
    } else {
      code_flag = 1;
      f_lseek(&data->f, file->range_start);
    }
    if (file->range_end == -1) {
      file->range_end = f_size(&data->f) - 1;
    } else if (file->range_end >= data->total) {
      file->range_end = -2;
    } else {
      code_flag = 1;
    }
    if (code_flag == 1) {
      data->code = 206;
    } else {
      data->code = 200;
    }
    if (file->range_start == -2 || file->range_end == -2) {
      file->len = data->total;
  	  data->code = 416;
    } else {
      file->len = file->range_end - file->range_start + 1;
    }
#else
    file->len = f_size(&data->f);
#endif
//  printf("%s(%d) : file open size : %d\r\n", __func__, __LINE__, f_size(&file->fat_file));
    data->type = fftBigfile;
    file->data = NULL;
    file->index = 0;
    return 1;
  }
}
#endif // #if (LOAD_FS_SDCARD==1)

#if (LOAD_FS_FLS==1)
static int fs_open_custom_fls_webfile(struct fs_file *file)
{
  struct fs_custom_data *data = (struct fs_custom_data *)file->pextension;
  char filepath[FF_MAX_LFN+1] = {0};

  snprintf(filepath, FF_MAX_LFN, "%d:/%s%s", DEV_FLS, FLS_DIR_WEB_PATH, data->name);
  if (f_open(&data->f, filepath, FA_READ) != FR_OK) {
    return 0;
  }

  file->data = NULL;
  data->type = fftBigfile;
  file->len = f_size(&data->f);
  file->index = 0;
  return 1;
}
#endif // #if (LOAD_FS_FLS==1)
#endif // #if defined(__FILESYSTEM__)

int fs_open_custom(struct fs_file *file, const char *name)
{
  int fsopenres = 0;
  struct fs_custom_data *data = (struct fs_custom_data *)mem_malloc(sizeof(struct fs_custom_data));
  if (data == NULL) {
    flprintf("malloc fail\r\n");
    return 0;
  } else {
    data->name = name;
//  data->buffer = NULL;
    file->data = NULL;
    file->len = 0;
    file->index = 0;
    file->pextension = data;
#if HTTPD_PRECALCULATED_CHECKSUM
    file->chksum = NULL;
    file->chksum_count = 0;
#endif
    file->flags = 0;
#if LWIP_HTTPD_FILE_STATE
    file->state = fs_state_init(file, name);
#endif
  }

#if 0
  if (strncmp(name, "/stream/h264.mp4", strlen("/stream/h264.mp4")) == 0) {
  	flprintf("Open\r\n");
    fsopenres = fs_open_custom_stream_h264_mp4(file);
  } else
#endif
#if defined(__WIFI__)
  if (strncmp(name, "/wifi/scanap.js", strlen("/wifi/scanap.js")) == 0) {
    fsopenres = fs_open_custom_wifi_scanap_js(file);
  } else
#endif /* defined(__WIFI__) */
#if JPEG_SNAPSHOT
  if (strncmp(name, "/still.jpg", strlen("/still.jpg")) == 0) {
    fsopenres = fs_open_custom_still_jpg(file);
  } else
#endif
#if defined(__FILESYSTEM__)
#if (LOAD_FS_SDCARD==1)
  if (strncmp(name, "/"SD_DIR_ROOT_PATH"/", strlen("/"SD_DIR_ROOT_PATH"/")) == 0) {
    fsopenres = fs_open_custom_sdcard(file);
  } else
#endif /* (LOAD_FS_SDCARD==1) */
#if (LOAD_FS_FLS==1)
  {
	fsopenres = fs_open_custom_fls_webfile(file);
  }
#endif /* (LOAD_FS_FLS==1) */
#endif /* defined(__FILESYSTEM__) */
  if (fsopenres == 1) {
  	return 1;
  }
  mem_free(data);
  file->pextension = NULL;
  return 0;
  UNUSED(file);
  UNUSED(name);
}

int fs_read_custom(struct fs_file *file, char *buffer, int count)
{
  struct fs_custom_data *data = (struct fs_custom_data *)file->pextension;
#if defined(__FILESYSTEM__)
  if (data->type == fftBigfile) {
    FRESULT fres;
    UINT br = 0, read = file->len - file->index;
    if(read > (UINT)count) {
      read = count;
    }
    fres = f_read(&data->f, (void *)buffer, read, &br);
    if (fres == FR_OK) {
      file->index += br;
      return br;
    } else {
      printf("%s, f_read error(%d)\r\n", __func__, fres);
    }
  } else {
    printf("%s, type(%d)\r\n", __func__, data->type);
  }
#endif
  return 0;
  UNUSED(file);
  UNUSED(buffer);
  UNUSED(count);
}

void fs_close_custom(struct fs_file *file)
{
  struct fs_custom_data *data = (struct fs_custom_data *)file->pextension;
#if defined(__FILESYSTEM__)
  if (data->f.obj.fs != NULL) {
    f_close(&data->f);
  }
  if (data->type == fftBigfile) {
    file->data = NULL;
  } else
#endif
#if JPEG_SNAPSHOT
  if (data->type == fftJpeg) {
    file->data = NULL;
    gptMsgShare.JPEG_STILL_REF--;
    if (gptMsgShare.JPEG_STILL_REF == 0) {
      gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_IDE;
    } else if (gptMsgShare.JPEG_STILL_REF < 0) {
      printf("%s(%d) : JPEG_STILL_REF(%d) Error!\r\n", __func__, __LINE__, gptMsgShare.JPEG_STILL_REF);
      gptMsgShare.JPEG_STILL_REF = 0;
    }
  } else
#endif
  if (data->type == fftMalloc) {
    if (file->data) {
      mem_free((void *)file->data);
    }
    file->data = NULL;
  }
#if 0
  } else if (data->type == fftVidStream) {
    flprintf("Close\r\n");
//  if (data->buffer) {
//    mem_free(data->buffer);
//    data->buffer = NULL;
//  }
    http_queue_deinit();
  }
#endif
  mem_free(data);
  file->pextension = NULL;
}
#endif
#endif
