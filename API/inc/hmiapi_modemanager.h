/*******************************************************
 File Name   : hmiapi_modemanager.h

 Created by  :  Dong.Sen(Neusoft)

 Created Date:  2015/07/15
l
Description   :   Modemanager Hmiapi
*********************************************************/
#ifndef MODEMANAGER_H
#define MODEMANAGER_H

/****************Include  Section Begin***************************************************/
#include "singleton.h"
#include "hmiapi_abstract.h"
#include "hmiapi_audiomanager.h"
#ifndef HMI_API_STUB
#include "panda_dbus.h"
#include "panda_dbus_private.h"
#endif //HMI_API_STUB
/****************Include  Section End*****************************************************/
typedef int tInterfaceClient_sx5_modemanager;
typedef long HProxy;
/****************Marco Definition Section Begin*******************************************/
#define UNUSED_PARAMETER(x) (void)x
#define ModemanagerClassIns   ModemanagerClass::Instance()
#define D_HMIAPI_EV_MODE_OBTAIN_GETTED          M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x01)
#define D_HMIAPI_EV_MODE_STATUS_FOCUSED  	    M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x02)
#define D_HMIAPI_EV_MODE_STATUS_ABOLISH     	M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x03)
#define D_HMIAPI_EV_FRM_FIRST_BASE_SRC_ACTIVE   M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x04)
#define D_HMIAPI_EV_MODE_VIEDIO_FORBID          M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x05)
#define D_HMIAPI_EV_MODE_LASTMODE_GETTED        M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x06)
#define D_HMIAPI_EV_MODE_GET_LAST_ACHIEVED      M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x07)
#define D_HMIAPI_EV_MODE_DIAGNOSE_IN            M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x09)
#define D_HMIAPI_EV_MODE_DIAGNOSE_OUT           M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x0A)
#define D_HMIAPI_EV_MODE_POWERMODE_CHANGED      M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x0B)
#define D_HMIAPI_EV_MODE_BATSTATE_CHANGED       M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x0C)
#define D_HMIAPI_EV_MODE_ONEMIN_LEFT            M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x0D)
#define D_HMIAPI_EV_MODE_BAT_TOOLOW             M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x0E)
#define D_HMIAPI_EV_MODE_BAT_TOOHIGH            M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x0F)
#define D_HMIAPI_EV_MODE_CHGPHONE_TOCALL        M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x10)
#define D_HMIAPI_EV_MODE_RVC_IN                 M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x11)
#define D_HMIAPI_EV_MODE_RVC_OUT                M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x12)
#define D_HMIAPI_EV_MODE_CAST_FOCUSED           M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x13)
#define D_HMIAPI_EV_MODE_TOD_ON                 M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x14)
#define D_HMIAPI_EV_MODE_TOD_OFF                M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x15)
#define D_HMIAPI_EV_MODE_AUDIO_ON               M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x16)
#define D_HMIAPI_EV_MODE_AUDIO_OFF              M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x17)
#define D_HMIAPI_EV_MODE_ENABLE_TOUCHPANEL_CHANGE    M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x18)
#define D_HMIAPI_EV_MODE_ENABLE_HARDKEY_CHANGE       M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x19)
#define D_HMIAPI_EV_MODE_SAVE_LASTSCREEN        M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x1A)
#define D_HMIAPI_EV_MODE_SHOW_DVR              M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x1C)
#define D_HMIAPI_EV_MODE_SHOW_AVM               M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x1D)
#define D_HMIAPI_EV_MODE_SHOW_RVC              M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x21)
#define D_HMIAPI_EV_MODE_EXIT_AVM_RVC_DVR      M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x22)
//#define D_HMIAPI_EV_MODE_SOURCE_CHANGE_VR       M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x23)
#define D_HMIAPI_EV_MODE_ENTER_AVM_DISP              M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x2D)
#define D_HMIAPI_EV_MODE_ENTER_RVC_DISP              M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x2E)
//#define D_HMIAPI_EV_MODE_SOURCE_CHANGE_VR_NAVI       M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x25)
#define D_HMIAPI_EV_MODE_CARPLAYRING_STOP       M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x26)
#define D_HMIAPI_EV_MODE_VIDEO_PAUSED     	    M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x27)
#define D_HMIAPI_EV_MODE_VIDEO_RESUME           M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x28)
//#define D_HMIAPI_EV_MODE_DISP_SHOW_PDC          M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x29)
//#define D_HMIAPI_EV_MODE_DISP_HIDE_PDC          M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x2A)

//#define D_HMIAPI_EV_MODE_VEHICLEMODEL           M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x29) //delete
//#define D_HMIAPI_EV_MODE_360PANORAMIC           M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x2A) //delete
#define D_HMIAPI_EV_MODE_CARPLAYCHANGE          M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x2B)
#define D_HMIAPI_EV_MODE_SAVE_EVLASTSCREEN      M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x2C)
#define D_HMIAPI_EV_MODE_CAST_ABOLISH           M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x31)
//#define D_HMIAPI_EV_MODE_OPM_STATUS             M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x32)
#define D_HMIAPI_EV_MODE_OPM_AUTHORITY          M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x33)
#define D_HMIAPI_EV_MODE_NAVI_IS_READY          M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x34)
#define D_HMIAPI_EV_MODE_NAVI_VOLBAR_HIDE          M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x35)
#define D_HMIAPI_EV_MODE_COLORBARSHOW           M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x36)
#define D_HMIAPI_EV_MODE_COLORBARHIDE           M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x37)
#define D_HMIAPI_EV_MODE_PHONE_ABOLISH          M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x38)
#define D_HMIAPI_EV_MODE_IGN_STATUS             M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x39)
#define D_HMIAPI_EV_MODE_HMISHOWING               M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x3A)
#define D_HMIAPI_EV_MODE_HMIISREADY               M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x3C)

#define D_HMIAPI_EV_MODE_USB_EJECT_COMPLETE     M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x1F)
#define D_HMIAPI_EV_MODE_IPOD_EJECT_COMPLETE    M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x2F)
#define D_HMIAPI_EV_MODE_BTA_EJECT_COMPLETE     M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x3F)
#define D_HMIAPI_EV_MODE_AUX_EJECT_COMPLETE     M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x4F)
#define D_HMIAPI_EV_MODE_CYCLE_NEXT_SOURCE      M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x5F)
#define D_HMIAPI_EV_MODE_POWER_ON_UNMUTE        M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x6F)
#define D_HMIAPI_EV_MODE_DIAGONS_CHANGED        M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x7F)
#define D_HMIAPI_EV_MODE_STATUS_CHANGED         M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x8F)
#define D_HMIAPI_EV_MODE_LINK_EJECT_COMPLETE    M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x9F)
#define D_HMIAPI_EV_MODE_CARSPEEDCHANG          M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x41)
#define D_HMIAPI_EV_MODE_PMSTATECHANGE          M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x42)
#define D_HMIAPI_EV_MODE_NAVISPEAK              M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x43)
#define D_HMIAPI_EV_MODE_LINK_FOCUSED           M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x44) //CM7C-3130 Bug is Modified By zhoudan
#define D_HMIAPI_EV_MODE_VR_VOL_END             M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x45) //CM7C-3392
#define D_HMIAPI_EV_MODE_FIRST_BASE_SRC_ACTIVE    M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x46)
#define D_HMIAPI_EV_MODE_NEED_CHANGE2SRCVIEW    M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x47)
#define D_HMIAPI_EV_MODE_USBPLAYCHANGE        M_HMIAPI_MAKEEV(D_HMIAPI_DOMAIN_MODEMANAGER, 0x48)//wangtingting add for sx5-477

#define HMI_MODE_SPECIALMODE_NONE       (0)
#define HMI_MODE_SPECIALMODE_NAVI       (1)
#define HMI_MODE_SPECIALMODE_PHONE      (2)
#define HMI_MODE_SPECIALMODE_MIRRORLINK (3)
#define HMI_MODE_SPECIALMODE_MIRACAST   (4)
#define HMI_MODE_SPECIALMODE_CARPLAY    (5)

#define HMI_MODE_DIAGNOSE_STATUS_OFF    (0)
#define HMI_MODE_DIAGNOSE_STATUS_ON     (1)

#define HMI_MODE_NAVISPEAK_STATUS_OFF    (0)
#define HMI_MODE_NAVISPEAK_STATUS_ON     (1)
#define HMI_MODE_CARPLAYALT_STATUS_OFF    (0)
#define HMI_MODE_CARPLAYALT_STATUS_ON     (1)

#define SWITCH_DEBUG (1)
/****************Marco Definition Section End*********************************************/

/****************Struct Definition Section Start*********************************************/

enum HMI_Mode
{
    E_MODE_SRC_TUNER,
    E_MODE_SRC_USB_AUDIO,
    E_MODE_SRC_USB_VIDEO,
    E_MODE_SRC_HDD_AUDIO,
    E_MODE_SRC_HDD_VIDEO,
    E_MODE_SRC_IPOD,
    E_MODE_SRC_AUX,
    E_MODE_SRC_BTAUDIO,
    E_MODE_SRC_MIRRORLINK,
    E_MODE_SRC_MIRACAST,
    E_MODE_SRC_CARPLAY,
    E_MODE_INT_PHONE,
    E_MODE_INT_RINGTONE,
    E_MODE_INT_ECALL,
    E_MODE_INT_CARPLAY_TEL,
    E_MODE_INT_CARPLAY_RING,
    E_MODE_INT_CARPLAY_SIRI,
    E_MODE_INT_VR,
    E_MODE_MIX_NAVI,
    E_MODE_MIX_CARPLAY_ALT,
    E_MODE_MAX,
};

enum HMI_ModeStatus
{
    E_MODE_STATUS_ACTIVE,
    E_MODE_STATUS_PAUSED,
    E_MODE_STATUS_ABOLISH,
    E_MODE_STATUS_FOCUSED,
};

enum HMI_ScreenType
{
   E_SCREEN_HOME,
   E_SCREEN_SOURCE,
   E_SCREEN_NAVI,
   E_SCREEN_TOD_SOURCE = 30,
   E_SCREEN_TOD_NAVI,
};

enum HMI_PowerMode
{
   E_SYSTEM_SLEEP,
   E_HMI_INACTIVE,
   E_ACC_ON_NORMAL,
   E_ACC_ON_BLACK,
   E_ACC_ON_CALL_ON,
   E_ACC_ON_TOD_ON,
   E_ACC_ON_AVM,
   E_ACC_ON_RVC,
   E_ACC_ON_AVM_CALL,
   E_ACC_ON_RVC_CALL,
   E_ACC_ON_CALL_AVM,
   E_ACC_ON_CALL_RVC,
   E_ACC_ON_BAT_BAD,
   E_SYSTEM_STANDBY,
   E_SLEEP_PROCESS,
   E_STATE_NONE,
};

enum SX5Mode
{
    SX5_MODE_SRC_TUNER,
    SX5_MODE_SRC_USB_AUDIO,
    SX5_MODE_SRC_USB_VIDEO,
    SX5_MODE_SRC_HDD_AUDIO,
    SX5_MODE_SRC_HDD_VIDEO,
    SX5_MODE_SRC_IPOD,
    SX5_MODE_SRC_AUX,
    SX5_MODE_SRC_BTAUDIO,
    SX5_MODE_SRC_MIRRORLINK,
    SX5_MODE_SRC_MIRACAST,
    SX5_MODE_SRC_CARPLAY,
    SX5_MODE_INT_PHONE,
    SX5_MODE_INT_RINGTONE,
    SX5_MODE_INT_ECALL,
    SX5_MODE_INT_CARPLAY_TEL,
    SX5_MODE_INT_CARPLAY_RING,
    SX5_MODE_INT_CARPLAY_SIRI,
    SX5_MODE_INT_VR,
    SX5_MODE_MIX_NAVI,
    SX5_MODE_MIX_CARPLAY_ALT,
    SX5_MODE_MAX,
};
enum ModeStatus
{
    SX5_MODE_STATUS_ACTIVE,
    SX5_MODE_STATUS_PAUSED,
    SX5_MODE_STATUS_ABOLISH,
    SX5_MODE_STATUS_FOCUSED,
    MODE_STATUS_UNVALIABLE,
};
//juanjuan add start
typedef struct
{
    enum SX5Mode mode;
    enum ModeStatus status;
} HMI_SRCMode;
//juanjuan add end
/****************Struct Definition Section End*********************************************/
#ifndef HMI_API_STUB
extern HProxy PROXY_MODE;
#endif //HMI_API_STUB
/****************Class Declaration Section Begin*******************************************/
class ModemanagerClass;
class KeyToneClass: public QObject
{
    Q_OBJECT
public:
    KeyToneClass(QObject* parent = 0);
    ~KeyToneClass();
public slots:
    void req_keytone();

};

class ModemanagerClass : public AbstractAPIClass
{
   Q_OBJECT
#if 0
   DECLARE_SINGLETON(ModemanagerClass)
#else
protected: \
        ModemanagerClass();\
        static  ModemanagerClass* m_pInstance; \
public:  \
        static  ModemanagerClass* Instance();
#endif
public:

    ~ModemanagerClass();

    void sendEvent(int eventID);
    void concreteEvent(QEvent *);
    void Modemanager_Init(QObject* parent = 0);

    /*juanjuan add start*/
    HMI_SRCMode Get_BaseSrcMode(){return m_baseSrcMode;}
    HMI_SRCMode Get_IntSrcMode(){return m_intSrcMode;}
    HMI_SRCMode Get_CurSrcMode(){return m_curSrcMode;}
    HMI_SRCMode Get_PreSrcMode(){return m_preSrcMode;}
    SX5Mode Get_ReqBaseSrcMode(){return m_reqBaseSrcMode;}
    /*juanjuan add end*/
    int  Get_CurrentMode();
    int  Get_CarplayMode();
    int  Get_LastMode();
    int  Get_PreviousMode();
    int  Get_UsbPreviousMode(); //14188	CM7C-2900
    int  Get_SpecialMode();
    int  Get_NavispekStatus();
	int  Get_opm_authority();
    //SX5-6832
    void  Set_avm_type(int var);
//    int  Get_opmStatus();
    int  Get_panel_enable();
    int  Get_hardkey_enable();
    void Set_hardkey_enable(int enable);
    //16015	CM7C-3715
    void Set_mode_src(enum HMI_Mode mode);
    void Req_ModeRequest(enum HMI_Mode request);

    void Req_ModeRelease(enum HMI_Mode request);
    void Req_ModeObtain();
    void Req_disp_key_avm_rvc(int i_type,int i_state);
    void Req_call_hmi_is_ready();
//ren remove start 0810
    //void Req_opm_authority();
//ren remove end 0810
    void Ope_ShowlayerManager(int modeID,int screenId); //wangtingting merge on 0425
    void Ope_Shownavi();
    void Ope_Hidenavi();
    void Ope_Showcarplay();
    void Ope_Hidecarplay();
    void Ope_ShowWelink(); //1219
    void Ope_HideWelink();
    void Ope_ShowMirrorLink();
    void Ope_HideMirrorLink();
    void Ope_ShowMiraCast();
    void Ope_HideMiraCast();
    void Ope_Showusbv();
    void Ope_Hideusbv();
    void Ope_PowerPress(int flag);
    void Ope_play_key_sound();
    void Ope_DVR_OnOff();
    void Req_GetPmState();
    int Get_hk_dispflag();
    void Set_hk_dispflag(int i_flag);

    void Set_PreviousMode(int ipremode);
    enum HMI_ScreenType Get_ScreenType();
    void Ope_save_last_screent(enum HMI_ScreenType type);
//	void Ope_set_opm_state(unsigned int state);

    void on_mode_obtain(HProxy proxy, enum SX5Mode mode);
    void on_signal_status_change(HProxy proxy, enum SX5Mode mode, enum ModeStatus status);
    void on_signal_video_pause(HProxy proxy, enum SX5Mode mode);
    void on_signal_mode_resume(HProxy proxy, enum SX5Mode mode);
    void on_signal_video_forbid(HProxy proxy);
#if SWITCH_DEBUG
    void on_get_event_authority(HProxy proxy, unsigned int SK, unsigned int HK);
#endif
    void on_signal_pdc_rvc_avm_show(HProxy proxy, unsigned int type);
    //void on_signal_pm_state_changed(HProxy proxy, enum PowerMode state);
    void on_signal_screen_showing(HProxy proxy, unsigned int type);
	void on_get_opm_authority(HProxy proxy, unsigned int authority);
    void on_signal_opm_authority_changed(HProxy proxy, unsigned int authority);
	void on_signal_navigation_ready(HProxy proxy);
    void on_signal_hide_volume_bar(HProxy proxy);
    void on_signal_show_colorbar(HProxy proxy, unsigned int side, unsigned int show);
 //   void on_signal_only_update_parkview(HProxy proxy, unsigned int state);
    //void on_signal_ign_state_changed(HProxy proxy, enum IgnState state);
//    void on_signal_opm_state_changed(HProxy proxy, unsigned int state);
    void on_signal_car_state_change(HProxy proxy, unsigned int state);

    void sendToFrmModeManager(int i_eventID);
    void set_diagnose_status(int i_var);
    int  get_diagnose_status();

    bool is_AVM_available();
    bool is_Electricvehicle();
    bool is_TBox_available();

    void set_isFirstTimePowerOn_Status(bool bIsFirstPowerOnFlag);
    bool get_is_firstTimePowerOn();
    bool is_navishow();
    bool is_carplayshow();
    int get_hmi_is_ready();
    int get_dvrons_time(); //CM7C-2600
    int Get_RequestMode();
    void Set_RequestMode(int i_var);
    void set_carPlayOnOff(int bCarPlayOnOff);
    int get_carPlayOnOff();
	bool is_linkshow();
    bool is_castshow();
    void send_lastscreen_save();
    int Get_avm_rvc_pdc();
    int Get_Todstatus();
    int Get_volummode();
    void Set_volummode(int mode);
    int Get_SourceMode();
	bool Get_isnaviready();
	void Ope_SoftUpgrade();
    void Ope_ctrlvolbar(int i_ope);
    int Get_PmState();
    int Get_hmiShow();
    void Set_dvrons_time(int time); //CM7C-2600
    int Get_VideoPaused();
    void Set_VideoPaused();
    int Get_ign_status();
    int Get_SrcMode();

    int Get_befoerVRsource();
    int Get_Carspeed();
    int Get_preMode();
    int Get_beforeUsbvSource();
    void Req_transferred_over();
    void Set_quitAVMRVCDVR_flag();
//SX7-309 Bug is Modified By zhoudan
    void Set_ReqModeCallType(int i_callType);
//SX7-309 Bug is Modified By zhoudan

    int return_available_Source(int mode);//juanjuan add
    void startKeyTouchTimer(); //start a timer to disable key and touch for a while if nessesary
    void killKeyTouchTimer(); //kill this timer and enable key and touch if nessesary
    //wangtingting add for sx5-477 start
    void set_usbVideoswitchOnOff(int iUsbVideoswitchOnOff);
    int get_usbVideoswitchOnOff();
    //wangtingting add for sx5-477 end
private:
#ifdef HMI_API_STUB
    HProxy PROXY_MODE;
#else //HMI_API_STUB
    struct tRemoteComponent proxy_MODEMANAGER;
#endif //HMI_API_STUB

    int chgModeToHMI(enum SX5Mode mode);
    void on_signal_srcmode_change(HProxy proxy, enum SX5Mode mode, enum ModeStatus status);//juanjuan add
    Frm_CommonClass* frameWorkClass;
    /*juanjuan add start*/
    HMI_SRCMode m_baseSrcMode;
    HMI_SRCMode m_intSrcMode;
    HMI_SRCMode m_curSrcMode;
    HMI_SRCMode m_preSrcMode;
    SX5Mode m_reqBaseSrcMode;
    /*juanjuan add end*/
    int m_icurmode;
    int m_icarplaymode;
    int m_imodestatus;
    int m_iSpecialMode;
    int m_ilastmode;
    int m_ipreviousMode;
    int m_iusbpreviousMode;//14188	CM7C-2900
    int m_idiagnose_status;
    int m_navispeakflag;
    int m_touch_enable;
    int m_hardkey;
    int m_firstshowflag;
    int m_dvrons_time; //CM7C-2600
    int m_hk_dispflag;
    bool m_IsFirstPowerOn;
    bool m_bnaviisshow;
    bool m_bcarplayisshow;
    bool m_blinkshow;
    bool m_bcastshow;
    enum HMI_ScreenType m_enscreentype;
    bool m_phonehandfree;
    int m_isourcemode;
    int m_irequestmode;
	int m_iCarPlayOnOff;
    int m_i360View;
    int m_iVehicleModule;
    int m_avm_rvc_pdc_type;
    int m_disp_parkview_sts;
    int m_todstatus;
    int m_ivolummode;
    int m_ipmstate;
    int m_ihmishowflag;
//	int m_iOpm_status;
    int m_iOpm_authority;
	bool m_isnaviok;
    int  m_timerid;
    void timerEvent(QTimerEvent* event);

    int m_videopaused;
    int m_mode_src;
    int m_carspeed;
    int m_navivolbarflag;
    KeyToneClass *p_KeyToneClass;
    QThread *p_KeyToneClass_workThread;
    int m_beforevrsource;
    int m_premode;
    int m_beforeusbvsource;
    int m_avmrvcdvr_flag;
    int m_KeyTouchTimer;
    int m_iUsbVideoswitchOnOff; //wangtingting add for sx5-477
signals:
    void sigkeytone();

    friend class KeyToneClass;
};
/****************Class Declaration Section End*********************************************/

/****************Function Prototype Declaration Section Begin******************************/
void hmi_mode_obtain(HProxy proxy, enum SX5Mode mode);
void hmi_signal_status_change(HProxy proxy, enum SX5Mode mode, enum ModeStatus status);
void hmi_signal_video_pause(HProxy proxy, enum SX5Mode mode);
void hmi_signal_mode_resume(HProxy proxy, enum SX5Mode mode);
void hmi_signal_video_forbid(HProxy proxy);
void hmi_signal_diagnose_in(HProxy proxy);
void hmi_signal_diagnose_out(HProxy proxy);
void hmi_signal_already_focused(HProxy proxy, enum SX5Mode mode);
void hmi_signal_pdc_rvc_avm_show(HProxy proxy, unsigned int type);
//void hmi_signal_pm_state_changed(HProxy proxy, enum PowerMode state);
void hmi_signal_screen_showing(HProxy proxy, unsigned int type);
void hmi_get_opm_authority(HProxy proxy, unsigned int authority);
void hmi_signal_opm_authority_changed(HProxy proxy, unsigned int authority);
void hmi_signal_navigation_ready(HProxy proxy);
void hmi_signal_hide_volume_bar(HProxy proxy);
void hmi_signal_show_colorbar(HProxy proxy, unsigned int side, unsigned int show);
//void hmi_signal_only_update_parkview(HProxy proxy,  unsigned int state);
//void hmi_signal_ign_state_changed(HProxy proxy, enum IgnState state);
//void hmi_signal_opm_state_changed(HProxy proxy, unsigned int state);
void hmi_signal_car_state_change(HProxy proxy, unsigned int state);
/****************Function Prototype Declaration Section End********************************/
#endif // MODEMANAGER_H
