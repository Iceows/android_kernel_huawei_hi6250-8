/******************************************************************************

                   (C), 2001-2011, 

 ******************************************************************************
          : omnvinterace.hf
          : 
            : 
        : 2013515
        :
        : OMNVNV bin
        :
        :
  1.        : 2013515
            : j00174725
        : 

******************************************************************************/


/*****************************************************************************
  1 
*****************************************************************************/
#include "vos.h"

#ifndef __OM_NV_INTERFACE_H__
#define __OM_NV_INTERFACE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif
/*****************************************************************************
  2 
*****************************************************************************/
/*  */
#define USIMM_TEST_CARD_CFG_NUM                     (8)
#define USIMM_TEST_CARD_PLMN_LEN                    (3)
#define USIMM_CDMATEST_CARD_PLMN_LEN                (2)

/**/
#define OM_AGING_TEST_NUM                           (20)

#define SYS_ZSP_LDF_NUM                             (64)

#define SYS_HIFI_LDF_NUM                            (64)


#define STK_TERMINAL_ND                             (0x1)
#define STK_TERMINAL_NK                             (0x2)
#define STK_TERMINAL_NA                             (0x4)
#define STK_TERMINAL_NS                             (0x8)
#define STK_TERMINAL_NL                             (0x10)

#define STK_SIM_PROFILE_DEFAULT                     {0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x0F, 0x00, 0xDF, 0x7F, 0x03,\
                                                     0x00, 0x1F, 0xE2, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00}

#define STK_USIM_PROFILE_DEFAULT                    {0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0x00, 0xDF, 0xFF, 0x03,\
                                                     0x00, 0x1F, 0xE2, 0x00, 0x00, 0x00, 0xC3, 0xCB, 0x00, 0x00,\
                                                     0x00, 0x01, 0x00, 0x00, 0x91, 0x00, 0x00, 0x00, 0x00, 0x08}

#define VSIM_DH_PUBIIC_KEY                          (128)
#define VSIM_DH_PRIVATE_KEY                         (128)
#define VSIM_DH_AGREE_KEY                           (256)
#define VSIM_FILE_WRITE_COUNT                       (1)
#define VSIM_FILE_READ_COUNT                        (1)

#define VSIM_KEYLEN_MAX                             (128)                   /* 128 */

#define VSIM_EF_LOCI_LEN                            (11)
#define VSIM_EF_PS_LOCI_LEN                         (14)
#define VSIM_EF_FPLMN_LEN                           (60)                    /* FPLMN20 */

#define ESN_NV_DATA_LEN                             (8)
#define MEID_NV_DATA_LEN                            (8)

#define PESN_NV_DATA_LEN                            (4)
#define MEID_NV_DATA_LEN_NEW                        (7)

#define USIMM_PLMN_DATA_MAX_LEN                     (32)

/*****************************************************************************
  3 
*****************************************************************************/
/*****************************************************************************
     : SC_APSEC_DEFAULT_KEY_VER_ENUM
   : 

  1.       : 20130105
           : l00198894
       : V7R1C50 ISDB
*****************************************************************************/
enum SC_APSEC_DEFAULT_KEY_VER_ENUM
{
    SC_APSEC_DEFAULT_KEY_VER_TEST       =   0,              /*  */
    SC_APSEC_DEFAULT_KEY_VER_DCM        =   1,              /* Docomo */
    SC_APSEC_DEFAULT_KEY_VER_BUTT,
};
typedef VOS_UINT8 SC_APSEC_DEFAULT_KEY_VER_ENUM_UINT8;

enum OM_OUTPUT_PORT
{
    OM_OUTPUT_SHELL = 0,        /*SHELL*/
    OM_OUTPUT_FS,               /**/
    OM_OUTPUT_SDT,              /*SDT*/
    OM_OUTPUT_BUTT
};
typedef VOS_UINT32      OM_OUTPUT_PORT_ENUM_UINT32;

/*****************************************************************************
     : AT_PHY_PORT_ENUM
   : 
*****************************************************************************/
enum AT_PHY_PORT_ENUM
{
    AT_UART_PORT = 0,
    AT_PCUI_PORT,
    AT_CTRL_PORT,
    AT_HSUART_PORT,
    AT_PCUI2_PORT,
    AT_PORT_BUTT
};
typedef VOS_UINT32  AT_PHY_PORT_ENUM_UINT32;

enum
{
    CPM_IND_PORT = AT_PORT_BUTT,    /* OM */
    CPM_CFG_PORT,                   /* OM */
    CPM_SD_PORT,
    CPM_WIFI_OM_IND_PORT,           /* WIFIOM */
    CPM_WIFI_OM_CFG_PORT,           /* WIFIOM */
    CPM_WIFI_AT_PORT,               /* WIFIAT */
    CPM_HSIC_IND_PORT,
    CPM_HSIC_CFG_PORT,
    CPM_VCOM_IND_PORT,              /* VCOMOM */
    CPM_VCOM_CFG_PORT,              /* VCOMOM */
    CPM_FS_PORT,
    CPM_PORT_BUTT
};
typedef VOS_UINT32  CPM_PHY_PORT_ENUM_UINT32;

enum
{
    CPM_OM_PORT_TYPE_USB,
    CPM_OM_PORT_TYPE_VCOM,
    CPM_OM_PORT_TYPE_WIFI,
    CPM_OM_PORT_TYPE_SD,
    CPM_OM_PORT_TYPE_FS,
    CPM_OM_PORT_TYPE_HSIC,
    CBP_OM_PORT_TYPE_BUTT
};
typedef VOS_UINT32 CPM_OM_PORT_ENUM_UINT32;



/*****************************************************************************
     : VSIM_ALGORITHM_TYPE_ENUM
   : 

  1.       : 20130827
           : zhuli
       : V9R1 vSIM
*****************************************************************************/
enum VSIM_ALGORITHM_TYPE_ENUM
{
    VSIM_ALGORITHM_NULL = 0,
    VSIM_ALGORITHM_AES  = 1,
    VSIM_ALGORITHM_3DES = 2,
    VSIM_ALGORITHM_BUTT
};
typedef VOS_UINT32 VSIM_ALGORITHM_TYPE_ENUM_UINT32;

/*****************************************************************************
     : VSIM_ACTIVE_TYPE_ENUM
   : 

  1.       : 20130827
           : zhuli
       : V9R1 vSIM
*****************************************************************************/
enum VSIM_ACTIVE_TYPE_ENUM
{
    VSIM_VIRT_CARD_DEACTIVE = 0,
    VSIM_VIRT_CARD_ACTIVE   = 1,
    VSIM_ACTIVE_TYPE_BUTT
};
typedef VOS_UINT8 VSIM_ACTIVE_TYPE_ENUM_UINT8;

/*****************************************************************************
     : USIMM_CARD_STATUS_REG_TYPE_ENUM
   : SIM

  1.       : 20130925
           : h59254
       : V3R3 SIM
*****************************************************************************/
enum USIMM_CARD_STATUS_REG_TYPE_ENUM
{
    USIMM_CARD_STATUS_REG_TYPE_E5       = 0,    /* E5 */
    USIMM_CARD_STATUS_REG_TYPE_M2M      = 1,    /* M2M */
    USIMM_CARD_STATUS_REG_TYPE_BUTT
};

typedef VOS_UINT16 USIMM_CARD_STATUS_REG_TYPE_ENUM_UINT16;

/*****************************************************************************
     : EVENT_RESEND_FLAG_ENUM
   : 

  1.       : 20140116
           : zhuli
       : V9R1 C+L 
*****************************************************************************/
enum EVENT_RESEND_FLAG_ENUM
{
    EVENT_RESEND_OFF        = 0,
    EVENT_RESEND_ON         = 1,
    EVENT_RESEND_FLAG_BUTT
};
typedef VOS_UINT8 EVENT_RESEND_FLAG_ENUM_UINT8;

/*****************************************************************************
     : ESN_MEID_FLAG_ENUM
   : 

  1.       : 2014073
           : zhuli
       : V9R1 C+L 
*****************************************************************************/
enum ESN_MEID_FLAG_ENUM
{
    ESN_MEID_DISABLE        = 0,
    ESN_ME_ENABLE_FLAG      = 1,
    MEID_ME_ENABLE_FLAG     = 2,
    ESN_MEID_ENABLE_BOTH    = 3,
    ESN_MEID_ENABLE_FLAG_BUTT
};
typedef VOS_UINT8 ESN_MEID_FLAG_ENUM_UINT8;

/*****************************************************************************
  4 
*****************************************************************************/


/*****************************************************************************
  5 
*****************************************************************************/

/*****************************************************************************
  6 
*****************************************************************************/


/*****************************************************************************
  7 STRUCT
*****************************************************************************/
/*****************************************************************************
    : OM_PASSWORD_STRU
  : 
NVID: 1

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucPassword[16];
}OM_AUTH_PASSWORD_STRU;

/*****************************************************************************
    : OM_VERIFYCNT_STRU
  : 
NVID: 2 

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usVerifyCnt;
}OM_VERIFYCNT_STRU;


/*****************************************************************************
    : OM_TTL_STRU
  : 
NVID: 3

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                        ulTtl;
}OM_TTL_STRU;

/*****************************************************************************
    : OM_TTL_STRU
  : 
NVID: 5

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usYear;
    VOS_UINT16                          usMonth;
    VOS_UINT16                          usDayOfWeek;
    VOS_UINT16                          usDay;
    VOS_UINT16                          usHour;
    VOS_UINT16                          usMinute;
    VOS_UINT16                          usSecond;
    VOS_UINT16                          usMilliSeconds;
}OM_CALIBRATED_TIME_STRU;

/*****************************************************************************
    : OM_SERIAL_NUM_STRU
  : 
NVID: 6 SerialNumber

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucSerialNumber[20];
}OM_SERIAL_NUM_STRU;

/*****************************************************************************
    : OM_LOG_FILE_MAX_SIZE_STRU
  : 
NVID: 8 LogFileMaxSize

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulLogFileMaxSize;
}OM_LOG_FILE_MAX_SIZE_STRU;


/*****************************************************************************
    : OM_WATCHDOG_FLAG_STRU
  : 
NVID: 9 WatchDogFlag

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulWatchDogFlag; /*[0,1]*/
}OM_WATCHDOG_FLAG_STRU;

/*****************************************************************************
    : OM_KICKDOG_TIME_STRU
  :
NVID: 11 KickDog_Time

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulKickDogTime;
}OM_KICKDOG_TIME_STRU;

/*****************************************************************************
    : OM_SCHEDULE_WATCH_TIME_STRU
  :
NVID: 12 ScheduleWatchDog_Time

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulScheduleWatchDogTime;
}OM_SCHEDULE_WATCH_TIME_STRU;

/*****************************************************************************
    : OM_MONITORPARA_CTRL_STRU
  :
NVID:   MonitorParaCtrl

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucTempOverCount;
    VOS_UINT8                           ucTempResumeCount;
    VOS_UINT8                           ucTempCloseCount;
    VOS_UINT8                           ucTempReserve;
    VOS_UINT32                          ulWaitTimer;
}OM_MONITORPARA_CTRL_STRU;

/*****************************************************************************
    : OM_USIM_TEMP_PROTECT_STRU
  : 
NVID: 14 UsimTempProtect

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulProtectEnable;
    VOS_INT32                           lPmicCloseAdcThreshold;
    VOS_INT32                           lPmicAlarmAdcThreshold;
    VOS_INT32                           lPmicResumeAdcThreshold;
    OM_MONITORPARA_CTRL_STRU            stMonitorParaCtrl;
}OM_USIM_TEMP_PROTECT_STRU;


/*****************************************************************************
    : OM_RF_CONFIG_STRU
  :
NVID: 16 RfConfig

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    MODEM_ID_ENUM_UINT16                enModemID;
    VOS_UINT16                          usNetworkMode;  /**/
    VOS_UINT16                          usBand;
    VOS_UINT16                          usSlotCnt;      /**/
    VOS_UINT16                          usChannel;
    VOS_UINT16                          usPower;        /*PA*/
    VOS_UINT16                          usModType;      /**/
}OM_RF_CONFIG_STRU;


/*****************************************************************************
    : OM_AGINGTEST_TABLE_STRU
  : NV
NVID: 16 AgingTestTable

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulIsEnable;     /*VOS_YES/VOS_NO*/
    VOS_UINT32                          ulTimerLen;     /**/
    VOS_UINT32                          ulNumber;       /**/
    OM_RF_CONFIG_STRU                   astOmRfConfig[OM_AGING_TEST_NUM];
}OM_AGING_TEST_NV_STRU;

/*****************************************************************************
    : OM_LIVE_TIME_CONTROL_STRU
  : 
NVID: 22 LiveTimeControl

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/

typedef struct
{
    VOS_UINT32                          ulEnable;
    VOS_UINT32                          ulCycle;
} OM_LIVE_TIME_CONTROL_STRU;


/*****************************************************************************
    : OM_LIVE_TIME_STRU
  : 
NVID: 23 LiveTime

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulLiveTime;
}OM_LIVE_TIME_STRU;

/*****************************************************************************
    : OM_SPY_QOSCNF_STRU
  :
NVID:  Spy QOSCNF

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usDownLoadQos;
    VOS_UINT16                          usUpLoadQos;
}OM_SPY_QOSCNF_STRU;

/*****************************************************************************
    : OM_MONITOR_CPU_STRU
  : 
NVID: 24 MonitorCPU

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucSPYCPUEnable;
    VOS_UINT8                           ucCPUUpLimit;
    VOS_UINT8                           ucCPUDownLimit;
    VOS_UINT8                           ucSleepRunBase;
    VOS_UINT8                           ucSleepWFIBase;
    VOS_UINT8                           ucUpLimitTimeMax;
    VOS_UINT8                           ucDownLimitTimeMax;
    VOS_UINT8                           ucRepirValue;
    OM_SPY_QOSCNF_STRU                  astSpyQosCnf[2];
}OM_MONITOR_CPU_STRU;

/*****************************************************************************
    : OM_MAX_REG_CNT_GSM_STRU
  :
NVID: 28 Max_Reg_Cnt_in_GSM_Mode

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulRegCnt;
}OM_MAX_REG_CNT_GSM_STRU;

/*****************************************************************************
    : OM_DFS_CONFIG_THRESHOLD_STRU
  :
NVID:  Threshold

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usUpLimite;
    VOS_UINT16                          usDownLimite;
}OM_DFS_CONFIG_THRESHOLD_STRU;

/*****************************************************************************
    : OM_DFS_CONFIG_STRU
  : 
NVID: 29 DFS_Config

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulEnableFlag;
    VOS_UINT32                          ulTimerLen;
    VOS_UINT16                          usUpTime;
    VOS_UINT16                          usDownTime;
    VOS_UINT32                          ulProfileNum;
    OM_DFS_CONFIG_THRESHOLD_STRU        astThreshold[20];
}OM_DFS_CONFIG_STRU;

/*****************************************************************************
    : OM_SW_VER_FLAG_STRU
  :
NVID: 36 NV_SW_VER_Flag

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulNvSwVerFlag; /*[0,1]*/
}OM_SW_VER_FLAG_STRU;

/*****************************************************************************
    : OM_ERRORLOG_ENABLE_FLAG_STRU
  :
NVID: 37 NV_ERRORLOG_ENABLEFLAG

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulErrorLogEnable; /*[0,1]*/
}OM_ERRORLOG_ENABLE_FLAG_STRU;

/*****************************************************************************
    : OM_ERRORLOG_FLUSHBUFINTER_STRU
  :
NVID: 38 NV_ERRORLOG_FLUSHBUFINTERVALUE

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulErrorFlushInter;
}OM_ERRORLOG_FLUSHBUFINTER_STRU;

/*****************************************************************************
    : OM_ERRORLOG_RECORDPERIOD_STRU
  :
NVID: 39 NV_ERRORLOG_RECORDPERIOD

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulErrorRecord; /*[0,103680]*/
}OM_ERRORLOG_RECORDPERIOD_STRU;


/*****************************************************************************
    : OM_MONITOR_PARA_CTRL_STRU
  :
NVID: 40 MonitorParaCtrl

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucTempOverCount;     /**/
    VOS_UINT8                           ucTempRsumeCount;    /**/
    VOS_UINT8                           ucTempCloseCount;    /**/
    VOS_UINT8                           ucTempPowerOffCount; /**/
    VOS_UINT32                          ulWaitTimer;         /**/
}TEMP_PARA_CTRL_STRU;

/*****************************************************************************
    : OM_USIM_TEMP_PROTECT_NEW_STRU
  :
NVID: 40 UsimTempProtect_New

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulIsEnable;           /*, 01*/
    VOS_UINT32                          ulLocation;           /*,06 */
    VOS_INT32                           lPowerOffThreshold;   /**/
    VOS_INT32                           lCloseAdcThreshold;   /**/
    VOS_INT32                           lAlarmAdcThreshold;   /**/
    VOS_INT32                           lResumeAdcThreshold;  /**/
    VOS_INT32                           lSIMPowerOffThreshold;  /*E5SIM*/
    TEMP_PARA_CTRL_STRU                 stTempCtrlPara;
}SPY_TEMP_PROTECT_NV_STRU;

/*****************************************************************************
    : CPM_CBT_PORT_ENUM_UINT32
  : CBT
NVID: 45 en_NV_Item_Om_Port_Type

  1.       : 201463
           : h59254
       : 
*****************************************************************************/
enum
{
    CPM_CBT_PORT_USB,
    CPM_CBT_PORT_VCOM,
    CPM_CBT_PORT_SOCKET,
    CBP_CBT_PORT_BUTT
};
typedef VOS_UINT32 CPM_CBT_PORT_ENUM_UINT32;

/*****************************************************************************
    : OM_PORT_CFG_STRU
  :
NVID: 45 NV_OM_Port_Type

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    CPM_OM_PORT_ENUM_UINT32             enPortNum;         /*  */
    CPM_CBT_PORT_ENUM_UINT32            enCbtPortNum;      /*  */
}OM_CHANNLE_PORT_CFG_STRU;

/*****************************************************************************
    : OM_PORT_CFG_STRU
  :
NVID: 46/47 NV_PsLog_Port_Type

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    OM_OUTPUT_PORT_ENUM_UINT32          enPortType;     /**/
    VOS_UINT32                          ulMaxFileSize;  /**/
}OM_PORT_CFG_STRU;

/*****************************************************************************
    : OM_M2_ENABLE_STRU
  : 
NVID: 59 M2_ENABLE

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulM2Enable;
}OM_M2_ENABLE_STRU;

/*****************************************************************************
    : LOG_FILE_SAVE_CONFIG_STRU
  :
NVID: 117 LOG_FILE_SAVE_CONFIG

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulResult;       /*  */
    VOS_UINT32                          ulLogFileSize;  /* LOG */
}LOG_FILE_SAVE_CONFIG_STRU;

/*****************************************************************************
    : USIMM_TERMINAL_CAPABILITY_STRU
  :
NVID: 130

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                  ulIsEnable;
    VOS_UINT32                                  ulLen;
    VOS_UINT8                                   aucData[10];
    VOS_UINT16                                  usRsv;
}USIMM_TERMINAL_CAPABILITY_STRU;

/*****************************************************************************
    : OM_MANUFACTUREINFO_STRU
  :
NVID: 32768 ManufactureInfo

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucInfo[128];
}OM_MANUFACTUREINFO_STRU;


/*****************************************************************************
    : OM_MAC_ADDR_STRU
  :
NVID: 50014 NV_MACADDR

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucMacAddr[32];
}OM_MAC_ADDR_STRU;

/*****************************************************************************
    : OM_MEAN_THROUGHPUT_STRU
  : 
NVID: 50031 MEAN_THROUGHPUT 

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucMeanThroughPut[2];
}OM_MEAN_THROUGHPUT_STRU;


/*****************************************************************************
    : OM_ISDB_DEFAULT_KEY_STRU
  :
NVID: 52006 ISDB_DEFAULT_KEY 

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    SC_APSEC_DEFAULT_KEY_VER_ENUM_UINT8 aenISDBKey[2];
}OM_ISDB_DEFAULT_KEY_STRU;


/*****************************************************************************
    : OM_SIM_ATR_FLAG_STRU
  :
NVID: 52007 SimAtrFlag  129

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                        ulSimAtrFlag;
}OM_SIM_ATR_FLAG_STRU;

/*****************************************************************************
    : OM_NV_CONFIG_STRU
  :
NVID: 120 NV_OAM_Config

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucOamConfig;
    VOS_UINT8                           aucRev[15];
}OM_NV_CONFIG_STRU;

/*****************************************************************************
    : LDF_DUMP_DATA_INFO_STRU
  :
NVID:  LDF_DUMP_DATA_INFO_STRU

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulDataAddr;
    VOS_UINT32                          ulDataLen;
}LDF_DUMP_DATA_INFO_STRU;

/*****************************************************************************
    : ZSP_LDF_NV_CONFIG_STRU
  : 
NVID: 122 NV_ZSP_LDF_CONFIG/NV_HIFI_LDF_CONFIG

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulDataNum;
    LDF_DUMP_DATA_INFO_STRU             astLDFData[SYS_ZSP_LDF_NUM];
}ZSP_LDF_NV_CONFIG_STRU;

/*****************************************************************************
    : HIFI_LDF_NV_CONFIG_STRU
  : 
NVID: 123 NV_ZSP_LDF_CONFIG/NV_HIFI_LDF_CONFIG

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulDataNum;
    LDF_DUMP_DATA_INFO_STRU             astLDFData[SYS_HIFI_LDF_NUM];
}HIFI_LDF_NV_CONFIG_STRU;

/*****************************************************************************
    : OM_PRI_THRESHOLD_STRU
  : 
NVID: 127 NV_PRIORITY_THRESHOLD

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          ausNvMiddle[6];
    VOS_UINT16                          ausRev[2];
}OM_PRI_THRESHOLD_STRU;

/*****************************************************************************
    : USIMM_FEATURE_CFG_BIT_STRU
  : USIMM

  1.       : 2012625
           : h59254
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulAtt_flg               : 1;
    VOS_UINT32                          ulNFCFlg                : 1;
    VOS_UINT32                          ulAidLenCheckFlg        : 1;
    VOS_UINT32                          ulAuthCmdCheckFlg       : 1;
    VOS_UINT32                          ulCglaInsCheckFlg       : 1;
    VOS_UINT32                          ulCglaSelectCheckFlg    : 1;
    VOS_UINT32                          ulTmo_flg               : 1;
    VOS_UINT32                          ulImsiPolling           : 1;
    VOS_UINT32                          ulCglaGetRsp            : 1;
    VOS_UINT32                          ulAIDFCPSave            : 1;
    VOS_UINT32                          ulCUIMCheck             : 1;
    VOS_UINT32                          ulTimerOutInd           : 1;
    VOS_UINT32                          ulPKCS15                : 1;
    VOS_UINT32                          ulP2ActiveAID           : 1;
    VOS_UINT32                          ulWarningDataInd        : 1;

    VOS_UINT32                          bMeidPesnAPWriteEnable  : 1;  /* Meid PesnAPmodem */
    VOS_UINT32                          ulADFActiveAll          : 1;

    /* CSIM APPAID,ICChisi */
    VOS_UINT32                          ulCsimAppAidCheck       : 1;

    /* CSIMSTATSU60 */
    VOS_UINT32                          ulCsimReselectFile      : 1;

    VOS_UINT32                          ulImsAccessCustomFlg    : 1;

    VOS_UINT32                          ulDocomoReadOPFileFlg   : 1;

    /* CGLA&0x6F  */
    VOS_UINT32                          ulCglaGetRspCustomFlg   : 1;

    VOS_UINT32                          ulRsv                   : 10;
}USIMM_FEATURE_CFG_BIT_STRU;

/*****************************************************************************
    : USIMM_FEATURE_CFG_STRU
  : USIMM
NVID: 9090 UsimSupportFeatureCfg
:       NV
            NV binUSIMM_FEATURE_CFG_NV_STRU
  1.       : 2012625
           : h59254
       : 
*****************************************************************************/
typedef struct
{
    union
    {
        VOS_UINT32                      aulValue[1];
        USIMM_FEATURE_CFG_BIT_STRU      stFeatureCfg;
    }unCfg;
}USIMM_FEATURE_CFG_STRU;

/*****************************************************************************
    : USIMM_FEATURE_CFG_NV_STRU
  : USIMM
NVID: 9090 UsimSupportFeatureCfg
:     USIMM_FEATURE_CFG_STRU
          NV
  1.       : 2012625
           : h59254
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulCfg;
}USIMM_FEATURE_CFG_NV_STRU;

/*****************************************************************************
    : STK_FEATURE_CFG_BIT_STRU
  : STK

  1.       : 2012625
           : h59254
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulLociEventCmp          : 1;

    VOS_UINT32                          ulRsv                   :31;
}STK_FEATURE_CFG_BIT_STRU;

/*****************************************************************************
    : STK_FEATURE_CFG_STRU
  : USIMM
NVID: 4005 STKSupportFeatureCfg
:       NV
            NV binUSIMM_FEATURE_CFG_NV_STRU
  1.       : 2012625
           : h59254
       : 
*****************************************************************************/
typedef struct
{
    union
    {
        VOS_UINT32                    aulValue[1];
        STK_FEATURE_CFG_BIT_STRU      stFeatureCfg;
    }unCfg;
}STK_FEATURE_CFG_STRU;

/*****************************************************************************
    : STK_FEATURE_CFG_NV_STRU
  : STK
NVID: 4005 STKSupportFeatureCfg
:     USIMM_FEATURE_CFG_STRU
          NV
  1.       : 2012625
           : h59254
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulCfg;
}STK_FEATURE_CFG_NV_STRU;

/*****************************************************************************
    : OM_USIM_NEED_FCP_STRU
  :
NVID: 9108 USIM_Status_Need_Fcp

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulUsimNeedFcp;
}OM_USIM_NEED_FCP_STRU;


/*****************************************************************************
    : OM_USIM_OP_FILE_CFG_STRU
  :
NVID: 9126/9127 UsimOpfileCfg/simOpfileCfg

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usOptionFileNum;
    VOS_UINT16                          ausOptionFileList[16];
    VOS_UINT16                          usReserved;
}OM_USIM_OP_FILE_CFG_STRU;

/*****************************************************************************
     : USIMM_TEST_CARD_PLMN_ST
   : USIMMPLMN

  1.       : 20130104
           : h59254
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucPlmn[USIMM_TEST_CARD_PLMN_LEN];
    VOS_UINT8                           ucRsv;
}USIMM_TEST_CARD_PLMN_ST;

/*****************************************************************************
     : USIMM_TEST_CARD_PLMN_ST
   : USIMMPLMN

  1.       : 20130104
           : h59254
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucPlmn[USIMM_CDMATEST_CARD_PLMN_LEN];
    VOS_UINT8                           ucRsv1;
    VOS_UINT8                           ucRsv2;
}USIMM_CDMATEST_CARD_PLMN_ST;

/*****************************************************************************
     : USIMM_TEST_CARD_CFG_ST
   : USIMM
NVID: 9129 TestCardCfg
  1.       : 20130104
           : h59254
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulPlmnNum;
    USIMM_TEST_CARD_PLMN_ST             astPlmnList[USIMM_TEST_CARD_CFG_NUM];
}USIMM_TEST_CARD_CFG_ST;

/*****************************************************************************
     : USIMM_CDMATEST_CARD_CFG_ST
   : USIMM
 NVID  : 4004 CdmaTestCardCfg
  1.       : 20130104
           : h59254
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulPlmnNum;
    USIMM_CDMATEST_CARD_PLMN_ST         astPlmnList[USIMM_TEST_CARD_CFG_NUM];
}USIMM_CDMATEST_CARD_CFG_ST;

/*****************************************************************************
    : OM_BBP_DUMP_ENABLE_STRU
  :
NVID: 9205 NV_BBP_DUMP_ENABLE

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                        ulEnable;
}OM_BBP_DUMP_ENABLE_STRU;


/*****************************************************************************
    : OM_FILE_EF6F62_STRU
  :
NVID: 6657 FILE_EF6F62

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[250];
    VOS_UINT16                          usReserved;
}OM_FILE_EF6F62_STRU;

/*****************************************************************************
    : OM_FILE_EF6F7B_STRU
  :
NVID: 6658 FILE_EF6F7B

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[12];
}OM_FILE_EF6F7B_STRU;

/*****************************************************************************
    : OM_FILE_EF6F60_STRU
  :
NVID: 6659 FILE_EF6F60

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[170];
    VOS_UINT16                          usReserved;
}OM_FILE_EF6F60_STRU;
/*****************************************************************************
    : OM_FILE_EF6F61_STRU
  :
NVID: 6660 FILE_EF6F61

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[250];
    VOS_UINT16                          usReserved;
}OM_FILE_EF6F61_STRU;

/*****************************************************************************
    : OM_FILE_EF6F31_STRU
  :
NVID: 6661 FILE_EF6F31

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[2];
    VOS_UINT16                          usReserved;
}OM_FILE_EF6F31_STRU;

/*****************************************************************************
    : OM_FILE_EF6FAD_STRU
  :
NVID: 6662 FILE_EF6FAD

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[4];
}OM_FILE_EF6FAD_STRU;

/*****************************************************************************
    : OM_FILE_EF6F38_STRU
  :
NVID: 6663 FILE_EF6F38

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[8];
}OM_FILE_EF6F38_STRU;

/*****************************************************************************
    : OM_FILE_EF6F7E_STRU
  :
NVID: 6664 FILE_EF6F7E

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[12];
}OM_FILE_EF6F7E_STRU;

/*****************************************************************************
    : OM_FILE_EF6F73_STRU
  :
NVID:  6665 FILE_EF6F73

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[14];
    VOS_UINT16                          usReserved;
}OM_FILE_EF6F73_STRU;

/*****************************************************************************
    : OM_FILE_EF6F53_STRU
  :
NVID:  6666 FILE_EF6F53

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[12];
}OM_FILE_EF6F53_STRU;

/*****************************************************************************
    : OM_FILE_EF6F07_STRU
  :
NVID:  6667 FILE_EF6F07

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[10];
    VOS_UINT16                          usReserved;
}OM_FILE_EF6F07_STRU;

/*****************************************************************************
    : OM_FILE_EF6F08_STRU
  :
NVID:  6668 FILE_EF6F08

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[34];
    VOS_UINT16                          usReserved;
}OM_FILE_EF6F08_STRU;

/*****************************************************************************
    : OM_FILE_EF6F09_STRU
  :
NVID:  6669 FILE_EF6F09

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[34];
    VOS_UINT16                          usReserved;
}OM_FILE_EF6F09_STRU;

/*****************************************************************************
    : OM_FILE_EF6F20_STRU
  :
NVID:  6670 FILE_EF6F20

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[2];
    VOS_UINT16                          usReserved;
}OM_FILE_EF6F20_STRU;

/*****************************************************************************
    : OM_FILE_EF6F52_STRU
  :
NVID:  6671 FILE_EF6F52

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[8];
}OM_FILE_EF6F52_STRU;

/*****************************************************************************
    : OM_FILE_EF4F20_STRU
  :
NVID:  6672 FILE_EF4F20

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[10];
    VOS_UINT16                          usReserved;
}OM_FILE_EF4F20_STRU;

/*****************************************************************************
    : OM_FILE_EF4F52_STRU
  :
NVID:  6673 FILE_EF4F52

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[10];
    VOS_UINT16                          usReserved;
}OM_FILE_EF4F52_STRU;

/*****************************************************************************
    : OM_FILE_EF6FB7_STRU
  :
NVID:  6674 FILE_EF6FB7

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[14];
    VOS_UINT16                          usReserved;
}OM_FILE_EF6FB7_STRU;

/*****************************************************************************
    : OM_FILE_EF6F78_STRU
  :
NVID:  6675 FILE_EF6F78

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[2];
    VOS_UINT16                          usReserved;
}OM_FILE_EF6F78_STRU;

/*****************************************************************************
    : OM_FILE_EF6F5B_STRU
  :
NVID:  6676 FILE_EF6F5B

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[6];
    VOS_UINT16                          usReserved;
}OM_FILE_EF6F5B_STRU;

/*****************************************************************************
    : OM_FILE_EF6F5C_STRU
  :
NVID:  6677 FILE_EF6F5C

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[4];
}OM_FILE_EF6F5C_STRU;

/*****************************************************************************
    : OM_FILE_EF6FC4_STRU
  :
NVID:  6678 FILE_EF6FC4

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[50];
    VOS_UINT16                          usReserved;
}OM_FILE_EF6FC4_STRU;

/*****************************************************************************
    : OM_FILE_EF6F74_STRU
  :
NVID:  6679 FILE_EF6F74

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucContent[50];
    VOS_UINT16                          usReserved;
}OM_FILE_EF6F74_STRU;

/*****************************************************************************
    : SI_PB_CTRL_INFO_ST
  :
NVID: 8244 UsimPBCtrlInfo

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef VOS_UINT32 SI_PB_STORATE_TYPE;
typedef struct
{
    SI_PB_STORATE_TYPE                  enCurMaterial;
    VOS_UINT8                           aucPasswords[8];
    VOS_UINT16                          usUsed;
    VOS_UINT16                          usTotal;
    VOS_UINT8                           ucAnrMaxNum;        /*BalongANR*/
    VOS_UINT8                           ucEmailFlag;        /*BalongEmail*/
    VOS_UINT8                           ucSPBFlag;         /*1*/
    VOS_UINT8                           ucRsv;
}SI_PB_CTRL_INFO_ST;

/*****************************************************************************
    : USIMM_CMD_DEBUG_MODE_ST
  :
NVID: 8257 UsimDebugMode

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usFlag;
    VOS_UINT16                          usTimerValue;
}USIMM_CMD_DEBUG_MODE_ST;

/*****************************************************************************
    : USIMM_STK_CFG_STRU
  :
NVID: 8261 TerminalProfile_Set

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucProfileLen;
    VOS_UINT8                           aucProfile[47];
}USIMM_USIM_PROFILE_STRU;


typedef struct
{
    VOS_UINT8                           ucProfileLen;
    VOS_UINT8                           aucProfile[31];
}USIMM_SIM_PROFILE_STRU;

typedef struct
{
    VOS_UINT8                           ucFuncEnable;
    VOS_UINT8                           ucTimer;
    VOS_UINT8                           ucTerminalType;
    VOS_UINT8                           ucRsv;
    USIMM_USIM_PROFILE_STRU             stUsimProfile;
    USIMM_SIM_PROFILE_STRU              stSimProfile;
}USIMM_STK_CFG_STRU;

/*****************************************************************************
    : USIMM_USIMSUPIMS_STRU
  : USIMIMS

  1.       : 201396
           : j00168360
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                           ulUsimSupImsEnable;
}USIMM_USIMSUPIMS_STRU;

/*****************************************************************************
    : SI_STK_MATCH_STRING_STRU
  :
NVID:

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usStrLen;
    VOS_UINT8                           aucMatchStr[22];
}SI_STK_MATCH_STRING_STRU;

/*****************************************************************************
    : USIMM_STK_CFG_STRU
  : Dual IMSI NV
NVID: 8318 StkDualIMSICtrl

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usDualIMSIEnable;      /* ITEM ID */
    VOS_UINT16                          usMatchStrCnt;
    SI_STK_MATCH_STRING_STRU            astMatchStr[3];
}SI_STK_IMSICHG_MATCH_STRU;

/*****************************************************************************
    : SI_STK_SMSIN_CTRL_STRU
  :
NVID: 8330 STK_SMSIND_CTRL

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulStkSmsindCtrl;
}SI_STK_SMSIN_CTRL_STRU;

/*****************************************************************************
    : OM_VIVO_STK_CODEC_STRU
  :
NVID: 8505 en_NV_Brazil_VIVO_STK_Codec_Correct

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulVivo;
}OM_VIVO_STK_CODEC_STRU;

/*****************************************************************************
    : NV_HUAWEI_DOUBLE_IMSI_CFG_I_STRU
  :
NVID: 50041 NV_HUAWEI_DOUBLE_IMSI_CFG_I

  1.       : 2013516
           : j00174725
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucCfg;
    VOS_UINT8                           ucEnable;
}NV_HUAWEI_DOUBLE_IMSI_CFG_I_STRU;

/*****************************************************************************
    : NV_ID_WRITE_SLICE_RECORD_FLAG_STRU
  :
NVID: 52007 NV_ID_WRITE_SLICE_RECORD_FLAG

  1.       : 2013711
           : d00212987
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulRecordFlag;
}NV_ID_WRITE_SLICE_RECORD_FLAG_STRU;

/*****************************************************************************
    : NV_SOCP_SDLOG_CFG_STRU
  :
NVID:   NV_ID_SOCP_SDLOG_CFG

  1.       : 2013711
           : z00212940
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32 ulSocpDelayWriteFlg;/* SOCP */
    VOS_UINT32 ulIndSocpLevel;     /* INDBUFFER */
    VOS_UINT32 ulCfgSocpLevel;     /* CFGBUFFER */
    VOS_UINT32 ulTimeOutValue;     /* SOCPBUFFER */
}NV_SOCP_SDLOG_CFG_STRU;

/*****************************************************************************
    : NV_ID_ERR_LOG_CTRL_INFO_STRU
  :
NVID: 133 NV_ID_ERR_LOG_CTRL_INFO

  1.       : 2013827
           : d00212987
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucAlmStatus; /* ,0:close;1:open  */
    VOS_UINT8                           ucAlmLevel;  /* &
                                                                Warning0x04
                                                                Minor0x03
                                                                Major0x02
                                                                Critical0x01
                                                              0x03 0x03/0x02/0x01 */
    VOS_UINT8                          aucReportBitMap[2]; /* A0000000 00000DCB
                                                              A
                                                              B
                                                              C
                                                              DRAT
                                                           */
}NV_ID_ERR_LOG_CTRL_INFO_STRU;

/*****************************************************************************
    : OM_ALARM_ID_DETAIL_STRU
  :
  1.       : 2013827
           : d00212987
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulAlarmid;        /*  */
    VOS_UINT32                          ulAlarmidDetail;  /* 32bitbit0 */
}OM_ALARM_ID_DETAIL_STRU;

/*****************************************************************************
    : NV_ALARM_ID_RELATIONSHIP_STRU
  :
NVID: 134 NV_ALARM_ID_RELATIONSHIP

  1.       : 2013827
           : d00212987
       : 
*****************************************************************************/
typedef struct
{
    OM_ALARM_ID_DETAIL_STRU          astOmAlarmidRelationship[40]; /* 40 */
}NV_ALARM_ID_RELATIONSHIP_STRU;

/*****************************************************************************
    : NV_ID_FTM_DETAIL_STRU
  :
NVID: 135 NV_ID_FTM_DETAIL

  1.       : 2013827
           : d00212987
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulFTMDetail; /* 32bitbit0 */
}NV_ID_FTM_DETAIL_STRU;

/*****************************************************************************
     : VSIM_KEYDATA_STRU
   : 
   : 

       :
  1.       : 2013827
           : zhuli
       : V9R1 vSIM
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulKeyLen;
    VOS_UINT8                           aucKey[VSIM_KEYLEN_MAX];
}VSIM_KEYDATA_STRU;

/*****************************************************************************
     : VSIM_CARD_STATE_NV_STRU
   : 
   : 

       :
  1.       : 2013827
           : zhuli
       : V9R1 vSIM
*****************************************************************************/
typedef struct
{
    VSIM_ACTIVE_TYPE_ENUM_UINT8         enVsimState;
    VOS_UINT8                           aucRfu[3];
}VSIM_CARD_STATE_NV_STRU;

/*****************************************************************************
     : VSIM_CARD_LOCI_FILE_NV_STRU
   : 
   : EF_LOCI

       :
  1.       : 2013916
           : H59524
       : V9R1 vSIM
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucEfloci[VSIM_EF_LOCI_LEN];
    VOS_UINT8                           ucRsv;
}VSIM_CARD_LOCI_FILE_NV_STRU;

/*****************************************************************************
     : VSIM_CARD_PSLOCI_FILE_NV_STRU
   : 
   : PS_EF_LOCI

       :
  1.       : 2013916
           : H59524
       : V9R1 vSIM
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucPsEfloci[VSIM_EF_PS_LOCI_LEN];
    VOS_UINT16                          usReserved;
}VSIM_CARD_PSLOCI_FILE_NV_STRU;

/*****************************************************************************
     : VSIM_CARD_FPLMN_FILE_NV_STRU
   : 
   : EF_FPLMN

       :
  1.       : 2013916
           : H59524
       : V9R1 vSIM
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucFplmn[VSIM_EF_FPLMN_LEN];
}VSIM_CARD_FPLMN_FILE_NV_STRU;

/*****************************************************************************
     : NVIM_VSIM_HVSDH_NV_STRU
   : 
   : NV

       :
  1.       : 2013827
           : zhuli
       : V9R1 vSIM
*****************************************************************************/
typedef struct
{
    VSIM_ALGORITHM_TYPE_ENUM_UINT32     enAlgorithm;
    VOS_UINT32                          ulDHLen;         /* DH */
    VSIM_KEYDATA_STRU                   stCPrivateKey;   /*  */
    VSIM_KEYDATA_STRU                   stCPublicKey;    /*  */
    VSIM_KEYDATA_STRU                   stSPublicKey;    /*  */
}NVIM_VSIM_HVSDH_NV_STRU;

/*****************************************************************************
    : NV_MODE_BASIC_PARA_STRU
  :
NVID:

  1.       : 2013927
           : g47350
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucABBSwitch;    /*ABB0ABB CH01ABB CH12*/
    VOS_UINT8                           ucRFSwitch;     /*RF0RFmipi0PA-Star1RFmipi1PA-StarSmart-StarBUCK62RFIC*/
    VOS_UINT8                           ucTCXOSwitch;   /*TCXO0TCXO01TCXO */
    VOS_UINT8                           ucRsv1;
}NV_MODE_BASIC_PARA_STRU;

/*****************************************************************************
    : NV_GUMODE_CHAN_PARA_STRU
  :138 en_NV_Item_GUMODE_CHAN_PARA
NVID:

  1.       : 2013927
           : g47350
       : 
*****************************************************************************/

typedef struct
{
    NV_MODE_BASIC_PARA_STRU             astModeBasicParam[2];    /**/
}NV_GUMODE_CHAN_PARA_STRU;


/*****************************************************************************
     : NV_RECORD_BASE_BOARD_XML_STRU
   : 
   : 139 XMLID

       :
  1.       : 20131021
           : d00212987
       :
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucXMLBaseBoardId[24]; /* base board xml  */
}NV_RECORD_BASE_BOARD_XML_STRU;


/*****************************************************************************
     : NV_RECORD_CURRENT_BOARD_XML_STRU
   : 
   : 140 XMLID

       :
  1.       : 20131021
           : d00212987
       :
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           aucXMLCurrentBoardId[24]; /* Currnet board xml  */
}NV_RECORD_CURRENT_BOARD_XML_STRU;

/*****************************************************************************
     : NV_FLASH_LOG_RECORD_STRU
   : 
   : 143 FLASHTRACE

       :
  1.       : 20131227
           : s00207770
       :
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulGULogFileSize;
    VOS_UINT32                          ulTLLogFileSize;
}NV_FLASH_LOG_RECORD_STRU;

/*****************************************************************************
     : NVIM_USIM_CARD_STATUS_CB_TYPE_STRU
   : SIMNV

  1.       : 20130925
           : h59254
       : V3R3 SIM
*****************************************************************************/
typedef struct
{
    USIMM_CARD_STATUS_REG_TYPE_ENUM_UINT16                  enType;
    VOS_UINT16                                              usReserved;
}NVIM_USIM_CARD_STATUS_CB_TYPE_STRU;

/*****************************************************************************
     : NV_EVENT_RESEND_CFG_STRU
   : 
   : 145 C+LNV

       :
  1.       : 2014115
           : z00100318
       :
*****************************************************************************/
typedef struct
{
    EVENT_RESEND_FLAG_ENUM_UINT8            enResendFlag;   /*10*/
    VOS_UINT8                               ucRetryTime;    /*0*/
    VOS_UINT8                               ucTimerLen;     /*, 0*/
    VOS_UINT8                               ucRsv;
}NV_EVENT_RESEND_CFG_STRU;

/*****************************************************************************
     : USIMM_DL_T_MODE_ENUM
   : T

  1.       : 20131021
           : L00256032
       : V9R1 T=1
*****************************************************************************/
enum USIMM_DL_T_MODE_ENUM
{
    USIMM_DL_T_MODE_T0        = 0,    /* T=0 */
    USIMM_DL_T_MODE_T1        = 1,    /* T=1 */
    USIMM_DL_T_MODE_BUTT
};
typedef VOS_UINT32      USIMM_DL_T_MODE_ENUM_UINT32;

/*****************************************************************************
     : USIMM_T1_ERR_DETECT_MODE_ENUM
   : T=1EDC
             ts_102221v110000pEDC1LRC
              NVts_102221ISO_IEC 7816-3

  1.       : 20131021
           : L00256032
       : V9R1 T=1
*****************************************************************************/
enum USIMM_T1_ERR_DETECT_MODE_ENUM
{
    USIMM_T1_ERR_DETECT_MODE_ISO_IEC_7816_3  = 0,           /* NVISO_IEC 7816-3 2006EDCATR(LRCCRC) */
    USIMM_T1_ERR_DETECT_MODE_TS102221        = 1,           /* TS_102221v110000pEDCLRC */

    USIMM_T1_ERR_DETECT_MODE_BUTT
};
typedef VOS_UINT32 USIMM_T1_ERR_DETECT_MODE_ENUM_UINT32;

/*****************************************************************************
     : USIMM_DL_T_MODE_ENUM
   : T

  1.       : 20131021
           : L00256032
       : V9R1 T=1
*****************************************************************************/
enum USIMM_DL_T1_GCF_FLAG_ENUM
{
    USIMM_DL_T1_GCF_DISABLE       = 0,    /* GCF */
    USIMM_DL_T1_GCF_ENABLE        = 1,    /* GCF */
    USIMM_DL_T1_GCF_BUTT
};
typedef VOS_UINT32      USIMM_DL_T1_GCF_FLAG_ENUM_UINT32;


/*****************************************************************************
    : NV_USIMM_T1_CTRL_STRU
  : en_NV_Item_Usimm_T1_Ctrl_PARA
NVID:

  1.       : 20131021
           : L00256032
       : V9R1 T=1
*****************************************************************************/
typedef struct
{
    USIMM_DL_T_MODE_ENUM_UINT32                             enTMode;
    USIMM_T1_ERR_DETECT_MODE_ENUM_UINT32                    enEDM;
    USIMM_DL_T1_GCF_FLAG_ENUM_UINT32                        enGcfFlag;
    VOS_UINT32                                              ulDefaultIFSD;
    VOS_UINT32                                              aulRsv[2];
}NV_USIMM_T1_CTRL_PARA_STRU;

/*****************************************************************************
     : NV_OM_CBT_LOG_ENABLE_STRU
   : CBT LOGNV 146

  1.       : 20140123
           : d00212987
       : V9R1 CBT LOG
*****************************************************************************/
typedef struct
{
    VOS_BOOL                           bCBTLogEnable;
}NV_OM_CBT_LOG_ENABLE_STRU;

/*****************************************************************************
     : NV_SLEEP_DRX_RESET_ENABLE_STRU
   : SLEEPNV 149

  1.       : 20140313
           : s00207770
       : V9R1
*****************************************************************************/
typedef struct
{
    VOS_BOOL                          bResetEnable;
}NV_SLEEP_DRX_RESET_ENABLE_STRU;

/*****************************************************************************
     : NV_ESN_MEID_STRU
   : NV_ESN_MEID_STRU 9283

  1.       : 2014073
           : zhuli
       : X
*****************************************************************************/
typedef struct
{
    ESN_MEID_FLAG_ENUM_UINT8        enEsnMeIDFlag;
    VOS_UINT8                       ucRSv;
    VOS_UINT8                       aucEsn[ESN_NV_DATA_LEN];  /*Len+Data*/
    VOS_UINT8                       aucMeID[MEID_NV_DATA_LEN]; /*Len+Data*/
    VOS_UINT8                       aucRfu[6];
}NV_ESN_MEID_STRU;

/*****************************************************************************
     : NV_DDR_ADJUST_ENABLE_STRU
   : DDR

  1.       : 20141113
           : s00207770
       : V8R2
*****************************************************************************/
typedef struct
{
    VOS_BOOL                            bEnable;       /**/
    VOS_UINT32                          ulRsv;
}NV_DDR_ADJUST_ENABLE_STRU;

/*****************************************************************************
     : NV_PESN_STRU
   : NV_PESN_STRU 4002

  1.       : 20150717
           : z00316370
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                       aucPEsn[PESN_NV_DATA_LEN];      /*Data*/
}NV_PESN_STRU;

/*****************************************************************************
     : NV_MEID_STRU
   : NV_MEID_STRU 4003

  1.       : 20150717
           : z00316370
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                       aucMeID[MEID_NV_DATA_LEN_NEW];  /*Data*/
    VOS_UINT8                       aucRsv[1];
}NV_MEID_STRU;

/*****************************************************************************
     : USIMM_NV_CARDAPP_ENUM
   : USIMM

  1.       : 20150323
           : ganlan
       : V8R2 UICC MutliApp PhaseI
*****************************************************************************/
enum USIMM_NV_CARDAPP_ENUM
{
    USIMM_NV_GUTL_APP          = 0x00000000,
    USIMM_NV_IMS_APP           = 0x00000001,
    USIMM_NV_CDMA_APP          = 0x00000002,
    USIMM_NV_CARDAPP_BUTT      = 0x00000003
};
typedef VOS_UINT32 USIMM_NV_CARDAPP_ENUM_UINT32;

/*****************************************************************************
     : USIMM_APP_PRIORITY_CONFIG_STRU
   : USIMM

  1.       : 20150323
           : ganlan
       : V8R2 UICC MutliApp PhaseI
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucAppNum;
    VOS_UINT8                           aucRsv[3];
    USIMM_NV_CARDAPP_ENUM_UINT32        aenAppList[5];
}USIMM_APP_PRIORITY_CONFIG_STRU;

/*****************************************************************************
     : NV_SLEEP_BBE16_CTRL_STRU
   : BBE16

  1.       : 20151029
           : xushutao
       : X
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulSleepBBE16Switch;
}NV_SLEEP_BBE16_CTRL_STRU;

/*****************************************************************************
     : SC_PERS_PLATFORM_ENUM_UINT32
   : SC Modem

  1.       : 20160106
           : d00212987
       : SCCDMA
*****************************************************************************/
enum SC_PERS_PLATFORM_ENUM
{
    SC_PERS_PLATFORM_G,   /*  */

    SC_PERS_PLATFORM_C,   /*  */

    SC_PERS_PLATFORM_BUT,
};
typedef VOS_UINT32 SC_PERS_PLATFORM_ENUM_UINT32;

/*****************************************************************************
     : SC_PERS_SUPPORT_PLATFORM_STRU
   : SC Modem

 1.       : 20160106
          : d00212987
      : SCCDMA
*****************************************************************************/
typedef struct
{
    SC_PERS_PLATFORM_ENUM_UINT32        enScPersModem;
}SC_PERS_SUPPORT_PLATFORM_STRU;

/*****************************************************************************
     : NV_SC_PERS_CTRL_STRU
   : SC 

  1.       : 20151130
           : d00212987
       : SC
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usImei0ReplaseImei2;
    VOS_UINT16                          usRsv;
}NV_SC_PERS_CTRL_STRU;

/*****************************************************************************
     : NV_XPDS_FEATURE_CONTROL_STRU
   : xpds

 1.       : 20160126
          : d00212987
      : xpds
*****************************************************************************/
typedef struct
{
    VOS_UINT8                   ucSiWaitGpsPseudoCnfTimeLen;   /* NIGPS: */
    VOS_UINT8                   ucNiWaitGpsPseudoCnfTimeLen;   /* NIGPS: */
    VOS_UINT8                   ucWaitCasRefLocIndTimeLen;     /* CAS REF LOC IND: */
    VOS_UINT8                   ucMsaMaxPilotCnt;              /* MSAPilot */

    VOS_UINT8                   ucRsv[8];
}NV_XPDS_FEATURE_CONTROL_STRU;

/*****************************************************************************
     : NV_ISIM_ENABLE_STRU
   : ISIMNV

  1.       : 20160311
           : z00316370
       : PAM_USIM_IT5_ISIM 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucIsimEnableFlag;
    VOS_UINT8                           aucRsv[3];
}NV_ISIM_ENABLE_STRU;

/*****************************************************************************
     : USIMM_ACK_PROTECT_NV_STRU
   : ACKNV

  1.       : 20160822
           : g00256031
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucEnable;               /* 10 */
    VOS_UINT8                           ucRsv;
    VOS_UINT16                          usProtectTime;          /*  */
}USIMM_ACK_PROTECT_NV_STRU;

/*****************************************************************************
     : USIMM_CHECK_KEY_FILE_NV_STRU
   : UsimNVNV_ID: 4013 

  :
1.        : 201695
          : d00212987
      : USIM API
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucEnable;               /* 10  */
    VOS_UINT8                           ucInitCheckTimeLen;     /* 5s */
    VOS_UINT8                           ucCheckTimeLen;         /* 60s */
    VOS_UINT8                           ucRcv;
}USIMM_CHECK_KEY_FILE_NV_STRU;

/*****************************************************************************
     : USIMM_DISABLE_REAL_ISIM_CTRL_NV_STRU
   : ISIM

  1.       : 20161124
           : z00316370
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucDisableRealIsim;               /* 1ISIM0ISIM */
    VOS_UINT8                           aucRsv[3];
}USIMM_DISABLE_REAL_ISIM_CTRL_NV_STRU;

/*****************************************************************************
     : NV_SC_PERS_IMEI_BIND_SLOT_STRU
   : IMEI

  1.       : 2017410
           : d00212987
       : IMEI
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usImeiBindSlotFlag;
    VOS_UINT16                          usRsv;
}NV_SC_PERS_IMEI_BIND_SLOT_STRU;

/*****************************************************************************
     : USIM_CLOSE_CHANNEL_SND_STATUS_STRU
   : status,
 NV ID : 4045

  1.       : 20171125
           : d00212987
       : Creat
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulEnable;
}USIM_CLOSE_CHANNEL_SND_STATUS_STRU;

/*****************************************************************************
     : USIMM_PROVIDE_COMMAND_CTRL_NV_STRU
   : 
 NV ID : 4046

  1.       : 20171127
           : z00377832
       : UNIFILOCAL INFORMATION
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulFilterProvideCommandFlag;
}USIMM_PROVIDE_COMMAND_CTRL_NV_STRU;

/*****************************************************************************
    : USIMM_STATUS_PROTECTRESET_CHECKSW_STRU
  : StatusCheckSWNV
NVID:  4072
:
  1.       : 2018322
           : d00212987
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulCheckSwFlg;
}USIMM_STATUS_PROTECTRESET_CHECKSW_STRU;

/*****************************************************************************
    : USIMM_PLMN_DIGIT_NV_CTRL_STRU
  : MNCNV
NVID  : 4074
:
  1.       : 201876
           : z00377832
       : 
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulEnable; /* 0:disable, 1:enable */
    VOS_UINT32                          ulThreeBytesMncNum;
    VOS_UINT32                          aulThreeBytesMncPlmnData[USIMM_PLMN_DATA_MAX_LEN];
    VOS_UINT32                          ulTwoBytesMncNum;
    VOS_UINT32                          aulTwoBytesMncPlmnData[USIMM_PLMN_DATA_MAX_LEN];
}USIMM_PLMN_DIGIT_NV_CTRL_STRU;

/*****************************************************************************
  8 UNION
*****************************************************************************/


/*****************************************************************************
  9 OTHERS
*****************************************************************************/

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of NasNvInterface.h */
