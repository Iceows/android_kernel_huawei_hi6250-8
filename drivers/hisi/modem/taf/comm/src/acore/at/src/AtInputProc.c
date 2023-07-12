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
#include "ATCmdProc.h"

#include "PppInterface.h"
#include "AtUsbComItf.h"
#include "AtInputProc.h"
#include "AtCsdInterface.h"
#include "AtTafAgentInterface.h"
#include "TafAgentInterface.h"
#include "cpm.h"
#include "AtCmdMsgProc.h"
#include "AtDataProc.h"
#include "ImmInterface.h"
#include "mdrv.h"

#include "AtMntn.h"
#include "AcpuReset.h"

/* Added by l00167671 for NV , 2013-05-17, begin */
#include "NasNvInterface.h"
/* Added by l00167671 for NV , 2013-05-17, end*/

#include "AtInternalMsg.h"
#include  "product_config.h"



/*****************************************************************************
    .C
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_AT_INPUTPROC_C

/*****************************************************************************
   2 
*****************************************************************************/
VOS_UINT32                              g_ulAtUsbDebugFlag = VOS_FALSE;

/*****************************************************************************
    : g_astAtHsicCtx
  : HSIC AT
                
                HSIC AT
                

1.        : 2012224
          : L47619
      : 
2.        : 2012628
          : L47619
      : UDI_ACM_HSIC_ACM10_IDHSIC4
3.        : 2012830
          : l00198894
      : UDI_ACM_HSIC_ACM10_IDUDI_ACM_HSIC_ACM12_ID

*****************************************************************************/

/* AT/DIAG */
VOS_UINT8                               gucOmDiagIndex    = AT_MAX_CLIENT_NUM;

/* USB NCMUDI */
UDI_HANDLE                              g_ulAtUdiNdisHdl  = UDI_INVALID_HANDLE;

/* USB-MODEM, HSIC-MODEMHS-UARTPL */
UDI_HANDLE                              g_alAtUdiHandle[AT_CLIENT_BUTT] = {UDI_INVALID_HANDLE};

/* ATDRV  */
AT_UART_FORMAT_PARAM_STRU               g_astAtUartFormatTab[] =
{
    /* auto detect (not support) */

    /* 8 data 2 stop */
    {AT_UART_FORMAT_8DATA_2STOP,            AT_UART_DATA_LEN_8_BIT,
     AT_UART_STOP_LEN_2_BIT,                AT_UART_PARITY_LEN_0_BIT},

    /* 8 data 1 parity 1 stop*/
    {AT_UART_FORMAT_8DATA_1PARITY_1STOP,    AT_UART_DATA_LEN_8_BIT,
     AT_UART_STOP_LEN_1_BIT,                AT_UART_PARITY_LEN_1_BIT},

    /* 8 data 1 stop */
    {AT_UART_FORMAT_8DATA_1STOP,            AT_UART_DATA_LEN_8_BIT,
     AT_UART_STOP_LEN_1_BIT,                AT_UART_PARITY_LEN_0_BIT},

    /* 7 data 2 stop */
    {AT_UART_FORMAT_7DATA_2STOP,            AT_UART_DATA_LEN_7_BIT,
     AT_UART_STOP_LEN_2_BIT,                AT_UART_PARITY_LEN_0_BIT},

    /* 7 data 1 parity 1 stop */
    {AT_UART_FORMAT_7DATA_1PARITY_1STOP,    AT_UART_DATA_LEN_7_BIT,
     AT_UART_STOP_LEN_1_BIT,                AT_UART_PARITY_LEN_1_BIT},

    /* 7 data 1 stop */
    {AT_UART_FORMAT_7DATA_1STOP,            AT_UART_DATA_LEN_7_BIT,
     AT_UART_STOP_LEN_1_BIT,                AT_UART_PARITY_LEN_0_BIT}
};


/*****************************************************************************
   3 
*****************************************************************************/

/*****************************************************************************
   4 
*****************************************************************************/

/*****************************************************************************
     : AT_GetAtMsgStruMsgLength
   : AT_MSG_STRU
   : VOS_UINT32 ulInfoLength   AT_MSG_STRUusLen
             VOS_UINT32 *pulMsgLength  AT_MSG_STRU
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 2011422
           : /f62575
       : , DTS2011041502672

*****************************************************************************/
VOS_VOID AT_GetAtMsgStruMsgLength(
    VOS_UINT32                          ulInfoLength,
    VOS_UINT32                         *pulMsgLength
)
{
    if (ulInfoLength > 4)
    {
        *pulMsgLength = (sizeof(AT_MSG_STRU) - VOS_MSG_HEAD_LENGTH)
                      + (ulInfoLength - 4);
    }
    else
    {
        *pulMsgLength = sizeof(AT_MSG_STRU) - VOS_MSG_HEAD_LENGTH;
    }

    return;
}

/*****************************************************************************
     : AT_GetUserTypeFromIndex
   : 
   : VOS_UINT8                           ucIndex
             VOS_UINT8                          *pucUserType
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 2011422
           : /f62575
       : , DTS2011041502672

*****************************************************************************/
VOS_VOID AT_GetUserTypeFromIndex(
    VOS_UINT8                           ucIndex,
    VOS_UINT8                          *pucUserType
)
{
    if (ucIndex < AT_MAX_CLIENT_NUM)
    {
        *pucUserType    = gastAtClientTab[ucIndex].UserType;
    }
    else
    {
        *pucUserType    = AT_BUTT_USER;
    }

    return;
}

/*****************************************************************************
     : AT_VcomCmdStreamEcho
   : Vcom AT
   : VOS_UINT8                           ucIndex
             VOS_UINT8                          *pData
             VOS_UINT16                          usLen
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 2012125
           : l00227485
       : 

  2.       : 2015527
           : l00198894
       : TSTS
*****************************************************************************/
VOS_VOID AT_VcomCmdStreamEcho(
    VOS_UINT8                           ucIndex,
    VOS_UINT8                          *pData,
    VOS_UINT16                          usLen
)
{
    VOS_UINT8                          *pucSystemAppConfig;

    pucSystemAppConfig                  = AT_GetSystemAppConfigAddr();

    /* E5 */
    /* AGPS */
    if ( (SYSTEM_APP_WEBUI != *pucSystemAppConfig)
      && (AT_CLIENT_TAB_APP9_INDEX != ucIndex)
      && (AT_CLIENT_TAB_APP12_INDEX != ucIndex)
      && (AT_CLIENT_TAB_APP24_INDEX != ucIndex)
    )
    {
        APP_VCOM_Send(gastAtClientTab[ucIndex].ucPortNo, pData, usLen);
    }

    return;
}

/*****************************************************************************
     : AT_SetAts3Value
   : ATS3
   : VOS_UINT8 ucValue
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 20130410
           : f00179208
       : 
*****************************************************************************/
VOS_VOID AT_SetAts3Value(VOS_UINT8 ucValue)
{
    if (VOS_TRUE == g_ulAtUsbDebugFlag)
    {
        ucAtS3 = ucValue;
    }

    return;
}

/*****************************************************************************
     : AT_CmdStreamEcho
   : AT
   : VOS_UINT8         ucIndex
             VOS_UINT8*        pData
             VOS_UINT16        usLen
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 2015527
           : l00198894
       : 

*****************************************************************************/
VOS_VOID AT_CmdStreamEcho(
    VOS_UINT8                           ucIndex,
    VOS_UINT8*                          pData,
    VOS_UINT16                          usLen
)
{
    VOS_UINT32                          ulMuxUserFlg;
    VOS_UINT32                          ulHsicUserFlg;
    VOS_UINT16                          usEchoLen;

    ulHsicUserFlg = AT_CheckHsicUser(ucIndex);
    ulMuxUserFlg  = AT_CheckMuxUser(ucIndex);

    /* pData<CR><LF>2 */
    if ((usLen > 2) && (ucAtS3 == pData[usLen - 2]) && (ucAtS4 == pData[usLen - 1]))
    {
        /* <LF> */
        usEchoLen = usLen - 1;
    }
    else
    {
        usEchoLen = usLen;
    }

    if(AT_USBCOM_USER == gastAtClientTab[ucIndex].UserType)
    {
        /*USB COM*/
        DMS_COM_SEND(AT_USB_COM_PORT_NO, pData, usEchoLen);
        AT_MNTN_TraceCmdResult(ucIndex, pData, usEchoLen);
    }
    else if (AT_CTR_USER == gastAtClientTab[ucIndex].UserType)
    {
        DMS_COM_SEND(AT_CTR_PORT_NO, pData, usEchoLen);
        AT_MNTN_TraceCmdResult(ucIndex, pData, usEchoLen);
    }
    else if(AT_PCUI2_USER == gastAtClientTab[ucIndex].UserType)
    {
        /*PCUI2*/
        DMS_COM_SEND(AT_PCUI2_PORT_NO, pData, usEchoLen);
        AT_MNTN_TraceCmdResult(ucIndex, pData, usEchoLen);
    }
    else if (AT_MODEM_USER == gastAtClientTab[ucIndex].UserType)
    {
        AT_SendDataToModem(ucIndex, pData, usEchoLen);
    }
    else if (AT_APP_USER == gastAtClientTab[ucIndex].UserType)
    {
        /* VCOM AT */
        AT_VcomCmdStreamEcho(ucIndex, pData, usEchoLen);
    }
    else if (AT_SOCK_USER == gastAtClientTab[ucIndex].UserType)
    {
        if ( BSP_MODULE_SUPPORT == mdrv_misc_support_check(BSP_MODULE_TYPE_WIFI) )
        {
            CPM_ComSend(CPM_AT_COMM, pData, VOS_NULL_PTR, usEchoLen);
        }
    }
    else if (AT_NDIS_USER == gastAtClientTab[ucIndex].UserType)
    {
        /* NDIS AT */
        AT_WARN_LOG("AT_CmdStreamEcho:WARNING: NDIS AT");
    }
    else if (VOS_TRUE == ulHsicUserFlg)
    {
/* Added by j00174725 for V3R3 Cut Out Memory2013-11-07,  Begin */
/* Added by j00174725 for V3R3 Cut Out Memory2013-11-07,  End */
    }
    else if (VOS_TRUE == ulMuxUserFlg)
    {
        /* MUX user */
        AT_MuxCmdStreamEcho(ucIndex, pData, usEchoLen);
    }
    else
    {
        AT_LOG1("AT_CmdStreamEcho:WARNING: Abnormal UserType,ucIndex:",ucIndex);
    }

    return;
}

/*****************************************************************************
 Prototype      : At_CmdStreamPreProc
 Description    :
 Input          : ucChar --- 
 Output         : ---
 Return Value   : AT_SUCCESS --- 
                  AT_FAILURE --- 
 Calls          : ---
 Called By      : ---

 History        : ---
  1.Date        : 2005-04-19
    Author      : ---
    Modification: Created function
  2.     : 2007-03-27
         : h59254
     : :A32D09820(PC-Lint)
  3.     : 2010-07-09
         : S62952
     : :DTS2010071000707
  4.     : 2010-08-05
         : S62952
     : :DTS2010080401112
  5.     : 2010-12-23
         : S62952
     : :DTS2010122002081,E5
  6.     : 20110224
         : A00165503
     : : DTS2011022404828MODEMAT
  7.       : 2011103
           : /l60609
       : AT Project: DRV/DMS
  8.       : 20111019
           : S62952
       : AT Project: modem
  9.       : 2012224
           : L47619
       : V7R1C50 IPC:HSIC AT
 10.       : 2012529
           : f62575
       : DTS2012052902986
 11.       : 201286
           : l60609
       : MUXmux
 12.       : 20121128
           : l00227485
       : DSDA:VCOM AT
 13.       : 20130527
           : f00179208
       : V3R3 PPP PROJECT
 14.       : 2014422
           : A00165503
       : DTS2014042208020: APPVCOM9APPVCOM12
 15.       : 2015527
           : l00198894
       : TSTS
*****************************************************************************/
VOS_UINT32 At_CmdStreamPreProc(VOS_UINT8 ucIndex, VOS_UINT8* pData, VOS_UINT16 usLen)
{
    VOS_UINT8                          *pHead = TAF_NULL_PTR;
    VOS_UINT16                          usCount = 0;
    VOS_UINT16                          usTotal = 0;

    pHead = pData;

    if (VOS_TRUE == g_ulAtUsbDebugFlag)
    {
        (VOS_VOID)vos_printf("At_CmdStreamPreProc: AtEType = %d, UserType = %d, ucAtS3 = %d\r\n",
                   gucAtEType, gastAtClientTab[ucIndex].UserType, ucAtS3);
    }

    /*  */
    if( AT_E_ECHO_CMD == gucAtEType )
    {
        AT_CmdStreamEcho(ucIndex, pData, usLen);
    }

    /* MACMP:AT+CMGS=**<CR><^z><Z>(AT+CMGW=**<CR><^z><Z>)
       
       <^z><Z> */
    AT_DiscardInvalidCharForSms(pData, &usLen);

    /* AT: <CR>/<ctrl-z>/<ESC> */
    while(usCount++ < usLen)
    {
        if (At_CheckSplitChar((*((pData + usCount) - 1))))
        {
            if(g_aucAtDataBuff[ucIndex].ulBuffLen > 0)
            {
                if((g_aucAtDataBuff[ucIndex].ulBuffLen + usCount) >= AT_COM_BUFF_LEN)
                {
                    g_aucAtDataBuff[ucIndex].ulBuffLen = 0;
                    return AT_FAILURE;
                }
                TAF_MEM_CPY_S(&g_aucAtDataBuff[ucIndex].aucDataBuff[g_aucAtDataBuff[ucIndex].ulBuffLen],
                    AT_COM_BUFF_LEN - g_aucAtDataBuff[ucIndex].ulBuffLen,
                    pHead,
                    usCount);
                At_SendCmdMsg(ucIndex,g_aucAtDataBuff[ucIndex].aucDataBuff, (TAF_UINT16)(g_aucAtDataBuff[ucIndex].ulBuffLen + usCount), AT_NORMAL_TYPE_MSG);
                pHead   = pData + usCount;
                usTotal = usCount;
                g_aucAtDataBuff[ucIndex].ulBuffLen = 0;
            }
            else
            {
                At_SendCmdMsg(ucIndex, pHead, usCount - usTotal, AT_NORMAL_TYPE_MSG);
                pHead   = pData + usCount;
                usTotal = usCount;
            }
        }
    }

    if(usTotal < usLen)
    {
        if((g_aucAtDataBuff[ucIndex].ulBuffLen + (usLen - usTotal)) >= AT_COM_BUFF_LEN)
        {
            g_aucAtDataBuff[ucIndex].ulBuffLen = 0;
            return AT_FAILURE;
        }
        TAF_MEM_CPY_S(&g_aucAtDataBuff[ucIndex].aucDataBuff[g_aucAtDataBuff[ucIndex].ulBuffLen],
            AT_COM_BUFF_LEN - g_aucAtDataBuff[ucIndex].ulBuffLen,
            pHead,
            (TAF_UINT32)(usLen - usTotal));
        g_aucAtDataBuff[ucIndex].ulBuffLen += (VOS_UINT16)((pData - pHead) + usLen);
    }

    return AT_SUCCESS;
}

/*****************************************************************************
 Prototype      : AT_StopFlowCtrl
 Description    : AT
 Input          : ucIndex
 Output         : ---
 Return Value   : VOS_VOID

 Calls          : ---
 Called By      : ---

 History        : ---
  1.Date        : 2007-09-25
    Author      : L47619
    Modification: Created function

  2.       : 2013923
           : A00165503
       : UART-MODEM: 
*****************************************************************************/
VOS_VOID AT_StopFlowCtrl(VOS_UINT8 ucIndex)
{
    switch (gastAtClientTab[ucIndex].UserType)
    {
        case AT_MODEM_USER:
            AT_MNTN_TraceStopFlowCtrl(ucIndex, AT_FC_DEVICE_TYPE_MODEM);
            AT_CtrlCTS(ucIndex, AT_IO_LEVEL_HIGH);
            break;

        case AT_HSUART_USER:
            AT_MNTN_TraceStopFlowCtrl(ucIndex, AT_FC_DEVICE_TYPE_HSUART);
            AT_CtrlCTS(ucIndex, AT_IO_LEVEL_HIGH);
            break;

        default:
            break;
    }

    return;
}

/*****************************************************************************
     : At_OmDataProc
   : OM
   : VOS_UINT8                           ucPortNo
             VOS_UINT8                          *pData
             VOS_UINT16                          usLen
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 20111010
           : /l60609
       : 

  2.       : 2015522
           : l00198894
       : TSTS
*****************************************************************************/
VOS_UINT32 At_OmDataProc (
    VOS_UINT8                           ucPortNo,
    VOS_UINT8                          *pData,
    VOS_UINT16                          usLen
)
{
    VOS_UINT32                          ulRst;

    /*OMUART PCUI CTRL*/
    switch(ucPortNo)
    {
        case AT_UART_PORT_NO:
            if (VOS_NULL_PTR == g_apAtPortDataRcvFuncTab[AT_UART_PORT])
            {
                AT_ERR_LOG("At_OmDataProc: Uart port proc func is NULL!");
                return VOS_ERR;
            }

            ulRst = g_apAtPortDataRcvFuncTab[AT_UART_PORT](pData, usLen);
            break;

        case AT_USB_COM_PORT_NO:
            if (VOS_NULL_PTR == g_apAtPortDataRcvFuncTab[AT_PCUI_PORT])
            {
                AT_ERR_LOG("At_OmDataProc: PCUI proc func is NULL!");
                return VOS_ERR;
            }

            ulRst = g_apAtPortDataRcvFuncTab[AT_PCUI_PORT](pData, usLen);
            break;

        case AT_CTR_PORT_NO:
            if (VOS_NULL_PTR == g_apAtPortDataRcvFuncTab[AT_CTRL_PORT])
            {
                AT_ERR_LOG("At_OmDataProc: CTRL proc func is NULL!");
                return VOS_ERR;
            }

            ulRst = g_apAtPortDataRcvFuncTab[AT_CTRL_PORT](pData, usLen);
            break;

        case AT_PCUI2_PORT_NO:
            if (VOS_NULL_PTR == g_apAtPortDataRcvFuncTab[AT_PCUI2_PORT])
            {
                AT_ERR_LOG("At_OmDataProc: PCUI2 proc func is NULL!");
                return VOS_ERR;
            }

            ulRst = g_apAtPortDataRcvFuncTab[AT_PCUI2_PORT](pData, usLen);
            break;

        case AT_HSUART_PORT_NO:
            if (VOS_NULL_PTR == g_apAtPortDataRcvFuncTab[AT_HSUART_PORT])
            {
                AT_ERR_LOG("At_OmDataProc: HSUART proc func is NULL!");
                return VOS_ERR;
            }

            ulRst = g_apAtPortDataRcvFuncTab[AT_HSUART_PORT](pData, usLen);
            break;

        default:
            AT_WARN_LOG("At_OmDataProc: don't proc data of this port!");
            return VOS_ERR;
    }

    return ulRst;
}

/*****************************************************************************
     : At_DataStreamPreProc
   : AT
   : TAF_UINT8 ucIndex
             TAF_UINT8 DataMode
             TAF_UINT8* pData
             TAF_UINT16 usLen
   : 
     : TAF_UINT32
   :
   :

       :
  1.Date        : 2005-04-19
    Author      : ---
    Modification: Created function
  2.       : 20111010
           : /l60609
       : OM

*****************************************************************************/
TAF_UINT32 At_DataStreamPreProc (TAF_UINT8 ucIndex,TAF_UINT8 DataMode,TAF_UINT8* pData, TAF_UINT16 usLen)
{

    AT_LOG1("At_DataStreamPreProc ucIndex:",ucIndex);
    AT_LOG1("At_DataStreamPreProc usLen:",usLen);
    AT_LOG1("At_DataStreamPreProc DataMode:",DataMode);

    switch(DataMode)    /*  */
    {
        case AT_CSD_DATA_MODE:
            break;

        /* Modified by L60609 for AT Project2011-10-04,  Begin*/
        /*OM*/
        case AT_DIAG_DATA_MODE:
        case AT_OM_DATA_MODE:
            At_OmDataProc(gastAtClientTab[ucIndex].ucPortNo, pData,usLen);
            break;
        /* Modified by L60609 for AT Project2011-10-04,  End*/

        default:
            AT_WARN_LOG("At_DataStreamPreProc DataMode Wrong!");
            break;
    }
    return AT_SUCCESS;
}

/* Modified by s62952 for BalongV300R002 Build 2012-02-28, begin */
/*****************************************************************************
     : AT_CsdDataModeRcvModemMsc
   : CS
   : ucIndex -- 
   : 
     : AT_XXX  --- ATC
   :
   :

       :
  1.       : 2010518
           : h44270
       : 
  2.       : 2010728
           : z00161729
       : DTS2010082602962:
  3.       : 20111024
           : f00179208
       : AT,VP
  3.       : 20111228
           : w00199382
       : PSAPI
  4.       : 2012810
           : L00171473
       : DTS2012082204471, TQE
*****************************************************************************/
VOS_UINT32  AT_CsdDataModeRcvModemMsc(
    VOS_UINT8                           ucIndex
)
{
    /* Modified by w00199382 for PS Project2011-12-06,  Begin*/
    TAFAGERNT_MN_CALL_INFO_STRU         astCallInfos[MN_CALL_MAX_NUM];
    VOS_UINT32                          i;
    VOS_UINT8                           ucNumOfCalls;
    VOS_UINT32                          ulRlst;


    TAF_MEM_SET_S(astCallInfos, sizeof(astCallInfos), 0x00, sizeof(astCallInfos));


    /* VIDEOVIDEO
       VIDEO */

    /* Modified by l60609 for DSDA PhaseIII, 2013-3-13, begin */
    ulRlst          = TAF_AGENT_GetCallInfoReq(ucIndex, &ucNumOfCalls, astCallInfos);
    /* Modified by l60609 for DSDA PhaseIII, 2013-3-13, end */

    if(VOS_OK == ulRlst)
    {
        for (i = 0; i < ucNumOfCalls; i++)
        {
            if (MN_CALL_TYPE_VIDEO == astCallInfos[i].enCallType)
            {
                /*  */
                if (gastAtClientTab[ucIndex].CmdCurrentOpt != AT_CMD_END_SET)
                {
                    TAF_LOG1(WUEPS_PID_AT, 0, PS_LOG_LEVEL_INFO, "At_SetHPara: ulNumOfCalls is ",(TAF_INT32)ucNumOfCalls);

                    if(AT_SUCCESS == MN_CALL_End(gastAtClientTab[ucIndex].usClientId,
                                                 0,
                                                 astCallInfos[i].callId,
                                                 VOS_NULL_PTR))
                    {
                        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_END_SET;
                        return AT_SUCCESS;
                    }
                    else
                    {
                        return AT_ERROR;
                    }
                }
                else
                {
                    /* DTR */
                    return AT_SUCCESS;
                }
            }
        }
    }

    /* Modified by w00199382 for PS Project2011-12-06,  End*/

    return AT_CME_UNKNOWN;
}

/*****************************************************************************
     : AT_PppDataModeRcvModemMsc
   : PPP
   : ucIndex -- 
   : 
     : AT_XXX  --- ATC
   :
   :

       :
  1.       : 2010518
           : h44270
       : 

  2.       : 20111020
           : A00165503
       : AT, 
*****************************************************************************/
VOS_UINT32  AT_PppDataModeRcvModemMsc(
    VOS_UINT8                           ucIndex,
    AT_DCE_MSC_STRU                     *pMscStru
)
{
    /* 1.(AT_CMD_PS_DATA_CALL_END_SET != gastAtClientTab[ucIndex].CmdCurrentOpt)
         :PDP DEACTIVEDTR
         
        2.:,PPPPPP
          DTRPPP
    */
    if (pMscStru->OP_Dtr && (0 == pMscStru->ucDtr))
    {
        if ( (AT_CMD_PS_DATA_CALL_END_SET      == gastAtClientTab[ucIndex].CmdCurrentOpt)
          || (AT_CMD_WAIT_PPP_PROTOCOL_REL_SET == gastAtClientTab[ucIndex].CmdCurrentOpt) )
        {
            return AT_SUCCESS;
        }

        /* PPPUE
           UEDTR,PPP*/
        if (0 == (gastAtClientTab[ucIndex].ModemStatus & IO_CTRL_CTS))
        {
            AT_StopFlowCtrl((TAF_UINT8)ucIndex);
        }

        /*PPPPPP*/
        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId, PPP_AT_CTRL_REL_PPP_REQ);

        /*PPPHDLC*/
        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId, PPP_AT_CTRL_HDLC_DISABLE);

        /* ModemATAT */
        AT_STOP_TIMER_CMD_READY(ucIndex);

        /*EVENT - RCV Down DTR to Disconnect PPP in Abnormal procedure(PDP type:IP) ;index*/
        AT_EventReport(WUEPS_PID_AT, NAS_OM_EVENT_DTE_DOWN_DTR_RELEASE_PPP_IP_TYPE,
                        &ucIndex, sizeof(ucIndex));

        if ( VOS_OK == TAF_PS_CallEnd(WUEPS_PID_AT,
                                      AT_PS_BuildExClientId(gastAtClientTab[ucIndex].usClientId),
                                      0,
                                      gastAtClientTab[ucIndex].ucCid) )
        {
            /*  */
            if (AT_SUCCESS != At_StartTimer(AT_SET_PARA_TIME, ucIndex))
            {
                AT_ERR_LOG("At_UsbModemStatusPreProc:ERROR:Start Timer");
                return AT_FAILURE;
            }

            /*  */
            gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_PS_DATA_CALL_END_SET;
        }
        else
        {
            return AT_FAILURE;
        }
    }

    return AT_SUCCESS;
}

/*****************************************************************************
     : AT_IpDataModeRcvModemMsc
   : IP
   : ucIndex -- 
   : 
     : AT_XXX  --- ATC
   :
   :

       :
  1.       : 2010518
           : h44270
       : 

  2.       : 20111020
           : A00165503
       : AT, 
*****************************************************************************/
VOS_UINT32  AT_IpDataModeRcvModemMsc(
    VOS_UINT8                           ucIndex,
    AT_DCE_MSC_STRU                     *pMscStru
)
{
    if (pMscStru->OP_Dtr && (0 == pMscStru->ucDtr))
    {
        /**/
        if (0 == (gastAtClientTab[ucIndex].ModemStatus & IO_CTRL_CTS))
        {
            AT_StopFlowCtrl((TAF_UINT8)ucIndex);
        }

        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId, PPP_AT_CTRL_REL_PPP_RAW_REQ);

        /*PPPHDLC*/
        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId, PPP_AT_CTRL_HDLC_DISABLE);

        /* ModemATAT */
        AT_STOP_TIMER_CMD_READY(ucIndex);;

        /*EVENT - RCV Down DTR to Disconnect PPP in Abnormal procedure(PDP type:PPP) ;index*/
        AT_EventReport(WUEPS_PID_AT, NAS_OM_EVENT_DTE_DOWN_DTR_RELEASE_PPP_PPP_TYPE,
                        &ucIndex, sizeof(ucIndex));

        if ( VOS_OK == TAF_PS_CallEnd(WUEPS_PID_AT,
                                      AT_PS_BuildExClientId(gastAtClientTab[ucIndex].usClientId),
                                      0,
                                      gastAtClientTab[ucIndex].ucCid) )
        {
            gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_PS_DATA_CALL_END_SET;
            return AT_SUCCESS;
        }
        else
        {
            return AT_FAILURE;
        }
    }

    return AT_SUCCESS;
}

/*****************************************************************************
     : AT_MODEM_ProcDtrChange
   : Modem DTR
   : ucIndex   --- 
             pstDceMsc --- ()
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 20130921
           : j00174725
       : 
*****************************************************************************/
VOS_VOID AT_MODEM_ProcDtrChange(
    VOS_UINT8                           ucIndex,
    AT_DCE_MSC_STRU                    *pstDceMsc
)
{
    if (1 == pstDceMsc->ucDtr)
    {
        /*DSRCTS*/
        AT_CtrlDSR(ucIndex, AT_IO_LEVEL_HIGH);
        AT_StopFlowCtrl(ucIndex);
    }
    else
    {
        /* QDSRDTRDSR
           PCDTR
           UEDTR DCD */
        if ( (AT_DATA_MODE == gastAtClientTab[ucIndex].Mode)
          && (AT_CSD_DATA_MODE == gastAtClientTab[ucIndex].DataMode) )
        {
            g_ucDtrDownFlag = VOS_TRUE;
        }

        AT_CtrlDCD(ucIndex, AT_IO_LEVEL_LOW);
    }

}

/*****************************************************************************
     : AT_MODEM_WriteMscCmd
   : MODEM
   : ucIndex    - 
             pstDceMsc  - ()
   : 
     : AT_SUCCESS - 
             AT_FAILURE - 
   :
   :

       :
  1.       : 20131112
           : A00165503
       : 
*****************************************************************************/
VOS_UINT32 AT_MODEM_WriteMscCmd(
    VOS_UINT8                           ucIndex,
    AT_DCE_MSC_STRU                    *pstDceMsc
)
{
    UDI_HANDLE                          lUdiHandle;
    VOS_INT32                           lResult;

    /* UDI */
    lUdiHandle = g_alAtUdiHandle[ucIndex];
    if (UDI_INVALID_HANDLE == lUdiHandle)
    {
        AT_ERR_LOG("AT_MODEM_WriteMscCmd: Invalid UDI handle!");
        return AT_FAILURE;
    }

    /*  */
    lResult = mdrv_udi_ioctl(lUdiHandle, ACM_MODEM_IOCTL_MSC_WRITE_CMD, pstDceMsc);
    if (VOS_OK != lResult)
    {
        AT_ERR_LOG("AT_MODEM_WriteMscCmd: Write failed!");
        return AT_FAILURE;
    }

    return AT_SUCCESS;
}

/*****************************************************************************
 Prototype      : AT_MODEM_StartFlowCtrl
 Description    : AT
 Input          :
 Output         : ---
 Return Value   : AT_SUCCESS --- 
                  AT_FAILURE --- 
 Calls          : ---
 Called By      : ---

 History        : ---
  1.       : 2007-09-25
           : L47619
       : Created function

  2.       : 20111217
           : c00173809
       : PS

  3.       : 2013923
           : A00165503
       : UART-MODEM: 
*****************************************************************************/
VOS_UINT32 AT_MODEM_StartFlowCtrl(
    VOS_UINT32                          ulParam1,
    VOS_UINT32                          ulParam2
)
{
    VOS_UINT8                           ucIndex;

    for (ucIndex = 0; ucIndex < AT_MAX_CLIENT_NUM; ucIndex++)
    {
        if ( (AT_MODEM_USER == gastAtClientTab[ucIndex].UserType)
          && (AT_DATA_MODE == gastAtClientTab[ucIndex].Mode) )
        {
            if ( (AT_PPP_DATA_MODE == gastAtClientTab[ucIndex].DataMode)
              || (AT_IP_DATA_MODE == gastAtClientTab[ucIndex].DataMode)
              || (AT_CSD_DATA_MODE == gastAtClientTab[ucIndex].DataMode) )
            {
                AT_MNTN_TraceStartFlowCtrl(ucIndex, AT_FC_DEVICE_TYPE_MODEM);
                AT_CtrlCTS(ucIndex, AT_IO_LEVEL_LOW);
            }
        }
    }

    return AT_SUCCESS;
}

/*****************************************************************************
 Prototype      : AT_MODEM_StopFlowCtrl
 Description    : AT
 Input          :
 Output         : ---
 Return Value   : AT_SUCCESS --- 
                  AT_FAILURE --- 
 Calls          : ---
 Called By      : ---

 History        : ---
  1.       : 2007-09-25
           : L47619
       : Created function

  2.       : 20111217
           : c00173809
       : PS

  3.       : 2013923
           : A00165503
       : UART-MODEM: 
*****************************************************************************/
VOS_UINT32 AT_MODEM_StopFlowCtrl(
    VOS_UINT32                          ulParam1,
    VOS_UINT32                          ulParam2
)
{
    VOS_UINT8                           ucIndex;

    for(ucIndex = 0; ucIndex < AT_MAX_CLIENT_NUM; ucIndex++)
    {
        if ( (AT_MODEM_USER == gastAtClientTab[ucIndex].UserType)
          && (AT_DATA_MODE == gastAtClientTab[ucIndex].Mode) )
        {
            if ( (AT_PPP_DATA_MODE == gastAtClientTab[ucIndex].DataMode)
                || (AT_IP_DATA_MODE == gastAtClientTab[ucIndex].DataMode)
                || (AT_CSD_DATA_MODE == gastAtClientTab[ucIndex].DataMode) )
            {
                AT_MNTN_TraceStopFlowCtrl(ucIndex, AT_FC_DEVICE_TYPE_MODEM);
                AT_CtrlCTS(ucIndex, AT_IO_LEVEL_HIGH);
            }
        }
    }

    return AT_SUCCESS;
}

/*****************************************************************************
 Prototype      : AT_ModemStatusPreProc
 Description    : 
 Input          : ucIndex    --- 
                  pMscStru   --- ()
 Output         : ---
 Return Value   : AT_SUCCESS --- 
                  AT_FAILURE --- 
 Calls          : ---
 Called By      : ---

 History        : ---
  1.Date        : 2007-11-14
    Author      : s62952
    Modification: Created function

  3.       : 20101229
           : z00161729
       : DTS2010123000175:TMEno number

  4.       : 20130921
           : j00174725
       : UART-MODEM: UART

  5.       : 20131220
           : A00165503
       : DTS2013121910186: DTR, online-cmd
*****************************************************************************/
VOS_UINT32 AT_ModemStatusPreProc(
    VOS_UINT8                           ucIndex,
    AT_DCE_MSC_STRU                    *pMscStru
)
{

    NAS_OM_EVENT_ID_ENUM_UINT16         enEventId;




    if (VOS_NULL_PTR == pMscStru)
    {
        return AT_FAILURE;
    }

    if (pMscStru->OP_Dtr)
    {
        enEventId = (0 != pMscStru->ucDtr) ?
                    NAS_OM_EVENT_DTE_UP_DTR : NAS_OM_EVENT_DTE_DOWN_DTR;

        AT_EventReport(WUEPS_PID_AT, enEventId, &ucIndex, sizeof(VOS_UINT8));

        if (VOS_TRUE == AT_CheckModemUser(ucIndex))
        {
            AT_MODEM_ProcDtrChange(ucIndex, pMscStru);
        }

    }

    /* MSC */
    if ( (AT_DATA_MODE == gastAtClientTab[ucIndex].Mode)
      || (AT_ONLINE_CMD_MODE == gastAtClientTab[ucIndex].Mode) )
    {
        switch (gastAtClientTab[ucIndex].DataMode)
        {
        case AT_CSD_DATA_MODE:
            if ((pMscStru->OP_Dtr) && (0 == pMscStru->ucDtr))
            {
                AT_CsdDataModeRcvModemMsc(ucIndex);
            }
            return AT_SUCCESS;

        case AT_PPP_DATA_MODE:
            return AT_PppDataModeRcvModemMsc(ucIndex, pMscStru);

        case AT_IP_DATA_MODE:
            return AT_IpDataModeRcvModemMsc(ucIndex, pMscStru);

        default:
            AT_WARN_LOG("At_UsbModemStatusPreProc: DataMode Wrong!");
            break;
        }
    }
    else
    {
         /* 
            CSD */
         if ((pMscStru->OP_Dtr) && (0 == pMscStru->ucDtr))
         {
             AT_CsdDataModeRcvModemMsc(ucIndex);
         }
    }

    return AT_SUCCESS;
}
/* Modified by s62952 for BalongV300R002 Build 2012-02-28, end */

/*****************************************************************************
     : AT_ModemSetCtlStatus
   : 
   : pMscStru --- 
   : 
     : 
   :
   :

       :
  1.       : 20111017
           : s62952
       : 
  2.       : 20130522
           : f00179208
       : V3R3 PPP PROJECT
*****************************************************************************/
VOS_VOID AT_ModemSetCtlStatus(
    VOS_UINT8                           ucIndex,
    AT_DCE_MSC_STRU                    *pMscStru
)
{
    if (TAF_NULL_PTR == pMscStru)
    {
        return;
    }

    /*dsr*/
    if ( pMscStru->OP_Dsr )
    {
        if ( 1 == pMscStru->ucDsr )
        {
            gastAtClientTab[ucIndex].ModemStatus |= IO_CTRL_DSR;
        }
        else
        {
            gastAtClientTab[ucIndex].ModemStatus &= ~IO_CTRL_DSR;
        }
    }

    /*CTS*/
    if ( pMscStru->OP_Cts )
    {
        if ( 1 == pMscStru->ucCts )
        {
            gastAtClientTab[ucIndex].ModemStatus |= IO_CTRL_CTS;
        }
        else
        {
            gastAtClientTab[ucIndex].ModemStatus &= ~IO_CTRL_CTS;
        }
    }

    /*RI*/
    if ( pMscStru->OP_Ri )
    {
        if ( 1 == pMscStru->ucRi )
        {
            gastAtClientTab[ucIndex].ModemStatus |= IO_CTRL_RI;
        }
        else
        {
            gastAtClientTab[ucIndex].ModemStatus &= ~IO_CTRL_RI;
        }
    }

    /*DCD*/
    if ( pMscStru->OP_Dcd )
    {
        if ( 1 == pMscStru->ucDcd )
        {
            gastAtClientTab[ucIndex].ModemStatus |= IO_CTRL_DCD;
        }
        else
        {
            gastAtClientTab[ucIndex].ModemStatus &= ~IO_CTRL_DCD;
        }
    }

    /*FC*/
    if ( pMscStru->OP_Fc )
    {
        if ( 1 == pMscStru->ucFc )
        {
            gastAtClientTab[ucIndex].ModemStatus |= IO_CTRL_FC;
        }
        else
        {
            gastAtClientTab[ucIndex].ModemStatus &= ~IO_CTRL_FC;
        }
    }

}

/*****************************************************************************
     : AT_SetModemStatus
   : 
   : ucIndex  --- 
             pMscStru --- 
   : 
     : AT_SUCCESS --- 
             AT_FAILURE --- 
   :
   :

       :
  1.       : 20111017
           : s62952
       : 

  2.       : 20130527
           : f00179208
       : V3R3 PPP PROJECT

  3.       : 20130921
           : j00174725
       : UART-MODEM: UART
*****************************************************************************/
VOS_UINT32 AT_SetModemStatus(
    VOS_UINT8                           ucIndex,
    AT_DCE_MSC_STRU                    *pstMsc
)
{
    VOS_UINT32                          ulResult;

    if (VOS_NULL_PTR == pstMsc)
    {
        return AT_FAILURE;
    }

    if (ucIndex >= AT_CLIENT_BUTT)
    {
        return AT_FAILURE;
    }

    /* */
    AT_ModemSetCtlStatus(ucIndex, pstMsc);

    /*  */
    AT_MNTN_TraceOutputMsc(ucIndex, pstMsc);

    /*  */
    switch (gastAtClientTab[ucIndex].UserType)
    {
        case AT_MODEM_USER:
            ulResult = AT_MODEM_WriteMscCmd(ucIndex, pstMsc);
            break;


        default:
            ulResult = AT_SUCCESS;
            break;
    }

    return ulResult;
}

/* At_SetModemStatusForFC, At_SetModemStatus */

/*****************************************************************************
     : At_ModemEst
   : Modem
   : VOS_UINT8                           ucIndex
             AT_CLIENT_ID_ENUM_UINT16            usClientId
             VOS_UINT8                           ucPortType
             AT_USER_TYPE                        ucUserType
             VOS_UINT8                           ucDlci
   : 
     : VOS_UINT32
             AT_SUCCESS
             AT_FAILURE
   :
   :

       :
  1.Date        : 2007-11-14
    Author      : 62952
    Modification: Created function
  2.       : 2011930
           : /l60609
       : AT Project: At_RegTafCallBackFunc; client id
  3.       : 20130527
           : f00179208
       : V3R3 PPP PROJECT
*****************************************************************************/
VOS_UINT32 At_ModemEst (
    VOS_UINT8                           ucIndex,
    AT_CLIENT_ID_ENUM_UINT16            usClientId,
    VOS_UINT8                           ucPortNo
)
{

    /*  */
    TAF_MEM_SET_S(&gastAtClientTab[ucIndex], sizeof(AT_CLIENT_MANAGE_STRU), 0x00, sizeof(AT_CLIENT_MANAGE_STRU));

    /*  */
    gastAtClientTab[ucIndex].usClientId      = usClientId;
    gastAtClientTab[ucIndex].ucPortType      = ucPortNo;
    gastAtClientTab[ucIndex].ucDlci          = AT_MODEM_USER_DLCI;
    gastAtClientTab[ucIndex].ucPortNo        = ucPortNo;
    gastAtClientTab[ucIndex].UserType        = AT_MODEM_USER;
    gastAtClientTab[ucIndex].ucUsed          = AT_CLIENT_USED;

    /* PS_MEMSET */
    gastAtClientTab[ucIndex].Mode            = AT_CMD_MODE;
    gastAtClientTab[ucIndex].IndMode         = AT_IND_MODE;
    gastAtClientTab[ucIndex].DataMode        = AT_DATA_BUTT_MODE;
    gastAtClientTab[ucIndex].DataState       = AT_DATA_STOP_STATE;
    gastAtClientTab[ucIndex].CmdCurrentOpt   = AT_CMD_CURRENT_OPT_BUTT;
    g_stParseContext[ucIndex].ucClientStatus = AT_FW_CLIENT_STATUS_READY;

    AT_LOG1("At_ModemEst ucIndex:",ucIndex);

    return AT_SUCCESS;
}

/*****************************************************************************
     : At_ModemMscInd
   : Modem Msc Ind
   : VOS_UINT8                           ucIndex
             VOS_UINT8                           ucDlci
             AT_DCE_MSC_STRU                    *pMscStru
   : 
     : VOS_UINT32
   :
   :

       :
  1.Date        : 2007-11-14
    Author      : s62952
    Modification: Created function
  2.       : 20111017
           : /l60609
       : AT Project
  3.       : 20130522
           : f00179208
       : V3R3 PPP PROJECT
*****************************************************************************/
VOS_UINT32 At_ModemMscInd (
    VOS_UINT8                           ucIndex,
    VOS_UINT8                           ucDlci,
    AT_DCE_MSC_STRU                    *pMscStru
)
{
    AT_PPP_MODEM_MSC_IND_MSG_STRU      *pMsg;
    VOS_UINT32                          ulLength;
    VOS_UINT_PTR                        ulTmpAddr;

    ulLength = (sizeof(AT_PPP_MODEM_MSC_IND_MSG_STRU) - VOS_MSG_HEAD_LENGTH)
               + (sizeof(AT_DCE_MSC_STRU) - 2);

    pMsg = ( AT_PPP_MODEM_MSC_IND_MSG_STRU * )PS_ALLOC_MSG( PS_PID_APP_PPP, ulLength );
    if ( VOS_NULL_PTR == pMsg )
    {
        /*---:*/
        AT_WARN_LOG("At_ModemMscInd: Alloc AT_PPP_MODEM_MSC_IND_MSG_STRU msg fail!");
        return AT_FAILURE;
    }

    /*:*/
    pMsg->MsgHeader.ulSenderCpuId   = VOS_LOCAL_CPUID;
    pMsg->MsgHeader.ulSenderPid     = PS_PID_APP_PPP;
    pMsg->MsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pMsg->MsgHeader.ulReceiverPid   = WUEPS_PID_AT;
    pMsg->MsgHeader.ulLength        = ulLength;
    pMsg->MsgHeader.ulMsgName       = AT_PPP_MODEM_MSC_IND_MSG;

    /**/
    pMsg->ucIndex                   = ucIndex;
    pMsg->ucDlci                    = ucDlci;

    /*  */
    ulTmpAddr = (VOS_UINT_PTR)(pMsg->aucMscInd);

    TAF_MEM_CPY_S((VOS_VOID *)ulTmpAddr, sizeof(AT_DCE_MSC_STRU), (VOS_UINT8 *)pMscStru, sizeof(AT_DCE_MSC_STRU));

    /*  */
    if ( VOS_OK != PS_SEND_MSG( PS_PID_APP_PPP, pMsg ) )
    {
        /*---:*/
        AT_WARN_LOG( "At_ModemMscInd:WARNING:SEND AT_PPP_MODEM_MSC_IND_MSG_STRU msg FAIL!" );
        return AT_FAILURE;
    }

    return AT_SUCCESS;
}

/******************************************************************************
    : AT_UsbModemGetUlDataBuf
  : modem
  :  ppstBuf    ----      
  : 
    : AT_SUCCESS ----      
            AT_FAILURE ----      
  :
  :

      :
 1.       : 20111017
          : sunshaohua
      : 
  2.       : 2011128
           : /l60609
       : BufIMM_ZC_STRU
  3.       : 2012810
           : L00171473
       : DTS2012082204471, TQE
  4.       : 20130527
           : f00179208
       : V3R3 PPP PROJECT
*****************************************************************************/
VOS_UINT32 AT_ModemGetUlDataBuf(
    VOS_UINT8                           ucIndex,
    IMM_ZC_STRU                       **ppstBuf
)
{
    ACM_WR_ASYNC_INFO                   stCtlParam;
    VOS_INT32                           ulResult;


    TAF_MEM_SET_S(&stCtlParam, sizeof(stCtlParam), 0x00, sizeof(stCtlParam));


    /* Modified by L60609 for PS Project2011-12-06,  Begin*/

    /* buffer */
    ulResult = mdrv_udi_ioctl(g_alAtUdiHandle[ucIndex], ACM_IOCTL_GET_RD_BUFF, &stCtlParam);

    if ( VOS_OK != ulResult )
    {
        AT_ERR_LOG1("AT_ModemGetUlDataBuf, WARNING, Get modem buffer failed code %d!",
                  ulResult);
        AT_MODEM_DBG_UL_GET_RD_FAIL_NUM(1);
        return AT_FAILURE;
    }

    if (VOS_NULL_PTR == stCtlParam.pVirAddr)
    {
        AT_ERR_LOG("AT_ModemGetUlDataBuf, WARNING, Data buffer error");
        AT_MODEM_DBG_UL_INVALID_RD_NUM(1);
        return AT_FAILURE;
    }

    AT_MODEM_DBG_UL_GET_RD_SUCC_NUM(1);

    *ppstBuf = (IMM_ZC_STRU *)stCtlParam.pVirAddr;

    /* Modified by L60609 for PS Project2011-12-06,  End*/
    return AT_SUCCESS;
}

/*****************************************************************************
     : At_ModemDataInd
   : USB Modem
   : VOS_UINT8                           ucPortType,
             VOS_UINT8                           ucDlci,
             IMM_ZC_STRU                        *pstData,
   : 
     : VOS_UINT32
             AT_SUCCESS
             AT_SUCCESS
   :
   :

       :
  1.Date        : 2007-11-14
    Author      : s62952
    Modification: Created function
  2.       : 20111015
           : /l60609
       : AT Project:  client id
  3.       : 2011128
           : /l60609
       : BufIMM_ZC_STRU
  4.       : 2011128
           : w00199382
       : CSD
  5.       : 20130522
           : f00179208
       : V3R3 PPP PROJECT
*****************************************************************************/
VOS_UINT32 At_ModemDataInd(
    VOS_UINT8                           ucIndex,
    VOS_UINT8                           ucDlci,
    IMM_ZC_STRU                        *pstData
)
{
    /* Modified by L60609 for PS Project2011-12-06,  Begin*/
    AT_DCE_MSC_STRU                     stMscStru;
    VOS_UINT32                          ulRet;
    /* pData */
    VOS_UINT8                          *pData;
    /* usLen */
    VOS_UINT16                          usLen;

    /* indexDlci */
    if (AT_CLIENT_TAB_MODEM_INDEX != ucIndex)
    {
        /**/
        AT_ModemFreeUlDataBuf(ucIndex, pstData);
        return AT_FAILURE;
    }

    /* pstData(IMM_ZC_STRU)pDatausLen */
    pData = pstData->data;
    usLen = (VOS_UINT16)pstData->len;

    if ( AT_CMD_MODE == gastAtClientTab[ucIndex].Mode )
    {
        /*ModemPPP*/
        if ((usLen > 0) && (0x7e == pData[0]) && (0x7e == pData[usLen - 1]))
        {
            /*BSP*/
            AT_ModemFreeUlDataBuf(ucIndex, pstData);
            return AT_SUCCESS;
        }

        ulRet = At_CmdStreamPreProc(ucIndex,pData,usLen);

        /*BSP*/
        AT_ModemFreeUlDataBuf(ucIndex, pstData);
        return ulRet;
    }

    /* modem*/
    switch ( gastAtClientTab[ucIndex].DataMode )
    {
        case AT_PPP_DATA_MODE:

            /* (AT2D17549)MAC 10.6.2.
               "+++"DTR
            */
            if (3 == usLen)
            {
                if (('+' == pData[0]) && ('+' == pData[1]) && ('+' == pData[2]))
                {
                    /*DTR*/
                    TAF_MEM_SET_S(&stMscStru, (VOS_SIZE_T)sizeof(stMscStru), 0x00, (VOS_SIZE_T)sizeof(stMscStru));
                    stMscStru.OP_Dtr = 1;
                    stMscStru.ucDtr  = 0;
                    At_ModemMscInd(ucIndex, ucDlci, &stMscStru);
                    break;
                }
            }
            /* PPP */
            PPP_PullPacketEvent(gastAtClientTab[ucIndex].usPppId, pstData);
            return AT_SUCCESS;

        case AT_IP_DATA_MODE:
            if (3 == usLen)
            {
                if (('+' == pData[0]) && ('+' == pData[1]) && ('+' == pData[2]))
                {
                    /*DTR*/
                    TAF_MEM_SET_S(&stMscStru, (VOS_SIZE_T)sizeof(stMscStru), 0x00, (VOS_SIZE_T)sizeof(stMscStru));
                    stMscStru.OP_Dtr = 1;
                    stMscStru.ucDtr  = 0;
                    At_ModemMscInd(ucIndex, ucDlci, &stMscStru);
                    break;
                }
            }
            /* PPP */
            PPP_PullRawDataEvent(gastAtClientTab[ucIndex].usPppId, pstData);
            return AT_SUCCESS;

        /* Modified by s62952 for AT Project2011-10-17,  Begin*/
        case AT_CSD_DATA_MODE:
            /* Added by w00199382 for PS Project2011-12-06,  Begin*/
            /* Added by w00199382 for PS Project2011-12-06,  End*/
         /* Modified by s62952 for AT Project2011-10-17,  end*/

        default:
            AT_WARN_LOG("At_ModemDataInd: DataMode Wrong!");
            break;
    }

    /**/
    AT_ModemFreeUlDataBuf(ucIndex, pstData);
    /* Modified by L60609 for PS Project2011-12-06,  End*/
    return AT_SUCCESS;
}

/*****************************************************************************
    : AT_ModemInitUlDataBuf
  : MODEM
  : ucIndex        ----   
            ulEachBuffSize ----   BUFFER
            ulTotalBuffNum ----   BUFFER
  : 
    : AT_SUCCESS     ----   
            AT_FAILURE     ----   
  :
  :

      :
 1.       : 20111017
          : sunshaohua
      : 
 2.       : 20130528
          : f00179208
      : V3R3 PPP PROJECT
*****************************************************************************/
VOS_UINT32 AT_ModemInitUlDataBuf(
    VOS_UINT8                           ucIndex,
    VOS_UINT32                          ulEachBuffSize,
    VOS_UINT32                          ulTotalBuffNum
)
{
    ACM_READ_BUFF_INFO                  stReadBuffInfo;
    VOS_INT32                           ulResult;


    /*  */
    stReadBuffInfo.u32BuffSize = ulEachBuffSize;
    stReadBuffInfo.u32BuffNum  = ulTotalBuffNum;

    ulResult= mdrv_udi_ioctl(g_alAtUdiHandle[ucIndex], ACM_IOCTL_RELLOC_READ_BUFF, &stReadBuffInfo);

    if ( VOS_OK != ulResult )
    {
        AT_ERR_LOG1("AT_ModemInitUlDataBuf, WARNING, Initialize data buffer failed code %d!\r\n",
                  ulResult);

        return AT_FAILURE;
    }

    return AT_SUCCESS;
}

/*****************************************************************************
    : AT_ModemFreeUlDataBuf
  : 
  : pstBuf     ----      
  : 
    : AT_SUCCESS ----      
            AT_FAILURE ----      
  :
  :

      :
 1.       : 20111017
          : sunshaohua
      : 
 2.       : 2011128
          : /l60609
      : BufIMM_ZC_STRU
 3.       : 20130528
          : f00179208
      : V3R3 PPP PROJECT
*****************************************************************************/
VOS_UINT32 AT_ModemFreeUlDataBuf(
    VOS_UINT8                           ucIndex,
    IMM_ZC_STRU                        *pstBuf
)
{
    ACM_WR_ASYNC_INFO                   stCtlParam;
    VOS_INT32                           ulResult;

    /* Modified by L60609 for PS Project2011-12-06,  Begin*/
    /*  */
    stCtlParam.pVirAddr = (VOS_CHAR*)pstBuf;
    stCtlParam.pPhyAddr = VOS_NULL_PTR;
    stCtlParam.u32Size  = 0;
    stCtlParam.pDrvPriv = VOS_NULL_PTR;
    /* Modified by L60609 for PS Project2011-12-06,  End*/

    ulResult = mdrv_udi_ioctl(g_alAtUdiHandle[ucIndex], ACM_IOCTL_RETURN_BUFF, &stCtlParam);

    if ( VOS_OK != ulResult )
    {
        AT_ERR_LOG1("AT_ModemFreeUlDataBuf, ERROR, Return modem buffer failed, code %d!\r\n",
                  ulResult);
        AT_MODEM_DBG_UL_RETURN_BUFF_FAIL_NUM(1);
        return AT_FAILURE;
    }

    AT_MODEM_DBG_UL_RETURN_BUFF_SUCC_NUM(1);

    return AT_SUCCESS;
}

/*****************************************************************************
    : AT_ModemFreeDlDataBuf
  : PPP
  : pstBuf     ----      
  : 
    : AT_SUCCESS ----      
            AT_FAILURE ----      
  :
  :

      :
 1.       : 20111017
          : sunshaohua
      : 
 2.       : 2011128
          : /l60609
      : BufIMM_ZC_STRU
*****************************************************************************/
VOS_VOID AT_ModemFreeDlDataBuf(
    IMM_ZC_STRU                        *pstBuf
)
{
    AT_MODEM_DBG_DL_FREE_BUFF_NUM(1);

    /* Modified by L60609 for PS Project2011-12-06,  Begin*/
    /* pstBuf */
    IMM_ZcFree(pstBuf);
    /* Modified by L60609 for PS Project2011-12-06,  End*/
    return;
}

/*****************************************************************************
     : AT_ModemWriteData
   : MODEM
   : ucIndex    ----      
             pstBuf     ----      
   : 
     : AT_SUCCESS ----      
             AT_FAILURE ----      
   :
   :

       :
  1.       : 20111018
           : sunshaohua
       : 

  2.       : 2011128
           : /l60609
       : BufIMM_ZC_STRU

  3.       : 20130528
           : f00179208
       : V3R3 PPP PROJECT

  4.       : 2013921
           : j00174725
       : UART-MODEM: 
*****************************************************************************/
VOS_UINT32 AT_ModemWriteData(
    VOS_UINT8                           ucIndex,
    IMM_ZC_STRU                        *pstBuf
)
{
    ACM_WR_ASYNC_INFO                   stCtlParam;
    VOS_INT32                           ulResult;

    /*  */
    stCtlParam.pVirAddr                 = (VOS_CHAR*)pstBuf;
    stCtlParam.pPhyAddr                 = VOS_NULL_PTR;
    stCtlParam.u32Size                  = 0;
    stCtlParam.pDrvPriv                 = VOS_NULL_PTR;

    if (UDI_INVALID_HANDLE == g_alAtUdiHandle[ucIndex])
    {
        AT_ModemFreeDlDataBuf(pstBuf);
        return AT_FAILURE;
    }

    /* */
    ulResult = mdrv_udi_ioctl(g_alAtUdiHandle[ucIndex], ACM_IOCTL_WRITE_ASYNC, &stCtlParam);

    if (VOS_OK != ulResult)
    {
        AT_WARN_LOG("AT_ModemWriteData: Write data failed with code!\r\n");
        AT_MODEM_DBG_DL_WRITE_ASYNC_FAIL_NUM(1);
        AT_ModemFreeDlDataBuf(pstBuf);
        return AT_FAILURE;
    }

    AT_MODEM_DBG_DL_WRITE_ASYNC_SUCC_NUM(1);

    return AT_SUCCESS;
}

/*****************************************************************************
     : AT_SendDataToModem
   : modem
   : pucDataBuf   ----    
             usLen        ----    
   :
     : AT_SUCCESS ----      
             AT_FAILURE ----      
   :
   :

       :
  1.       : 20111018
           : sunshaohua
       : 

  2.       : 2011128
           : /l60609
       : BufIMM_ZC_STRU

  3.       : 2012831
           : l60609
       : APIMM

  4.       : 20130528
           : f00179208
       : V3R3 PPP PROJECT

  5.       : 2013921
           : j00174725
       : UART-MODEM: , 
*****************************************************************************/
VOS_UINT32 AT_SendDataToModem(
    VOS_UINT8                           ucIndex,
    VOS_UINT8                          *pucDataBuf,
    VOS_UINT16                          usLen
)
{
    IMM_ZC_STRU                        *pstData;
    VOS_CHAR                           *pstZcPutData;

    pstData = VOS_NULL_PTR;

    /* Modified by L60609 for PS Project, 2011-12-06,  Begin*/
    pstData = IMM_ZcStaticAlloc((VOS_UINT16)usLen);

    if (VOS_NULL_PTR == pstData)
    {
        return AT_FAILURE;
    }

    /**/
    /* Modified by l60609 for AP 2012-08-30 Begin */
    pstZcPutData = (VOS_CHAR *)IMM_ZcPut(pstData, usLen);
    /* Modified by l60609 for AP 2012-08-30 End */

    TAF_MEM_CPY_S(pstZcPutData, usLen, pucDataBuf, usLen);

    /*MODEM*/
    if (AT_SUCCESS != AT_ModemWriteData(ucIndex, pstData))
    {
        return AT_FAILURE;
    }
    /* Modified by L60609 for PS Project, 2011-12-06,  End*/

    return AT_SUCCESS;
}

/*****************************************************************************
     : AT_SendZcDataToModem
   : SK_buffmodem
   : pucDataBuf   ----    
             usLen        ----    
   :
     : AT_SUCCESS ----      
             AT_FAILURE ----      
   :
   :

       :
  1.       : 20111018
           : sunshaohua
       : 

  2.       : 2011128
           : /l60609
       : BufIMM_ZC_STRU

  3.       : 20130528
           : f00179208
       : V3R3 PPP PROJECT

  4.       : 2013921
           : j00174725
       : UART-MODEM: UART

  5.       : 2015331
           : A00165503
       : DTS2015032704953: HSUARTCMD/ONLINE_CMD,
                 HSUART, , 
                 
*****************************************************************************/
VOS_UINT32 AT_SendZcDataToModem(
    VOS_UINT16                          usPppId,
    IMM_ZC_STRU                        *pstDataBuf
)
{
    VOS_UINT32                          ulResult;
    VOS_UINT8                           ucIndex;

    ucIndex = gastAtPppIndexTab[usPppId];

    if ( (AT_CMD_MODE        == gastAtClientTab[ucIndex].Mode)
      || (AT_ONLINE_CMD_MODE == gastAtClientTab[ucIndex].Mode) )
    {
        IMM_ZcFree(pstDataBuf);
        return AT_FAILURE;
    }

    switch (gastAtClientTab[ucIndex].UserType)
    {
        case AT_MODEM_USER:
            ulResult = AT_ModemWriteData(ucIndex, pstDataBuf);
            break;


        default:
            IMM_ZcFree(pstDataBuf);
            ulResult = AT_FAILURE;
            break;
    }

    return ulResult;
}

/*****************************************************************************
     : AT_SendCsdZcDataToModem
   : SK_buffmodem
   : pucDataBuf   ----    
             usLen        ----    
   :
     : AT_SUCCESS ----      
             AT_FAILURE ----      
   :
   :

       :
  1.       : 20130528
           : f00179208
       : V3R3 PPP PROJECT

  2.       : 2013921
           : j00174725
       : UART-MODEM: , 
*****************************************************************************/
VOS_UINT32 AT_SendCsdZcDataToModem(
    VOS_UINT8                           ucIndex,
    IMM_ZC_STRU                        *pstDataBuf
)
{
    /*MODEM*/
    if (AT_SUCCESS != AT_ModemWriteData(ucIndex, pstDataBuf))
    {
        return AT_FAILURE;
    }

    return AT_SUCCESS;
}

/*****************************************************************************
    : AT_UsbModemEnableCB
  : MODEM
  : ucEnable    ----  
  : 
    : 
  :
  :

      :
 1.       : 20111017
          : sunshaohua
      : 
 2.       : 20130527
          : f00179208
      : V3R3 PPP PROJECT
*****************************************************************************/
VOS_VOID AT_UsbModemEnableCB(PS_BOOL_ENUM_UINT8 ucEnable)
{
    VOS_UINT8                           ucIndex;

    ucIndex = AT_CLIENT_TAB_MODEM_INDEX;

    AT_ModemeEnableCB(ucIndex, ucEnable);

    return;
}

/*****************************************************************************
     : AT_UsbModemReadDataCB
   : MODEM
   : VOS_VOID
   : 
     : VOS_VOID
   :
   :

       :
 1.       : 20111018
          : sunshaohua
      : 
 2.       : 2011128
          : /l60609
      : BufIMM_ZC_STRU
 3.       : 20130525
          : f00179208
      : V3R3 PPP PROJECT
*****************************************************************************/
VOS_VOID AT_UsbModemReadDataCB( VOS_VOID )
{
    VOS_UINT8                           ucIndex;
    VOS_UINT8                           ucDlci;
    IMM_ZC_STRU                        *pstBuf;

    pstBuf          = VOS_NULL_PTR;

    /* HSIC MODEM */
    ucIndex     = AT_CLIENT_TAB_MODEM_INDEX;

    AT_MODEM_DBG_UL_DATA_READ_CB_NUM(1);

    /* Modified by L60609 for PS Project2011-12-06,  Begin*/
    if (AT_SUCCESS == AT_ModemGetUlDataBuf(ucIndex, &pstBuf))
    {

        /*MODEM */
        ucDlci      = AT_MODEM_USER_DLCI;

        /*  */
        At_ModemDataInd(ucIndex, ucDlci, pstBuf);
    }
    /* Modified by L60609 for PS Project2011-12-06,  End*/

    return;
}

/*****************************************************************************
    : AT_UsbModemReadMscCB
  : 
  : pstRcvedMsc ----      
  : 
    : AT_SUCCESS  ----      
            AT_FAILURE  ----      
  :
  :

      :
 1.       : 20111017
          : sunshaohua
      : 
 2.       : 20130525
          : f00179208
      : V3R3 PPP PROJECT
*****************************************************************************/
VOS_VOID AT_UsbModemReadMscCB(AT_DCE_MSC_STRU *pstRcvedMsc)
{
    VOS_UINT8                           ucIndex;
    VOS_UINT8                           ucDlci;

    if (VOS_NULL_PTR == pstRcvedMsc)
    {
        AT_WARN_LOG("AT_UsbModemReadMscCB, WARNING, Receive NULL pointer MSC info!");

        return;
    }

    /* MODEM */
    ucIndex     = AT_CLIENT_TAB_MODEM_INDEX;

    /*MODEM */
    ucDlci      = AT_MODEM_USER_DLCI;

     /*  */
    AT_MNTN_TraceInputMsc(ucIndex, pstRcvedMsc);

    At_ModemMscInd(ucIndex, ucDlci, pstRcvedMsc);

    return;
}

/*****************************************************************************
    : AT_UsbModemInit
  : MODEM
  : VOS_VOID
  : 
    : VOS_VOID
  :
  :

      :
 1.       : 20111017
          : sunshaohua
      : 
 2.       : 20130527
          : f00179208
      : V3R3 PPP PROJECT
 3.       : 20131106
          : j00174725
      : V3R3 
 4.       : 20151022
          : y00213812
      : copy
*****************************************************************************/
VOS_VOID AT_UsbModemInit( VOS_VOID )
{
    UDI_OPEN_PARAM_S                    stParam;
    VOS_UINT8                           ucIndex;

    ucIndex         = AT_CLIENT_TAB_MODEM_INDEX;
    stParam.devid   = UDI_ACM_MODEM_ID;

    /* DeviceID */
    g_alAtUdiHandle[ucIndex] = mdrv_udi_open(&stParam);

    if (UDI_INVALID_HANDLE == g_alAtUdiHandle[ucIndex])
    {
        AT_ERR_LOG("AT_UsbModemInit, ERROR, Open usb modem device failed!");

        return;
    }

    /* MODEM */
    if (VOS_OK != mdrv_udi_ioctl (g_alAtUdiHandle[ucIndex], ACM_IOCTL_SET_READ_CB, AT_UsbModemReadDataCB))
    {
        AT_ERR_LOG("AT_UsbModemInit, ERROR, Set data read callback for modem failed!");

        return;
    }

    /* MODEM */
    if (VOS_OK != mdrv_udi_ioctl (g_alAtUdiHandle[ucIndex], ACM_IOCTL_SET_FREE_CB, AT_ModemFreeDlDataBuf))
    {
        AT_ERR_LOG("AT_UsbModemInit, ERROR, Set memory free callback for modem failed!");

        return;
    }

    /* MODEM */
    if (VOS_OK != mdrv_udi_ioctl (g_alAtUdiHandle[ucIndex], ACM_IOCTL_WRITE_DO_COPY, (void *)0))
    {
        AT_ERR_LOG("AT_UsbModemInit, ERROR, Set not do copy for modem failed!");

        return;
    }

    /*  */
    if (VOS_OK != mdrv_udi_ioctl (g_alAtUdiHandle[ucIndex], ACM_MODEM_IOCTL_SET_MSC_READ_CB, AT_UsbModemReadMscCB))
    {
        AT_ERR_LOG("AT_UsbModemInit, ERROR, Set msc read callback for modem failed!");

        return;
    }

    /* MODEM */
    if (VOS_OK != mdrv_udi_ioctl (g_alAtUdiHandle[ucIndex], ACM_MODEM_IOCTL_SET_REL_IND_CB, AT_UsbModemEnableCB))
    {
        AT_ERR_LOG("AT_UsbModemInit, ERROR, Set enable callback for modem failed!");

        return;
    }

    /* MODEMbuffer */
    AT_ModemInitUlDataBuf(ucIndex, AT_MODEM_UL_DATA_BUFF_SIZE, AT_MODEM_UL_DATA_BUFF_NUM);

    /* MODME */
    AT_InitModemStats();

    /*client id*/
    At_ModemEst(ucIndex, AT_CLIENT_ID_MODEM, AT_USB_MODEM_PORT_NO);

    AT_ConfigTraceMsg(ucIndex, ID_AT_CMD_MODEM, ID_AT_MNTN_RESULT_MODEM);

    return;
}

/*****************************************************************************
     : AT_UsbModemClose
   : USBMODEM
   : VOS_VOID
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 201216
           :  l00171473
       : 
  2.       : 2012522
           : f00179208
       : DTS2012052205142, VIDEO PHONE
*****************************************************************************/
VOS_VOID AT_UsbModemClose(VOS_VOID)
{
    AT_CLIENT_TAB_INDEX_UINT8           ucIndex;

    ucIndex = AT_CLIENT_TAB_MODEM_INDEX;

    /* MODEM(TTF) */

    AT_DeRegModemPsDataFCPoint(ucIndex, AT_GET_RABID_FROM_EXRABID(gastAtClientTab[ucIndex].ucExPsRabId));

    if (UDI_INVALID_HANDLE != g_alAtUdiHandle[ucIndex])
    {
        mdrv_udi_close(g_alAtUdiHandle[ucIndex]);

        g_alAtUdiHandle[ucIndex] = UDI_INVALID_HANDLE;

        (VOS_VOID)vos_printf("AT_UsbModemClose....\n");
    }

    return;
}

/*****************************************************************************
     : AT_SetUsbDebugFlag
   : 
   : VOS_UINT32 ulFlag
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 20130410
           : f00179208
       : 
*****************************************************************************/
VOS_VOID AT_SetUsbDebugFlag(VOS_UINT32 ulFlag)
{
    g_ulAtUsbDebugFlag = ulFlag;
}

/*****************************************************************************
 Prototype      : At_RcvFromUsbCom
 Description    : ATUSB COM
 Input          : ucPortNo     --    
                  pucData      --    AT
                  uslength   --    AT
 Output         : ---
 Return Value   : AT_DRV_FAILURE --- 
                  AT_DRV_FAILURE --- 
 Calls          : ---
 Called By      : ---

 History        : ---
  1.Date        : 2008-06-09
    Author      : L47619
    Modification: Created function
  2.Date        : 2012-02-24
    Author      : L47619
    Modification: V7R1C50 IPC:HSIC ATAT
  3.       : 201283
           : L60609
       : MUXMUX ATAT
*****************************************************************************/

VOS_INT At_RcvFromUsbCom(
    VOS_UINT8                           ucPortNo,
    VOS_UINT8                          *pData,
    VOS_UINT16                          uslength
)
{
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulRet;
    /* Added by L60609 for MUX2012-08-08,  Begin */
    VOS_UINT32                          ulMuxUserFlg;
    VOS_UINT32                          ulHsicUserFlg;
    /* Added by L60609 for MUX2012-08-08,  End */

    if (VOS_TRUE == g_ulAtUsbDebugFlag)
    {
        (VOS_VOID)vos_printf("At_RcvFromUsbCom: PortNo = %d, length = %d, data = %s\r\n", ucPortNo, uslength, pData);
    }

    if (VOS_NULL_PTR == pData)
    {
        AT_WARN_LOG("At_RcvFromUsbCom: pData is NULL PTR!");
        return AT_DRV_FAILURE;
    }

    if (0 == uslength)
    {
        AT_WARN_LOG("At_RcvFromUsbCom: uslength is 0!");
        return AT_DRV_FAILURE;
    }

    /*PCUICTRL*/
    for (ucIndex = 0; ucIndex < AT_MAX_CLIENT_NUM; ucIndex++)
    {
        /* Modified by L60609 for MUX2012-08-03,  Begin */
        ulMuxUserFlg = AT_CheckMuxUser(ucIndex);
        ulHsicUserFlg = AT_CheckHsicUser(ucIndex);

        if ((AT_USBCOM_USER == gastAtClientTab[ucIndex].UserType)
         || (AT_CTR_USER == gastAtClientTab[ucIndex].UserType)
         || (AT_PCUI2_USER == gastAtClientTab[ucIndex].UserType)
         || (AT_UART_USER == gastAtClientTab[ucIndex].UserType)
         || (VOS_TRUE == ulMuxUserFlg)
         || (VOS_TRUE == ulHsicUserFlg))
        {
            if (AT_CLIENT_NULL != gastAtClientTab[ucIndex].ucUsed)
            {
                if (gastAtClientTab[ucIndex].ucPortNo == ucPortNo)
                {
                    break;
                }
            }
        }
        /* Modified by L60609 for MUX2012-08-03,  End */
    }

    if (VOS_TRUE == g_ulAtUsbDebugFlag)
    {
        (VOS_VOID)vos_printf("At_RcvFromUsbCom: ucIndex = %d\r\n", ucIndex);
    }

    if (ucIndex >= AT_MAX_CLIENT_NUM)
    {
        AT_WARN_LOG("At_RcvFromUsbCom (ucIndex >= AT_MAX_CLIENT_NUM)");
        return AT_DRV_FAILURE;
    }

    if (VOS_TRUE == g_ulAtUsbDebugFlag)
    {
        (VOS_VOID)vos_printf("At_RcvFromUsbCom: CmdMode = %d\r\n", gastAtClientTab[ucIndex].Mode);
    }

    if (AT_CMD_MODE == gastAtClientTab[ucIndex].Mode)
    {
        ulRet = At_CmdStreamPreProc(ucIndex,pData,uslength);
    }
    else
    {
        ulRet = At_DataStreamPreProc(ucIndex,gastAtClientTab[ucIndex].DataMode,pData,uslength);
    }

    if ( AT_SUCCESS == ulRet )
    {
        return AT_DRV_SUCCESS;
    }
    else
    {
        return AT_DRV_FAILURE;
    }
}


/*****************************************************************************
     : At_UsbPcuiEst
   : USB PCUI
   : TAF_UINT8 ucPortNo
   : 
     : VOS_OK
             VOS_ERROR
   :
   :

       :
  1.       : 2008-06-09
           : L47619
       : Created function
  2.       : 2010716
           : /f62575
       : DTS2010071402189ATCLIENT ID
  3.       : 2010914
           : z00161729
       : DTS2010090901291:PC VOICE,PC///
  4.       : 2011930
           : /l60609
       : AT Project: At_RegTafCallBackFunc; client id;DRV->DMS
  5.       : 20131106
           : j00174725
       : V3R3 
*****************************************************************************/
VOS_UINT32 At_UsbPcuiEst(VOS_UINT8 ucPortNo)
{
    /* Modified by L60609 for AT Project2011-10-04,  Begin*/
    VOS_UINT8                           ucIndex;

    if (AT_USB_COM_PORT_NO != ucPortNo)
    {
        AT_WARN_LOG("At_UsbPcuiEst the PortNo is error)");
        return VOS_ERR;
    }

    ucIndex = AT_CLIENT_TAB_PCUI_INDEX;

    /*  */
    TAF_MEM_SET_S(&gastAtClientTab[ucIndex], sizeof(AT_CLIENT_MANAGE_STRU), 0x00, sizeof(AT_CLIENT_MANAGE_STRU));

    AT_ConfigTraceMsg(ucIndex, ID_AT_CMD_PCUI, ID_AT_MNTN_RESULT_PCUI);

    /*  */
    gastAtClientTab[ucIndex].usClientId      = AT_CLIENT_ID_PCUI;
    gastAtClientTab[ucIndex].ucPortNo        = ucPortNo;
    gastAtClientTab[ucIndex].UserType        = AT_USBCOM_USER;
    gastAtClientTab[ucIndex].ucUsed          = AT_CLIENT_USED;

    /* PS_MEMSET */
    gastAtClientTab[ucIndex].Mode            = AT_CMD_MODE;
    gastAtClientTab[ucIndex].IndMode         = AT_IND_MODE;
    gastAtClientTab[ucIndex].DataMode        = AT_DATA_BUTT_MODE;
    gastAtClientTab[ucIndex].DataState       = AT_DATA_STOP_STATE;
    gastAtClientTab[ucIndex].CmdCurrentOpt   = AT_CMD_CURRENT_OPT_BUTT;
    g_stParseContext[ucIndex].ucClientStatus = AT_FW_CLIENT_STATUS_READY;

    /* Modified by L60609 for AT Project2011-10-14,  Begin*/
    /*DMS*/
    (VOS_VOID)DMS_COM_RCV_CALLBACK_REGI(ucPortNo, (pComRecv)At_RcvFromUsbCom);


    /* Modified by L60609 for AT Project2011-10-14,  End*/

    AT_LOG1("At_UsbPcuiEst ucIndex:",ucIndex);

    /* Modified by L60609 for AT Project2011-10-04,  End*/
    return VOS_OK;
}


/*****************************************************************************
     : At_UsbCtrEst
   : AT PIDcontrol 
   : ucPortNo    - 
   : 
     : VOS_OK
             VOS_ERROR
   :
   :

       :
  1.       : 2008712
           : s62952
       : 

  2.       : 20111003
           : /l60609
       : AT Project: At_RegTafCallBackFunc; client id;DRV->DMS

  3.       : 20111115
           : o00132663
       : ATM2

  4.      : 20130313
          : z00214637
      : BodySAR

  5.       : 20131106
           : j00174725
       : V3R3 
*****************************************************************************/
VOS_UINT32 At_UsbCtrEst(VOS_UINT8 ucPortNo)
{
    /* Modified by L60609 for AT Project2011-10-04,  Begin*/
    VOS_UINT8                           ucIndex;


    if (AT_CTR_PORT_NO != ucPortNo)
    {
        AT_WARN_LOG("At_UsbCtrEst the PortNo is error)");
        return VOS_ERR;
    }

    ucIndex = AT_CLIENT_TAB_CTRL_INDEX;

    /*  */
    TAF_MEM_SET_S(&gastAtClientTab[ucIndex], sizeof(AT_CLIENT_MANAGE_STRU), 0x00, sizeof(AT_CLIENT_MANAGE_STRU));

    AT_ConfigTraceMsg(ucIndex, ID_AT_CMD_CTRL, ID_AT_MNTN_RESULT_CTRL);

    /*  */
    gastAtClientTab[ucIndex].usClientId      = AT_CLIENT_ID_CTRL;
    gastAtClientTab[ucIndex].ucPortNo        = ucPortNo;
    gastAtClientTab[ucIndex].UserType        = AT_CTR_USER;
    gastAtClientTab[ucIndex].ucUsed          = AT_CLIENT_USED;

    /* PS_MEMSET */
    gastAtClientTab[ucIndex].Mode            = AT_CMD_MODE;
    gastAtClientTab[ucIndex].IndMode         = AT_IND_MODE;
    gastAtClientTab[ucIndex].DataMode        = AT_DATA_BUTT_MODE;
    gastAtClientTab[ucIndex].DataState       = AT_DATA_STOP_STATE;
    gastAtClientTab[ucIndex].CmdCurrentOpt   = AT_CMD_CURRENT_OPT_BUTT;
    g_stParseContext[ucIndex].ucClientStatus = AT_FW_CLIENT_STATUS_READY;


    /*DMS*/
    (VOS_VOID)DMS_COM_RCV_CALLBACK_REGI(ucPortNo, (pComRecv)At_RcvFromUsbCom);
    /* Modified by L60609 for AT Project2011-10-14,  End*/

    return VOS_OK;
}

/*****************************************************************************
     : At_UsbPcui2Est
   : AT PIDPCUI2
   : ucPortNo    - 
   : 
     : VOS_OK
             VOS_ERROR
   :
   :

       :
  1.       : 2015527
           : l00198894
       : TSTS
*****************************************************************************/
VOS_UINT32 At_UsbPcui2Est(VOS_UINT8 ucPortNo)
{
    VOS_UINT8                           ucIndex;

    if (AT_PCUI2_PORT_NO != ucPortNo)
    {
        AT_WARN_LOG("At_UsbPcui2Est the PortNo is error)");
        return VOS_ERR;
    }

    ucIndex = AT_CLIENT_TAB_PCUI2_INDEX;

    /*  */
    TAF_MEM_SET_S(&gastAtClientTab[ucIndex], sizeof(AT_CLIENT_MANAGE_STRU), 0x00, sizeof(AT_CLIENT_MANAGE_STRU));

    AT_ConfigTraceMsg(ucIndex, ID_AT_CMD_PCUI2, ID_AT_MNTN_RESULT_PCUI2);

    /*  */
    gastAtClientTab[ucIndex].usClientId      = AT_CLIENT_ID_PCUI2;
    gastAtClientTab[ucIndex].ucPortNo        = ucPortNo;
    gastAtClientTab[ucIndex].UserType        = AT_PCUI2_USER;
    gastAtClientTab[ucIndex].ucUsed          = AT_CLIENT_USED;

    /* PS_MEMSET */
    gastAtClientTab[ucIndex].Mode            = AT_CMD_MODE;
    gastAtClientTab[ucIndex].IndMode         = AT_IND_MODE;
    gastAtClientTab[ucIndex].DataMode        = AT_DATA_BUTT_MODE;
    gastAtClientTab[ucIndex].DataState       = AT_DATA_STOP_STATE;
    gastAtClientTab[ucIndex].CmdCurrentOpt   = AT_CMD_CURRENT_OPT_BUTT;
    g_stParseContext[ucIndex].ucClientStatus = AT_FW_CLIENT_STATUS_READY;

    /*DMS*/
    (VOS_VOID)DMS_COM_RCV_CALLBACK_REGI(ucPortNo, (pComRecv)At_RcvFromUsbCom);

    return VOS_OK;
}

/******************************************************************************
     : AT_UART_GetUlDataBuff
   : UART
   : ucIndex    - 
   : ppucData   - 
             pulLen     - 
     : AT_SUCCESS - 
             AT_FAILURE - 
   :
   :

       :
  1.       : 20111017
           : sunshaohua
       : 

  2.       : 2012810
           : L00171473
       : DTS2012082204471, TQE

  3.       : 2013925
           : j00174725
       : UART-MODEM: UDI
*****************************************************************************/
VOS_UINT32 AT_UART_GetUlDataBuff(
    VOS_UINT8                           ucIndex,
    VOS_UINT8                         **ppucData,
    VOS_UINT32                         *pulLen
)
{
    ACM_WR_ASYNC_INFO                   stCtlParam;
    UDI_HANDLE                          lUdiHandle;
    VOS_INT32                           lResult;

    lUdiHandle = g_alAtUdiHandle[ucIndex];
    if (UDI_INVALID_HANDLE == lUdiHandle)
    {
        AT_ERR_LOG("AT_UART_GetUlDataBuff: Invalid UDI handle!\r\n");
        return AT_FAILURE;
    }

    /* BUFFER */
    stCtlParam.pVirAddr = VOS_NULL_PTR;
    stCtlParam.pPhyAddr = VOS_NULL_PTR;
    stCtlParam.u32Size  = 0;
    stCtlParam.pDrvPriv = VOS_NULL_PTR;

    lResult = mdrv_udi_ioctl(lUdiHandle, UART_IOCTL_GET_RD_BUFF, &stCtlParam);
    if (VOS_OK != lResult)
    {
        AT_ERR_LOG("AT_UART_GetUlDataBuff: Get buffer failed!\r\n");
        return AT_FAILURE;
    }

    if ( (VOS_NULL_PTR == stCtlParam.pVirAddr)
      || (AT_INIT_DATA_LEN == stCtlParam.u32Size) )
    {
        AT_ERR_LOG("AT_UART_GetUlDataBuff: Data buffer error!\r\n");
        return AT_FAILURE;
    }

    *ppucData = (VOS_UINT8 *)stCtlParam.pVirAddr;
    *pulLen   = stCtlParam.u32Size;

    return AT_SUCCESS;
}

/*****************************************************************************
     : AT_UART0_WriteDlDataSync
   : UART
   : ucIndex    - 
             pucData    - 
             ulLen      - 
   : 
     : AT_SUCCESS - 
             AT_FAILURE - 
   :
   :

       :
  1.       : 20111017
           : sunshaohua
       : 

  2.       : 2011127
           :  l00171473
       : DTS2011120801675 UART

  3.       : 20131112
           : A00165503
       : UART-MODEM: UDI
*****************************************************************************/
VOS_UINT32 AT_UART_WriteDataSync(
    VOS_UINT8                           ucIndex,
    VOS_UINT8                          *pucData,
    VOS_UINT32                          ulLen
)
{
    UDI_HANDLE                          lUdiHandle;
    VOS_INT32                           lResult;

    /* UDI */
    lUdiHandle = g_alAtUdiHandle[ucIndex];
    if (UDI_INVALID_HANDLE == lUdiHandle)
    {
        AT_ERR_LOG("AT_UART_WriteDataSync: Invalid UDI handle!\r\n");
        return AT_FAILURE;
    }

    /*  */
    if ((VOS_NULL_PTR == pucData) || (0 == ulLen))
    {
        AT_ERR_LOG("AT_UART_WriteDataSync: DATA is invalid!\r\n");
        return AT_FAILURE;
    }

    lResult = mdrv_udi_write(lUdiHandle, pucData, ulLen);
    if (VOS_OK != lResult)
    {
        AT_HSUART_DBG_DL_WRITE_SYNC_FAIL_LEN(ulLen);
        AT_ERR_LOG("AT_UART_WriteDataSync: Write buffer failed!\r\n");
        return AT_FAILURE;
    }

    AT_HSUART_DBG_DL_WRITE_SYNC_SUCC_LEN(ulLen);

    return AT_SUCCESS;
}

/*****************************************************************************
     : AT_UART_SendDlData
   : UART
   : ucIndex    - 
             pucData    - 
             usLen      - 
   : 
     : AT_SUCCESS - 
             AT_FAILURE - 
   :
   :

       :
  1.       : 20130921
           : j00174725
       : 
*****************************************************************************/
VOS_UINT32 AT_UART_SendDlData(
    VOS_UINT8                           ucIndex,
    VOS_UINT8                          *pucData,
    VOS_UINT16                          usLen
)
{
    /* UART,  */
    return AT_UART_WriteDataSync(ucIndex, pucData, usLen);
}

/*****************************************************************************
     : AT_UART_SendRawDataFromOm
   : OMUART, OM
   : pucVirAddr - 
             pucPhyAddr - 
             usLen   - 
   :
     : VOS_OK  - 
             VOS_ERR - 
   :
   :

       :
  1.       : 20111018
           : sunshaohua
       : 

  2.       : 20131112
           : A00165503
       : UART-MODEM: UART
*****************************************************************************/
VOS_UINT32 AT_UART_SendRawDataFromOm(
    VOS_UINT8                          *pucVirAddr,
    VOS_UINT8                          *pucPhyAddr,
    VOS_UINT32                          ulLen
)
{
    VOS_UINT32                          ulResult;
    VOS_UINT8                           ucIndex;

    ucIndex  = AT_CLIENT_TAB_UART_INDEX;

    ulResult = AT_UART_WriteDataSync(ucIndex, pucVirAddr, (VOS_UINT16)ulLen);
    if (AT_SUCCESS != ulResult)
    {
        return VOS_ERR;
    }

    return VOS_OK;
}

/*****************************************************************************
     : AT_HSUART_UlDataReadCB
   : UART
   : VOS_VOID
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 20111018
           : sunshaohua
       : 
*****************************************************************************/
VOS_VOID AT_UART_UlDataReadCB(VOS_VOID)
{
    VOS_UINT8                          *pucData = VOS_NULL_PTR;
    VOS_UINT32                          ulLen;
    VOS_UINT8                           ucIndex;

    ulLen   = 0;
    ucIndex = AT_CLIENT_TAB_UART_INDEX;

    if (AT_SUCCESS == AT_UART_GetUlDataBuff(ucIndex, &pucData, &ulLen))
    {
        /*  */
        At_RcvFromUsbCom(AT_UART_PORT_NO, pucData, (VOS_UINT16)ulLen);
    }

    return;
}

/*****************************************************************************
     : AT_UART_InitLink
   : NVUART
   : ucIndex ---- 
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 2010928
           : A00165503
       : 

  2.       : 20111024
           : /l60609
       : AT Project: client id

  3.       : 2011127
           :  l00171473
       : DTS2011120801675, UART

  4.       : 2012810
           : L00171473
       : DTS2012082204471, TQE

  5.       : 20130517
           : m00217266
       : nv
*****************************************************************************/
VOS_VOID AT_UART_InitLink(VOS_UINT8 ucIndex)
{
    TAF_AT_NVIM_DEFAULT_LINK_OF_UART_STRU    stDefaultLinkType;


    stDefaultLinkType.enUartLinkType = AT_UART_LINK_TYPE_BUTT;


    /*  */
    TAF_MEM_SET_S(&gastAtClientTab[ucIndex], sizeof(AT_CLIENT_MANAGE_STRU), 0x00, sizeof(AT_CLIENT_MANAGE_STRU));

    gastAtClientTab[ucIndex].ucPortNo  = AT_UART_PORT_NO;
    gastAtClientTab[ucIndex].UserType  = AT_UART_USER;
    gastAtClientTab[ucIndex].ucUsed    = AT_CLIENT_USED;


    /* UARTNV */
    if (NV_OK != NV_ReadEx(MODEM_ID_0, en_NV_Item_DEFAULT_LINK_OF_UART,
                        &stDefaultLinkType.enUartLinkType,
                        sizeof(stDefaultLinkType.enUartLinkType)))
    {
        /* NVUARTOM */
        AT_ERR_LOG("AT_UART_InitLink:Read NV failed!");

        /*AT/OM*/
        gucAtOmIndex = ucIndex;

        /* OM */
        At_SetMode(ucIndex, AT_DATA_MODE, AT_OM_DATA_MODE);
        gastAtClientTab[ucIndex].DataState = AT_DATA_START_STATE;

        AT_AddUsedClientId2Tab(AT_CLIENT_TAB_UART_INDEX);

        /* OAMUARTOM */
        CBTCPM_NotifyChangePort(AT_UART_PORT);
    }
    else
    {
        /* NVUART */
        if (AT_UART_LINK_TYPE_AT != stDefaultLinkType.enUartLinkType)
        {
            AT_NORM_LOG("AT_UART_InitLink:DEFAULT UART LINK TYPE is OM!");

            /*AT/OM*/
            gucAtOmIndex = ucIndex;

            /* OM */
            At_SetMode(ucIndex, AT_DATA_MODE, AT_OM_DATA_MODE);
            gastAtClientTab[ucIndex].DataState = AT_DATA_START_STATE;

            AT_AddUsedClientId2Tab(AT_CLIENT_TAB_UART_INDEX);

            /* OAMUARTOM */
            CBTCPM_NotifyChangePort(AT_UART_PORT);
        }
        else
        {
            /*  */
            gastAtClientTab[ucIndex].usClientId      = AT_CLIENT_ID_UART;

            /* PS_MEMSET */
            gastAtClientTab[ucIndex].Mode            = AT_CMD_MODE;
            gastAtClientTab[ucIndex].IndMode         = AT_IND_MODE;
            gastAtClientTab[ucIndex].DataMode        = AT_DATA_BUTT_MODE;
            gastAtClientTab[ucIndex].DataState       = AT_DATA_STOP_STATE;
            gastAtClientTab[ucIndex].CmdCurrentOpt   = AT_CMD_CURRENT_OPT_BUTT;
            g_stParseContext[ucIndex].ucClientStatus = AT_FW_CLIENT_STATUS_READY;
        }
    }

    return;
}

/*****************************************************************************
     : AT_HSUART_InitPort
   : Uart
   : VOS_VOID
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 20111017
           : sunshaohua
       : 

  2.       : 2011127
           :  l00171473
       : DTS2011120801675: UARTUART
                 ATBuffer

  3.       : 2013921
           : j00174725
       : UART-MODEM: UDI

  4.       : 20131106
           : j00174725
       : V3R3 
*****************************************************************************/
VOS_VOID AT_UART_InitPort(VOS_VOID)
{
    UDI_OPEN_PARAM_S                    stParam;
    UDI_HANDLE                          lUdiHandle;
    VOS_UINT8                           ucIndex;

    stParam.devid = UDI_UART_0_ID;
    ucIndex       = AT_CLIENT_TAB_UART_INDEX;

    AT_ConfigTraceMsg(ucIndex, ID_AT_CMD_UART, ID_AT_MNTN_RESULT_UART);

    lUdiHandle = mdrv_udi_open(&stParam);
    if (UDI_INVALID_HANDLE != lUdiHandle)
    {
        /* UART */
        if (VOS_OK != mdrv_udi_ioctl (lUdiHandle, UART_IOCTL_SET_READ_CB, AT_UART_UlDataReadCB))
        {
            AT_ERR_LOG("AT_UART_InitPort: Reg data read callback failed!\r\n");
        }

        /* UART */
        AT_UART_InitLink(ucIndex);
        g_alAtUdiHandle[ucIndex] = lUdiHandle;
    }
    else
    {
        AT_ERR_LOG("AT_UART_InitPort: Open UART device failed!\r\n");
        g_alAtUdiHandle[ucIndex] = UDI_INVALID_HANDLE;
    }

    return;
}


/*****************************************************************************
     : AT_CtrlDCD
   : DCD
   : ucIndex   - 
             enIoLevel - 
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 2013923
           : A00165503
       : 
*****************************************************************************/
VOS_VOID AT_CtrlDCD(
    VOS_UINT8                           ucIndex,
    AT_IO_LEVEL_ENUM_UINT8              enIoLevel
)
{
    AT_DCE_MSC_STRU                     stDceMsc;
    NAS_OM_EVENT_ID_ENUM_UINT16         enEventId;

    TAF_MEM_SET_S(&stDceMsc, sizeof(stDceMsc), 0x00, sizeof(AT_DCE_MSC_STRU));

    stDceMsc.OP_Dcd = VOS_TRUE;
    stDceMsc.ucDcd  = enIoLevel;

    AT_SetModemStatus(ucIndex, &stDceMsc);

    enEventId = (AT_IO_LEVEL_HIGH == enIoLevel) ?
                NAS_OM_EVENT_DCE_UP_DCD : NAS_OM_EVENT_DCE_DOWN_DCD;

    AT_EventReport(WUEPS_PID_AT, enEventId, &ucIndex, sizeof(VOS_UINT8));

    return;
}

/*****************************************************************************
     : AT_CtrlDSR
   : DSR
   : ucIndex   - 
             enIoLevel - 
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 2013923
           : A00165503
       : 
*****************************************************************************/
VOS_VOID AT_CtrlDSR(
    VOS_UINT8                           ucIndex,
    AT_IO_LEVEL_ENUM_UINT8              enIoLevel
)
{
    AT_DCE_MSC_STRU                     stDceMsc;
    NAS_OM_EVENT_ID_ENUM_UINT16         enEventId;

    TAF_MEM_SET_S(&stDceMsc, sizeof(stDceMsc), 0x00, sizeof(AT_DCE_MSC_STRU));

    stDceMsc.OP_Dsr = VOS_TRUE;
    stDceMsc.ucDsr  = enIoLevel;

    AT_SetModemStatus(ucIndex, &stDceMsc);

    enEventId = (AT_IO_LEVEL_HIGH == enIoLevel) ?
                NAS_OM_EVENT_DCE_UP_DSR: NAS_OM_EVENT_DCE_DOWN_DSR;

    AT_EventReport(WUEPS_PID_AT, enEventId, &ucIndex, sizeof(VOS_UINT8));

    return;
}

/*****************************************************************************
     : AT_CtrlCTS
   : CTS
   : ucIndex   - 
             enIoLevel - 
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 2013923
           : A00165503
       : 
*****************************************************************************/
VOS_VOID AT_CtrlCTS(
    VOS_UINT8                           ucIndex,
    AT_IO_LEVEL_ENUM_UINT8              enIoLevel
)
{
    AT_DCE_MSC_STRU                     stDceMsc;
    NAS_OM_EVENT_ID_ENUM_UINT16         enEventId;

    TAF_MEM_SET_S(&stDceMsc, sizeof(stDceMsc), 0x00, sizeof(AT_DCE_MSC_STRU));

    stDceMsc.OP_Cts = VOS_TRUE;
    stDceMsc.ucCts  = enIoLevel;

    AT_SetModemStatus(ucIndex, &stDceMsc);

    enEventId = (AT_IO_LEVEL_HIGH == enIoLevel) ?
                NAS_OM_EVENT_DCE_UP_CTS: NAS_OM_EVENT_DCE_DOWN_CTS;

    AT_EventReport(WUEPS_PID_AT, enEventId, &ucIndex, sizeof(VOS_UINT8));

    return;
}

/*****************************************************************************
     : AT_CtrlRI
   : RI
   : ucIndex   - 
             enIoLevel - 
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 2013923
           : A00165503
       : 
*****************************************************************************/
VOS_VOID AT_CtrlRI(
    VOS_UINT8                           ucIndex,
    AT_IO_LEVEL_ENUM_UINT8              enIoLevel
)
{
    AT_DCE_MSC_STRU                     stDceMsc;
    NAS_OM_EVENT_ID_ENUM_UINT16         enEventId;

    TAF_MEM_SET_S(&stDceMsc, sizeof(stDceMsc), 0x00, sizeof(AT_DCE_MSC_STRU));

    stDceMsc.OP_Ri = VOS_TRUE;
    stDceMsc.ucRi  = enIoLevel;

    AT_SetModemStatus(ucIndex, &stDceMsc);

    enEventId = (AT_IO_LEVEL_HIGH == enIoLevel) ?
                NAS_OM_EVENT_DCE_UP_RI: NAS_OM_EVENT_DCE_DOWN_RI;

    AT_EventReport(WUEPS_PID_AT, enEventId, &ucIndex, sizeof(VOS_UINT8));

    return;
}

/*****************************************************************************
     : AT_GetIoLevel
   : 
   : ucIndex          - 
             ucIoCtrl         - :
                                IO_CTRL_FC
                                IO_CTRL_DSR
                                IO_CTRL_DTR
                                IO_CTRL_RFR
                                IO_CTRL_CTS
                                IO_CTRL_RI
                                IO_CTRL_DCD
   : 
     : AT_IO_LEVEL_LOW  - 
             AT_IO_LEVEL_HIGH - 
   :
   :

       :
  1.       : 2013923
           : A00165503
       : 
*****************************************************************************/
AT_IO_LEVEL_ENUM_UINT8 AT_GetIoLevel(
    VOS_UINT8                           ucIndex,
    VOS_UINT8                           ucIoCtrl
)
{
    if (0 == (gastAtClientTab[ucIndex].ModemStatus & ucIoCtrl))
    {
        return AT_IO_LEVEL_LOW;
    }

    return AT_IO_LEVEL_HIGH;
}

/*****************************************************************************
     : App_VcomRecvCallbackRegister
   : 
   : uPortNo
             pCallback
   : 
     : int
   :
   :

       :
  1.       : 20111017
           : sunshaohua
       : 
*****************************************************************************/
int  App_VcomRecvCallbackRegister(unsigned char  uPortNo, pComRecv pCallback)
{
    return VOS_OK;
}

/*****************************************************************************
     : At_RcvFromAppCom
   : AT
   : ucVcomId    - VCOM
             *pData      - 
             uslength    - 
   : 
     : AT_SUCCESS --- 
             AT_FAILURE --- 
   :
   :

       :
  1.       : 201097
           : s62952
       : 
  2.       : 20111020
           : /l60609
       : AT Project:  client id
  3.       : 20121128
           : l00227485
       : DSDA:8VCOM
*****************************************************************************/
VOS_INT AT_RcvFromAppCom(
    VOS_UINT8                           ucVcomId,
    VOS_UINT8                          *pData,
    VOS_UINT16                          uslength
)
{
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulRet;

    if (ucVcomId >= APP_VCOM_DEV_INDEX_BUTT)
    {
        AT_WARN_LOG("AT_RcvFromAppCom: Port No ERR!");
        return VOS_ERR;
    }
    /* APPVCOMAT */
    if (ucVcomId >= AT_VCOM_AT_CHANNEL_MAX)
    {
        AT_WARN_LOG("AT_RcvFromAppCom: Port No ERR!");
        return VOS_ERR;
    }
    if (VOS_NULL_PTR == pData)
    {
        AT_WARN_LOG("AT_RcvFromAppCom: pData is NULL PTR!");
        return VOS_ERR;
    }

    if (0 == uslength)
    {
        AT_WARN_LOG("AT_RcvFromAppCom: uslength is 0!");
        return VOS_ERR;
    }

    /* Index */
    ucIndex = AT_CLIENT_TAB_APP_INDEX + ucVcomId;

    /* APP */
    if ((AT_APP_USER != gastAtClientTab[ucIndex].UserType)
     || (AT_CLIENT_NULL == gastAtClientTab[ucIndex].ucUsed))
    {
        AT_WARN_LOG("AT_RcvFromAppCom: APP client is unused!");
        return VOS_ERR;
    }

    if (AT_CMD_MODE == gastAtClientTab[ucIndex].Mode)
    {
        ulRet = At_CmdStreamPreProc(ucIndex, pData, uslength);
    }
    else
    {
        ulRet = At_DataStreamPreProc(ucIndex, gastAtClientTab[ucIndex].DataMode, pData, uslength);
    }

    if ( AT_SUCCESS == ulRet )
    {
        return VOS_OK;
    }
    else
    {
        return VOS_ERR;
    }
}

/*****************************************************************************
     : AT_AppComEst
   : AT PIDAPP
   : 
   : 
     : AT_SUCCESS --- 
             AT_FAILURE --- 
   :
   :

       :
  1.       : 201097
           : s62952
       : 
  2.       : 2012910
           : l60609
       : APCHDATA
  3.       : 20121129
           : l00227485
       : DSDA:VCOM AT
  4.       : 20131106
           : j00174725
       : V3R3 
*****************************************************************************/
VOS_INT32 AT_AppComEst(VOS_VOID)
{
    VOS_UINT8                           ucIndex;
    VOS_UINT8                           ucLoop;

    for (ucLoop = 0; ucLoop < AT_VCOM_AT_CHANNEL_MAX; ucLoop++)
    {
        ucIndex = AT_CLIENT_TAB_APP_INDEX + ucLoop;

        /*  */
        TAF_MEM_SET_S(&gastAtClientTab[ucIndex], sizeof(AT_CLIENT_MANAGE_STRU), 0x00, sizeof(AT_CLIENT_MANAGE_STRU));

        AT_ConfigTraceMsg(ucIndex, (ID_AT_CMD_APP + ucLoop), (ID_AT_MNTN_RESULT_APP + ucLoop));

        gastAtClientTab[ucIndex].usClientId     = AT_CLIENT_ID_APP + ucLoop;

        /*  */
        gastAtClientTab[ucIndex].ucPortNo        = APP_VCOM_DEV_INDEX_0 + ucLoop;
        gastAtClientTab[ucIndex].UserType        = AT_APP_USER;
        gastAtClientTab[ucIndex].ucUsed          = AT_CLIENT_USED;
        gastAtClientTab[ucIndex].Mode            = AT_CMD_MODE;
        gastAtClientTab[ucIndex].IndMode         = AT_IND_MODE;
        gastAtClientTab[ucIndex].DataMode        = AT_DATA_BUTT_MODE;
        gastAtClientTab[ucIndex].DataState       = AT_DATA_STOP_STATE;
        gastAtClientTab[ucIndex].CmdCurrentOpt   = AT_CMD_CURRENT_OPT_BUTT;
        g_stParseContext[ucIndex].ucClientStatus = AT_FW_CLIENT_STATUS_READY;

        /*  */
        APP_VCOM_RegDataCallback(gastAtClientTab[ucIndex].ucPortNo, (SEND_UL_AT_FUNC)AT_RcvFromAppCom);

    }

    return VOS_OK;
}

/*****************************************************************************
     : AT_RcvFromSock
   : ATSOCK
   : ucPortNo    - 
             *pData      - 
             uslength    - 
   : 
     : AT_SUCCESS --- 
             AT_FAILURE --- 

   :
   :

       :
  1.       : 2010913
           : s62952
       : 
  2.       : 20111018
           : /l60609
       : AT Project:  client id

*****************************************************************************/
VOS_INT AT_RcvFromSock(
    VOS_UINT8                          *pData,
    VOS_UINT32                         uslength
)
{
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulRet;

    ucIndex = AT_CLIENT_TAB_SOCK_INDEX;

    if (VOS_NULL_PTR == pData)
    {
        AT_WARN_LOG("AT_RcvFromSock: pData is NULL PTR!");
        return VOS_ERR;
    }
    if (0 == uslength)
    {
        AT_WARN_LOG("AT_RcvFromSock: uslength is 0!");
        return VOS_ERR;
    }

    if ((AT_SOCK_USER != gastAtClientTab[ucIndex].UserType)
        ||(AT_CLIENT_NULL == gastAtClientTab[ucIndex].ucUsed))
    {
        AT_WARN_LOG("AT_RcvFromSock: SOCK client is unused!");
        return VOS_ERR;
    }

    if (AT_CMD_MODE == gastAtClientTab[ucIndex].Mode)
    {
        ulRet = At_CmdStreamPreProc(ucIndex,pData,(VOS_UINT16)uslength);
    }
    else
    {
        ulRet = At_DataStreamPreProc(ucIndex,gastAtClientTab[ucIndex].DataMode,pData,(VOS_UINT16)uslength);
    }
    if ( AT_SUCCESS == ulRet )
    {
        return VOS_OK;
    }
    else
    {
        return VOS_ERR;
    }
}

/*****************************************************************************
     : AT_SockComEst
   : AT PIDsock
   : ucPortNo    - 
   : 
     : AT_SUCCESS --- 
             AT_FAILURE --- 
   :
   :

       :
  1.       : 201097
           : s62952
       : 

  2.       : 2011103
           : /l60609
       : AT Project: At_RegTafCallBackFunc; client id

  3.       : 20131106
           : j00174725
       : V3R3 
*****************************************************************************/
VOS_INT32 AT_SockComEst(VOS_UINT8 ucPortNo)
{

    VOS_UINT8                           ucIndex;

    ucIndex = AT_CLIENT_TAB_SOCK_INDEX;

    if (AT_SOCK_PORT_NO != ucPortNo)
    {
        AT_WARN_LOG("At_SockComEst the PortNo is error)");
        return VOS_ERR;
    }

    TAF_MEM_SET_S(&gastAtClientTab[ucIndex], sizeof(AT_CLIENT_MANAGE_STRU), 0x00, sizeof(AT_CLIENT_MANAGE_STRU));

    AT_ConfigTraceMsg(ucIndex, ID_AT_CMD_SOCK, ID_AT_MNTN_RESULT_SOCK);

    gastAtClientTab[ucIndex].usClientId      = AT_CLIENT_ID_SOCK;
    gastAtClientTab[ucIndex].ucPortNo        = ucPortNo;
    gastAtClientTab[ucIndex].UserType        = AT_SOCK_USER;
    gastAtClientTab[ucIndex].ucUsed          = AT_CLIENT_USED;
    gastAtClientTab[ucIndex].Mode            = AT_CMD_MODE;
    gastAtClientTab[ucIndex].IndMode         = AT_IND_MODE;
    gastAtClientTab[ucIndex].DataMode        = AT_DATA_BUTT_MODE;
    gastAtClientTab[ucIndex].DataState       = AT_DATA_STOP_STATE;
    gastAtClientTab[ucIndex].CmdCurrentOpt   = AT_CMD_CURRENT_OPT_BUTT;
    g_stParseContext[ucIndex].ucClientStatus = AT_FW_CLIENT_STATUS_READY;

    /* Modified by s62952 for AT Project2011-10-17,  Begin*/
    /*DMS*/
    (VOS_VOID)CPM_LogicRcvReg(CPM_AT_COMM,(CBTCPM_RCV_FUNC)AT_RcvFromSock);
    /* Modified by s62952 for AT Project2011-10-17,  end*/

    return VOS_OK;
}

/*****************************************************************************
     : AT_RcvFromAppSock
   : AT
   : ucPortNo    - 
             *pData      - 
             uslength    - 
   : 
     : AT_SUCCESS --- 
             AT_FAILURE --- 
   :
   :

       :
  1.       : 201097
           : s62952
       : 
  2.       : 20111018
           : /l60609
       : AT Project:  client id

*****************************************************************************/
VOS_INT AT_RcvFromAppSock(
    VOS_UINT8                           ucPortNo,
    VOS_UINT8                          *pData,
    VOS_UINT16                          uslength
)
{
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulRet;

    ucIndex = AT_CLIENT_TAB_APPSOCK_INDEX;

    if (VOS_NULL_PTR == pData)
    {
        AT_WARN_LOG("AT_RcvFromAppSock: pData is NULL PTR!");
        return VOS_ERR;
    }

    if (0 == uslength)
    {
        AT_WARN_LOG("AT_RcvFromAppSock: uslength is 0!");
        return VOS_ERR;
    }

    if (AT_APP_SOCK_PORT_NO != ucPortNo)
    {
        AT_WARN_LOG("AT_RcvFromAppSock: Port No ERR!");
        return VOS_ERR;
    }

    if ( (AT_APP_SOCK_USER != gastAtClientTab[ucIndex].UserType)
       ||(AT_CLIENT_NULL == gastAtClientTab[ucIndex].ucUsed))
    {
        AT_WARN_LOG("AT_RcvFromAppSock: SOCK client is unused!");
        return VOS_ERR;
    }

    if (AT_CMD_MODE == gastAtClientTab[ucIndex].Mode)
    {
        ulRet = At_CmdStreamPreProc(ucIndex,pData,uslength);
    }
    else
    {
        ulRet = At_DataStreamPreProc(ucIndex,gastAtClientTab[ucIndex].DataMode,pData,uslength);
    }

    if ( AT_SUCCESS == ulRet )
    {
        return VOS_OK;
    }
    else
    {
        return VOS_ERR;
    }
}

/*****************************************************************************
     : AT_AppSockComEst
   : AT PIDE5 sock
   : ucPortNo    - 
   : 
     : AT_SUCCESS --- 
             AT_FAILURE --- 
   :
   :

       :
  1.       : 201097
           : s62952
       : 
  2.       : 2011103
           : /l60609
       : AT Project: At_RegTafCallBackFunc; client id

  3.       : 20131106
           : j00174725
       : V3R3 
*****************************************************************************/
VOS_INT32 AT_AppSockComEst(VOS_UINT8 ucPortNo)
{
    /* Modified by L60609 for AT Project2011-10-04,  Begin*/
    VOS_UINT8                           ucIndex;

    ucIndex = AT_CLIENT_TAB_APPSOCK_INDEX;

    if (AT_APP_SOCK_PORT_NO != ucPortNo)
    {
         AT_WARN_LOG("AT_E5SockComEst the PortNo is error)");
        return VOS_ERR;
    }

    AT_ConfigTraceMsg(ucIndex, ID_AT_CMD_APPSOCK, ID_AT_MNTN_RESULT_APPSOCK);

    /*  */
    TAF_MEM_SET_S(&gastAtClientTab[ucIndex], sizeof(AT_CLIENT_MANAGE_STRU), 0x00, sizeof(AT_CLIENT_MANAGE_STRU));

    /*  */
    gastAtClientTab[ucIndex].usClientId      = AT_CLIENT_ID_APPSOCK;
    gastAtClientTab[ucIndex].ucPortNo        = ucPortNo;
    gastAtClientTab[ucIndex].UserType        = AT_APP_SOCK_USER;
    gastAtClientTab[ucIndex].ucUsed          = AT_CLIENT_USED;
    gastAtClientTab[ucIndex].Mode            = AT_CMD_MODE;
    gastAtClientTab[ucIndex].IndMode         = AT_IND_MODE;
    gastAtClientTab[ucIndex].DataMode        = AT_DATA_BUTT_MODE;
    gastAtClientTab[ucIndex].DataState       = AT_DATA_STOP_STATE;
    gastAtClientTab[ucIndex].CmdCurrentOpt   = AT_CMD_CURRENT_OPT_BUTT;
    g_stParseContext[ucIndex].ucClientStatus = AT_FW_CLIENT_STATUS_READY;

    /* Modified by s62952 for AT Project2011-10-17,  Begin*/
    /*DMS*/
    (VOS_VOID)App_VcomRecvCallbackRegister(ucPortNo, (pComRecv)AT_RcvFromAppSock);
    /* Modified by s62952 for AT Project2011-10-17,  end*/

    /* Modified by L60609 for AT Project2011-10-04,  End*/

    return VOS_OK;
}

/* Modified by s62952 for BalongV300R002 Build 2012-02-28, begin */
/******************************************************************************
     : At_UsbGetWwanMode
   : NDIS
   : 
   : 
     : 1 : WCDMA
             2 : CDMA

   :
   :

       :
  1.       : 20114
           : s62952
       : 
******************************************************************************/
VOS_UINT32 At_UsbGetWwanMode(VOS_VOID)
{
    return WWAN_WCDMA;
}

/*****************************************************************************
     : AT_UsbNcmConnStatusChgCB
   : NDIS
   : enStatus --- 
             pBuffer  --- 
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 201595
           : A00165503
       : 
*****************************************************************************/
VOS_VOID AT_UsbNcmConnStatusChgCB(NCM_IOCTL_CONNECT_STUS_E enStatus, VOS_VOID *pBuffer)
{
    AT_MSG_STRU                        *pstMsg = VOS_NULL_PTR;

    /*
     * , 
     */

    if (NCM_IOCTL_STUS_BREAK == enStatus)
    {
        /* OSA */
        pstMsg = (AT_MSG_STRU *)AT_ALLOC_MSG_WITH_HDR(sizeof(AT_MSG_STRU));
        if (VOS_NULL_PTR == pstMsg)
        {
            AT_ERR_LOG("AT_UsbNcmConnStatusChgCB: Alloc message failed!");
            return;
        }

        /*  */
        TAF_MEM_SET_S(AT_GET_MSG_ENTITY(pstMsg), AT_GET_MSG_LENGTH(pstMsg), 0x00, AT_GET_MSG_LENGTH(pstMsg));

        /*  */
        AT_CFG_INTRA_MSG_HDR(pstMsg, ID_AT_NCM_CONN_STATUS_CMD);

        /*  */
        pstMsg->ucType  = AT_NCM_CONN_STATUS_MSG;
        pstMsg->ucIndex = AT_CLIENT_TAB_NDIS_INDEX;

        /*  */
        AT_SEND_MSG(pstMsg);
    }

    return;
}

/*****************************************************************************
     : At_RcvFromNdisCom
   : NDISAT
   : ucPortNo    - 
             *pucData    - 
             uslength    - 
   : 
     : AT_SUCCESS --- 
             AT_FAILURE --- 
   :
   :

       :
  1.       : 201143
           : s62952
       : 

*****************************************************************************/
VOS_INT AT_RcvFromNdisCom(
    VOS_UINT8                           *pucData,
    VOS_UINT16                          uslength
)
{
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulRet;

    /*  */
    if (VOS_NULL_PTR == pucData)
    {
        AT_WARN_LOG("At_RcvFromNdisCom: pData is NULL PTR!");
        return VOS_ERR;
    }

    /*  */
    if (0 == uslength)
    {
        AT_WARN_LOG("At_RcvFromNdisCom: uslength is 0!");
        return VOS_ERR;
    }

    ucIndex = AT_CLIENT_TAB_NDIS_INDEX;

    /* NDIS */
    if ( (AT_NDIS_USER != gastAtClientTab[ucIndex].UserType)
       ||(AT_CLIENT_NULL == gastAtClientTab[ucIndex].ucUsed))
    {
        AT_WARN_LOG("At_RcvFromNdisCom: NDIS is unused");
        return VOS_ERR;
    }

    /*NDIS*/
    DMS_SetNdisChanStatus(ACM_EVT_DEV_READY);

    if (AT_CMD_MODE == gastAtClientTab[ucIndex].Mode)
    {
        ulRet = At_CmdStreamPreProc(ucIndex,pucData,uslength);
    }
    else
    {
        ulRet = At_DataStreamPreProc(ucIndex,gastAtClientTab[ucIndex].DataMode,pucData,uslength);
    }

    if ( AT_SUCCESS == ulRet )
    {
        return VOS_OK;
    }
    else
    {
        return VOS_ERR;
    }
}

/*****************************************************************************
     : AT_UsbNdisEst
   : AT PID6.2 NDIS AT
   : VOS_VOID
   : 
     : VOS_UINT32
             AT_SUCCESS
   :
   :

       :
  1.       : 2009-08-31
           : L47619
       : Created function

  2.       : 2011103
           : /l60609
       : AT Project: At_RegTafCallBackFunc; client id

  3.       : 20131106
           : j00174725
       : V3R3 
*****************************************************************************/
VOS_UINT32 AT_UsbNdisEst(VOS_VOID)
{
    /* Modified by L60609 for AT Project2011-10-04,  Begin*/
    VOS_UINT8                           ucIndex;

    ucIndex = AT_CLIENT_TAB_NDIS_INDEX;

    /*  */
    TAF_MEM_SET_S(&gastAtClientTab[ucIndex], sizeof(AT_CLIENT_MANAGE_STRU), 0x00, sizeof(AT_CLIENT_MANAGE_STRU));

    AT_ConfigTraceMsg(ucIndex, ID_AT_CMD_NDIS, ID_AT_MNTN_RESULT_NDIS);

    /*  */
    gastAtClientTab[ucIndex].usClientId      = AT_CLIENT_ID_NDIS;
    gastAtClientTab[ucIndex].ucPortNo        = AT_NDIS_PORT_NO;
    gastAtClientTab[ucIndex].UserType        = AT_NDIS_USER;
    gastAtClientTab[ucIndex].ucUsed          = AT_CLIENT_USED;

    /* PS_MEMSET */
    gastAtClientTab[ucIndex].Mode            = AT_CMD_MODE;
    gastAtClientTab[ucIndex].IndMode         = AT_IND_MODE;
    gastAtClientTab[ucIndex].DataMode        = AT_DATA_BUTT_MODE;
    gastAtClientTab[ucIndex].DataState       = AT_DATA_STOP_STATE;
    gastAtClientTab[ucIndex].CmdCurrentOpt   = AT_CMD_CURRENT_OPT_BUTT;
    g_stParseContext[ucIndex].ucClientStatus = AT_FW_CLIENT_STATUS_READY;

    /*NDIS ADDR*/
    TAF_MEM_SET_S(&gstAtNdisAddParam, sizeof(gstAtNdisAddParam), 0x00, sizeof(AT_DIAL_PARAM_STRU));

    return VOS_OK;
}

/*****************************************************************************
    : AT_OpenUsbNdis
  : NCM
  : VOS_VOID
  : 
    : VOS_VOID
  :
  :

      :
 1.       : 20111222
          : c00173809
      : PS NCM 
*****************************************************************************/
VOS_VOID AT_OpenUsbNdis(VOS_VOID)
{
    UDI_OPEN_PARAM_S                    stParam;
    VOS_UINT32                          ulRst;

    stParam.devid   = UDI_NCM_NDIS_ID;

    /* DeviceID */
    g_ulAtUdiNdisHdl = mdrv_udi_open(&stParam);

    if (UDI_INVALID_HANDLE == g_ulAtUdiNdisHdl)
    {
        AT_ERR_LOG("AT_OpenUsbNdis, ERROR, Open usb ndis device failed!");

        return;
    }

    /* DMS */
    /*lint -e732   */
    ulRst =  DMS_USB_NAS_REGFUNC((USBNdisStusChgFunc)AT_UsbNcmConnStatusChgCB,
                                 (USB_NAS_AT_CMD_RECV)AT_RcvFromNdisCom,
                                 (USB_NAS_GET_WWAN_MODE)At_UsbGetWwanMode);
    if (VOS_OK != ulRst)
    {
        AT_ERR_LOG("AT_OpenUsbNdis, ERROR, Reg NCM failed!");

        return;
    }
    /*lint +e732*/
    /* Modified by L60609 for AT Project2011-10-04,  End*/

    return;
}

/*****************************************************************************
     : AT_CloseUsbNdis
   : NCM
   : VOS_VOID
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 20111222
           : c00173809
       : PS NCM 

  2.       : 201595
           : A00165503
       : DTS2015090105100: NDIS
*****************************************************************************/
VOS_VOID AT_CloseUsbNdis(VOS_VOID)
{
    /* NDIS */
    AT_UsbNcmConnStatusChgCB(NCM_IOCTL_STUS_BREAK, VOS_NULL_PTR);

    if (UDI_INVALID_HANDLE != g_ulAtUdiNdisHdl)
    {
        mdrv_udi_close(g_ulAtUdiNdisHdl);

        g_ulAtUdiNdisHdl = UDI_INVALID_HANDLE;
    }

    return;
}

/* Added by L60609 for MUX2012-08-03,  Begin */
/*****************************************************************************
     : AT_GetMuxSupportFlg
   : MUX
   : VOS_VOID
   : 
     : VOS_UINT8
   :
   :

       :
  1.       : 201289
           : l60609
       : 
  2.       : 201334
           : l60609
       : DSDA PHASE III
*****************************************************************************/
VOS_UINT8 AT_GetMuxSupportFlg(VOS_VOID)
{
    /* Modified by l60609 for DSDA Phase III, 2013-3-4, Begin */
    return AT_GetCommCtxAddr()->stMuxCtx.ucMuxSupportFlg;
    /* Modified by l60609 for DSDA Phase III, 2013-3-4, End */
}

/*****************************************************************************
     : AT_SetMuxSupportFlg
   : MUX
   : VOS_UINT8 ucMuxSupportFlg
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 201289
           : l60609
       : 
  2.       : 201334
           : l60609
       : DSDA PHASE III
*****************************************************************************/
VOS_VOID AT_SetMuxSupportFlg(VOS_UINT8 ucMuxSupportFlg)
{
    /* Modified by l60609 for DSDA Phase III, 2013-3-4, Begin */
    AT_COMM_CTX_STRU                   *pstCommCtx = VOS_NULL_PTR;

    pstCommCtx = AT_GetCommCtxAddr();

    pstCommCtx->stMuxCtx.ucMuxSupportFlg = ucMuxSupportFlg;
    /* Modified by l60609 for DSDA Phase III, 2013-3-4, End */
}

/*****************************************************************************
     : AT_CheckMuxDlci
   : DLCI
   : AT_MUX_DLCI_TYPE_ENUM_UINT8 enDlci
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 201288
           : l60609
       : 

*****************************************************************************/
VOS_UINT32 AT_CheckMuxDlci(AT_MUX_DLCI_TYPE_ENUM_UINT8 enDlci)
{
    /* 8DLCI181-8VOS_OKVOS_ERR */
    if ((enDlci >= AT_MUX_DLCI1_ID)
     && (enDlci < (AT_MUX_DLCI1_ID + AT_MUX_AT_CHANNEL_MAX)))
    {
        return VOS_OK;
    }

    return VOS_ERR;
}

/*****************************************************************************
     : AT_CheckMuxUser
   : MUX
   : VOS_UINT8 ucIndex
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 201283
           : L60609
       : 

*****************************************************************************/
VOS_UINT32 AT_CheckMuxUser(VOS_UINT8 ucIndex)
{
    return VOS_FALSE;
}

/*****************************************************************************
     : AT_IsHsicOrMuxUser
   : HSICMUX
   : VOS_UINT8 ucIndex
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 2012813
           : l60609
       : 

*****************************************************************************/
VOS_UINT32 AT_IsHsicOrMuxUser(VOS_UINT8 ucIndex)
{
    VOS_UINT32                          ulHsicUserFlg;
    VOS_UINT32                          ulMuxUserFlg;

    ulHsicUserFlg = AT_CheckHsicUser(ucIndex);
    ulMuxUserFlg  = AT_CheckMuxUser(ucIndex);

    /* HSICMUX */
    if ((VOS_FALSE == ulHsicUserFlg)
     && (VOS_FALSE == ulMuxUserFlg))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

/*****************************************************************************
     : AT_GetMuxDlciFromClientIdx
   : client indexMUXdlci
   : VOS_UINT8                           ucIndex
             AT_MUX_DLCI_TYPE_ENUM_UINT8        *penDlci
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 201286
           : l60609
       : 
  2.       : 201334
           : l60609
       : DSDA PHASE III
*****************************************************************************/
VOS_UINT32 AT_GetMuxDlciFromClientIdx(
    VOS_UINT8                           ucIndex,
    AT_MUX_DLCI_TYPE_ENUM_UINT8        *penDlci
)
{
    VOS_UINT8                           ucLoop;

    /* Modified by l60609 for DSDA Phase III, 2013-3-4, Begin */
    for (ucLoop = 0; ucLoop < AT_MUX_AT_CHANNEL_MAX; ucLoop++)
    {
        if (ucIndex == AT_GetCommCtxAddr()->stMuxCtx.astMuxClientTab[ucLoop].enAtClientTabIdx)
        {
            *penDlci = AT_GetCommCtxAddr()->stMuxCtx.astMuxClientTab[ucLoop].enDlci;
            break;
        }
    }
    /* Modified by l60609 for DSDA Phase III, 2013-3-4, End */

    if (ucLoop >= AT_MUX_AT_CHANNEL_MAX)
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}
/*****************************************************************************
     : AT_CheckPcuiCtrlConcurrent
   : PCUICTRL
   : VOS_UINT8
             VOS_UINT8
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 2013418
           : z00220246
       : 

  2.       : 2015527
           : l00198894
       : TSTS
*****************************************************************************/
VOS_UINT32 AT_CheckPcuiCtrlConcurrent(
    VOS_UINT8                           ucIndexOne,
    VOS_UINT8                           ucIndexTwo
)
{
    VOS_UINT32                          ulUserFlg1;
    VOS_UINT32                          ulUserFlg2;

    ulUserFlg1 = AT_CheckUserType(ucIndexOne, AT_USBCOM_USER);
    ulUserFlg1 |= AT_CheckUserType(ucIndexOne, AT_CTR_USER);
    ulUserFlg1 |= AT_CheckUserType(ucIndexOne, AT_PCUI2_USER);

    if (VOS_TRUE != ulUserFlg1)
    {
        return VOS_FALSE;
    }

    ulUserFlg2 = AT_CheckUserType(ucIndexTwo, AT_USBCOM_USER);
    ulUserFlg2 |= AT_CheckUserType(ucIndexTwo, AT_CTR_USER);
    ulUserFlg2 |= AT_CheckUserType(ucIndexTwo, AT_PCUI2_USER);

    if (VOS_TRUE != ulUserFlg2)
    {
        return VOS_FALSE;
    }

    if (gastAtClientTab[ucIndexOne].UserType != gastAtClientTab[ucIndexTwo].UserType)
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}

/*****************************************************************************
     : AT_IsConcurrentPorts
   : 
   : VOS_UINT8                           ucIndexOne
             VOS_UINT8                           ucIndexTwo
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 201283
           : L60609
       : 
  2.       : 20121129
           : f00179208
       : DSDA: VCOM AT
*****************************************************************************/
VOS_UINT32 AT_IsConcurrentPorts(
    VOS_UINT8                           ucIndexOne,
    VOS_UINT8                           ucIndexTwo
)
{
    VOS_UINT32                          ulMuxUserFlg1;
    VOS_UINT32                          ulHsicUserFlg1;
    VOS_UINT32                          ulAppUserFlg1;
    VOS_UINT32                          ulMuxUserFlg2;
    VOS_UINT32                          ulHsicUserFlg2;
    VOS_UINT32                          ulAppUserFlg2;

    /*  */
    ulMuxUserFlg1  = AT_CheckMuxUser(ucIndexOne);
    ulHsicUserFlg1 = AT_CheckHsicUser(ucIndexOne);
    ulAppUserFlg1  = AT_CheckAppUser(ucIndexOne);

    ulMuxUserFlg2  = AT_CheckMuxUser(ucIndexTwo);
    ulHsicUserFlg2 = AT_CheckHsicUser(ucIndexTwo);
    ulAppUserFlg2  = AT_CheckAppUser(ucIndexTwo);

    /* 1HSICMUX,2HSICMUX */
    if ((VOS_TRUE == ulMuxUserFlg1)
     || (VOS_TRUE == ulHsicUserFlg1))
    {
        if ((VOS_TRUE == ulMuxUserFlg2)
         || (VOS_TRUE == ulHsicUserFlg2))
        {
            return VOS_TRUE;
        }
    }

    /* 1APP,2APP */
    if (VOS_TRUE == ulAppUserFlg1)
    {
        if (VOS_TRUE == ulAppUserFlg2)
        {
            return VOS_TRUE;
        }
    }

    /* PCUICTRL */
    if (VOS_TRUE == AT_GetPcuiCtrlConcurrentFlag())
    {
        if (VOS_TRUE == AT_CheckPcuiCtrlConcurrent(ucIndexOne, ucIndexTwo))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}

/*****************************************************************************
     : AT_MuxCmdStreamEcho
   : MUX
   : VOS_UINT8                           ucIndex
             VOS_UINT8                          *pData
             VOS_UINT16                          usLen
   : 
     : VOS_VOID
   :
   :

       :
  1.       : 201286
           : l60609
       : 

  2.       : 2015527
           : l00198894
       : TSTS
*****************************************************************************/
VOS_VOID AT_MuxCmdStreamEcho(
    VOS_UINT8                           ucIndex,
    VOS_UINT8                          *pData,
    VOS_UINT16                          usLen
)
{
    AT_MUX_DLCI_TYPE_ENUM_UINT8         enDlci;
    VOS_UINT32                          ulRslt;

    enDlci = AT_MUX_DLCI_TYPE_BUTT;

    ulRslt = AT_GetMuxDlciFromClientIdx(ucIndex, &enDlci);

    if (VOS_TRUE != ulRslt)
    {
        return;
    }

    MUX_DlciDlDataSend(enDlci, pData, usLen);
    AT_MNTN_TraceCmdResult(ucIndex, pData, usLen);

    return;
}

/*****************************************************************************
     : AT_MemSingleCopy
   : 
   : pMemSrc  -- TTF
   : 
     : 
   :
   :

       :
  1.       : 20111017
           : sunshaohua
       : 

*****************************************************************************/
VOS_VOID AT_MemSingleCopy(
    VOS_UINT8                          *pucDest,
    VOS_UINT8                          *pucSrc,
    VOS_UINT32                          ulLen
)
{
    /* CacheDM */
    mdrv_memcpy(pucDest, pucSrc, (unsigned long)ulLen);

    return;
}

/* Added by j00174725 for V3R3 Cut Out Memory2013-11-07,  Begin */

/*****************************************************************************
     : AT_SendMuxSelResultData
   : ATMUX
   : VOS_UINT8                           ucIndex
             VOS_UINT8                          *pData
             VOS_UINT16                          usLen
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 201287
           : l60609
       : 
  2.       : 2013225
           : l60609
       : DSDA Phase III
*****************************************************************************/
VOS_UINT32 AT_SendMuxSelResultData(
    VOS_UINT8                           ucIndex,
    VOS_UINT8                          *pData,
    VOS_UINT16                          usLen
)
{
    VOS_UINT8                           ucLoop;
    /* Modified by l60609 for DSDA Phase III, 2013-2-25, Begin */
    AT_COMM_CTX_STRU                   *pCommCtx = VOS_NULL_PTR;

    pCommCtx = AT_GetCommCtxAddr();

    /* MUXindex */
    for (ucLoop = 0; ucLoop < AT_MUX_AT_CHANNEL_MAX; ucLoop++)
    {
        if (ucIndex == pCommCtx->stMuxCtx.astMuxClientTab[ucLoop].enAtClientTabIdx)
        {
            break;
        }
    }

    if (ucLoop >= AT_MUX_AT_CHANNEL_MAX)
    {
        return VOS_ERR;
    }

    /* AT */
    if (AT_HSIC_REPORT_OFF == pCommCtx->stMuxCtx.astMuxClientTab[ucLoop].enRptType)
    {
        return VOS_ERR;
    }
    /* Modified by l60609 for DSDA Phase III, 2013-2-25, End */

    AT_SendMuxResultData(ucIndex, pData, usLen);

    return VOS_OK;
}


/*****************************************************************************
     : AT_SendMuxResultData
   : ATMUX
   : VOS_UINT8                           ucIndex
             VOS_UINT8                          *pData
             VOS_UINT16                          usLen
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 201286
           : l60609
       : 

*****************************************************************************/
VOS_UINT32 AT_SendMuxResultData(
    VOS_UINT8                           ucIndex,
    VOS_UINT8                          *pData,
    VOS_UINT16                          usLen
)
{
    AT_MUX_DLCI_TYPE_ENUM_UINT8         enDlci;
    VOS_UINT32                          ulRslt;

    enDlci = AT_MUX_DLCI_TYPE_BUTT;

    /* MUX */
    if (VOS_TRUE != AT_GetMuxSupportFlg())
    {
        return AT_FAILURE;
    }

    /* Dlci */
    ulRslt = AT_GetMuxDlciFromClientIdx(ucIndex, &enDlci);

    if (VOS_TRUE != ulRslt)
    {
        return AT_FAILURE;
    }

    AT_MNTN_TraceCmdResult(ucIndex, pData, usLen);

    /* MUX */
    MUX_DlciDlDataSend(enDlci, pData, usLen);

    return AT_SUCCESS;
}


/*****************************************************************************
     : AT_SndDipcPdpActInd
   : ATDIPCPDP
   : ucCid          ----  CID
             ucRabId        ----  RABID
   : 
     : 
   :
   :

       :
  1.       : 2012217
           : L47619
       : 
*****************************************************************************/
VOS_VOID AT_SndDipcPdpActInd(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCid,
    VOS_UINT8                           ucRabId
)
{
    VOS_UINT32                      ulLength;
    AT_DIPC_PDP_ACT_STRU           *pstAtDipcPdpAct;
    AT_MODEM_PS_CTX_STRU           *pstPsModemCtx = VOS_NULL_PTR;

    ulLength        = sizeof( AT_DIPC_PDP_ACT_STRU ) - VOS_MSG_HEAD_LENGTH;
    pstAtDipcPdpAct = ( AT_DIPC_PDP_ACT_STRU *)PS_ALLOC_MSG( WUEPS_PID_AT, ulLength );

    if ( VOS_NULL_PTR == pstAtDipcPdpAct )
    {
        /*---:*/
        AT_WARN_LOG( "AT_SndDipcPdpActInd:ERROR:Allocates a message packet for AT_DIPC_PDP_ACT_STRU FAIL!" );
        return;
    }

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(usClientId);

    /*:*/
    pstAtDipcPdpAct->ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstAtDipcPdpAct->ulSenderPid     = WUEPS_PID_AT;
    pstAtDipcPdpAct->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstAtDipcPdpAct->ulReceiverPid   = PS_PID_APP_DIPC;
    pstAtDipcPdpAct->ulLength        = ulLength;
    /*:*/
    pstAtDipcPdpAct->enMsgType       = ID_AT_DIPC_PDP_ACT_IND;
    pstAtDipcPdpAct->ucRabId         = ucRabId;
    pstAtDipcPdpAct->enUdiDevId      = (UDI_DEVICE_ID_E)pstPsModemCtx->astChannelCfg[ucCid].ulRmNetId;

    /*:*/
    if ( VOS_OK != PS_SEND_MSG( WUEPS_PID_AT, pstAtDipcPdpAct ) )
    {
        /*---:*/
        AT_WARN_LOG( "AT_SndDipcPdpActInd:WARNING:SEND AT_DIPC_PDP_ACT_STRU msg FAIL!" );
    }

    return;
}



/*****************************************************************************
     : AT_SndDipcPdpDeactInd
   : ATDIPCPDP
   : ucRabId        ----  RABID
   : 
     : 
   :
   :

       :
  1.       : 2012217
           : L47619
       : 
*****************************************************************************/
VOS_VOID AT_SndDipcPdpDeactInd(
    VOS_UINT8                           ucRabId
)
{
    VOS_UINT32                      ulLength;
    AT_DIPC_PDP_DEACT_STRU         *pstAtDipcPdpDeact;

    ulLength          = sizeof( AT_DIPC_PDP_DEACT_STRU ) - VOS_MSG_HEAD_LENGTH;
    pstAtDipcPdpDeact = ( AT_DIPC_PDP_DEACT_STRU *)PS_ALLOC_MSG( WUEPS_PID_AT, ulLength );

    if ( VOS_NULL_PTR == pstAtDipcPdpDeact )
    {
        /*---:*/
        AT_WARN_LOG( "AT_SndDipcPdpDeactInd:ERROR:Allocates a message packet for AT_DIPC_PDP_DEACT_STRU FAIL!" );
        return;
    }

    /*:*/
    pstAtDipcPdpDeact->ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstAtDipcPdpDeact->ulSenderPid     = WUEPS_PID_AT;
    pstAtDipcPdpDeact->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstAtDipcPdpDeact->ulReceiverPid   = PS_PID_APP_DIPC;
    pstAtDipcPdpDeact->ulLength        = ulLength;
    /*:*/
    pstAtDipcPdpDeact->enMsgType       = ID_AT_DIPC_PDP_REL_IND;
    pstAtDipcPdpDeact->ucRabId         = ucRabId;

    /*:*/
    if ( VOS_OK != PS_SEND_MSG( WUEPS_PID_AT, pstAtDipcPdpDeact ) )
    {
        /*---:*/
        AT_WARN_LOG( "AT_SndDipcPdpDeactInd:WARNING:SEND AT_DIPC_PDP_DEACT_STRU msg FAIL!" );
    }

    return;
}


/*****************************************************************************
     : AT_SetAtChdataCidActStatus
   : AP-MODEMg_astAtChdataCfgCIDPDP
   : ucCid      ----      CID
             ulIsCidAct ----      CIDPDP
   :
     : VOS_VOID
   :
   :

       :
  1.       : 2012217
           : L47619
       : 
*****************************************************************************/
VOS_VOID AT_SetAtChdataCidActStatus(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCid,
    VOS_UINT32                          ulIsCidAct
)
{
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;

    /* CID */
    if ( ucCid > TAF_MAX_CID_NV)
    {
        AT_ERR_LOG1("AT_SetAtChdataCidActStatus, WARNING, CID error:%d\r\n", ucCid);
        return;
    }

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(usClientId);

    /* CID */
    pstPsModemCtx->astChannelCfg[ucCid].ulRmNetActFlg = ulIsCidAct;

    return;
}



/*****************************************************************************
     : AT_CleanAtChdataCfg
   : AP-MODEMCID
   :
             ucCid      ----      CID
   :
     : VOS_VOID
   :
   :

       :
  1.       : 2012217
           : L47619
       : 
*****************************************************************************/
VOS_VOID AT_CleanAtChdataCfg(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucCid
)
{
    AT_MODEM_PS_CTX_STRU               *pstPsModemCtx = VOS_NULL_PTR;

    /* CID */
    if ( ucCid > TAF_MAX_CID_NV)
    {
        AT_ERR_LOG1("AT_CleanAtChdataCfg, WARNING, CID error:%d\r\n", ucCid);
        return;
    }

    pstPsModemCtx = AT_GetModemPsCtxAddrFromClientId(usClientId);

    /* CID */
    pstPsModemCtx->astChannelCfg[ucCid].ulUsed     = VOS_FALSE;
    pstPsModemCtx->astChannelCfg[ucCid].ulRmNetId  = AT_PS_INVALID_RMNET_ID;

    /* CIDPDP */
    pstPsModemCtx->astChannelCfg[ucCid].ulRmNetActFlg = VOS_FALSE;

    return;
}

/*****************************************************************************
     : AT_CheckHsicUser
   : HSIC
   : ucIndex - 
   : 
     :
   :
   :

       :
  1.       : 20120218
           : l00198894
       : 
  2.       : 20120418
           : l00198894
       : AP-Modem
  3.       : 20121213
           : L00171473
       : DTS2012121802573, TQE
*****************************************************************************/
VOS_UINT32 AT_CheckHsicUser(VOS_UINT8 ucIndex)
{
    return VOS_FALSE;

}

/* Added by l60609 for AP 2012-09-10 Begin */
/*****************************************************************************
     : AT_CheckAppUser
   : APP
   : VOS_UINT8 ucIndex
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 2012910
           : L60609
       : 
*****************************************************************************/
VOS_UINT32 AT_CheckAppUser(VOS_UINT8 ucIndex)
{
    if (AT_APP_USER != gastAtClientTab[ucIndex].UserType)
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;

}
/* Added by l60609 for AP 2012-09-10 End */

/*****************************************************************************
     : AT_CheckNdisUser
   : NDIS
   : VOS_UINT8 ucIndex
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 201352
           : l60609
       : 

*****************************************************************************/
VOS_UINT32 AT_CheckNdisUser(VOS_UINT8 ucIndex)
{
    if (AT_NDIS_USER != gastAtClientTab[ucIndex].UserType)
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;

}

/*****************************************************************************
     : AT_CheckHsUartUser
   : UART
   : VOS_UINT8 ucIndex
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 2013918
           : z00189113
       : 
*****************************************************************************/
VOS_UINT32 AT_CheckHsUartUser(VOS_UINT8 ucIndex)
{

    return VOS_FALSE;
}

/*****************************************************************************
     : AT_CheckModemUser
   : MODEM
   : VOS_UINT8 ucIndex
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 2013918
           : z00189113
       : 
*****************************************************************************/
VOS_UINT32 AT_CheckModemUser(VOS_UINT8 ucIndex)
{
    if (AT_MODEM_USER != gastAtClientTab[ucIndex].UserType)
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

/*****************************************************************************
     : AT_CheckUserType
   : IndexUserType
   : VOS_UINT8            ucIndex
             AT_USER_TYPE         enUserType
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 2015527
           : l00198894
       : 

*****************************************************************************/
VOS_UINT32 AT_CheckUserType(
    VOS_UINT8                               ucIndex,
    AT_USER_TYPE                            enUserType
)
{
    if (enUserType != gastAtClientTab[ucIndex].UserType)
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

/*****************************************************************************
    : AT_InitFcMap
  : g_stFcIdMaptoFcPri
  : 
  : 
    : 
  :
  :

      :
 1.       : 2012217
          : L47619
      : 
 2.       : 20130417
          : f00179208
      : C
*****************************************************************************/
VOS_VOID AT_InitFcMap(VOS_VOID)
{
    VOS_UINT8       ucLoop;

    /* g_stFcIdMaptoFcPri */
    for (ucLoop = 0; ucLoop < FC_ID_BUTT; ucLoop++)
    {
        g_stFcIdMaptoFcPri[ucLoop].ulUsed  = VOS_FALSE;
        g_stFcIdMaptoFcPri[ucLoop].enFcPri = FC_PRI_BUTT;
        g_stFcIdMaptoFcPri[ucLoop].ulRabIdMask  = 0;
        g_stFcIdMaptoFcPri[ucLoop].enModemId    = MODEM_ID_BUTT;
    }
}

/*****************************************************************************
     : AT_SendDiagCmdFromOm
   : UEOMPCDIAG
   : VOS_UINT8    ucPortNo  --    
             VOS_UINT8    ucType    --    
             VOS_UINT8   *pData     --    DIAG
             VOS_UINT16   uslength  --    DIAG
   : 
     : VOS_UINT32
   :
   :

 1.       : 2008609
          : L47619
      : 

 2.       : 2010927
          : A00165503
      : 

 3.       : 20111024
           : /l60609
       : AT Project:  client id

*****************************************************************************/
VOS_UINT32 AT_SendDiagCmdFromOm(
    VOS_UINT8                           ucPortNo,
    VOS_UINT8                           ucType,
    VOS_UINT8                          *pData,
    VOS_UINT16                          uslength
)
{
    return VOS_OK;
}

/*****************************************************************************
     : AT_SendPcuiDataFromOm
   : OMPCUI
   : pucVirAddr - 
             pucPhyAddr - 
             ulLength   - 
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 20111010
           : /l60609
       : 

*****************************************************************************/
VOS_UINT32 AT_SendPcuiDataFromOm(
    VOS_UINT8                          *pucVirAddr,
    VOS_UINT8                          *pucPhyAddr,
    VOS_UINT32                          ulLength
)
{
    if (AT_SUCCESS != At_SendData(AT_CLIENT_TAB_PCUI_INDEX,
                                  gastAtClientTab[AT_CLIENT_TAB_PCUI_INDEX].DataMode,
                                  pucVirAddr,
                                  (VOS_UINT16)ulLength))
    {
        return VOS_ERR;
    }
    else
    {
        return VOS_OK;
    }
}

/*****************************************************************************
     : AT_SendCtrlDataFromOm
   : OMCTRL
   : pucVirAddr - 
             pucPhyAddr - 
             ulLength   - 
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 20111010
           : /l60609
       : 

*****************************************************************************/
VOS_UINT32 AT_SendCtrlDataFromOm(
    VOS_UINT8                          *pucVirAddr,
    VOS_UINT8                          *pucPhyAddr,
    VOS_UINT32                          ulLength
)
{
    if (AT_SUCCESS != At_SendData(AT_CLIENT_TAB_CTRL_INDEX,
                                  gastAtClientTab[AT_CLIENT_TAB_CTRL_INDEX].DataMode,
                                  pucVirAddr,
                                  (VOS_UINT16)ulLength))
    {
        return VOS_ERR;
    }
    else
    {
        return VOS_OK;
    }
}

/*****************************************************************************
     : AT_SendPcui2DataFromOm
   : OMPcui2
   : pucVirAddr - 
             pucPhyAddr - 
             ulLength   - 
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 2015522
           : l00198894
       : TSTS
*****************************************************************************/
VOS_UINT32 AT_SendPcui2DataFromOm(
    VOS_UINT8                          *pucVirAddr,
    VOS_UINT8                          *pucPhyAddr,
    VOS_UINT32                          ulLength
)
{
    if (AT_SUCCESS != At_SendData(AT_CLIENT_TAB_PCUI2_INDEX,
                                  gastAtClientTab[AT_CLIENT_TAB_PCUI2_INDEX].DataMode,
                                  pucVirAddr,
                                  (VOS_UINT16)ulLength))
    {
        return VOS_ERR;
    }
    else
    {
        return VOS_OK;
    }
}


/*****************************************************************************
     : AT_QuerySndFunc
   : OM
   : AT_PHY_PORT_ENUM_UINT32 ulPhyPort
   : 
     : pAtDataSndFunc
   :
   :

       :
  1.       : 20111010
           : /l60609
       : 

  2.       : 20131202
           : j00174725
       : UART
*****************************************************************************/
CBTCPM_SEND_FUNC AT_QuerySndFunc(AT_PHY_PORT_ENUM_UINT32 ulPhyPort)
{
    switch(ulPhyPort)
    {
        case AT_UART_PORT:
            return AT_UART_SendRawDataFromOm;

        case AT_PCUI_PORT:
            return AT_SendPcuiDataFromOm;

        case AT_CTRL_PORT:
            return AT_SendCtrlDataFromOm;


        default:
            AT_WARN_LOG("AT_QuerySndFunc: don't proc data of this port!");
            return VOS_NULL_PTR;
    }
}

/*****************************************************************************
 Prototype      : At_SendCmdMsg
 Description    : AT
 Input          : ucIndex --- ID
                  pData   --- 
                  usLen   --- 
                  ucType  --- 
 Output         : ---
 Return Value   : AT_SUCCESS --- 
                  AT_FAILURE --- 
 Calls          : ---
 Called By      : ---

 History        : ---
  1.Date        : 2005-04-19
    Author      : ---
    Modification: Created function
  2.     : 2007-03-27
         : h59254
     : :A32D09820(PC-Lint)
*****************************************************************************/
TAF_UINT32 At_SendCmdMsg (TAF_UINT8 ucIndex,TAF_UINT8* pData, TAF_UINT16 usLen,TAF_UINT8 ucType)
{
    AT_MSG_STRU                        *pMsg = TAF_NULL_PTR;
    AT_CMD_MSG_NUM_CTRL_STRU           *pstMsgNumCtrlCtx = VOS_NULL_PTR;
    VOS_UINT_PTR                        ulTmpAddr;
    VOS_UINT32                          ulLength;
    VOS_ULONG                           ulLockLevel;
    MODEM_ID_ENUM_UINT16                enModemId;
    pstMsgNumCtrlCtx = AT_GetMsgNumCtrlCtxAddr();

    if (VOS_NULL_PTR == pData)
    {
        AT_WARN_LOG("At_SendCmdMsg :pData is null ptr!");
        return AT_FAILURE;
    }

    if (0 == usLen)
    {
        AT_WARN_LOG("At_SendCmdMsg ulLength = 0");
        return AT_FAILURE;
    }

    if (AT_COM_BUFF_LEN < usLen)
    {
        AT_WARN_LOG("At_SendCmdMsg ulLength > AT_COM_BUFF_LEN");
        return AT_FAILURE;
    }

    /* ITEM4 */
    AT_GetAtMsgStruMsgLength(usLen, &ulLength);
    pMsg = (AT_MSG_STRU *)PS_ALLOC_MSG(WUEPS_PID_AT, ulLength);
    if ( pMsg == TAF_NULL_PTR )
    {
        AT_ERR_LOG("At_SendCmdMsg:ERROR:Alloc Msg");
        return AT_FAILURE;
    }

    if (AT_NORMAL_TYPE_MSG == ucType)
    {
        if (pstMsgNumCtrlCtx->ulMsgCount > AT_MAX_MSG_NUM)
        {
            /*  */
            PS_FREE_MSG(WUEPS_PID_AT, pMsg);

            return AT_FAILURE;
        }

        /*  */
        /*lint -e571*/
        VOS_SpinLockIntLock(&(pstMsgNumCtrlCtx->stSpinLock), ulLockLevel);
        /*lint +e571*/

        pstMsgNumCtrlCtx->ulMsgCount++;

        VOS_SpinUnlockIntUnlock(&(pstMsgNumCtrlCtx->stSpinLock), ulLockLevel);
    }

    /* pMsg->aucValue;*/
    pMsg->ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pMsg->ulSenderPid       = WUEPS_PID_AT;
    pMsg->ulReceiverPid     = WUEPS_PID_AT;

    if (AT_COMBIN_BLOCK_MSG == ucType)
    {
        pMsg->enMsgId = ID_AT_COMBIN_BLOCK_CMD;
    }
    else
    {
        pMsg->enMsgId = AT_GetCmdMsgID(ucIndex);
    }

    pMsg->ucType            = ucType;     /*  */
    pMsg->ucIndex           = ucIndex;    /*  */
    pMsg->usLen             = usLen;    /*  */

    enModemId               = MODEM_ID_0;
    if (VOS_OK != AT_GetModemIdFromClient(ucIndex, &enModemId))
    {
        enModemId = MODEM_ID_0;
    }

    pMsg->enModemId     = (VOS_UINT8)enModemId;
    /* */
    pMsg->enVersionId   = 0xAA;
    pMsg->aucReserved   = 0;

    TAF_MEM_SET_S(pMsg->aucValue, sizeof(pMsg->aucValue), 0x00, sizeof(pMsg->aucValue));
    AT_GetUserTypeFromIndex(ucIndex, &pMsg->ucUserType);


    /*  */
    ulTmpAddr = (VOS_UINT_PTR)(pMsg->aucValue);
    TAF_MEM_CPY_S((VOS_VOID*)ulTmpAddr, usLen, pData, usLen);  /*  */

    /*AT_PID;*/
    if ( 0 != PS_SEND_MSG( WUEPS_PID_AT, pMsg ) )
    {
        AT_ERR_LOG("At_SendCmdMsg:ERROR:VOS_SendMsg");

        /* ulMsgCount-- */

        return AT_FAILURE;
    }
    return AT_SUCCESS;
}

/*****************************************************************************
     : AT_IsApPort
   : HSICMUXVCOMAPP
   : VOS_UINT8 ucIndex
   : 
     : VOS_UINT32
   :
   :

       :
  1.       : 2013314
           : l60609
       : DSDA PHASE III
*****************************************************************************/
VOS_UINT32 AT_IsApPort(VOS_UINT8 ucIndex)
{
    VOS_UINT32                          ulHsicUserFlg;
    VOS_UINT32                          ulMuxUserFlg;
    VOS_UINT32                          ulVcomUserFlg;
    VOS_UINT8                          *pucSystemAppConfig;

    if (0 == g_stAtDebugInfo.ucUnCheckApPortFlg)
    {
        /*  */
        pucSystemAppConfig                  = AT_GetSystemAppConfigAddr();

        ulHsicUserFlg = AT_CheckHsicUser(ucIndex);
        ulMuxUserFlg  = AT_CheckMuxUser(ucIndex);
        ulVcomUserFlg = AT_CheckAppUser(ucIndex);

        if (SYSTEM_APP_ANDROID == *pucSystemAppConfig)
        {
            /* HSICMUXVCOM */
            if ((VOS_FALSE == ulHsicUserFlg)
             && (VOS_FALSE == ulMuxUserFlg)
             && (VOS_FALSE == ulVcomUserFlg))
            {
                return VOS_FALSE;
            }
        }
        else
        {
            /* HSICMUX */
            if ((VOS_FALSE == ulHsicUserFlg)
             && (VOS_FALSE == ulMuxUserFlg))
            {
                return VOS_FALSE;
            }
        }
    }

    return VOS_TRUE;
}

/*****************************************************************************
     : AT_ProcCCpuResetBefore
   : C
   : 
   : 
     : VOS_INT
   :
   :

      :
 1.       : 20130417
          : f00179208
      : 
 2.       : 20131113
          : j00174725
      : ID
 3.       : 20160122
          : z00301431
      : DTS2015103001118,set modemstatus
*****************************************************************************/
VOS_INT AT_ProcCCpuResetBefore(VOS_VOID)
{
    AT_MSG_STRU                        *pstMsg = VOS_NULL_PTR;

    printk("\n AT_ProcCCpuResetBefore enter, %u \n", VOS_GetSlice());

    /*  */
    AT_SetResetFlag(VOS_TRUE);

    DMS_InitModemStatus();

    /* TAFAGENT */
    TAF_AGENT_ClearAllSem();

    /*  */
    pstMsg = (AT_MSG_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_AT,
                                                         sizeof(AT_MSG_STRU));
    if (VOS_NULL_PTR == pstMsg)
    {
        printk("\n AT_ProcCCpuResetBefore alloc msg fail, %u \n", VOS_GetSlice());
        return VOS_ERROR;
    }

    /*  */
    TAF_MEM_SET_S((VOS_CHAR *)pstMsg + VOS_MSG_HEAD_LENGTH,
               (VOS_SIZE_T)(sizeof(AT_MSG_STRU) - VOS_MSG_HEAD_LENGTH),
               0x00,
               (VOS_SIZE_T)(sizeof(AT_MSG_STRU) - VOS_MSG_HEAD_LENGTH));

    /*  */
    pstMsg->ulReceiverCpuId             = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ucType                      = ID_CCPU_AT_RESET_START_IND;

    pstMsg->enMsgId                     = ID_AT_COMM_CCPU_RESET_START;

    /*  */
    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_AT, pstMsg))
    {
        printk("\n AT_ProcCCpuResetBefore send msg fail, %u \n", VOS_GetSlice());
        return VOS_ERROR;
    }

    /*  */
    if (VOS_OK != VOS_SmP(AT_GetResetSem(), AT_RESET_TIMEOUT_LEN))
    {
        printk("\n AT_ProcCCpuResetBefore VOS_SmP fail, %u \n", VOS_GetSlice());
        AT_DBG_LOCK_BINARY_SEM_FAIL_NUM(1);

        return VOS_ERROR;
    }

    /*  */
    AT_DBG_SAVE_CCPU_RESET_BEFORE_NUM(1);

    printk("\n AT_ProcCCpuResetBefore succ, %u \n", VOS_GetSlice());

    return VOS_OK;
}

/*****************************************************************************
     : AT_ProcCCpuResetAfter
   : C
   : 
   : 
     : VOS_INT
   :
   :

      :
 1.       : 20130417
          : f00179208
      : 
 2.       : 20131113
          : j00174725
      : ID
*****************************************************************************/
VOS_INT AT_ProcCCpuResetAfter(VOS_VOID)
{
    AT_MSG_STRU                        *pstMsg = VOS_NULL_PTR;

    printk("\n AT_ProcCCpuResetAfter enter, %u \n", VOS_GetSlice());

    /*  */
    pstMsg = (AT_MSG_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_AT,
                                                         sizeof(AT_MSG_STRU));
    if (VOS_NULL_PTR == pstMsg)
    {
        printk("\n AT_ProcCCpuResetAfter alloc msg fail, %u \n", VOS_GetSlice());
        return VOS_ERROR;
    }

    /*  */
    TAF_MEM_SET_S((VOS_CHAR *)pstMsg + VOS_MSG_HEAD_LENGTH,
               (VOS_SIZE_T)(sizeof(AT_MSG_STRU) - VOS_MSG_HEAD_LENGTH),
               0x00,
               (VOS_SIZE_T)(sizeof(AT_MSG_STRU) - VOS_MSG_HEAD_LENGTH));

    /*  */
    pstMsg->ulReceiverCpuId             = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;
    pstMsg->ucType                      = ID_CCPU_AT_RESET_END_IND;

    pstMsg->enMsgId                     = ID_AT_COMM_CCPU_RESET_END;

    /*  */
    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_AT, pstMsg))
    {
        printk("\n AT_ProcCCpuResetAfter send msg fail, %u \n", VOS_GetSlice());
        return VOS_ERROR;
    }

    /*  */
    AT_DBG_SAVE_CCPU_RESET_AFTER_NUM(1);

    printk("\n AT_ProcCCpuResetAfter succ, %u \n", VOS_GetSlice());

    return VOS_OK;
}

/*****************************************************************************
     : AT_CCpuResetCallback
   : CAT
   : enParam    -- 0 
             iUserData  -- 
   : 
     : VOS_INT
   :
   :

      :
 1.       : 20130417
          : f00179208
      : 
*****************************************************************************/
VOS_INT AT_CCpuResetCallback(
    DRV_RESET_CB_MOMENT_E               enParam,
    VOS_INT                             iUserData
)
{
    /*  */
    if (MDRV_RESET_CB_BEFORE == enParam)
    {
        return AT_ProcCCpuResetBefore();
    }
    /*  */
    else if (MDRV_RESET_CB_AFTER == enParam)
    {
        return AT_ProcCCpuResetAfter();
    }
    else
    {
        return VOS_ERROR;
    }
}

/*****************************************************************************
     : AT_HifiResetCallback
   : HIFIAT
   : lResetId -- 0 
             iUserData
   : 
     : VOS_INT
   :
   :

      :
 1.       : 20130412
          : f00179208
      : 
 2.       : 20130708
          : L47619
      : Modified for HIFI Reset End Report
 3.       : 20131113
          : j00174725
      : ID
*****************************************************************************/
VOS_INT AT_HifiResetCallback(
    DRV_RESET_CB_MOMENT_E               enParam,
    VOS_INT                             iUserData
)
{
    AT_MSG_STRU                        *pstMsg = VOS_NULL_PTR;

    /* 0 */
    if (MDRV_RESET_CB_BEFORE == enParam)
    {
        printk("\n AT_HifiResetCallback before reset enter, %u \n", VOS_GetSlice());
        /*  */
        pstMsg = (AT_MSG_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_AT,
                                                             sizeof(AT_MSG_STRU));
        if (VOS_NULL_PTR == pstMsg)
        {
            printk("\n AT_HifiResetCallback before reset alloc msg fail, %u \n", VOS_GetSlice());
            return VOS_ERROR;
        }

        /*  */
        TAF_MEM_SET_S((VOS_CHAR *)pstMsg + VOS_MSG_HEAD_LENGTH,
                   (VOS_SIZE_T)(sizeof(AT_MSG_STRU) - VOS_MSG_HEAD_LENGTH),
                   0x00,
                   (VOS_SIZE_T)(sizeof(AT_MSG_STRU) - VOS_MSG_HEAD_LENGTH));

        /*  */
        pstMsg->ulReceiverCpuId             = VOS_LOCAL_CPUID;
        pstMsg->ulReceiverPid               = WUEPS_PID_AT;
        pstMsg->ucType                      = ID_HIFI_AT_RESET_START_IND;

        pstMsg->enMsgId                     = ID_AT_COMM_HIFI_RESET_START;

        /*  */
        if (VOS_OK != PS_SEND_MSG(WUEPS_PID_AT, pstMsg))
        {
            printk("\n AT_HifiResetCallback after reset alloc msg fail, %u \n", VOS_GetSlice());
            return VOS_ERROR;
        }

        return VOS_OK;
    }
    /*  */
    else if (MDRV_RESET_CB_AFTER == enParam)
    {
        printk("\n AT_HifiResetCallback after reset enter, %u \n", VOS_GetSlice());
        /* Added by L47619 for HIFI Reset End Report, 2013/07/08, begin */
        /*  */
        pstMsg = (AT_MSG_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_AT,
                                                             sizeof(AT_MSG_STRU));
        if (VOS_NULL_PTR == pstMsg)
        {
            printk("\n AT_HifiResetCallback after reset alloc msg fail, %u \n", VOS_GetSlice());
            return VOS_ERROR;
        }

        /*  */
        TAF_MEM_SET_S((VOS_CHAR *)pstMsg + VOS_MSG_HEAD_LENGTH,
                   (VOS_SIZE_T)(sizeof(AT_MSG_STRU) - VOS_MSG_HEAD_LENGTH),
                   0x00,
                   (VOS_SIZE_T)(sizeof(AT_MSG_STRU) - VOS_MSG_HEAD_LENGTH));

        /*  */
        pstMsg->ulReceiverCpuId             = VOS_LOCAL_CPUID;
        pstMsg->ulReceiverPid               = WUEPS_PID_AT;
        pstMsg->ucType                      = ID_HIFI_AT_RESET_END_IND;

        pstMsg->enMsgId                     = ID_AT_COMM_HIFI_RESET_END;

        /*  */
        if (VOS_OK != PS_SEND_MSG(WUEPS_PID_AT, pstMsg))
        {
            printk("\n AT_HifiResetCallback after reset send msg fail, %u \n", VOS_GetSlice());
            return VOS_ERROR;
        }
        /* Added by L47619 for HIFI Reset End Report, 2013/07/08, end */
        return VOS_OK;
    }
    else
    {
        return VOS_ERROR;
    }
}


/*****************************************************************************
    : AT_ModemeEnableCB
  : MODEM
  : ucEnable    ----  
  : 
    : 
  :
  :

      :
  1.       : 20130525
           : /00179208
       : 
*****************************************************************************/
VOS_VOID AT_ModemeEnableCB(
    VOS_UINT8                           ucIndex,
    PS_BOOL_ENUM_UINT8                  ucEnable
)
{
    /* 
     PPP
       ATPDP.
    */
    if (PS_FALSE == ucEnable)
    {
        if (AT_PPP_DATA_MODE == gastAtClientTab[ucIndex].DataMode)
        {
            PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId,
                                   PPP_AT_CTRL_REL_PPP_REQ);

            /*  */
            if (0 == (gastAtClientTab[ucIndex].ModemStatus & IO_CTRL_CTS))
            {
                AT_StopFlowCtrl(ucIndex);
            }

            /*  */
            if (VOS_OK != TAF_PS_CallEnd(WUEPS_PID_AT,
                                         AT_PS_BuildExClientId(gastAtClientTab[ucIndex].usClientId),
                                         0,
                                         gastAtClientTab[ucIndex].ucCid))
            {
                AT_ERR_LOG("AT_ModemeEnableCB: TAF_PS_CallEnd failed in <AT_PPP_DATA_MODE>!");
                return;
            }
        }
        else if (AT_IP_DATA_MODE == gastAtClientTab[ucIndex].DataMode)
        {
            PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId,
                                   PPP_AT_CTRL_REL_PPP_RAW_REQ);

            /*  */
            if (0 == (gastAtClientTab[ucIndex].ModemStatus & IO_CTRL_CTS))
            {
                AT_StopFlowCtrl(ucIndex);
            }

            /*  */
            if ( VOS_OK != TAF_PS_CallEnd(WUEPS_PID_AT,
                                          AT_PS_BuildExClientId(gastAtClientTab[ucIndex].usClientId),
                                          0,
                                          gastAtClientTab[ucIndex].ucCid) )
            {
                AT_ERR_LOG("AT_ModemeEnableCB: TAF_PS_CallEnd failed in <AT_IP_DATA_MODE>!");
                return;
            }
        }
        else
        {
            /* elsePCLINT */
        }

        /* PPPHDLC */
        PPP_RcvAtCtrlOperEvent(gastAtClientTab[ucIndex].usPppId,
                               PPP_AT_CTRL_HDLC_DISABLE);

        /*  */
        AT_StopRelTimer(ucIndex, &gastAtClientTab[ucIndex].hTimer);

        /* At_ModemRelIndUSB
           Modemalways-onAT
           :
        */
        gastAtClientTab[ucIndex].Mode            = AT_CMD_MODE;
        gastAtClientTab[ucIndex].IndMode         = AT_IND_MODE;
        gastAtClientTab[ucIndex].DataMode        = AT_DATA_BUTT_MODE;
        gastAtClientTab[ucIndex].DataState       = AT_DATA_STOP_STATE;
        gastAtClientTab[ucIndex].CmdCurrentOpt   = AT_CMD_CURRENT_OPT_BUTT;
        g_stParseContext[ucIndex].ucClientStatus = AT_FW_CLIENT_STATUS_READY;
    }
}





