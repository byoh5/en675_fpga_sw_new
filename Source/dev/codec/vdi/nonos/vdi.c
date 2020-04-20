//------------------------------------------------------------------------------
// File: vdi.c
//
// Copyright (c) 2006, Chips & Media.  All rights reserved.
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "../vdi.h"
#include "../vdi_osal.h"
#include "../mm.h"
#include "coda9/coda9_regdefine.h"
#include "wave/wave5_regdefine.h"
#include "vpuapi/vpuapi.h"
#include "main_helper.h"
#include "misc/debug.h"

#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WIN32) || defined(__MINGW32__)
#elif defined(linux) || defined(__linux) || defined(ANDROID)
#else
#if (REQUIRED_VPU_MEMORY_SIZE > VPUDRV_INIT_VIDEO_MEMORY_SIZE_IN_BYTE)
#error "Warnning : VPU memory will be overflow"
#endif
#endif

#ifdef NONOS_SW_UART
void test_SwUartHandler(void);
#endif

#define VPU_BIT_REG_SIZE                    (0x4000*MAX_NUM_VPU_CORE)
#    define VPU_BIT_REG_BASE                REG_BASE_H265// //ksj porting for EN675

#ifdef EYENIX_TEST
#define VDI_SRAM_BASE_ADDR                  0x00//(0x80500000)//0x00
#else
#define VDI_SRAM_BASE_ADDR                  0x00
#endif

#define VDI_CODA9_SRAM_SIZE                 0x34600     // FHD MAX size, 0x17D00  4K MAX size 0x34600
#define VDI_WAVE511_SRAM_SIZE               0x23000     /* 10bit profile : 8Kx8K -> 143360, 4Kx2K -> 71680
                                                         *  8bit profile : 8Kx8K -> 114688, 4Kx2K -> 57344
                                                         */
#define VDI_WAVE521_SRAM_SIZE               0x20400     /* 10bit profile : 8Kx8K -> 132096, 4Kx2K -> 66560
                                                         *  8bit profile : 8Kx8K ->  99328, 4Kx2K -> 51176
                                                         */
#define VDI_WAVE521C_SRAM_SIZE              0x23000     /* 10bit profile : 8Kx8K -> 143360, 4Kx2K -> 71680
                                                         *  8bit profile : 8Kx8K -> 114688, 4Kx2K -> 57344
                                                         * NOTE: Decoder > Encoder
                                                         */
#ifdef EYENIX_TEST

#define VPU_DRAM_PHYSICAL_BASE				(0x90000000 + 0x5000000)//(DDR0_BASE + 0xAB88000)//(DDR0_BASE + 0x2000000)//(DDR0_BASE + 0xAB88000)//(DDR1_BASE + 0x1C00000/*0x2300000*/)//, size(60Mbyte)
//#define VPU_DRAM_PHYSICAL_BASE              (DDR1_BASE+0x600000)//(DDR0_BASE+0x7800000)//(DDR0_BASE+0xB00000)//(DDR0_BASE+0x6400000)//(DDR0_BASE+0x800000) //0x00
/* resolution information
8MP 3840x2160
5MP 2560x1920
3MP 2304x1296
2MP 1920x1080
1MP 1280x720
 */
//memory test  /* ENCODE Max DDR1 128M, heap 6M use*////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//HEVC  	(8MP)		VPU(45M)+YUV(36M) :   	1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//////HEVC  	(8MP)		VPU(40M)+YUV(36M) :   	1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (2), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//////HEVC  	(8MP)		VPU(28M)+YUV(36M) :   	1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (2), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 2
//HEVC 		(5MP) 		VPU(29M)+YUV(21M) :		1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//HEVC 		(3MP)	 	VPU(20M)+YUV(15M) :		1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//HEVC 		(2MP) 		VPU(16M)+YUV(9M)  :		1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//HEVC		(1MP)		VPU(12M)+YUV(3M)  :		1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3

//AVC  		(8MP)		VPU(49M)+YUV(36M) :   	1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//////AVC  		(8MP)		VPU(43M)+YUV(36M) :   	1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (2), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//AVC 		(5MP) 		VPU(31M)+YUV(21M) :		1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//AVC 		(3MP)	 	VPU(21M)+YUV(15M) :		1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//AVC 		(2MP) 		VPU(18M)+YUV(9M)  :		1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//AVC		(1MP)		VPU(13M)+YUV(3M)  :		1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//memory test  /* DECODE Max DDR1 128M, heap 6M use*////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//HEVC  	(8MP)		108M  			  :   	1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//HEVC 		(5MP)		77M  			  :   	1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//HEVC 		(3MP)		60M  			  :   	1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//HEVC  	(2MP)		43M  			  :   	1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//HEVC  	(1MP)		32M  			  :   	1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3

//AVC  		(8MP)		293M  			  :   	1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//AVC  		(5MP)		210M  			  :   	1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//AVC  		(3MP)		203M  			  :   	1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
//AVC  		(2MP)		82M  			  :   	1CH, ENC_STREAM_BUF_SIZE 2M(1), MAX_NUM_INSTANCE (2), VLC_BUF_NUM (3), 	COMMAND_QUEUE_DEPTH (2), ENC_SRC_BUF_NUM 3
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//ENCODE 2CH TEST
//MAIN 2MP + SUB 1MP  (30*1024*1024+11*1024*1024) // 32M
//MAIN 5MP + SUB 1MP  (44*1024*1024+24*1024*1024) // 68M
//MAIN 8MP + SUB 1MP  (62*1024*1024+39*1024*1024) // 102M
#define VPU_DRAM_SIZE (80*1024*1024)//((80*1024*1024)+(768*1024))
//((64*1024*1024)+(16*1024*1024)+(768*1024)) //HEVC 4K + AVC HD

//(64*1024*1024)//HEVC 4K VLC 2
//(69*1024*1024)//HEVC 4K VLC 3
//(72*1024*1024)//AVC 4K VLC 3
//(14*1024*1024)//HEVC HD VLC 3
//(16*1024*1024)//AVC HD VLC 3

#else
#define VPU_DRAM_PHYSICAL_BASE              0x00
#define VPU_DRAM_SIZE                       (128*1024*1024)
#endif

#ifdef SUPPORT_MULTI_CORE_IN_ONE_DRIVER
#define VPU_CORE_BASE_OFFSET                0x4000
#endif

#define VDI_SYSTEM_ENDIAN                   VDI_LITTLE_ENDIAN
#define VDI_128BIT_BUS_SYSTEM_ENDIAN        VDI_128BIT_LITTLE_ENDIAN



typedef struct vpu_buffer_t vpudrv_buffer_t;	

typedef struct vpu_buffer_pool_t
{
    vpudrv_buffer_t vdb;
    int inuse;
} vpu_buffer_pool_t;

typedef struct  {
    unsigned long coreIdx;
    int vpu_fd;	
    vpu_instance_pool_t *pvip;
    int task_num;
    int clock_state;
    vpudrv_buffer_t vdb_video_memory;
    vpudrv_buffer_t vdb_register;
    vpu_buffer_t vpu_common_memory;
    vpu_buffer_pool_t vpu_buffer_pool[MAX_VPU_BUFFER_POOL];
    int vpu_buffer_pool_count;
    int product_code;
} vdi_info_t;

static vdi_info_t s_vdi_info[MAX_VPU_CORE_NUM];
#ifdef SUPPORT_MULTI_CORE_IN_ONE_DRIVER
static vpu_instance_pool_t s_vip[MAX_VPU_CORE_NUM];	// it can be used for a buffer space to save context for app process. to support running VPU in multiple process. this space should be a shared buffer.
#else
static vpu_instance_pool_t s_vip;	// it can be used for a buffer space to save context for app process. to support running VPU in multiple process. this space should be a shared buffer.
#endif
static int swap_endian(unsigned long coreIdx, unsigned char *data, int len, int endian);


int vdi_lock(unsigned long coreIdx)
{
    /* need to implement */
	dbgprintf("[%s %d]\n",__func__,__LINE__);

    return 0;
}

void vdi_unlock(unsigned long coreIdx)
{
    /* need to implement */
	dbgprintf("[%s %d]\n",__func__,__LINE__);
}

int vdi_disp_lock(unsigned long coreIdx)
{
    /* need to implement */

	dbgprintf("[%s %d]\n",__func__,__LINE__);
    return 0;
}

void vdi_disp_unlock(unsigned long coreIdx)
{
	dbgprintf("[%s %d]\n",__func__,__LINE__);
    /* need to implement */
}

int vmem_lock_init(unsigned long coreIdx)
{
    /* need to implement */
	dbgprintf("[%s %d]\n",__func__,__LINE__);
    return 0;
}

int vmem_lock(unsigned long coreIdx)
{

    /* need to implement */
	dbgprintf("[%s %d]\n",__func__,__LINE__);
    return 0;
}

void vmem_unlock(unsigned long coreIdx)
{
    /* need to implement */
	dbgprintf("[%s %d]\n",__func__,__LINE__);
}

int vmem_lock_deinit(unsigned long coreIdx)
{
    /* need to implement */
	dbgprintf("[%s %d]\n",__func__,__LINE__);
    return 0;
}

int vdi_probe(unsigned long coreIdx)
{
    int ret;

    dbgprintf("[%s %d]\n",__func__,__LINE__);
    ret = vdi_init(coreIdx);
    vdi_release(coreIdx);

    return ret;
}

int vdi_init(unsigned long coreIdx)
{
    int ret;
    vdi_info_t *vdi;
    int i;
    Uint32 product_code;

    dbgprintf("[%s %d]\n",__func__,__LINE__);
    if (coreIdx >= MAX_NUM_VPU_CORE)
        return 0;

    dbgprintf("[%s %d]\n",__func__,__LINE__);
    vdi = &s_vdi_info[coreIdx];

    dbgprintf("[%s %d]\n",__func__,__LINE__);
    if (vdi->vpu_fd != -1 && vdi->vpu_fd != 0x00)
    {
        vdi->task_num++;		
        return 0;
    }

    dbgprintf("[%s %d]\n",__func__,__LINE__);
    vdi->vpu_fd = 1;
    dbgprintf("[%s %d]\n",__func__,__LINE__);
    osal_memset(vdi->vpu_buffer_pool, 0x00, sizeof(vpu_buffer_pool_t)*MAX_VPU_BUFFER_POOL);

    dbgprintf("[%s %d]\n",__func__,__LINE__);
    if (!vdi_get_instance_pool(coreIdx))
    {
        VLOG(ERR, "[VDI] fail to create shared info for saving context \n");
        goto ERR_VDI_INIT;
    }
    dbgprintf("[%s %d]\n",__func__,__LINE__);

    vdi->vdb_video_memory.phys_addr = VPU_DRAM_PHYSICAL_BASE;
    vdi->vdb_video_memory.size = VPU_DRAM_SIZE;

    VLOG(INFO, "[VDI] vdi->vdb_video_memory.phys_addr!!!!!!!%x \n", vdi->vdb_video_memory.phys_addr);
    VLOG(INFO, "[VDI] vdi->vdb_video_memory.size!!!!!!!%x \n", vdi->vdb_video_memory.size);
    VLOG(INFO, "[VDI] VPU_DRAM_PHYSICAL_BASE!!!!!!!%x \n", VPU_DRAM_PHYSICAL_BASE);
    VLOG(INFO, "[VDI] VPU_DRAM_SIZE!!!!!!!%x \n", VPU_DRAM_SIZE);
#if 0
    if (REQUIRED_VPU_MEMORY_SIZE > vdi->vdb_video_memory.size)
    {
        VLOG(ERR, "[VDI] Warning : VPU memory will be overflow\n");
    }
#endif

    if (vdi_allocate_common_memory(coreIdx) < 0) 
    {
        VLOG(ERR, "[VDI] fail to get vpu common buffer from driver\n");
        goto ERR_VDI_INIT;
    }

    if (!vdi->pvip->instance_pool_inited)
        osal_memset(&vdi->pvip->vmem, 0x00, sizeof(video_mm_t));

#ifdef SUPPORT_MULTI_CORE_IN_ONE_DRIVER
    ret = vmem_init(&vdi->pvip->vmem, vdi->vdb_video_memory.phys_addr + (vdi->pvip->vpu_common_buffer.size*MAX_VPU_CORE_NUM),
            vdi->vdb_video_memory.size - (vdi->pvip->vpu_common_buffer.size*MAX_VPU_CORE_NUM));		
#else
    ret = vmem_init(&vdi->pvip->vmem, vdi->vdb_video_memory.phys_addr + vdi->pvip->vpu_common_buffer.size,
            vdi->vdb_video_memory.size - vdi->pvip->vpu_common_buffer.size);
#endif
    vmem_lock_init(coreIdx);

    if (ret < 0)
    {
        VLOG(ERR, "[VDI] fail to init vpu memory management logic\n");
        goto ERR_VDI_INIT;
    }

    vdi->vdb_register.phys_addr = VPU_BIT_REG_BASE;
    vdi->vdb_register.virt_addr = VPU_BIT_REG_BASE;
    vdi->vdb_register.size = VPU_BIT_REG_SIZE;

    vdi_set_clock_gate(coreIdx, TRUE);
    vdi->product_code = vdi_read_register(coreIdx, VPU_PRODUCT_CODE_REGISTER);
    product_code = vdi->product_code;
    if (PRODUCT_CODE_W_SERIES(product_code))
    {
        if (vdi_read_register(coreIdx, W5_VCPU_CUR_PC) == 0) // if BIT processor is not running.
        {
            for (i=0; i<64; i++)
                vdi_write_register(coreIdx, (i*4) + 0x100, 0x0); 
        }
    }
    else // CODA9XX
    {
        if (vdi_read_register(coreIdx, BIT_CUR_PC) == 0) // if BIT processor is not running.
        {
            for (i=0; i<64; i++)
                vdi_write_register(coreIdx, (i*4) + 0x100, 0x0); 
        }
    }
    vdi_set_clock_gate(coreIdx, FALSE);

    vdi->coreIdx = coreIdx;
    vdi->task_num++;	

    VLOG(INFO, "[VDI] success to init driver \n");

    return 0;

ERR_VDI_INIT:

    vdi_release(coreIdx);
    return -1;
}

int vdi_set_bit_firmware_to_pm(unsigned long coreIdx, const unsigned short *code)
{
	dbgprintf("[%s %d]\n",__func__,__LINE__);
    return 0;
}

int vdi_release(unsigned long coreIdx)
{
    int i;
    vpudrv_buffer_t vdb = {0, };
    vdi_info_t *vdi = &s_vdi_info[coreIdx];

    dbgprintf("[%s %d]\n",__func__,__LINE__);
    if (!vdi || vdi->vpu_fd == -1 || vdi->vpu_fd == 0x00)
        return 0;

    if (vdi_lock(coreIdx) < 0)
    {
        VLOG(ERR, "[VDI] fail to handle lock function\n");
        return -1;
    }

    if (vdi->task_num > 1) // means that the opened instance remains 
    {
        vdi->task_num--;
        vdi_unlock(coreIdx);
        return 0;
    }
    vmem_lock_deinit(coreIdx);
    vmem_exit(&vdi->pvip->vmem);

    osal_memset(&vdi->vdb_register, 0x00, sizeof(vpudrv_buffer_t));

    // get common memory information to free virtual address
    vdb.size = 0;
    for (i=0; i<MAX_VPU_BUFFER_POOL; i++)
    {
        if (vdi->vpu_common_memory.phys_addr >= vdi->vpu_buffer_pool[i].vdb.phys_addr &&
                vdi->vpu_common_memory.phys_addr < (vdi->vpu_buffer_pool[i].vdb.phys_addr + vdi->vpu_buffer_pool[i].vdb.size))
        {
            vdi->vpu_buffer_pool[i].inuse = 0;
            vdi->vpu_buffer_pool_count--;
            vdb = vdi->vpu_buffer_pool[i].vdb;
            break;
        }
    }

    if (vdb.size > 0)
        osal_memset(&vdi->vpu_common_memory, 0x00, sizeof(vpu_buffer_t));

    vdi->task_num--;
    vdi->vpu_fd = -1;

    vdi_unlock(coreIdx);

    osal_memset(vdi, 0x00, sizeof(vdi_info_t));

    return 0;
}

int vdi_get_common_memory(unsigned long coreIdx, vpu_buffer_t *vb)
{
    vdi_info_t *vdi;

    dbgprintf("[%s %d]\n",__func__,__LINE__);
    if (coreIdx >= MAX_NUM_VPU_CORE)
        return -1;

    vdi = &s_vdi_info[coreIdx];

    if(!vdi || vdi->vpu_fd==-1 || vdi->vpu_fd==0x00)
        return -1;

    osal_memcpy(vb, &vdi->vpu_common_memory, sizeof(vpu_buffer_t));

    return 0;
}

int vdi_allocate_common_memory(unsigned long core_idx)
{
    vdi_info_t *vdi = &s_vdi_info[core_idx];
    vpudrv_buffer_t vdb;
    int i;

    dbgprintf("[%s %d]\n",__func__,__LINE__);
    if (core_idx >= MAX_NUM_VPU_CORE)
        return -1;

    if(!vdi || vdi->vpu_fd==-1 || vdi->vpu_fd==0x00)
        return -1;

    //printf("vdi->pvip->vpu_common_buffer.size %d!!!!!!!! \n",vdi->pvip->vpu_common_buffer.size);
    if (vdi->pvip->vpu_common_buffer.size == 0)
    {
        vdb.size = SIZE_COMMON*MAX_VPU_CORE_NUM;
        vdb.phys_addr = vdi->vdb_video_memory.phys_addr; // set at the beginning of base address
        vdb.virt_addr = vdi->vdb_video_memory.phys_addr;
        vdb.base = vdi->vdb_video_memory.base;

        // convert os driver buffer type to vpu buffer type		
#ifdef SUPPORT_MULTI_CORE_IN_ONE_DRIVER
        vdi->pvip->vpu_common_buffer.size = SIZE_COMMON;
        vdi->pvip->vpu_common_buffer.phys_addr = (unsigned int)(vdb.phys_addr + (core_idx*SIZE_COMMON));
        vdi->pvip->vpu_common_buffer.base = (unsigned long)(vdb.base + (core_idx*SIZE_COMMON));
        vdi->pvip->vpu_common_buffer.virt_addr = (unsigned long)(vdb.virt_addr + (core_idx*SIZE_COMMON));
#else
        vdi->pvip->vpu_common_buffer.size = SIZE_COMMON;
        vdi->pvip->vpu_common_buffer.phys_addr = (unsigned int)(vdb.phys_addr);
        vdi->pvip->vpu_common_buffer.base = (unsigned long)(vdb.base);
        vdi->pvip->vpu_common_buffer.virt_addr = (unsigned long)(vdb.virt_addr);
#endif

        //printf("vdi->vpu_common_memory 0x%x!!!!!!!! \n", vdi->vpu_common_memory);
        //printf("vdi->pvip->vpu_common_buffer 0x%x!!!!!!!! \n", vdi->pvip->vpu_common_buffer);
        //printf("sizeof(vpudrv_buffer_t) %d!!!!!!!! \n", sizeof(vpudrv_buffer_t));
        osal_memcpy(&vdi->vpu_common_memory, &vdi->pvip->vpu_common_buffer, sizeof(vpudrv_buffer_t));

    }	
    else
    {
        vdb.size = SIZE_COMMON*MAX_VPU_CORE_NUM;
        vdb.phys_addr = vdi->vdb_video_memory.phys_addr; // set at the beginning of base address
        vdb.base =  vdi->vdb_video_memory.base;			
        vdb.virt_addr = vdi->vdb_video_memory.phys_addr;		

#ifdef SUPPORT_MULTI_CORE_IN_ONE_DRIVER
        vdi->pvip->vpu_common_buffer.virt_addr = (unsigned long)(vdb.virt_addr + (core_idx*SIZE_COMMON)); 
#else
        vdi->pvip->vpu_common_buffer.virt_addr = vdb.virt_addr; 
#endif
        osal_memcpy(&vdi->vpu_common_memory, &vdi->pvip->vpu_common_buffer, sizeof(vpudrv_buffer_t));

        VLOG(INFO, "[VDI] vdi_allocate_common_memory, physaddr=0x%x, virtaddr=0x%x\n", (int)vdi->pvip->vpu_common_buffer.phys_addr, (int)vdi->pvip->vpu_common_buffer.virt_addr);
    }

    for (i=0; i<MAX_VPU_BUFFER_POOL; i++)
    {
        if (vdi->vpu_buffer_pool[i].inuse == 0)
        {
            vdi->vpu_buffer_pool[i].vdb = vdb;
            vdi->vpu_buffer_pool_count++;
            vdi->vpu_buffer_pool[i].inuse = 1;
            break;
        }
    }

    VLOG(INFO, "[VDI] vdi_get_common_memory physaddr=0x%x, size=%d, virtaddr=0x%x\n", vdi->vpu_common_memory.phys_addr, vdi->vpu_common_memory.size, vdi->vpu_common_memory.virt_addr);
    return 0;
}

vpu_instance_pool_t *vdi_get_instance_pool(unsigned long coreIdx)
{
    vdi_info_t *vdi;

    dbgprintf("[%s %d]\n",__func__,__LINE__);
    if (coreIdx >= MAX_VPU_CORE_NUM)
        return NULL;

    vdi = &s_vdi_info[coreIdx];

    if(!vdi || vdi->vpu_fd == -1 || vdi->vpu_fd ==0x00 )
        return NULL;

    if (!vdi->pvip)
    {
#ifdef SUPPORT_MULTI_CORE_IN_ONE_DRIVER
        vdi->pvip = &s_vip[coreIdx];
#else
        vdi->pvip = &s_vip;
#endif
        osal_memset(vdi->pvip, 0, sizeof(vpu_instance_pool_t));		
    }

    return (vpu_instance_pool_t *)vdi->pvip;
}

int vdi_open_instance(unsigned long coreIdx, unsigned long instIdx)
{
    vdi_info_t *vdi = NULL;

    dbgprintf("[%s %d]\n",__func__,__LINE__);
    if (coreIdx >= MAX_VPU_CORE_NUM)
        return -1;

    vdi = &s_vdi_info[coreIdx];

    if(!vdi || vdi->vpu_fd ==-1 || vdi->vpu_fd == 0x00)
        return -1;

    vdi->pvip->vpu_instance_num++;

    return 0;
}

int vdi_close_instance(unsigned long coreIdx, unsigned long instIdx)
{
    vdi_info_t *vdi = NULL;

    dbgprintf("[%s %d]\n",__func__,__LINE__);
    if (coreIdx >= MAX_VPU_CORE_NUM)
        return -1;

    vdi = &s_vdi_info[coreIdx];

    if(!vdi || vdi->vpu_fd ==-1 || vdi->vpu_fd == 0x00)
        return -1;

    vdi->pvip->vpu_instance_num--;

    return 0;
}

int vdi_get_instance_num(unsigned long coreIdx)
{
    vdi_info_t *vdi = NULL;

    dbgprintf("[%s %d]\n",__func__,__LINE__);
    if (coreIdx >= MAX_VPU_CORE_NUM)
        return -1;

    vdi = &s_vdi_info[coreIdx];

    if(!vdi || vdi->vpu_fd ==-1 || vdi->vpu_fd == 0x00)
        return -1;

    return vdi->pvip->vpu_instance_num;
}

int vdi_hw_reset(unsigned long coreIdx) // DEVICE_ADDR_SW_RESET
{
    vdi_info_t *vdi = NULL;

    dbgprintf("[%s %d]\n",__func__,__LINE__);
    if (coreIdx >= MAX_VPU_CORE_NUM)
        return -1;

    vdi = &s_vdi_info[coreIdx];

    if(!vdi || !vdi || vdi->vpu_fd ==-1 || vdi->vpu_fd == 0x00)
        return -1;

    // to do any action for hw reset

    return 0;
}

#ifdef EYENIX_TEST
#ifdef DIRECT_APBMESSAGE
void APB_WRITE(unsigned int addr, unsigned int data)
{
    unsigned long *reg_addr;
	reg_addr = (unsigned int *)(addr + (unsigned int)VPU_BIT_REG_BASE);
	*(volatile unsigned int *)reg_addr = data;
	printf("APB_WRITE 0x%x 0x%x\n",reg_addr,data);
}

unsigned int APB_READ(unsigned int addr)
{
    unsigned long *reg_addr;
	reg_addr = (unsigned int *)(addr + (unsigned long)VPU_BIT_REG_BASE);
	printf("APB_READ 0x%x 0x%x\n",reg_addr, *(volatile unsigned int *)reg_addr);
	return *(volatile unsigned int *)reg_addr;
}
#endif
#endif


void vdi_write_register(unsigned long coreIdx, unsigned int addr, unsigned int data)
{
#ifdef DIRECT_APBMESSAGE
	APB_WRITE(addr, data);
#else
    vdi_info_t *vdi = NULL;
    unsigned long *reg_addr;

    if (coreIdx >= MAX_NUM_VPU_CORE)
        return;

    vdi = &s_vdi_info[coreIdx];

    if(!vdi || vdi->vpu_fd==-1 || vdi->vpu_fd == 0x00)
        return;

#ifdef SUPPORT_MULTI_CORE_IN_ONE_DRIVER
    reg_addr = (unsigned int *)(addr + (unsigned long)vdi->vdb_register.virt_addr + (coreIdx*VPU_CORE_BASE_OFFSET));
#else
    reg_addr = (unsigned int *)(addr + (unsigned long)vdi->vdb_register.virt_addr);
#endif
    *(volatile unsigned int *)reg_addr = data;

#ifdef DIRECT_APBMESSAGE
    dbgprintf("\nAPB WRITE 0x%x 0x%x\n", addr, data);
#endif

#endif
}

unsigned int vdi_read_register(unsigned long coreIdx, unsigned int addr)
{
#ifdef DIRECT_APBMESSAGE
	return APB_READ(addr);
#else
    vdi_info_t *vdi = NULL;
    unsigned int *reg_addr; //ksj porting for EN675

    if (coreIdx >= MAX_NUM_VPU_CORE)
        return (unsigned int)-1;

    vdi = &s_vdi_info[coreIdx];

    if(!vdi || vdi->vpu_fd==-1 || vdi->vpu_fd == 0x00)
        return (unsigned int)-1;

#ifdef SUPPORT_MULTI_CORE_IN_ONE_DRIVER	
    reg_addr = (unsigned int *)(addr + (unsigned int)vdi->vdb_register.virt_addr + (coreIdx*VPU_CORE_BASE_OFFSET)); //ksj porting for EN675
#else
    reg_addr = (unsigned int *)(addr + (unsigned int)vdi->vdb_register.virt_addr); //ksj porting for EN675
#endif

#ifdef DIRECT_APBMESSAGE
    dbgprintf("\nAPB READ 0x%x 0x%x\n", addr, *(volatile unsigned int *)reg_addr);
#endif
    return *(volatile unsigned int *)reg_addr; //ksj porting for EN675
#endif
}

#define FIO_TIMEOUT         10000
unsigned int vdi_fio_read_register(unsigned long coreIdx, unsigned int addr)
{
    unsigned int ctrl;
    unsigned int count = 0;
    unsigned int data  = 0xffffffff;

    ctrl  = (addr&0xffff);
    ctrl |= (0<<16);    /* read operation */
    vdi_write_register(coreIdx, W5_VPU_FIO_CTRL_ADDR, ctrl);
    count = FIO_TIMEOUT;
    while (count--) {
        ctrl = vdi_read_register(coreIdx, W5_VPU_FIO_CTRL_ADDR);
        if (ctrl & 0x80000000) {
            data = vdi_read_register(coreIdx, W5_VPU_FIO_DATA);
            break;
        }
    }

    dbgprintf("vdi_fio_read_register data 0x%x\n",data);
    return data;
}

void vdi_fio_write_register(unsigned long coreIdx, unsigned int addr, unsigned int data)
{
    unsigned int ctrl;
    unsigned int count = 0;

    vdi_write_register(coreIdx, W5_VPU_FIO_DATA, data);
    ctrl  = (addr&0xffff);
    ctrl |= (1<<16);    /* write operation */
    vdi_write_register(coreIdx, W5_VPU_FIO_CTRL_ADDR, ctrl);

    count = FIO_TIMEOUT;
    while (count--) {
        ctrl = vdi_read_register(coreIdx, W5_VPU_FIO_CTRL_ADDR);
        if (ctrl & 0x80000000) {
            break;
        }
    }
}

int vdi_clear_memory(unsigned long coreIdx, PhysicalAddress addr, int len, int endian)
{
    vdi_info_t *vdi;
    vpudrv_buffer_t vdb;
    unsigned long offset;
    int i;
    Uint8*  zero;

#ifdef SUPPORT_MULTI_CORE_IN_ONE_DRIVER
    coreIdx = 0;
#endif

    if (coreIdx >= MAX_NUM_VPU_CORE)
        return -1;

    vdi = &s_vdi_info[coreIdx];

    if(!vdi || vdi->vpu_fd == -1 || vdi->vpu_fd == 0x00)
        return -1;

    osal_memset(&vdb, 0x00, sizeof(vpudrv_buffer_t));

    for (i=0; i<MAX_VPU_BUFFER_POOL; i++)
    {
        if (vdi->vpu_buffer_pool[i].inuse == 1)
        {
            vdb = vdi->vpu_buffer_pool[i].vdb;
            if (addr >= vdb.phys_addr && addr < (vdb.phys_addr + vdb.size))
                break;
        }
    }

    if (!vdb.size) {
        VLOG(ERR, "address 0x%08x is not mapped address!!!\n", (int)addr);
        return -1;
    }

    offset = (unsigned long)(addr - vdb.phys_addr);

    zero = (Uint8*)osal_malloc(len);
    osal_memset((void*)zero, 0x00, len);

    osal_memcpy((void *)((unsigned long)vdb.virt_addr+offset), zero, len);	

    osal_free(zero);

    return len;
}

int vdi_write_memory(unsigned long coreIdx, PhysicalAddress addr, unsigned char *data, int len, int endian)
{	
    vdi_info_t *vdi;
    vpudrv_buffer_t vdb = {0};
    unsigned long offset;
    int i;

#ifdef SUPPORT_MULTI_CORE_IN_ONE_DRIVER
    coreIdx = 0;
#endif
    vdi = &s_vdi_info[coreIdx];

    if(!vdi || vdi->vpu_fd==-1 || vdi->vpu_fd == 0x00)
        return -1;

    for (i=0; i<MAX_VPU_BUFFER_POOL; i++)
    {
        if (vdi->vpu_buffer_pool[i].inuse == 1)
        {
            vdb = vdi->vpu_buffer_pool[i].vdb;
            if (addr >= vdb.phys_addr && addr < (vdb.phys_addr + vdb.size))
                break;
        }
    }

    if (!vdb.size) { //lint !e644
        VLOG(ERR, "address 0x%08x is not mapped address!!!\n", addr);
        return -1;
    }

    offset =  (unsigned long)(addr -vdb.phys_addr);

    swap_endian(coreIdx, data, len, endian);
    osal_memcpy((void *)((unsigned long)vdb.virt_addr+offset), data, len);	

    return len;

}

int vdi_read_memory(unsigned long coreIdx, PhysicalAddress addr, unsigned char *data, int len, int endian)
{
    vdi_info_t *vdi = NULL;
    vpudrv_buffer_t vdb = {0};
    unsigned long offset;
    int i;

#ifdef SUPPORT_MULTI_CORE_IN_ONE_DRIVER
    coreIdx = 0;
#endif
    if (coreIdx >= MAX_NUM_VPU_CORE)
        return -1;

    vdi = &s_vdi_info[coreIdx];

    if(!vdi || vdi->vpu_fd==-1 || vdi->vpu_fd == 0x00)
        return -1;

    for (i=0; i<MAX_VPU_BUFFER_POOL; i++)
    {
        if (vdi->vpu_buffer_pool[i].inuse == 1)
        {
            vdb = vdi->vpu_buffer_pool[i].vdb;
            if (addr >= vdb.phys_addr && addr < (vdb.phys_addr + vdb.size))
                break;		
        }
    }

    if (!vdb.size) //lint !e644
        return -1;

    offset =  (unsigned long)(addr -vdb.phys_addr);

    osal_memcpy(data, (const void *)((unsigned long)vdb.virt_addr+offset), len);
    swap_endian(coreIdx, data, len,  endian);

    return len;
}

int vdi_allocate_dma_memory(unsigned long coreIdx, vpu_buffer_t *vb, int memTypes, int instIndex)
{
    vdi_info_t *vdi = NULL;
    int i;
    unsigned long offset;
    vpudrv_buffer_t vdb = {0};

#ifdef SUPPORT_MULTI_CORE_IN_ONE_DRIVER
    coreIdx = 0;
#endif

    if (coreIdx >= MAX_NUM_VPU_CORE)
        return -1;

    vdi = &s_vdi_info[coreIdx];

    if(!vdi || vdi->vpu_fd==-1 || vdi->vpu_fd == 0x00)
        return -1;	

    vdb.size = vb->size;
    vmem_lock(coreIdx);
    vdb.phys_addr = (PhysicalAddress)vmem_alloc(&vdi->pvip->vmem, vdb.size, 0);
    dbgprintf("vdb.phys_addr %x vdb.size %d\n", vdb.phys_addr, vdb.size);
    vmem_unlock(coreIdx);

    if ((PhysicalAddress)vdb.phys_addr == (PhysicalAddress)-1)
        return -1; // not enough memory

    offset = (unsigned long)(vdb.phys_addr - vdi->vdb_video_memory.phys_addr);
    vdb.base = (unsigned long )vdi->vdb_video_memory.base + offset;
    vdb.virt_addr = vdb.phys_addr;

    vb->phys_addr = (unsigned long)vdb.phys_addr;
    vb->base = (unsigned long)vdb.base;
    vb->virt_addr = (unsigned long)vb->phys_addr;

    for (i=0; i<MAX_VPU_BUFFER_POOL; i++)
    {
        if (vdi->vpu_buffer_pool[i].inuse == 0)
        {
            vdi->vpu_buffer_pool[i].vdb = vdb;
            vdi->vpu_buffer_pool_count++;
            vdi->vpu_buffer_pool[i].inuse = 1;
            break;
        }
    }

    return 0;
}

int vdi_attach_dma_memory(unsigned long coreIdx, vpu_buffer_t *vb)
{
    vdi_info_t *vdi;
    int i;
    unsigned long offset;
    vpudrv_buffer_t vdb = {0};

#ifdef SUPPORT_MULTI_CORE_IN_ONE_DRIVER
    coreIdx = 0;
#endif

    if (coreIdx >= MAX_NUM_VPU_CORE)
        return -1;

    vdi = &s_vdi_info[coreIdx];

    if(!vb || !vdi || vdi->vpu_fd==-1 || vdi->vpu_fd == 0x00)
        return -1;

    vdb.size = vb->size;
    vdb.phys_addr = vb->phys_addr;
    offset = (unsigned long)(vdb.phys_addr - vdi->vdb_video_memory.phys_addr);
    vdb.base = (unsigned long )vdi->vdb_video_memory.base + offset;
    vdb.virt_addr = vb->virt_addr;

    for (i=0; i<MAX_VPU_BUFFER_POOL; i++)
    {
        if (vdi->vpu_buffer_pool[i].vdb.phys_addr == vb->phys_addr)
        {
            vdi->vpu_buffer_pool[i].vdb = vdb;
            vdi->vpu_buffer_pool[i].inuse = 1;
            break;
        }
        else
        {
            if (vdi->vpu_buffer_pool[i].inuse == 0)
            {
                vdi->vpu_buffer_pool[i].vdb = vdb;
                vdi->vpu_buffer_pool_count++;
                vdi->vpu_buffer_pool[i].inuse = 1;
                break;
            }
        }		
    }

    return 0;
}

int vdi_dettach_dma_memory(unsigned long coreIdx, vpu_buffer_t *vb)
{
    vdi_info_t *vdi;
    int i;

#ifdef SUPPORT_MULTI_CORE_IN_ONE_DRIVER
    coreIdx = 0;
#endif

    if (coreIdx >= MAX_NUM_VPU_CORE)
        return -1;

    vdi = &s_vdi_info[coreIdx];

    if(!vb || !vdi || vdi->vpu_fd==-1 || vdi->vpu_fd == 0x00)
        return -1;

    if (vb->size == 0)
        return -1;

    for (i=0; i<MAX_VPU_BUFFER_POOL; i++)
    {
        if (vdi->vpu_buffer_pool[i].vdb.phys_addr == vb->phys_addr)
        {
            vdi->vpu_buffer_pool[i].inuse = 0;
            vdi->vpu_buffer_pool_count--;
            break;
        }
    }

    return 0;
}

void vdi_free_dma_memory(unsigned long coreIdx, vpu_buffer_t *vb, int memTypes, int instIndex)
{
    vdi_info_t *vdi;
    int i;
    vpudrv_buffer_t vdb = {0};

#ifdef SUPPORT_MULTI_CORE_IN_ONE_DRIVER
    coreIdx = 0;
#endif
    if (coreIdx >= MAX_NUM_VPU_CORE)
        return;

    vdi = &s_vdi_info[coreIdx];

    if(!vb || !vdi || vdi->vpu_fd==-1 || vdi->vpu_fd == 0x00)
        return;

    if (vb->size == 0)
        return;

    for (i=0; i<MAX_VPU_BUFFER_POOL; i++)
    {
        if (vdi->vpu_buffer_pool[i].vdb.phys_addr == vb->phys_addr)
        {
            vdi->vpu_buffer_pool[i].inuse = 0;
            vdi->vpu_buffer_pool_count--;
            vdb = vdi->vpu_buffer_pool[i].vdb;
            break;
        }
    }

    if (!vdb.size) //lint !e644
    {
        VLOG(ERR, "[VDI] invalid buffer to free address = 0x%x\n", (int)vdb.virt_addr);
        return ;
    }
    vmem_lock(coreIdx);
    vmem_free(&vdi->pvip->vmem, (unsigned long)vdb.phys_addr, 0);
    vmem_unlock(coreIdx);
    osal_memset(vb, 0, sizeof(vpu_buffer_t));
}

int vdi_get_sram_memory(unsigned long coreIdx, vpu_buffer_t *vb)
{
    vdi_info_t *vdi = &s_vdi_info[coreIdx];
    Uint32      sram_size=0;

    if (coreIdx >= MAX_NUM_VPU_CORE)
        return -1;

    if(!vb || !vdi)
        return -1;

    switch (vdi->product_code) {
        case BODA950_CODE:
        case CODA960_CODE:
        case CODA980_CODE:
            sram_size = VDI_CODA9_SRAM_SIZE; break;
        case WAVE511_CODE:
            sram_size = VDI_WAVE511_SRAM_SIZE; break;
        case WAVE521_CODE:
            sram_size = VDI_WAVE521_SRAM_SIZE; break;
        case WAVE521C_CODE:
            sram_size = VDI_WAVE521C_SRAM_SIZE; break;
        case WAVE521C_DUAL_CODE:
            sram_size = VDI_WAVE521C_SRAM_SIZE; break;
        case WAVE517_CODE:
            sram_size = VDI_WAVE521C_SRAM_SIZE; break;
        default:
            VLOG(ERR, "[VDI] check product_code(%x)\n", vdi->product_code);
            break;
    }

    // if we can know the sram address directly in vdi layer, we use it first for sdram address
    vb->phys_addr = VDI_SRAM_BASE_ADDR+(coreIdx*sram_size);
    vb->size = sram_size;

    return 0;
}

int vdi_set_clock_gate(unsigned long coreIdx, int enable)
{
    vdi_info_t *vdi = NULL;

    if (coreIdx >= MAX_NUM_VPU_CORE)
        return -1;

    vdi = &s_vdi_info[coreIdx];

    if(!vdi || vdi->vpu_fd==-1 || vdi->vpu_fd == 0x00)
        return -1;

    if (vdi->product_code == WAVE512_CODE || vdi->product_code == WAVE515_CODE || vdi->product_code == WAVE517_CODE ||
            vdi->product_code == WAVE521_CODE || vdi->product_code == WAVE521C_CODE || vdi->product_code == WAVE511_CODE || vdi->product_code == WAVE521C_DUAL_CODE ) {
        return 0;
    }

    vdi->clock_state = enable;

    return 0;	
}

int vdi_get_clock_gate(unsigned long coreIdx)
{
    vdi_info_t *vdi = NULL;

    if (coreIdx >= MAX_NUM_VPU_CORE)
        return -1;

    vdi = &s_vdi_info[coreIdx];

    if(!vdi || vdi->vpu_fd==-1 || vdi->vpu_fd == 0x00)
        return -1;

    return vdi->clock_state;
}

int vdi_wait_bus_busy(unsigned long coreIdx, int timeout, unsigned int gdi_busy_flag)
{
    vdi_info_t *vdi = &s_vdi_info[coreIdx];
    if(!vdi || vdi->vpu_fd==-1 || vdi->vpu_fd == 0x00)
        return -1;

    dbgprintf("vdi_wait_bus_busy vdi->product_code 0x%x\n",vdi->product_code);
    //VDI must implement timeout action in this function for multi-vpu core scheduling efficiency.
    //the setting small value as timeout gives a chance to wait the other vpu core.
    while(1)
    {
        if (vdi->product_code == WAVE521_CODE || vdi->product_code == WAVE521C_CODE || vdi->product_code == WAVE511_CODE || vdi->product_code == WAVE521C_DUAL_CODE ||
            vdi->product_code == WAVE517_CODE) {
            if (vdi_fio_read_register(coreIdx, gdi_busy_flag) == 0x3f) break;
        }
        else if (PRODUCT_CODE_W_SERIES(vdi->product_code)) {
            if (vdi_fio_read_register(coreIdx, gdi_busy_flag) == 0x738) break;
        }
        else {
            if (vdi_read_register(coreIdx, gdi_busy_flag) == 0x77) break;
        }
        //Sleep(1);	// 1ms sec
        //if (count++ > timeout)
        //	return -1;
    }	

    return 0;
}

int vdi_wait_vpu_busy(unsigned long coreIdx, int timeout, unsigned int addr_bit_busy_flag)
{
    vdi_info_t *vdi = &s_vdi_info[coreIdx];
    int        normalReg = TRUE;

    if(!vdi || vdi->vpu_fd==-1 || vdi->vpu_fd == 0x00)
        return -1;
#ifdef NONOS_SW_UART
    test_SwUartHandler();
#endif
    //VDI must implement timeout action in this function for multi-vpu core scheduling efficiency.
    //the setting small value as timeout gives a chance to wait the other vpu core.
    dbgprintf("vdi->product_code %x\n",vdi->product_code);
    if (PRODUCT_CODE_W_SERIES(vdi->product_code)) {
        if (addr_bit_busy_flag&0x8000) normalReg = FALSE;
    }
    dbgprintf("normalReg %d\n",normalReg);
    while(1)
    {
        if (normalReg == TRUE) {
            if (vdi_read_register(coreIdx, addr_bit_busy_flag) == 0) break;
            else	VLOG(ERR, "[VDI] vdi_wait_vpu_busy\n");
        }
        else {
            if (vdi_fio_read_register(coreIdx, addr_bit_busy_flag) == 0) break;
            else 	VLOG(ERR, "[VDI] vdi_wait_vpu_busy\n");
        }

        //Sleep(1);	// 1ms sec
        //if (count++ > timeout)
        //	return -1;
    }	

    return 0;
}

int vdi_wait_vcpu_bus_busy(unsigned long coreIdx, int timeout, unsigned int addr_bit_busy_flag)
{
    vdi_info_t *vdi = &s_vdi_info[coreIdx];
    int        normalReg = TRUE;

    if(!vdi || vdi->vpu_fd==-1 || vdi->vpu_fd == 0x00)
        return -1;

    //VDI must implement timeout action in this function for multi-vpu core scheduling efficiency.
    //the setting small value as timeout gives a chance to wait the other vpu core.
    if (PRODUCT_CODE_W_SERIES(vdi->product_code)) {
        if (addr_bit_busy_flag&0x8000) normalReg = FALSE;
    }

    while(1)
    {
        if (normalReg == TRUE) {
            if (vdi_read_register(coreIdx, addr_bit_busy_flag) == 0x40) break;
        }
        else {
            if (vdi_fio_read_register(coreIdx, addr_bit_busy_flag) == 0x40) break;
        }

        //Sleep(1);	// 1ms sec
        //if (count++ > timeout)
        //	return -1;
    }	

    return 0;
}

#ifdef NONOS_SW_UART
void test_SwUartHandler(void)
{
	unsigned int regSwUartStatus;
	unsigned int regSwUartTxData;
	unsigned char *strRegSwUartTxData;
	int i = 0;
	char uartTx[1024];
	int testcnt = 0;

	osal_memset(uartTx, 0, sizeof(char)*1024);
	while(1)
	{
		if(testcnt++>10240)
			break;

		regSwUartStatus = vdi_read_register(0, W5_SW_UART_STATUS);
		if (regSwUartStatus == (unsigned int)-1)
		{
			continue;
		}

		if ((regSwUartStatus & (1<<1)))
		{
			regSwUartTxData = vdi_read_register(0, W5_SW_UART_TX_DATA);
			if (regSwUartTxData == (unsigned int)-1)
			{
				continue;
			}

			VLOG(INFO,"test uart\n");
			regSwUartStatus &= ~(1<<1);
			vdi_write_register(0, W5_SW_UART_STATUS, regSwUartStatus);
			strRegSwUartTxData = (unsigned char *)&regSwUartTxData;
			VLOG(INFO, "%c", strRegSwUartTxData);
			for (i=0; i < 4; i++)
			{
				if (strRegSwUartTxData[i] == '\n')
				{
					VLOG(INFO, "%s \n", uartTx);
					osal_memset(uartTx, 0, sizeof(unsigned char)*1024);
				}
				else
				{
					strncat((char *)uartTx, (const char *)(strRegSwUartTxData + i), 1);
				}
			}
		}

	}
	//VLOG(1, "exit SwUartHandler\n");

}
#endif

#ifdef SUPPORT_MULTI_INST_INTR
int vdi_wait_interrupt(unsigned long core_idx, unsigned int instIdx, int timeout)
#else
int vdi_wait_interrupt(unsigned long core_idx, int timeout)
#endif
{
    vdi_info_t *vdi = &s_vdi_info[core_idx];
    int intr_reason;
    //unsigned long cur_time;

    //VDI must implement timeout action in this function for multi-vpu core scheduling efficiency.
    //the setting small value as timeout gives a chance to wait the other vpu core.
#ifdef SUPPORT_INTERRUPT
#error "Need to implement"
    if(!vdi || vdi->vpu_fd==-1 || vdi->vpu_fd == 0x00)
        return -1;
#else
    int int_sts_reg;
    int int_reason_reg;
    UINT testcntall = 0;

    if(!vdi || vdi->vpu_fd==-1 || vdi->vpu_fd == 0x00)
        return -1;

    if (PRODUCT_CODE_W_SERIES(vdi->product_code)) {
        int_sts_reg = W5_VPU_VPU_INT_STS;
    }
    else {
        int_sts_reg = BIT_INT_STS;
    }

    if (PRODUCT_CODE_W_SERIES(vdi->product_code)) {
        int_reason_reg = W5_VPU_VINT_REASON;
    }
    else {
        int_reason_reg = BIT_INT_REASON;
    }

#if 0//def EYENIX_TEST

    //cur_time = jiffies;
    unsigned int testcnt = 0;
    while(1)
    {
        if (vdi_read_register(core_idx, int_sts_reg))
        {
            if ((intr_reason=vdi_read_register(core_idx, int_reason_reg)))
            {
                if (PRODUCT_CODE_W_SERIES(vdi->product_code)) 
                {
                    vdi_write_register(core_idx, W5_VPU_VINT_REASON_CLR, intr_reason);
                    vdi_write_register(core_idx, W5_VPU_VINT_CLEAR, 0x1);
                }
                else
                {
                    vdi_write_register(core_idx, BIT_INT_CLEAR, 0x1);
                }

                break;
            }
        }
        //else
       // 	dbgprintf("wait irq\n");

        if(testcnt++>4294960)
        {
        	dbgprintf("time out wait 10 seconds\n");

         WaitXms(1000*10);
         if (vdi_read_register(core_idx, int_sts_reg))
         {
             if ((intr_reason=vdi_read_register(core_idx, int_reason_reg)))
             {
                 if (PRODUCT_CODE_W_SERIES(vdi->product_code))
                 {
                     vdi_write_register(core_idx, W5_VPU_VINT_REASON_CLR, intr_reason);
                     vdi_write_register(core_idx, W5_VPU_VINT_CLEAR, 0x1);
                 }
                 else
                 {
                     vdi_write_register(core_idx, BIT_INT_CLEAR, 0x1);
                 }

                 break;
             }
             dbgprintf("vdi_read_register intr_reason %d \n", vdi_read_register);
        	 WaitXms(1000*1);
         }
         return -1;
        }

        /*
           if(jiffies_to_msecs(jiffies - cur_time) > timeout) {
           return -1;
           }
           */
    }	
#else

    while(1)
    {
#ifdef NONOS_SW_UART
        test_SwUartHandler();
#endif
        if (vdi_read_register(core_idx, int_sts_reg))
        {
            if ((intr_reason=vdi_read_register(core_idx, int_reason_reg)))
            {
                if (PRODUCT_CODE_W_SERIES(vdi->product_code))
                {
                    vdi_write_register(core_idx, W5_VPU_VINT_REASON_CLR, intr_reason);
                    vdi_write_register(core_idx, W5_VPU_VINT_CLEAR, 0x1);
                }
                else
                {
                    vdi_write_register(core_idx, BIT_INT_CLEAR, 0x1);
                }

                break;
            }
        }
        else
        {
        	//testcntall++;
        	//if(testcntall>0xFFFFFFF){
        	//	printf("wait cnt %d \n",testcntall);
        	//	testcntall = 0;
        	//}
        	//dbgprintf("_");
    		Visp();
        }

        /*
           if(jiffies_to_msecs(jiffies - cur_time) > timeout) {
           return -1;
           }
           */
    }

#endif

#endif

    return intr_reason;
}

int vdi_get_system_endian(unsigned long coreIdx)
{
    vdi_info_t *vdi = &s_vdi_info[coreIdx];

    if(!vdi || vdi->vpu_fd == -1 || vdi->vpu_fd == 0x00)
        return -1;

    if (PRODUCT_CODE_W_SERIES(vdi->product_code))
        return VDI_128BIT_BUS_SYSTEM_ENDIAN;
    else
        return VDI_SYSTEM_ENDIAN;
}

int vdi_convert_endian(unsigned long coreIdx, unsigned int endian)
{
    Uint32 productCode;

    productCode = vdi_read_register(coreIdx, VPU_PRODUCT_CODE_REGISTER);
    if (PRODUCT_CODE_W_SERIES(productCode)) {
        switch (endian) {
            case VDI_LITTLE_ENDIAN:       endian = 0x00; break;
            case VDI_BIG_ENDIAN:          endian = 0x0f; break;
            case VDI_32BIT_LITTLE_ENDIAN: endian = 0x04; break;
            case VDI_32BIT_BIG_ENDIAN:    endian = 0x03; break;
        }
    } 
    return (endian&0x0f); 
}

static Uint32 convert_endian_coda9_to_wave4(Uint32 endian)
{
    Uint32 converted_endian = endian;
    switch(endian) {
        case VDI_LITTLE_ENDIAN:       converted_endian = 0; break;
        case VDI_BIG_ENDIAN:          converted_endian = 7; break;
        case VDI_32BIT_LITTLE_ENDIAN: converted_endian = 4; break;
        case VDI_32BIT_BIG_ENDIAN:    converted_endian = 3; break;
    }
    return converted_endian;
}

int swap_endian(unsigned long coreIdx, unsigned char *data, int len, int endian)
{
    vdi_info_t* vdi = &s_vdi_info[coreIdx];
    int         changes;
    int         sys_endian;
    BOOL        byteChange, wordChange, dwordChange, lwordChange;

    if (PRODUCT_CODE_W_SERIES(vdi->product_code)) {
        sys_endian = VDI_128BIT_BUS_SYSTEM_ENDIAN;
    }
    else if(PRODUCT_CODE_NOT_W_SERIES(vdi->product_code)) {
        sys_endian = VDI_SYSTEM_ENDIAN;
    }
    else {
        VLOG(ERR, "Unknown product id : %08x\n", vdi->product_code);
        return -1;
    }

    endian     = vdi_convert_endian(coreIdx, endian);
    sys_endian = vdi_convert_endian(coreIdx, sys_endian);
    if (endian == sys_endian)
        return 0;

    if (PRODUCT_CODE_W_SERIES(vdi->product_code)) {
    }
    else if (PRODUCT_CODE_NOT_W_SERIES(vdi->product_code)) {
        endian     = convert_endian_coda9_to_wave4(endian);
        sys_endian = convert_endian_coda9_to_wave4(sys_endian);
    }
    else {
        VLOG(ERR, "Unknown product id : %08x\n", vdi->product_code);
        return -1;
    }

    changes     = endian ^ sys_endian;
    byteChange  = changes&0x01;
    wordChange  = ((changes&0x02) == 0x02);
    dwordChange = ((changes&0x04) == 0x04);
    lwordChange = ((changes&0x08) == 0x08);

    if (byteChange)  byte_swap(data, len);
    if (wordChange)  word_swap(data, len);
    if (dwordChange) dword_swap(data, len);
    if (lwordChange) lword_swap(data, len);

    return 1;
}

#ifdef EYENIX_TEST
#ifdef DIRECT_APBMESSAGE
#include "chagall17.h"
//#include "wave5_regdefine.h"



int APB_DIRECT_CONTROL(void)
{
	int coreIdx = 0;
	Uint32          i, regVal;
	Uint32 __VPU_BUSY_TIMEOUT = VPU_BUSY_CHECK_TIMEOUT;
    Uint32  val;
    Int32  reason = -1;

	//2 Encoder codec mode !

    vdi_info_t *vdi = &s_vdi_info[coreIdx];
    if(vdi)
    {
    	vdi->vpu_fd = 0x01;
    	vdi->product_code = WAVE521C_CODE;
    }

	//[vdi_init 224]///////////////////////////////////////////////////////////////////////////////////////////////
    printf("\nvdi_init\n");
    printf("APB_READ(0x1044) %x\n",APB_READ(0x1044));
	if(APB_READ(0x1044) != 0x521c) //0x521c
	{
		VLOG(ERR, "Failed to read APB_READ(0x1044) != 0x521c\n");
		return 0;
	}

	//APB_WRITE(0x100, 0x0); ~ APB_WRITE(0x1fc, 0x0);
	for(i=0x100; i<0x200; i+=0x4)
	{
		APB_WRITE(i, 0x0);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuReset////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Wave5VpuReset\n");
    // VPU doesn't send response. Force to set BUSY flag to 0.
	APB_WRITE(0x70, 0x0);
	APB_READ(0x98);	// 0x800d0908
	APB_READ(0x9c);	// 0x8018003

	// Step1 : disable request
	vdi_fio_write_register(coreIdx, W5_BACKBONE_GDI_BUS_CTRL, 0x4);

	// Step2 : Waiting for completion of bus transaction
	//APB_WRITE(0x24, 0x4);
	//APB_WRITE(0x20, 0x1fe10);
	//APB_READ(0x20); // 0x80000000
	//APB_WRITE(0x20, 0xfe14);
	//APB_READ(0x20); // 0x80000000
	//APB_READ(0x24); // 0x3f

	if (vdi_wait_bus_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_BACKBONE_GDI_BUS_STATUS) == -1) {
		vdi_fio_write_register(coreIdx, W5_BACKBONE_GDI_BUS_CTRL, 0x00);
		VLOG(ERR, "VpuReset Error \n");
		return RETCODE_VPU_RESPONSE_TIMEOUT;
	}

	//APB_WRITE(0x50, 0xfffffff);
	VpuWriteReg(coreIdx, W5_VPU_RESET_REQ, 0xfffffff);

	//APB_READ(0x54); // 0x0
	if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_RESET_STATUS) == -1) {
		VpuWriteReg(coreIdx, W5_VPU_RESET_REQ, 0);
		//vdi_log(coreIdx, W5_RESET_VPU, 2);
		return RETCODE_VPU_RESPONSE_TIMEOUT;
	}

	//APB_WRITE(0x50, 0x0);
	VpuWriteReg(coreIdx, W5_VPU_RESET_REQ, 0);

    // Step3 : must clear GDI_BUS_CTRL after done SW_RESET
	//APB_WRITE(0x24, 0x0);
	//APB_WRITE(0x20, 0x1fe10);
	//APB_READ(0x20);	// 0x80000000
    vdi_fio_write_register(coreIdx, W5_BACKBONE_GDI_BUS_CTRL, 0x00);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//Wave5VpuInit ///////////////////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuInit\n");
	//firmware dsp copy
	osal_memcpy(0x8ab88000, (char *)bit_code_h, HEVC_CODE_LITTLE_LEN*2);

	APB_WRITE(0x0, 0x0);
	//APB_WRITE(0x100, 0x0);~APB_WRITE(0x1fc 0x0
	for(i=0x100; i<0x200; i+=0x4)
	{
		APB_WRITE(i, 0x0);
	}

	APB_WRITE(0x60, 0x80000900);
	APB_WRITE(0x64, 0x0);
	APB_WRITE(0x68, 0x8ab88000);
	APB_WRITE(0x110, 0x8ab88000);
	APB_WRITE(0x114, 0x100000);
	APB_WRITE(0x118, 0x0);
	APB_WRITE(0x11c, 0x8ac88000);
	APB_WRITE(0x120, 0x100000);
	APB_WRITE(0x130, 0xffff);
	APB_WRITE(0x12c, 0x0);
	APB_WRITE(0x48, 0xa340);

	regVal = APB_READ(0x98); // 0x800d0908
	if (((regVal>>16)&1) == 1) {
		regVal = ((WAVE5_PROC_AXI_ID << 28) |
				  (WAVE5_PRP_AXI_ID << 24) |
				  (WAVE5_FBD_Y_AXI_ID << 20) |
				  (WAVE5_FBC_Y_AXI_ID << 16) |
				  (WAVE5_FBD_C_AXI_ID << 12) |
				  (WAVE5_FBC_C_AXI_ID << 8) |
				  (WAVE5_PRI_AXI_ID << 4) |
				  (WAVE5_SEC_AXI_ID << 0));
		vdi_fio_write_register(coreIdx, W5_BACKBONE_PROG_AXI_ID, regVal);
		//APB_WRITE(0x24, 0x0);
		//APB_WRITE(0x20, 0x1fe0c);
		//APB_READ(0x20);//0x80000000
	}

	APB_WRITE(0x124, 0x0);
	APB_WRITE(0x128, 0x23000);

	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x1);
	APB_WRITE(0x6c, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        VLOG(ERR, "VPU init(W5_VPU_REMAP_CORE_START) timeout\n");
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70);  //0x1
    //APB_READ(0x70);  // 0x0

	regVal = VpuReadReg(coreIdx, W5_RET_SUCCESS);
    if (regVal == 0) {
        Uint32      reasonCode = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);
        VLOG(ERR, "VPU init(W5_RET_SUCCESS) failed(%d) REASON CODE(%08x)\n", regVal, reasonCode);
        return RETCODE_FAILURE;
    }
	//APB_READ(0x108); // 0x1
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//SetupWave5Properties ///////////////////////////////////////////////////////////////////////////////////////
    printf("SetupWave5Properties\n");
    VpuWriteReg(coreIdx, W5_QUERY_OPTION, GET_VPU_INFO);
    VpuWriteReg(coreIdx, W5_VPU_BUSY_STATUS, 1);
    VpuWriteReg(coreIdx, W5_COMMAND, W5_QUERY);
    VpuWriteReg(coreIdx, W5_VPU_HOST_INT_REQ, 1);
    //APB_WRITE(0x104 0x0
	//APB_WRITE(0x70 0x1
	//APB_WRITE(0x100 0x4000
	//APB_WRITE(0x38 0x1
    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70 0x0

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {
    	//APB_READ(0x108 0x1
    	return RETCODE_QUERY_FAILURE;
    }
    else {
    	VpuReadReg(coreIdx, W5_RET_PRODUCT_NAME);
    	VpuReadReg(coreIdx, W5_PRODUCT_NUMBER);
    	//APB_READ(0x11c 0x57415645
    	//APB_READ(0x1044 0x521c

        VpuReadReg(coreIdx, W5_RET_PRODUCT_VERSION);
        VpuReadReg(coreIdx, W5_RET_FW_VERSION);
        VpuReadReg(coreIdx, W5_RET_CUSTOMER_ID);
        VpuReadReg(coreIdx, W5_RET_STD_DEF0);
        VpuReadReg(coreIdx, W5_RET_STD_DEF1);
        VpuReadReg(coreIdx, W5_RET_CONF_FEATURE);
        VpuReadReg(coreIdx, W5_RET_CONF_DATE);
        VpuReadReg(coreIdx, W5_RET_CONF_REVISION);
        VpuReadReg(coreIdx, W5_RET_CONF_TYPE);
    	//APB_READ(0x120 0x521c
    	//APB_READ(0x118 0x2e4a7
    	//APB_READ(0x140 0x0
    	//APB_READ(0x124 0x800d0908
    	//APB_READ(0x128 0x8018003
    	//APB_READ(0x12c 0x507
    	//APB_READ(0x130 0x1341660
    	//APB_READ(0x134 0x2b518
    	//APB_READ(0x138 0x0

    }
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//Wave5VpuReInit /////////////////////////////////////////
    printf("Wave5VpuReInit\n");
    VpuReadReg(coreIdx, W5_VPU_REMAP_PADDR);
	//APB_READ(0x68 0x8ab88000
    //////////////////////////////////////////////////////////


	//SetupWave5Properties ///////////////////////////////////
    VpuWriteReg(coreIdx, W5_QUERY_OPTION, GET_VPU_INFO);
    VpuWriteReg(coreIdx, W5_VPU_BUSY_STATUS, 1);
    VpuWriteReg(coreIdx, W5_COMMAND, W5_QUERY);
    VpuWriteReg(coreIdx, W5_VPU_HOST_INT_REQ, 1);
	//APB_WRITE(0x104 0x0
	//APB_WRITE(0x70 0x1
	//APB_WRITE(0x100 0x4000
	//APB_WRITE(0x38 0x1

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70 0x0
    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {
    	//APB_READ(0x108 0x1
    	return RETCODE_QUERY_FAILURE;
    }
    else {
    	VpuReadReg(coreIdx, W5_RET_PRODUCT_NAME);
    	VpuReadReg(coreIdx, W5_PRODUCT_NUMBER);
    	//APB_READ(0x11c 0x57415645
    	//APB_READ(0x1044 0x521c

        VpuReadReg(coreIdx, W5_RET_PRODUCT_VERSION);
        VpuReadReg(coreIdx, W5_RET_FW_VERSION);
        VpuReadReg(coreIdx, W5_RET_CUSTOMER_ID);
        VpuReadReg(coreIdx, W5_RET_STD_DEF0);
        VpuReadReg(coreIdx, W5_RET_STD_DEF1);
        VpuReadReg(coreIdx, W5_RET_CONF_FEATURE);
        VpuReadReg(coreIdx, W5_RET_CONF_DATE);
        VpuReadReg(coreIdx, W5_RET_CONF_REVISION);
        VpuReadReg(coreIdx, W5_RET_CONF_TYPE);
    	//APB_READ(0x120 0x521c
    	//APB_READ(0x118 0x2e4a7
    	//APB_READ(0x140 0x0
    	//APB_READ(0x124 0x800d0908
    	//APB_READ(0x128 0x8018003
    	//APB_READ(0x12c 0x507
    	//APB_READ(0x130 0x1341660
    	//APB_READ(0x134 0x2b518
    	//APB_READ(0x138 0x0
    }
	/////////////////////////////////////////////////////////////

	//Wave5VpuBuildUpEncParam /////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuBuildUpEncParam\n");

	osal_memset(0x8b188000, 0x00, 0x20000);
	
	APB_WRITE(0x114, 0x8b188000);
	APB_WRITE(0x118, 0x20000);

	APB_READ(0x1044);// 0x521c
	APB_WRITE(0x124, 0xf);
	APB_WRITE(0x13c, 0x0);
	//APB_WRITE(0x128, 0x2);
	APB_WRITE(0x128, 0x0);

	VpuWriteReg(coreIdx, W5_VPU_BUSY_STATUS, 1);
	VpuWriteReg(coreIdx, W5_RET_SUCCESS, 0);
	//APB_WRITE(0x70 0x1
	//APB_WRITE(0x108 0x0
	VpuWriteReg(coreIdx, W5_CMD_ENC_VCORE_INFO, 1);
	//APB_WRITE(0x194 0x1

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x8);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {   // Check QUEUE_DONE
       return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70 0x0

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {  // FAILED for adding into VCPU QUEUE
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);
        if (regVal != WAVE5_QUEUEING_FAIL)
            VLOG(ERR, "FAIL_REASON = 0x%x\n", regVal);
        if (regVal == 2)
        	return RETCODE_INVALID_SFS_INSTANCE;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
        	return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
        	return RETCODE_ERROR_FW_FATAL;
        else
        	return RETCODE_FAILURE;
    }
	//APB_READ(0x108 0x1
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////


	//Wave5VpuBuildUpEncParam /////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuBuildUpEncParam\n");
	
	osal_memset(0x8b1a8000, 0x00, 0x20000);
	
	APB_WRITE(0x114, 0x8b1a8000);
	APB_WRITE(0x118, 0x20000);

	APB_READ(0x1044);// 0x521c
	APB_WRITE(0x124, 0xf);
	APB_WRITE(0x13c, 0x0);
	//APB_WRITE(0x128, 0x2);
	APB_WRITE(0x128, 0x0);

	VpuWriteReg(coreIdx, W5_VPU_BUSY_STATUS, 1);
	VpuWriteReg(coreIdx, W5_RET_SUCCESS, 0);
	//APB_WRITE(0x70, 0x1);
	//APB_WRITE(0x108, 0x0);
	VpuWriteReg(coreIdx, W5_CMD_ENC_VCORE_INFO, 1);
	//APB_WRITE(0x194, 0x1);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x30001);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x8);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {   // Check QUEUE_DONE
       return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70);// 0x0

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {  // FAILED for adding into VCPU QUEUE
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);
        if (regVal != WAVE5_QUEUEING_FAIL)
            VLOG(ERR, "FAIL_REASON = 0x%x\n", regVal);
        if (regVal == 2)
        	return RETCODE_INVALID_SFS_INSTANCE;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
        	return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
        	return RETCODE_ERROR_FW_FATAL;
        else
        	return RETCODE_FAILURE;
    }
	//APB_READ(0x108);// 0x1
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuEncInitSeq //////////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuEncInitSeq\n");
	APB_WRITE(0x104, 0x1);
	APB_WRITE(0x11c, 0x1);
	APB_WRITE(0x120, 0x1045);
	APB_WRITE(0x124, 0x0);
	APB_WRITE(0x128, 0x0);
	APB_WRITE(0x12c, 0x0);
	APB_WRITE(0x130, 0x0);
	APB_WRITE(0x134, 0x0);
	APB_WRITE(0x138, 0x0);
	APB_WRITE(0x13c, 0x0);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x200);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70 0x0

	APB_WRITE(0x104, 0x0);
	APB_WRITE(0x11c, 0x8700f00);
	APB_WRITE(0x120, 0x0);
	APB_WRITE(0x124, 0x8020001);
	APB_WRITE(0x128, 0x60000000);
	APB_WRITE(0x12c, 0x0);
	APB_WRITE(0x130, 0x50001);
	APB_WRITE(0x13c, 0xd);
	APB_WRITE(0x148, 0x0);
	APB_WRITE(0x150, 0x1e);
	APB_WRITE(0x154, 0x1e8480);
	APB_WRITE(0x16c, 0x0);
	APB_WRITE(0x158, 0xbb8fc025);
	APB_WRITE(0x1c4, 0x8002);
	APB_WRITE(0x15c, 0xacc8);
	APB_WRITE(0x168, 0xcc8cc8);
	APB_WRITE(0x160, 0x1010101);
	APB_WRITE(0x164, 0x1010101);
	APB_WRITE(0x170, 0x0);
	APB_WRITE(0x1a8, 0x3800410);
	APB_WRITE(0x1ac, 0x0);
	APB_WRITE(0x134, 0x0);
	APB_WRITE(0x138, 0x0);
	APB_WRITE(0x140, 0x0);
	APB_WRITE(0x1b0, 0x0);
	APB_WRITE(0x174, 0x3e8);
	APB_WRITE(0x178, 0x7530);
	APB_WRITE(0x17c, 0x0);
	APB_WRITE(0x184, 0x0);
	APB_WRITE(0x188, 0x0);
	APB_WRITE(0x18c, 0x0);
	APB_WRITE(0x190, 0x0);
	APB_WRITE(0x194, 0x0);
	APB_WRITE(0x198, 0x0);
	APB_WRITE(0x19c, 0x0);
	APB_WRITE(0x144, 0x0);
	APB_WRITE(0x1a0, 0x0);
	APB_WRITE(0x1a4, 0x0);

	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x200);

	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70); //0x0

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == 0) {
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);
        if (regVal != WAVE5_QUEUEING_FAIL)
            VLOG(ERR, "FAIL_REASON = 0x%x\n", regVal);

        if ( regVal == WAVE5_QUEUEING_FAIL)
            return RETCODE_QUEUEING_FAILURE;
        else if (regVal == WAVE5_SYSERR_ACCESS_VIOLATION_HW)
            return RETCODE_MEMORY_ACCESS_VIOLATION;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
            return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
            return RETCODE_ERROR_FW_FATAL;
        else
            return RETCODE_FAILURE;
    }
	//APB_READ(0x108); //0x1
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//HandlingInterruptFlag ///////////////////////////////////////////////////////////////////////////////////
    printf("HandlingInterruptFlag\n");
    while(1)
    {
		if(APB_READ(0x44) == 1) // 0x1
		{
			if((reason = APB_READ(0x4c)) == 0x200) // 0x200
			{
				APB_WRITE(0x34, 0x200);
				APB_WRITE(0x3c, 0x1);
				break;
			}
		}
    }

    if (reason & (1 << INT_WAVE5_ENC_SET_PARAM)) {
        regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
        //APB_READ(0x1e8); // 0x1
        regVal = (regVal & 0xffff);
        if (regVal & (1 << 0/*instIndex*/)) {
            //ownInt = 1;
            //reason = (1 << INT_WAVE5_ENC_SET_PARAM);
            //remain_intr &= ~(Uint32)reason;

            //regVal = regVal & ~(1UL << instIndex);
            //VpuWriteReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST, regVal);
            APB_WRITE(0x1e8, 0x0);
        }
    }

    //VpuWriteReg(coreIdx, W5_VPU_VINT_REASON, remain_intr);
	APB_WRITE(0x4c, 0x0);
	//Wave5VpuWaitInterrupt : int_reason = 0x200
	//[vdi_get_instance_pool 468]
	//[vdi_unlock 161]
	//Wave5VpuClearInterrupt : flags = 0x200

	reason = VpuReadReg(coreIdx, W5_VPU_VINT_REASON_USR);
	//APB_READ(0x30); // 0x200
	reason &= ~reason;
    VpuWriteReg(coreIdx, W5_VPU_VINT_REASON_USR, reason);
	//APB_WRITE(0x30, 0x0);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuEncGetSeqInfo ////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuEncGetSeqInfo\n");
    //SendQuery
	APB_WRITE(0x104, 0x2);
	APB_WRITE(0x70, 0x1);
	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x4000);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
    	//APB_READ(0x70 0x0
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE)
        return RETCODE_FAILURE;
	//APB_READ(0x108 0x1

	APB_READ(0x1e0);// 0x0

    if (VpuReadReg(coreIdx, W5_RET_ENC_ENCODING_SUCCESS) != 1) {
        VpuReadReg(coreIdx, W5_RET_ENC_ERR_INFO);
        return RETCODE_FAILURE;
    }
	//APB_READ(0x1dc);// 0x1

    VpuReadReg(coreIdx, W5_RET_ENC_WARN_INFO);
	//APB_READ(0x1d4);// 0x0

	APB_READ(0x1fc);// 0x0
	APB_READ(0x11c);// 0x2
	APB_READ(0x120);// 0x1
	APB_READ(0x16c);// 0x0
	APB_READ(0x1b0);// 0x485000
	APB_READ(0x1b4);// 0x4540
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuEncInitSeq //////////////////////////////////////////////////////////////////////////////////
	printf("Wave5VpuEncInitSeq\n");
	APB_WRITE(0x104, 0x1);
	APB_WRITE(0x11c, 0x1);
	APB_WRITE(0x120, 0x1045);
	APB_WRITE(0x124, 0x0);
	APB_WRITE(0x128, 0x0);
	APB_WRITE(0x12c, 0x0);
	APB_WRITE(0x130, 0x0);
	APB_WRITE(0x134, 0x0);
	APB_WRITE(0x138, 0x0);
	APB_WRITE(0x13c, 0x0);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x30001);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x200);
	APB_WRITE(0x38, 0x1);

	if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70 0x0

	APB_WRITE(0x104, 0x0);
	APB_WRITE(0x11c, 0x2d00500);
	APB_WRITE(0x120, 0x0);
	APB_WRITE(0x124, 0x20001);
	APB_WRITE(0x128, 0x60000040);
	APB_WRITE(0x12c, 0x0);
	APB_WRITE(0x130, 0x50001);
	APB_WRITE(0x13c, 0xd);
	APB_WRITE(0x148, 0x10000);
	APB_WRITE(0x150, 0x1e);
	APB_WRITE(0x154, 0x1e8480);
	APB_WRITE(0x16c, 0x0);
	APB_WRITE(0x158, 0xbb8fc025);
	APB_WRITE(0x1c4, 0x8002);
	APB_WRITE(0x15c, 0xacc8);
	APB_WRITE(0x168, 0xcc8cc8);
	APB_WRITE(0x160, 0x1010101);
	APB_WRITE(0x164, 0x1010101);
	APB_WRITE(0x170, 0x0);
	APB_WRITE(0x1a8, 0x3800410);
	APB_WRITE(0x1ac, 0x0);
	APB_WRITE(0x134, 0x0);
	APB_WRITE(0x138, 0x0);
	APB_WRITE(0x140, 0x0);
	APB_WRITE(0x1b0, 0x0);
	APB_WRITE(0x174, 0x3e8);
	APB_WRITE(0x178, 0x7530);
	APB_WRITE(0x17c, 0x0);

	APB_WRITE(0x110, 0x30001);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x200);

	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70);// 0x0

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == 0) {
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);
        if (regVal != WAVE5_QUEUEING_FAIL)
            VLOG(ERR, "FAIL_REASON = 0x%x\n", regVal);

        if ( regVal == WAVE5_QUEUEING_FAIL)
            return RETCODE_QUEUEING_FAILURE;
        else if (regVal == WAVE5_SYSERR_ACCESS_VIOLATION_HW)
            return RETCODE_MEMORY_ACCESS_VIOLATION;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
            return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
            return RETCODE_ERROR_FW_FATAL;
        else
            return RETCODE_FAILURE;
    }
	//APB_READ(0x108);// 0x1
    /////////////////////////////////////////////////////////////////////////////////////////////////////////


	//HandlingInterruptFlag /////////////////////////////////////////////////////////////////////////////////
    printf("HandlingInterruptFlag\n");
    while(1)
    {
		if(APB_READ(0x44) == 1) // 0x1
		{
			if((reason = APB_READ(0x4c)) == 0x200) // 0x200
			{
				APB_WRITE(0x34, 0x200);
				APB_WRITE(0x3c, 0x1);
				break;
			}
		}
    }

    if (reason & (1 << INT_WAVE5_ENC_SET_PARAM)) {
        regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
        //APB_READ(0x1e8); // 0x2
        regVal = (regVal & 0xffff);
        if (regVal & (1 << 1/*instIndex*/)) {
            //ownInt = 1;
            //reason = (1 << INT_WAVE5_ENC_SET_PARAM);
            //remain_intr &= ~(Uint32)reason;

            //regVal = regVal & ~(1UL << instIndex);
            //VpuWriteReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST, regVal);
            APB_WRITE(0x1e8, 0x0);
        }
    }

    //VpuWriteReg(coreIdx, W5_VPU_VINT_REASON, remain_intr);
	APB_WRITE(0x4c, 0x0);
	//Wave5VpuWaitInterrupt : int_reason = 0x200
	//[vdi_get_instance_pool 468]
	//[vdi_unlock 161]
	//Wave5VpuClearInterrupt : flags = 0x200

	reason = VpuReadReg(coreIdx, W5_VPU_VINT_REASON_USR);
	//APB_READ(0x30 0x200

	reason &= ~reason;
    VpuWriteReg(coreIdx, W5_VPU_VINT_REASON_USR, reason);
	//APB_WRITE(0x30 0x0
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////


	//Wave5VpuEncGetSeqInfo ///////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuEncGetSeqInfo\n");
	APB_WRITE(0x104, 0x2);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x110, 0x30001);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x4000);
	APB_WRITE(0x38, 0x1);

	if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
		//APB_READ(0x70 0x0
		return RETCODE_VPU_RESPONSE_TIMEOUT;
	}

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE)
        return RETCODE_FAILURE;
	//APB_READ(0x108 0x1

	APB_READ(0x1e0);// 0x0

    if (VpuReadReg(coreIdx, W5_RET_ENC_ENCODING_SUCCESS) != 1) {
        VpuReadReg(coreIdx, W5_RET_ENC_ERR_INFO);
        return RETCODE_FAILURE;
    }
	//APB_READ(0x1dc 0x1

    VpuReadReg(coreIdx, W5_RET_ENC_WARN_INFO);
	//APB_READ(0x1d4 0x0

	APB_READ(0x1fc);// 0x1
	APB_READ(0x11c);// 0x2
	APB_READ(0x120);// 0x1
	APB_READ(0x16c);// 0x0
	APB_READ(0x1b0);// 0x150200
	APB_READ(0x1b4);// 0x2b00
	///////////////////////////////////////////////////////////////////////////////////////////////////////


	//Wave5VpuEncRegisterFramebuffer //////////////////////////////////////////////////////////////////////
	printf("Wave5VpuEncRegisterFramebuffer\n");
	//W5_CMD_SET_FB_ADDR_TASK_BUF
	APB_WRITE(0x1d4, 0x8e394000);
	//W5_CMD_SET_FB_TASK_BUF_SIZE
	APB_WRITE(0x1d8, 0xd93540);

	//W5_ADDR_SUB_SAMPLED_FB_BASE
	APB_WRITE(0x12c, 0x8e294000);
	//W5_SUB_SAMPLED_ONE_FB_SIZE
	APB_WRITE(0x130, 0x7f800);

	//W5_PIC_SIZE
	APB_WRITE(0x11c, 0xf000870);
	//W5_FBC_STRIDE
	APB_WRITE(0x128, 0x3c001e00);
	//W5_COMMON_PIC_INFO
	APB_WRITE(0x118, 0x10f00);
	APB_WRITE(0x104, 0x18);
	APB_WRITE(0x120, 0x1);
	APB_WRITE(0x134, 0x8c988000);
	APB_WRITE(0x138, 0x8d171000);
	APB_WRITE(0x13c, 0x8e1cc000);
	APB_WRITE(0x140, 0x8e24c000);
	APB_WRITE(0x1b4, 0x8e148000);
	APB_WRITE(0x144, 0x8d568000);
	APB_WRITE(0x148, 0x8dd51000);
	APB_WRITE(0x14c, 0x8e20b480);
	APB_WRITE(0x150, 0x8e26dc00);
	APB_WRITE(0x1b8, 0x8e187c00);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x80);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70);// 0x0

    regVal = VpuReadReg(coreIdx, W5_RET_SUCCESS);
    if (regVal == 0) {
        return RETCODE_FAILURE;
    }
	//APB_READ(0x108);// 0x1
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuEncRegisterFramebuffer//////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuEncRegisterFramebuffer\n");
    //W5_CMD_SET_FB_ADDR_TASK_BUF
	APB_WRITE(0x1d4, 0x8f744000);
	//W5_CMD_SET_FB_TASK_BUF_SIZE
	APB_WRITE(0x1d8, 0x3f3100);

	//W5_ADDR_SUB_SAMPLED_FB_BASE
	APB_WRITE(0x12c, 0x8f724000);
	//W5_SUB_SAMPLED_ONE_FB_SIZE
	APB_WRITE(0x130, 0xe100);

	//W5_PIC_SIZE
	APB_WRITE(0x11c, 0x50002d0);
	//W5_FBC_STRIDE
	APB_WRITE(0x128, 0x14000a00);
	//W5_COMMON_PIC_INFO
	APB_WRITE(0x118, 0x10500);
	APB_WRITE(0x104, 0x18);
	APB_WRITE(0x120, 0x1);
	APB_WRITE(0x134, 0x8f450000);
	APB_WRITE(0x138, 0x8f531000);
	APB_WRITE(0x13c, 0x8f708000);
	APB_WRITE(0x140, 0x8f718000);
	APB_WRITE(0x1b4, 0x8f6f8000);
	APB_WRITE(0x144, 0x8f5a4000);
	APB_WRITE(0x148, 0x8f685000);
	APB_WRITE(0x14c, 0x8f70f080);
	APB_WRITE(0x150, 0x8f71c380);
	APB_WRITE(0x1b8, 0x8f6ff800);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x30001);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x80);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70);// 0x0

    regVal = VpuReadReg(coreIdx, W5_RET_SUCCESS);
    if (regVal == 0) {
        return RETCODE_FAILURE;
    }
	//APB_READ(0x108);// 0x1
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//Wave5VpuEncGetHeader //////////////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuEncGetHeader\n");
    //W5_CMD_ENC_BS_START_ADDR
	APB_WRITE(0x118, 0x8ad88000);
	//W5_CMD_ENC_BS_SIZE
	APB_WRITE(0x11c, 0x200000);
	//W5_CMD_ENC_PIC_USE_SEC_AXI
	APB_WRITE(0x124, 0x0);
	//W5_CMD_ENC_PIC_CODE_OPTION
	APB_WRITE(0x164, 0x1c);
	//W5_CMD_ENC_PIC_SRC_PIC_IDX
	APB_WRITE(0x144, 0x0);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x100);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {   // Check QUEUE_DONE
       return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70); //0x0
    //W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x10000

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {           // FAILED for adding a command into VCPU QUEUE
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);

        if ( regVal == WAVE5_QUEUEING_FAIL)
            return RETCODE_QUEUEING_FAILURE;
        else if (regVal == WAVE5_SYSERR_ACCESS_VIOLATION_HW)
            return RETCODE_MEMORY_ACCESS_VIOLATION;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
            return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
            return RETCODE_ERROR_FW_FATAL;
        else
            return RETCODE_FAILURE;
    }
	//APB_READ(0x108); // 0x1
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//HandlingInterruptFlag //////////////////////////////////////////////////////////////////////////////////////
    printf("HandlingInterruptFlag\n");
    while(1)
    {
		if(APB_READ(0x44) == 1) // 0x1
		{
			if((reason = APB_READ(0x4c)) == 0x100) // 0x200
			{
				APB_WRITE(0x34, 0x100);
				APB_WRITE(0x3c, 0x1);
				break;
			}
		}
    }

    if (reason & (1 << INT_WAVE5_DEC_PIC)) {
        regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
    	//APB_READ(0x1e8 0x1

        regVal = (regVal & 0xffff);
        if (regVal & (1 << 0/*instance->instIndex*/)) {
            //ownInt = 1;
            int orgReason = reason;
            reason = (1 << INT_WAVE5_DEC_PIC);
            //remain_intr &= ~(Uint32)reason;
            /* Clear Low Latency Interrupt if two interrupts are occured */
            if (orgReason & (1 << INT_WAVE5_ENC_LOW_LATENCY)) {
                regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
                regVal = (regVal>>16);
                if (regVal & (1 << 0/*instance->instIndex*/)) {
                    //remain_intr &= ~(1<<INT_WAVE5_ENC_LOW_LATENCY);
                    Wave5VpuClearInterrupt(coreIdx, 1<<INT_WAVE5_ENC_LOW_LATENCY);
                }
            }
            regVal = regVal & ~(1UL << 0/*instance->instIndex*/);
            VpuWriteReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST, regVal);
            //APB_WRITE(0x1e8, 0x0);
        }
    }

    //VpuWriteReg(coreIdx, W5_VPU_VINT_REASON, remain_intr);
	APB_WRITE(0x4c, 0x0);

	//Wave5VpuWaitInterrupt : int_reason = 0x100
	//[vdi_get_instance_pool 468]
	//[vdi_unlock 161]
	//Wave5VpuClearInterrupt : flags = 0x100

	reason = VpuReadReg(coreIdx, W5_VPU_VINT_REASON_USR);
	//APB_READ(0x30); // 0x100

	reason &= ~reason;
    VpuWriteReg(coreIdx, W5_VPU_VINT_REASON_USR, reason);
	//APB_WRITE(0x30, 0x0);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//Wave5VpuEncGetResult //////////////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuEncGetResult\n");
    //SendQuery
	APB_WRITE(0x104, 0x2);
	APB_WRITE(0x70, 0x1);
	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x4000);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70); // 0x0

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE)
        return RETCODE_FAILURE;
	//APB_READ(0x108); // 0x1

    //W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x0


	if(APB_READ(0x1dc) != 1)
		return RETCODE_FAILURE;
	//W5_RET_ENC_ENCODING_SUCCESS
	//APB_READ(0x1dc); // 0x1

	//W5_RET_ENC_WARN_INFO
	APB_READ(0x1d4); // 0x0
	//W5_RET_ENC_PIC_NUM
	APB_READ(0x158); // 0x0
	//W5_RET_ENC_PIC_TYPE
	APB_READ(0x124); // 0x0
	//W5_RET_ENC_NUT_0
	APB_READ(0x15c); // 0x0
	//W5_RET_ENC_NUT_1
	APB_READ(0x160); // 0x0
	//W5_RET_ENC_PIC_IDX
	APB_READ(0x12c); // 0xfffffffd
	//W5_RET_ENC_SVC_LAYER
	APB_READ(0x170); // 0x0
	//W5_RET_ENC_PIC_SLICE_NUM
	APB_READ(0x130); // 0x0
	//W5_RET_ENC_PIC_SKIP
	APB_READ(0x134); // 0x0
	//W5_RET_ENC_PIC_NUM_INTRA
	APB_READ(0x138); // 0x0
	//W5_RET_ENC_PIC_NUM_MERGE
	APB_READ(0x13c); // 0x0
	//W5_RET_ENC_PIC_NUM_SKIP
	APB_READ(0x144); // 0x0
	//W5_RET_ENC_PIC_AVG_CTU_QP
	APB_READ(0x148); // 0x0
	//W5_RET_ENC_PIC_BYTE
	APB_READ(0x14c); // 0x54
	//W5_RET_ENC_GOP_PIC_IDX
	APB_READ(0x150); // 0xffffffff
	//W5_RET_ENC_PIC_POC
	APB_READ(0x128); // 0x0
	//W5_RET_ENC_USED_SRC_IDX
	APB_READ(0x154); // 0xfffffffd
	//W5_RET_ENC_SRC_BUF_FLAG
	APB_READ(0x18c); // 0x0

    //W5_RET_ENC_WR_PTR
	APB_READ(0x118); // 0x8ad88054
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); // 0x8ad88000

	//W5_RET_ENC_PIC_DIST_LOW
	APB_READ(0x164); // 0x0
	//W5_RET_ENC_PIC_DIST_HIGH
	APB_READ(0x168); // 0x0
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); // 0x8ad88000

	//RET_ENC_HOST_CMD_TICK
	APB_READ(0x1b8); // 0xd02ce
	//RET_ENC_PREPARE_START_TICK
	APB_READ(0x1bc); // 0xd02e6
	//RET_ENC_PREPARE_END_TICK
	APB_READ(0x1c0); // 0xd0386
	//RET_ENC_PROCESSING_START_TICK
	APB_READ(0x1c4); // 0xd03b0
	//RET_ENC_PROCESSING_END_TICK
	APB_READ(0x1c8); // 0xd03b3
	//RET_ENC_ENCODING_START_TICK
	APB_READ(0x1cc); // 0xd03c8
	//RET_ENC_ENCODING_END_TICK
	APB_READ(0x1d0); // 0xd03df
	printf("00     0     0    -3    8ad88000  8ad88054       54      0     -3 0000    0     69888    40960      768     5888      0 0\n");
	//00     0     0    -3    8ad88000  8ad88054       54      0     -3 0000    0     69888    40960      768     5888      0 0
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//Wave5VpuEncGetHeader ////////////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuEncGetHeader\n");
    //W5_CMD_ENC_BS_START_ADDR
	APB_WRITE(0x118, 0x8af88000);
	//W5_CMD_ENC_BS_SIZE
	APB_WRITE(0x11c, 0x200000);
	//W5_CMD_ENC_PIC_USE_SEC_AXI
	APB_WRITE(0x124, 0x0);
	//W5_CMD_ENC_PIC_CODE_OPTION
	APB_WRITE(0x164, 0x18);
	//W5_CMD_ENC_PIC_SRC_PIC_IDX
	APB_WRITE(0x144, 0x0);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x30001);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x100);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {   // Check QUEUE_DONE
       return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70);	// 0x0

    //W5_RET_QUEUE_STATUS
	APB_READ(0x1e0);	// 0x10000

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {           // FAILED for adding a command into VCPU QUEUE
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);

        if ( regVal == WAVE5_QUEUEING_FAIL)
            return RETCODE_QUEUEING_FAILURE;
        else if (regVal == WAVE5_SYSERR_ACCESS_VIOLATION_HW)
            return RETCODE_MEMORY_ACCESS_VIOLATION;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
            return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
            return RETCODE_ERROR_FW_FATAL;
        else
            return RETCODE_FAILURE;
    }
	//APB_READ(0x108);	// 0x1
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//HandlingInterruptFlag 350 ///////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("HandlingInterruptFlag\n");
    while(1)
    {
		if(APB_READ(0x44) == 1) // 0x1
		{
			if((reason = APB_READ(0x4c)) == 0x100) // 0x200
			{
				APB_WRITE(0x34, 0x100);
				APB_WRITE(0x3c, 0x1);
				break;
			}
		}
    }
    if (reason & (1 << INT_WAVE5_DEC_PIC)) {
        regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
    	//APB_READ(0x1e8 0x2

        regVal = (regVal & 0xffff);
        if (regVal & (1 << 1/*instance->instIndex*/)) {
            //ownInt = 1;
            int orgReason = reason;
            reason = (1 << INT_WAVE5_DEC_PIC);
            //remain_intr &= ~(Uint32)reason;
            /* Clear Low Latency Interrupt if two interrupts are occured */
            if (orgReason & (1 << INT_WAVE5_ENC_LOW_LATENCY)) {
                regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
                regVal = (regVal>>16);
                if (regVal & (1 << 1/*instance->instIndex*/)) {
                    //remain_intr &= ~(1<<INT_WAVE5_ENC_LOW_LATENCY);
                    Wave5VpuClearInterrupt(coreIdx, 1<<INT_WAVE5_ENC_LOW_LATENCY);
                }
            }
            regVal = regVal & ~(1UL << 1/*instance->instIndex*/);
            VpuWriteReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST, regVal);
            //APB_WRITE(0x1e8, 0x0);
        }
    }

    //VpuWriteReg(coreIdx, W5_VPU_VINT_REASON, remain_intr);
	APB_WRITE(0x4c, 0x0);

	//Wave5VpuWaitInterrupt : int_reason = 0x100
	//[vdi_get_instance_pool 468]
	//[vdi_unlock 161]
	//Wave5VpuClearInterrupt : flags = 0x100

	reason = VpuReadReg(coreIdx, W5_VPU_VINT_REASON_USR);
	APB_READ(0x30);// 0x100

	reason &= ~reason;
	VpuWriteReg(coreIdx, W5_VPU_VINT_REASON_USR, reason);
	//APB_WRITE(0x30, 0x0);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuEncGetResult ////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Wave5VpuEncGetResult\n");
	//SendQuery
	APB_WRITE(0x104, 0x2);
	APB_WRITE(0x70, 0x1);
	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x30001);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x4000);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70); // 0x0

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE)
        return RETCODE_FAILURE;
	//APB_READ(0x108); // 0x1

    //W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x0


	if(APB_READ(0x1dc) != 1)
		return RETCODE_FAILURE;
	//W5_RET_ENC_ENCODING_SUCCESS
	//APB_READ(0x1dc); // 0x1

	//W5_RET_ENC_WARN_INFO
	APB_READ(0x1d4); // 0x0
	//W5_RET_ENC_PIC_NUM
	APB_READ(0x158); // 0x0
	//W5_RET_ENC_PIC_TYPE
	APB_READ(0x124); // 0x0
	//W5_RET_ENC_NUT_0
	APB_READ(0x15c); // 0x0
	//W5_RET_ENC_NUT_1
	APB_READ(0x160); // 0x0
	//W5_RET_ENC_PIC_IDX
	APB_READ(0x12c); // 0xfffffffd
	//W5_RET_ENC_SVC_LAYER
	APB_READ(0x170); // 0x0
	//W5_RET_ENC_PIC_SLICE_NUM
	APB_READ(0x130); // 0x0
	//W5_RET_ENC_PIC_SKIP
	APB_READ(0x134); // 0x0
	//W5_RET_ENC_PIC_NUM_INTRA
	APB_READ(0x138); // 0x0
	//W5_RET_ENC_PIC_NUM_MERGE
	APB_READ(0x13c); // 0x0
	//W5_RET_ENC_PIC_NUM_SKIP
	APB_READ(0x144); // 0x0
	//W5_RET_ENC_PIC_AVG_CTU_QP
	APB_READ(0x148); // 0x0
	//W5_RET_ENC_PIC_BYTE
	APB_READ(0x14c); // 0x20
	//W5_RET_ENC_GOP_PIC_IDX
	APB_READ(0x150); // 0xffffffff
	//W5_RET_ENC_PIC_POC
	APB_READ(0x128); // 0x0
	//W5_RET_ENC_USED_SRC_IDX
	APB_READ(0x154); // 0xfffffffd
	//W5_RET_ENC_SRC_BUF_FLAG
	APB_READ(0x18c); // 0x0

	//W5_RET_ENC_WR_PTR
	APB_READ(0x118); // 0x8af88020
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); // 0x8af88000

	//W5_RET_ENC_PIC_DIST_LOW
	APB_READ(0x164); // 0x0
	//W5_RET_ENC_PIC_DIST_HIGH
	APB_READ(0x168); // 0x0
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); // 0x8af88000

	//RET_ENC_HOST_CMD_TICK
	APB_READ(0x1b8); // 0xe254f
	//RET_ENC_PREPARE_START_TICK
	APB_READ(0x1bc); // 0xe256e
	//RET_ENC_PREPARE_END_TICK
	APB_READ(0x1c0); // 0xe25bc
	//RET_ENC_PROCESSING_START_TICK
	APB_READ(0x1c4); // 0xe25e5
	//RET_ENC_PROCESSING_END_TICK
	APB_READ(0x1c8); // 0xe25e8
	//RET_ENC_ENCODING_START_TICK
	APB_READ(0x1cc); // 0xe25fa
	//RET_ENC_ENCODING_END_TICK
	APB_READ(0x1d0); // 0xe2607

	printf("01     0     0    -3    8af88000  8af88020       20      0     -3 0000    0     47104    19968      768     3328      0 0\n");
	//01     0     0    -3    8af88000  8af88020       20      0     -3 0000    0     47104    19968      768     3328      0 0
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	//Wave5VpuEncode //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Wave5VpuEncode\n");
	//W5_CMD_ENC_BS_START_ADDR
	APB_WRITE(0x118, 0x8ad88000);
	//W5_CMD_ENC_BS_SIZE
	APB_WRITE(0x11c, 0x200000);
	//W5_CMD_ENC_PIC_SRC_AXI_SEL
	APB_WRITE(0x160, 0x0);
	//W5_CMD_ENC_PIC_USE_SEC_AXI
	APB_WRITE(0x124, 0x0);
	//W5_CMD_ENC_PIC_REPORT_ENDIAN
	APB_WRITE(0x12c, 0x10);

	//W5_CMD_ENC_PIC_CODE_OPTION
	APB_WRITE(0x164, 0x3);
	//W5_CMD_ENC_PIC_PIC_PARAM
	APB_WRITE(0x168, 0x0);

	//W5_CMD_ENC_PIC_SRC_PIC_IDX
	APB_WRITE(0x144, 0x0);
	//W5_CMD_ENC_PIC_SRC_ADDR_Y
	APB_WRITE(0x148, 0x8b1c8000);
	//W5_CMD_ENC_PIC_SRC_ADDR_U
	APB_WRITE(0x14c, 0x8b9b1000);
	//W5_CMD_ENC_PIC_SRC_ADDR_V
	APB_WRITE(0x150, 0xffffffff);

	//W5_CMD_ENC_PIC_SRC_STRIDE
	APB_WRITE(0x154, 0xf000f00);

	//W5_CMD_ENC_PIC_SRC_FORMAT
	APB_WRITE(0x158, 0x3e2);
	//W5_CMD_ENC_PIC_CUSTOM_MAP_OPTION_ADDR
	APB_WRITE(0x13c, 0x0);
	//W5_CMD_ENC_PIC_CUSTOM_MAP_OPTION_PARAM
	APB_WRITE(0x138, 0x0);
	//W5_CMD_ENC_PIC_LONGTERM_PIC
	APB_WRITE(0x16c, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_SIGMA_Y
	APB_WRITE(0x170, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_SIGMA_C
	APB_WRITE(0x174, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_MEAN_Y
	APB_WRITE(0x178, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_MEAN_C
	APB_WRITE(0x17c, 0x0);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x100);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {   // Check QUEUE_DONE
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70); // 0x0

    //W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x10000

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {           // FAILED for adding a command into VCPU QUEUE
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);
        if (regVal != WAVE5_QUEUEING_FAIL)
            VLOG(ERR, "FAIL_REASON = 0x%x\n", regVal);

        if (regVal == WAVE5_QUEUEING_FAIL)
            return RETCODE_QUEUEING_FAILURE;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
            return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
            return RETCODE_ERROR_FW_FATAL;
        else
            return RETCODE_FAILURE;
    }
	//APB_READ(0x108); // 0x1
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//HandlingInterruptFlag //////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("HandlingInterruptFlag\n");
    while(1)
    {
		if(APB_READ(0x44) == 1) // 0x1
		{
			if((reason = APB_READ(0x4c)) == 0x100) // 0x200
			{
				APB_WRITE(0x34, 0x100);
				APB_WRITE(0x3c, 0x1);
				break;
			}
		}
    }

    if (reason & (1 << INT_WAVE5_DEC_PIC)) {
        regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
    	//APB_READ(0x1e8 0x1

        regVal = (regVal & 0xffff);
        if (regVal & (1 << 0/*instance->instIndex*/)) {
            //ownInt = 1;
            int orgReason = reason;
            reason = (1 << INT_WAVE5_DEC_PIC);
            //remain_intr &= ~(Uint32)reason;
            /* Clear Low Latency Interrupt if two interrupts are occured */
            if (orgReason & (1 << INT_WAVE5_ENC_LOW_LATENCY)) {
                regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
                regVal = (regVal>>16);
                if (regVal & (1 << 0/*instance->instIndex*/)) {
                    //remain_intr &= ~(1<<INT_WAVE5_ENC_LOW_LATENCY);
                    Wave5VpuClearInterrupt(coreIdx, 1<<INT_WAVE5_ENC_LOW_LATENCY);
                }
            }
            regVal = regVal & ~(1UL << 0/*instance->instIndex*/);
            VpuWriteReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST, regVal);
            //APB_WRITE(0x1e8, 0x0);
        }
    }

    //VpuWriteReg(coreIdx, W5_VPU_VINT_REASON, remain_intr);
	APB_WRITE(0x4c, 0x0);

	//Wave5VpuWaitInterrupt : int_reason = 0x100
	//[vdi_get_instance_pool 468]
	//[vdi_unlock 161]
	//Wave5VpuClearInterrupt : flags = 0x100

	reason = VpuReadReg(coreIdx, W5_VPU_VINT_REASON_USR);
	//APB_READ(0x30); // 0x100

	reason &= ~reason;
	VpuWriteReg(coreIdx, W5_VPU_VINT_REASON_USR, reason);
	//APB_WRITE(0x30, 0x0);


	//Wave5VpuEncGetResult /////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Wave5VpuEncGetResult\n");
	//SendQuery
	APB_WRITE(0x104, 0x2);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x4000);
	APB_WRITE(0x38, 0x1);

	if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
	   return RETCODE_VPU_RESPONSE_TIMEOUT;
	}
	//APB_READ(0x70); // 0x0

	if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE)
	    return RETCODE_FAILURE;
	//APB_READ(0x108); // 0x1

	//W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x0


	if(APB_READ(0x1dc) != 1)
		return RETCODE_FAILURE;
	//W5_RET_ENC_ENCODING_SUCCESS
	//APB_READ(0x1dc); // 0x1

	//W5_RET_ENC_WARN_INFO
	APB_READ(0x1d4); // 0x0
	//W5_RET_ENC_PIC_NUM
	APB_READ(0x158); // 0x1
	//W5_RET_ENC_PIC_TYPE
	APB_READ(0x124); // 0x0
	//W5_RET_ENC_NUT_0
	APB_READ(0x15c); // 0x13
	//W5_RET_ENC_NUT_1
	APB_READ(0x160); // 0x0
	//W5_RET_ENC_PIC_IDX
	APB_READ(0x12c); // 0x0
	//W5_RET_ENC_SVC_LAYER
	APB_READ(0x170); // 0x0
	//W5_RET_ENC_PIC_SLICE_NUM
	APB_READ(0x130); // 0x55980
	//W5_RET_ENC_PIC_SKIP
	APB_READ(0x134); // 0x0
	//W5_RET_ENC_PIC_NUM_INTRA
	APB_READ(0x138); // 0x1fa40
	//W5_RET_ENC_PIC_NUM_MERGE
	APB_READ(0x13c); // 0x0
	//W5_RET_ENC_PIC_NUM_SKIP
	APB_READ(0x144); // 0x0
	//W5_RET_ENC_PIC_AVG_CTU_QP
	APB_READ(0x148); // 0x2a
	//W5_RET_ENC_PIC_BYTE
	APB_READ(0x14c); // 0xc469a
	//W5_RET_ENC_GOP_PIC_IDX
	APB_READ(0x150); // 0x0
	//W5_RET_ENC_PIC_POC
	APB_READ(0x128); // 0x0
	//W5_RET_ENC_USED_SRC_IDX
	APB_READ(0x154); // 0x0
	//W5_RET_ENC_SRC_BUF_FLAG
	APB_READ(0x18c); // 0x1

    //W5_RET_ENC_WR_PTR
	APB_READ(0x118); // 0x8ae4c69a
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); // 0x8ad88000

	//W5_RET_ENC_PIC_DIST_LOW
	APB_READ(0x164); // 0x8004e9a7
	//W5_RET_ENC_PIC_DIST_HIGH
	APB_READ(0x168); // 0x8b1a3e7e
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); // 0x8ad88000

	//RET_ENC_HOST_CMD_TICK
	APB_READ(0x1b8); // 0x18c262
	//RET_ENC_PREPARE_START_TICK
	APB_READ(0x1bc); // 0x18c282
	//RET_ENC_PREPARE_END_TICK
	APB_READ(0x1c0); // 0x18c36f
	//RET_ENC_PROCESSING_START_TICK
	APB_READ(0x1c4); // 0x18c3a9
	//RET_ENC_PROCESSING_END_TICK
	APB_READ(0x1c8); // 0x19361d
	//RET_ENC_ENCODING_START_TICK
	APB_READ(0x1cc); // 0x193667
	//RET_ENC_ENCODING_END_TICK
	APB_READ(0x1d0); // 0x1973d8

	printf("00     1     0     0    8ad88000  8ae4c69a    c469a      0      0 0001    0  11630080    60672  7500800  4026624      0 0\n");
	//00     1     0     0    8ad88000  8ae4c69a    c469a      0      0 0001    0  11630080    60672  7500800  4026624      0 0
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




	//Wave5VpuEncode ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Wave5VpuEncode\n");
	//W5_CMD_ENC_BS_START_ADDR
	APB_WRITE(0x118, 0x8af88000);
	//W5_CMD_ENC_BS_SIZE
	APB_WRITE(0x11c, 0x200000);
	//W5_CMD_ENC_PIC_SRC_AXI_SEL
	APB_WRITE(0x160, 0x0);
	//W5_CMD_ENC_PIC_USE_SEC_AXI
	APB_WRITE(0x124, 0x0);
	//W5_CMD_ENC_PIC_REPORT_ENDIAN
	APB_WRITE(0x12c, 0x10);

	//W5_CMD_ENC_PIC_CODE_OPTION
	APB_WRITE(0x164, 0x3);
	//W5_CMD_ENC_PIC_PIC_PARAM
	APB_WRITE(0x168, 0x0);

	//W5_CMD_ENC_PIC_SRC_PIC_IDX
	APB_WRITE(0x144, 0x0);
	//W5_CMD_ENC_PIC_SRC_ADDR_Y
	APB_WRITE(0x148, 0x8f1a8000);
	//W5_CMD_ENC_PIC_SRC_ADDR_U
	APB_WRITE(0x14c, 0x8f289000);
	//W5_CMD_ENC_PIC_SRC_ADDR_V
	APB_WRITE(0x150, 0xffffffff);

	//W5_CMD_ENC_PIC_SRC_STRIDE;
	APB_WRITE(0x154, 0x5000500);

	//W5_CMD_ENC_PIC_SRC_FORMAT
	APB_WRITE(0x158, 0x3e2);
	//W5_CMD_ENC_PIC_CUSTOM_MAP_OPTION_ADDR
	APB_WRITE(0x13c, 0x0);
	//W5_CMD_ENC_PIC_CUSTOM_MAP_OPTION_PARAM
	APB_WRITE(0x138, 0x0);
	//W5_CMD_ENC_PIC_LONGTERM_PIC
	APB_WRITE(0x16c, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_SIGMA_Y
	APB_WRITE(0x170, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_SIGMA_C
	APB_WRITE(0x174, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_MEAN_Y
	APB_WRITE(0x178, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_MEAN_C
	APB_WRITE(0x17c, 0x0);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x30001);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x100);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {   // Check QUEUE_DONE
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70); // 0x0

    //W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x10000

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {           // FAILED for adding a command into VCPU QUEUE
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);
        if (regVal != WAVE5_QUEUEING_FAIL)
            VLOG(ERR, "FAIL_REASON = 0x%x\n", regVal);

        if (regVal == WAVE5_QUEUEING_FAIL)
            return RETCODE_QUEUEING_FAILURE;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
            return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
            return RETCODE_ERROR_FW_FATAL;
        else
            return RETCODE_FAILURE;
    }
	//APB_READ(0x108); // 0x1
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//HandlingInterruptFlag //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("HandlingInterruptFlag\n");
    while(1)
	{
		if(APB_READ(0x44) == 1) // 0x1
		{
			if((reason = APB_READ(0x4c)) == 0x100) // 0x100
			{
				APB_WRITE(0x34, 0x100);
				APB_WRITE(0x3c, 0x1);
				break;
			}
		}
	}

    if (reason & (1 << INT_WAVE5_DEC_PIC)) {
        regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
    	//APB_READ(0x1e8 0x2

        regVal = (regVal & 0xffff);
        if (regVal & (1 << 1/*instance->instIndex*/)) {
            //ownInt = 1;
            int orgReason = reason;
            reason = (1 << INT_WAVE5_DEC_PIC);
            //remain_intr &= ~(Uint32)reason;
            /* Clear Low Latency Interrupt if two interrupts are occured */
            if (orgReason & (1 << INT_WAVE5_ENC_LOW_LATENCY)) {
                regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
                regVal = (regVal>>16);
                if (regVal & (1 << 1/*instance->instIndex*/)) {
                    //remain_intr &= ~(1<<INT_WAVE5_ENC_LOW_LATENCY);
                    Wave5VpuClearInterrupt(coreIdx, 1<<INT_WAVE5_ENC_LOW_LATENCY);
                }
            }
            regVal = regVal & ~(1UL << 1/*instance->instIndex*/);
            VpuWriteReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST, regVal);
            //APB_WRITE(0x1e8, 0x0);
        }
    }

    //VpuWriteReg(coreIdx, W5_VPU_VINT_REASON, remain_intr);
	APB_WRITE(0x4c, 0x0);

	//Wave5VpuWaitInterrupt : int_reason = 0x100
	//[vdi_get_instance_pool 468]
	//[vdi_unlock 161]
	//Wave5VpuClearInterrupt : flags = 0x100

	reason = VpuReadReg(coreIdx, W5_VPU_VINT_REASON_USR);
	//APB_READ(0x30 0x100

	reason &= ~reason;
	VpuWriteReg(coreIdx, W5_VPU_VINT_REASON_USR, reason);
	//APB_WRITE(0x30 0x0
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuEncGetResult ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Wave5VpuEncGetResult\n");
	//SendQuery
	APB_WRITE(0x104, 0x2);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x110, 0x30001);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x4000);
	APB_WRITE(0x38, 0x1);

	if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
	   return RETCODE_VPU_RESPONSE_TIMEOUT;
	}
	//APB_READ(0x70 0x0

	if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE)
	    return RETCODE_FAILURE;
	//APB_READ(0x108 0x1

	//W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); //  0x0

	if(APB_READ(0x1dc) != 1)
		return RETCODE_FAILURE;
	//W5_RET_ENC_ENCODING_SUCCESS
	//APB_READ(0x1dc); //  0x1

	//W5_RET_ENC_WARN_INFO
	APB_READ(0x1d4); //  0x0
	//W5_RET_ENC_PIC_NUM
	APB_READ(0x158); //  0x1
	//W5_RET_ENC_PIC_TYPE
	APB_READ(0x124); //  0x0
	//W5_RET_ENC_NUT_0
	APB_READ(0x15c); //  0x5
	//W5_RET_ENC_NUT_1
	APB_READ(0x160); //  0x0
	//W5_RET_ENC_PIC_IDX
	APB_READ(0x12c); //  0x0
	//W5_RET_ENC_SVC_LAYER
	APB_READ(0x170); //  0x0
	//W5_RET_ENC_PIC_SLICE_NUM
	APB_READ(0x130); //  0x10000
	//W5_RET_ENC_PIC_SKIP
	APB_READ(0x134); //  0x0
	//W5_RET_ENC_PIC_NUM_INTRA
	APB_READ(0x138); //  0x2
	//W5_RET_ENC_PIC_NUM_MERGE
	APB_READ(0x13c); //  0x87c0
	//W5_RET_ENC_PIC_NUM_SKIP
	APB_READ(0x144); //  0x8b1c3e80
	//W5_RET_ENC_PIC_AVG_CTU_QP
	APB_READ(0x148); //  0x20
	//W5_RET_ENC_PIC_BYTE
	APB_READ(0x14c); //  0x348a0
	//W5_RET_ENC_GOP_PIC_IDX
	APB_READ(0x150); //  0x0
	//W5_RET_ENC_PIC_POC
	APB_READ(0x128); //  0x0
	//W5_RET_ENC_USED_SRC_IDX
	APB_READ(0x154); //  0x0
	//W5_RET_ENC_SRC_BUF_FLAG
	APB_READ(0x18c); //  0x1

	//W5_RET_ENC_WR_PTR
	APB_READ(0x118); //  0x8afbc8a0
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); //  0x8af88000

	//W5_RET_ENC_PIC_DIST_LOW
	APB_READ(0x164); //  0xe0c00
	//W5_RET_ENC_PIC_DIST_HIGH
	APB_READ(0x168); //  0x8b1c3e81
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); //  0x8af88000

	//RET_ENC_HOST_CMD_TICK
	APB_READ(0x1b8); //  0x1a459b
	//RET_ENC_PREPARE_START_TICK
	APB_READ(0x1bc); //  0x1a45bc
	//RET_ENC_PREPARE_END_TICK
	APB_READ(0x1c0); //  0x1a4657
	//RET_ENC_PROCESSING_START_TICK
	APB_READ(0x1c4); //  0x1a468e
	//RET_ENC_PROCESSING_END_TICK
	APB_READ(0x1c8); //  0x1a51c6
	//RET_ENC_ENCODING_START_TICK
	APB_READ(0x1cc); //  0x1a520c
	//RET_ENC_ENCODING_END_TICK
	APB_READ(0x1d0); //  0x1a6341

	printf("01     1     0     0    8af88000  8afbc8a0    348a0      0      0 0001    0   1943040    39680   735232  1127680      0 0\n");
	//01     1     0     0    8af88000  8afbc8a0    348a0      0      0 0001    0   1943040    39680   735232  1127680      0 0
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuEncode////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Wave5VpuEncode\n");
	//W5_CMD_ENC_BS_START_ADDR
	APB_WRITE(0x118, 0x8ad88000);
	//W5_CMD_ENC_BS_SIZE
	APB_WRITE(0x11c, 0x200000);
	//W5_CMD_ENC_PIC_SRC_AXI_SEL
	APB_WRITE(0x160, 0x0);
	//W5_CMD_ENC_PIC_USE_SEC_AXI
	APB_WRITE(0x124, 0x0);
	//W5_CMD_ENC_PIC_REPORT_ENDIAN
	APB_WRITE(0x12c, 0x10);

	//W5_CMD_ENC_PIC_CODE_OPTION
	APB_WRITE(0x164, 0x3);
	//W5_CMD_ENC_PIC_PIC_PARAM
	APB_WRITE(0x168, 0x0);

	//W5_CMD_ENC_PIC_SRC_PIC_IDX
	APB_WRITE(0x144, 0x1);
	//W5_CMD_ENC_PIC_SRC_ADDR_Y
	APB_WRITE(0x148, 0x8bda8000);
	//W5_CMD_ENC_PIC_SRC_ADDR_U
	APB_WRITE(0x14c, 0x8c591000);
	//W5_CMD_ENC_PIC_SRC_ADDR_V
	APB_WRITE(0x150, 0xffffffff);

	//W5_CMD_ENC_PIC_SRC_ADDR_V
	APB_WRITE(0x154, 0xf000f00);

	//W5_CMD_ENC_PIC_SRC_FORMAT
	APB_WRITE(0x158, 0x3e2);
	//W5_CMD_ENC_PIC_CUSTOM_MAP_OPTION_ADDR
	APB_WRITE(0x13c, 0x0);
	//W5_CMD_ENC_PIC_CUSTOM_MAP_OPTION_PARAM
	APB_WRITE(0x138, 0x0);
	//W5_CMD_ENC_PIC_LONGTERM_PIC
	APB_WRITE(0x16c, 0x0);
	//W5_CMD_ENC_PIC_LONGTERM_PIC
	APB_WRITE(0x170, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_SIGMA_C
	APB_WRITE(0x174, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_MEAN_Y
	APB_WRITE(0x178, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_MEAN_C
	APB_WRITE(0x17c, 0x0);

	//W5_CMD_ENC_PIC_WP_PIXEL_MEAN_C
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x100);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {   // Check QUEUE_DONE
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70); // 0x0

    //W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x10000

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {           // FAILED for adding a command into VCPU QUEUE
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);
        if (regVal != WAVE5_QUEUEING_FAIL)
            VLOG(ERR, "FAIL_REASON = 0x%x\n", regVal);

        if (regVal == WAVE5_QUEUEING_FAIL)
            return RETCODE_QUEUEING_FAILURE;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
            return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
            return RETCODE_ERROR_FW_FATAL;
        else
            return RETCODE_FAILURE;
    }
	//APB_READ(0x108); // 0x1
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//HandlingInterruptFlag 350 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while(1)
    {
		if(APB_READ(0x44) == 1) // 0x1
		{
			if((reason = APB_READ(0x4c)) == 0x100) // 0x200
			{
				APB_WRITE(0x34, 0x100);
				APB_WRITE(0x3c, 0x1);
				break;
			}
		}
    }

    if (reason & (1 << INT_WAVE5_DEC_PIC)) {
        regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
    	//APB_READ(0x1e8 0x1

        regVal = (regVal & 0xffff);
        if (regVal & (1 << 0/*instance->instIndex*/)) {
            //ownInt = 1;
            int orgReason = reason;
            reason = (1 << INT_WAVE5_DEC_PIC);
            //remain_intr &= ~(Uint32)reason;
            /* Clear Low Latency Interrupt if two interrupts are occured */
            if (orgReason & (1 << INT_WAVE5_ENC_LOW_LATENCY)) {
                regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
                regVal = (regVal>>16);
                if (regVal & (1 << 0/*instance->instIndex*/)) {
                    //remain_intr &= ~(1<<INT_WAVE5_ENC_LOW_LATENCY);
                    Wave5VpuClearInterrupt(coreIdx, 1<<INT_WAVE5_ENC_LOW_LATENCY);
                }
            }
            regVal = regVal & ~(1UL << 0/*instance->instIndex*/);
            VpuWriteReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST, regVal);
            //APB_WRITE(0x1e8, 0x0);
        }
    }

    //VpuWriteReg(coreIdx, W5_VPU_VINT_REASON, remain_intr);
	APB_WRITE(0x4c, 0x0);

	//Wave5VpuWaitInterrupt : int_reason = 0x100
	//[vdi_get_instance_pool 468]
	//[vdi_unlock 161]
	//Wave5VpuClearInterrupt : flags = 0x100

	reason = VpuReadReg(coreIdx, W5_VPU_VINT_REASON_USR);
	//APB_READ(0x30 0x100

	reason &= ~reason;
	VpuWriteReg(coreIdx, W5_VPU_VINT_REASON_USR, reason);
	//APB_WRITE(0x30 0x0
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuEncGetResult /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//SendQuery
	APB_WRITE(0x104, 0x2);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x4000);
	APB_WRITE(0x38, 0x1);

	if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
	   return RETCODE_VPU_RESPONSE_TIMEOUT;
	}
	//APB_READ(0x70); // 0x0

	if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE)
	    return RETCODE_FAILURE;
	//APB_READ(0x108); // 0x1

	//W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x0

	if(APB_READ(0x1dc) != 1)
		return RETCODE_FAILURE;
	//W5_RET_ENC_ENCODING_SUCCESS
	//APB_READ(0x1dc); // 0x1


	//W5_RET_ENC_WARN_INFO
	APB_READ(0x1d4); // 0x0
	//W5_RET_ENC_PIC_NUM
	APB_READ(0x158); // 0x2
	//W5_RET_ENC_PIC_TYPE
	APB_READ(0x124); // 0x1
	//W5_RET_ENC_NUT_0
	APB_READ(0x15c); // 0x1
	//W5_RET_ENC_NUT_1
	APB_READ(0x160); // 0x0
	//W5_RET_ENC_PIC_IDX
	APB_READ(0x12c); // 0x1
	//W5_RET_ENC_SVC_LAYER
	APB_READ(0x170); // 0x0
	//W5_RET_ENC_PIC_SLICE_NUM
	APB_READ(0x130); // 0x55980
	//W5_RET_ENC_PIC_SKIP
	APB_READ(0x134); // 0x0
	//W5_RET_ENC_PIC_NUM_INTRA
	APB_READ(0x138); // 0x1b599
	//W5_RET_ENC_PIC_NUM_MERGE
	APB_READ(0x13c); // 0x1d80
	//W5_RET_ENC_PIC_NUM_SKIP
	APB_READ(0x144); // 0x95
	//W5_RET_ENC_PIC_NUM_SKIP
	APB_READ(0x148); // 0x33
	//W5_RET_ENC_PIC_BYTE
	APB_READ(0x14c); // 0x39200
	//W5_RET_ENC_GOP_PIC_IDX
	APB_READ(0x150); // 0x0
	//W5_RET_ENC_PIC_POC
	APB_READ(0x128); // 0x1
	//W5_RET_ENC_USED_SRC_IDX
	APB_READ(0x154); // 0x1
	//W5_RET_ENC_SRC_BUF_FLAG
	APB_READ(0x18c); // 0x2

    //W5_RET_ENC_WR_PTR
	APB_READ(0x118); // 0x8adc1200
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); // 0x8ad88000
	//W5_RET_ENC_PIC_DIST_LOW
	APB_READ(0x164); // 0x8004edbf
	//W5_RET_ENC_PIC_DIST_HIGH
	APB_READ(0x168); // 0x8b1a3e7e
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); // 0x8ad88000

	//RET_ENC_HOST_CMD_TICK
	APB_READ(0x1b8); // 0x1b1ed3
	//RET_ENC_PREPARE_START_TICK
	APB_READ(0x1bc); // 0x1b1efd
	//RET_ENC_PREPARE_END_TICK
	APB_READ(0x1c0); // 0x1b1ff7
	//RET_ENC_PREPARE_END_TICK
	APB_READ(0x1c4); // 0x1b2032
	//RET_ENC_PROCESSING_END_TICK
	APB_READ(0x1c8); // 0x1b9649
	//RET_ENC_ENCODING_START_TICK
	APB_READ(0x1cc); // 0x1b9693
	//RET_ENC_ENCODING_END_TICK
	APB_READ(0x1d0); // 0x1bb555


	printf("00     2     1     1    8ad88000  8adc1200    39200      1      1 0002    0   9863680    64000  7739136  2015744      0 0\n");
	//00     2     1     1    8ad88000  8adc1200    39200      1      1 0002    0   9863680    64000  7739136  2015744      0 0
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuEncode ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Wave5VpuEncode\n");
	//W5_CMD_ENC_BS_START_ADDR
	APB_WRITE(0x118, 0x8af88000);
	//W5_CMD_ENC_BS_SIZE
	APB_WRITE(0x11c, 0x200000);
	//W5_CMD_ENC_PIC_SRC_AXI_SEL
	APB_WRITE(0x160, 0x0);
	//W5_CMD_ENC_PIC_USE_SEC_AXI
	APB_WRITE(0x124, 0x0);
	//W5_CMD_ENC_PIC_REPORT_ENDIAN
	APB_WRITE(0x12c, 0x10);

	//W5_CMD_ENC_PIC_CODE_OPTION
	APB_WRITE(0x164, 0x3);
	//W5_CMD_ENC_PIC_PIC_PARAM
	APB_WRITE(0x168, 0x0);

	//W5_CMD_ENC_PIC_SRC_PIC_IDX
	APB_WRITE(0x144, 0x1);
	//W5_CMD_ENC_PIC_SRC_ADDR_Y
	APB_WRITE(0x148, 0x8f2fc000);
	//W5_CMD_ENC_PIC_SRC_ADDR_U
	APB_WRITE(0x14c, 0x8f3dd000);
	//W5_CMD_ENC_PIC_SRC_ADDR_V
	APB_WRITE(0x150, 0xffffffff);

	//W5_CMD_ENC_PIC_SRC_STRIDE
	APB_WRITE(0x154, 0x5000500);

	//W5_CMD_ENC_PIC_SRC_FORMAT
	APB_WRITE(0x158, 0x3e2);
	//W5_CMD_ENC_PIC_CUSTOM_MAP_OPTION_ADDR
	APB_WRITE(0x13c, 0x0);
	//W5_CMD_ENC_PIC_CUSTOM_MAP_OPTION_PARAM
	APB_WRITE(0x138, 0x0);
	//W5_CMD_ENC_PIC_LONGTERM_PIC
	APB_WRITE(0x16c, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_SIGMA_Y
	APB_WRITE(0x170, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_SIGMA_C
	APB_WRITE(0x174, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_MEAN_Y
	APB_WRITE(0x178, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_MEAN_C
	APB_WRITE(0x17c, 0x0);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x30001);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x100);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {   // Check QUEUE_DONE
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70); // 0x0

    //W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x10000

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {           // FAILED for adding a command into VCPU QUEUE
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);
        if (regVal != WAVE5_QUEUEING_FAIL)
            VLOG(ERR, "FAIL_REASON = 0x%x\n", regVal);

        if (regVal == WAVE5_QUEUEING_FAIL)
            return RETCODE_QUEUEING_FAILURE;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
            return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
            return RETCODE_ERROR_FW_FATAL;
        else
            return RETCODE_FAILURE;
    }
	//APB_READ(0x108); // 0x1
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//HandlingInterruptFlag /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while(1)
    {
		if(APB_READ(0x44) == 1) // 0x1
		{
			if((reason = APB_READ(0x4c)) == 0x100) // 0x200
			{
				APB_WRITE(0x34, 0x100);
				APB_WRITE(0x3c, 0x1);
				break;
			}
		}
    }

    if (reason & (1 << INT_WAVE5_DEC_PIC)) {
        regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
    	//APB_READ(0x1e8 0x2

        regVal = (regVal & 0xffff);
        if (regVal & (1 << 1/*instance->instIndex*/)) {
            //ownInt = 1;
            int orgReason = reason;
            reason = (1 << INT_WAVE5_DEC_PIC);
            //remain_intr &= ~(Uint32)reason;
            /* Clear Low Latency Interrupt if two interrupts are occured */
            if (orgReason & (1 << INT_WAVE5_ENC_LOW_LATENCY)) {
                regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
                regVal = (regVal>>16);
                if (regVal & (1 << 1/*instance->instIndex*/)) {
                    //remain_intr &= ~(1<<INT_WAVE5_ENC_LOW_LATENCY);
                    Wave5VpuClearInterrupt(coreIdx, 1<<INT_WAVE5_ENC_LOW_LATENCY);
                }
            }
            regVal = regVal & ~(1UL << 1/*instance->instIndex*/);
            VpuWriteReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST, regVal);
            //APB_WRITE(0x1e8, 0x0);
        }
    }

    //VpuWriteReg(coreIdx, W5_VPU_VINT_REASON, remain_intr);
	APB_WRITE(0x4c, 0x0);

	//Wave5VpuWaitInterrupt : int_reason = 0x100
	//[vdi_get_instance_pool 468]
	//[vdi_unlock 161]
	//Wave5VpuClearInterrupt : flags = 0x100

	reason = VpuReadReg(coreIdx, W5_VPU_VINT_REASON_USR);
	//APB_READ(0x30 0x100

	reason &= ~reason;
	VpuWriteReg(coreIdx, W5_VPU_VINT_REASON_USR, reason);
	//APB_WRITE(0x30 0x0
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuEncGetResult ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//SendQuery
	APB_WRITE(0x104, 0x2);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x110, 0x30001);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x4000);
	APB_WRITE(0x38, 0x1);

	if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
	   return RETCODE_VPU_RESPONSE_TIMEOUT;
	}
	//APB_READ(0x70);// 0x0

	if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
	   return RETCODE_VPU_RESPONSE_TIMEOUT;
	}
	//APB_READ(0x108 0x1

	//W5_RET_QUEUE_STATUS
	APB_READ(0x1e0);// 0x0


	if(APB_READ(0x1dc) != 1)
		return RETCODE_FAILURE;
	//W5_RET_ENC_ENCODING_SUCCESS
	//APB_READ(0x1dc);// 0x1

	//W5_RET_ENC_WARN_INFO
	APB_READ(0x1d4);// 0x0
	//W5_RET_ENC_PIC_NUM
	APB_READ(0x158);// 0x2
	//W5_RET_ENC_PIC_TYPE
	APB_READ(0x124);// 0x1
	//W5_RET_ENC_NUT_0
	APB_READ(0x15c);// 0x1
	//W5_RET_ENC_NUT_1
	APB_READ(0x160);// 0x0
	//W5_RET_ENC_PIC_IDX
	APB_READ(0x12c);// 0x1
	//W5_RET_ENC_SVC_LAYER
	APB_READ(0x170);// 0x0
	//W5_RET_ENC_PIC_SLICE_NUM
	APB_READ(0x130);// 0x10000
	//W5_RET_ENC_PIC_SKIP
	APB_READ(0x134);// 0x0
	//W5_RET_ENC_PIC_NUM_INTRA
	APB_READ(0x138);// 0x2
	//W5_RET_ENC_PIC_NUM_MERGE
	APB_READ(0x13c);// 0x87c0
	//W5_RET_ENC_PIC_NUM_SKIP
	APB_READ(0x144);// 0x8b1c3e80
	//W5_RET_ENC_PIC_AVG_CTU_QP
	APB_READ(0x148);// 0x23
	//W5_RET_ENC_PIC_BYTE
	APB_READ(0x14c);// 0x254c7
	//W5_RET_ENC_GOP_PIC_IDX
	APB_READ(0x150);// 0x0
	//W5_RET_ENC_PIC_POC
	APB_READ(0x128);// 0x1
	//W5_RET_ENC_USED_SRC_IDX
	APB_READ(0x154);// 0x1
	//W5_RET_ENC_SRC_BUF_FLAG
	APB_READ(0x18c);// 0x2

    //W5_RET_ENC_WR_PTR
	APB_READ(0x118);// 0x8afad4c7
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114);// 0x8af88000

	//W5_RET_ENC_RD_PTR
	APB_READ(0x164);// 0xe1430
	//W5_RET_ENC_PIC_DIST_HIGH
	APB_READ(0x168);// 0x8b1c3e81
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114);// 0x8af88000

	//RET_ENC_HOST_CMD_TICK
	APB_READ(0x1b8);// 0x1c7036
	//RET_ENC_PREPARE_START_TICK
	APB_READ(0x1bc);// 0x1c7065
	//RET_ENC_PREPARE_END_TICK
	APB_READ(0x1c0);// 0x1c7104
	//RET_ENC_PROCESSING_START_TICK
	APB_READ(0x1c4);// 0x1c713a
	//RET_ENC_PROCESSING_END_TICK
	APB_READ(0x1c8);// 0x1c7ca4
	//RET_ENC_ENCODING_START_TICK
	APB_READ(0x1cc);// 0x1c7ceb
	//RET_ENC_ENCODING_END_TICK
	APB_READ(0x1d0);// 0x1c88ad
	printf("01     2     1     1    8af88000  8afad4c7    254c7      1      1 0002    0   1603328    40704   748032   770560      0 0\n");
	//01     2     1     1    8af88000  8afad4c7    254c7      1      1 0002    0   1603328    40704   748032   770560      0 0
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}

int APB_DIRECT_CONTROL_416_HEVC_ENC(void)
{
	int coreIdx = 0;
	Uint32          i, regVal;
	Uint32 __VPU_BUSY_TIMEOUT = VPU_BUSY_CHECK_TIMEOUT;
    Uint32  val;
    Int32  reason = -1;

	//416 Encoder codec mode !

    vdi_info_t *vdi = &s_vdi_info[coreIdx];
    if(vdi)
    {
    	vdi->vpu_fd = 0x01;
    	vdi->product_code = WAVE521C_CODE;
    }

	//[vdi_init 224]///////////////////////////////////////////////////////////////////////////////////////////////
    printf("\nvdi_init\n");
    printf("APB_READ(0x1044) %x\n",APB_READ(0x1044));
	if(APB_READ(0x1044) != 0x521c) //0x521c
	{
		VLOG(ERR, "Failed to read APB_READ(0x1044) != 0x521c\n");
		return 0;
	}
	APB_WRITE(0x4, 0x0);

	//APB_WRITE(0x100, 0x0); ~ APB_WRITE(0x1fc, 0x0);
	for(i=0x100; i<0x200; i+=0x4)
	{
		APB_WRITE(i, 0x0);
	}
	if(APB_READ(0x1044) != 0x521c) //0x521c
	{
		VLOG(ERR, "Failed to read APB_READ(0x1044) != 0x521c\n");
		return 0;
	}
	if(APB_READ(0x1044) != 0x521c) //0x521c
	{
		VLOG(ERR, "Failed to read APB_READ(0x1044) != 0x521c\n");
		return 0;
	}
	if(APB_READ(0x1044) != 0x521c) //0x521c
	{
		VLOG(ERR, "Failed to read APB_READ(0x1044) != 0x521c\n");
		return 0;
	}

	APB_WRITE(0x4, 0x0);

	//APB_WRITE(0x100, 0x0); ~ APB_WRITE(0x1fc, 0x0);
	for(i=0x100; i<0x200; i+=0x4)
	{
		APB_WRITE(i, 0x0);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WaitXms(1);
	//Wave5VpuReset////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Wave5VpuReset\n");
    // VPU doesn't send response. Force to set BUSY flag to 0.
	APB_WRITE(0x70, 0x0);
	APB_READ(0x98);	// 0x800d0908
	APB_READ(0x9c);	// 0x8018003

	// Step1 : disable request
	vdi_fio_write_register(coreIdx, W5_BACKBONE_GDI_BUS_CTRL, 0x4);

	// Step2 : Waiting for completion of bus transaction
	//APB_WRITE(0x24, 0x4);
	//APB_WRITE(0x20, 0x1fe10);
	//APB_READ(0x20); // 0x80000000


	if (vdi_wait_bus_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_BACKBONE_GDI_BUS_STATUS) == -1) {
		vdi_fio_write_register(coreIdx, W5_BACKBONE_GDI_BUS_CTRL, 0x00);
		VLOG(ERR, "VpuReset Error \n");
		return RETCODE_VPU_RESPONSE_TIMEOUT;
	}
	//APB_WRITE(0x20, 0xfe14);
	//APB_READ(0x20); // 0x80000000
	//APB_READ(0x24); // 0x3f

	VpuWriteReg(coreIdx, W5_VPU_RESET_REQ, 0xfffffff);
	//APB_WRITE(0x50, 0xfffffff);

	if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_RESET_STATUS) == -1) {
		VpuWriteReg(coreIdx, W5_VPU_RESET_REQ, 0);
		//vdi_log(coreIdx, W5_RESET_VPU, 2);
		return RETCODE_VPU_RESPONSE_TIMEOUT;
	}
	//APB_READ(0x54); // 0x0

	VpuWriteReg(coreIdx, W5_VPU_RESET_REQ, 0);
	//APB_WRITE(0x50, 0x0);

    // Step3 : must clear GDI_BUS_CTRL after done SW_RESET
    vdi_fio_write_register(coreIdx, W5_BACKBONE_GDI_BUS_CTRL, 0x00);
	//APB_WRITE(0x24, 0x0);
	//APB_WRITE(0x20, 0x1fe10);
	//APB_READ(0x20);	// 0x80000000
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    WaitXms(1);
	//Wave5VpuInit ///////////////////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuInit\n");
	//firmware dsp copy
	//osal_memcpy(0x8ab88000, (char *)bit_code_h, HEVC_CODE_LITTLE_LEN*2);
	//BDmaMemCpy_Codec(CPU_ID, 0x8ab88000, bit_code_h, HEVC_CODE_LITTLE_LEN*2);
    BDmaMemSet_isr(CPU_ID, 0x8ab88000, 0x00, 1048576);
    BDmaMemCpy_isr(CPU_ID, 0x8ab88000, bit_code_h, HEVC_CODE_LITTLE_LEN*2);
	if(APB_READ(0x1044) != 0x521c) //0x521c
	{
		VLOG(ERR, "Failed to read APB_READ(0x1044) != 0x521c\n");
		return 0;
	}
	if(APB_READ(0x1044) != 0x521c) //0x521c
	{
		VLOG(ERR, "Failed to read APB_READ(0x1044) != 0x521c\n");
		return 0;
	}
	APB_WRITE(0x0, 0x0);
	//APB_WRITE(0x100, 0x0);~APB_WRITE(0x1fc 0x0
	for(i=0x100; i<0x200; i+=0x4)
	{
		APB_WRITE(i, 0x0);
	}

	APB_WRITE(0x60, 0x80000900);
	APB_WRITE(0x64, 0x0);
	APB_WRITE(0x68, 0x8ab88000);
	APB_WRITE(0x110, 0x8ab88000);
	APB_WRITE(0x114, 0x100000);
	APB_WRITE(0x118, 0x0);
	APB_WRITE(0x11c, 0x8ac88000);
	APB_WRITE(0x120, 0x100000);
	APB_WRITE(0x130, 0xffff);
	APB_WRITE(0x12c, 0x0);
	APB_WRITE(0x48, 0xa340);

	regVal = APB_READ(0x98); // 0x800d0908
	if (((regVal>>16)&1) == 1) {
		regVal = ((WAVE5_PROC_AXI_ID << 28) |
				  (WAVE5_PRP_AXI_ID << 24) |
				  (WAVE5_FBD_Y_AXI_ID << 20) |
				  (WAVE5_FBC_Y_AXI_ID << 16) |
				  (WAVE5_FBD_C_AXI_ID << 12) |
				  (WAVE5_FBC_C_AXI_ID << 8) |
				  (WAVE5_PRI_AXI_ID << 4) |
				  (WAVE5_SEC_AXI_ID << 0));
		vdi_fio_write_register(coreIdx, W5_BACKBONE_PROG_AXI_ID, regVal);
		//APB_WRITE(0x24, 0x0);
		//APB_WRITE(0x20, 0x1fe0c);
		//APB_READ(0x20);//0x80000000
	}

	APB_WRITE(0x124, 0x0);
	APB_WRITE(0x128, 0x23000);

	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x1);
	APB_WRITE(0x6c, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        VLOG(ERR, "VPU init(W5_VPU_REMAP_CORE_START) timeout\n");
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70);  //0x1
    //APB_READ(0x70);  // 0x0

	regVal = VpuReadReg(coreIdx, W5_RET_SUCCESS);
    if (regVal == 0) {
        Uint32      reasonCode = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);
        VLOG(ERR, "VPU init(W5_RET_SUCCESS) failed(%d) REASON CODE(%08x)\n", regVal, reasonCode);
        return RETCODE_FAILURE;
    }
	//APB_READ(0x108); // 0x1
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    WaitXms(1);
	//SetupWave5Properties ///////////////////////////////////////////////////////////////////////////////////////
    printf("SetupWave5Properties\n");
    VpuWriteReg(coreIdx, W5_QUERY_OPTION, GET_VPU_INFO);
    VpuWriteReg(coreIdx, W5_VPU_BUSY_STATUS, 1);
    VpuWriteReg(coreIdx, W5_COMMAND, W5_QUERY);
    VpuWriteReg(coreIdx, W5_VPU_HOST_INT_REQ, 1);
    //APB_WRITE(0x104 0x0
	//APB_WRITE(0x70 0x1
	//APB_WRITE(0x100 0x4000
	//APB_WRITE(0x38 0x1
    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70 0x0

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {
    	//APB_READ(0x108 0x1
    	return RETCODE_QUERY_FAILURE;
    }
    else {
    	VpuReadReg(coreIdx, W5_RET_PRODUCT_NAME);
    	VpuReadReg(coreIdx, W5_PRODUCT_NUMBER);
    	//APB_READ(0x11c 0x57415645
    	//APB_READ(0x1044 0x521c

        VpuReadReg(coreIdx, W5_RET_PRODUCT_VERSION);
        VpuReadReg(coreIdx, W5_RET_FW_VERSION);
        VpuReadReg(coreIdx, W5_RET_CUSTOMER_ID);
        VpuReadReg(coreIdx, W5_RET_STD_DEF0);
        VpuReadReg(coreIdx, W5_RET_STD_DEF1);
        VpuReadReg(coreIdx, W5_RET_CONF_FEATURE);
        VpuReadReg(coreIdx, W5_RET_CONF_DATE);
        VpuReadReg(coreIdx, W5_RET_CONF_REVISION);
        VpuReadReg(coreIdx, W5_RET_CONF_TYPE);
    	//APB_READ(0x120 0x521c
    	//APB_READ(0x118 0x2e4a7
    	//APB_READ(0x140 0x0
    	//APB_READ(0x124 0x800d0908
    	//APB_READ(0x128 0x8018003
    	//APB_READ(0x12c 0x507
    	//APB_READ(0x130 0x1341660
    	//APB_READ(0x134 0x2b518
    	//APB_READ(0x138 0x0

    }
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    WaitXms(1);
	//Wave5VpuBuildUpEncParam /////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuBuildUpEncParam\n");
	APB_WRITE(0x114, 0x8af88000);
	APB_WRITE(0x118, 0x20000);

	APB_READ(0x1044);// 0x521c
	APB_WRITE(0x124, 0xf);
	APB_WRITE(0x13c, 0x0);
	APB_WRITE(0x128, 0x0);

	VpuWriteReg(coreIdx, W5_VPU_BUSY_STATUS, 1);
	VpuWriteReg(coreIdx, W5_RET_SUCCESS, 0);
	//APB_WRITE(0x70 0x1
	//APB_WRITE(0x108 0x0
	VpuWriteReg(coreIdx, W5_CMD_ENC_VCORE_INFO, 1);
	//APB_WRITE(0x194 0x1

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x8);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {   // Check QUEUE_DONE
       return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70 0x0

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {  // FAILED for adding into VCPU QUEUE
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);
        if (regVal != WAVE5_QUEUEING_FAIL)
            VLOG(ERR, "FAIL_REASON = 0x%x\n", regVal);
        if (regVal == 2)
        	return RETCODE_INVALID_SFS_INSTANCE;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
        	return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
        	return RETCODE_ERROR_FW_FATAL;
        else
        	return RETCODE_FAILURE;
    }
	//APB_READ(0x108 0x1
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    WaitXms(1);

	//Wave5VpuEncInitSeq //////////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuEncInitSeq\n");
	if(APB_READ(0x1044) != 0x521c) //0x521c
	{
		VLOG(ERR, "Failed to read APB_READ(0x1044) != 0x521c\n");
		return 0;
	}
	APB_WRITE(0x104, 0x1);
	APB_WRITE(0x11c, 0x1);
	APB_WRITE(0x120, 0x1045);
	APB_WRITE(0x124, 0x0);
	APB_WRITE(0x128, 0x0);
	APB_WRITE(0x12c, 0x0);
	APB_WRITE(0x130, 0x0);
	APB_WRITE(0x134, 0x0);
	APB_WRITE(0x138, 0x0);
	APB_WRITE(0x13c, 0x0);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x200);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70 0x0

	APB_WRITE(0x104, 0x0);
	APB_WRITE(0x11c, 0xf001a0);
	APB_WRITE(0x120, 0x0);
	APB_WRITE(0x124, 0x8020001);
	APB_WRITE(0x128, 0x60000000);
	APB_WRITE(0x12c, 0x0);
	APB_WRITE(0x130, 0x50001);
	APB_WRITE(0x13c, 0xd);
	APB_WRITE(0x148, 0x0);
	APB_WRITE(0x150, 0x1e);
	APB_WRITE(0x154, 0x1e8480);
	APB_WRITE(0x16c, 0x0);
	APB_WRITE(0x158, 0xbb8fc025);
	APB_WRITE(0x1c4, 0x8002);
	APB_WRITE(0x15c, 0xacc8);
	APB_WRITE(0x168, 0xcc8cc8);
	APB_WRITE(0x160, 0x1010101);
	APB_WRITE(0x164, 0x1010101);
	APB_WRITE(0x170, 0x0);
	APB_WRITE(0x1a8, 0x3800410);
	APB_WRITE(0x1ac, 0x0);
	APB_WRITE(0x134, 0x0);
	APB_WRITE(0x138, 0x0);
	APB_WRITE(0x140, 0x0);
	APB_WRITE(0x1b0, 0x0);
	APB_WRITE(0x174, 0x3e8);
	APB_WRITE(0x178, 0x7530);
	APB_WRITE(0x17c, 0x0);
	APB_WRITE(0x184, 0x0);
	APB_WRITE(0x188, 0x0);
	APB_WRITE(0x18c, 0x0);
	APB_WRITE(0x190, 0x0);
	APB_WRITE(0x194, 0x0);
	APB_WRITE(0x198, 0x0);
	APB_WRITE(0x19c, 0x0);
	APB_WRITE(0x144, 0x0);
	APB_WRITE(0x1a0, 0x0);
	APB_WRITE(0x1a4, 0x0);

	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x200);

	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70); //0x0

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == 0) {
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);
        if (regVal != WAVE5_QUEUEING_FAIL)
            VLOG(ERR, "FAIL_REASON = 0x%x\n", regVal);

        if ( regVal == WAVE5_QUEUEING_FAIL)
            return RETCODE_QUEUEING_FAILURE;
        else if (regVal == WAVE5_SYSERR_ACCESS_VIOLATION_HW)
            return RETCODE_MEMORY_ACCESS_VIOLATION;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
            return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
            return RETCODE_ERROR_FW_FATAL;
        else
            return RETCODE_FAILURE;
    }
	//APB_READ(0x108); //0x1
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    WaitXms(1);
	//HandlingInterruptFlag ///////////////////////////////////////////////////////////////////////////////////
    printf("HandlingInterruptFlag\n");
    while(1)
    {
		if(APB_READ(0x44) == 1) // 0x1
		{
			if((reason = APB_READ(0x4c)) == 0x200) // 0x200
			{
				APB_WRITE(0x34, 0x200);
				APB_WRITE(0x3c, 0x1);
				break;
			}
		}
    }

    if (reason & (1 << INT_WAVE5_ENC_SET_PARAM)) {
        regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
        //APB_READ(0x1e8); // 0x1
        regVal = (regVal & 0xffff);
        if (regVal & (1 << 0/*instIndex*/)) {
            //ownInt = 1;
            //reason = (1 << INT_WAVE5_ENC_SET_PARAM);
            //remain_intr &= ~(Uint32)reason;

            //regVal = regVal & ~(1UL << instIndex);
            //VpuWriteReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST, regVal);
            APB_WRITE(0x1e8, 0x0);
        }
    }

    //VpuWriteReg(coreIdx, W5_VPU_VINT_REASON, remain_intr);
	APB_WRITE(0x4c, 0x0);
	//Wave5VpuWaitInterrupt : int_reason = 0x200
	//[vdi_get_instance_pool 468]
	//[vdi_unlock 161]
	//Wave5VpuClearInterrupt : flags = 0x200

	reason = VpuReadReg(coreIdx, W5_VPU_VINT_REASON_USR);
	//APB_READ(0x30); // 0x200
	reason &= ~reason;
    VpuWriteReg(coreIdx, W5_VPU_VINT_REASON_USR, reason);
	//APB_WRITE(0x30, 0x0);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuEncGetSeqInfo ////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuEncGetSeqInfo\n");
    //SendQuery
	APB_WRITE(0x104, 0x2);
	APB_WRITE(0x70, 0x1);
	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x4000);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
    	//APB_READ(0x70 0x0
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE)
        return RETCODE_FAILURE;
	//APB_READ(0x108 0x1

	APB_READ(0x1e0);// 0x0

    if (VpuReadReg(coreIdx, W5_RET_ENC_ENCODING_SUCCESS) != 1) {
        VpuReadReg(coreIdx, W5_RET_ENC_ERR_INFO);
        return RETCODE_FAILURE;
    }
	//APB_READ(0x1dc);// 0x1

    VpuReadReg(coreIdx, W5_RET_ENC_WARN_INFO);
	//APB_READ(0x1d4);// 0x0

	APB_READ(0x1fc);// 0x0
	APB_READ(0x11c);// 0x2
	APB_READ(0x120);// 0x1
	APB_READ(0x16c);// 0x0
	APB_READ(0x1b0);// 0x134380
	APB_READ(0x1b4);// 0x4540
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	WaitXms(1);
	//Wave5VpuEncRegisterFramebuffer //////////////////////////////////////////////////////////////////////
	printf("Wave5VpuEncRegisterFramebuffer\n");
	//W5_CMD_SET_FB_ADDR_TASK_BUF
	APB_WRITE(0x1d4, 0x8b05c000);
	//W5_CMD_SET_FB_TASK_BUF_SIZE
	APB_WRITE(0x1d8, 0x3a0fc0);

	//W5_ADDR_SUB_SAMPLED_FB_BASE
	APB_WRITE(0x12c, 0x8b054000);
	//W5_SUB_SAMPLED_ONE_FB_SIZE
	APB_WRITE(0x130, 0x1c00);

	//W5_PIC_SIZE
	APB_WRITE(0x11c, 0x1a000f0);
	//W5_FBC_STRIDE
	APB_WRITE(0x128, 0x6800340);
	//W5_COMMON_PIC_INFO
	APB_WRITE(0x118, 0x101a0);
	APB_WRITE(0x104, 0x18);
	APB_WRITE(0x120, 0x1);
	APB_WRITE(0x134, 0x8aff8000);
	APB_WRITE(0x138, 0x8b010600);
	APB_WRITE(0x13c, 0x8b04c000);
	APB_WRITE(0x140, 0x8b050000);
	APB_WRITE(0x1b4, 0x8b048000);
	APB_WRITE(0x144, 0x8b020000);
	APB_WRITE(0x148, 0x8b038600);
	APB_WRITE(0x14c, 0x8b04cf00);
	APB_WRITE(0x150, 0x8b050780);
	APB_WRITE(0x1b8, 0x8b048e00);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x80);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70);// 0x0

    regVal = VpuReadReg(coreIdx, W5_RET_SUCCESS);
    if (regVal == 0) {
        return RETCODE_FAILURE;
    }
	//APB_READ(0x108);// 0x1
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    WaitXms(1);
    osal_memset(0x8ad88000, 0x00, 1*1024*1024);
    hwflush_dcache_range(0x8ad88000, 1*1024*1024);
	//Wave5VpuEncGetHeader //////////////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuEncGetHeader\n");
    //W5_CMD_ENC_BS_START_ADDR
	APB_WRITE(0x118, 0x8ad88000);
	//W5_CMD_ENC_BS_SIZE
	APB_WRITE(0x11c, 0x200000);
	//W5_CMD_ENC_PIC_USE_SEC_AXI
	APB_WRITE(0x124, 0x0);
	//W5_CMD_ENC_PIC_CODE_OPTION
	APB_WRITE(0x164, 0x1c);
	//W5_CMD_ENC_PIC_SRC_PIC_IDX
	APB_WRITE(0x144, 0x0);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x100);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {   // Check QUEUE_DONE
       return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70); //0x0
    //W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x10000

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {           // FAILED for adding a command into VCPU QUEUE
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);

        if ( regVal == WAVE5_QUEUEING_FAIL)
            return RETCODE_QUEUEING_FAILURE;
        else if (regVal == WAVE5_SYSERR_ACCESS_VIOLATION_HW)
            return RETCODE_MEMORY_ACCESS_VIOLATION;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
            return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
            return RETCODE_ERROR_FW_FATAL;
        else
            return RETCODE_FAILURE;
    }
	//APB_READ(0x108); // 0x1
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    WaitXms(1);
	//HandlingInterruptFlag //////////////////////////////////////////////////////////////////////////////////////
    printf("HandlingInterruptFlag\n");
    while(1)
    {
		if(APB_READ(0x44) == 1) // 0x1
		{
			if((reason = APB_READ(0x4c)) == 0x100) // 0x200
			{
				APB_WRITE(0x34, 0x100);
				APB_WRITE(0x3c, 0x1);
				break;
			}
		}
    }

    if (reason & (1 << INT_WAVE5_DEC_PIC)) {
        regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
    	//APB_READ(0x1e8 0x1

        regVal = (regVal & 0xffff);
        if (regVal & (1 << 0/*instance->instIndex*/)) {
            //ownInt = 1;
            int orgReason = reason;
            reason = (1 << INT_WAVE5_DEC_PIC);
            //remain_intr &= ~(Uint32)reason;
            /* Clear Low Latency Interrupt if two interrupts are occured */
            if (orgReason & (1 << INT_WAVE5_ENC_LOW_LATENCY)) {
                regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
                regVal = (regVal>>16);
                if (regVal & (1 << 0/*instance->instIndex*/)) {
                    //remain_intr &= ~(1<<INT_WAVE5_ENC_LOW_LATENCY);
                    Wave5VpuClearInterrupt(coreIdx, 1<<INT_WAVE5_ENC_LOW_LATENCY);
                }
            }
            regVal = regVal & ~(1UL << 0/*instance->instIndex*/);
            VpuWriteReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST, regVal);
            //APB_WRITE(0x1e8, 0x0);
        }
    }

    //VpuWriteReg(coreIdx, W5_VPU_VINT_REASON, remain_intr);
	APB_WRITE(0x4c, 0x0);

	//Wave5VpuWaitInterrupt : int_reason = 0x100
	//[vdi_get_instance_pool 468]
	//[vdi_unlock 161]
	//Wave5VpuClearInterrupt : flags = 0x100

	reason = VpuReadReg(coreIdx, W5_VPU_VINT_REASON_USR);
	//APB_READ(0x30); // 0x100

	reason &= ~reason;
    VpuWriteReg(coreIdx, W5_VPU_VINT_REASON_USR, reason);
	//APB_WRITE(0x30, 0x0);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    WaitXms(1);
	//Wave5VpuEncGetResult //////////////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuEncGetResult\n");
    //SendQuery
	APB_WRITE(0x104, 0x2);
	APB_WRITE(0x70, 0x1);
	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x4000);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70); // 0x0

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE)
        return RETCODE_FAILURE;
	//APB_READ(0x108); // 0x1

    //W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x0


	if(APB_READ(0x1dc) != 1)
		return RETCODE_FAILURE;
	//W5_RET_ENC_ENCODING_SUCCESS
	//APB_READ(0x1dc); // 0x1

	//W5_RET_ENC_WARN_INFO
	APB_READ(0x1d4); // 0x0
	//W5_RET_ENC_PIC_NUM
	APB_READ(0x158); // 0x0
	//W5_RET_ENC_PIC_TYPE
	APB_READ(0x124); // 0x0
	//W5_RET_ENC_NUT_0
	APB_READ(0x15c); // 0x0
	//W5_RET_ENC_NUT_1
	APB_READ(0x160); // 0x0
	//W5_RET_ENC_PIC_IDX
	APB_READ(0x12c); // 0xfffffffd
	//W5_RET_ENC_SVC_LAYER
	APB_READ(0x170); // 0x0
	//W5_RET_ENC_PIC_SLICE_NUM
	APB_READ(0x130); // 0x0
	//W5_RET_ENC_PIC_SKIP
	APB_READ(0x134); // 0x0
	//W5_RET_ENC_PIC_NUM_INTRA
	APB_READ(0x138); // 0x0
	//W5_RET_ENC_PIC_NUM_MERGE
	APB_READ(0x13c); // 0x0
	//W5_RET_ENC_PIC_NUM_SKIP
	APB_READ(0x144); // 0x0
	//W5_RET_ENC_PIC_AVG_CTU_QP
	APB_READ(0x148); // 0x0
	//W5_RET_ENC_PIC_BYTE
	APB_READ(0x14c); // 0x53
	//W5_RET_ENC_GOP_PIC_IDX
	APB_READ(0x150); // 0xffffffff
	//W5_RET_ENC_PIC_POC
	APB_READ(0x128); // 0x0
	//W5_RET_ENC_USED_SRC_IDX
	APB_READ(0x154); // 0xfffffffd
	//W5_RET_ENC_SRC_BUF_FLAG
	APB_READ(0x18c); // 0x0

    //W5_RET_ENC_WR_PTR
	APB_READ(0x118); // 0x8ad88053
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); // 0x8ad88000

	//W5_RET_ENC_PIC_DIST_LOW
	APB_READ(0x164); // 0x0
	//W5_RET_ENC_PIC_DIST_HIGH
	APB_READ(0x168); // 0x0
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); // 0x8ad88000

	//RET_ENC_HOST_CMD_TICK
	APB_READ(0x1b8); // 0x2d66d
	//RET_ENC_PREPARE_START_TICK
	APB_READ(0x1bc); // 0x2d691
	//RET_ENC_PREPARE_END_TICK
	APB_READ(0x1c0); // 0x2d72f
	//RET_ENC_PROCESSING_START_TICK
	APB_READ(0x1c4); // 0x2d759
	//RET_ENC_PROCESSING_END_TICK
	APB_READ(0x1c8); // 0x2d75c
	//RET_ENC_ENCODING_START_TICK
	APB_READ(0x1cc); // 0x2d770
	//RET_ENC_ENCODING_END_TICK
	APB_READ(0x1d0); // 0x2d786
	printf("00     0     0    -3    8ad88000  8ad88053       53      0     -3 0000    0     71936    40448      768     5632      0 0\n");
	//00     0     0    -3    8ad88000  8ad88053       53      0     -3 0000    0     71936    40448      768     5632      0 0
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	WaitXms(1);
	printf("make_raw_nv12\n");
	BYTE *addY, *addU, *addV;
	addY = 0x8afa8000;
	addU = 0x8afc0600;
	addV = 0xffffffff;
	make_raw_nv12(416, 240, addY, addU, addV);

	osal_memset(0x8ad88000+0x53, 0x00, 1*1024*1024);
    hwflush_dcache_range(0x8ad88000+0x53, 1*1024*1024);
	//Wave5VpuEncode //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Wave5VpuEncode\n");
	//W5_CMD_ENC_BS_START_ADDR
	APB_WRITE(0x118, 0x8ad88000+0x53);
	//W5_CMD_ENC_BS_SIZE
	APB_WRITE(0x11c, 0x200000);
	//W5_CMD_ENC_PIC_SRC_AXI_SEL
	APB_WRITE(0x160, 0x0);
	//W5_CMD_ENC_PIC_USE_SEC_AXI
	APB_WRITE(0x124, 0x0);
	//W5_CMD_ENC_PIC_REPORT_ENDIAN
	APB_WRITE(0x12c, 0x10);

	//W5_CMD_ENC_PIC_CODE_OPTION
	APB_WRITE(0x164, 0x3);
	//W5_CMD_ENC_PIC_PIC_PARAM
	APB_WRITE(0x168, 0x0);

	//W5_CMD_ENC_PIC_SRC_PIC_IDX
	APB_WRITE(0x144, 0x0);
	//W5_CMD_ENC_PIC_SRC_ADDR_Y
	APB_WRITE(0x148, 0x8afa8000);
	//W5_CMD_ENC_PIC_SRC_ADDR_U
	APB_WRITE(0x14c, 0x8afc0600);
	//W5_CMD_ENC_PIC_SRC_ADDR_V
	APB_WRITE(0x150, 0xffffffff);

	//W5_CMD_ENC_PIC_SRC_STRIDE
	APB_WRITE(0x154, 0x1a001a0);

	//W5_CMD_ENC_PIC_SRC_FORMAT
	APB_WRITE(0x158, 0x3e2);
	//W5_CMD_ENC_PIC_CUSTOM_MAP_OPTION_ADDR
	APB_WRITE(0x13c, 0x0);
	//W5_CMD_ENC_PIC_CUSTOM_MAP_OPTION_PARAM
	APB_WRITE(0x138, 0x0);
	//W5_CMD_ENC_PIC_LONGTERM_PIC
	APB_WRITE(0x16c, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_SIGMA_Y
	APB_WRITE(0x170, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_SIGMA_C
	APB_WRITE(0x174, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_MEAN_Y
	APB_WRITE(0x178, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_MEAN_C
	APB_WRITE(0x17c, 0x0);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x100);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {   // Check QUEUE_DONE
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70); // 0x0

    //W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x10000

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {           // FAILED for adding a command into VCPU QUEUE
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);
        if (regVal != WAVE5_QUEUEING_FAIL)
            VLOG(ERR, "FAIL_REASON = 0x%x\n", regVal);

        if (regVal == WAVE5_QUEUEING_FAIL)
            return RETCODE_QUEUEING_FAILURE;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
            return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
            return RETCODE_ERROR_FW_FATAL;
        else
            return RETCODE_FAILURE;
    }
	//APB_READ(0x108); // 0x1
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    WaitXms(1);

	//HandlingInterruptFlag //////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("HandlingInterruptFlag\n");
    while(1)
    {
		if(APB_READ(0x44) == 1) // 0x1
		{
			if((reason = APB_READ(0x4c)) == 0x100) // 0x200
			{
				APB_WRITE(0x34, 0x100);
				APB_WRITE(0x3c, 0x1);
				break;
			}
		}
    }

    if (reason & (1 << INT_WAVE5_DEC_PIC)) {
        regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
    	//APB_READ(0x1e8 0x1

        regVal = (regVal & 0xffff);
        if (regVal & (1 << 0/*instance->instIndex*/)) {
            //ownInt = 1;
            int orgReason = reason;
            reason = (1 << INT_WAVE5_DEC_PIC);
            //remain_intr &= ~(Uint32)reason;
            /* Clear Low Latency Interrupt if two interrupts are occured */
            if (orgReason & (1 << INT_WAVE5_ENC_LOW_LATENCY)) {
                regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
                regVal = (regVal>>16);
                if (regVal & (1 << 0/*instance->instIndex*/)) {
                    //remain_intr &= ~(1<<INT_WAVE5_ENC_LOW_LATENCY);
                    Wave5VpuClearInterrupt(coreIdx, 1<<INT_WAVE5_ENC_LOW_LATENCY);
                }
            }
            regVal = regVal & ~(1UL << 0/*instance->instIndex*/);
            VpuWriteReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST, regVal);
            //APB_WRITE(0x1e8, 0x0);
        }
    }

    //VpuWriteReg(coreIdx, W5_VPU_VINT_REASON, remain_intr);
	APB_WRITE(0x4c, 0x0);

	//Wave5VpuWaitInterrupt : int_reason = 0x100
	//[vdi_get_instance_pool 468]
	//[vdi_unlock 161]
	//Wave5VpuClearInterrupt : flags = 0x100

	reason = VpuReadReg(coreIdx, W5_VPU_VINT_REASON_USR);
	//APB_READ(0x30); // 0x100

	reason &= ~reason;
	VpuWriteReg(coreIdx, W5_VPU_VINT_REASON_USR, reason);
	//APB_WRITE(0x30, 0x0);


	//Wave5VpuEncGetResult /////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Wave5VpuEncGetResult\n");
	//SendQuery
	APB_WRITE(0x104, 0x2);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x4000);
	APB_WRITE(0x38, 0x1);

	if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
	   return RETCODE_VPU_RESPONSE_TIMEOUT;
	}
	//APB_READ(0x70); // 0x0

	if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE)
	    return RETCODE_FAILURE;
	//APB_READ(0x108); // 0x1

	//W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x0
	//W5_RET_ENC_ENCODING_SUCCESS

	if(APB_READ(0x1dc) != 1)
		return RETCODE_FAILURE;
	//APB_READ(0x1dc); // 0x1

	//W5_RET_ENC_WARN_INFO
	APB_READ(0x1d4); // 0x0
	//W5_RET_ENC_PIC_NUM
	APB_READ(0x158); // 0x1
	//W5_RET_ENC_PIC_TYPE
	APB_READ(0x124); // 0x0
	//W5_RET_ENC_NUT_0
	APB_READ(0x15c); // 0x13
	//W5_RET_ENC_NUT_1
	APB_READ(0x160); // 0x0
	//W5_RET_ENC_PIC_IDX
	APB_READ(0x12c); // 0x0
	//W5_RET_ENC_SVC_LAYER
	APB_READ(0x170); // 0x0
	//W5_RET_ENC_PIC_SLICE_NUM
	APB_READ(0x130); // 0x55980
	//W5_RET_ENC_PIC_SKIP
	APB_READ(0x134); // 0x0
	//W5_RET_ENC_PIC_NUM_INTRA
	APB_READ(0x138); // 0x618
	//W5_RET_ENC_PIC_NUM_MERGE
	APB_READ(0x13c); // 0x0
	//W5_RET_ENC_PIC_NUM_SKIP
	APB_READ(0x144); // 0x0
	//W5_RET_ENC_PIC_AVG_CTU_QP
	APB_READ(0x148); // 0x17
	//W5_RET_ENC_PIC_BYTE
	APB_READ(0x14c); // 0x8dc8
	//W5_RET_ENC_GOP_PIC_IDX
	APB_READ(0x150); // 0x0
	//W5_RET_ENC_PIC_POC
	APB_READ(0x128); // 0x0
	//W5_RET_ENC_USED_SRC_IDX
	APB_READ(0x154); // 0x0
	//W5_RET_ENC_SRC_BUF_FLAG
	APB_READ(0x18c); // 0x1

    //W5_RET_ENC_WR_PTR
	APB_READ(0x118); // 0x8ad90dc8
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); // 0x8ad88000

	//W5_RET_ENC_PIC_DIST_LOW
	APB_READ(0x164); // 0x8004e9a7
	//W5_RET_ENC_PIC_DIST_HIGH
	APB_READ(0x168); // 0x8afa3e7e
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); // 0x8ad88000

	//RET_ENC_HOST_CMD_TICK
	APB_READ(0x1b8); // 0x3d0a6
	//RET_ENC_PREPARE_START_TICK
	APB_READ(0x1bc); // 0x3d1be
	//RET_ENC_PREPARE_END_TICK
	APB_READ(0x1c0); // 0x3d1be
	//RET_ENC_PROCESSING_START_TICK
	APB_READ(0x1c4); // 0x3d1f7
	//RET_ENC_PROCESSING_END_TICK
	APB_READ(0x1c8); // 0x3d3f8
	//RET_ENC_ENCODING_START_TICK
	APB_READ(0x1cc); // 0x3d442
	//RET_ENC_ENCODING_END_TICK
	APB_READ(0x1d0); // 0x3d648

	printf("00     1     0     0    8ad88000  8ad90dc8     8dc8      0      0 0001    0    369152    60416   131328   132608      0 0\n");
	//00     1     0     0    8ad88000  8ad90dc8     8dc8      0      0 0001    0    369152    60416   131328   132608      0 0
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



}

#if 0
int APB_DIRECT_CONTROL_416_HEVC_DEC(void)
{
	int coreIdx = 0;
	Uint32          i, regVal;
	Uint32 __VPU_BUSY_TIMEOUT = VPU_BUSY_CHECK_TIMEOUT;
    Uint32  val;
    Int32  reason = -1;

	//416 Encoder codec mode !

    vdi_info_t *vdi = &s_vdi_info[coreIdx];
    if(vdi)
    {
    	vdi->vpu_fd = 0x01;
    	vdi->product_code = WAVE521C_CODE;
    }

	//[vdi_init 224]///////////////////////////////////////////////////////////////////////////////////////////////
    printf("\nvdi_init\n");
    printf("APB_READ(0x1044) %x\n",APB_READ(0x1044));
	if(APB_READ(0x1044) != 0x521c) //0x521c
	{
		VLOG(ERR, "Failed to read APB_READ(0x1044) != 0x521c\n");
		return 0;
	}

	//APB_WRITE(0x100, 0x0); ~ APB_WRITE(0x1fc, 0x0);
	for(i=0x100; i<0x200; i+=0x4)
	{
		APB_WRITE(i, 0x0);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuReset////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Wave5VpuReset\n");
    // VPU doesn't send response. Force to set BUSY flag to 0.
	APB_WRITE(0x70, 0x0);
	APB_READ(0x98);	// 0x800d0908
	APB_READ(0x9c);	// 0x8018003

	// Step1 : disable request
	vdi_fio_write_register(coreIdx, W5_BACKBONE_GDI_BUS_CTRL, 0x4);

	// Step2 : Waiting for completion of bus transaction
	//APB_WRITE(0x24, 0x4);
	//APB_WRITE(0x20, 0x1fe10);
	//APB_READ(0x20); // 0x80000000


	if (vdi_wait_bus_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_BACKBONE_GDI_BUS_STATUS) == -1) {
		vdi_fio_write_register(coreIdx, W5_BACKBONE_GDI_BUS_CTRL, 0x00);
		VLOG(ERR, "VpuReset Error \n");
		return RETCODE_VPU_RESPONSE_TIMEOUT;
	}
	//APB_WRITE(0x20, 0xfe14);
	//APB_READ(0x20); // 0x80000000
	//APB_READ(0x24); // 0x3f

	VpuWriteReg(coreIdx, W5_VPU_RESET_REQ, 0xfffffff);
	//APB_WRITE(0x50, 0xfffffff);

	if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_RESET_STATUS) == -1) {
		VpuWriteReg(coreIdx, W5_VPU_RESET_REQ, 0);
		//vdi_log(coreIdx, W5_RESET_VPU, 2);
		return RETCODE_VPU_RESPONSE_TIMEOUT;
	}
	//APB_READ(0x54); // 0x0

	VpuWriteReg(coreIdx, W5_VPU_RESET_REQ, 0);
	//APB_WRITE(0x50, 0x0);

    // Step3 : must clear GDI_BUS_CTRL after done SW_RESET
    vdi_fio_write_register(coreIdx, W5_BACKBONE_GDI_BUS_CTRL, 0x00);
	//APB_WRITE(0x24, 0x0);
	//APB_WRITE(0x20, 0x1fe10);
	//APB_READ(0x20);	// 0x80000000
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//Wave5VpuInit ///////////////////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuInit\n");
	//firmware dsp copy
	//osal_memcpy(0x8ab88000, (char *)bit_code_h, HEVC_CODE_LITTLE_LEN*2);
    BDmaMemCpy_isr(CPU_ID, 0x8ab88000, bit_code_h, HEVC_CODE_LITTLE_LEN*2);
	
	APB_WRITE(0x0, 0x0);
	//APB_WRITE(0x100, 0x0);~APB_WRITE(0x1fc 0x0
	for(i=0x100; i<0x200; i+=0x4)
	{
		APB_WRITE(i, 0x0);
	}

	APB_WRITE(0x60, 0x80000900);
	APB_WRITE(0x64, 0x0);
	APB_WRITE(0x68, 0x8ab88000);
	APB_WRITE(0x110, 0x8ab88000);
	APB_WRITE(0x114, 0x100000);
	APB_WRITE(0x118, 0x0);
	APB_WRITE(0x11c, 0x8ac88000);
	APB_WRITE(0x120, 0x100000);
	APB_WRITE(0x130, 0xffff);
	APB_WRITE(0x12c, 0x0);
	APB_WRITE(0x48, 0xa340);

	regVal = APB_READ(0x98); // 0x800d0908
	if (((regVal>>16)&1) == 1) {
		regVal = ((WAVE5_PROC_AXI_ID << 28) |
				  (WAVE5_PRP_AXI_ID << 24) |
				  (WAVE5_FBD_Y_AXI_ID << 20) |
				  (WAVE5_FBC_Y_AXI_ID << 16) |
				  (WAVE5_FBD_C_AXI_ID << 12) |
				  (WAVE5_FBC_C_AXI_ID << 8) |
				  (WAVE5_PRI_AXI_ID << 4) |
				  (WAVE5_SEC_AXI_ID << 0));
		vdi_fio_write_register(coreIdx, W5_BACKBONE_PROG_AXI_ID, regVal);
		//APB_WRITE(0x24, 0x0);
		//APB_WRITE(0x20, 0x1fe0c);
		//APB_READ(0x20);//0x80000000
	}

	APB_WRITE(0x124, 0x0);
	APB_WRITE(0x128, 0x23000);

	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x1);
	APB_WRITE(0x6c, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        VLOG(ERR, "VPU init(W5_VPU_REMAP_CORE_START) timeout\n");
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70);  //0x1
    //APB_READ(0x70);  // 0x0

	regVal = VpuReadReg(coreIdx, W5_RET_SUCCESS);
    if (regVal == 0) {
        Uint32      reasonCode = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);
        VLOG(ERR, "VPU init(W5_RET_SUCCESS) failed(%d) REASON CODE(%08x)\n", regVal, reasonCode);
        return RETCODE_FAILURE;
    }
	//APB_READ(0x108); // 0x1
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//SetupWave5Properties ///////////////////////////////////////////////////////////////////////////////////////
    printf("SetupWave5Properties\n");
    VpuWriteReg(coreIdx, W5_QUERY_OPTION, GET_VPU_INFO);
    VpuWriteReg(coreIdx, W5_VPU_BUSY_STATUS, 1);
    VpuWriteReg(coreIdx, W5_COMMAND, W5_QUERY);
    VpuWriteReg(coreIdx, W5_VPU_HOST_INT_REQ, 1);
    //APB_WRITE(0x104 0x0
	//APB_WRITE(0x70 0x1
	//APB_WRITE(0x100 0x4000
	//APB_WRITE(0x38 0x1
    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70 0x0

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {
    	//APB_READ(0x108 0x1
    	return RETCODE_QUERY_FAILURE;
    }
    else {
    	VpuReadReg(coreIdx, W5_RET_PRODUCT_NAME);
    	VpuReadReg(coreIdx, W5_PRODUCT_NUMBER);
    	//APB_READ(0x11c 0x57415645
    	//APB_READ(0x1044 0x521c

        VpuReadReg(coreIdx, W5_RET_PRODUCT_VERSION);
        VpuReadReg(coreIdx, W5_RET_FW_VERSION);
        VpuReadReg(coreIdx, W5_RET_CUSTOMER_ID);
        VpuReadReg(coreIdx, W5_RET_STD_DEF0);
        VpuReadReg(coreIdx, W5_RET_STD_DEF1);
        VpuReadReg(coreIdx, W5_RET_CONF_FEATURE);
        VpuReadReg(coreIdx, W5_RET_CONF_DATE);
        VpuReadReg(coreIdx, W5_RET_CONF_REVISION);
        VpuReadReg(coreIdx, W5_RET_CONF_TYPE);
    	//APB_READ(0x120 0x521c
    	//APB_READ(0x118 0x2e4a7
    	//APB_READ(0x140 0x0
    	//APB_READ(0x124 0x800d0908
    	//APB_READ(0x128 0x8018003
    	//APB_READ(0x12c 0x507
    	//APB_READ(0x130 0x1341660
    	//APB_READ(0x134 0x2b518
    	//APB_READ(0x138 0x0

    }
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuBuildUpDecParam /////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuBuildUpDecParam\n");
	
    VpuWriteReg(coreIdx, W5_CMD_DEC_VCORE_INFO, 1);
	//APB WRITE 0x194 0x1

    VpuWriteReg(coreIdx, W5_CMD_NUM_CQ_DEPTH_M1, COMMAND_QUEUE_DEPTH -1 );
	//APB WRITE 0x13c 0x0

    osal_memset(0x8b988000, 0x00, 0x180000);

    VpuWriteReg(coreIdx, W5_ADDR_WORK_BASE, 0x8b988000);
	//APB WRITE 0x114 0x8b988000

	VpuWriteReg(coreIdx, W5_WORK_SIZE,      0x180000);
	//APB WRITE 0x118 0x180000

    VpuWriteReg(coreIdx, W5_CMD_DEC_BS_START_ADDR, 0x8ad88000);
	//APB WRITE 0x11c 0x8ad88000
	
    VpuWriteReg(coreIdx, W5_CMD_DEC_BS_SIZE, 0xc00000);
	//APB WRITE 0x120 0xc00000

    //bsEndian = vdi_convert_endian(coreIdx, streamEndian);
	APB_READ(0x1044);	// 0x521c
	
    /* NOTE: When endian mode is 0, SDMA reads MSB first */
    //bsEndian = (~bsEndian&VDI_128BIT_ENDIAN_MASK);
    //VpuWriteReg(coreIdx, W5_CMD_BS_PARAM, bsEndian);
	APB_WRITE(0x124); // 0xf

    VpuWriteReg(coreIdx, W5_VPU_BUSY_STATUS, 1);
    //APB WRITE 0x70 0x1

    VpuWriteReg(coreIdx, W5_RET_SUCCESS, 0);	//for debug
    //APB WRITE 0x108 0x0

    //Wave5BitIssueCommand(instance, W5_CREATE_INSTANCE);
    APB_WRITE(0x110, 0x0);
    APB_WRITE(0x70, 0x1);
    APB_WRITE(0x100, 0x8);
    APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {   // Check QUEUE_DONE
        //APB READ 0x70 0x0

        //if (instance->loggingEnable)
        //    vdi_log(coreIdx, W5_CREATE_INSTANCE, 2);
        //vdi_free_dma_memory(coreIdx, &pDecInfo->vbWork);
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {           // FAILED for adding into VCPU QUEUE
        //APB READ 0x108 0x1
        //vdi_free_dma_memory(coreIdx, &pDecInfo->vbWork);
        return RETCODE_FAILURE;
		 
    }

    VpuReadReg(coreIdx, W5_PRODUCT_NUMBER);
    //APB READ 0x1044 0x521c
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    //Wave5VpuDecGetRdPtr /////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuDecGetRdPtr\n");

    // Send QUERY cmd
    VpuWriteReg(coreIdx, W5_QUERY_OPTION, 0x5);
    //APB WRITE 0x104 0x5

    VpuWriteReg(coreIdx, W5_VPU_BUSY_STATUS, 1);
    //APB WRITE 0x70 0x1

    //Wave5BitIssueCommand(instance, W5_QUERY);
    APB_WRITE(0x110, 0x0);
    APB_WRITE(0x70, 0x1);
    APB_WRITE(0x100, 0x4000);
    APB_WRITE(0x38, 0x1);


    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        //if (instance->loggingEnable)
        //    vdi_log(instance->coreIdx, W5_QUERY, 2);
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
    //APB READ 0x70 0x0

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE)
        return RETCODE_FAILURE;
    //APB READ 0x108 0x1

    VpuReadReg(coreIdx, W5_RET_QUERY_DEC_BS_RD_PTR);
    //APB READ 0x11c 0x8ad88000
    printf("ProductVpuDecGetRdPtr  : rdptr = 0x8ad88000\n");
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    //APB READ 0x1044 0x521c

    //APB READ 0x1044 0x521c
    //Wave5VpuDecGetRdPtr /////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuDecGetRdPtr\n");

    VpuWriteReg(coreIdx, W5_QUERY_OPTION, 0x5);
    //APB WRITE 0x104 0x5

    VpuWriteReg(coreIdx, W5_VPU_BUSY_STATUS, 1);
    //APB WRITE 0x70 0x1

    //Wave5BitIssueCommand(instance, W5_QUERY);
    APB_WRITE(0x110, 0x0);
    APB_WRITE(0x70, 0x1);
    APB_WRITE(0x100, 0x4000);
    APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        //if (instance->loggingEnable)
        //    vdi_log(instance->coreIdx, W5_QUERY, 2);
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
    //APB READ 0x70 0x0

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE)
        return RETCODE_FAILURE;
    //APB READ 0x108 0x1

    VpuReadReg(coreIdx, W5_RET_QUERY_DEC_BS_RD_PTR);
    //APB READ 0x11c 0x8ad88000
    printf("ProductVpuDecGetRdPtr  : rdptr = 0x8ad88000\n");
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



    Wave5VpuDecSetBitstreamFlag

    APB WRITE 0x120 0x0

    APB WRITE 0x11c 0x8adc8000

    APB WRITE 0x110 0x0

    APB WRITE 0x70 0x1

    APB WRITE 0x100 0x8000

    APB WRITE 0x38 0x1
    vdi->product_code 521c
    normalReg 1

    APB READ 0x70 0x0

    APB READ 0x108 0x1
    [vdi_get_instance_pool 468]
    [vdi_unlock 161]
    [vdi_get_instance_pool 468]
    [vdi_lock 153]
    [vdi_get_instance_pool 468]
    [vdi_get_instance_pool 468]

	
	
	
	
	

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////


	//Wave5VpuEncInitSeq //////////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuEncInitSeq\n");
	APB_WRITE(0x104, 0x1);
	APB_WRITE(0x11c, 0x1);
	APB_WRITE(0x120, 0x1045);
	APB_WRITE(0x124, 0x0);
	APB_WRITE(0x128, 0x0);
	APB_WRITE(0x12c, 0x0);
	APB_WRITE(0x130, 0x0);
	APB_WRITE(0x134, 0x0);
	APB_WRITE(0x138, 0x0);
	APB_WRITE(0x13c, 0x0);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x200);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70 0x0

	APB_WRITE(0x104, 0x0);
	APB_WRITE(0x11c, 0xf001a0);
	APB_WRITE(0x120, 0x0);
	APB_WRITE(0x124, 0x8020001);
	APB_WRITE(0x128, 0x60000000);
	APB_WRITE(0x12c, 0x0);
	APB_WRITE(0x130, 0x50001);
	APB_WRITE(0x13c, 0xd);
	APB_WRITE(0x148, 0x0);
	APB_WRITE(0x150, 0x1e);
	APB_WRITE(0x154, 0x1e8480);
	APB_WRITE(0x16c, 0x0);
	APB_WRITE(0x158, 0xbb8fc025);
	APB_WRITE(0x1c4, 0x8002);
	APB_WRITE(0x15c, 0xacc8);
	APB_WRITE(0x168, 0xcc8cc8);
	APB_WRITE(0x160, 0x1010101);
	APB_WRITE(0x164, 0x1010101);
	APB_WRITE(0x170, 0x0);
	APB_WRITE(0x1a8, 0x3800410);
	APB_WRITE(0x1ac, 0x0);
	APB_WRITE(0x134, 0x0);
	APB_WRITE(0x138, 0x0);
	APB_WRITE(0x140, 0x0);
	APB_WRITE(0x1b0, 0x0);
	APB_WRITE(0x174, 0x3e8);
	APB_WRITE(0x178, 0x7530);
	APB_WRITE(0x17c, 0x0);
	APB_WRITE(0x184, 0x0);
	APB_WRITE(0x188, 0x0);
	APB_WRITE(0x18c, 0x0);
	APB_WRITE(0x190, 0x0);
	APB_WRITE(0x194, 0x0);
	APB_WRITE(0x198, 0x0);
	APB_WRITE(0x19c, 0x0);
	APB_WRITE(0x144, 0x0);
	APB_WRITE(0x1a0, 0x0);
	APB_WRITE(0x1a4, 0x0);

	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x200);

	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70); //0x0

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == 0) {
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);
        if (regVal != WAVE5_QUEUEING_FAIL)
            VLOG(ERR, "FAIL_REASON = 0x%x\n", regVal);

        if ( regVal == WAVE5_QUEUEING_FAIL)
            return RETCODE_QUEUEING_FAILURE;
        else if (regVal == WAVE5_SYSERR_ACCESS_VIOLATION_HW)
            return RETCODE_MEMORY_ACCESS_VIOLATION;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
            return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
            return RETCODE_ERROR_FW_FATAL;
        else
            return RETCODE_FAILURE;
    }
	//APB_READ(0x108); //0x1
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//HandlingInterruptFlag ///////////////////////////////////////////////////////////////////////////////////
    printf("HandlingInterruptFlag\n");
    while(1)
    {
		if(APB_READ(0x44) == 1) // 0x1
		{
			if((reason = APB_READ(0x4c)) == 0x200) // 0x200
			{
				APB_WRITE(0x34, 0x200);
				APB_WRITE(0x3c, 0x1);
				break;
			}
		}
    }

    if (reason & (1 << INT_WAVE5_ENC_SET_PARAM)) {
        regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
        //APB_READ(0x1e8); // 0x1
        regVal = (regVal & 0xffff);
        if (regVal & (1 << 0/*instIndex*/)) {
            //ownInt = 1;
            //reason = (1 << INT_WAVE5_ENC_SET_PARAM);
            //remain_intr &= ~(Uint32)reason;

            //regVal = regVal & ~(1UL << instIndex);
            //VpuWriteReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST, regVal);
            APB_WRITE(0x1e8, 0x0);
        }
    }

    //VpuWriteReg(coreIdx, W5_VPU_VINT_REASON, remain_intr);
	APB_WRITE(0x4c, 0x0);
	//Wave5VpuWaitInterrupt : int_reason = 0x200
	//[vdi_get_instance_pool 468]
	//[vdi_unlock 161]
	//Wave5VpuClearInterrupt : flags = 0x200

	reason = VpuReadReg(coreIdx, W5_VPU_VINT_REASON_USR);
	//APB_READ(0x30); // 0x200
	reason &= ~reason;
    VpuWriteReg(coreIdx, W5_VPU_VINT_REASON_USR, reason);
	//APB_WRITE(0x30, 0x0);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuEncGetSeqInfo ////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuEncGetSeqInfo\n");
    //SendQuery
	APB_WRITE(0x104, 0x2);
	APB_WRITE(0x70, 0x1);
	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x4000);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
    	//APB_READ(0x70 0x0
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE)
        return RETCODE_FAILURE;
	//APB_READ(0x108 0x1

	APB_READ(0x1e0);// 0x0

    if (VpuReadReg(coreIdx, W5_RET_ENC_ENCODING_SUCCESS) != 1) {
        VpuReadReg(coreIdx, W5_RET_ENC_ERR_INFO);
        return RETCODE_FAILURE;
    }
	//APB_READ(0x1dc);// 0x1

    VpuReadReg(coreIdx, W5_RET_ENC_WARN_INFO);
	//APB_READ(0x1d4);// 0x0

	APB_READ(0x1fc);// 0x0
	APB_READ(0x11c);// 0x2
	APB_READ(0x120);// 0x1
	APB_READ(0x16c);// 0x0
	APB_READ(0x1b0);// 0x134380
	APB_READ(0x1b4);// 0x4540
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuEncRegisterFramebuffer //////////////////////////////////////////////////////////////////////
	printf("Wave5VpuEncRegisterFramebuffer\n");
	//W5_CMD_SET_FB_ADDR_TASK_BUF
	APB_WRITE(0x1d4, 0x8b05c000);
	//W5_CMD_SET_FB_TASK_BUF_SIZE
	APB_WRITE(0x1d8, 0x3a0fc0);

	//W5_ADDR_SUB_SAMPLED_FB_BASE
	APB_WRITE(0x12c, 0x8b054000);
	//W5_SUB_SAMPLED_ONE_FB_SIZE
	APB_WRITE(0x130, 0x1c00);

	//W5_PIC_SIZE
	APB_WRITE(0x11c, 0x1a000f0);
	//W5_FBC_STRIDE
	APB_WRITE(0x128, 0x6800340);
	//W5_COMMON_PIC_INFO
	APB_WRITE(0x118, 0x101a0);
	APB_WRITE(0x104, 0x18);
	APB_WRITE(0x120, 0x1);
	APB_WRITE(0x134, 0x8aff8000);
	APB_WRITE(0x138, 0x8b010600);
	APB_WRITE(0x13c, 0x8b04c000);
	APB_WRITE(0x140, 0x8b050000);
	APB_WRITE(0x1b4, 0x8b048000);
	APB_WRITE(0x144, 0x8b020000);
	APB_WRITE(0x148, 0x8b038600);
	APB_WRITE(0x14c, 0x8b04cf00);
	APB_WRITE(0x150, 0x8b050780);
	APB_WRITE(0x1b8, 0x8b048e00);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x80);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70);// 0x0

    regVal = VpuReadReg(coreIdx, W5_RET_SUCCESS);
    if (regVal == 0) {
        return RETCODE_FAILURE;
    }
	//APB_READ(0x108);// 0x1
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Wave5VpuEncGetHeader //////////////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuEncGetHeader\n");
    //W5_CMD_ENC_BS_START_ADDR
	APB_WRITE(0x118, 0x8ad88000);
	//W5_CMD_ENC_BS_SIZE
	APB_WRITE(0x11c, 0x200000);
	//W5_CMD_ENC_PIC_USE_SEC_AXI
	APB_WRITE(0x124, 0x0);
	//W5_CMD_ENC_PIC_CODE_OPTION
	APB_WRITE(0x164, 0x1c);
	//W5_CMD_ENC_PIC_SRC_PIC_IDX
	APB_WRITE(0x144, 0x0);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x100);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {   // Check QUEUE_DONE
       return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70); //0x0
    //W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x10000

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {           // FAILED for adding a command into VCPU QUEUE
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);

        if ( regVal == WAVE5_QUEUEING_FAIL)
            return RETCODE_QUEUEING_FAILURE;
        else if (regVal == WAVE5_SYSERR_ACCESS_VIOLATION_HW)
            return RETCODE_MEMORY_ACCESS_VIOLATION;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
            return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
            return RETCODE_ERROR_FW_FATAL;
        else
            return RETCODE_FAILURE;
    }
	//APB_READ(0x108); // 0x1
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//HandlingInterruptFlag //////////////////////////////////////////////////////////////////////////////////////
    printf("HandlingInterruptFlag\n");
    while(1)
    {
		if(APB_READ(0x44) == 1) // 0x1
		{
			if((reason = APB_READ(0x4c)) == 0x100) // 0x200
			{
				APB_WRITE(0x34, 0x100);
				APB_WRITE(0x3c, 0x1);
				break;
			}
		}
    }

    if (reason & (1 << INT_WAVE5_DEC_PIC)) {
        regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
    	//APB_READ(0x1e8 0x1

        regVal = (regVal & 0xffff);
        if (regVal & (1 << 0/*instance->instIndex*/)) {
            //ownInt = 1;
            int orgReason = reason;
            reason = (1 << INT_WAVE5_DEC_PIC);
            //remain_intr &= ~(Uint32)reason;
            /* Clear Low Latency Interrupt if two interrupts are occured */
            if (orgReason & (1 << INT_WAVE5_ENC_LOW_LATENCY)) {
                regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
                regVal = (regVal>>16);
                if (regVal & (1 << 0/*instance->instIndex*/)) {
                    //remain_intr &= ~(1<<INT_WAVE5_ENC_LOW_LATENCY);
                    Wave5VpuClearInterrupt(coreIdx, 1<<INT_WAVE5_ENC_LOW_LATENCY);
                }
            }
            regVal = regVal & ~(1UL << 0/*instance->instIndex*/);
            VpuWriteReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST, regVal);
            //APB_WRITE(0x1e8, 0x0);
        }
    }

    //VpuWriteReg(coreIdx, W5_VPU_VINT_REASON, remain_intr);
	APB_WRITE(0x4c, 0x0);

	//Wave5VpuWaitInterrupt : int_reason = 0x100
	//[vdi_get_instance_pool 468]
	//[vdi_unlock 161]
	//Wave5VpuClearInterrupt : flags = 0x100

	reason = VpuReadReg(coreIdx, W5_VPU_VINT_REASON_USR);
	//APB_READ(0x30); // 0x100

	reason &= ~reason;
    VpuWriteReg(coreIdx, W5_VPU_VINT_REASON_USR, reason);
	//APB_WRITE(0x30, 0x0);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//Wave5VpuEncGetResult //////////////////////////////////////////////////////////////////////////////////////////
    printf("Wave5VpuEncGetResult\n");
    //SendQuery
	APB_WRITE(0x104, 0x2);
	APB_WRITE(0x70, 0x1);
	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x4000);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70); // 0x0

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE)
        return RETCODE_FAILURE;
	//APB_READ(0x108); // 0x1

    //W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x0


	if(APB_READ(0x1dc) != 1)
		return RETCODE_FAILURE;
	//W5_RET_ENC_ENCODING_SUCCESS
	//APB_READ(0x1dc); // 0x1

	//W5_RET_ENC_WARN_INFO
	APB_READ(0x1d4); // 0x0
	//W5_RET_ENC_PIC_NUM
	APB_READ(0x158); // 0x0
	//W5_RET_ENC_PIC_TYPE
	APB_READ(0x124); // 0x0
	//W5_RET_ENC_NUT_0
	APB_READ(0x15c); // 0x0
	//W5_RET_ENC_NUT_1
	APB_READ(0x160); // 0x0
	//W5_RET_ENC_PIC_IDX
	APB_READ(0x12c); // 0xfffffffd
	//W5_RET_ENC_SVC_LAYER
	APB_READ(0x170); // 0x0
	//W5_RET_ENC_PIC_SLICE_NUM
	APB_READ(0x130); // 0x0
	//W5_RET_ENC_PIC_SKIP
	APB_READ(0x134); // 0x0
	//W5_RET_ENC_PIC_NUM_INTRA
	APB_READ(0x138); // 0x0
	//W5_RET_ENC_PIC_NUM_MERGE
	APB_READ(0x13c); // 0x0
	//W5_RET_ENC_PIC_NUM_SKIP
	APB_READ(0x144); // 0x0
	//W5_RET_ENC_PIC_AVG_CTU_QP
	APB_READ(0x148); // 0x0
	//W5_RET_ENC_PIC_BYTE
	APB_READ(0x14c); // 0x53
	//W5_RET_ENC_GOP_PIC_IDX
	APB_READ(0x150); // 0xffffffff
	//W5_RET_ENC_PIC_POC
	APB_READ(0x128); // 0x0
	//W5_RET_ENC_USED_SRC_IDX
	APB_READ(0x154); // 0xfffffffd
	//W5_RET_ENC_SRC_BUF_FLAG
	APB_READ(0x18c); // 0x0

    //W5_RET_ENC_WR_PTR
	APB_READ(0x118); // 0x8ad88053
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); // 0x8ad88000

	//W5_RET_ENC_PIC_DIST_LOW
	APB_READ(0x164); // 0x0
	//W5_RET_ENC_PIC_DIST_HIGH
	APB_READ(0x168); // 0x0
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); // 0x8ad88000

	//RET_ENC_HOST_CMD_TICK
	APB_READ(0x1b8); // 0x2d66d
	//RET_ENC_PREPARE_START_TICK
	APB_READ(0x1bc); // 0x2d691
	//RET_ENC_PREPARE_END_TICK
	APB_READ(0x1c0); // 0x2d72f
	//RET_ENC_PROCESSING_START_TICK
	APB_READ(0x1c4); // 0x2d759
	//RET_ENC_PROCESSING_END_TICK
	APB_READ(0x1c8); // 0x2d75c
	//RET_ENC_ENCODING_START_TICK
	APB_READ(0x1cc); // 0x2d770
	//RET_ENC_ENCODING_END_TICK
	APB_READ(0x1d0); // 0x2d786
	printf("00     0     0    -3    8ad88000  8ad88053       53      0     -3 0000    0     71936    40448      768     5632      0 0\n");
	//00     0     0    -3    8ad88000  8ad88053       53      0     -3 0000    0     71936    40448      768     5632      0 0
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//Wave5VpuEncode //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Wave5VpuEncode\n");
	//W5_CMD_ENC_BS_START_ADDR
	APB_WRITE(0x118, 0x8ad88000);
	//W5_CMD_ENC_BS_SIZE
	APB_WRITE(0x11c, 0x200000);
	//W5_CMD_ENC_PIC_SRC_AXI_SEL
	APB_WRITE(0x160, 0x0);
	//W5_CMD_ENC_PIC_USE_SEC_AXI
	APB_WRITE(0x124, 0x0);
	//W5_CMD_ENC_PIC_REPORT_ENDIAN
	APB_WRITE(0x12c, 0x10);

	//W5_CMD_ENC_PIC_CODE_OPTION
	APB_WRITE(0x164, 0x3);
	//W5_CMD_ENC_PIC_PIC_PARAM
	APB_WRITE(0x168, 0x0);

	//W5_CMD_ENC_PIC_SRC_PIC_IDX
	APB_WRITE(0x144, 0x0);
	//W5_CMD_ENC_PIC_SRC_ADDR_Y
	APB_WRITE(0x148, 0x8afa8000);
	//W5_CMD_ENC_PIC_SRC_ADDR_U
	APB_WRITE(0x14c, 0x8afc0600);
	//W5_CMD_ENC_PIC_SRC_ADDR_V
	APB_WRITE(0x150, 0xffffffff);

	//W5_CMD_ENC_PIC_SRC_STRIDE
	APB_WRITE(0x154, 0x1a001a0);

	//W5_CMD_ENC_PIC_SRC_FORMAT
	APB_WRITE(0x158, 0x3e2);
	//W5_CMD_ENC_PIC_CUSTOM_MAP_OPTION_ADDR
	APB_WRITE(0x13c, 0x0);
	//W5_CMD_ENC_PIC_CUSTOM_MAP_OPTION_PARAM
	APB_WRITE(0x138, 0x0);
	//W5_CMD_ENC_PIC_LONGTERM_PIC
	APB_WRITE(0x16c, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_SIGMA_Y
	APB_WRITE(0x170, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_SIGMA_C
	APB_WRITE(0x174, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_MEAN_Y
	APB_WRITE(0x178, 0x0);
	//W5_CMD_ENC_PIC_WP_PIXEL_MEAN_C
	APB_WRITE(0x17c, 0x0);

	//Wave5BitIssueCommand
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x100);
	APB_WRITE(0x38, 0x1);

    if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {   // Check QUEUE_DONE
        return RETCODE_VPU_RESPONSE_TIMEOUT;
    }
	//APB_READ(0x70); // 0x0

    //W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x10000

    if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE) {           // FAILED for adding a command into VCPU QUEUE
        regVal = VpuReadReg(coreIdx, W5_RET_FAIL_REASON);
        if (regVal != WAVE5_QUEUEING_FAIL)
            VLOG(ERR, "FAIL_REASON = 0x%x\n", regVal);

        if (regVal == WAVE5_QUEUEING_FAIL)
            return RETCODE_QUEUEING_FAILURE;
        else if (regVal == WAVE5_SYSERR_WATCHDOG_TIMEOUT)
            return RETCODE_VPU_RESPONSE_TIMEOUT;
        else if (regVal == WAVE5_ERROR_FW_FATAL)
            return RETCODE_ERROR_FW_FATAL;
        else
            return RETCODE_FAILURE;
    }
	//APB_READ(0x108); // 0x1
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//HandlingInterruptFlag //////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("HandlingInterruptFlag\n");
    while(1)
    {
		if(APB_READ(0x44) == 1) // 0x1
		{
			if((reason = APB_READ(0x4c)) == 0x100) // 0x200
			{
				APB_WRITE(0x34, 0x100);
				APB_WRITE(0x3c, 0x1);
				break;
			}
		}
    }

    if (reason & (1 << INT_WAVE5_DEC_PIC)) {
        regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
    	//APB_READ(0x1e8 0x1

        regVal = (regVal & 0xffff);
        if (regVal & (1 << 0/*instance->instIndex*/)) {
            //ownInt = 1;
            int orgReason = reason;
            reason = (1 << INT_WAVE5_DEC_PIC);
            //remain_intr &= ~(Uint32)reason;
            /* Clear Low Latency Interrupt if two interrupts are occured */
            if (orgReason & (1 << INT_WAVE5_ENC_LOW_LATENCY)) {
                regVal = VpuReadReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST);
                regVal = (regVal>>16);
                if (regVal & (1 << 0/*instance->instIndex*/)) {
                    //remain_intr &= ~(1<<INT_WAVE5_ENC_LOW_LATENCY);
                    Wave5VpuClearInterrupt(coreIdx, 1<<INT_WAVE5_ENC_LOW_LATENCY);
                }
            }
            regVal = regVal & ~(1UL << 0/*instance->instIndex*/);
            VpuWriteReg(coreIdx, W5_RET_QUEUE_CMD_DONE_INST, regVal);
            //APB_WRITE(0x1e8, 0x0);
        }
    }

    //VpuWriteReg(coreIdx, W5_VPU_VINT_REASON, remain_intr);
	APB_WRITE(0x4c, 0x0);

	//Wave5VpuWaitInterrupt : int_reason = 0x100
	//[vdi_get_instance_pool 468]
	//[vdi_unlock 161]
	//Wave5VpuClearInterrupt : flags = 0x100

	reason = VpuReadReg(coreIdx, W5_VPU_VINT_REASON_USR);
	//APB_READ(0x30); // 0x100

	reason &= ~reason;
	VpuWriteReg(coreIdx, W5_VPU_VINT_REASON_USR, reason);
	//APB_WRITE(0x30, 0x0);


	//Wave5VpuEncGetResult /////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Wave5VpuEncGetResult\n");
	//SendQuery
	APB_WRITE(0x104, 0x2);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x110, 0x10000);
	APB_WRITE(0x70, 0x1);
	APB_WRITE(0x100, 0x4000);
	APB_WRITE(0x38, 0x1);

	if (vdi_wait_vpu_busy(coreIdx, __VPU_BUSY_TIMEOUT, W5_VPU_BUSY_STATUS) == -1) {
	   return RETCODE_VPU_RESPONSE_TIMEOUT;
	}
	//APB_READ(0x70); // 0x0

	if (VpuReadReg(coreIdx, W5_RET_SUCCESS) == FALSE)
	    return RETCODE_FAILURE;
	//APB_READ(0x108); // 0x1

	//W5_RET_QUEUE_STATUS
	APB_READ(0x1e0); // 0x0
	//W5_RET_ENC_ENCODING_SUCCESS

	if(APB_READ(0x1dc) != 1)
		return RETCODE_FAILURE;
	//APB_READ(0x1dc); // 0x1

	//W5_RET_ENC_WARN_INFO
	APB_READ(0x1d4); // 0x0
	//W5_RET_ENC_PIC_NUM
	APB_READ(0x158); // 0x1
	//W5_RET_ENC_PIC_TYPE
	APB_READ(0x124); // 0x0
	//W5_RET_ENC_NUT_0
	APB_READ(0x15c); // 0x13
	//W5_RET_ENC_NUT_1
	APB_READ(0x160); // 0x0
	//W5_RET_ENC_PIC_IDX
	APB_READ(0x12c); // 0x0
	//W5_RET_ENC_SVC_LAYER
	APB_READ(0x170); // 0x0
	//W5_RET_ENC_PIC_SLICE_NUM
	APB_READ(0x130); // 0x55980
	//W5_RET_ENC_PIC_SKIP
	APB_READ(0x134); // 0x0
	//W5_RET_ENC_PIC_NUM_INTRA
	APB_READ(0x138); // 0x618
	//W5_RET_ENC_PIC_NUM_MERGE
	APB_READ(0x13c); // 0x0
	//W5_RET_ENC_PIC_NUM_SKIP
	APB_READ(0x144); // 0x0
	//W5_RET_ENC_PIC_AVG_CTU_QP
	APB_READ(0x148); // 0x17
	//W5_RET_ENC_PIC_BYTE
	APB_READ(0x14c); // 0x8dc8
	//W5_RET_ENC_GOP_PIC_IDX
	APB_READ(0x150); // 0x0
	//W5_RET_ENC_PIC_POC
	APB_READ(0x128); // 0x0
	//W5_RET_ENC_USED_SRC_IDX
	APB_READ(0x154); // 0x0
	//W5_RET_ENC_SRC_BUF_FLAG
	APB_READ(0x18c); // 0x1

    //W5_RET_ENC_WR_PTR
	APB_READ(0x118); // 0x8ad90dc8
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); // 0x8ad88000

	//W5_RET_ENC_PIC_DIST_LOW
	APB_READ(0x164); // 0x8004e9a7
	//W5_RET_ENC_PIC_DIST_HIGH
	APB_READ(0x168); // 0x8afa3e7e
	//W5_RET_ENC_RD_PTR
	APB_READ(0x114); // 0x8ad88000

	//RET_ENC_HOST_CMD_TICK
	APB_READ(0x1b8); // 0x3d0a6
	//RET_ENC_PREPARE_START_TICK
	APB_READ(0x1bc); // 0x3d1be
	//RET_ENC_PREPARE_END_TICK
	APB_READ(0x1c0); // 0x3d1be
	//RET_ENC_PROCESSING_START_TICK
	APB_READ(0x1c4); // 0x3d1f7
	//RET_ENC_PROCESSING_END_TICK
	APB_READ(0x1c8); // 0x3d3f8
	//RET_ENC_ENCODING_START_TICK
	APB_READ(0x1cc); // 0x3d442
	//RET_ENC_ENCODING_END_TICK
	APB_READ(0x1d0); // 0x3d648

	printf("00     1     0     0    8ad88000  8ad90dc8     8dc8      0      0 0001    0    369152    60416   131328   132608      0 0\n");
	//00     1     0     0    8ad88000  8ad90dc8     8dc8      0      0 0001    0    369152    60416   131328   132608      0 0
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



}
#endif
#endif
#endif
