/*******************************************************************************
 *                                                                             *
 * Copyright (c) 2009 Texas Instruments Incorporated - http://www.ti.com/      *
 *                        ALL RIGHTS RESERVED                                  *
 *                                                                             *
 ******************************************************************************/

#include <xdc/std.h>
#include <mcfw/interfaces/link_api/system_tiler.h>
#include "captureLink_priv.h"
#include "mcfw/interfaces/common_def/ti_vsys_common_def.h"


char *gCaptureLink_portName[] = {
    "VIP0 PortA", "VIP0 PortB", "VIP1 PortA", "VIP1 PortB",
};

char *gCaptureLink_ifName[] = {
    " 8-bit", "16-bit", "24-bit",
};

char *gCaptureLink_modeName[] = {
    "Non-mux Embedded Sync",
    "Line-mux Embedded Sync",
    "Pixel-mux Embedded Sync",
    "Non-mux Discrete Sync - HSYNC_VBLK",
    "Non-mux Discrete Sync - HSYNC_VSYNC",
    "Non-mux Discrete Sync - ACTVID_VBLK",
    "Non-mux Discrete Sync - ACTVID_VSYNC",
    "Split Line Embedded Sync"
};

/* driver callback */
Int32 CaptureLink_drvCallback(FVID2_Handle handle, Ptr appData, Ptr reserved)
{
    CaptureLink_Obj *pObj = (CaptureLink_Obj *) appData;

    if (pObj->cbCount && (pObj->cbCount % CAPTURE_LINK_TSK_TRIGGER_COUNT) == 0)
    {
        Utils_tskSendCmd(&pObj->tsk, SYSTEM_CMD_NEW_DATA);
    }
    pObj->cbCount++;

    return FVID2_SOK;
}

Int32 CaptureLink_drvInstSetFrameSkip(CaptureLink_Obj * pObj, UInt16 instId,
                                      UInt32 frameSkipMask)
{
    Vps_CaptFrameSkip frameSkip;
    UInt16 outId, chId;
    Int32 status = FVID2_SOK;
    CaptureLink_InstObj *pInst;
    Vps_CaptCreateParams *pVipCreateArgs;

    pInst = &pObj->instObj[instId];
    pVipCreateArgs = &pInst->createArgs;

    /*
     * set frame skip using a IOCTL if enabled
     */
    for (outId = 0; outId < pVipCreateArgs->numStream; outId++)
    {
        for (chId = 0; chId < pInst->numChPerOutput; chId++)
        {

            frameSkip.channelNum = pVipCreateArgs->channelNumMap[outId][chId];

            frameSkip.frameSkipMask = frameSkipMask;

            status = FVID2_control(pInst->captureVipHandle,
                                   IOCTL_VPS_CAPT_SET_FRAME_SKIP,
                                   &frameSkip, NULL);
            UTILS_assert(status == FVID2_SOK);
        }
    }

    return status;
}

Void CaptureLink_drvBlindAreaChnlParamInit(CaptureLink_Obj * pObj,Vps_CaptOutInfo *pVipOutPrm,
                                        UInt16 queId, UInt16 queChId)
{
    UInt16 winId;
    CaptureLink_ChObj *pChObj;

    for(winId = 0;winId<CAPTURE_LINK_MAX_BLIND_AREA_PER_CHNL;winId++)
    {
        pChObj = &pObj->chObj[queId][queChId];


        pChObj->blindArea[winId].dataFormat = pVipOutPrm->dataFormat;
        pChObj->blindArea[winId].destPitch[0] = pVipOutPrm->pitch[0];
        pChObj->blindArea[winId].destPitch[1] = pVipOutPrm->pitch[1];

        if(((pChObj->blindArea[winId].dataFormat != FVID2_DF_YUV422I_YUYV)
            &&(pChObj->blindArea[winId].dataFormat != FVID2_DF_YUV420SP_UV))
            || pVipOutPrm->memType == VPS_VPDMA_MT_TILEDMEM)
        {
            // now we only support 2 format without tiler memory
            pChObj->blindArea[winId].dataFormat = FVID2_DF_INVALID;
        }
    }

}

Int32 CaptureLink_drvChObjReset(CaptureLink_Obj * pObj)
{
    CaptureLink_ChObj *pChObj;
    Int32 queId, chId;

    memset(pObj->chObj, 0, sizeof(pObj->chObj));

    for(queId=0; queId<CAPTURE_LINK_MAX_OUT_QUE; queId++)
    {
        for(chId=0; chId<CAPTURE_LINK_MAX_CH_PER_OUT_QUE; chId++)
        {
            pChObj = &pObj->chObj[queId][chId];

            pChObj->instId = 0xFF;
            pChObj->frameSkipMask = 0;
            pChObj->skipOddFields = FALSE;
            pChObj->skipOddFieldsThreshold = 0;
            pChObj->oddFieldSkipRatio = CAPTURE_LINK_ODD_FIELD_SKIP_NONE;
            pChObj->prevFid = 0;

            pChObj->chRtResolutionChange = FALSE;
        }
    }

    return FVID2_SOK;
}

Int32 CaptureLink_drvSkipOddFields(
            CaptureLink_Obj * pObj,
            CaptureLink_SkipOddFields *pPrm
            )
{
    CaptureLink_ChObj *pChObj;
    CaptureLink_InstObj *pInst;
    Vps_CaptFrameSkip frameSkipMask;
    Int32 chId, numChInQue;

    if(pPrm->queId>=CAPTURE_LINK_MAX_OUT_QUE)
        return FVID2_EFAIL;

    numChInQue = pObj->info.queInfo[pPrm->queId].numCh;

    for(chId=0; chId<numChInQue; chId++)
    {
        pChObj = &pObj->chObj[pPrm->queId][chId];

        pChObj->skipOddFields = FALSE;

        if(pPrm->skipOddFieldsChBitMask & (1<<chId))
        {
            pChObj->skipOddFields = TRUE;
        }

        pInst = &pObj->instObj[pChObj->instId];

        frameSkipMask.channelNum = CaptureLink_makeChannelNum(pPrm->queId, chId);

        if(pChObj->skipOddFields==FALSE)
        {
            frameSkipMask.frameSkipMask = 0;
        }
        else
        {
            if (pChObj->oddFieldSkipRatio != pPrm->oddFieldSkipRatio)
            {
                switch (pPrm->oddFieldSkipRatio)
                {
                    case CAPTURE_LINK_ODD_FIELD_SKIP_ALL:
                    {
                        pChObj->frameSkipMask = 0xAAAAAAAA;
                        break;
                    }
                    case CAPTURE_LINK_ODD_FIELD_SKIP_1_4:
                    {
                        pChObj->frameSkipMask = 0x45454545;
                        break;
                    }
                    case CAPTURE_LINK_ODD_FIELD_SKIP_NONE:
                    case CAPTURE_LINK_ODD_FIELD_SKIP_1_2:
                    default:
                    {
                        pChObj->frameSkipMask = 0;
#ifdef SYSTEM_DEBUG_CAPTURE
                        Vps_printf(" %d: CAPTURE: unsupported oddFieldSkipRatio: %d, NO field will be skipped!\n",
                        Utils_getCurTimeInMsec(), pPrm->oddFieldSkipRatio);
#endif
                    }
                } // end of switch
            }
            pChObj->oddFieldSkipRatio   = pPrm->oddFieldSkipRatio;
            frameSkipMask.frameSkipMask = pChObj->frameSkipMask;
        }

        FVID2_control(
                pInst->captureVipHandle,
                IOCTL_VPS_CAPT_SET_FRAME_SKIP,
                &frameSkipMask,
                NULL
                );

    }

    return FVID2_SOK;
}

Int32 CaptureLink_drvSetResolution(
            CaptureLink_Obj * pObj,
            CaptureLink_SetResolution *pPrm
            )
{
    CaptureLink_ChObj *pChObj;
    CaptureLink_InstObj *pInst;
    System_LinkChInfo *pQueChInfo;

    if((pPrm->queId >= CAPTURE_LINK_MAX_OUT_QUE) ||
       (pPrm->chId >= CAPTURE_LINK_MAX_CH_PER_OUT_QUE))
        return FVID2_EFAIL;

    pQueChInfo = &pObj->info.queInfo[pPrm->queId].chInfo[pPrm->chId];
    pChObj     = &pObj->chObj[pPrm->queId][pPrm->chId];

    if(pChObj->instId >= VPS_CAPT_INST_MAX)
        return FVID2_EFAIL;

    pInst = &pObj->instObj[pChObj->instId];

    if ((pPrm->width  > pInst->maxWidth) ||
        (pPrm->height > pInst->maxHeight))
        return FVID2_EFAIL;

    pChObj->chRtResolutionChange = TRUE;
    pQueChInfo->width            = pPrm->width;
    pQueChInfo->height           = pPrm->height;

    return FVID2_SOK;
}

Int32 CaptureLink_drvRtResolutionUpdate(CaptureLink_Obj * pObj, FVID2_Frame *pFrame)
{
    CaptureLink_ChObj *pChObj;
    Int32 status = FVID2_SOK;
    Int32 queId, queChId;
    System_FrameInfo *frameInfo;
    System_LinkChInfo *pQueChInfo;

    queId = CaptureLink_getQueId(pFrame->channelNum);
    queChId = CaptureLink_getQueChId(pFrame->channelNum);

    UTILS_assert(queId < CAPTURE_LINK_MAX_OUT_QUE);
    UTILS_assert(queChId < CAPTURE_LINK_MAX_CH_PER_OUT_QUE);

    pChObj = &pObj->chObj[queId][queChId];
    pQueChInfo = &pObj->info.queInfo[queId].chInfo[queChId];
    frameInfo = (System_FrameInfo *)pFrame->appData;
    UTILS_assert(frameInfo!=NULL);
    if (pChObj->chRtResolutionChange == TRUE)
    {
        pChObj->chRtResolutionChange = FALSE;
#ifdef SYSTEM_DEBUG_CAPTURE
        Vps_printf(" %d: CAPTURE: Dynamic resolution change: width: %d, height: %d!\n",
                       Utils_getCurTimeInMsec(),
                       pQueChInfo->width,
                       pQueChInfo->height);
#endif
    }

    /*
       Populate frameInfo for all frames for safety
       in case other links are assuming each frame has the RT params updated
       There is optimization space here
    */
    if (frameInfo)
    {
        frameInfo->rtChInfoUpdate      = TRUE;

        frameInfo->rtChInfo.width      = pQueChInfo->width;
        frameInfo->rtChInfo.height     = pQueChInfo->height;
        frameInfo->rtChInfo.pitch[0]   = pQueChInfo->pitch[0];
        frameInfo->rtChInfo.pitch[1]   = pQueChInfo->pitch[1];
        frameInfo->rtChInfo.pitch[2]   = pQueChInfo->pitch[2];

        frameInfo->rtChInfo.startX     = pQueChInfo->startX;
        frameInfo->rtChInfo.startY     = pQueChInfo->startY;
        frameInfo->rtChInfo.bufType    = pQueChInfo->bufType;
        frameInfo->rtChInfo.scanFormat = pQueChInfo->scanFormat;
        frameInfo->rtChInfo.dataFormat = pQueChInfo->dataFormat;
        frameInfo->rtChInfo.memType    = pQueChInfo->memType;
    }

    return status;

}

Int32 CaptureLink_drvCheckAndSetFrameSkipMask(CaptureLink_Obj * pObj, FVID2_Frame *pFrame)
{
    CaptureLink_InstObj *pInst;
    CaptureLink_ChObj *pChObj;
    Vps_CaptFrameSkip frameSkipMask;
    Int32 status = FVID2_SOK;
    Int32 queId, queChId;

    queId = CaptureLink_getQueId(pFrame->channelNum);
    queChId = CaptureLink_getQueChId(pFrame->channelNum);

    UTILS_assert(queId < CAPTURE_LINK_MAX_OUT_QUE);
    UTILS_assert(queChId < CAPTURE_LINK_MAX_CH_PER_OUT_QUE);

    pChObj = &pObj->chObj[queId][queChId];

    if(pChObj->skipOddFields==FALSE)
    {
        return FVID2_SOK;
    }

    if(pChObj->instId >= VPS_CAPT_INST_MAX)
        return FVID2_EFAIL;

    pInst = &pObj->instObj[pChObj->instId];

    if (pChObj->oddFieldSkipRatio == CAPTURE_LINK_ODD_FIELD_SKIP_ALL)
    {
        if(pFrame->fid!=0)
        {
            pChObj->skipOddFieldsThreshold++;

            if(pChObj->skipOddFieldsThreshold > CAPTURE_LINK_SKIP_ODD_FIELDS_THRESHOLD)
            {
                pChObj->skipOddFieldsThreshold = 0;

                /* need to invert and set frame skip mask */
                pChObj->frameSkipMask = ~pChObj->frameSkipMask;

                frameSkipMask.channelNum = pFrame->channelNum;
                frameSkipMask.frameSkipMask = pChObj->frameSkipMask;

                status = FVID2_control(
                        pInst->captureVipHandle,
                        IOCTL_VPS_CAPT_SET_FRAME_SKIP,
                        &frameSkipMask,
                        NULL
                        );
                UTILS_assert(status==FVID2_SOK);
            }
        }
    }
    else if (pChObj->oddFieldSkipRatio == CAPTURE_LINK_ODD_FIELD_SKIP_1_4)
    {
        if (pFrame->fid == 1) /* odd field */
        {
            if (pChObj->prevFid == 1)
            {
                pChObj->skipOddFieldsThreshold++;
            }
        }
        pChObj->prevFid = pFrame->fid;

        if(pChObj->skipOddFieldsThreshold > CAPTURE_LINK_SKIP_ODD_FIELDS_THRESHOLD)
        {
            pChObj->skipOddFieldsThreshold = 0;

            /* need to shift and set frame skip mask, the shifted bit should be set to MSB in case continous shifting happens */
            pChObj->frameSkipMask = (pChObj->frameSkipMask>>1)|((pChObj->frameSkipMask&0x1)<<31);

            frameSkipMask.channelNum = pFrame->channelNum;
            frameSkipMask.frameSkipMask = pChObj->frameSkipMask;
            status = FVID2_control(
                    pInst->captureVipHandle,
                    IOCTL_VPS_CAPT_SET_FRAME_SKIP,
                    &frameSkipMask,
                    NULL
                    );
            UTILS_assert(status==FVID2_SOK);
          }
      }
      else
      {
#ifdef SYSTEM_DEBUG_CAPTURE
            Vps_printf(" %d: CAPTURE: unsupported oddFieldSkipRatio: %d, NO field will be skipped!\n",
                       Utils_getCurTimeInMsec(), pChObj->oddFieldSkipRatio);
#endif
      }

    return status;
}

/* Create capture driver */
Int32 CaptureLink_drvCreateInst(CaptureLink_Obj * pObj, UInt16 instId)
{
    CaptureLink_VipInstParams *pInstPrm;
    CaptureLink_InstObj *pInst;
    Vps_CaptCreateParams *pVipCreateArgs;
    Vps_CaptOutInfo *pVipOutPrm;
    CaptureLink_OutParams *pOutPrm;
    System_LinkChInfo *pQueChInfo;
    UInt16 queId, queChId, outId, chId, inWidth, inHeight;

    pInstPrm = &pObj->createArgs.vipInst[instId];
    pInst = &pObj->instObj[instId];
    pVipCreateArgs = &pInst->createArgs;

    pInst->instId = pInstPrm->vipInstId;
    pInst->numChPerOutput = pInstPrm->numChPerOutput;
    pInst->enableTs = pInstPrm->enableTimestampInInterrupt;

    CaptureLink_drvInitCreateArgs(pVipCreateArgs);

    pVipCreateArgs->inDataFormat = pInstPrm->inDataFormat;

    if (pObj->createArgs.overrideNumBufsInInstPrms)
    {
        pInst->numBufsPerCh = pInstPrm->numBufsPerCh;
    }
    else
    {
        pInst->numBufsPerCh = pObj->createArgs.numBufsPerCh;
    }

    pVipCreateArgs->videoCaptureMode    = pInstPrm->videoCaptureMode;
    pVipCreateArgs->videoIfMode         = pInstPrm->videoIfMode     ;
    pVipCreateArgs->inScanFormat        = pInstPrm->inScanFormat    ;
    pVipCreateArgs->numCh               = 1;

    switch (pInstPrm->standard)
    {
        case FVID2_STD_1080P_60:
        case FVID2_STD_1080P_30:
            pInst->maxWidth = 1920;
            pInst->maxHeight = 1080;
            inWidth  = pInst->maxWidth;
            inHeight = pInst->maxHeight;
            break;

        case FVID2_STD_1080I_60:
            if (pVipCreateArgs->inDataFormat == FVID2_DF_RGB24_888)
            {
                pVipCreateArgs->inScanFormat = FVID2_SF_PROGRESSIVE;
                    /* since in 24-bit discrete sync mode FID signal
                        is not connected in board */
            }
            pInst->maxWidth = 1920;
            pInst->maxHeight = 540;
            inWidth  = pInst->maxWidth;
            inHeight = pInst->maxHeight;
            break;

        case FVID2_STD_720P_60:
            pInst->maxWidth = 1280;
            pInst->maxHeight = 720;
            inWidth  = pInst->maxWidth;
            inHeight = pInst->maxHeight;
            break;

        case FVID2_STD_D1:

            if(pObj->createArgs.enableSdCrop)
                pInst->maxWidth = 352;
            else
                pInst->maxWidth = 360;

            if(pObj->isPalMode)
                pInst->maxHeight = 288;
            else
                pInst->maxHeight = 240;

            // psuedo HD mode
            if(pObj->createArgs.fakeHdMode)
            {
                pVipCreateArgs->videoCaptureMode =
                    VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC;

                {
                    pInst->maxWidth = 1920;
                    pInst->maxHeight = 1080;
                }
            }

            inWidth  = pInst->maxWidth;
            inHeight = pInst->maxHeight;
            break;

        case FVID2_STD_MUX_2CH_D1:
            pVipCreateArgs->numCh = 2;

            if(pObj->createArgs.enableSdCrop)
                pInst->maxWidth = 704;
            else
                pInst->maxWidth = 720;

            if(pObj->isPalMode)
                pInst->maxHeight = 288;
            else
                pInst->maxHeight = 240;

            inWidth  = pInst->maxWidth;
            inHeight = pInst->maxHeight;

            break;

        case FVID2_STD_MUX_4CH_960H:
            pVipCreateArgs->numCh = 4;

            if(pObj->createArgs.enableSdCrop)
                pInst->maxWidth = 944;
            else
                pInst->maxWidth = 960;

            if(pObj->isPalMode)
                pInst->maxHeight = 288;
            else
                pInst->maxHeight = 240;

            inWidth  = pInst->maxWidth;
            inHeight = pInst->maxHeight;
            break;

        case FVID2_STD_MUX_4CH_D1:
            pVipCreateArgs->numCh = 4;

            if(pObj->createArgs.enableSdCrop)
                pInst->maxWidth = 704;
            else
                pInst->maxWidth = 720;

            if(pObj->isPalMode)
                pInst->maxHeight = 288;
            else
                pInst->maxHeight = 240;

            inWidth  = pInst->maxWidth;
            inHeight = pInst->maxHeight;
            break;

        case FVID2_STD_MUX_4CH_HALF_D1:
            pVipCreateArgs->numCh = 4;

            if(pObj->createArgs.enableSdCrop)
                pInst->maxWidth = 352;
            else
                pInst->maxWidth = 360;

            if(pObj->isPalMode)
                pInst->maxHeight = 288;
            else
                pInst->maxHeight = 240;

            inWidth  = pInst->maxWidth;
            inHeight = pInst->maxHeight;
            break;

        default:
            pInst->maxWidth = 1920;
            pInst->maxHeight = 1080;

            inWidth  = pInst->maxWidth;
            inHeight = pInst->maxHeight;
            break;
    }

    if (pInst->numChPerOutput == 0)
    {
        pInst->numChPerOutput = pVipCreateArgs->numCh;
    }
    UTILS_assert(pVipCreateArgs->numCh >= pInst->numChPerOutput);

    pVipCreateArgs->periodicCallbackEnable = TRUE;

    pVipCreateArgs->numStream = pInstPrm->numOutput;

    for (outId = 0; outId < pVipCreateArgs->numStream; outId++)
    {
        pVipOutPrm = &pVipCreateArgs->outStreamInfo[outId];
        pOutPrm = &pInstPrm->outParams[outId];

        pVipOutPrm->dataFormat = pOutPrm->dataFormat;

        pVipOutPrm->memType = VPS_VPDMA_MT_NONTILEDMEM;

        if (pObj->createArgs.tilerEnable &&
            (pOutPrm->dataFormat == FVID2_DF_YUV420SP_UV ||
             pOutPrm->dataFormat == FVID2_DF_YUV422SP_UV))
        {
            pVipOutPrm->memType = VPS_VPDMA_MT_TILEDMEM;
        }

        pVipOutPrm->pitch[0] =
            VpsUtils_align(pInst->maxWidth, VPS_BUFFER_ALIGNMENT * 2);
        if ((pInst->maxWidth == 704)         &&
            (pObj->createArgs.enableSdCrop) &&
            (pObj->createArgs.doCropInCapture))
        {
            pVipOutPrm->pitch[0] =
                VpsUtils_align(720, VPS_BUFFER_ALIGNMENT * 2);
        }

        if ((pInst->maxWidth == 944)         &&
            (pObj->createArgs.enableSdCrop) &&
            (pObj->createArgs.doCropInCapture))
        {
            pVipOutPrm->pitch[0] =
                VpsUtils_align(960, VPS_BUFFER_ALIGNMENT * 2);
        }

        if (pVipOutPrm->dataFormat == FVID2_DF_YUV422I_YUYV)
            pVipOutPrm->pitch[0] *= 2;
        if (pVipOutPrm->dataFormat == FVID2_DF_RGB24_888)
            pVipOutPrm->pitch[0] *= 3;

        pVipOutPrm->pitch[1] = pVipOutPrm->pitch[0];

        if (CaptureLink_drvIsDataFormatTiled(pVipCreateArgs, outId))
        {
            pVipOutPrm->pitch[0] = VPSUTILS_TILER_CNT_8BIT_PITCH;
            pVipOutPrm->pitch[1] = VPSUTILS_TILER_CNT_16BIT_PITCH;
        }

        pVipOutPrm->pitch[2] = 0;

        pVipOutPrm->scEnable = pOutPrm->scEnable;

        if (pInst->maxHeight <= 288)
        {
            pVipOutPrm->maxOutHeight = VPS_CAPT_MAX_OUT_HEIGHT_288_LINES;
        }
        else if (pInst->maxHeight <= 576)
        {
            pVipOutPrm->maxOutHeight = VPS_CAPT_MAX_OUT_HEIGHT_576_LINES;
        }
        else if (pInst->maxHeight <= 720)
        {
            pVipOutPrm->maxOutHeight = VPS_CAPT_MAX_OUT_HEIGHT_720_LINES;
        }
        else if (pInst->maxHeight <= 1080)
        {
            pVipOutPrm->maxOutHeight = VPS_CAPT_MAX_OUT_HEIGHT_1080_LINES;
        }
        else
        {
            pVipOutPrm->maxOutHeight = VPS_CAPT_MAX_OUT_HEIGHT_UNLIMITED;
        }

        /* KC:
             No need to keep unlimited height
             This was kept mainly for debug and statistics collection purpose.
             In real system we should limit the height to max expected
        */
        // pVipOutPrm->maxOutHeight = VPS_CAPT_MAX_OUT_HEIGHT_UNLIMITED;

        if (pInst->maxWidth <= 352)
        {
            pVipOutPrm->maxOutWidth = VPS_CAPT_MAX_OUT_WIDTH_352_PIXELS;
        }
        else if (pInst->maxWidth <= 768)
        {
            pVipOutPrm->maxOutWidth = VPS_CAPT_MAX_OUT_WIDTH_768_PIXELS;
        }
        else if (pInst->maxWidth <= 1280)
        {
            pVipOutPrm->maxOutWidth = VPS_CAPT_MAX_OUT_WIDTH_1280_PIXELS;
        }
        else if (pInst->maxWidth <= 1920)
        {
            pVipOutPrm->maxOutWidth = VPS_CAPT_MAX_OUT_WIDTH_1920_PIXELS;
        }
        else
        {
            pVipOutPrm->maxOutWidth = VPS_CAPT_MAX_OUT_WIDTH_UNLIMITED;
        }

        /* KC:
             No need to keep unlimited Width
             This was kept mainly for debug and statistics collection purpose.
             In real system we should limit the Width to max expected
        */
        // pVipOutPrm->maxOutWidth = VPS_CAPT_MAX_OUT_WIDTH_UNLIMITED;


        if (pVipOutPrm->scEnable)
        {
            Vps_CaptScParams *pScParams;

            pScParams = &pVipCreateArgs->scParams;

            pScParams->inScanFormat = FVID2_SF_PROGRESSIVE; // NOT USED
            pScParams->inWidth = inWidth;
            pScParams->inHeight = inHeight;
            pScParams->inCropCfg.cropStartX = 0;
            pScParams->inCropCfg.cropStartY = 0;
            pScParams->inCropCfg.cropWidth = (pScParams->inWidth & 0xFFFE);
            pScParams->inCropCfg.cropHeight = (pScParams->inHeight & 0xFFFE);
            pScParams->outWidth = pOutPrm->scOutWidth;
            if (pObj->isPalMode)
            {
                /* input source is PAL mode scOutHeight is setup assuming
                 * NTSC so convert it to 288 lines for PAL mode */
                if (pOutPrm->scOutHeight == 240)
                    pOutPrm->scOutHeight = 288;
            }
            pScParams->outHeight = pOutPrm->scOutHeight;
            pScParams->scConfig = NULL;
            pScParams->scCoeffConfig = NULL;
        }

        for (chId = 0; chId < pInst->numChPerOutput; chId++)
        {
            queId = pOutPrm->outQueId;
            queChId = pObj->info.queInfo[queId].numCh;

            pQueChInfo = &pObj->info.queInfo[queId].chInfo[queChId];

            pQueChInfo->dataFormat = (FVID2_DataFormat) pVipOutPrm->dataFormat;
            pQueChInfo->memType = (Vps_VpdmaMemoryType) pVipOutPrm->memType;

            if (pVipOutPrm->scEnable && !pObj->createArgs.fakeHdMode)
            {
                pQueChInfo->width = pOutPrm->scOutWidth;
                pQueChInfo->height = pOutPrm->scOutHeight;
            }
            else
            {
                pQueChInfo->width = pInst->maxWidth;
                pQueChInfo->height = pInst->maxHeight;
            }

            pQueChInfo->startX = 0;
            pQueChInfo->startY = 0;
            pQueChInfo->pitch[0] = pVipOutPrm->pitch[0];
            pQueChInfo->pitch[1] = pVipOutPrm->pitch[1];
            pQueChInfo->pitch[2] = pVipOutPrm->pitch[2];

            if(pObj->createArgs.fakeHdMode && pInstPrm->standard == FVID2_STD_D1)
            {
                pQueChInfo->scanFormat = FVID2_SF_PROGRESSIVE;
            }
            else
            {
                pQueChInfo->scanFormat = pVipCreateArgs->inScanFormat;
            }

            pObj->info.queInfo[queId].numCh++;

            pVipCreateArgs->channelNumMap[outId][chId] =
                CaptureLink_makeChannelNum(queId, queChId);

            pObj->chObj[queId][queChId].instId = instId;

            //configure EDMA stuff for blind area
            CaptureLink_drvBlindAreaChnlParamInit(pObj,
                                                pVipOutPrm,
                                                queId,queChId);
        }
    }

    memset(&pInst->cbPrm, 0, sizeof(pInst->cbPrm));

    pInst->cbPrm.appData = pObj;

    if (instId == 0)
        pInst->cbPrm.cbFxn = CaptureLink_drvCallback;

    pObj->enableCheckOverflowDetect = TRUE;
    if (pVipCreateArgs->videoCaptureMode ==
        VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_DISCRETE_SYNC_ACTVID_VBLK)
    {
        pObj->enableCheckOverflowDetect = FALSE;
    }

    pVipCreateArgs->muxModeStartChId    = pInstPrm->muxModeStartChId;

    pVipCreateArgs->vipParserInstConfig = NULL;
    pVipCreateArgs->vipParserPortConfig = NULL;
    pVipCreateArgs->cscConfig           = NULL;

    if(pInstPrm->useAdvancedParams)
    {
        int i, j;

        pVipCreateArgs->vipParserInstConfig = &pInst->vipParserInstConfig;
        pVipCreateArgs->vipParserPortConfig = &pInst->vipParserPortConfig;
        pVipCreateArgs->cscConfig           = &pInst->cscConfig;

        pInst->vipParserInstConfig.clipActive = pInstPrm->advancedParams.clipActive;
        pInst->vipParserInstConfig.clipBlank  = pInstPrm->advancedParams.clipBlank;

        pInst->vipParserPortConfig.ctrlChanSel                = pInstPrm->advancedParams.ctrlChanSel;
        pInst->vipParserPortConfig.ancChSel8b                 = pInstPrm->advancedParams.ancChSel8b;
        pInst->vipParserPortConfig.pixClkEdgePol              = pInstPrm->advancedParams.pixClkEdgePol;
        pInst->vipParserPortConfig.invertFidPol               = pInstPrm->advancedParams.invertFidPol;

        pInst->vipParserPortConfig.embConfig.errCorrEnable    = pInstPrm->advancedParams.embSyncErrCorrEnable;
        pInst->vipParserPortConfig.embConfig.srcNumPos        = pInstPrm->advancedParams.embSyncSrcNumPos;
        pInst->vipParserPortConfig.embConfig.isMaxChan3Bits   = pInstPrm->advancedParams.embSyncIsMaxChan3Bits;

        pInst->vipParserPortConfig.disConfig.fidSkewPostCnt   = pInstPrm->advancedParams.disSyncFidSkewPostCnt;
        pInst->vipParserPortConfig.disConfig.fidSkewPreCnt    = pInstPrm->advancedParams.disSyncFidSkewPreCnt;
        pInst->vipParserPortConfig.disConfig.lineCaptureStyle = pInstPrm->advancedParams.disSyncLineCaptureStyle;
        pInst->vipParserPortConfig.disConfig.fidDetectMode    = pInstPrm->advancedParams.disSyncFidDetectMode;
        pInst->vipParserPortConfig.disConfig.actvidPol        = pInstPrm->advancedParams.disSyncActvidPol;
        pInst->vipParserPortConfig.disConfig.vsyncPol         = pInstPrm->advancedParams.disSyncVsyncPol;
        pInst->vipParserPortConfig.disConfig.hsyncPol         = pInstPrm->advancedParams.disSyncHsyncPol;

        pInst->cscConfig.bypass = pInstPrm->advancedParams.cscBypass;
        pInst->cscConfig.mode   = pInstPrm->advancedParams.cscMode;
        pInst->cscConfig.coeff  = &pInst->cscCoeffs;

        for(i=0; i<VPS_CSC_NUM_COEFF; i++)
        {
            pInst->cscCoeffs.addCoeff[i] = pInstPrm->advancedParams.cscAddCoeff[i];

            for(j=0; j<VPS_CSC_NUM_COEFF; j++)
                pInst->cscCoeffs.mulCoeff[i][j] = pInstPrm->advancedParams.cscMulCoeff[i][j];
        }

    }

    Vps_printf(" %d: CAPTURE: %s capture mode is [%s, %s] !!! \n",
               Utils_getCurTimeInMsec(),
               gCaptureLink_portName[pInstPrm->vipInstId],
               gCaptureLink_ifName[pVipCreateArgs->videoIfMode],
               gCaptureLink_modeName[pVipCreateArgs->videoCaptureMode]);

    pInst->captureVipHandle = FVID2_create(FVID2_VPS_CAPT_VIP_DRV,
                                           pInst->instId,
                                           pVipCreateArgs,
                                           &pInst->createStatus, &pInst->cbPrm);

    UTILS_assert(pInst->captureVipHandle != NULL);

    CaptureLink_drvAllocAndQueueFrames(pObj, pInst);

    if (pInst->instId == VPS_CAPT_INST_SC5_WB2)
    {
        CaptureLink_drvInstSetFrameSkip(pObj, instId, 0u);
    }
    else
    {
        CaptureLink_drvInstSetFrameSkip(pObj, instId, 0u);
    }

    return FVID2_SOK;
}

Void CaptureLink_drvBlindAreaInit(CaptureLink_Obj * pObj,
                                CaptureLink_CreateParams * pPrm)
{
    Int32 status,winId;
    UInt32 queId, channelId;
    CaptureLink_ChObj *pChObj;

    for(queId = 0;queId<CAPTURE_LINK_MAX_OUT_QUE;queId++)
    {
        for(channelId = 0;channelId<CAPTURE_LINK_MAX_CH_PER_OUT_QUE;channelId++)
        {
            pChObj = &pObj->chObj[queId][channelId];

            pChObj->numBlindArea = 0;

            for(winId = 0;winId< CAPTURE_LINK_MAX_BLIND_AREA_PER_CHNL;winId++)
            {
                pChObj->blindAreaConfig.win[winId].enableWin = FALSE;
            }
            pChObj->chBlindAreaConfigUpdate = FALSE;
        }
    }


    //create DMA channel for blind area
    status = Utils_dmaCreateCh(&pObj->dmaObj,
                                   UTILS_DMA_DEFAULT_EVENT_Q,
                                   CAPTURE_LINK_MAX_BLIND_WIN_PER_DMA, TRUE);
    UTILS_assert(status==FVID2_SOK);
}

/** Create capture link

    This creates
    - capture driver
    - video decoder driver
    - allocate and queues frames to the capture driver
    - DOES NOT start the capture ONLY make it ready for operation
*/
Int32 CaptureLink_drvCreate(CaptureLink_Obj * pObj,
                            CaptureLink_CreateParams * pPrm)
{
    Int32 status;
    UInt32 queId, instId;

#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: Create in progress !!!\n", Utils_getCurTimeInMsec());
#endif

    UTILS_MEMLOG_USED_START();
    memcpy(&pObj->createArgs, pPrm, sizeof(*pPrm));

    CaptureLink_drvChObjReset(pObj);

    #ifndef SYSTEM_USE_TILER
    if (pObj->createArgs.tilerEnable)
    {
        Vps_printf("CAPTURELINK:!!WARNING.FORCIBLY DISABLING TILER since tiler is disabled at build time");
        pObj->createArgs.tilerEnable = FALSE;
    }
    #endif

    pObj->captureDequeuedFrameCount = 0;
    pObj->captureQueuedFrameCount = 0;
    pObj->cbCount = 0;
    pObj->cbCountServicedCount = 0;
    pObj->prevFrameCount = 0;
    pObj->totalCpuLoad = 0;
    pObj->resetCount = 0;
    pObj->resetTotalTime = 0;
    pObj->prevResetTime = 0;
    pObj->isPalMode = FALSE;

    pObj->brightness = 0x1c;                               /* TUNED for
                                                            * specific
                                                            * scene's, to
                                                            * make black
                                                            * blacker */
    pObj->contrast = 0x89;                                 /* TUNED for
                                                            * specific
                                                            * scene's, to
                                                            * make black
                                                            * blacker */
    pObj->saturation = 128;                                /* default */

    pObj->hue        = 128;

    memset(pObj->captureFrameCount, 0, sizeof(pObj->captureFrameCount));

    pObj->info.numQue = CAPTURE_LINK_MAX_OUT_QUE;

    pObj->isPalMode = pPrm->isPalMode;

    for (queId = 0; queId < CAPTURE_LINK_MAX_OUT_QUE; queId++)
    {
        status = Utils_bufCreate(&pObj->bufQue[queId], FALSE, FALSE);
        UTILS_assert(status == FVID2_SOK);

        pObj->info.queInfo[queId].numCh = 0;
    }

    /*
     * Create global VIP capture handle, used for dequeue,
     * queue from all active captures
     */
    pObj->fvidHandleVipAll = FVID2_create(FVID2_VPS_CAPT_VIP_DRV,
                                          VPS_CAPT_INST_VIP_ALL,
                                          NULL, NULL, NULL);
    UTILS_assert(pObj->fvidHandleVipAll != NULL);


    if (CAPTURE_LINK_TMP_BUF_SIZE)
    {
        pObj->tmpBufAddr = Utils_memAlloc(CAPTURE_LINK_TMP_BUF_SIZE, 32);
    }
#if 0
    status = FVID2_control(pObj->fvidHandleVipAll,
                           IOCTL_VPS_CAPT_DROP_DATA_BUFFER,
                           pObj->tmpBufAddr, NULL);
    UTILS_assert(status == FVID2_SOK);
#endif

    status = FVID2_control(pObj->fvidHandleVipAll,
                           IOCTL_VPS_CAPT_RESET_VIP0, NULL, NULL);
    UTILS_assert(status == FVID2_SOK);

    status = FVID2_control(pObj->fvidHandleVipAll,
                           IOCTL_VPS_CAPT_RESET_VIP1, NULL, NULL);
    UTILS_assert(status == FVID2_SOK);

    System_clearVipResetFlag(SYSTEM_VIP_0);
    System_clearVipResetFlag(SYSTEM_VIP_1);

    for (instId = 0; instId < pPrm->numVipInst; instId++)
    {
        CaptureLink_drvCreateInst(pObj, instId);
    }

    /* this is effective only if CaptureLink_CreateParams.numExtraBufs > 0 */
    CaptureLink_drvAllocAndQueueExtraFrames(pObj, 0, 0, 0);

    CaptureLink_drvBlindAreaInit(pObj,pPrm);

    UTILS_MEMLOG_USED_END(pObj->memUsed);
    UTILS_MEMLOG_PRINT("CAPTURE:",
                       pObj->memUsed,
                       UTILS_ARRAYSIZE(pObj->memUsed));
#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: Create Done !!!\n", Utils_getCurTimeInMsec());
#endif

    return status;
}

Int32 CaptureLink_drvOverflowDetectAndReset(CaptureLink_Obj * pObj,
                                            Bool doForceReset)
{
    Vps_CaptOverFlowStatus overFlowStatus;
    UInt32 curTime = Utils_getCurTimeInMsec();;

#if 0
    {
        UInt32 elaspedTime;

        elaspedTime = Utils_getCurTimeInMsec() - pObj->startTime;

        if ((elaspedTime - pObj->prevResetTime) > 5 * 1000)
        {
            pObj->prevResetTime = elaspedTime;
            doForceReset = TRUE;
        }
    }
#endif

    FVID2_control(pObj->fvidHandleVipAll,
                  IOCTL_VPS_CAPT_CHECK_OVERFLOW, NULL, &overFlowStatus);

    if (doForceReset)
    {
        overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTA] = TRUE;
        overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTB] = TRUE;
        overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTA] = TRUE;
        overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTB] = TRUE;
    }

    if (overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTA]
        ||
        overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTB]
        ||
        overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTA]
        || overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTB])
    {
        // System_haltExecution();

        pObj->resetCount++;
        curTime = Utils_getCurTimeInMsec();
    }

    if (overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTA]
        || overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTB])
    {
        System_lockVip(SYSTEM_VIP_0);

        Vps_rprintf
            (" %d: CAPTURE: Overflow detected on VIP0, Total Resets = %d\n",
             Utils_getCurTimeInMsec(), pObj->resetCount);
    }
    if (overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTA]
        || overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTB])
    {
        System_lockVip(SYSTEM_VIP_1);

        Vps_rprintf
            (" %d: CAPTURE: Overflow detected on VIP1, Total Resets = %d\n",
             Utils_getCurTimeInMsec(), pObj->resetCount);
    }

    FVID2_control(pObj->fvidHandleVipAll,
                  IOCTL_VPS_CAPT_RESET_AND_RESTART, &overFlowStatus, NULL);

    if (overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTA]
        || overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTB])
    {
        System_setVipResetFlag(SYSTEM_VIP_0);
        System_unlockVip(SYSTEM_VIP_0);
    }
    if (overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTA]
        || overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTB])
    {
        System_setVipResetFlag(SYSTEM_VIP_1);
        System_unlockVip(SYSTEM_VIP_1);
    }
    if (overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTA]
        ||
        overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP0_PORTB]
        ||
        overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTA]
        || overFlowStatus.isPortOverFlowed[VPS_CAPT_INST_VIP1_PORTB])
    {
        curTime = Utils_getCurTimeInMsec() - curTime;
        pObj->resetTotalTime += curTime;
    }

    return FVID2_SOK;
}

Int32 CaptureLink_drvPrintRtStatus(CaptureLink_Obj * pObj, UInt32 frameCount,
                                   UInt32 elaspedTime)
{
    UInt32 fps = (frameCount * 10000) / (elaspedTime);

    Vps_rprintf
        (" %-8s: Capture FPS: %d.%d fps ... in %d.%d secs\n",
         "CAPTURE",
         fps/10,
         fps%10,
         elaspedTime/1000,
         (elaspedTime%1000)/100
         );

    return 0;
}

Int32 CapterLink_drvBlindAreaConfigure(CaptureLink_Obj * pObj,
                                          CaptureLink_BlindInfo *blindAreaInfo)
{
    UInt32 i;
    UInt32 queId, channelId;
    CaptureLink_ChObj *pChObj;

    queId = blindAreaInfo->queId;
    channelId = blindAreaInfo->channelId;

    UTILS_assert(blindAreaInfo->numBlindArea <= CAPTURE_LINK_MAX_BLIND_AREA_PER_CHNL);
    UTILS_assert(blindAreaInfo->numBlindArea <= pObj->createArgs.maxBlindAreasPerCh);
    UTILS_assert(channelId <CAPTURE_LINK_MAX_CH_PER_OUT_QUE);

    pChObj = &pObj->chObj[queId][channelId];
    pChObj->blindAreaConfig.channelId = channelId;

    pChObj->blindAreaConfig.queId  = queId;
    pChObj->blindAreaConfig.numBlindArea  = blindAreaInfo->numBlindArea;

    for(i = 0;i<blindAreaInfo->numBlindArea;i++)
    {
        if(blindAreaInfo->win[i].enableWin == TRUE)
        {
            pChObj->blindAreaConfig.win[i].enableWin = TRUE;
            pChObj->blindAreaConfig.win[i].startX = blindAreaInfo->win[i].startX;
            pChObj->blindAreaConfig.win[i].startY = blindAreaInfo->win[i].startY;
            pChObj->blindAreaConfig.win[i].width  = blindAreaInfo->win[i].width;
            pChObj->blindAreaConfig.win[i].height = blindAreaInfo->win[i].height;
            pChObj->blindAreaConfig.win[i].fillColorYUYV = blindAreaInfo->win[i].fillColorYUYV;
        }
        else
        {
            pChObj->blindAreaConfig.win[i].enableWin = FALSE;
        }

    }

    pChObj->chBlindAreaConfigUpdate =  TRUE;
    // if it is the dataFormat we're not support, we should disable the BlindArea here
    if(pChObj->blindArea[0].dataFormat == FVID2_DF_INVALID)
    {
        pChObj->blindAreaConfig.numBlindArea = 0;
    }

    return 0;
}

static Void CaptureLink_setFrameWallTime(CaptureLink_Obj * pObj,
                                         FVID2_Frame  *frame)
{
    System_FrameInfo *frameInfo;

    frameInfo = frame->appData;
    if (frameInfo)
    {
        #ifdef SYSTEM_DEBUG_AVSYNC_DETAILED_LOGS
        AvsyncLink_logCaptureTS(frame->channelNum,frameInfo->ts64);
        #endif
    }
}

Void CaptureLink_drvBlindAreaProcessData(CaptureLink_Obj * pObj,
                                        UInt32 queId,
                                        UInt32 queChId,
                                        FVID2_Frame *pFrame)
{
    Int32 status,i;
    UInt32 blindWin;
    CaptureLink_ChObj *pChObj;

    pChObj = &pObj->chObj[queId][queChId];

    //add blind area mask process here.
    if(pChObj->chBlindAreaConfigUpdate == TRUE)
    {
        CaptureLink_BlindInfo *blindAreaInfo;

        blindAreaInfo = &pChObj->blindAreaConfig;
        blindWin = 0;
        pChObj->numBlindArea = 0;
        for(i = 0;i<blindAreaInfo->numBlindArea;i++)
        {
            if(blindAreaInfo->win[i].enableWin == TRUE)
            {
              pChObj->blindArea[blindWin].startX = blindAreaInfo->win[i].startX;
              pChObj->blindArea[blindWin].startY = blindAreaInfo->win[i].startY;
              pChObj->blindArea[blindWin].width  = blindAreaInfo->win[i].width;
              pChObj->blindArea[blindWin].height = blindAreaInfo->win[i].height;
              pChObj->blindArea[blindWin].fillColorYUYV = blindAreaInfo->win[i].fillColorYUYV;
              pChObj->blindArea[blindWin].fastFillColorIndex = UTILS_DMA_FAST_FILL_COLOR_BLACK;
              pChObj->numBlindArea++;
              blindWin++;
            }
        }
        pChObj->chBlindAreaConfigUpdate = FALSE;
    }

    if(pChObj->numBlindArea != 0)
    {
        for(blindWin = 0;blindWin < pChObj->numBlindArea;blindWin ++)
        {
            pChObj->blindArea[blindWin].destAddr[0] =
                        pFrame->addr[0][0];
            pChObj->blindArea[blindWin].destAddr[1] =
                        pFrame->addr[0][1];
        }
        status = Utils_dmaFastFill2D(&pObj->dmaObj, pChObj->blindArea, pChObj->numBlindArea);
        UTILS_assert(status == FVID2_SOK);
    }
}



Int32 CaptureLink_drvProcessData(CaptureLink_Obj * pObj)
{
    UInt32 frameId, queId, streamId, queChId, elaspedTime;
    FVID2_FrameList frameList;
    FVID2_Frame *pFrame;
    volatile UInt32 sendMsgToTsk = 0;
    Int32 status;

    pObj->cbCountServicedCount++;

    System_displayUnderflowCheck(FALSE);

    for (streamId = 0; streamId < CAPTURE_LINK_MAX_OUTPUT_PER_INST; streamId++)
    {
        /*
         * Deque frames for all active handles
         */
        FVID2_dequeue(pObj->fvidHandleVipAll,
                      &frameList, streamId, BIOS_NO_WAIT);

        if (frameList.numFrames)
        {
            for (frameId = 0; frameId < frameList.numFrames; frameId++)
            {
                pFrame = frameList.frames[frameId];

                CaptureLink_setFrameWallTime(pObj,pFrame);

                queId = CaptureLink_getQueId(pFrame->channelNum);
                queChId = CaptureLink_getQueChId(pFrame->channelNum);

                UTILS_assert(queId < CAPTURE_LINK_MAX_OUT_QUE);
                UTILS_assert(queChId < CAPTURE_LINK_MAX_CH_PER_OUT_QUE);

                CaptureLink_drvCheckAndSetFrameSkipMask(pObj, pFrame);

                pObj->captureDequeuedFrameCount++;
                pObj->captureFrameCount[queId][queChId]++;

                CaptureLink_drvRtResolutionUpdate(pObj, pFrame);

                CaptureLink_drvBlindAreaProcessData(pObj,
                                                    queId,
                                                    queChId,
                                                    pFrame);
                pFrame->perFrameCfg = NULL;
                pFrame->channelNum = queChId;
                sendMsgToTsk |= (1 << queId);
                status = Utils_bufPutFullFrame(&pObj->bufQue[queId], pFrame);
                UTILS_assert(status == FVID2_SOK);
            }

#ifdef SYSTEM_DEBUG_CAPTURE_RT
            Vps_printf(" %d: CAPTURE: Dequeued %d frames !!!\n",
                       Utils_getCurTimeInMsec(), frameList.numFrames);
#endif
        }
    }

    elaspedTime = Utils_getCurTimeInMsec() - pObj->startTime;

    if ((elaspedTime - pObj->prevTime) > SYSTEM_RT_STATS_LOG_INTERVAL * 1000)
    {
        #ifdef SYSTEM_PRINT_RT_AVG_STATS_LOG
        CaptureLink_drvPrintRtStatus(pObj,
                                     pObj->captureDequeuedFrameCount -
                                     pObj->prevFrameCount,
                                     elaspedTime - pObj->prevTime);
        #endif

        pObj->prevFrameCount = pObj->captureDequeuedFrameCount;
        pObj->prevTime = elaspedTime;
    }

    for (queId = 0; queId < CAPTURE_LINK_MAX_OUT_QUE; queId++)
    {
        if (sendMsgToTsk & 0x1)
        {
            /* send data available message to next tsk */
            System_sendLinkCmd(pObj->createArgs.outQueParams[queId].nextLink,
                               SYSTEM_CMD_NEW_DATA);
        }

        sendMsgToTsk >>= 1;
        if (sendMsgToTsk == 0)
            break;
    }

    if (pObj->enableCheckOverflowDetect)
    {
        CaptureLink_drvOverflowDetectAndReset(pObj, FALSE);
    }

    pObj->exeTime = Utils_getCurTimeInMsec() - pObj->startTime;

    return FVID2_SOK;
}

Int32 CaptureLink_drvPutEmptyFrames(CaptureLink_Obj * pObj,
                                    FVID2_FrameList * pFrameList)
{
    UInt32 frameId;
    FVID2_Frame *pFrame;
    System_FrameInfo *pFrameInfo;

    if (pFrameList->numFrames)
    {
        for (frameId = 0; frameId < pFrameList->numFrames; frameId++)
        {
            pFrame = pFrameList->frames[frameId];


            pFrameInfo = (System_FrameInfo *) pFrame->appData;
            UTILS_assert(pFrameInfo != NULL);

            pFrame->perFrameCfg = &pFrameInfo->captureRtParams;

            if(pFrameInfo->captureChannelNum == CAPTURE_LINK_EXTRA_FRAME_CH_ID)
            {
                UInt32 cookie;

                /* this frame is from extra buffer pool,
                    make it go to the current CH ID which need extra buffers at this moment

                    since pObj->extraFrameObj.captureChannelNum could get
                    updated at run-time in a different thread
                    protect this via interrupt lock
                */

                cookie = Hwi_disable();

                pFrame->channelNum = pObj->extraFrameObj.captureChannelNum;

                Hwi_restore(cookie);
            }
            else
            {
                pFrame->channelNum = pFrameInfo->captureChannelNum;
            }
        }

#ifdef SYSTEM_DEBUG_CAPTURE_RT
        Vps_printf(" %d: CAPTURE: Queued back %d frames !!!\n",
                   Utils_getCurTimeInMsec(), pFrameList->numFrames);
#endif

        pObj->captureQueuedFrameCount += pFrameList->numFrames;

        FVID2_queue(pObj->fvidHandleVipAll, pFrameList, VPS_CAPT_STREAM_ID_ANY);
    }

    return FVID2_SOK;
}

Void CaptureLink_drvSetFrameCropBufPtr(CaptureLink_Obj * pObj,
                                       FVID2_FrameList * pFrameList)
{
    if (pObj->createArgs.enableSdCrop
        &&
        pObj->createArgs.doCropInCapture)
    {
        Int i;
        FVID2_Frame *pFrame;

        for (i = 0; i < pFrameList->numFrames; i++)
        {
            pFrame = pFrameList->frames[i];
            pFrame->addr[0][0] = (Ptr)((UInt32)pFrame->addr[0][0] +
                                       (CAPTURE_LINK_SD_CROP_LEN_PIXELS_LEFTMARGIN * 2));
        }
    }
}

Void CaptureLink_drvReSetFrameCropBufPtr(CaptureLink_Obj * pObj,
                                         FVID2_FrameList * pFrameList)
{
    if (pObj->createArgs.enableSdCrop
        &&
        pObj->createArgs.doCropInCapture)
    {
        Int i;
        FVID2_Frame *pFrame;

        for (i = 0; i < pFrameList->numFrames; i++)
        {
            pFrame = pFrameList->frames[i];
            pFrame->addr[0][0] = (Ptr)((UInt32)pFrame->addr[0][0] -
                                       (CAPTURE_LINK_SD_CROP_LEN_PIXELS_LEFTMARGIN * 2));
        }
    }
}

static UInt32 CaptureLink_GetTicks(Ptr args)
{
    FVID2_Frame *pFrame = (FVID2_Frame *) args;

    if ((pFrame != NULL) && (pFrame->appData != NULL))
    {
        ((System_FrameInfo *)(pFrame->appData))->ts64 = Avsync_getWallTime();
        return (UInt32) ((System_FrameInfo *)(pFrame->appData))->ts64;
    }
    /* If debug flags are enabled for capture driver, we could get
        NULL pointers for pFrame */
    return (UInt32) Avsync_getWallTime();
}

static Int32 CaptureLink_cfgTimeStamp (FVID2_Handle handle, UInt32 isPalMode)
{
    Int32 status;
    Vps_CaptFrameTimeStampParms tsParams;

    tsParams.enableBestEffort = TRUE;
    tsParams.timeStamper = &CaptureLink_GetTicks;
    if (isPalMode)
    {
        tsParams.expectedFps = 50;
    }
    else
    {
        tsParams.expectedFps = 60;
    }

    status = FVID2_control(
        handle,
        IOCTL_VPS_CAPT_CFG_TIME_STAMPING_FRAMES,
        &tsParams,
        NULL
        );

    UTILS_assert(status==FVID2_SOK);

    return status;
}

Int32 CaptureLink_drvStart(CaptureLink_Obj * pObj)
{
    UInt32 instId;
    CaptureLink_InstObj *pInstObj;

#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: Start in progress !!!\n", Utils_getCurTimeInMsec());
#endif

    Task_sleep(100);

    pObj->prevVideoDetectCheckTime = pObj->prevTime =
            pObj->startTime = Utils_getCurTimeInMsec();

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInstObj = &pObj->instObj[instId];

        if (pInstObj->enableTs == TRUE)
        {
            /* Enable time stamping in interrupt context */
            CaptureLink_cfgTimeStamp(pInstObj->captureVipHandle,
                pObj->isPalMode);
            Vps_printf(" %d: CAPTURE: Enabled Time Stamping !!!\n",
                Utils_getCurTimeInMsec());
        }

        /* VIP capture */
        FVID2_start(pInstObj->captureVipHandle, NULL);
        pInstObj->captureStartTime = Avsync_getWallTime();
    }

#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: Start Done !!!\n", Utils_getCurTimeInMsec());
#endif

    return FVID2_SOK;
}

Int32 CaptureLink_drvFlush(FVID2_Handle captureVipHandle, char *portName)
{
    Int32 status;
    FVID2_FrameList frameList;

    do
    {
        status = FVID2_control(captureVipHandle,
                               IOCTL_VPS_CAPT_FLUSH, NULL, &frameList);

#ifdef SYSTEM_DEBUG_CAPTURE_RT
        Vps_rprintf(" %d: CAPTURE: %s: Flushed %d frames.\n",
                    Utils_getCurTimeInMsec(), portName, frameList.numFrames);
#endif

    } while (frameList.numFrames != 0 && status == FVID2_SOK);

    if (status != FVID2_SOK)
    {
#ifdef SYSTEM_DEBUG_CAPTURE_RT
        Vps_rprintf(" %d: CAPTURE: %s: Flushing ... ERROR !!!\n",
                    Utils_getCurTimeInMsec(), portName);
#endif
    }

    return FVID2_SOK;
}

Int32 CaptureLink_drvStop(CaptureLink_Obj * pObj)
{
    UInt32 instId;
    CaptureLink_InstObj *pInstObj;

#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: Stop in progress !!!\n", Utils_getCurTimeInMsec());
#endif

    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInstObj = &pObj->instObj[instId];

        /* VIP capture */
        FVID2_stop(pInstObj->captureVipHandle, NULL);

        CaptureLink_drvFlush(pInstObj->captureVipHandle,
                             gCaptureLink_portName[pInstObj->instId]);
    }

    pObj->exeTime = Utils_getCurTimeInMsec() - pObj->startTime;

#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: Stop Done !!!\n", Utils_getCurTimeInMsec());
#endif

    return FVID2_SOK;
}

UInt32 CaptureLink_drvBlindAreaDelete(CaptureLink_Obj * pObj)
{
    UInt32 status;

    status = Utils_dmaDeleteCh(&pObj->dmaObj);
    UTILS_assert(status==FVID2_SOK);

    return status;
}

Int32 CaptureLink_drvDelete(CaptureLink_Obj * pObj)
{
    UInt32 instId;
    UInt32 queId;
    CaptureLink_InstObj *pInstObj;
    Int32 status = FVID2_SOK;

#ifdef SYSTEM_DEBUG_CAPTURE
    CaptureLink_drvPrintStatus(pObj);
#endif

#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: Delete in progress !!!\n", Utils_getCurTimeInMsec());
#endif

    if (CAPTURE_LINK_TMP_BUF_SIZE)
    {
        Utils_memFree(pObj->tmpBufAddr, CAPTURE_LINK_TMP_BUF_SIZE);
    }


    for (instId = 0; instId < pObj->createArgs.numVipInst; instId++)
    {
        pInstObj = &pObj->instObj[instId];

        /* VIP capture */
        FVID2_delete(pInstObj->captureVipHandle, NULL);

        CaptureLink_drvFreeFrames(pObj, pInstObj);
    }

    /* this is effective only if CaptureLink_CreateParams.numExtraBufs > 0 */
    CaptureLink_drvFreeExtraFrames(pObj);

    status = FVID2_delete(pObj->fvidHandleVipAll, NULL);

    for (queId = 0; queId < CAPTURE_LINK_MAX_OUT_QUE; queId++)
    {
        Utils_bufDelete(&pObj->bufQue[queId]);
    }

    status = CaptureLink_drvBlindAreaDelete(pObj);

#ifdef SYSTEM_DEBUG_CAPTURE
    Vps_printf(" %d: CAPTURE: Delete Done !!!\n", Utils_getCurTimeInMsec());
#endif

    return status;
}

Int32 CaptureLink_drvSetExtraFramesChId(CaptureLink_Obj * pObj, CaptureLink_ExtraFramesChId *pPrm)
{
    UInt32 cookie;

    /* check limits of queId and chId */
    if(pPrm->queId > CAPTURE_LINK_MAX_OUT_QUE)
        return FVID2_EFAIL;

    if(pPrm->chId > CAPTURE_LINK_MAX_CH_PER_OUT_QUE)
        return FVID2_EFAIL;

    /* since pObj->extraFrameObj.captureChannelNum could get
        updated at run-time in a different thread
        protect this via interrupt lock
    */

    cookie = Hwi_disable();

    pObj->extraFrameObj.captureChannelNum =
        CaptureLink_makeChannelNum(pPrm->queId, pPrm->chId);
        ;

    Hwi_restore(cookie);

    return FVID2_SOK;
}

Int32 CaptureLink_drvAllocAndQueueExtraFrames(CaptureLink_Obj * pObj, UInt32 instId, UInt32 streamId, UInt32 chId)
{
    Int32 status;
    UInt16 frameId;
    Vps_CaptOutInfo *pOutInfo;
    FVID2_Frame *frames;
    System_FrameInfo *pFrameInfo;
    FVID2_FrameList frameList;
    FVID2_Format *pFormat;
    Int i,j;
    CaptureLink_ExtraFrameObj *pExtraFrameObj;
    Vps_CaptCreateParams *pVipCreateArgs;

    if(pObj->createArgs.numExtraBufs == 0)
        return 0; /* no need to alocate extra buffers */

    /* limit numExtraBufs to CAPTURE_LINK_MAX_FRAMES_PER_CH */
    if(pObj->createArgs.numExtraBufs > CAPTURE_LINK_MAX_FRAMES_PER_CH)
        pObj->createArgs.numExtraBufs = CAPTURE_LINK_MAX_FRAMES_PER_CH;

    pExtraFrameObj = &pObj->extraFrameObj;

    pFormat = &pExtraFrameObj->format;

    /* extra frames allocated using instId, streamId , chId info */

    pVipCreateArgs = &pObj->instObj[instId].createArgs;

    pExtraFrameObj->captureChannelNum = pVipCreateArgs->channelNumMap[streamId][chId];

    pOutInfo = &pVipCreateArgs->outStreamInfo[streamId];

    pExtraFrameObj->tilerUsed =
        CaptureLink_drvIsDataFormatTiled(pVipCreateArgs, streamId);

    pFrameInfo = &pExtraFrameObj->frameInfo[0];
    frames = &pExtraFrameObj->frames[0];


    /* fill format with channel specific values */

    pFormat->channelNum =
        pExtraFrameObj->captureChannelNum;

    pFormat->width = pObj->instObj[instId].maxWidth;
    pFormat->height = pObj->instObj[instId].maxHeight;

    if(pFormat->height < CAPTURE_LINK_HEIGHT_MIN_LINES)
        pFormat->height = CAPTURE_LINK_HEIGHT_MIN_LINES;

    pFormat->pitch[0] = pOutInfo->pitch[0];
    pFormat->pitch[1] = pOutInfo->pitch[1];
    pFormat->pitch[2] = pOutInfo->pitch[2];
    pFormat->fieldMerged[0] = FALSE;
    pFormat->fieldMerged[1] = FALSE;
    pFormat->fieldMerged[2] = FALSE;
    pFormat->dataFormat = pOutInfo->dataFormat;
    pFormat->scanFormat = FVID2_SF_PROGRESSIVE;
    pFormat->bpp = FVID2_BPP_BITS8;                  /* ignored */

    if (pFormat->dataFormat == FVID2_DF_RAW_VBI)
    {
        pFormat->height = CAPTURE_LINK_RAW_VBI_LINES;
    }

    /*
     * alloc memory based on 'format'
     * Allocated frame info is put in frames[]
     * numBufsPerCh is the number of buffers per channel to
     * allocate
     */
    if (pExtraFrameObj->tilerUsed)
    {
        Utils_tilerFrameAlloc(pFormat, frames,
                              pObj->createArgs.numExtraBufs);
    }
    else
    {
        #ifdef SYSTEM_DEBUG_MEMALLOC
        Vps_printf("CAPTURE:ALLOCINFO:FMT[%d]/PITCH[%d]/HEIGHT[%d]/NUMBUFS[%d]",
                   pFormat->dataFormat,
                   pFormat->pitch[0],
                   pFormat->height,
                   pObj->createArgs.numExtraBufs);
        #endif
        Utils_memFrameAlloc(pFormat, frames,
                            pObj->createArgs.numExtraBufs);
    }

    /*
     * Set rtParams for every frame in perFrameCfg
     */
    for (frameId = 0; frameId < pObj->createArgs.numExtraBufs; frameId++)
    {
        frames[frameId].perFrameCfg =
            &pFrameInfo[frameId].captureRtParams;
        frames[frameId].subFrameInfo = NULL;
        frames[frameId].appData = &pFrameInfo[frameId];
        frames[frameId].reserved = NULL;
        for (i = 0; i < FVID2_MAX_FIELDS; i++)
        {
            for (j = 0; j < FVID2_MAX_PLANES ; j++)
            {
                pExtraFrameObj->origAddr[frameId][i][j] =
                    frames[frameId].addr[i][j];
            }
        }
        pFrameInfo[frameId].captureChannelNum = CAPTURE_LINK_EXTRA_FRAME_CH_ID;
        pFrameInfo[frameId].rtChInfoUpdate = FALSE;

        frameList.frames[frameId] = &frames[frameId];

#if 1 //def SYSTEM_VERBOSE_PRINTS
            Vps_rprintf(" %d: CAPTURE: %d: 0x%08x, %d x %d, %08x B --> Extra Frames \n",
                        Utils_getCurTimeInMsec(),
                        frameId, frames[frameId].addr[0][0],
                        pFormat->pitch[0] / 2, pFormat->height,
                        pFormat->height * pFormat->pitch[0]);
#endif
    }

    /*
     * Set number of frame in frame list
     */
    frameList.numFrames = pObj->createArgs.numExtraBufs;

    /*
     * queue the frames in frameList
     * All allocate frames are queued here as an example.
     * In general atleast 2 frames per channel need to queued
     * before starting capture,
     * else frame will get dropped until frames are queued
     */
    status =
        FVID2_queue(pObj->fvidHandleVipAll, &frameList, VPS_CAPT_STREAM_ID_ANY);

    UTILS_assert(status == FVID2_SOK);
    pObj->captureQueuedFrameCount += frameList.numFrames;

    return status;
}

Int32 CaptureLink_drvFreeExtraFrames(CaptureLink_Obj * pObj)
{
    FVID2_Frame *pFrames;
    Int i,j;
    CaptureLink_ExtraFrameObj *pExtraFrameObj;

    if(pObj->createArgs.numExtraBufs == 0)
        return 0; /* no need to free extra buffers */

    pExtraFrameObj = &pObj->extraFrameObj;

    pFrames = &pExtraFrameObj->frames[0];

    for (i = 0; i < FVID2_MAX_FIELDS; i++)
    {
        for (j = 0; j < FVID2_MAX_PLANES ; j++)
        {
            if (pExtraFrameObj->frames[0].addr[i][j] !=
                pExtraFrameObj->origAddr[0][i][j])
            {
                Vps_printf("CAPTURELINK: !!WARN. Mismatch FrameAddr:%p,OrigAddr:%p\n",
                           pExtraFrameObj->frames[0].addr[i][j], pExtraFrameObj->origAddr[0][i][j]);
                pExtraFrameObj->frames[0].addr[i][j] = pExtraFrameObj->origAddr[0][i][j];
            }
        }
    }


    if(pExtraFrameObj->tilerUsed)
    {
        SystemTiler_freeAll();
    }
    else
    {
        /*
         * free frames for this channel, based on pFormat
         */
        Utils_memFrameFree(&pExtraFrameObj->format, pFrames,
                           pObj->createArgs.numExtraBufs);
    }

    return FVID2_SOK;
}


/*
 * Allocate and queue frames to driver
 *
 * pDrvObj - capture driver information */
Int32 CaptureLink_drvAllocAndQueueFrames(CaptureLink_Obj * pObj,
                                         CaptureLink_InstObj * pDrvObj)
{
    Int32 status;
    UInt16 streamId, chId, frameId, idx;
    Vps_CaptOutInfo *pOutInfo;
    FVID2_Frame *frames;
    System_FrameInfo *pFrameInfo;
    FVID2_FrameList frameList;
    FVID2_Format format;
    Int i,j;
    UInt32 numBufsPerCh;

    /*
     * init frameList for list of frames that are queued per CH to driver
     */
    frameList.perListCfg = NULL;
    frameList.reserved = NULL;

#if 0
    if (CAPTURE_LINK_NUM_BUFS_PER_CH_DEFAULT ==
        pObj->createArgs.numBufsPerCh)
    {
        pObj->createArgs.numBufsPerCh = CAPTURE_LINK_FRAMES_PER_CH;
    }
    else
    {
        /* Bound the numBufsPerCh between min and max supported */
        if (pObj->createArgs.numBufsPerCh < CAPTURE_LINK_MIN_FRAMES_PER_CH)
        {
            Vps_printf("CAPTURE:Create args numBufsPerCh[%d] < min[%d]"
                       "Overriding create args",
                       pObj->createArgs.numBufsPerCh,
                       CAPTURE_LINK_MIN_FRAMES_PER_CH);
            pObj->createArgs.numBufsPerCh = CAPTURE_LINK_MIN_FRAMES_PER_CH;

        }
        else
        {
            if (pObj->createArgs.numBufsPerCh > CAPTURE_LINK_MAX_FRAMES_PER_CH)
            {
                Vps_printf("CAPTURE:Create args numBufsPerCh[%d] > max[%d]"
                           "Overriding create args",
                           pObj->createArgs.numBufsPerCh,
                           CAPTURE_LINK_MAX_FRAMES_PER_CH);
                pObj->createArgs.numBufsPerCh = CAPTURE_LINK_MAX_FRAMES_PER_CH;
            }
        }
    }
#endif

    if (CAPTURE_LINK_NUM_BUFS_PER_CH_DEFAULT == pDrvObj->numBufsPerCh)
    {
        pDrvObj->numBufsPerCh = CAPTURE_LINK_FRAMES_PER_CH;
    }

    /* Bound the numBufsPerCh between min and max supported */
    if (pDrvObj->numBufsPerCh < CAPTURE_LINK_MIN_FRAMES_PER_CH)
    {
        Vps_printf("CAPTURE:Create args numBufsPerCh[%d] < min[%d]"
                   "Overriding create args",
                   pDrvObj->numBufsPerCh,
                   CAPTURE_LINK_MIN_FRAMES_PER_CH);
        pDrvObj->numBufsPerCh = CAPTURE_LINK_MIN_FRAMES_PER_CH;

    }
    else
    {
        if (pDrvObj->numBufsPerCh > CAPTURE_LINK_MAX_FRAMES_PER_CH)
        {
            Vps_printf("CAPTURE:Create args numBufsPerCh[%d] > max[%d]"
                       "Overriding create args",
                       pDrvObj->numBufsPerCh,
                       CAPTURE_LINK_MAX_FRAMES_PER_CH);
            pDrvObj->numBufsPerCh = CAPTURE_LINK_MAX_FRAMES_PER_CH;
        }
    }


    numBufsPerCh = pDrvObj->numBufsPerCh;

    /*
     * for every stream and channel in a capture handle
     */
    for (streamId = 0; streamId < pDrvObj->createArgs.numStream; streamId++)
    {
        for (chId = 0; chId < pDrvObj->numChPerOutput; chId++)
        {

            pOutInfo = &pDrvObj->createArgs.outStreamInfo[streamId];

            /*
             * base index for pDrvObj->frames[] and pDrvObj->frameInfo[]
             */
            idx =
                VPS_CAPT_CH_PER_PORT_MAX * numBufsPerCh *
                streamId + numBufsPerCh * chId;
            if (idx >= CAPTURE_LINK_MAX_FRAMES_PER_HANDLE)
            {
                idx = 0u;
            }

            pFrameInfo = &pDrvObj->frameInfo[idx];
            frames = &pDrvObj->frames[idx];

            /* fill format with channel specific values */
            format.channelNum =
                pDrvObj->createArgs.channelNumMap[streamId][chId];
            format.width = pDrvObj->maxWidth;
            format.height = pDrvObj->maxHeight;

            if(format.height < CAPTURE_LINK_HEIGHT_MIN_LINES)
                format.height = CAPTURE_LINK_HEIGHT_MIN_LINES;

            format.pitch[0] = pOutInfo->pitch[0];
            format.pitch[1] = pOutInfo->pitch[1];
            format.pitch[2] = pOutInfo->pitch[2];
            format.fieldMerged[0] = FALSE;
            format.fieldMerged[1] = FALSE;
            format.fieldMerged[2] = FALSE;
            format.dataFormat = pOutInfo->dataFormat;
            format.scanFormat = FVID2_SF_PROGRESSIVE;
            format.bpp = FVID2_BPP_BITS8;                  /* ignored */

            if (format.dataFormat == FVID2_DF_RAW_VBI)
            {
                format.height = CAPTURE_LINK_RAW_VBI_LINES;
            }

            /*
             * alloc memory based on 'format'
             * Allocated frame info is put in frames[]
             * CAPTURE_LINK_APP_FRAMES_PER_CH is the number of buffers per channel to
             * allocate
             */
            if (CaptureLink_drvIsDataFormatTiled
                (&pDrvObj->createArgs, streamId))
            {
                status = Utils_tilerFrameAlloc(&format, frames,
                                      numBufsPerCh);
            }
            else
            {
                #ifdef SYSTEM_DEBUG_MEMALLOC
                Vps_printf("CAPTURE:ALLOCINFO:FMT[%d]/PITCH[%d]/HEIGHT[%d]/NUMBUFS[%d]",
                           format.dataFormat,
                           format.pitch[0],
                           format.height,
                           numBufsPerCh);
                #endif
                status = Utils_memFrameAlloc(&format, frames,
                                    numBufsPerCh);
            }
            UTILS_assert(status == FVID2_SOK);

            /*
             * Set rtParams for every frame in perFrameCfg
             */
            for (frameId = 0; frameId < numBufsPerCh; frameId++)
            {
                frames[frameId].perFrameCfg =
                    &pFrameInfo[frameId].captureRtParams;
                frames[frameId].subFrameInfo = NULL;
                frames[frameId].appData = &pFrameInfo[frameId];
                frames[frameId].reserved = NULL;
                for (i = 0; i < FVID2_MAX_FIELDS; i++)
                {
                    for (j = 0; j < FVID2_MAX_PLANES ; j++)
                    {
                        pDrvObj->origAddr[idx + frameId][i][j] =
                            frames[frameId].addr[i][j];
                    }
                }
                pFrameInfo[frameId].captureChannelNum =
                    frames[frameId].channelNum;
                pFrameInfo[frameId].rtChInfoUpdate = FALSE;

                frameList.frames[frameId] = &frames[frameId];

#ifdef SYSTEM_VERBOSE_PRINTS
                if (pDrvObj->instId == 0 && streamId == 0 && chId == 0)
                {
                    Vps_rprintf(" %d: CAPTURE: %d: 0x%08x, %d x %d, %08x B\n",
                                Utils_getCurTimeInMsec(),
                                frameId, frames[frameId].addr[0][0],
                                format.pitch[0] / 2, format.height,
                                format.height * format.pitch[0]);
                }
#endif
            }
#ifdef SYSTEM_VERBOSE_PRINTS
            if (pDrvObj->instId == 0 && streamId == 0 && chId == 0)
            {
                Vps_rprintf(" %d: CAPTURE: 0x%08x %08x B\n",
                            Utils_getCurTimeInMsec(),
                            frames[0].addr[0][0],
                            format.height * format.pitch[0] * frameId);
            }
#endif

            /*
             * Set number of frame in frame list
             */
            frameList.numFrames = numBufsPerCh;

            /*
             * queue the frames in frameList
             * All allocate frames are queued here as an example.
             * In general atleast 2 frames per channel need to queued
             * before starting capture,
             * else frame will get dropped until frames are queued
             */
            status =
                FVID2_queue(pDrvObj->captureVipHandle, &frameList, streamId);
            UTILS_assert(status == FVID2_SOK);
            pObj->captureQueuedFrameCount += frameList.numFrames;
        }
    }

    return FVID2_SOK;
}

/*
 * Free allocated frames
 *
 * pDrvObj - capture driver information */
Int32 CaptureLink_drvFreeFrames(CaptureLink_Obj * pObj,
                                CaptureLink_InstObj * pDrvObj)
{
    UInt32 idx;
    UInt16 streamId, chId;
    FVID2_Format format;
    FVID2_Frame *pFrames;
    Vps_CaptOutInfo *pOutInfo;
    UInt32 tilerUsed = FALSE;
    Int i,j;
    UInt32 numBufsPerCh;

    numBufsPerCh = pDrvObj->numBufsPerCh;

    for (streamId = 0; streamId < pDrvObj->createArgs.numStream; streamId++)
    {
        for (chId = 0; chId < pDrvObj->numChPerOutput; chId++)
        {
            pOutInfo = &pDrvObj->createArgs.outStreamInfo[streamId];

            idx = VPS_CAPT_CH_PER_PORT_MAX *
                numBufsPerCh * streamId +
                numBufsPerCh * chId;

            if (idx >= CAPTURE_LINK_MAX_FRAMES_PER_HANDLE)
            {
                idx = 0u;
            }

            pFrames = &pDrvObj->frames[idx];

            for (i = 0; i < FVID2_MAX_FIELDS; i++)
            {
                for (j = 0; j < FVID2_MAX_PLANES ; j++)
                {
                    if (pDrvObj->frames[idx].addr[i][j] !=
                        pDrvObj->origAddr[idx][i][j])
                    {
                        Vps_printf("CAPTURELINK: !!WARN. Mismatch FrameAddr:%p,OrigAddr:%p Idx %d, ixj %dx%d DriverInst %d\n",
                                   pDrvObj->frames[idx].addr[i][j], pDrvObj->origAddr[idx][i][j],
                                   idx, i, j, pDrvObj->instId);
                        pDrvObj->frames[idx].addr[i][j] = pDrvObj->origAddr[idx][i][j];
                    }
                }
            }

            /* fill format with channel specific values */
            format.channelNum =
                pDrvObj->createArgs.channelNumMap[streamId][chId];
            format.width = pDrvObj->maxWidth;
            format.height = pDrvObj->maxHeight;

            if(format.height < CAPTURE_LINK_HEIGHT_MIN_LINES)
                format.height = CAPTURE_LINK_HEIGHT_MIN_LINES;

            format.pitch[0] = pOutInfo->pitch[0];
            format.pitch[1] = pOutInfo->pitch[1];
            format.pitch[2] = pOutInfo->pitch[2];
            format.fieldMerged[0] = FALSE;
            format.fieldMerged[1] = FALSE;
            format.fieldMerged[2] = FALSE;
            format.dataFormat = pOutInfo->dataFormat;
            format.scanFormat = FVID2_SF_PROGRESSIVE;
            format.bpp = FVID2_BPP_BITS8;                  /* ignored */

            if (format.dataFormat == FVID2_DF_RAW_VBI)
            {
                format.height = CAPTURE_LINK_RAW_VBI_LINES;
            }

            if (CaptureLink_drvIsDataFormatTiled
                (&pDrvObj->createArgs, streamId))
            {
                /*
                 * cannot free tiled frames
                 */
                tilerUsed = TRUE;
            }
            else
            {
                /*
                 * free frames for this channel, based on pFormat
                 */
                Utils_memFrameFree(&format, pFrames,
                                   numBufsPerCh);
            }
        }
    }

    if (tilerUsed)
    {
        SystemTiler_freeAll();
    }

    return FVID2_SOK;
}

UInt32 CaptureLink_drvIsDataFormatTiled(Vps_CaptCreateParams * createArgs,
                                        UInt16 streamId)
{
    Vps_CaptOutInfo *pOutInfo;

    pOutInfo = &createArgs->outStreamInfo[streamId];

    if ((pOutInfo->dataFormat == FVID2_DF_YUV420SP_UV ||
         pOutInfo->dataFormat == FVID2_DF_YUV422SP_UV)
        && pOutInfo->memType == VPS_VPDMA_MT_TILEDMEM)
    {
        return TRUE;
    }

    return FALSE;
}

/*
 * Init create arguments to default values
 *
 * createArgs - create arguments */
Int32 CaptureLink_drvInitCreateArgs(Vps_CaptCreateParams * createArgs)
{
    UInt16 chId, streamId;
    Vps_CaptOutInfo *pOutInfo;
    Vps_CaptScParams *pScParams;

    memset(createArgs, 0, sizeof(*createArgs));

    createArgs->videoCaptureMode =
        VPS_CAPT_VIDEO_CAPTURE_MODE_SINGLE_CH_NON_MUX_EMBEDDED_SYNC;

    createArgs->videoIfMode = VPS_CAPT_VIDEO_IF_MODE_8BIT;

    createArgs->inDataFormat = FVID2_DF_YUV422P;
    createArgs->periodicCallbackEnable = FALSE;
    createArgs->numCh = 1;
    createArgs->numStream = 1;

    createArgs->vipParserInstConfig = NULL;
    createArgs->vipParserPortConfig = NULL;
    createArgs->cscConfig = NULL;

    pScParams = &createArgs->scParams;

    pScParams->inScanFormat = FVID2_SF_PROGRESSIVE;
    pScParams->inWidth = 360;
    pScParams->inHeight = 240;
    pScParams->inCropCfg.cropStartX = 0;
    pScParams->inCropCfg.cropStartY = 0;
    pScParams->inCropCfg.cropWidth = pScParams->inWidth;
    pScParams->inCropCfg.cropHeight = pScParams->inHeight;
    pScParams->outWidth = pScParams->inWidth;
    pScParams->outHeight = pScParams->inHeight;
    pScParams->scConfig = NULL;
    pScParams->scCoeffConfig = NULL;

    for (streamId = 0; streamId < VPS_CAPT_STREAM_ID_MAX; streamId++)
    {
        pOutInfo = &createArgs->outStreamInfo[streamId];

        pOutInfo->memType = VPS_VPDMA_MT_NONTILEDMEM;
        pOutInfo->maxOutHeight = VPS_CAPT_MAX_OUT_HEIGHT_UNLIMITED;
        pOutInfo->maxOutWidth  = VPS_CAPT_MAX_OUT_WIDTH_UNLIMITED;

        pOutInfo->dataFormat = FVID2_DF_INVALID;

        pOutInfo->scEnable = FALSE;
        pOutInfo->subFrameModeEnable = FALSE;
        pOutInfo->numLinesInSubFrame = 0;
        pOutInfo->subFrameCb = NULL;

        if (streamId == 0)
        {
            pOutInfo->dataFormat = FVID2_DF_YUV422I_YUYV;
        }

        for (chId = 0; chId < VPS_CAPT_CH_PER_PORT_MAX; chId++)
        {
            createArgs->channelNumMap[streamId][chId] =
                Vps_captMakeChannelNum(0, streamId, chId);
        }
    }

    return 0;
}

Int32 CaptureLink_getCpuLoad()
{
    gCaptureLink_obj.totalCpuLoad += Load_getCPULoad();
    gCaptureLink_obj.cpuLoadCount++;

    return 0;
}

Int32 CaptureLink_drvPrintStatus(CaptureLink_Obj * pObj)
{
    UInt32 fps;

    FVID2_control(pObj->fvidHandleVipAll,
                  IOCTL_VPS_CAPT_PRINT_ADV_STATISTICS,
                  (Ptr) pObj->exeTime, NULL);

    fps = (pObj->captureDequeuedFrameCount * 100) / (pObj->exeTime / 10);

    Vps_printf(" %d: CAPTURE: Fields = %d (fps = %d, CPU Load = %d)\r\n",
               Utils_getCurTimeInMsec(),
               pObj->captureDequeuedFrameCount,
               fps, pObj->totalCpuLoad / pObj->cpuLoadCount);

    Vps_printf(" %d: CAPTURE: Num Resets = %d (Avg %d ms per reset)\r\n",
               Utils_getCurTimeInMsec(),
               pObj->resetCount, pObj->resetTotalTime / pObj->resetCount);

    System_memPrintHeapStatus();

    return 0;
}

Int32 CaptureLink_printBufferStatus(CaptureLink_Obj * pObj)
{
    Uint8 i, str[256];

    Vps_rprintf(
        " \n"
        " *** CAPTURE Statistics *** \n"
        "%d: CAPTURE: Queued to driver = %d, Dequeued from driver = %d\r\n",
        Utils_getCurTimeInMsec(), pObj->captureQueuedFrameCount, pObj->captureDequeuedFrameCount);

    for (i=0; i<1; i++)
//        for (i=0; i<CAPTURE_LINK_MAX_OUT_QUE; i++)
    {
        sprintf ((char *) str, "CAPTURE [%d]", i);
        Utils_bufPrintStatus(str, &pObj->bufQue[i]);
    }
    return 0;
}

Int32 CaptureLink_drvEnableWrBkCapt(CaptureLink_Obj * pObj, CaptureLink_CtrlWrbkCapt *pPrm)
{
    Int32 status;
    UInt32 instId;
    CaptureLink_InstObj *pInst;

    instId = pObj->chObj[pPrm->queId][pPrm->chId].instId;
    pInst = &pObj->instObj[instId];

    if (TRUE == pPrm->enable)
    {
        status = FVID2_start(pInst->captureVipHandle, NULL);
    }
    else
    {
        status = FVID2_stop(pInst->captureVipHandle, NULL);
    }

    return (status);
}

Int32 CaptureLink_drvSetScParams(CaptureLink_Obj * pObj, CaptureLink_ScParams *pPrm)
{
    Int32 status;
    UInt32 instId;
    CaptureLink_InstObj *pInst;
    Vps_CaptScParams scParams;
    CaptureLink_SetResolution resPrm;

    scParams.inScanFormat = FVID2_SF_PROGRESSIVE; // NOT USED
    scParams.inWidth = pPrm->inWidth;
    scParams.inHeight = pPrm->inHeight;
    scParams.inCropCfg.cropStartX = 0;
    scParams.inCropCfg.cropStartY = 0;
    scParams.inCropCfg.cropWidth = (scParams.inWidth & 0xFFFE);
    scParams.inCropCfg.cropHeight = (scParams.inHeight & 0xFFFE);
    scParams.outWidth = pPrm->outWidth;
    scParams.outHeight = pPrm->outHeight;
    scParams.scConfig = NULL;
    scParams.scCoeffConfig = NULL;

    instId = pObj->chObj[pPrm->queId][pPrm->chId].instId;
    pInst = &pObj->instObj[instId];

    status = FVID2_control(
                pInst->captureVipHandle,
                IOCTL_VPS_CAPT_SET_SC_PARAMS,
                &scParams,
                NULL);

    resPrm.queId = pPrm->queId;
    resPrm.width = scParams.outWidth;
    resPrm.height = scParams.outHeight;
    CaptureLink_drvSetResolution(pObj, &resPrm);

    return (status);
}

