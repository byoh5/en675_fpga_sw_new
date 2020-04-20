//--=========================================================================--
//  This file is a part of QC Tool project
//-----------------------------------------------------------------------------
//
//       This confidential and proprietary software may be used only
//     as authorized by a licensing agreement from Chips&Media Inc.
//     In the event of publication, the following notice is applicable:
//
//            (C) COPYRIGHT 2004 - 2011   CHIPS&MEDIA INC.
//                      ALL RIGHTS RESERVED
//
//       The entire notice above must be reproduced on all authorized
//       copies.
//
//--=========================================================================--

#include "main_helper.h"
//#include <libavformat/avformat.h>//ksj porting for EN675
#include "wave/wave5_regdefine.h"

//ksj porting for EN675 [[
#define MKTAG(a,b,c,d) ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))
#define MKBETAG(a,b,c,d) ((d) | ((c) << 8) | ((b) << 16) | ((unsigned)(a) << 24))

/**
 * Identify the syntax and semantics of the bitstream.
 * The principle is roughly:
 * Two decoders with the same ID can decode the same streams.
 * Two encoders with the same ID can encode compatible streams.
 * There may be slight deviations from the principle due to implementation
 * details.
 *
 * If you add a codec ID to this list, add it so that
 * 1. no value of a existing codec ID changes (that would break ABI),
 * 2. Give it a value which when taken as ASCII is recognized uniquely by a human as this specific codec.
 *    This ensures that 2 forks can independently add AVCodecIDs without producing conflicts.
 *
 * After adding new codec IDs, do not forget to add an entry to the codec
 * descriptor list and bump libavcodec minor version.
 */
enum AVCodecID {
    AV_CODEC_ID_NONE,

    /* video codecs */
    AV_CODEC_ID_MPEG1VIDEO,
    AV_CODEC_ID_MPEG2VIDEO, ///< preferred ID for MPEG-1/2 video decoding
    AV_CODEC_ID_MPEG2VIDEO_XVMC,
    AV_CODEC_ID_H261,
    AV_CODEC_ID_H263,
    AV_CODEC_ID_RV10,
    AV_CODEC_ID_RV20,
    AV_CODEC_ID_MJPEG,
    AV_CODEC_ID_MJPEGB,
    AV_CODEC_ID_LJPEG,
    AV_CODEC_ID_SP5X,
    AV_CODEC_ID_JPEGLS,
    AV_CODEC_ID_MPEG4,
    AV_CODEC_ID_RAWVIDEO,
    AV_CODEC_ID_MSMPEG4V1,
    AV_CODEC_ID_MSMPEG4V2,
    AV_CODEC_ID_MSMPEG4V3,
    AV_CODEC_ID_WMV1,
    AV_CODEC_ID_WMV2,
    AV_CODEC_ID_H263P,
    AV_CODEC_ID_H263I,
    AV_CODEC_ID_FLV1,
    AV_CODEC_ID_SVQ1,
    AV_CODEC_ID_SVQ3,
    AV_CODEC_ID_DVVIDEO,
    AV_CODEC_ID_HUFFYUV,
    AV_CODEC_ID_CYUV,
    AV_CODEC_ID_H264,
    AV_CODEC_ID_INDEO3,
    AV_CODEC_ID_VP3,
    AV_CODEC_ID_THEORA,
    AV_CODEC_ID_ASV1,
    AV_CODEC_ID_ASV2,
    AV_CODEC_ID_FFV1,
    AV_CODEC_ID_4XM,
    AV_CODEC_ID_VCR1,
    AV_CODEC_ID_CLJR,
    AV_CODEC_ID_MDEC,
    AV_CODEC_ID_ROQ,
    AV_CODEC_ID_INTERPLAY_VIDEO,
    AV_CODEC_ID_XAN_WC3,
    AV_CODEC_ID_XAN_WC4,
    AV_CODEC_ID_RPZA,
    AV_CODEC_ID_CINEPAK,
    AV_CODEC_ID_WS_VQA,
    AV_CODEC_ID_MSRLE,
    AV_CODEC_ID_MSVIDEO1,
    AV_CODEC_ID_IDCIN,
    AV_CODEC_ID_8BPS,
    AV_CODEC_ID_SMC,
    AV_CODEC_ID_FLIC,
    AV_CODEC_ID_TRUEMOTION1,
    AV_CODEC_ID_VMDVIDEO,
    AV_CODEC_ID_MSZH,
    AV_CODEC_ID_ZLIB,
    AV_CODEC_ID_QTRLE,
    AV_CODEC_ID_TSCC,
    AV_CODEC_ID_ULTI,
    AV_CODEC_ID_QDRAW,
    AV_CODEC_ID_VIXL,
    AV_CODEC_ID_QPEG,
    AV_CODEC_ID_PNG,
    AV_CODEC_ID_PPM,
    AV_CODEC_ID_PBM,
    AV_CODEC_ID_PGM,
    AV_CODEC_ID_PGMYUV,
    AV_CODEC_ID_PAM,
    AV_CODEC_ID_FFVHUFF,
    AV_CODEC_ID_RV30,
    AV_CODEC_ID_RV40,
    AV_CODEC_ID_VC1,
    AV_CODEC_ID_WMV3,
    AV_CODEC_ID_LOCO,
    AV_CODEC_ID_WNV1,
    AV_CODEC_ID_AASC,
    AV_CODEC_ID_INDEO2,
    AV_CODEC_ID_FRAPS,
    AV_CODEC_ID_TRUEMOTION2,
    AV_CODEC_ID_BMP,
    AV_CODEC_ID_CSCD,
    AV_CODEC_ID_MMVIDEO,
    AV_CODEC_ID_ZMBV,
    AV_CODEC_ID_AVS,
    AV_CODEC_ID_SMACKVIDEO,
    AV_CODEC_ID_NUV,
    AV_CODEC_ID_KMVC,
    AV_CODEC_ID_FLASHSV,
    AV_CODEC_ID_CAVS,
    AV_CODEC_ID_JPEG2000,
    AV_CODEC_ID_VMNC,
    AV_CODEC_ID_VP5,
    AV_CODEC_ID_VP6,
    AV_CODEC_ID_VP6F,
    AV_CODEC_ID_TARGA,
    AV_CODEC_ID_DSICINVIDEO,
    AV_CODEC_ID_TIERTEXSEQVIDEO,
    AV_CODEC_ID_TIFF,
    AV_CODEC_ID_GIF,
    AV_CODEC_ID_DXA,
    AV_CODEC_ID_DNXHD,
    AV_CODEC_ID_THP,
    AV_CODEC_ID_SGI,
    AV_CODEC_ID_C93,
    AV_CODEC_ID_BETHSOFTVID,
    AV_CODEC_ID_PTX,
    AV_CODEC_ID_TXD,
    AV_CODEC_ID_VP6A,
    AV_CODEC_ID_AMV,
    AV_CODEC_ID_VB,
    AV_CODEC_ID_PCX,
    AV_CODEC_ID_SUNRAST,
    AV_CODEC_ID_INDEO4,
    AV_CODEC_ID_INDEO5,
    AV_CODEC_ID_MIMIC,
    AV_CODEC_ID_RL2,
    AV_CODEC_ID_ESCAPE124,
    AV_CODEC_ID_DIRAC,
    AV_CODEC_ID_BFI,
    AV_CODEC_ID_CMV,
    AV_CODEC_ID_MOTIONPIXELS,
    AV_CODEC_ID_TGV,
    AV_CODEC_ID_TGQ,
    AV_CODEC_ID_TQI,
    AV_CODEC_ID_AURA,
    AV_CODEC_ID_AURA2,
    AV_CODEC_ID_V210X,
    AV_CODEC_ID_TMV,
    AV_CODEC_ID_V210,
    AV_CODEC_ID_DPX,
    AV_CODEC_ID_MAD,
    AV_CODEC_ID_FRWU,
    AV_CODEC_ID_FLASHSV2,
    AV_CODEC_ID_CDGRAPHICS,
    AV_CODEC_ID_R210,
    AV_CODEC_ID_ANM,
    AV_CODEC_ID_BINKVIDEO,
    AV_CODEC_ID_IFF_ILBM,
    AV_CODEC_ID_IFF_BYTERUN1,
    AV_CODEC_ID_KGV1,
    AV_CODEC_ID_YOP,
    AV_CODEC_ID_VP8,
    AV_CODEC_ID_PICTOR,
    AV_CODEC_ID_ANSI,
    AV_CODEC_ID_A64_MULTI,
    AV_CODEC_ID_A64_MULTI5,
    AV_CODEC_ID_R10K,
    AV_CODEC_ID_MXPEG,
    AV_CODEC_ID_LAGARITH,
    AV_CODEC_ID_PRORES,
    AV_CODEC_ID_JV,
    AV_CODEC_ID_DFA,
    AV_CODEC_ID_WMV3IMAGE,
    AV_CODEC_ID_VC1IMAGE,
    AV_CODEC_ID_UTVIDEO,
    AV_CODEC_ID_BMV_VIDEO,
    AV_CODEC_ID_VBLE,
    AV_CODEC_ID_DXTORY,
    AV_CODEC_ID_V410,
    AV_CODEC_ID_XWD,
    AV_CODEC_ID_CDXL,
    AV_CODEC_ID_XBM,
    AV_CODEC_ID_ZEROCODEC,
    AV_CODEC_ID_MSS1,
    AV_CODEC_ID_MSA1,
    AV_CODEC_ID_TSCC2,
    AV_CODEC_ID_MTS2,
    AV_CODEC_ID_CLLC,
    AV_CODEC_ID_MSS2,
    AV_CODEC_ID_VP9,
    AV_CODEC_ID_AIC,
    AV_CODEC_ID_ESCAPE130_DEPRECATED,
    AV_CODEC_ID_G2M_DEPRECATED,
    AV_CODEC_ID_WEBP_DEPRECATED,

    AV_CODEC_ID_BRENDER_PIX= MKBETAG('B','P','I','X'),
    AV_CODEC_ID_Y41P       = MKBETAG('Y','4','1','P'),
    AV_CODEC_ID_ESCAPE130  = MKBETAG('E','1','3','0'),
    AV_CODEC_ID_EXR        = MKBETAG('0','E','X','R'),
    AV_CODEC_ID_AVRP       = MKBETAG('A','V','R','P'),

    AV_CODEC_ID_012V       = MKBETAG('0','1','2','V'),
    AV_CODEC_ID_G2M        = MKBETAG( 0 ,'G','2','M'),
    AV_CODEC_ID_AVUI       = MKBETAG('A','V','U','I'),
    AV_CODEC_ID_AYUV       = MKBETAG('A','Y','U','V'),
    AV_CODEC_ID_TARGA_Y216 = MKBETAG('T','2','1','6'),
    AV_CODEC_ID_V308       = MKBETAG('V','3','0','8'),
    AV_CODEC_ID_V408       = MKBETAG('V','4','0','8'),
    AV_CODEC_ID_YUV4       = MKBETAG('Y','U','V','4'),
    AV_CODEC_ID_SANM       = MKBETAG('S','A','N','M'),
    AV_CODEC_ID_PAF_VIDEO  = MKBETAG('P','A','F','V'),
    AV_CODEC_ID_AVRN       = MKBETAG('A','V','R','n'),
    AV_CODEC_ID_CPIA       = MKBETAG('C','P','I','A'),
    AV_CODEC_ID_XFACE      = MKBETAG('X','F','A','C'),
    AV_CODEC_ID_SGIRLE     = MKBETAG('S','G','I','R'),
    AV_CODEC_ID_MVC1       = MKBETAG('M','V','C','1'),
    AV_CODEC_ID_MVC2       = MKBETAG('M','V','C','2'),
    AV_CODEC_ID_SNOW       = MKBETAG('S','N','O','W'),
    AV_CODEC_ID_WEBP       = MKBETAG('W','E','B','P'),
    AV_CODEC_ID_SMVJPEG    = MKBETAG('S','M','V','J'),
    AV_CODEC_ID_HEVC       = MKBETAG('H','2','6','5'),
#define AV_CODEC_ID_H265 AV_CODEC_ID_HEVC

    /* various PCM "codecs" */
    AV_CODEC_ID_FIRST_AUDIO = 0x10000,     ///< A dummy id pointing at the start of audio codecs
    AV_CODEC_ID_PCM_S16LE = 0x10000,
    AV_CODEC_ID_PCM_S16BE,
    AV_CODEC_ID_PCM_U16LE,
    AV_CODEC_ID_PCM_U16BE,
    AV_CODEC_ID_PCM_S8,
    AV_CODEC_ID_PCM_U8,
    AV_CODEC_ID_PCM_MULAW,
    AV_CODEC_ID_PCM_ALAW,
    AV_CODEC_ID_PCM_S32LE,
    AV_CODEC_ID_PCM_S32BE,
    AV_CODEC_ID_PCM_U32LE,
    AV_CODEC_ID_PCM_U32BE,
    AV_CODEC_ID_PCM_S24LE,
    AV_CODEC_ID_PCM_S24BE,
    AV_CODEC_ID_PCM_U24LE,
    AV_CODEC_ID_PCM_U24BE,
    AV_CODEC_ID_PCM_S24DAUD,
    AV_CODEC_ID_PCM_ZORK,
    AV_CODEC_ID_PCM_S16LE_PLANAR,
    AV_CODEC_ID_PCM_DVD,
    AV_CODEC_ID_PCM_F32BE,
    AV_CODEC_ID_PCM_F32LE,
    AV_CODEC_ID_PCM_F64BE,
    AV_CODEC_ID_PCM_F64LE,
    AV_CODEC_ID_PCM_BLURAY,
    AV_CODEC_ID_PCM_LXF,
    AV_CODEC_ID_S302M,
    AV_CODEC_ID_PCM_S8_PLANAR,
    AV_CODEC_ID_PCM_S24LE_PLANAR_DEPRECATED,
    AV_CODEC_ID_PCM_S32LE_PLANAR_DEPRECATED,
    AV_CODEC_ID_PCM_S24LE_PLANAR = MKBETAG(24,'P','S','P'),
    AV_CODEC_ID_PCM_S32LE_PLANAR = MKBETAG(32,'P','S','P'),
    AV_CODEC_ID_PCM_S16BE_PLANAR = MKBETAG('P','S','P',16),

    /* various ADPCM codecs */
    AV_CODEC_ID_ADPCM_IMA_QT = 0x11000,
    AV_CODEC_ID_ADPCM_IMA_WAV,
    AV_CODEC_ID_ADPCM_IMA_DK3,
    AV_CODEC_ID_ADPCM_IMA_DK4,
    AV_CODEC_ID_ADPCM_IMA_WS,
    AV_CODEC_ID_ADPCM_IMA_SMJPEG,
    AV_CODEC_ID_ADPCM_MS,
    AV_CODEC_ID_ADPCM_4XM,
    AV_CODEC_ID_ADPCM_XA,
    AV_CODEC_ID_ADPCM_ADX,
    AV_CODEC_ID_ADPCM_EA,
    AV_CODEC_ID_ADPCM_G726,
    AV_CODEC_ID_ADPCM_CT,
    AV_CODEC_ID_ADPCM_SWF,
    AV_CODEC_ID_ADPCM_YAMAHA,
    AV_CODEC_ID_ADPCM_SBPRO_4,
    AV_CODEC_ID_ADPCM_SBPRO_3,
    AV_CODEC_ID_ADPCM_SBPRO_2,
    AV_CODEC_ID_ADPCM_THP,
    AV_CODEC_ID_ADPCM_IMA_AMV,
    AV_CODEC_ID_ADPCM_EA_R1,
    AV_CODEC_ID_ADPCM_EA_R3,
    AV_CODEC_ID_ADPCM_EA_R2,
    AV_CODEC_ID_ADPCM_IMA_EA_SEAD,
    AV_CODEC_ID_ADPCM_IMA_EA_EACS,
    AV_CODEC_ID_ADPCM_EA_XAS,
    AV_CODEC_ID_ADPCM_EA_MAXIS_XA,
    AV_CODEC_ID_ADPCM_IMA_ISS,
    AV_CODEC_ID_ADPCM_G722,
    AV_CODEC_ID_ADPCM_IMA_APC,
    AV_CODEC_ID_VIMA       = MKBETAG('V','I','M','A'),
    AV_CODEC_ID_ADPCM_AFC  = MKBETAG('A','F','C',' '),
    AV_CODEC_ID_ADPCM_IMA_OKI = MKBETAG('O','K','I',' '),
    AV_CODEC_ID_ADPCM_DTK  = MKBETAG('D','T','K',' '),
    AV_CODEC_ID_ADPCM_IMA_RAD = MKBETAG('R','A','D',' '),
    AV_CODEC_ID_ADPCM_G726LE = MKBETAG('6','2','7','G'),

    /* AMR */
    AV_CODEC_ID_AMR_NB = 0x12000,
    AV_CODEC_ID_AMR_WB,

    /* RealAudio codecs*/
    AV_CODEC_ID_RA_144 = 0x13000,
    AV_CODEC_ID_RA_288,

    /* various DPCM codecs */
    AV_CODEC_ID_ROQ_DPCM = 0x14000,
    AV_CODEC_ID_INTERPLAY_DPCM,
    AV_CODEC_ID_XAN_DPCM,
    AV_CODEC_ID_SOL_DPCM,

    /* audio codecs */
    AV_CODEC_ID_MP2 = 0x15000,
    AV_CODEC_ID_MP3, ///< preferred ID for decoding MPEG audio layer 1, 2 or 3
    AV_CODEC_ID_AAC,
    AV_CODEC_ID_AC3,
    AV_CODEC_ID_DTS,
    AV_CODEC_ID_VORBIS,
    AV_CODEC_ID_DVAUDIO,
    AV_CODEC_ID_WMAV1,
    AV_CODEC_ID_WMAV2,
    AV_CODEC_ID_MACE3,
    AV_CODEC_ID_MACE6,
    AV_CODEC_ID_VMDAUDIO,
    AV_CODEC_ID_FLAC,
    AV_CODEC_ID_MP3ADU,
    AV_CODEC_ID_MP3ON4,
    AV_CODEC_ID_SHORTEN,
    AV_CODEC_ID_ALAC,
    AV_CODEC_ID_WESTWOOD_SND1,
    AV_CODEC_ID_GSM, ///< as in Berlin toast format
    AV_CODEC_ID_QDM2,
    AV_CODEC_ID_COOK,
    AV_CODEC_ID_TRUESPEECH,
    AV_CODEC_ID_TTA,
    AV_CODEC_ID_SMACKAUDIO,
    AV_CODEC_ID_QCELP,
    AV_CODEC_ID_WAVPACK,
    AV_CODEC_ID_DSICINAUDIO,
    AV_CODEC_ID_IMC,
    AV_CODEC_ID_MUSEPACK7,
    AV_CODEC_ID_MLP,
    AV_CODEC_ID_GSM_MS, /* as found in WAV */
    AV_CODEC_ID_ATRAC3,
#if FF_API_VOXWARE
    AV_CODEC_ID_VOXWARE,
#endif
    AV_CODEC_ID_APE,
    AV_CODEC_ID_NELLYMOSER,
    AV_CODEC_ID_MUSEPACK8,
    AV_CODEC_ID_SPEEX,
    AV_CODEC_ID_WMAVOICE,
    AV_CODEC_ID_WMAPRO,
    AV_CODEC_ID_WMALOSSLESS,
    AV_CODEC_ID_ATRAC3P,
    AV_CODEC_ID_EAC3,
    AV_CODEC_ID_SIPR,
    AV_CODEC_ID_MP1,
    AV_CODEC_ID_TWINVQ,
    AV_CODEC_ID_TRUEHD,
    AV_CODEC_ID_MP4ALS,
    AV_CODEC_ID_ATRAC1,
    AV_CODEC_ID_BINKAUDIO_RDFT,
    AV_CODEC_ID_BINKAUDIO_DCT,
    AV_CODEC_ID_AAC_LATM,
    AV_CODEC_ID_QDMC,
    AV_CODEC_ID_CELT,
    AV_CODEC_ID_G723_1,
    AV_CODEC_ID_G729,
    AV_CODEC_ID_8SVX_EXP,
    AV_CODEC_ID_8SVX_FIB,
    AV_CODEC_ID_BMV_AUDIO,
    AV_CODEC_ID_RALF,
    AV_CODEC_ID_IAC,
    AV_CODEC_ID_ILBC,
    AV_CODEC_ID_OPUS_DEPRECATED,
    AV_CODEC_ID_COMFORT_NOISE,
    AV_CODEC_ID_TAK_DEPRECATED,
    AV_CODEC_ID_METASOUND,
    AV_CODEC_ID_FFWAVESYNTH = MKBETAG('F','F','W','S'),
    AV_CODEC_ID_SONIC       = MKBETAG('S','O','N','C'),
    AV_CODEC_ID_SONIC_LS    = MKBETAG('S','O','N','L'),
    AV_CODEC_ID_PAF_AUDIO   = MKBETAG('P','A','F','A'),
    AV_CODEC_ID_OPUS        = MKBETAG('O','P','U','S'),
    AV_CODEC_ID_TAK         = MKBETAG('t','B','a','K'),
    AV_CODEC_ID_EVRC        = MKBETAG('s','e','v','c'),
    AV_CODEC_ID_SMV         = MKBETAG('s','s','m','v'),

    /* subtitle codecs */
    AV_CODEC_ID_FIRST_SUBTITLE = 0x17000,          ///< A dummy ID pointing at the start of subtitle codecs.
    AV_CODEC_ID_DVD_SUBTITLE = 0x17000,
    AV_CODEC_ID_DVB_SUBTITLE,
    AV_CODEC_ID_TEXT,  ///< raw UTF-8 text
    AV_CODEC_ID_XSUB,
    AV_CODEC_ID_SSA,
    AV_CODEC_ID_MOV_TEXT,
    AV_CODEC_ID_HDMV_PGS_SUBTITLE,
    AV_CODEC_ID_DVB_TELETEXT,
    AV_CODEC_ID_SRT,
    AV_CODEC_ID_MICRODVD   = MKBETAG('m','D','V','D'),
    AV_CODEC_ID_EIA_608    = MKBETAG('c','6','0','8'),
    AV_CODEC_ID_JACOSUB    = MKBETAG('J','S','U','B'),
    AV_CODEC_ID_SAMI       = MKBETAG('S','A','M','I'),
    AV_CODEC_ID_REALTEXT   = MKBETAG('R','T','X','T'),
    AV_CODEC_ID_SUBVIEWER1 = MKBETAG('S','b','V','1'),
    AV_CODEC_ID_SUBVIEWER  = MKBETAG('S','u','b','V'),
    AV_CODEC_ID_SUBRIP     = MKBETAG('S','R','i','p'),
    AV_CODEC_ID_WEBVTT     = MKBETAG('W','V','T','T'),
    AV_CODEC_ID_MPL2       = MKBETAG('M','P','L','2'),
    AV_CODEC_ID_VPLAYER    = MKBETAG('V','P','l','r'),
    AV_CODEC_ID_PJS        = MKBETAG('P','h','J','S'),
    AV_CODEC_ID_ASS        = MKBETAG('A','S','S',' '),  ///< ASS as defined in Matroska

    /* other specific kind of codecs (generally used for attachments) */
    AV_CODEC_ID_FIRST_UNKNOWN = 0x18000,           ///< A dummy ID pointing at the start of various fake codecs.
    AV_CODEC_ID_TTF = 0x18000,
    AV_CODEC_ID_BINTEXT    = MKBETAG('B','T','X','T'),
    AV_CODEC_ID_XBIN       = MKBETAG('X','B','I','N'),
    AV_CODEC_ID_IDF        = MKBETAG( 0 ,'I','D','F'),
    AV_CODEC_ID_OTF        = MKBETAG( 0 ,'O','T','F'),
    AV_CODEC_ID_SMPTE_KLV  = MKBETAG('K','L','V','A'),
    AV_CODEC_ID_DVD_NAV    = MKBETAG('D','N','A','V'),


    AV_CODEC_ID_PROBE = 0x19000, ///< codec_id is not known (like AV_CODEC_ID_NONE) but lavf should attempt to identify it

    AV_CODEC_ID_MPEG2TS = 0x20000, /**< _FAKE_ codec to indicate a raw MPEG-2 TS
                                * stream (only used by libavformat) */
    AV_CODEC_ID_MPEG4SYSTEMS = 0x20001, /**< _FAKE_ codec to indicate a MPEG-4 Systems
                                * stream (only used by libavformat) */
    AV_CODEC_ID_FFMETADATA = 0x21000,   ///< Dummy codec for streams containing only metadata information.

#if FF_API_CODEC_ID
#include "old_codec_ids.h"
#endif
};
//ksj porting for EN675 ]]

// include in the ffmpeg header
typedef struct {
    CodStd      codStd;
    Uint32      mp4Class;
    Uint32      codecId;
    Uint32      fourcc;
} CodStdTab;

#ifndef MKTAG
#define MKTAG(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))
#endif

static const CodStdTab codstd_tab[] = {
    { STD_AVC,          0, AV_CODEC_ID_H264,            MKTAG('H', '2', '6', '4') },
    { STD_AVC,          0, AV_CODEC_ID_H264,            MKTAG('X', '2', '6', '4') },
    { STD_AVC,          0, AV_CODEC_ID_H264,            MKTAG('A', 'V', 'C', '1') },
    { STD_AVC,          0, AV_CODEC_ID_H264,            MKTAG('V', 'S', 'S', 'H') },
    { STD_H263,         0, AV_CODEC_ID_H263,            MKTAG('H', '2', '6', '3') },
    { STD_H263,         0, AV_CODEC_ID_H263,            MKTAG('X', '2', '6', '3') },
    { STD_H263,         0, AV_CODEC_ID_H263,            MKTAG('T', '2', '6', '3') },
    { STD_H263,         0, AV_CODEC_ID_H263,            MKTAG('L', '2', '6', '3') },
    { STD_H263,         0, AV_CODEC_ID_H263,            MKTAG('V', 'X', '1', 'K') },
    { STD_H263,         0, AV_CODEC_ID_H263,            MKTAG('Z', 'y', 'G', 'o') },
    { STD_H263,         0, AV_CODEC_ID_H263,            MKTAG('H', '2', '6', '3') },
    { STD_H263,         0, AV_CODEC_ID_H263,            MKTAG('I', '2', '6', '3') },    /* intel h263 */
    { STD_H263,         0, AV_CODEC_ID_H263,            MKTAG('H', '2', '6', '1') },
    { STD_H263,         0, AV_CODEC_ID_H263,            MKTAG('U', '2', '6', '3') },
    { STD_H263,         0, AV_CODEC_ID_H263,            MKTAG('V', 'I', 'V', '1') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('F', 'M', 'P', '4') },
    { STD_MPEG4,        5, AV_CODEC_ID_MPEG4,           MKTAG('D', 'I', 'V', 'X') },    // DivX 4
    { STD_MPEG4,        1, AV_CODEC_ID_MPEG4,           MKTAG('D', 'X', '5', '0') },
    { STD_MPEG4,        2, AV_CODEC_ID_MPEG4,           MKTAG('X', 'V', 'I', 'D') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('M', 'P', '4', 'S') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('M', '4', 'S', '2') },    //MPEG-4 version 2 simple profile
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG( 4 ,  0 ,  0 ,  0 ) },    /* some broken avi use this */
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('D', 'I', 'V', '1') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('B', 'L', 'Z', '0') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('M', 'P', '4', 'V') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('U', 'M', 'P', '4') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('W', 'V', '1', 'F') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('S', 'E', 'D', 'G') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('R', 'M', 'P', '4') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('3', 'I', 'V', '2') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('F', 'F', 'D', 'S') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('F', 'V', 'F', 'W') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('D', 'C', 'O', 'D') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('M', 'V', 'X', 'M') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('P', 'M', '4', 'V') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('S', 'M', 'P', '4') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('D', 'X', 'G', 'M') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('V', 'I', 'D', 'M') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('M', '4', 'T', '3') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('G', 'E', 'O', 'X') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('H', 'D', 'X', '4') }, /* flipped video */
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('D', 'M', 'K', '2') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('D', 'I', 'G', 'I') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('I', 'N', 'M', 'C') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('E', 'P', 'H', 'V') }, /* Ephv MPEG-4 */
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('E', 'M', '4', 'A') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('M', '4', 'C', 'C') }, /* Divio MPEG-4 */
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('S', 'N', '4', '0') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('V', 'S', 'P', 'X') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('U', 'L', 'D', 'X') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('G', 'E', 'O', 'V') },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG('S', 'I', 'P', 'P') }, /* Samsung SHR-6040 */
    { STD_DIV3,         0, AV_CODEC_ID_MSMPEG4V3,       MKTAG('D', 'I', 'V', '3') }, /* default signature when using MSMPEG4 */
    { STD_DIV3,         0, AV_CODEC_ID_MSMPEG4V3,       MKTAG('M', 'P', '4', '3') },
    { STD_DIV3,         0, AV_CODEC_ID_MSMPEG4V3,       MKTAG('M', 'P', 'G', '3') },
    { STD_MPEG4,        1, AV_CODEC_ID_MSMPEG4V3,       MKTAG('D', 'I', 'V', '5') },
    { STD_MPEG4,        1, AV_CODEC_ID_MSMPEG4V3,       MKTAG('D', 'I', 'V', '6') },
    { STD_MPEG4,        5, AV_CODEC_ID_MSMPEG4V3,       MKTAG('D', 'I', 'V', '4') },
    { STD_DIV3,         0, AV_CODEC_ID_MSMPEG4V3,       MKTAG('D', 'V', 'X', '3') },
    { STD_DIV3,         0, AV_CODEC_ID_MSMPEG4V3,       MKTAG('A', 'P', '4', '1') },    //Another hacked version of Microsoft's MP43 codec. 
    { STD_MPEG4,        0, AV_CODEC_ID_MSMPEG4V3,       MKTAG('C', 'O', 'L', '1') },
    { STD_MPEG4,        0, AV_CODEC_ID_MSMPEG4V3,       MKTAG('C', 'O', 'L', '0') },    // not support ms mpeg4 v1, 2    
    { STD_MPEG4,      256, AV_CODEC_ID_FLV1,            MKTAG('F', 'L', 'V', '1') }, /* Sorenson spark */
    { STD_VC1,          0, AV_CODEC_ID_WMV1,            MKTAG('W', 'M', 'V', '1') },
    { STD_VC1,          0, AV_CODEC_ID_WMV2,            MKTAG('W', 'M', 'V', '2') },
    { STD_MPEG2,        0, AV_CODEC_ID_MPEG1VIDEO,      MKTAG('M', 'P', 'G', '1') },
    { STD_MPEG2,        0, AV_CODEC_ID_MPEG1VIDEO,      MKTAG('M', 'P', 'G', '2') },
    { STD_MPEG2,        0, AV_CODEC_ID_MPEG2VIDEO,      MKTAG('M', 'P', 'G', '2') },
    { STD_MPEG2,        0, AV_CODEC_ID_MPEG2VIDEO,      MKTAG('M', 'P', 'E', 'G') },
    { STD_MPEG2,        0, AV_CODEC_ID_MPEG1VIDEO,      MKTAG('M', 'P', '2', 'V') },
    { STD_MPEG2,        0, AV_CODEC_ID_MPEG1VIDEO,      MKTAG('P', 'I', 'M', '1') },
    { STD_MPEG2,        0, AV_CODEC_ID_MPEG2VIDEO,      MKTAG('P', 'I', 'M', '2') },
    { STD_MPEG2,        0, AV_CODEC_ID_MPEG1VIDEO,      MKTAG('V', 'C', 'R', '2') },
    { STD_MPEG2,        0, AV_CODEC_ID_MPEG1VIDEO,      MKTAG( 1 ,  0 ,  0 ,  16) },
    { STD_MPEG2,        0, AV_CODEC_ID_MPEG2VIDEO,      MKTAG( 2 ,  0 ,  0 ,  16) },
    { STD_MPEG4,        0, AV_CODEC_ID_MPEG4,           MKTAG( 4 ,  0 ,  0 ,  16) },
    { STD_MPEG2,        0, AV_CODEC_ID_MPEG2VIDEO,      MKTAG('D', 'V', 'R', ' ') },
    { STD_MPEG2,        0, AV_CODEC_ID_MPEG2VIDEO,      MKTAG('M', 'M', 'E', 'S') },
    { STD_MPEG2,        0, AV_CODEC_ID_MPEG2VIDEO,      MKTAG('L', 'M', 'P', '2') }, /* Lead MPEG2 in avi */
    { STD_MPEG2,        0, AV_CODEC_ID_MPEG2VIDEO,      MKTAG('S', 'L', 'I', 'F') },
    { STD_MPEG2,        0, AV_CODEC_ID_MPEG2VIDEO,      MKTAG('E', 'M', '2', 'V') },
    { STD_VC1,          0, AV_CODEC_ID_WMV3,            MKTAG('W', 'M', 'V', '3') },
    { STD_VC1,          0, AV_CODEC_ID_VC1,             MKTAG('W', 'V', 'C', '1') },
    { STD_VC1,          0, AV_CODEC_ID_VC1,             MKTAG('W', 'M', 'V', 'A') },

    { STD_RV,           0, AV_CODEC_ID_RV30,            MKTAG('R','V','3','0') },
    { STD_RV,           0, AV_CODEC_ID_RV40,            MKTAG('R','V','4','0') },

    { STD_AVS,          0, AV_CODEC_ID_CAVS,            MKTAG('C','A','V','S') },
    { STD_AVS,          0, AV_CODEC_ID_AVS,             MKTAG('A','V','S','2') },
    { STD_VP3,          0, AV_CODEC_ID_VP3,             MKTAG('V', 'P', '3', '0') },
    { STD_VP3,          0, AV_CODEC_ID_VP3,             MKTAG('V', 'P', '3', '1') },
    { STD_THO,          0, AV_CODEC_ID_THEORA,          MKTAG('T', 'H', 'E', 'O') },
    { STD_VP8,          0, AV_CODEC_ID_VP8,             MKTAG('V', 'P', '8', '0') },
    { STD_VP9,          0, AV_CODEC_ID_VP9,             MKTAG('V', 'P', '9', '0') },
    //  { STD_VP6,              0, AV_CODEC_ID_VP6,             MKTAG('V', 'P', '6', '0') },
    //  { STD_VP6,              0, AV_CODEC_ID_VP6,             MKTAG('V', 'P', '6', '1') },
    //  { STD_VP6,              0, AV_CODEC_ID_VP6,             MKTAG('V', 'P', '6', '2') },
    //  { STD_VP6,              0, AV_CODEC_ID_VP6F,            MKTAG('V', 'P', '6', 'F') },
    //  { STD_VP6,              0, AV_CODEC_ID_VP6F,            MKTAG('F', 'L', 'V', '4') },
    { STD_HEVC,         0, AV_CODEC_ID_HEVC,            MKTAG('H', 'E', 'V', 'C') },
    { STD_HEVC,         0, AV_CODEC_ID_HEVC,            MKTAG('H', 'E', 'V', '1') },
    { STD_HEVC,         0, AV_CODEC_ID_HEVC,            MKTAG('H', 'V', 'C', '1') },
    { STD_HEVC,         0, AV_CODEC_ID_HEVC,            MKTAG('h', 'e', 'v', 'c') },
    { STD_HEVC,         0, AV_CODEC_ID_HEVC,            MKTAG('h', 'e', 'v', '1') },
    { STD_HEVC,         0, AV_CODEC_ID_HEVC,            MKTAG('h', 'v', 'c', '1') }
};


Int32 ConvFOURCCToMp4Class(
    Int32   fourcc
    )
{
    Uint32  i;
    Int32   mp4Class = -1;
    unsigned char str[5];

    str[0] = toupper((Int32)fourcc);
    str[1] = toupper((Int32)(fourcc>>8));
    str[2] = toupper((Int32)(fourcc>>16));
    str[3] = toupper((Int32)(fourcc>>24));
    str[4] = '\0';

    for(i=0; i<sizeof(codstd_tab)/sizeof(codstd_tab[0]); i++) {
        if (codstd_tab[i].fourcc == (Int32)MKTAG(str[0], str[1], str[2], str[3]) ) {
            mp4Class = codstd_tab[i].mp4Class;
            break;
        }
    }

    return mp4Class;
}

Int32 ConvFOURCCToCodStd(
    Uint32   fourcc
    )
{
    Int32   codStd = -1;
    Uint32  i;

    char str[5];

    str[0] = toupper((Int32)fourcc);
    str[1] = toupper((Int32)(fourcc>>8));
    str[2] = toupper((Int32)(fourcc>>16));
    str[3] = toupper((Int32)(fourcc>>24));
    str[4] = '\0';

    for(i=0; i<sizeof(codstd_tab)/sizeof(codstd_tab[0]); i++) {
        if (codstd_tab[i].fourcc == (Uint32)MKTAG(str[0], str[1], str[2], str[3])) {
            codStd = codstd_tab[i].codStd;
            break;
        }
    }

    return codStd;
}

Int32 ConvCodecIdToMp4Class(
    Uint32   codecId
    )
{
    Int32   mp4Class = -1;
    Uint32  i;

    for(i=0; i<sizeof(codstd_tab)/sizeof(codstd_tab[0]); i++) {
        if (codstd_tab[i].codecId == codecId) {
            mp4Class = codstd_tab[i].mp4Class;
            break;
        }
    }

    return mp4Class;
}

Int32 ConvCodecIdToCodStd(
    Int32   codecId
    )
{
    Int32   codStd = -1;
    Uint32  i;

    for(i=0; i<sizeof(codstd_tab)/sizeof(codstd_tab[0]); i++) {
        if (codstd_tab[i].codecId == codecId) {
            codStd = codstd_tab[i].codStd;
            break;
        }
    }

    return codStd;
}

Int32 ConvCodecIdToFourcc(
    Int32   codecId
    )
{
    Int32   fourcc = 0;
    Uint32   i;

    for(i=0; i<sizeof(codstd_tab)/sizeof(codstd_tab[0]); i++) {
        if (codstd_tab[i].codecId == codecId) {
            fourcc = codstd_tab[i].fourcc;
            break;
        }
    }
    return fourcc;
}


//////////////////////////////////////////////////////////////////////////////
// this is a sample to control VPU sub frame sync in view of Camera Interface.
//////////////////////////////////////////////////////////////////////////////
static void UpdateSubFrameSyncPos(
    EncHandle               handle, 
    EncSubFrameSyncState    *subFrameSyncState,
    Uint32                  y
    )
{
    subFrameSyncState->ipuEndOfRow = !subFrameSyncState->ipuEndOfRow; // Camera interface module should toggle ipuEndOfRow whenever 64line image is saved in frame buffer 
    VPU_EncGiveCommand(handle, ENC_SET_SUB_FRAME_SYNC, subFrameSyncState);
}

BOOL LoadYuvImageByYCbCrLine(
    EncHandle   handle,
    Uint32      coreIdx, 
    Uint8*      src, 
    size_t      picWidth, 
    size_t      picHeight,
    FrameBuffer* fb,
    Uint32      srcFbIndex
    )
{
    Int32                y, nY, nCb;
    PhysicalAddress      addrY, addrCb, addrCr;
    size_t               lumaSize, chromaSize=0, chromaStride = 0, chromaWidth=0;
    Uint8                *srcY, *srcCb, *srcCr;
    size_t               stride      = fb->stride;
    EndianMode           endian      = (EndianMode)fb->endian;
    FrameBufferFormat    format      = fb->format;
    BOOL                 interLeave  = fb->cbcrInterleave;
    int                  twice       = 1 << interLeave;
    int                  cnt=0;
    Uint32               YStartPos=0;
    EncInfo*             pEncInfo = VPU_HANDLE_TO_ENCINFO(handle);
    BOOL                 subFrameSyncEn = pEncInfo->subFrameSyncConfig.subFrameSyncOn; 
    EncSubFrameSyncState *subFrameSyncState = &pEncInfo->subFrameSyncState;
    BOOL                 remainDataWrite = (pEncInfo->subFrameSyncConfig.subFrameSyncSrcWriteMode & REMAIN_SRC_DATA_WRITE) ? TRUE : FALSE;
    Uint32               writeSrcLine = pEncInfo->subFrameSyncConfig.subFrameSyncSrcWriteMode & ~REMAIN_SRC_DATA_WRITE;

    if (subFrameSyncEn == TRUE && writeSrcLine == SRC_0LINE_WRITE && remainDataWrite == FALSE)
        return TRUE;

    switch (format) {
    case FORMAT_420:
        nY = picHeight;
        nCb = picHeight / 2;
        chromaSize = picWidth * picHeight / 4;
        chromaStride = stride / 2;
        chromaWidth = picWidth / 2;
        break;
    case FORMAT_224:
        nY = picHeight;
        nCb = picHeight / 2;
        chromaSize = picWidth * picHeight / 2;
        chromaStride = stride;
        chromaWidth = picWidth;
        break;
    case FORMAT_422:
        nY = picHeight;
        nCb = picHeight;
        chromaSize = picWidth * picHeight / 2;
        chromaStride = stride / 2;
        chromaWidth = picWidth / 2;
        break;
    case FORMAT_444:
        nY = picHeight;
        nCb = picHeight;
        chromaSize = picWidth * picHeight;
        chromaStride = stride;
        chromaWidth = picWidth;
        break;
    case FORMAT_400:
        nY = picHeight;
        nCb = 0;
        chromaSize = picWidth * picHeight / 4;
        chromaStride = stride / 2;
        chromaWidth = picWidth / 2;
        break;
    case FORMAT_YUYV:
    case FORMAT_YVYU:
    case FORMAT_UYVY:
    case FORMAT_VYUY:
    case FORMAT_YUYV_P10_16BIT_MSB:
    case FORMAT_YUYV_P10_16BIT_LSB:
    case FORMAT_YUYV_P10_32BIT_MSB:
    case FORMAT_YUYV_P10_32BIT_LSB:
    case FORMAT_YVYU_P10_16BIT_MSB:
    case FORMAT_YVYU_P10_16BIT_LSB:
    case FORMAT_YVYU_P10_32BIT_MSB:
    case FORMAT_YVYU_P10_32BIT_LSB:
    case FORMAT_UYVY_P10_16BIT_MSB:
    case FORMAT_UYVY_P10_16BIT_LSB:
    case FORMAT_UYVY_P10_32BIT_MSB:
    case FORMAT_UYVY_P10_32BIT_LSB:
    case FORMAT_VYUY_P10_16BIT_MSB:
    case FORMAT_VYUY_P10_16BIT_LSB:
    case FORMAT_VYUY_P10_32BIT_MSB:
    case FORMAT_VYUY_P10_32BIT_LSB:
        nY = picHeight;
        nCb = 0;
        break;
    case FORMAT_420_P10_16BIT_LSB:
    case FORMAT_420_P10_16BIT_MSB:
        nY = picHeight;
        nCb = picHeight/2;
        chromaSize = picWidth * picHeight/2;
        chromaStride = stride / 2;
        chromaWidth = picWidth;
        picWidth *= 2;
        break;
    case FORMAT_420_P10_32BIT_LSB:
    case FORMAT_420_P10_32BIT_MSB:
        nY = picHeight;
        nCb = picHeight/2;
        picWidth = VPU_ALIGN32(picWidth);
        chromaWidth = ((VPU_ALIGN16(picWidth/2*twice)+2)/3*4);
        chromaStride = VPU_ALIGN16(stride/2)*twice;
        if ( interLeave == 1)
            chromaStride = stride;
        chromaSize = chromaWidth * picHeight/2;
        picWidth   = ((VPU_ALIGN16(picWidth)+2)/3)*4;
        break;
    default:
        nY = picHeight;
        nCb = picHeight / 2;
        chromaSize = picWidth * picHeight / 4;
        chromaStride = stride / 2;
        chromaWidth = picWidth / 2;
        break;
    }

    lumaSize = picWidth * picHeight;

    srcY  = src;
    srcCb = src + lumaSize;
    srcCr = src + lumaSize + chromaSize;

    addrY  = fb->bufY;
    addrCb = fb->bufCb;
    addrCr = fb->bufCr;

    if ( nCb )
        cnt = nY / nCb;

    if ( subFrameSyncEn == TRUE ) {
        subFrameSyncState->ipuCurFrameIndex = (1 << srcFbIndex);
        if (remainDataWrite == FALSE || (writeSrcLine == 0 && remainDataWrite == TRUE) ) {
            subFrameSyncState->ipuEndOfRow = 0;
            subFrameSyncState->ipuNewFrame = !subFrameSyncState->ipuNewFrame;           // indicate the current is not start of the frame;
            VPU_EncGiveCommand(handle , ENC_SET_SUB_FRAME_SYNC, subFrameSyncState);
        }
        else {
            YStartPos = writeSrcLine;
        }
    }

    for (y = YStartPos; y < nY; ++y) {
        vdi_write_memory(coreIdx, addrY + stride * y, (Uint8 *)(srcY + y * picWidth), picWidth, endian);
        if (format == FORMAT_400) {
            if ( subFrameSyncEn == TRUE && ( (y-YStartPos+1) > (64-1) && ((y-YStartPos+1) % 64) == 0) ) {
                UpdateSubFrameSyncPos(handle, subFrameSyncState, y);
                if ( writeSrcLine == y+1)
                    return TRUE;
            }
            continue;
        }
        if (format >= FORMAT_YUYV && format <= FORMAT_VYUY_P10_32BIT_LSB) {
            if ( subFrameSyncEn == TRUE && ( (y-YStartPos+1) > (64-1) && ((y-YStartPos+1) % 64) == 0) ) {
                UpdateSubFrameSyncPos(handle, subFrameSyncState, y);
                if ( writeSrcLine == y+1 )
                    return TRUE;
            }
            continue;
        }
        if (interLeave == TRUE) {
            if (cnt == 2 && ((y%2)==0))
                vdi_write_memory(coreIdx, addrCb + stride * y/cnt, (Uint8 *)(srcCb+ y/cnt * picWidth), picWidth, endian);
        }
        else {
            if (cnt == 2 && ((y%2)==0)) {
                vdi_write_memory(coreIdx, addrCb + chromaStride * y/cnt, (Uint8 *)(srcCb + y/cnt * chromaWidth), chromaWidth, endian);
                vdi_write_memory(coreIdx, addrCr + chromaStride * y/cnt, (Uint8 *)(srcCr + y/cnt * chromaWidth), chromaWidth, endian);
            }
        }
        if ( subFrameSyncEn == TRUE && ( (y-YStartPos+1) > (64-1) && ((y-YStartPos+1) % 64) == 0) ) {
            UpdateSubFrameSyncPos(handle, subFrameSyncState, y);
            if ( writeSrcLine == y+1 )
                return TRUE;
        }
    }
    //remain y line
    if ( subFrameSyncEn == TRUE && ( (y % 64) != 0) ) {//y is already added number one at for loop
        UpdateSubFrameSyncPos(handle, subFrameSyncState, y-1);
    }

    return TRUE;//lint !e438
}

//////////////////// DRAM Read/Write helper Function ////////////////////////////
BOOL LoadYuvImageBurstFormat(
    Uint32      coreIdx, 
    Uint8*      src, 
    size_t      picWidth, 
    size_t      picHeight,
    FrameBuffer* fb,
    BOOL        convertCbcrIntl
    )
{
    Int32               y, nY, nCb, nCr;
    PhysicalAddress     addr;
    size_t              lumaSize, chromaSize=0, chromaStride = 0, chromaWidth=0;
    Uint8*              puc;
    size_t              stride      = fb->stride;
    EndianMode          endian      = (EndianMode)fb->endian;
    FrameBufferFormat   format      = fb->format;
    BOOL                interLeave  = fb->cbcrInterleave;
    int                 twice       = 1 << interLeave;

    switch (format) {
    case FORMAT_420:
        nY = picHeight;
        nCb = nCr = picHeight / 2;
        chromaSize = picWidth * picHeight / 4;
        chromaStride = stride / 2;
        chromaWidth = picWidth / 2;
        break;
    case FORMAT_224:
        nY = picHeight;
        nCb = nCr = picHeight / 2;
        chromaSize = picWidth * picHeight / 2;
        chromaStride = stride;
        chromaWidth = picWidth;
        break;
    case FORMAT_422:
        nY = picHeight;
        nCb = nCr = picHeight;
        chromaSize = picWidth * picHeight / 2;
        chromaStride = stride / 2;
        chromaWidth = picWidth / 2;
        break;
    case FORMAT_444:
        nY = picHeight;
        nCb = nCr = picHeight;
        chromaSize = picWidth * picHeight;
        chromaStride = stride;
        chromaWidth = picWidth;
        break;
    case FORMAT_400:
        nY = picHeight;
        nCb = nCr = 0;
        chromaSize = picWidth * picHeight / 4;
        chromaStride = stride / 2;
        chromaWidth = picWidth / 2;
        break;
    case FORMAT_YUYV:
    case FORMAT_YVYU:
    case FORMAT_UYVY:
    case FORMAT_VYUY:
    case FORMAT_YUYV_P10_16BIT_MSB:
    case FORMAT_YUYV_P10_16BIT_LSB:
    case FORMAT_YUYV_P10_32BIT_MSB:
    case FORMAT_YUYV_P10_32BIT_LSB:
    case FORMAT_YVYU_P10_16BIT_MSB:
    case FORMAT_YVYU_P10_16BIT_LSB:
    case FORMAT_YVYU_P10_32BIT_MSB:
    case FORMAT_YVYU_P10_32BIT_LSB:
    case FORMAT_UYVY_P10_16BIT_MSB:
    case FORMAT_UYVY_P10_16BIT_LSB:
    case FORMAT_UYVY_P10_32BIT_MSB:
    case FORMAT_UYVY_P10_32BIT_LSB:
    case FORMAT_VYUY_P10_16BIT_MSB:
    case FORMAT_VYUY_P10_16BIT_LSB:
    case FORMAT_VYUY_P10_32BIT_MSB:
    case FORMAT_VYUY_P10_32BIT_LSB:
        nY = picHeight;
        nCb = nCr = 0;
        break;
    case FORMAT_420_P10_16BIT_LSB:
    case FORMAT_420_P10_16BIT_MSB:
        nY = picHeight;
        nCb = nCr = picHeight/2;
        chromaSize = picWidth * picHeight/2;
        chromaStride = stride / 2;
        chromaWidth = picWidth;
        picWidth *= 2;
        break;
    case FORMAT_420_P10_32BIT_LSB:
    case FORMAT_420_P10_32BIT_MSB:
        nY = picHeight;
        nCb = nCr = picHeight/2;
        picWidth = VPU_ALIGN32(picWidth);
        chromaWidth = ((VPU_ALIGN16(picWidth/2*twice)+2)/3*4);
        chromaStride = VPU_ALIGN16(stride/2)*twice;
        if ( interLeave == 1)
            chromaStride = stride;
        chromaSize = chromaWidth * picHeight/2;
        picWidth   = ((VPU_ALIGN16(picWidth)+2)/3)*4;
        break;
    default:
        nY = picHeight;
        nCb = nCr = picHeight / 2;
        chromaSize = picWidth * picHeight / 4;
        chromaStride = stride / 2;
        chromaWidth = picWidth / 2;
        break;
    }

    puc = src;
    addr = fb->bufY;
    lumaSize = picWidth * picHeight;

    if( picWidth == stride) { // for fast write
        vdi_write_memory(coreIdx, addr, (Uint8 *)( puc ), lumaSize, endian);

        if( format == FORMAT_400)
            return FALSE;
        if (format >= FORMAT_YUYV && format <= FORMAT_VYUY_P10_32BIT_LSB)
            return FALSE;

        if (interLeave == TRUE) {
            UNREFERENCED_PARAMETER(convertCbcrIntl);
            puc = src + lumaSize;
            addr = fb->bufCb;
            vdi_write_memory(coreIdx, addr, (Uint8 *)puc, chromaSize*2, endian);
        }
        else {
            if ( chromaWidth == chromaStride )
            {
                puc = src + lumaSize;
                addr = fb->bufCb;
                vdi_write_memory(coreIdx, addr, (Uint8 *)puc, chromaSize, endian);

                puc = src + lumaSize + chromaSize;
                addr = fb->bufCr;
                vdi_write_memory(coreIdx, addr, (Uint8 *)puc, chromaSize, endian);
            }
            else
            {
                puc = src + lumaSize;
                addr = fb->bufCb;
                for (y = 0; y < nCb; ++y) {
                    vdi_write_memory(coreIdx, addr + chromaStride * y, (Uint8 *)(puc + y * chromaWidth), chromaWidth, endian);
                }

                puc = src + lumaSize + chromaSize;
                addr = fb->bufCr;
                for (y = 0; y < nCr; ++y) {
                    vdi_write_memory(coreIdx, addr + chromaStride * y, (Uint8 *)(puc + y * chromaWidth), chromaWidth, endian);
                }
            }
        }
    }
    else {
        for (y = 0; y < nY; ++y) {
            vdi_write_memory(coreIdx, addr + stride * y, (Uint8 *)(puc + y * picWidth), picWidth, endian);
        }

        if (format == FORMAT_400) {
            return FALSE;
        }
        if (format >= FORMAT_YUYV && format <= FORMAT_VYUY_P10_32BIT_LSB) {
            return FALSE;
        }

        if (interLeave == TRUE) {
            UNREFERENCED_PARAMETER(convertCbcrIntl);
            puc = src + lumaSize;
            addr = fb->bufCb;
            for (y = 0; y < nCb; ++y) {
                vdi_write_memory(coreIdx, addr + stride * y, (Uint8 *)(puc + y * picWidth), picWidth, endian);
            }
        }
        else {
            puc = src + lumaSize;
            addr = fb->bufCb;
            for (y = 0; y < nCb; ++y) {
                vdi_write_memory(coreIdx, addr + chromaStride * y, (Uint8 *)(puc + y * chromaWidth), chromaWidth, endian);
            }

            puc = src + lumaSize + chromaSize;
            addr = fb->bufCr;
            for (y = 0; y < nCr; ++y) {
                vdi_write_memory(coreIdx, addr + chromaStride * y, (Uint8 *)(puc + y * chromaWidth), chromaWidth, endian);
            }
        }
    }

    return TRUE;//lint !e438
}

BOOL LoadTiledImageYuvBurst(
    VpuHandle       handle,
    Uint32          coreIdx, 
    BYTE*           pYuv, 
    size_t          picWidth, 
    size_t          picHeight, 
    FrameBuffer*    fb, 
    TiledMapConfig  mapCfg
    )
{   
    BYTE *pSrc;
    size_t              divX, divY;
    PhysicalAddress     pix_addr;
    size_t              rrow, ccol;
    size_t              offsetX,offsetY;
    size_t              stride_c;
    size_t              stride      = fb->stride;
    EndianMode          endian      = (EndianMode)fb->endian;
    FrameBufferFormat   format      = fb->format;
    BOOL                interLeave  = fb->cbcrInterleave;
    Int32               productId;
    Int32               dramBusWidth = 8;

    productId = VPU_GetProductId(coreIdx);
    if (PRODUCT_ID_W_SERIES(productId)) {
        dramBusWidth = 16;
    }

    offsetX = offsetY    = 0;

    divX = format == FORMAT_420 || format == FORMAT_422 ? 2 : 1;
    divY = format == FORMAT_420 || format == FORMAT_224 ? 2 : 1;

    switch (format) {
    case FORMAT_400:
        stride_c = 0;
        break;
    case FORMAT_420:
    case FORMAT_422:
        stride_c = stride / 2;
        break;
    case FORMAT_224:
    case FORMAT_444:
        stride_c = stride;
        break;
    default:
        stride_c = stride / 2;
        break;
    }

    // Y
    pSrc    = pYuv;

    // no opt code
    for (rrow=0; rrow <picHeight; rrow=rrow+1) 
    {
        for (ccol=0; ccol<picWidth; ccol=ccol+dramBusWidth)
        {    
            pix_addr = GetXY2AXIAddr(&mapCfg, 0/*luma*/, rrow +offsetY, ccol + offsetX, stride, fb);
            vdi_write_memory(coreIdx, pix_addr, pSrc+rrow*picWidth+ccol, 8, endian);
        }
    }

    if (format == FORMAT_400) {
        return 1;
    }

    if (interLeave == FALSE) { 
        // CB
        pSrc = pYuv + picWidth*picHeight;

        for (rrow=0; rrow <(picHeight/divY) ; rrow=rrow+1) {
            for (ccol=0; ccol<(picWidth/divX); ccol=ccol+dramBusWidth) {
                pix_addr = GetXY2AXIAddr(&mapCfg, 2, rrow + offsetY, ccol +offsetX, stride_c, fb);
                vdi_write_memory(coreIdx, pix_addr, pSrc+rrow*picWidth/divX+ccol, 8, endian);
            }
        }
        // CR

        pSrc = pYuv + picWidth*picHeight+ (picWidth/divX)*(picHeight/divY);

        for (rrow=0; rrow <picHeight/divY ; rrow=rrow+1) {
            for (ccol=0; ccol<picWidth/divX; ccol=ccol+dramBusWidth) {
                pix_addr = GetXY2AXIAddr(&mapCfg, 3, rrow  + offsetY ,ccol +offsetX, stride_c, fb);
                vdi_write_memory(coreIdx, pix_addr, pSrc+rrow*picWidth/divX+ccol, 8, endian);
            }
        }
    }
    else {

        BYTE * pTemp;
        BYTE * srcAddrCb;
        BYTE * srcAddrCr;

        size_t  cbcr_x;

        switch( format) {
        case FORMAT_444 :
            cbcr_x = picWidth*2;
            break;
        case FORMAT_420 :
            cbcr_x = picWidth  ;
            break;
        case FORMAT_422 :
            cbcr_x = picWidth  ;
            break;
        case FORMAT_224 :
            cbcr_x = picWidth*2;
            break;
        default:
            cbcr_x = picWidth  ;
            break;
        }

        stride = stride_c * 2;

        srcAddrCb = pYuv + picWidth*picHeight;
        srcAddrCr = pYuv + picWidth*picHeight + picWidth/divX*picHeight/divY;


        pTemp = (BYTE*)osal_malloc(sizeof(char)*8);
        if (!pTemp) {
            return FALSE;
        }

        for (rrow=0; rrow <picHeight/divY; rrow=rrow+1) {
            for (ccol=0; ccol<cbcr_x ; ccol=ccol+dramBusWidth) {     

                pTemp[0  ] = *srcAddrCb++;
                pTemp[0+2] = *srcAddrCb++;
                pTemp[0+4] = *srcAddrCb++;
                pTemp[0+6] = *srcAddrCb++;
                pTemp[0+1] = *srcAddrCr++;
                pTemp[0+3] = *srcAddrCr++;
                pTemp[0+5] = *srcAddrCr++;
                pTemp[0+7] = *srcAddrCr++;

                pix_addr = GetXY2AXIAddr(&mapCfg, 2, rrow + offsetY ,ccol + (offsetX*2), stride, fb);  
                vdi_write_memory(coreIdx, pix_addr, (unsigned char *)pTemp, 8, endian);
            }
        }
        osal_free(pTemp);
    }

    return TRUE;
}


static void SwapDword(unsigned char* data, int len)
{
    Uint32  temp;
    Uint32* ptr = (Uint32*)data;
    Int32   i, size = len/sizeof(Uint32);

    for (i=0; i<size; i+=2) {
        temp      = ptr[i];
        ptr[i]   = ptr[i+1];
        ptr[i+1] = temp;
    }
}

static void SwapLword(unsigned char* data, int len)
{
    Uint64  temp;
    Uint64* ptr = (Uint64*)data;
    Int32   i, size = len/sizeof(Uint64);

    for (i=0; i<size; i+=2) {
        temp      = ptr[i];
        ptr[i]   = ptr[i+1];
        ptr[i+1] = temp;
    }
}

static void SwapPixelOrder(
    Uint8*      data
    )
{
    Uint32*     temp;
    Uint32      temp2[4]={0,};
    Int32       i,j;

    for (i=0, j=3 ; i < 16 ; i+=4, j--) {
        temp = (Uint32*)(data+i);
        temp2[j] =  (*temp & 0xffc00000)>>20;
        temp2[j] |= (*temp & 0x003ff000);
        temp2[j] |= (*temp & 0x00000ffc)<<20;
    }

    osal_memcpy(data, temp2, 16);

//for matching with Ref-C
    SwapDword(data, 16);
    SwapLword(data, 16);
}

Uint32 StoreYuvImageBurstLinear(
    Uint32      coreIdx, 
    FrameBuffer *fbSrc, 
    TiledMapConfig  mapCfg, 
    Uint8       *pDst, 
    VpuRect     cropRect, 
    BOOL        enableCrop,
    BOOL        isVP9
    )
{
    Uint32          y, x;
    Uint32          pix_addr, div_x, div_y, chroma_stride=0;
    Uint8*          puc;
    Uint8*          rowBufferY, *rowBufferCb, *rowBufferCr;
    Uint32          stride      = fbSrc->stride;
    Uint32          height      = fbSrc->height;
    int             interLeave  = fbSrc->cbcrInterleave;
    BOOL            nv21        = fbSrc->nv21;
    EndianMode      endian      = (EndianMode)fbSrc->endian;
    FrameBufferFormat format    = (FrameBufferFormat)fbSrc->format;
    Uint32          width;
    Uint32          dstWidth=0, dstHeight=0;
    Uint32          offsetX, offsetY;
    Uint32          dstChromaHeight = 0;
    Uint32          dstChromaWidth = 0;
    Uint32          chromaHeight = 0;
    Uint32          bpp=8;
    Uint32          p10_32bit_interleave = 0;
    Int32           productId;
    Int32           dramBusWidth = 8;
    Uint32          totSize = 0;
    BOOL            copyLumaOnly = FALSE;

    //Int32           addr;
    Int32           baseY;
    Int32           baseCb;
    Int32           baseCr;
    Uint8           *pY;
    Uint8           *pCbTemp;
    Uint8           *pCb;
    Uint8           *pCr;

    productId = VPU_GetProductId(coreIdx);
    if (PRODUCT_ID_W_SERIES(productId)) {
        dramBusWidth = 16;
    }
    switch (fbSrc->format) {
    case FORMAT_420:
    case FORMAT_420_P10_16BIT_LSB:
    case FORMAT_420_P10_16BIT_MSB:
    case FORMAT_420_P10_32BIT_LSB:
    case FORMAT_420_P10_32BIT_MSB:
    case FORMAT_422:
    case FORMAT_422_P10_16BIT_LSB:
    case FORMAT_422_P10_16BIT_MSB:
    case FORMAT_422_P10_32BIT_LSB:
    case FORMAT_422_P10_32BIT_MSB:
        div_x = 2;
        break;
    default:
        div_x = 1;
    }

    switch (fbSrc->format) {
    case FORMAT_420:
    case FORMAT_420_P10_16BIT_LSB:
    case FORMAT_420_P10_16BIT_MSB:
    case FORMAT_420_P10_32BIT_LSB:
    case FORMAT_420_P10_32BIT_MSB:
    case FORMAT_224:
        div_y = 2;
        break;
    default:
        div_y = 1;
    }

    //for matching with Ref-C
    width     = (enableCrop == TRUE ? cropRect.right - cropRect.left : stride);
    dstHeight = (enableCrop == TRUE ? cropRect.bottom - cropRect.top : height);
    offsetX   = (enableCrop == TRUE ? cropRect.left : 0);
    offsetY   = (enableCrop == TRUE ? cropRect.top  : 0);

    switch (fbSrc->format) {
    case FORMAT_400:
        copyLumaOnly = TRUE;
        break;
    case FORMAT_YUYV:
    case FORMAT_YVYU:
    case FORMAT_UYVY:
    case FORMAT_VYUY:
        copyLumaOnly    = TRUE;
        dstWidth        = width * 2;
        dstChromaHeight = 0;
        chromaHeight    = 0;
        break;
    case FORMAT_YUYV_P10_16BIT_LSB:
    case FORMAT_YUYV_P10_16BIT_MSB:
    case FORMAT_YVYU_P10_16BIT_LSB:
    case FORMAT_YVYU_P10_16BIT_MSB:
    case FORMAT_UYVY_P10_16BIT_LSB:
    case FORMAT_UYVY_P10_16BIT_MSB:
    case FORMAT_VYUY_P10_16BIT_LSB:
    case FORMAT_VYUY_P10_16BIT_MSB:
        copyLumaOnly    = TRUE;
        dstWidth        = (width * 2)*2;
        dstChromaHeight = 0;
        chromaHeight    = 0;
        break;
    case FORMAT_YUYV_P10_32BIT_LSB:
    case FORMAT_YUYV_P10_32BIT_MSB:
    case FORMAT_YVYU_P10_32BIT_LSB:
    case FORMAT_YVYU_P10_32BIT_MSB:
    case FORMAT_UYVY_P10_32BIT_LSB:
    case FORMAT_UYVY_P10_32BIT_MSB:
    case FORMAT_VYUY_P10_32BIT_LSB:
    case FORMAT_VYUY_P10_32BIT_MSB:
        copyLumaOnly    = TRUE;
        dstWidth        = ((width+2)/3*4)*2;
        dstChromaHeight = 0;
        chromaHeight    = 0;
        break;
    case FORMAT_422_P10_16BIT_LSB: 
    case FORMAT_422_P10_16BIT_MSB:
        dstWidth = width * 2;
        bpp = 16;
        dstChromaWidth  = dstWidth / div_x;
        dstChromaHeight = dstHeight / div_y;
        chromaHeight    = height / div_y;
        chroma_stride   = (stride / div_x);
        break;
    case FORMAT_420_P10_16BIT_LSB: 
    case FORMAT_420_P10_16BIT_MSB:
        dstWidth = width * 2;
        bpp = 16;
        dstChromaWidth  = dstWidth / div_x;
        dstChromaHeight = dstHeight / div_y;
        chromaHeight    = height / div_y;
        chroma_stride = (stride / div_x);
        break;
    case FORMAT_420_P10_32BIT_LSB:
    case FORMAT_420_P10_32BIT_MSB:
#ifdef DUMP_YUV_WHOLE_DATA
        if (interLeave)
        {
            dstChromaWidth = ((VPU_ALIGN16(width/div_x))*2+11)/12*16;
            dstChromaWidth = VPU_ALIGN16(dstChromaWidth);
            dstChromaHeight = dstHeight / div_y;

            stride = stride;
            chroma_stride = stride;
            dstWidth = (VPU_ALIGN16(width)+11)/12*16;

            interLeave = 0;
        }
        else
        {
            dstChromaWidth = ((VPU_ALIGN16(width/div_x))+11)/12*16;
            dstChromaWidth = VPU_ALIGN16(dstChromaWidth);
            dstChromaHeight = dstHeight / div_y;
            chroma_stride = dstChromaWidth;
            stride    = (VPU_ALIGN16(stride)+11)/12*16;
            dstWidth = (VPU_ALIGN16(dstWidth)+11)/12*16;
        }
        break;
#else
        if (interLeave) {
            dstChromaWidth = ((VPU_ALIGN16(width*2/div_x))+11)/12*16;
            dstChromaWidth = VPU_ALIGN16(dstChromaWidth);
            if(isVP9 == TRUE) {
                dstChromaWidth = VPU_ALIGN32(dstChromaWidth);
            }
            chroma_stride = stride;

            dstChromaWidth = (width/div_x+2)/3*4;

            dstChromaHeight = dstHeight / div_y;
            chromaHeight    = height / div_y;

            dstWidth = (width+2)/3*4;

            interLeave = 0;
            p10_32bit_interleave = 1;
        }
        else {
            //dstChromaWidth = ((VPU_ALIGN16(width/div_x))+11)/12*16;
//          dstChromaWidth = VPU_ALIGN16(dstChromaWidth);
            //chroma_stride = dstChromaWidth;
            chroma_stride = stride / 2;

            dstChromaWidth = (width/2+2)/3*4;

            dstChromaHeight = dstHeight / div_y;
            chromaHeight    = height / div_y;

            dstWidth = (width+2)/3*4;
        }
        break;
#endif
    default:
        dstWidth = width;
        dstChromaWidth  = width / div_x;
        dstChromaHeight = dstHeight / div_y;
        chromaHeight    = height / div_y;
        chroma_stride   = (stride / div_x);
        break;
    }

    puc         = pDst;

    pY = (Uint8*)osal_malloc(stride * height);
    if ( !pY )
        return 0;
    pCbTemp = (Uint8*)osal_malloc(stride*4 * chromaHeight);
    if ( !pCbTemp )
        return 0;
    pCb = (Uint8*)osal_malloc(stride*4 * chromaHeight);
    if ( !pCb )
        return 0;
    pCr = (Uint8*)osal_malloc(stride*2 * chromaHeight);
    if ( !pCr )
        return 0;
    baseY = fbSrc->bufY;
    baseCb = fbSrc->bufCb;
    baseCr = fbSrc->bufCr;

    vdi_read_memory(coreIdx, fbSrc->bufY, pY, stride * height, endian);

    for (y=0 ; y<dstHeight ; y+=1) {
        pix_addr = GetXY2AXIAddr(&mapCfg, 0, y+offsetY, 0, stride, fbSrc);
        rowBufferY = pY + (pix_addr - baseY);
        // CHECK POINT
        for (x=0; x<stride ; x+=dramBusWidth) {  
            if ( fbSrc->format == FORMAT_420_P10_32BIT_MSB )
                SwapPixelOrder(rowBufferY+x);
        }
        // CHECK POINT
        osal_memcpy(puc+y*dstWidth, rowBufferY+offsetX, dstWidth);
        totSize += dstWidth;
    }

    if (copyLumaOnly == TRUE) {
        osal_free(pY);
        osal_free(pCb);
        osal_free(pCr);
        osal_free(pCbTemp);
        return totSize;
    }

    if (interLeave || p10_32bit_interleave) {
        Int32    cbcr_per_2pix=1;

        cbcr_per_2pix = (format==FORMAT_224||format==FORMAT_444) ? 2 : 1;
        vdi_read_memory(coreIdx, fbSrc->bufCb, pCbTemp, stride*cbcr_per_2pix * chromaHeight, endian);
    } else {
        vdi_read_memory(coreIdx, fbSrc->bufCb, pCb, chroma_stride * chromaHeight, endian);
        if ( (fbSrc->format == FORMAT_420_P10_32BIT_LSB || fbSrc->format == FORMAT_420_P10_32BIT_MSB) &&
            p10_32bit_interleave == 1) {
            // Nothing to do
        }
        else {
            vdi_read_memory(coreIdx, fbSrc->bufCr, pCr, chroma_stride * chromaHeight, endian);
        }
    }
        
    if (interLeave == TRUE || p10_32bit_interleave == TRUE) {
        //Uint8    pTemp[16];
        Uint8*   pTemp;
        Uint8*   dstAddrCb;
        Uint8*   dstAddrCr;
        Uint8*   ptrCb, *ptrCr;
        Int32    cbcr_per_2pix=1, k;
        Uint32*   pTempLeft32, *pTempRight32;
        Uint32   temp_32;

        dstAddrCb = pDst + dstWidth*dstHeight; 
        dstAddrCr = dstAddrCb + dstChromaWidth*dstChromaHeight;

        cbcr_per_2pix = (format==FORMAT_224||format==FORMAT_444) ? 2 : 1;

        for ( y = 0 ; y < dstChromaHeight; ++y ) {
            ptrCb = pCb;
            ptrCr = pCr;
            for ( x = 0 ; x < stride*cbcr_per_2pix ; x += dramBusWidth ) {
                pix_addr = GetXY2AXIAddr(&mapCfg, 2, y+(offsetY/div_y), x, stride, fbSrc);
                pTemp = pCbTemp + (pix_addr - baseCb);
                // CHECK POINT
                if ( fbSrc->format == FORMAT_420_P10_32BIT_MSB )
                    SwapPixelOrder(pTemp);
                // CHECK POINT

                if (interLeave == TRUE) {
                    for (k=0; k<dramBusWidth && (x+k) < stride; k+=(2*bpp/8)) {
                        if (bpp == 8) {
                            if (nv21) {
                                *ptrCr++ = pTemp[k];
                                *ptrCb++ = pTemp[k+1];
                            }
                            else {
                                *ptrCb++ = pTemp[k];
                                *ptrCr++ = pTemp[k+1];
                            }
                        } 
                        else {
                            if (nv21) {
                                *ptrCr++ = pTemp[k];
                                *ptrCr++ = pTemp[k+1];
                                *ptrCb++ = pTemp[k+2];
                                *ptrCb++ = pTemp[k+3];
                            }
                            else {
                                *ptrCb++ = pTemp[k];
                                *ptrCb++ = pTemp[k+1];
                                *ptrCr++ = pTemp[k+2];
                                *ptrCr++ = pTemp[k+3];
                            }
                        }
                    }
                }
                else {
                    for (k=0; k<dramBusWidth && (x+k) < stride; k+=8) {//(2*bpp/8)) {
                        pTempLeft32 = (Uint32*)&pTemp[k];
                        pTempRight32 = (Uint32*)&pTemp[k+4];

                        if (format==FORMAT_420_P10_32BIT_MSB) {
                            temp_32 = *pTempLeft32 & 0x003ff000;
                            *pTempLeft32 = (*pTempLeft32 & 0xffc00000)
                                | (*pTempLeft32 & 0x00000ffc) << 10
                                | (*pTempRight32 & 0x003ff000) >> 10;
                            *pTempRight32 = (temp_32) << 10
                                | (*pTempRight32 & 0xffc00000) >> 10
                                | (*pTempRight32 & 0x00000ffc);
                        }
                        else if (format==FORMAT_420_P10_32BIT_LSB) {
                            temp_32 = *pTempLeft32 & 0x000ffc00;
                            *pTempLeft32 = (*pTempLeft32 & 0x000003ff)
                                | (*pTempLeft32 & 0x3ff00000) >> 10
                                | (*pTempRight32 & 0x000ffc00) << 10;
                            *pTempRight32 = (temp_32) >> 10
                                | (*pTempRight32 & 0x000003ff) << 10
                                | (*pTempRight32 & 0x3ff00000);
                        }

                        if (nv21) {
                            *ptrCr++ = pTemp[k];
                            *ptrCr++ = pTemp[k+1];
                            *ptrCr++ = pTemp[k+2];
                            *ptrCr++ = pTemp[k+3];
                            *ptrCb++ = pTemp[k+4];
                            *ptrCb++ = pTemp[k+5];
                            *ptrCb++ = pTemp[k+6];
                            *ptrCb++ = pTemp[k+7];
                        }
                        else {
                            *ptrCb++ = pTemp[k];
                            *ptrCb++ = pTemp[k+1];
                            *ptrCb++ = pTemp[k+2];
                            *ptrCb++ = pTemp[k+3];
                            *ptrCr++ = pTemp[k+4];
                            *ptrCr++ = pTemp[k+5];
                            *ptrCr++ = pTemp[k+6];
                            *ptrCr++ = pTemp[k+7];
                        }
                    }
                }
            }
            osal_memcpy(dstAddrCb+y*dstChromaWidth, pCb+offsetX/div_x, dstChromaWidth);
            totSize += dstChromaWidth;
            osal_memcpy(dstAddrCr+y*dstChromaWidth, pCr+offsetX/div_x, dstChromaWidth);
            totSize += dstChromaWidth;
        }
    }
    else {      
        puc = pDst + dstWidth*dstHeight; 

        for (y = 0 ; y < dstChromaHeight; y += 1) {
            x = 0;
            pix_addr = GetXY2AXIAddr(&mapCfg, 2, y+(offsetY/div_y), x, chroma_stride, fbSrc);
            rowBufferCb = pCb + (pix_addr - baseCb);
            // CHECK POINT
            for (x = 0 ; x < chroma_stride; x += dramBusWidth) {
                if ( fbSrc->format == FORMAT_420_P10_32BIT_MSB )
                    SwapPixelOrder(rowBufferCb+x);
            }
            // CHECK POINT
            osal_memcpy(puc + (y*dstChromaWidth), rowBufferCb+offsetX/div_x, dstChromaWidth);
            totSize += dstChromaWidth;
        }

        puc += dstChromaWidth * dstChromaHeight;
        if ( (fbSrc->format == FORMAT_420_P10_32BIT_LSB || fbSrc->format == FORMAT_420_P10_32BIT_MSB) &&
            p10_32bit_interleave == 1)
        {
        }
        else
        {
            for (y = 0 ; y < dstChromaHeight; y += 1) {
                x = 0;
                pix_addr = GetXY2AXIAddr(&mapCfg, 3, y+(offsetY/div_y), x, chroma_stride, fbSrc);
                //vdi_read_memory(coreIdx, pix_addr, rowBufferCr+x, dramBusWidth,  endian);
                rowBufferCr = pCr + (pix_addr - baseCr);
                // CHECK POINT
                for ( x = 0 ; x < chroma_stride; x += dramBusWidth ) {
                    if ( fbSrc->format == FORMAT_420_P10_32BIT_MSB )
                        SwapPixelOrder(rowBufferCr+x);
                }
                // CHECK POINT
                osal_memcpy(puc + (y*dstChromaWidth), rowBufferCr+offsetX/div_x, dstChromaWidth);
                totSize += dstChromaWidth;
            }
        }
    }

    osal_free(pY);
    osal_free(pCb);
    osal_free(pCr);
    osal_free(pCbTemp);

    return totSize;
}

Uint32 StoreYuvImageBurstFormat(
    Uint32          coreIdx, 
    FrameBuffer*    fbSrc, 
    TiledMapConfig  mapCfg, 
    Uint8*          pDst, 
    VpuRect         cropRect, 
    BOOL            enableCrop
    )
{
    Uint32          y, x;
    Uint32          pix_addr, div_x, div_y, chroma_stride;
    Uint8*          puc;
    Uint8*          rowBufferY, *rowBufferCb, *rowBufferCr;
    Uint32          stride      = fbSrc->stride;
    Uint32          height      = fbSrc->height;
    int             interLeave  = fbSrc->cbcrInterleave;
    BOOL            nv21        = fbSrc->nv21;
    EndianMode      endian      = (EndianMode)fbSrc->endian;
    FrameBufferFormat format    = (FrameBufferFormat)fbSrc->format;
    Uint32          width;
    Uint32          dstWidth, dstHeight;
    Uint32          offsetX, offsetY;
    Uint32          dstChromaHeight;
    Uint32          dstChromaWidth;
    Uint32          bpp=8;
    Uint32          p10_32bit_interleave = 0;
    Int32           productId;
    Int32           dramBusWidth = 8;
        Uint32                  totSize = 0;

    productId = VPU_GetProductId(coreIdx);
    if (PRODUCT_ID_W_SERIES(productId)) {
        dramBusWidth = 16;
    }
    switch (fbSrc->format) {
    case FORMAT_420:
    case FORMAT_420_P10_16BIT_LSB:
    case FORMAT_420_P10_16BIT_MSB:
    case FORMAT_420_P10_32BIT_LSB:
    case FORMAT_420_P10_32BIT_MSB:
    case FORMAT_422:
    case FORMAT_422_P10_16BIT_LSB:
    case FORMAT_422_P10_16BIT_MSB:
    case FORMAT_422_P10_32BIT_LSB:
    case FORMAT_422_P10_32BIT_MSB:
        div_x = 2;
        break;
    default:
        div_x = 1;
    }

    switch (fbSrc->format) {
    case FORMAT_420:
    case FORMAT_420_P10_16BIT_LSB:
    case FORMAT_420_P10_16BIT_MSB:
    case FORMAT_420_P10_32BIT_LSB:
    case FORMAT_420_P10_32BIT_MSB:
    case FORMAT_224:
        div_y = 2;
        break;
    default:
        div_y = 1;
    }

    width     = (enableCrop == TRUE ? cropRect.right - cropRect.left : stride);
    dstHeight = (enableCrop == TRUE ? cropRect.bottom - cropRect.top : height);
    offsetX   = (enableCrop == TRUE ? cropRect.left : 0);
    offsetY   = (enableCrop == TRUE ? cropRect.top  : 0);

    switch (fbSrc->format) {
    case FORMAT_420_P10_16BIT_LSB: 
    case FORMAT_420_P10_16BIT_MSB:
        dstWidth = width * 2;
        bpp = 16;
        dstChromaWidth  = dstWidth / div_x;
        dstChromaHeight = dstHeight / div_y;
        chroma_stride   = (stride / div_x);
        break;
    case FORMAT_420_P10_32BIT_LSB:
    case FORMAT_420_P10_32BIT_MSB:
#ifdef DUMP_YUV_WHOLE_DATA
        if (interLeave)
        {
            dstChromaWidth = ((VPU_ALIGN16(width/div_x))*2+11)/12*16;
            dstChromaWidth = VPU_ALIGN16(dstChromaWidth);
            dstChromaHeight = dstHeight / div_y;

            stride = stride;
            chroma_stride = stride;
            dstWidth = (VPU_ALIGN16(width)+11)/12*16;

            interLeave = 0;
        }
        else
        {
            dstChromaWidth = ((VPU_ALIGN16(width/div_x))+11)/12*16;
            dstChromaWidth = VPU_ALIGN16(dstChromaWidth);
            dstChromaHeight = dstHeight / div_y;
            chroma_stride = dstChromaWidth;
            stride    = (VPU_ALIGN16(stride)+11)/12*16;
            dstWidth = (VPU_ALIGN16(dstWidth)+11)/12*16;
        }
        break;
#else
        if (interLeave)
        {
            dstChromaWidth = ((VPU_ALIGN16(width*2/div_x))+11)/12*16;
            dstChromaWidth = VPU_ALIGN16(dstChromaWidth);
            chroma_stride = stride;

            dstChromaWidth = (width+2)/3*4;
            dstChromaHeight = dstHeight / div_y;

            dstWidth = (width+2)/3*4;

            interLeave = 0;
            p10_32bit_interleave = 1;
        }
        else
        {
            dstChromaWidth = ((VPU_ALIGN16(width/div_x))+11)/12*16;
            dstChromaWidth = VPU_ALIGN16(dstChromaWidth);
            chroma_stride = dstChromaWidth;

            dstChromaWidth = (width/2+2)/3*4;
            dstChromaHeight = dstHeight / div_y;

            dstWidth = (width+2)/3*4;
        }
        break;
#endif
    case FORMAT_YUYV:
    case FORMAT_YUYV_P10_16BIT_MSB:
    case FORMAT_YUYV_P10_16BIT_LSB:
    case FORMAT_YUYV_P10_32BIT_MSB:
    case FORMAT_YUYV_P10_32BIT_LSB:
    case FORMAT_YVYU:
    case FORMAT_YVYU_P10_16BIT_MSB:
    case FORMAT_YVYU_P10_16BIT_LSB:
    case FORMAT_YVYU_P10_32BIT_MSB:
    case FORMAT_YVYU_P10_32BIT_LSB:
    case FORMAT_UYVY:
    case FORMAT_UYVY_P10_16BIT_MSB:
    case FORMAT_UYVY_P10_16BIT_LSB:
    case FORMAT_UYVY_P10_32BIT_MSB:
    case FORMAT_UYVY_P10_32BIT_LSB:
    case FORMAT_VYUY:
    case FORMAT_VYUY_P10_16BIT_MSB:
    case FORMAT_VYUY_P10_16BIT_LSB:
    case FORMAT_VYUY_P10_32BIT_MSB:
    case FORMAT_VYUY_P10_32BIT_LSB:
        dstWidth        = stride;
        dstChromaWidth  = 0;
        dstChromaHeight = 0;
        chroma_stride   = 0;
        break;
    default:
        dstWidth = width;
        dstChromaWidth  = width / div_x;
        dstChromaHeight = dstHeight / div_y;
        chroma_stride = (stride / div_x);
        break;
    }

    puc         = pDst;
    rowBufferY  = (Uint8*)osal_malloc(stride);
    if ( !rowBufferY )
        return 0;
    rowBufferCb = (Uint8*)osal_malloc(stride*4);
    if ( !rowBufferCb )
        return 0;
    rowBufferCr = (Uint8*)osal_malloc(stride*2);
    if ( !rowBufferCr )
        return 0;

    for ( y=0 ; y<dstHeight ; y+=1 ) 
    {
        for ( x=0; x<stride ; x+=dramBusWidth )
        {  
            pix_addr = GetXY2AXIAddr(&mapCfg, 0, y+offsetY, x, stride, fbSrc);
            vdi_read_memory(coreIdx, pix_addr, rowBufferY+x, dramBusWidth,  endian);
        }
        osal_memcpy(puc+y*dstWidth, rowBufferY+offsetX, dstWidth);
                totSize += dstWidth;
    }

    if (format == FORMAT_400) {
        osal_free(rowBufferY);
        osal_free(rowBufferCb);
        osal_free(rowBufferCr);
        return totSize;
    }

    if (interLeave == TRUE) {
        Uint8    pTemp[16];
        Uint8*   dstAddrCb;
        Uint8*   dstAddrCr;
        Uint8*   ptrCb, *ptrCr;
        Int32    cbcr_per_2pix=1, k;

        dstAddrCb = pDst + dstWidth*dstHeight; 
        dstAddrCr = dstAddrCb + dstChromaWidth*dstChromaHeight;

        cbcr_per_2pix = (format==FORMAT_224||format==FORMAT_444) ? 2 : 1;

        for ( y = 0 ; y < dstChromaHeight; ++y ) {
            ptrCb = rowBufferCb;
            ptrCr = rowBufferCr;
            for ( x = 0 ; x < stride*cbcr_per_2pix ; x += dramBusWidth ) {
                pix_addr = GetXY2AXIAddr(&mapCfg, 2, y+(offsetY/div_y), x, stride, fbSrc);
                vdi_read_memory(coreIdx, pix_addr,  pTemp, dramBusWidth,  endian); 
                // CHECK POINT
                if ( fbSrc->format == FORMAT_420_P10_32BIT_MSB )
                    SwapPixelOrder(pTemp);
                // CHECK POINT
                for (k=0; k<dramBusWidth && (x+k) < stride; k+=(2*bpp/8)) {
                    if (bpp == 8) {
                        if (nv21) {
                            *ptrCr++ = pTemp[k];
                            *ptrCb++ = pTemp[k+1];
                        }
                        else {
                            *ptrCb++ = pTemp[k];
                            *ptrCr++ = pTemp[k+1];
                        }
                    } 
                    else {
                        if (nv21) {
                            *ptrCr++ = pTemp[k];
                            *ptrCr++ = pTemp[k+1];
                            *ptrCb++ = pTemp[k+2];
                            *ptrCb++ = pTemp[k+3];
                        }
                        else {
                            *ptrCb++ = pTemp[k];
                            *ptrCb++ = pTemp[k+1];
                            *ptrCr++ = pTemp[k+2];
                            *ptrCr++ = pTemp[k+3];
                        }
                    }
                }
            }
            osal_memcpy(dstAddrCb+y*dstChromaWidth, rowBufferCb+offsetX/div_x, dstChromaWidth);
                        totSize += dstChromaWidth;
            osal_memcpy(dstAddrCr+y*dstChromaWidth, rowBufferCr+offsetX/div_x, dstChromaWidth);
                        totSize += dstChromaWidth;
        }
    }
    else {      
        puc = pDst + dstWidth*dstHeight; 

        for (y = 0 ; y < dstChromaHeight; y += 1) {
            for (x = 0 ; x < chroma_stride; x += dramBusWidth) {
                pix_addr = GetXY2AXIAddr(&mapCfg, 2, y+(offsetY/div_y), x, chroma_stride, fbSrc);
                vdi_read_memory(coreIdx, pix_addr, rowBufferCb+x, dramBusWidth,  endian);
            }
            osal_memcpy(puc + (y*dstChromaWidth), rowBufferCb+offsetX/div_x, dstChromaWidth);
                        totSize += dstChromaWidth;
        }

        puc += dstChromaWidth * dstChromaHeight;
        if ( (fbSrc->format == FORMAT_420_P10_32BIT_LSB || fbSrc->format == FORMAT_420_P10_32BIT_MSB) &&
            p10_32bit_interleave == 1)
        {
        }
        else
        {
            for (y = 0 ; y < dstChromaHeight; y += 1) {
                for ( x = 0 ; x < chroma_stride; x += dramBusWidth ) {
                    pix_addr = GetXY2AXIAddr(&mapCfg, 3, y+(offsetY/div_y), x, chroma_stride, fbSrc);
                    vdi_read_memory(coreIdx, pix_addr, rowBufferCr+x, dramBusWidth,  endian);
                }
                osal_memcpy(puc + (y*dstChromaWidth), rowBufferCr+offsetX/div_x, dstChromaWidth);
                                totSize += dstChromaWidth;
            }
        }
    }

    osal_free(rowBufferY);
    osal_free(rowBufferCb);
    osal_free(rowBufferCr);

    return totSize;
}

Uint8* GetYUVFromFrameBuffer(
    DecHandle       decHandle,
    FrameBuffer*    fb,
    VpuRect         rcFrame,
    Uint32*         retWidth,
    Uint32*         retHeight,
    Uint32*         retBpp,
    size_t*         retSize
    )
{
    Uint32          coreIdx = VPU_HANDLE_CORE_INDEX(decHandle);
    size_t          frameSizeY;                                         // the size of luma
    size_t          frameSizeC;                                         // the size of chroma
    size_t          frameSize;                                          // the size of frame
    Uint32          Bpp = 1;                                            //!<< Byte per pixel
    Uint32          picWidth, picHeight;
    Uint8*          pYuv;
    TiledMapConfig  mapCfg;

    picWidth  = rcFrame.right - rcFrame.left;
    picHeight = rcFrame.bottom - rcFrame.top;

    CalcYuvSize(fb->format, picWidth, fb->height, fb->cbcrInterleave, &frameSizeY, &frameSizeC, &frameSize, NULL, NULL, NULL);

    switch (fb->format) {
    case FORMAT_422_P10_16BIT_MSB:
    case FORMAT_422_P10_16BIT_LSB:
    case FORMAT_420_P10_16BIT_LSB:
    case FORMAT_420_P10_16BIT_MSB:
        Bpp = 2;
        break;
    case FORMAT_420_P10_32BIT_LSB:
    case FORMAT_420_P10_32BIT_MSB:
        picWidth = (picWidth/3)*4 + ((picWidth%3) ? 4 : 0);
        Bpp = 1;
        break;
    case FORMAT_422:
    case FORMAT_422_P10_32BIT_MSB:
    case FORMAT_422_P10_32BIT_LSB:
        break;
    default:
        Bpp = 1;
        break;
    }
    {
        Int32   temp_picWidth;
        Int32   chromaWidth;

        switch (fb->format) {
        case FORMAT_420_P10_32BIT_LSB:
        case FORMAT_420_P10_32BIT_MSB:
            temp_picWidth = VPU_ALIGN32(picWidth);
            chromaWidth = ((VPU_ALIGN16(temp_picWidth / 2*(1 << 1)) + 2) / 3 * 4);
            frameSizeY = (temp_picWidth + 2) / 3 * 4 * picHeight;
            frameSizeC = chromaWidth * picHeight / 2 * 2;
            frameSize = frameSizeY + frameSizeC;
            break;
        default:
            break;
        }       
    }
    if ((pYuv=(Uint8*)osal_malloc(frameSize)) == NULL) {
        VLOG(ERR, "%s:%d Failed to allocate memory\n", __FUNCTION__, __LINE__);
        return NULL;
    }

    VPU_DecGiveCommand(decHandle, GET_TILEDMAP_CONFIG, &mapCfg);
    if (fb->mapType == LINEAR_FRAME_MAP || fb->mapType == COMPRESSED_FRAME_MAP) {
        if (decHandle->codecMode == W_VP9_DEC) {
            *retSize = StoreYuvImageBurstLinear(coreIdx, fb, mapCfg, pYuv, rcFrame, TRUE, TRUE); 
        }
        else {
            *retSize = StoreYuvImageBurstLinear(coreIdx, fb, mapCfg, pYuv, rcFrame, TRUE, FALSE);
        }
    }
    else {
        *retSize = StoreYuvImageBurstFormat(coreIdx, fb, mapCfg, pYuv, rcFrame, TRUE); 
    }

    *retWidth  = picWidth;
    *retHeight = picHeight;
    *retBpp    = Bpp; 

    return pYuv;
}

void PrepareDecoderTest(
    DecHandle decHandle
    )
{
    UNREFERENCED_PARAMETER(decHandle);
}

int ProcessEncodedBitstreamBurst(Uint32 coreIdx, osal_file_t fp, int targetAddr,
    PhysicalAddress bsBufStartAddr, PhysicalAddress bsBufEndAddr,
    int size, int endian, Comparator comparator)
{
    Uint8 * buffer = 0;
    int room = 0;
    int file_wr_size = 0;

    buffer = (Uint8 *)osal_malloc(size);
    if( ( targetAddr + size ) > (int)bsBufEndAddr )
    {
        room = bsBufEndAddr - targetAddr;
        vdi_read_memory(coreIdx, targetAddr, buffer, room,  endian);
        vdi_read_memory(coreIdx, bsBufStartAddr, buffer+room, (size-room), endian);
    }
    else
    {
        vdi_read_memory(coreIdx, targetAddr, buffer, size, endian); 
    }   

    if ( comparator) {
        if (Comparator_Act(comparator, buffer, size) == FALSE) {
            osal_free(buffer);
            return 0;
        }
    }

    if (fp) {
        file_wr_size = osal_fwrite(buffer, sizeof(Uint8), size, fp);
        osal_fflush(fp);
    }

    osal_free( buffer );

    return file_wr_size;
}



