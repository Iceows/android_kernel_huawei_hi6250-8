/*
* Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
* foss@huawei.com
*
* If distributed as part of the Linux kernel, the following license terms
* apply:
*
* * This program is free software; you can redistribute it and/or modify
* * it under the terms of the GNU General Public License version 2 and
* * only version 2 as published by the Free Software Foundation.
* *
* * This program is distributed in the hope that it will be useful,
* * but WITHOUT ANY WARRANTY; without even the implied warranty of
* * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* * GNU General Public License for more details.
* *
* * You should have received a copy of the GNU General Public License
* * along with this program; if not, write to the Free Software
* * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
*
* Otherwise, the following license terms apply:
*
* * Redistribution and use in source and binary forms, with or without
* * modification, are permitted provided that the following conditions
* * are met:
* * 1) Redistributions of source code must retain the above copyright
* *    notice, this list of conditions and the following disclaimer.
* * 2) Redistributions in binary form must reproduce the above copyright
* *    notice, this list of conditions and the following disclaimer in the
* *    documentation and/or other materials provided with the distribution.
* * 3) Neither the name of Huawei nor the names of its contributors may
* *    be used to endorse or promote products derived from this software
* *    without specific prior written permission.
*
* * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*/

/*****************************************************************************
  1 
*****************************************************************************/
#include "RnicCtx.h"
#include "RnicEntity.h"
#include "PsCommonDef.h"
#include "AtRnicInterface.h"
#include "RnicProcMsg.h"
#include "RnicDemandDialFileIO.h"
#include "RnicLog.h"
#include "RnicDebug.h"
#include "TafNvInterface.h"




/*****************************************************************************
    .C
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_RNIC_CTX_C


/*****************************************************************************
  2 
*****************************************************************************/

/* RNIC CTX,RNIC */
RNIC_CTX_STRU                           g_stRnicCtx;

extern RNIC_STATS_INFO_STRU                    g_astRnicStats[RNIC_NET_ID_MAX_NUM];


/*****************************************************************************
     : RNIC_RnicCtxInit
   : RNICCTX
   : pstRnicCtx:RNIC
   : 
     : VOS_VOID
   :
   :

      :
 1.       : 20111206
          : f00179208
      : 
 2.       : 2012605
          : z60575
      : DTS2012060508406
 3.       : 20121123
          : f00179208
      : DSDA Phase I: RNIC
 3.       : 20130415
          : f00179208
      : C:
 4.       : 201307222
          : j00177245
      : warning
 5.       : 20140214
          : m00217266
      : L-C
 6.       : 201463
          : m00217266
      : DTS2014052902453
 7.       : 20150731
          : w00316404
      : DTS2015071401227
 8.       : 20161108
          : l00373346
      : DTS2016062107764
 9.       : 20170321
          : l00373346
      : RNICNAP+GRO
*****************************************************************************/
VOS_VOID RNIC_InitCtx(
    RNIC_CTX_STRU                      *pstRnicCtx
)
{
    VOS_UINT8                           ucIndex;

    TAF_MEM_SET_S(g_astRnicStats, sizeof(g_astRnicStats), 0x00, sizeof(g_astRnicStats));

    /* Added by m00217266 for L-C, 2014-2-19, begin */
    TAF_MEM_SET_S((VOS_VOID *)&g_stRnicRmnetConfigCheckInfo, sizeof(g_stRnicRmnetConfigCheckInfo), 0x00, sizeof(g_stRnicRmnetConfigCheckInfo));
    /* Added by m00217266 for L-C, 2014-2-19, end */


    for (ucIndex = 0 ; ucIndex < RNIC_NET_ID_MAX_NUM ; ucIndex++)
    {
        /* RNIC */
        RNIC_ClearNetDsFlowStats(ucIndex);

        /* RNIC PDP */
        RNIC_InitPdpCtx(&(pstRnicCtx->astSpecCtx[ucIndex].stPdpCtx), ucIndex);

        /*  */
        pstRnicCtx->astSpecCtx[ucIndex].enFlowCtrlStatus = RNIC_FLOW_CTRL_STATUS_STOP;

        /* ID */
        /* MODEM */
        pstRnicCtx->astSpecCtx[ucIndex].enModemId        = RNIC_GET_MODEM_ID_BY_NET_ID(ucIndex);

        /* Modified by l60609 for L-C, 2014-1-14, begin */
        pstRnicCtx->astSpecCtx[ucIndex].enModemType      = RNIC_MODEM_TYPE_INSIDE;
        /* Modified by l60609 for L-C, 2014-1-14, end */

        pstRnicCtx->astSpecCtx[ucIndex].enRmNetId        = ucIndex;
        pstRnicCtx->astSpecCtx[ucIndex].lSpePort         = RNIC_INVALID_SPE_PORT;
        pstRnicCtx->astSpecCtx[ucIndex].ulIpfPortFlg     = VOS_FALSE;

        pstRnicCtx->astSpecCtx[ucIndex].enRatType        = IMSA_RNIC_IMS_RAT_TYPE_BUTT;

    }

    /* RABID */
    for (ucIndex = 0 ; ucIndex < RNIC_MODEM_ID_MAX_NUM ; ucIndex++)
    {
        RNIC_InitRabidInfo(&pstRnicCtx->astRabIdInfo[ucIndex]);
    }

    /* Added by m00217266 for L-C, 2014-1-27, begin */
    /* PDNID */
    RNIC_InitPdnIdInfo(&pstRnicCtx->stPdnIdInfo);
    /* Added by m00217266 for L-C, 2014-1-27, end */

    /* RNIC */
    RNIC_InitAllTimers(pstRnicCtx->astTimerCtx);

    /*  */
    RNIC_InitDialMode(&(pstRnicCtx->stDialMode));

    /*  */
    RNIC_ClearTiDialDownExpCount();

    RNIC_SetTimer4WakeFlg(VOS_FALSE);

    /*  */
    RNIC_InitResetSem();

    /* IPF */
    RNIC_InitIpfMode(pstRnicCtx);

    return;
}

/*****************************************************************************
     : RNIC_InitRabidInfo
   : Rabid
   : pstRabIdInfo:RabId
   : 
     : VOS_VOID
   :
   :

      :
 1.       : 20121210
          : f00179208
      : 

*****************************************************************************/
VOS_VOID RNIC_InitRabidInfo(
    RNIC_RABID_INFO_STAU               *pstRabIdInfo
)
{

    TAF_MEM_SET_S(pstRabIdInfo->aucRmNetId,
               sizeof(pstRabIdInfo->aucRmNetId),
               RNIC_RMNET_ID_BUTT,
               RNIC_RAB_ID_MAX_NUM);

    return;
}

/* Added by m00217266 for L-C, 2014-2-8, begin */
/*****************************************************************************
     : RNIC_InitPdnIdInfo
   : Pdnid
   : pstPdnIdInfo:PdnId
   : 
     : VOS_VOID
   :
   :

      :
 1.       : 20140208
          : m00217266
      : 

*****************************************************************************/
VOS_VOID RNIC_InitPdnIdInfo(
    RNIC_PDNID_INFO_STAU               *pstPdnIdInfo
)
{

    TAF_MEM_SET_S(pstPdnIdInfo->aucRmNetId,
               sizeof(pstPdnIdInfo->aucRmNetId),
               RNIC_RMNET_ID_BUTT,
               RNIC_PDN_ID_MAX);

    return;
}
/* Added by m00217266 for L-C, 2014-2-8, end */


/*****************************************************************************
     : RNIC_InitPdpCtx
   : PDP
   : pstPdpCtx:PDP
             ucRmNetId:ID
   : 
     : VOS_VOID
   :
   :

      :
 1.       : 20111212
          : f00179208
      : 
 2.       : 20121123
          : f00179208
      : DSDA Phase I: RNIC
*****************************************************************************/
VOS_VOID RNIC_InitPdpCtx(
    RNIC_PDP_CTX_STRU                  *pstPdpCtx,
    VOS_UINT8                           ucRmNetId
)
{
    RNIC_InitIpv4PdpCtx(&(pstPdpCtx->stIpv4PdpInfo));
    RNIC_InitIpv6PdpCtx(&(pstPdpCtx->stIpv6PdpInfo));
    RNIC_InitIpv4v6PdpCtx(&(pstPdpCtx->stIpv4v6PdpInfo), ucRmNetId);

    IMM_ZcQueueHeadInit(&pstPdpCtx->stImsQue);

    return;
}

/*****************************************************************************
     : RNIC_ResetDialMode
   : 
   : pstDialMode:
   : 
     : VOS_VOID
   :
   :

      :
 1.       : 20130415
          : f00179208
      : 
*****************************************************************************/
VOS_VOID RNIC_ResetDialMode(
    RNIC_DIAL_MODE_STRU                *pstDialMode
)
{
    pstDialMode->enDialMode             = RNIC_DIAL_MODE_MANUAL;
    pstDialMode->ulIdleTime             = RNIC_DIAL_DEMA_IDLE_TIME;
    pstDialMode->enEventReportFlag      = RNIC_FORBID_EVENT_REPORT;

    return;
}

/*****************************************************************************
     : RNIC_InitDialMode
   : 
   : pstDialMode:
   : 
     : VOS_VOID
   :
   :

      :
 1.       : 20111212
          : f00179208
      : 
 2.       : 2012131
          : w00199382
      : /proc/dial
*****************************************************************************/
VOS_VOID RNIC_InitDialMode(
    RNIC_DIAL_MODE_STRU                *pstDialMode
)
{
    VOS_UINT32                          ulRlst;

    pstDialMode->enDialMode             = RNIC_DIAL_MODE_MANUAL;
    pstDialMode->ulIdleTime             = RNIC_DIAL_DEMA_IDLE_TIME;
    pstDialMode->enEventReportFlag      = RNIC_FORBID_EVENT_REPORT;

    ulRlst                              = RNIC_InitDemandDialFile();

    if ( VOS_ERR == ulRlst)
    {
       RNIC_ERROR_LOG(ACPU_PID_RNIC, "RNIC_InitDialMode:RNIC_InitDemandDialFile ERR!");
    }

    return;
}

/*****************************************************************************
     : RNIC_InitIpv4PdpCtx
   : IPV4 PDP
   : pstIpv4PdpCtx:IPV4 PDP
   : 
     : VOS_VOID
   :
   :

      :
 1.       : 20111219
          : f00179208
      : 

*****************************************************************************/
VOS_VOID RNIC_InitIpv4PdpCtx(
    RNIC_IPV4_PDP_INFO_STRU            *pstIpv4PdpCtx
)
{
    pstIpv4PdpCtx->enRegStatus = RNIC_PDP_REG_STATUS_DEACTIVE;
    /* Modified by l60609 for L-C, 2014-1-14, begin */
    pstIpv4PdpCtx->ucRabId     = RNIC_RAB_ID_INVALID;
    pstIpv4PdpCtx->ucPdnId     = RNIC_PDN_ID_INVALID;
    /* Modified by l60609 for L-C, 2014-1-14, end */
    pstIpv4PdpCtx->ulIpv4Addr  = 0;


    return;
}

/*****************************************************************************
     : RNIC_InitIpv6PdpCtx
   : IPV6 PDP
   : pstIpv4PdpCtx:IPV6 PDP
   : 
     : VOS_VOID
   :
   :

      :
 1.       : 20111219
          : f00179208
      : 

*****************************************************************************/
VOS_VOID RNIC_InitIpv6PdpCtx(
    RNIC_IPV6_PDP_INFO_STRU            *pstIpv6PdpCtx
)
{
    pstIpv6PdpCtx->enRegStatus = RNIC_PDP_REG_STATUS_DEACTIVE;
    /* Modified by l60609 for L-C, 2014-1-14, begin */
    pstIpv6PdpCtx->ucRabId     = RNIC_RAB_ID_INVALID;
    pstIpv6PdpCtx->ucPdnId     = RNIC_PDN_ID_INVALID;
    /* Modified by l60609 for L-C, 2014-1-14, end */

    TAF_MEM_SET_S(pstIpv6PdpCtx->aucIpv6Addr,
               sizeof(pstIpv6PdpCtx->aucIpv6Addr),
               0x00,
               RNIC_MAX_IPV6_ADDR_LEN);


    return;
}

/*****************************************************************************
     : RNIC_InitIpv4v6PdpCtx
   : IPV4V6 PDP
   : pstIpv4v6PdpCtx:IPV4V6 PDP
             ucRmNetId:ID
   : 
     : VOS_VOID
   :
   :

      :
 1.       : 20111219
          : f00179208
      : 
 2.       : 20121123
          : f00179208
      : DSDA Phase I: RNIC
*****************************************************************************/
VOS_VOID RNIC_InitIpv4v6PdpCtx(
    RNIC_IPV4V6_PDP_INFO_STRU          *pstIpv4v6PdpCtx,
    VOS_UINT8                           ucRmNetId
)
{
    RNIC_PDP_CTX_STRU                  *pstPdpAddr;

    /* PDP */
    pstPdpAddr                              = RNIC_GetPdpCtxAddr(ucRmNetId);

    /* Modified by l60609 for L-C, 2014-1-14, begin */
    pstIpv4v6PdpCtx->enRegStatus            = RNIC_PDP_REG_STATUS_DEACTIVE;
    pstIpv4v6PdpCtx->ucRabId                = RNIC_RAB_ID_INVALID;
    pstIpv4v6PdpCtx->ucPdnId                = RNIC_PDN_ID_INVALID;
    /* Modified by l60609 for L-C, 2014-1-14, end */

    pstPdpAddr->stIpv4PdpInfo.ucRabId       = RNIC_RAB_ID_INVALID;
    pstPdpAddr->stIpv6PdpInfo.ucRabId       = RNIC_RAB_ID_INVALID;

    pstIpv4v6PdpCtx->ulIpv4Addr             = 0;

    TAF_MEM_SET_S(pstIpv4v6PdpCtx->aucIpv6Addr,
               sizeof(pstIpv4v6PdpCtx->aucIpv6Addr),
               0x00,
               RNIC_MAX_IPV6_ADDR_LEN);


}

/*****************************************************************************
     : RNIC_InitResetSem
   : RNIC
   : 
   : 
     : VOS_VOID
   :
   :

      :
 1.       : 20130415
          : f00179208
      : 
*****************************************************************************/
VOS_VOID RNIC_InitResetSem(VOS_VOID)
{
    g_stRnicCtx.hResetSem  = VOS_NULL_PTR;

    /*  */
    if (VOS_OK != VOS_SmBCreate( "RNIC", 0, VOS_SEMA4_FIFO, &g_stRnicCtx.hResetSem))
    {
        PS_PRINTF("Create rnic acpu cnf sem failed!\r\n");
        RNIC_DBG_SET_SEM_INIT_FLAG(VOS_FALSE);
        RNIC_DBG_CREATE_BINARY_SEM_FAIL_NUM(1);

        return;
    }
    else
    {
        RNIC_DBG_SAVE_BINARY_SEM_ID(g_stRnicCtx.hResetSem);
    }

    RNIC_DBG_SET_SEM_INIT_FLAG(VOS_TRUE);

    return;
}

/*****************************************************************************
     : RNIC_InitIpfMode
   : IPF
   : *pstRnicCtx --- RNC
   : 
     : VOS_VOID
   :
   :

      :
 1.       : 2011108
          : l00373346
      : 
*****************************************************************************/
VOS_VOID RNIC_InitIpfMode(
    RNIC_CTX_STRU                      *pstRnicCtx
)
{
    TAF_NV_ADS_IPF_MODE_CFG_STRU        stIpfMode;

    TAF_MEM_SET_S(&stIpfMode, (VOS_SIZE_T)sizeof(stIpfMode), 0x00, (VOS_SIZE_T)sizeof(TAF_NV_ADS_IPF_MODE_CFG_STRU));

    pstRnicCtx->ucIpfMode = 0;

    /* NV */
    if (NV_OK == NV_ReadEx(MODEM_ID_0, en_NV_Item_ADS_IPF_MODE_CFG,
                      &stIpfMode, (VOS_UINT32)sizeof(TAF_NV_ADS_IPF_MODE_CFG_STRU)))
    {
        pstRnicCtx->ucIpfMode = stIpfMode.ucIpfMode;
    }

    return;
}



/*****************************************************************************
     : RNIC_ClearNetDsFlowStats
   : 
   : enRmNetId --- RMNET ID
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 2015330
           : A00165503
       : 
*****************************************************************************/
VOS_VOID RNIC_ClearNetDsFlowStats(RNIC_RMNET_ID_ENUM_UINT8 enRmNetId)
{
    RNIC_SPEC_CTX_STRU                 *pstNetCntxt = VOS_NULL_PTR;

    pstNetCntxt = RNIC_GET_SPEC_NET_CTX(enRmNetId);

    pstNetCntxt->stDsFlowStats.ulCurrentRecvRate   = 0;
    pstNetCntxt->stDsFlowStats.ulPeriodRecvPktNum  = 0;
    pstNetCntxt->stDsFlowStats.ulTotalRecvFluxLow  = 0;
    pstNetCntxt->stDsFlowStats.ulTotalRecvFluxHigh = 0;

    pstNetCntxt->stDsFlowStats.ulCurrentSendRate   = 0;
    pstNetCntxt->stDsFlowStats.ulPeriodSendPktNum  = 0;
    pstNetCntxt->stDsFlowStats.ulTotalSendFluxLow  = 0;
    pstNetCntxt->stDsFlowStats.ulTotalSendFluxHigh = 0;

    return;
}

/*****************************************************************************
     : RNIC_GetRnicCtxAddr
   : RNICCTX
   : 
   : 
     : RNICCTX
   :
   :

      :
 1.       : 20111206
          : f00179208
      : 

*****************************************************************************/
RNIC_CTX_STRU* RNIC_GetRnicCtxAddr(VOS_VOID)
{
    return &(g_stRnicCtx);
}

/*****************************************************************************
     : RNIC_GetRnicTiDialDownExpCount
   : 
   : 
   : 
     : 
   :
   :

      :
 1.       : 20111207
          : f00179208
      : 

*****************************************************************************/
VOS_UINT32 RNIC_GetTiDialDownExpCount( VOS_VOID)
{
    return (g_stRnicCtx.ulTiDialDownExpCount);
}


/*****************************************************************************
     : RNIC_TiDialDownExpCountPlusOne
   : 
   : 
   : 
     : VOS_VOID
   :
   :

      :
 1.       : 20111207
          : f00179208
      : 

*****************************************************************************/
VOS_VOID RNIC_IncTiDialDownExpCount( VOS_VOID)
{
    (g_stRnicCtx.ulTiDialDownExpCount)++;

    return;
}

/*****************************************************************************
     : RNIC_ClearRnicTiDialDownExpCount
   : 
   : 
   : 
     : VOS_VOID
   :
   :

      :
 1.       : 20111207
          : f00179208
      : 

*****************************************************************************/
VOS_VOID RNIC_ClearTiDialDownExpCount( VOS_VOID)
{
    g_stRnicCtx.ulTiDialDownExpCount = 0;

    return;
}

/*****************************************************************************
     : RNIC_GetCurrentUlRate
   : 
   : ucRmNetId: ID
   : 
     : RNIC
   :
   :

      :
 1.       : 20111207
          : f00179208
      : 
 2.       : 20121123
          : f00179208
      : DSDA Phase I: RNIC
*****************************************************************************/
VOS_UINT32 RNIC_GetCurrentUlRate(VOS_UINT8 ucRmNetId)
{
    return (g_stRnicCtx.astSpecCtx[ucRmNetId].stDsFlowStats.ulCurrentSendRate);
}

/*****************************************************************************
     : RNIC_SetCurrentUlRate
   : 
   : ulULDataRate:
             ucRmNetId: ID
   : 
     : VOS_VOID
   :
   :

      :
 1.       : 20111213
          : f00179208
      : 
 2.       : 20121123
          : f00179208
      : DSDA Phase I: RNIC
*****************************************************************************/
VOS_VOID RNIC_SetCurrentUlRate(
    VOS_UINT32                          ulULDataRate,
    VOS_UINT8                           ucRmNetId
)
{
    g_stRnicCtx.astSpecCtx[ucRmNetId].stDsFlowStats.ulCurrentSendRate = ulULDataRate;

    return;
}


/*****************************************************************************
     : RNIC_GetCurrentDlRate
   : 
   : ucRmNetId: ID
   : 
     : RNIC
   :
   :

      :
 1.       : 20111207
          : f00179208
      : 
 2.       : 20121123
          : f00179208
      : DSDA Phase I: RNIC
*****************************************************************************/
VOS_UINT32 RNIC_GetCurrentDlRate(VOS_UINT8 ucRmNetId)
{
    return (g_stRnicCtx.astSpecCtx[ucRmNetId].stDsFlowStats.ulCurrentRecvRate);
}

/*****************************************************************************
     : RNIC_SetCurrentDlRate
   : 
   : ulDLDataRate:
             ucRmNetId: ID
   : 
     : VOS_VOID
   :
   :

      :
 1.       : 20111207
          : f00179208
      : 
 2.       : 20121123
          : f00179208
      : DSDA Phase I: RNIC
*****************************************************************************/
VOS_VOID RNIC_SetCurrentDlRate(
    VOS_UINT32                          ulDLDataRate,
    VOS_UINT8                           ucRmNetId
)
{
    g_stRnicCtx.astSpecCtx[ucRmNetId].stDsFlowStats.ulCurrentRecvRate = ulDLDataRate;

    return;
}

/*****************************************************************************
     : RNIC_GetDialModeAddr
   : 
   : 
   : 
     : 
   :
   :

      :
 1.       : 20111207
          : f00179208
      : 
*****************************************************************************/
RNIC_DIAL_MODE_STRU* RNIC_GetDialModeAddr(VOS_VOID)
{
    return &(g_stRnicCtx.stDialMode);
}

/*****************************************************************************
     : RNIC_GetPdpCtxAddr
   : RNICPDP
   : ucRmNetId -- 
   : 
     : RNICPDP
   :
   :

      :
 1.       : 20111207
          : f00179208
      : 
 2.       : 20121123
          : f00179208
      : DSDA Phase I: RNIC
*****************************************************************************/
RNIC_PDP_CTX_STRU* RNIC_GetPdpCtxAddr(VOS_UINT8 ucRmNetId)
{
    return &(g_stRnicCtx.astSpecCtx[ucRmNetId].stPdpCtx);
}

/*****************************************************************************
     : RNIC_GetTimerAddr
   : RNIC CTX
   : 
   : 
     : 
   :
   :

      :
 1.       : 20111212
          : f00179208
      : 

*****************************************************************************/
RNIC_TIMER_CTX_STRU*  RNIC_GetTimerAddr( VOS_VOID )
{
    return g_stRnicCtx.astTimerCtx;
}

/*****************************************************************************
     : RNIC_GetTimer4WakeFlg
   : RNIC CTXTimer4
   : 
   : 
     : 
   :
   :

      :
 1.       : 201265
          : z60575
      : DTS2012060508406, 

*****************************************************************************/
VOS_UINT32 RNIC_GetTimer4WakeFlg(VOS_VOID)
{
    return g_stRnicCtx.ulSetTimer4WakeFlg;
}

/*****************************************************************************
     : RNIC_SetTimer4WakeFlg
   : RNIC CTXTimer4
   : 
   : 
     : 
   :
   :

      :
 1.       : 201265
          : z60575
      : DTS2012060508406, 

*****************************************************************************/
VOS_VOID RNIC_SetTimer4WakeFlg(VOS_UINT32 ulFlg)
{
    g_stRnicCtx.ulSetTimer4WakeFlg = ulFlg;
}

/*****************************************************************************
     : RNIC_GetSpecNetCardCtxAddr
   : RNIC
   : ucIndex -- 
   : 
     : RNIC
   :
   :

      :
 1.       : 20121124
          : f00179208
      : 
*****************************************************************************/
RNIC_SPEC_CTX_STRU *RNIC_GetSpecNetCardCtxAddr(VOS_UINT8 ucRmNetId)
{
    return &(g_stRnicCtx.astSpecCtx[ucRmNetId]);
}

/*****************************************************************************
     : RNIC_GetNetCntxtByRmNetId
   : RMNETID
   : enRmNetId --- RMNET ID
   : 
     : RNIC_SPEC_CTX_STRU*
   :
   :

       :
  1.       : 2015330
           : A00165503
       : 
*****************************************************************************/
RNIC_SPEC_CTX_STRU* RNIC_GetNetCntxtByRmNetId(RNIC_RMNET_ID_ENUM_UINT8 enRmNetId)
{
    if (enRmNetId >= RNIC_RMNET_ID_BUTT)
    {
        return VOS_NULL_PTR;
    }

    return RNIC_GET_SPEC_NET_CTX(enRmNetId);
}


/*****************************************************************************
     : RNIC_GetResetSem
   : RNIC
   : 
   : 
     : RNIC
   :
   :

      :
 1.       : 20130415
          : f00179208
      : 
*****************************************************************************/
VOS_SEM RNIC_GetResetSem(VOS_VOID)
{
    return g_stRnicCtx.hResetSem;
}



