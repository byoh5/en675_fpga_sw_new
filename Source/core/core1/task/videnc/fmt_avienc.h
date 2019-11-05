/* Copyright (c) 2013-2014 Eyenix Corporation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. The name of Eyenix may not be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* 4. This software may only be redistributed and used in connection with an Eyenix
* AVR product.
*
* THIS SOFTWARE IS PROVIDED BY EYENIX "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL EYENIX BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*
*/

#ifndef _MAKEAVI_H_
#define _MAKEAVI_H_

#include "ff.h"

//*************************************************************************************************
// typedefs
//-------------------------------------------------------------------------------------------------

/*
  Simple AVI Format
  
RIFF ('AVI '
      LIST ('hdrl' ... )
      LIST ('movi' ... )
      ['idx1' (<AVI Index>) ]
     )

RIFF ('AVI '

      LIST ('hdrl'
            'avih'(<Main AVI Header>)
            LIST ('strl'
                  'strh'(<Stream header>)
                  'strf'(<Stream format>)
                  [ 'strd'(<Additional header data>) ]
                  [ 'strn'(<Stream name>) ]
                  ...
                 )
             ...
           )

      LIST ('movi'
            {SubChunk | LIST ('rec '
                              SubChunk1
                              SubChunk2
                              ...
                             )
               ...
            }
            ...
           )

      ['idx1' (<AVI Index>) ]
     )
*/

#define AVI_HEADSIZE 4096

typedef struct {
	UINT option;							// Video, Audio, Text

	int i_width;
	int i_height;
	int f_fps;

	UINT i_movi_end;
	UINT i_riff;
	UINT i_header_end;

	UINT type;
	UINT un32vidicount;	// video i-frame count
	UINT un32vidscount;	// video total frame count
	UINT un32audscount;	// audio total frame count
	UINT un32txtscount;	// G-sensor, GPS, etc...

	UINT un32vidsmax;
	UINT un32audsmax;
	UINT un32txtsmax;
	UINT un32totalmax;
	UINT un32maxtime;
} t_avi_t;

#define FCC(ch1, ch2, ch3, ch4)	((DWORD)((ch1 << 24) | (ch2 << 16) | (ch3 << 8) | (ch4 << 0)))

#define NUM32(ch4) ((((DWORD)(ch4) & 0xFF) << 24) |     \
                  (((DWORD)(ch4) & 0xFF00) << 8) |    \
                  (((DWORD)(ch4) & 0xFF0000) >> 8) |  \
                  (((DWORD)(ch4) & 0xFF000000) >> 24))

#define NUM16(ch2) ((((WORD)(ch2) & 0xFF) << 8) |    \
                  (((WORD)(ch2) & 0xFF00) >> 8))

typedef DWORD FOURCC;

typedef struct {
	FOURCC	fcc;						// FOURCC 코드를 지정한다.
	DWORD	cb;							// 구조체의 사이즈를 지정한다. 최초의 8 바이트분을 공제한 값을 지정한다.
} __attribute__ ((packed)) FCC_CB_t;

typedef struct {
	FCC_CB_t fcccb;						// 값은 'LIST' 가 아니면 안된다.
	FOURCC fccListType;					// FOURCC 코드를 지정한다.
} __attribute__ ((packed)) RIFFLIST;

typedef struct {
	FCC_CB_t fcccb;						// 값은 'avih' 가 아니면 안된다.
	struct {
		DWORD	dwMicroSecPerFrame;		// 프레임간의 간격을 마이크로 세컨드 단위로 지정한다. 이 값은 파일의 전체의 타이밍을 나타낸다.
		DWORD	dwMaxBytesPerSec;		// 파일의 개산 최대 데이터 레이트를 지정한다. 이 값은, 메인 헤더 및 스트림헤더 체크에 포함되는 다른 파라미터에 따라 AVI 순서를 표시하기 위해서, 시스템이 처리해야 하는 매초의 바이트수를 나타낸다.
		DWORD	dwPaddingGranularity;	// 데이터의 아라이먼트를 바이트 단위로 지정한다. 이 값의 배수에 데이터를 패딩 한다.
		DWORD	dwFlags;				// 다음의 플래그의 0 개 이상의 비트마다 편성을 저장 한다.
		DWORD	dwTotalFrames;			// 파일내의 데이터의 프레임의 총수를 지정한다.
		DWORD	dwInitialFrames;		// 인터리브 된 파일의 시작 프레임을 지정한다. 인터리브 된 파일 이외에서는, 0 을 지정한다. 인터리브 된 파일을 생성 하는 경우, 파일내에서 AVI 순서의 시작 프레임보다 전에 있는 프레임수를, 이 멤버로 지정한다.
		DWORD	dwStreams;				// 파일내의 스트림수를 지정한다. 예를 들어, 오디오와 비디오를 포함한 파일에는 2 개의 스트림이 있다.
		DWORD	dwSuggestedBufferSize;	// 파일을 읽어내기 위한 버퍼 사이즈를 지정한다. 일반적으로, 이 사이즈는 파일내의 최대의 체크를 저장 하는데 충분한 크기로 한다. 0 으로 설정하거나 너무 작은 값으로 설정했을 경우, 재생 소프트웨어는 재생중에 메모리를 재할인 맞히고 하지 않으면 안되어, 퍼포먼스가 저하한다. 인터리브 된 파일의 경우, 버퍼 사이즈는 체크는 아니고 레코드 전체를 읽어내는데 충분한 크기가 아니면 안된다.
		DWORD	dwWidth;				// AVI 파일의 폭을 지정한다 (픽셀 단위).
		DWORD	dwHeight;				// AVI 파일의 높이를 지정한다 (픽셀 단위).
		DWORD	dwReserved[4];			// 예약이 끝난 상태. 이 배열은 제로로 설정한다.
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) AVIMAINHEADER;

// AVIMAINHEADER - dwFlags
#define AVIF_HASINDEX		 0x00000010 // Index at end of file?
#define AVIF_MUSTUSEINDEX	 0x00000020
#define AVIF_ISINTERLEAVED	 0x00000100
#define AVIF_TRUSTCKTYPE	 0x00000800 // Use CKType to find key frames
#define AVIF_WASCAPTUREFILE  0x00010000
#define AVIF_COPYRIGHTED	 0x00020000

/*
  @fcccb : Specifies a FOURCC code.  structure, not including the initial 8 bytes. 
  
  @fccType  
   Contains a FOURCC that specifies the type of the data contained in the stream.
   The following standard AVI values for video and audio are defined. 
   
   ------------------------------------- 
    FOURCC   Description
   ------------------------------------- 
   'auds'    Audio stream
   -------------------------------------
   'mids'    MIDI stream
   -------------------------------------
   'txts'    Text stream
   -------------------------------------
   'vids'    Video stream
   -------------------------------------
 
  @fccHandler
    Optionally, contains a FOURCC that identifies a specific data handler. The 
    data handler is the preferred handler for the stream. For audio and video 
    streams, this specifies the codec for decoding the stream. 

  @dwFlags
    Contains any flags for the data stream. The bits in the high-order word of 
    these flags are specific to the type of data contained in the stream. The 
    following standard flags are defined. 

    ----------------------------------------------------------------------------
    Value                   Meaning
    ----------------------------------------------------------------------------
    AVISF_DISABLED          Indicates this stream should not be enabled by default. 
    ----------------------------------------------------------------------------
    AVISF_VIDEO_PALCHANGES  Indicates this video stream contains palette changes.
                            This flag warns the playback software that it will 
                            need to animate the palette. 
    ----------------------------------------------------------------------------
 
  @wPriority
    Specifies priority of a stream type. For example, in a file with multiple 
    audio streams, the one with the highest priority might be the default stream. 

  @wLanguage
    Language tag. 

  @dwInitialFrames
    Specifies how far audio data is skewed ahead of the video frames in 
    interleaved files. Typically, this is about 0.75 seconds. If you are creating 
    interleaved files, specify the number of frames in the file prior to the 
    initial frame of the AVI sequence in this member. For more information, see 
    the remarks for the dwInitialFrames member of the AVIMAINHEADER structure.

  @dwScale
    Used with dwRate to specify the time scale that this stream will use. 
    Dividing dwRate by dwScale gives the number of samples per second. For video 
    streams, this is the frame rate. For audio streams, this rate corresponds to 
    the time needed to play nBlockAlign bytes of audio, which for PCM audio is 
    the just the sample rate. 

  @dwRate
    See dwScale. 

  @dwStart
    Specifies the starting time for this stream. The units are defined by the 
    dwRate and dwScale members in the main file header. Usually, this is zero, 
    but it can specify a delay time for a stream that does not start concurrently 
    with the file.

  @dwLength
    Specifies the length of this stream. The units are defined by the dwRate and 
    dwScale members of the stream's header.

  @dwSuggestedBufferSize
    Specifies how large a buffer should be used to read this stream. Typically, 
    this contains a value corresponding to the largest chunk present in the stream
    . Using the correct buffer size makes playback more efficient. Use zero if 
    you do not know the correct buffer size. 

  @dwQuality
    Specifies an indicator of the quality of the data in the stream. Quality is 
    represented as a number between 0 and 10,000. For compressed data, this 
    typically represents the value of the quality parameter passed to the 
    compression software. If set to ?1, drivers use the default quality value. 

  @dwSampleSize
    Specifies the size of a single sample of data. This is set to zero if the 
    samples can vary in size. If this number is nonzero, then multiple samples of 
    data can be grouped into a single chunk within the file. If it is zero, each 
    sample of data (such as a video frame) must be in a separate chunk. For video 
    streams, this number is typically zero, although it can be nonzero if all 
    video frames are the same size. For audio streams, this number should be the 
    same as the nBlockAlign member of the WAVEFORMATEX structure describing the 
    audio. 

  @rcFrame
    Specifies the destination rectangle for a text or video stream within the 
    movie rectangle specified by the dwWidth and dwHeight members of the AVI main 
    header structure. The rcFrame member is typically used in support of multiple 
    video streams. Set this rectangle to the coordinates corresponding to the 
    movie rectangle to update the whole movie rectangle. Units for this member 
    are pixels. The upper-left corner of the destination rectangle is relative to 
    the upper-left corner of the movie rectangle. 
*/
typedef struct {
	FCC_CB_t fcccb; 					// 값은 'strh' 가 아니면 안된다.
	struct {
		FOURCC	fccType;				// 스트림에 포함되는 데이터의 타입을 지정하는 FOURCC 를 포함한다. 비디오 및 오디오에 대해서 표준 AVI 값이 정의되고 있다. auds(오디오), mids(MIDI), txts(텍스트), vids(비디오)
		FOURCC	fccHandler;				// 특정의 데이터 핸들러를 나타내는 FOURCC 를 포함한다 (옵션). 데이터 핸들러는, 스트림에 대해서 적절한 핸들러이다. 오디오 스트림 또는 비디오 스트림에 대해, 스트림을 디코드하는 코덱을 지정한다. 
		DWORD	dwFlags;				// 데이터 스트림에 대한 플래그를 포함한다. 이러한 플래그의 상위 워드의 비트는, 스트림에 포함되는 데이터의 타입에 고유하다.
		WORD	wPriority;				// 스트림 타입의 우선 순위를 지정한다. 예를 들어, 복수의 오디오 스트림을 포함한 파일에서는, 우선 순위의 가장 높은 스트림이 디폴트의 스트림이 된다. 
		WORD	wLanguage;				// 
		DWORD	dwInitialFrames;		// 인터리브 된 파일로, 오디오 데이터가 비디오 프레임으로부터 어느 정도 skew 되고 있는지를 지정한다. 보통은, 약 0.75 초이다. 인터리브 된 파일을 생성 하는 경우, 파일내에서 AVI 순서의 시작 프레임보다 전에 있는 프레임수를, 이 멤버로 지정한다.
		DWORD	dwScale;				// dwRate 와 함께 사용해, 이 스트림이 사용하는 타임즈 케일을 지정한다. dwRate 를 dwScale 로 나누는 것으로, 1 초 쯤의 샘플수가 요구된다. 비디오 스트림의 경우, 이것은 frame rate이다. 오디오 스트림의 경우, 이 레이트는 nBlockAlign 바이트의 오디오의 재생에 필요한 시간에 상당한다. 이것은 PCM 오디오의 경우는 샘플 레이트에 동일해진다.
		DWORD	dwRate;					// dwScale 를 참조할것. 
		DWORD	dwStart;				// 이 스트림의 시작 타임을 지정한다. 단위는, 메인 파일 헤더의 dwRate 및 dwScale 멤버에 의해 정의된다.
		DWORD	dwLength;				// 이 스트림의 길이를 지정한다. 단위는, 스트림의 헤더의 dwRate 및 dwScale 멤버에 의해 정의된다. 
		DWORD	dwSuggestedBufferSize;	// 이 스트림을 읽어내기 위해서(때문에) 필요한 버퍼의 크기를 지정한다. 보통은, 스트림내의 최대의 체크에 대응하는 값이다. 올바른 버퍼 사이즈를 사용하는 것으로, 재생의 효율이 높아진다. 올바른 버퍼 사이즈를 모르는 경우는, 0 을 지정한다. 
		DWORD	dwQuality;				// 스트림내의 데이터의 품질을 나타내는 값을 지정한다. 품질은, 0 ~ 10,000 의 범위의 값으로 나타난다. 압축 데이터의 경우, 이것은 보통, 압축 소프트웨어에게 건네지는 품질 파라미터의 값을 나타낸다. -1 (으)로 설정했을 경우, 드라이버는 디폴트의 품질값을 사용한다. 
		DWORD	dwSampleSize;			// 데이터의 1 샘플의 사이즈를 지정한다. 샘플의 사이즈가 변화하는 경우는, 0 으로 설정한다. 이 값이 0 이 아닌 경우, 파일내에서 복수의 데이터 샘플을 1 개의 체크에 그룹화 할 수 있다. 0 의 경우, 각 데이터 샘플 (비디오 프레임등)은 각각 다른 체크에 포함되지 않으면 안 된다. 비디오 스트림의 경우, 이 값은 보통 0 이지만, 모든 비디오 프레임이 같은 사이즈이면, 0 이외의 값에도 할 수 있다. 오디오 스트림의 경우, 이 값은 오디오를 기술하는 WAVEFORMATEX 구조체의 nBlockAlign 멤버와 같지 않으면 안 된다. 
		struct {
			short int	left;
			short int	top;
			short int	right;
			short int	bottom;
		} rcFrame;						// AVI 메인 헤더 구조체의 dwWidth 및 dwHeight 멤버에 의해 지정되는 동영상 직사각형내의 텍스트 또는 비디오 스트림에 대한 목적지 직사각형을 지정한다. 보통,rcFrame 멤버는, 복수의 비디오 스트림을 지원 하기 위해서 사용된다. 이 직사각형은, 동영상 직사각형에 대응하는 좌표로 설정해, 동영상 직사각형 전체를 갱신한다. 이 멤버의 단위는 픽셀이다. 목적지 직사각형의 좌상구석은, 동영상 직사각형의 좌상구석으로부터의 상대 지정이 된다.
	} __attribute__ ((packed)) shData;
} __attribute__ ((packed)) AVISTREAMHEADER;

// AVISTREAMHEADER - dwFlags
#define AVISF_DISABLED			0x00000001
#define AVISF_VIDEO_PALCHANGES	0x00010000

typedef struct {
	FCC_CB_t fcccb; 					// 값은 'strf' 가 아니면 안된다.
	struct {
		DWORD	biSize; 				// 구조체가 필요로 하는 바이트수를 지정한다. 구조체가 컬러 테이블이나 컬러 마스크의 뒤에 계속되는 경우, 그 컬러 테이블이나 컬러 마스크의 사이즈는 이 값에는 포함되지 않는다. 주의를 참조할것. 
		LONG	biWidth;				// 비트 맵의 폭을 지정한다. 
		LONG	biHeight;				// 비트 맵의 높이를 지정한다 (픽셀 단위). 
		WORD	biPlanes;				// 타겟 장치에 대한 면의 수를 지정한다. 이것은 반드시 1 으로 설정한다. 
		WORD	biBitCount; 			// 1 픽셀 근처의 비트수를 지정한다. 
		FOURCC	biCompression;			// 비트 맵이 압축되고 있는 경우, 이 멤버는 압축을 지정하는 FOURCC 이다.
		DWORD	biSizeImage;			// 이미지의 사이즈를 지정한다 (바이트 단위). 비압축 RGB 비트 맵의 경우는, 0 으로 설정할 수 있다. 
		LONG	biXPelsPerMeter;		// 비트 맵의 타겟 장치의 수평 해상도를 1 미터 당의 픽셀 단위로 지정한다. 
		LONG	biYPelsPerMeter;		// 비트 맵의 타겟 장치의 수직 해상도를 1 미터 당의 픽셀 단위로 지정한다. 
		DWORD	biClrUsed;				// 컬러 테이블내의 컬러 인덱스 가운데, 비트 맵내에서 실제로 사용하는 인덱스의 수를 지정한다.
		DWORD	biClrImportant; 		// 비트 맵을 표시하기 위해서 중요라고 보여지는 컬러 인덱스수를 지정한다. 이 값이 제로의 경우는, 모든 색이 중요라고 보여진다. 
	} __attribute__ ((packed)) bmData;
} __attribute__ ((packed)) BITMAPINFOHEADER;

typedef struct {
	FCC_CB_t fcccb; 					// 값은 'strf' 가 아니면 안된다.
	struct { 
		WORD	wFormatTag; 			// 파형 오디오의 포맷 타입. 포맷 태그는, 많은 압축 알고리즘용으로 Microsoft Corporation 에 등록되어 있다. 포맷 태그의 완전한 리스트는, Mmreg.h 헤더 파일에 있다. 1 채널이나 2 채널의 PCM (Pulse Code Modulation) 데이터의 경우, 이 값은 WAVE_FORMAT_PCM 로 할 필요가 있다. 
		WORD	nChannels;				// 파형 오디오 데이터에 포함되는 채널수. 단청의 데이터는 1 채널을 사용해, 스테레오 데이터는 2 채널을 사용한다. 
		DWORD	nSamplesPerSec; 		// 샘플/초에 나타내는 샘플 레이트 (단위 Hz). wFormatTag 가 WAVE_FORMAT_PCM 의 경우,nSamplesPerSec 의 일반적인 값은 8.0 kHz, 11.025 kHz, 22.05 kHz, 44.1 kHz 가 된다. 비 PCM 포맷의 경우, 이 멤버는 제조업자의 포맷 태그 사양에 따라 산출할 필요가 있다. 
		DWORD	nAvgBytesPerSec;		// 포맷 태그에 필요한 평균 데이터 전송 레이트 (단위 바이트/초). wFormatTag 가 WAVE_FORMAT_PCM 의 경우,nAvgBytesPerSec 는 nSamplesPerSec 와 nBlockAlign 의 적과 동일해야 한다. 비 PCM 포맷의 경우, 이 멤버는 제조업자의 포맷 태그 사양에 따라 산출할 필요가 있다. 
		WORD	nBlockAlign;			// 블록 alignment (단위 바이트). 블록 alignment란,wFormatTag 포맷 타입의 데이터의 최소 구성 단위이다. wFormatTag 가 WAVE_FORMAT_PCM 또는 WAVE_FORMAT_EXTENSIBLE 의 경우,nBlockAlign 는nChannels 와 wBitsPerSample 의 적을 8 (1 바이트 당의 비트수)으로 나눈 값과 동일해야 한다. 비 PCM 포맷의 경우, 이 멤버는 제조업자의 포맷 태그 사양에 따라 산출할 필요가 있다. 소프트웨어는, 한 번에 복수의 nBlockAlign 바이트의 데이터를 처리할 필요가 있다. 장치에 대한 데이터의 쓰기와 읽기는 항상, 블록의 선두로부터 시작 해야 한다. 예를 들어, PCM 데이터의 재생을 샘플의 도중 (즉, 비블록 아라인 경계상)부터 시작 하는 것은 부정하다. 
		WORD	wBitsPerSample; 		// wFormatTag 포맷 타입의 1 샘플 근처의 비트수. wFormatTag 가 WAVE_FORMAT_PCM 의 경우,wBitsPerSample 는 8 또는 16 이 아니면 안된다. wFormatTag 가 WAVE_FORMAT_EXTENSIBLE 의 경우, 이 값은, 임의의 8 의 배수를 지정할 수 있다. 일부의 압축 스킴은 wBitsPerSample 의 값을 정의할 수 없기 때문에, 이 멤버에는 0 을 지정해도 상관없다. 
		WORD	cbSize; 				// WAVEFORMATEX 구조체의 마지막에 추가되는 추가 포맷 정보의 사이즈 (단위 바이트). 비 PCM 포맷은, 이 정보를 사용해 wFormatTag 의 추가 속성을 저장 할 수 있다. wFormatTag 에 추가 정보가 필요없는 경우는, 이 멤버는 제로로 설정해야 한다. WAVE_FORMAT_PCM 포맷 밖에 없는 경우, 이 멤버는 무시된다. 
	} __attribute__ ((packed)) wmData; 
} __attribute__ ((packed)) WAVEFORMATEX;

typedef struct {
	FCC_CB_t fcccb; 					// 값은 'strf' 가 아니면 안된다.
} __attribute__ ((packed)) TXTSFORMATEX;

typedef struct {
	FOURCC	dwChunkId;					// AVI 파일내의 스트림을 식별하는 FOURCC 를 지정한다. FOURCC 는 'xxyy'의 형식이 아니면 안된다. xx 는 스트림 번호, yy 는 스트림의 내용을 식별하는 2 자리수의 문자 코드이다. db비압축의 비디오 프레임. dc압축된 비디오 프레임. pc팔레트의 변경. wb오디오 데이터. 
	DWORD	dwFlags;					// 다음의 플래그의 0 개 이상의 비트마다 편성을 지정한다. AVIIF_KEYFRAME 데이타체크는 키 프레임이다. AVIIF_LIST 데이타체크는 'rec'리스트이다. AVIIF_NO_TIME 데이타체크는 스트림의 타이밍에 영향을 주지 않는다. 예를 들어, 이 플래그는 팔레트 변경 시에 설정한다. 
	DWORD	dwOffset;					// 파일내의 데이타체크의 위치를 지정한다. 값은, 바이트 단위로 'movi'리스트의 선두로부터 오프셋(offset)로서 지정할 필요가 있다. 다만, 일부의 AVI 파일에서는, 파일의 선두로부터의 오프셋(offset)로서 지정한다. 
	DWORD	dwSize;						// 데이타체크의 사이즈를 바이트 단위로 지정한다. 
} __attribute__ ((packed)) AVIINDEX_t;

// AVIOLDINDEX - dwFlags
#define AVIIF_LIST		 0x00000001
#define AVIIF_KEYFRAME	 0x00000010
#define AVIIF_NO_TIME	 0x00000100
#define AVIIF_COMPRESSOR 0x0FFF0000

/*
LIST ('strl'
      'strh'(<Stream header>)
      'strf'(<Stream format>)
*/
typedef struct {
	RIFFLIST list;
	AVISTREAMHEADER stream;
	BITMAPINFOHEADER info;
} __attribute__ ((packed)) avi_videostream_t;

typedef struct {
	RIFFLIST list;
	AVISTREAMHEADER stream;
	WAVEFORMATEX info;
} __attribute__ ((packed)) avi_audiostream_t;

typedef struct {
	RIFFLIST list;
	AVISTREAMHEADER stream;
	TXTSFORMATEX info;
} __attribute__ ((packed)) avi_accscstream_t;

/*
RIFF ('AVI '
      LIST ('hdrl'
            'avih'(<Main AVI Header>)
*/
typedef struct {
	RIFFLIST head;			// RIFF size  AVI
	RIFFLIST list;			// LIST size hdrl
	AVIMAINHEADER data;		// avih size mhData, 'avih' chunk
} __attribute__ ((packed)) AVI_HEADER_t;

//JUNK size
#define JUNKSIZE_VAT	3678	// Video(124) + Audio(102) + Text(84)
#define JUNKSIZE_VA		3762	// Video(124) + Audio(102)
#define JUNKSIZE_VT		3780	// Video(124) + Text(84)
#define JUNKSIZE_V		3864	// Video(124)

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern FCC_CB_t avi_write_chunkheader(UINT type, UINT data_size);
extern AVIINDEX_t avi_write_index(UINT type, UINT data_pos, UINT data_size, UINT key);
extern BYTE *avi_write_header(t_avi_t *avia);

//-------------------------------------------------------------------------------------------------
// Variable
#define WAVE_FORMAT_PCM			0x0001 /* PCM */
#define WAVE_FORMAT_ALAW		0x0006 /* ALAW */
#define WAVE_FORMAT_MULAW		0x0007 /* MULAW */

#define MIX_FREE_SD_4CENT		5

#endif
