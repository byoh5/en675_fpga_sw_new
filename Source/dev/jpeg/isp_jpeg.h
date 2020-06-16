#ifndef __ISP_JPEG_H__
#define __ISP_JPEG_H__

#define JPEG_ALIGN_SIZE   	32
#define JPEG_ALIGN(x) 		(((x) + JPEG_ALIGN_SIZE - 1) & ~(JPEG_ALIGN_SIZE - 1))

extern void enx_jpeg_reg_view(void);
extern void enx_jpeg_info_view(void);
extern UINT enx_jpeg_get_ts(void);
extern UINT enx_jpeg_get_height(void);
extern UINT enx_jpeg_get_width(void);
extern void enx_jpeg_set_dc_quantization_manual(UINT qp_DCL, UINT qp_DCC);
extern void enx_jpeg_set_dc_quantization_auto(void);
extern void jpeg_irq_handler(void *ctx);
extern UINT enx_jpeg_enc_start(UINT mode, UINT wdr, UINT dss);
extern void enx_jpeg_init(UINT raw_width, UINT raw_height);
extern void enx_jpeg_set_ts(UINT value);
extern void enx_jpeg_set_maxframe(UINT value);
extern void enx_jpeg_set_frame_size_rs(VideoResolution size);
extern void enx_jpeg_set_frame_rate_sf(void);
extern void enx_jpeg_set_quantize(UINT qp);
extern void enx_jpeg_set_bit_rate(VideoBitRateIdx cbr);
extern void enx_jpeg_set_bit_rate_mode(UINT nBrmode);
extern void enx_jpeg_cbr_processing(UINT jpg_size);

extern void enx_jpeg_on(void);

#endif // __ISP_JPEG_H__
