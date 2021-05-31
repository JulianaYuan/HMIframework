/***************************************************************************************
 File Name   : hmiapi_modemanager.cpp

 Created by  :  Dong.Sen(Neusoft)

 Created Date:  2015/07/15

 Description   :   Modemanager Hmiapi
***************************************************************************************/

/****************Include  Section Begin************************************************/
#include <cstring>
#include <stdlib.h>
#include "hmiapi_modemanager.h"
#include "hmifrm_funcevent.h"
#include "hmifrm_keyclass.h"
/****************Include  Section  End*************************************************/
#ifndef HMI_API_STUB
HProxy PROXY_MODE;
#endif //HMI_API_STUB
IMPLEMENT_SINGLETON(ModemanagerClass)

ModemanagerClass::ModemanagerClass()
{
    //juanjuan add start
    m_baseSrcMode.mode = SX5_MODE_MAX;
    m_baseSrcMode.status = MODE_STATUS_UNVALIABLE;
    m_intSrcMode.mode = SX5_MODE_MAX;
    m_intSrcMode.status = MODE_STATUS_UNVALIABLE;
    m_curSrcMode.mode = SX5_MODE_MAX;
    m_curSrcMode.status = MODE_STATUS_UNVALIABLE;
    m_preSrcMode.mode = SX5_MODE_SRC_TUNER;
    m_preSrcMode.status = MODE_STATUS_UNVALIABLE;
    m_reqBaseSrcMode = SX5_MODE_MAX;
    //juanjuan add end
    m_icurmode = -1;
    m_icarplaymode = -1;
    m_imodestatus = -1;
    m_iSpecialMode = HMI_MODE_SPECIALMODE_NONE;
    m_ilastmode = -1;
    m_ipreviousMode = 0;
    m_iusbpreviousMode = 0; //14188	CM7C-2900
    m_idiagnose_status = HMI_MODE_DIAGNOSE_STATUS_OFF;
    m_navispeakflag = HMI_MODE_NAVISPEAK_STATUS_OFF;
    m_IsFirstPowerOn = true;
    m_firstshowflag = 0;
    m_dvrons_time = 5000; //CM7C-2600
    m_hk_dispflag = 0;
#if SWITCH_DEBUG
    m_touch_enable = 0;
    m_hardkey = 0;
#else	
	m_touch_enable = 1;
    m_hardkey = 1;
#endif	
    m_enscreentype = E_SCREEN_SOURCE;
    m_bnaviisshow = false;
    m_bcarplayisshow = false;
    m_phonehandfree = false;
    m_isourcemode = 0;
    m_irequestmode = -1;
    m_iCarPlayOnOff = 0;//Off
    m_i360View = 0;//off
    m_iVehicleModule = 0;//SX5
	m_blinkshow = false;
    m_bcastshow = false;
    m_todstatus = 0;
    m_ivolummode = 0;
    m_ipmstate = 0;
    m_ihmishowflag = 0;
//	m_iOpm_status = 0; //0: OFF   1: ON
    m_iOpm_authority = 0; //0:Not Allowed   1:Allowed
	m_isnaviok = false;
    m_timerid = 0;
    m_videopaused = 0;
    HMI_LOG("14504 ModemanagerClass::ModemanagerClass() m_videopaused = 0");
    m_mode_src = -1;
    m_avm_rvc_pdc_type = 0;
    m_carspeed = 0;
	m_navivolbarflag = 0;
    m_premode = 0;
    m_beforeusbvsource = 0;
    m_avmrvcdvr_flag = 0;
    //16227	CM7C-3965
    p_KeyToneClass = new KeyToneClass(this);
    if (NULL == p_KeyToneClass)
    {
        HMI_ERROR("p_KeyToneClass malloc error");
    }

    p_KeyToneClass_workThread = new QThread();

    p_KeyToneClass->moveToThread(p_KeyToneClass_workThread);
    p_KeyToneClass_workThread->start();
    QObject::connect(this,SIGNAL(sigkeytone()),p_KeyToneClass,SLOT(req_keytone()),Qt::QueuedConnection);
    m_KeyTouchTimer = 0;//set 500ms timer to disable key and touch for a while,when timer triger,enable this
    m_iUsbVideoswitchOnOff = 0;
}

ModemanagerClass::~ModemanagerClass()
{
    if (NULL == p_KeyToneClass)
    {
        delete p_KeyToneClass;
        p_KeyToneClass = NULL;
    }

    if (NULL == p_KeyToneClass_workThread)
    {
        p_KeyToneClass_workThread->exit();
        delete p_KeyToneClass_workThread;
        p_KeyToneClass_workThread = NULL;
    }
}

void init_client_sx5_mode(tInterfaceClient_sx5_modemanager *client)
{
    // init the callbacks
/*
    client->on_mode_obtain = hmi_mode_obtain;
    client->on_signal_status_change = hmi_signal_status_change;
    client->on_signal_video_pause = hmi_signal_video_pause;
    client->on_signal_video_resume = hmi_signal_mode_resume;
    client->on_signal_video_forbid = hmi_signal_video_forbid;
    client->on_signal_diagnose_in = hmi_signal_diagnose_in;
    client->on_signal_diagnose_out = hmi_signal_diagnose_out;
    client->on_signal_already_focused = hmi_signal_already_focused;
    client->on_signal_pdc_rvc_avm_show = hmi_signal_pdc_rvc_avm_show;
    client->on_signal_pm_state_changed = hmi_signal_pm_state_changed;
    client->on_signal_screen_showing = hmi_signal_screen_showing;
    client->on_get_opm_authority = hmi_get_opm_authority;
    client->on_signal_opm_authority_changed = hmi_signal_opm_authority_changed;
	client->on_signal_navigation_ready = hmi_signal_navigation_ready;
    client->on_signal_hide_volume_bar = hmi_signal_hide_volume_bar;
    client->on_signal_show_colorbar = hmi_signal_show_colorbar;
  //  client->on_signal_only_update_parkview = hmi_signal_only_update_parkview;
    client->on_signal_ign_state_changed = hmi_signal_ign_state_changed;
//    client->on_signal_opm_state_changed = hmi_signal_opm_state_changed;
    client->on_signal_car_state_change = hmi_signal_car_state_change;
    //...
    */
}



void ModemanagerClass::Modemanager_Init(QObject *parent)
{
    if(NULL == parent)
    {
        HMI_WARNING("NULL == parent");
    }
    
    frameWorkClass = dynamic_cast<Frm_CommonClass*>(parent);

#ifdef HMI_API_STUB
    tInterfaceClient_sx5_modemanager* pclient;
   // pclient = sx5_mode_init();
   // init_client_sx5_mode(pclient);
#else //HMI_API_STUB
    proxy_MODEMANAGER.connection = mBusConnection;
    proxy_MODEMANAGER.path = "/sx5/modemanager";		//entertainment path
    proxy_MODEMANAGER.service = "sx5.modemanager";    // entertainment name
    PROXY_MODE = &proxy_MODEMANAGER;

    // init the entertainment manager dbus API proxy
    init_client_sx5_mode(&INTERFACE_CLIENT_sx5_modemanager);
    KeyCue_execInitProc(EKEYCUE_REQ_PLAY_BLOCK);
#endif //HMI_API_STUB
    return;
}

void ModemanagerClass::on_mode_obtain(HProxy proxy, enum SX5Mode mode)
{
    //if(NULL == proxy)
    {
        HMI_WARNING("NULL == proxy");
    }
    HMI_LOG("mode = %d", mode);
    //on_signal_srcmode_change(proxy,mode,MODE_STATUS_FOCUSED);
    m_icurmode = chgModeToHMI(mode);
    sendEvent(D_HMIAPI_EV_MODE_OBTAIN_GETTED);

    return;
}
//juanjuan add start 
void ModemanagerClass::on_signal_srcmode_change(HProxy proxy, enum SX5Mode mode, enum ModeStatus status)
{
    //if(NULL == proxy)
    {
        HMI_LOG("NULL == proxy");
    }
    HMI_LOG("mode = %d, status = %d", mode, status);
    if(m_baseSrcMode.mode == mode)m_baseSrcMode.status = status;
    if(m_intSrcMode.mode == mode)m_intSrcMode.status = status;
    if(m_preSrcMode.mode == mode)m_preSrcMode.status = status;

    HMI_LOG("m_baseSrcmode = %d, status = %d", m_baseSrcMode.mode, m_baseSrcMode.status);
    HMI_LOG("m_intSrcmode = %d, status = %d", m_intSrcMode.mode, m_intSrcMode.status);
    HMI_LOG("m_preSrcmode = %d, status = %d", m_preSrcMode.mode, m_preSrcMode.status);
    HMI_LOG("m_curSrcmode = %d, status = %d", m_curSrcMode.mode, m_curSrcMode.status);
}
//juanjuan add end
void ModemanagerClass::on_signal_status_change(HProxy proxy, enum SX5Mode mode, enum ModeStatus status)
{

    sendEvent(D_HMIAPI_EV_MODE_STATUS_CHANGED);
    return;
}

void ModemanagerClass::on_signal_video_pause(HProxy proxy, enum SX5Mode mode)
{

    HMI_LOG("mode = %d", mode);

    HMI_LOG("14504 ModemanagerClass::on_signal_video_pause() m_videopaused = 1");
    m_videopaused = 1;
    sendEvent(D_HMIAPI_EV_MODE_VIDEO_PAUSED);
    return;
}

void ModemanagerClass::on_signal_mode_resume(HProxy proxy, enum SX5Mode mode)
{

    HMI_LOG("mode = %d", mode);

    HMI_LOG("14504 ModemanagerClass::on_signal_mode_resume() m_videopaused = 0");
    m_videopaused = 0;
    sendEvent(D_HMIAPI_EV_MODE_VIDEO_RESUME);

    return;
}

void ModemanagerClass::on_signal_video_forbid(HProxy proxy)
{
    if(NULL == proxy)
    {
        HMI_WARNING("NULL == proxy");
    }

    HMI_LOG("ENTER");

    if(E_MODE_SRC_USB_VIDEO==Get_RequestMode())//修复SX5-6142 HK_DISP key 不能用的问题
	{
    	Set_RequestMode(-1);
	}
    if(SX5_MODE_SRC_USB_VIDEO == m_reqBaseSrcMode)
    {
        m_reqBaseSrcMode = m_baseSrcMode.mode;
    }
    sendEvent(D_HMIAPI_EV_MODE_VIEDIO_FORBID);
}

void ModemanagerClass::on_signal_pdc_rvc_avm_show(HProxy proxy, unsigned int type)
{
    if( NULL == proxy )
    {
        HMI_LOG("NULL == proxy");
        return;
    }

    HMI_LOG("type = %d", type);
    if(0 == type)
    {
        sendEvent(D_HMIAPI_EV_MODE_EXIT_AVM_RVC_DVR);
    }
    else if( 1 == type )
    {
        sendEvent(D_HMIAPI_EV_MODE_SHOW_AVM);
    }
    else if( 2 == type )
    {
        sendEvent(D_HMIAPI_EV_MODE_SHOW_RVC);
    }
    else if( 3 == type )
    {
        sendEvent(D_HMIAPI_EV_MODE_SHOW_DVR);
    }
    else
    {
        //do nothing
    }

    m_avm_rvc_pdc_type = type;

    return;
}
//SX5-6832
void ModemanagerClass::Set_avm_type(int var)
{
    HMI_LOG("IN");
    m_avm_rvc_pdc_type = var;
    return;
}

void ModemanagerClass::on_signal_navigation_ready(HProxy proxy)
{
    UNUSED_PARAMETER(proxy);
    HMI_LOG("IN");
    m_isnaviok = true;
    sendEvent(D_HMIAPI_EV_MODE_NAVI_IS_READY);
}

void ModemanagerClass::on_signal_hide_volume_bar(HProxy proxy)
{
    UNUSED_PARAMETER(proxy);
    HMI_LOG("IN");
    sendEvent(D_HMIAPI_EV_MODE_NAVI_VOLBAR_HIDE);
}
void ModemanagerClass::on_signal_show_colorbar(HProxy proxy, unsigned int side, unsigned int show)
{
    if( NULL == proxy )
    {
        HMI_LOG("NULL == proxy");
        return;
    }
    HMI_LOG("side = %d, show = %d", side, show);

    //Side      0 front        1 rear
    //Show      0 not show     1 show

    if ( 0 == side && 0 == show )
    {
        sendEvent(D_HMIAPI_EV_MODE_COLORBARHIDE);
    }
    else if ( 0 == side && 1 == show )
    {
        sendEvent(D_HMIAPI_EV_MODE_COLORBARSHOW);
    }
    else
    {
        //donothing
    }

    return;
}

void ModemanagerClass::on_signal_screen_showing(HProxy proxy, unsigned int type)
{
    if( NULL == proxy )
    {
        HMI_LOG("NULL == proxy");
        return;
    }

    return;
}

void ModemanagerClass::on_get_opm_authority(HProxy proxy, unsigned int authority)
{
    if( NULL == proxy )
    {
        HMI_LOG("NULL == proxy");
        return;
    }

    HMI_LOG("authority = %d", authority);

    m_iOpm_authority = authority;

    sendEvent(D_HMIAPI_EV_MODE_OPM_AUTHORITY);
}


void ModemanagerClass::on_signal_car_state_change(HProxy proxy, unsigned int state)
{
    UNUSED_PARAMETER(proxy);
    HMI_LOG("state = %d", state);
    m_carspeed = state;

    sendEvent(D_HMIAPI_EV_MODE_CARSPEEDCHANG);
}

int ModemanagerClass::Get_ign_status()
{
    return 0;
}

int ModemanagerClass::Get_SrcMode()
{
    HMI_LOG("m_mode_src = %d", m_mode_src);
    return m_mode_src;
}

int ModemanagerClass::Get_opm_authority()
{
    HMI_LOG("m_iOpm_authority = %d", m_iOpm_authority);
    return m_iOpm_authority;
}


int ModemanagerClass::Get_avm_rvc_pdc()
{
    HMI_LOG("m_avm_rvc_pdc_type = %d", m_avm_rvc_pdc_type);
    return m_avm_rvc_pdc_type;
}

int ModemanagerClass::Get_Todstatus()
{
    HMI_LOG("m_todstatus = %d", m_todstatus);
    return m_todstatus;
}

int ModemanagerClass::Get_volummode()
{
	HMI_LOG("m_ivolummode = %d", m_ivolummode);
    return m_ivolummode;
}

void ModemanagerClass::Set_volummode(int mode)
{
    HMI_LOG("mode = %d", mode);
    m_ivolummode = mode;
    return;
}

int  ModemanagerClass::Get_CurrentMode()
{
    HMI_LOG("m_icurmode = %d", m_icurmode);
    return m_icurmode;
}

int  ModemanagerClass::Get_CarplayMode()
{
    HMI_LOG("m_icarplaymode = %d", m_icarplaymode);
    return m_icarplaymode;
}

int  ModemanagerClass::Get_LastMode()
{
    return m_ilastmode;
}

int  ModemanagerClass::Get_PreviousMode()
{
    HMI_LOG("m_ipreviousMode = %d", m_ipreviousMode);
    return m_ipreviousMode;
}
//14188	CM7C-2900
int  ModemanagerClass::Get_UsbPreviousMode()
{
    HMI_LOG("m_iusbpreviousMode = %d", m_ipreviousMode);
    return m_iusbpreviousMode;
}

void  ModemanagerClass::Set_PreviousMode(int ipremode)
{
    HMI_LOG("ipremode = %d", ipremode);
    m_ipreviousMode = ipremode;
    //14188	CM7C-2900
    if (( E_MODE_SRC_USB_AUDIO != m_ipreviousMode) && ( E_MODE_SRC_USB_VIDEO != m_ipreviousMode))
    {
        m_iusbpreviousMode = m_ipreviousMode;
    }
}

enum HMI_ScreenType ModemanagerClass::Get_ScreenType()
{
    int type = 0;
    return m_enscreentype;
}

void ModemanagerClass::Ope_save_last_screent(enum HMI_ScreenType type)
{
    HMI_LOG("type = %d", type);
    HMI_LOG("m_ihmishowflag = %d", m_ihmishowflag);
    return;
}

int  ModemanagerClass::Get_NavispekStatus()
{
    HMI_LOG("m_navispeakflag = %d", m_navispeakflag);
    return m_navispeakflag;
}

int  ModemanagerClass::Get_panel_enable()
{
    return m_touch_enable;
}

int  ModemanagerClass::Get_hardkey_enable()
{
    HMI_LOG("m_hardkey=%d m_ihmishowflag=%d",m_hardkey,m_ihmishowflag);

    return m_hardkey;
}

void ModemanagerClass::Set_hardkey_enable(int enable)
{
    HMI_LOG("enable = %d", enable);
    m_hardkey = enable;
    return;
}
//16015	CM7C-3715
void ModemanagerClass::Set_mode_src(enum HMI_Mode mode)
{
    m_mode_src = mode;
    return;
}
void ModemanagerClass::Req_ModeRequest(enum HMI_Mode request)
{
    HMI_LOG("request = %d", request);


    enum SX5Mode enrequest = SX5_MODE_SRC_TUNER;
    int iPreviousMode_changeed = 0;
    m_irequestmode = request;
   return;
}

void ModemanagerClass::Req_ModeRelease(enum HMI_Mode request)
{
    HMI_LOG("request = %d", request);
  return;
}

void ModemanagerClass::Req_ModeObtain()
{
    HMI_LOG("ENTER");

    return;
}
void ModemanagerClass::Req_disp_key_avm_rvc(int i_type,int i_state)
{
    HMI_LOG("type = %d,i_state=%d",i_type,i_state);
    return;
}
void ModemanagerClass::Req_call_hmi_is_ready()
{
    HMI_LOG("ENTER");

    return;
}

void ModemanagerClass::Ope_ctrlvolbar(int i_ope)
{
    HMI_LOG("i_ope=%d",i_ope);

    return;
}

int ModemanagerClass::Get_PmState()
{
    return m_ipmstate;
}

int ModemanagerClass::Get_hmiShow()
{
    HMI_LOG("m_ihmishowflag = %d", m_ihmishowflag);
    return m_ihmishowflag;
}

int ModemanagerClass::get_hmi_is_ready()
{
    return m_firstshowflag;
}
int ModemanagerClass::get_dvrons_time() //CM7C-2600
{
    return m_dvrons_time;
}
void ModemanagerClass::Set_dvrons_time(int time) //CM7C-2600
{
    m_dvrons_time = time;
    return;
}
int ModemanagerClass::Get_RequestMode()
{
    HMI_LOG("m_irequestmode = %d", m_irequestmode);
    return m_irequestmode;
}
void ModemanagerClass::Set_RequestMode(int i_var)
{
    m_irequestmode = i_var;
}

bool ModemanagerClass::is_linkshow()
{
    return m_blinkshow;
}

bool ModemanagerClass::is_castshow()
{
    return m_bcastshow;
}

void ModemanagerClass::send_lastscreen_save()
{
    if(0 != m_firstshowflag)
    {
        sendEvent(D_HMIAPI_EV_MODE_SAVE_EVLASTSCREEN);
    }

    return;
}

void ModemanagerClass::Ope_ShowlayerManager(int modeID,int screenId)
{
}
//wangtingting merge on 0425 end

void ModemanagerClass::Ope_Shownavi()
{
#if 0
    HMI_LOG("ENTER");
    m_iSpecialMode = HMI_MODE_SPECIALMODE_NAVI;
    m_bnaviisshow = true;
    sx5_modemanager_call_show_navi(PROXY_MODE);
    return;
#endif
}

void ModemanagerClass::Ope_Hidenavi()
{
#if 0
    HMI_LOG("ENTER");
    m_iSpecialMode = HMI_MODE_SPECIALMODE_NONE;
    m_bnaviisshow = false;
    sx5_modemanager_call_hide_navi(PROXY_MODE);
    return;
#endif 
}

void ModemanagerClass::Ope_Showcarplay()
{
#if 0
    HMI_LOG("ENTER");
    m_iSpecialMode = HMI_MODE_SPECIALMODE_CARPLAY;
    m_bcarplayisshow = true;
    sx5_modemanager_call_show_carplay(PROXY_MODE);
    return;
#endif
}

void ModemanagerClass::Ope_Hidecarplay()
{
#if 0
    HMI_LOG("ENTER");
    m_iSpecialMode = HMI_MODE_SPECIALMODE_NONE;
    m_bcarplayisshow = false;
    sx5_modemanager_call_hide_carplay(PROXY_MODE);
    return;
#endif
}

void ModemanagerClass::Ope_ShowMirrorLink()
{
#if 0
    HMI_LOG("ENTER");;
    m_iSpecialMode = HMI_MODE_SPECIALMODE_MIRRORLINK;
    m_blinkshow = true;
    sx5_modemanager_call_show_mirrorlink(PROXY_MODE);
    return;
#endif
}
void ModemanagerClass::Ope_HideMirrorLink()
{
#if 0
    HMI_LOG("ENTER");;
    m_iSpecialMode = HMI_MODE_SPECIALMODE_NONE;
    m_blinkshow = false;
    sx5_modemanager_call_hide_mirrorlink(PROXY_MODE);
    return;
#endif
}
void ModemanagerClass::Ope_ShowMiraCast()
{
#if 0
    HMI_LOG("ENTER");;
    m_iSpecialMode = HMI_MODE_SPECIALMODE_MIRACAST;
    m_bcastshow = true;
    sx5_modemanager_call_show_miracast(PROXY_MODE);
    return;
#endif
}
void ModemanagerClass::Ope_HideMiraCast()
{
#if 0
    HMI_LOG("ENTER");;
    m_iSpecialMode = HMI_MODE_SPECIALMODE_NONE;
    m_bcastshow = false;
    sx5_modemanager_call_hide_miracast(PROXY_MODE);
    return;
#endif
}

void ModemanagerClass::Ope_Showusbv()
{
#if 0
    HMI_LOG("Enter");
    sx5_modemanager_call_show_usb_video(PROXY_MODE);
    return;
#endif
}

void ModemanagerClass::Ope_Hideusbv()
{
#if 0
    HMI_LOG("Enter");
    sx5_modemanager_call_hide_usb_video(PROXY_MODE);
    return;
#endif
}

void ModemanagerClass::Ope_DVR_OnOff()
{
    HMI_LOG("Enter");
   return;
}

void ModemanagerClass::Ope_play_key_sound()
{
    emit sigkeytone();

    return;
}

void ModemanagerClass::Req_GetPmState()
{
    HMI_LOG("IN");
    return;
}

int ModemanagerClass::Get_hk_dispflag()
{
    HMI_LOG("IN m_hk_dispflag=%d",m_hk_dispflag);
    return m_hk_dispflag;
}
void ModemanagerClass::Set_hk_dispflag(int i_flag)
{
    HMI_LOG("IN i_flag=%d",i_flag);
    m_hk_dispflag = i_flag;
    return;
}

void ModemanagerClass::Ope_PowerPress(int i_flag)
{
    HMI_LOG("i_flag=%d",i_flag);

   return;
}

void ModemanagerClass::Ope_SoftUpgrade()
{
    HMI_LOG("IN");
    return;
}

int ModemanagerClass::Get_SpecialMode()
{
    if (E_MODE_INT_PHONE == m_icurmode)
    {
        return HMI_MODE_SPECIALMODE_PHONE;
    }

    return m_iSpecialMode;
}

int ModemanagerClass::chgModeToHMI(enum SX5Mode mode)
{
    int itempmode = m_icurmode;

    switch(mode)
    {
    case SX5_MODE_SRC_TUNER:
        itempmode = E_MODE_SRC_TUNER;
        break;
    case SX5_MODE_SRC_USB_AUDIO:
        itempmode = E_MODE_SRC_USB_AUDIO;
        break;
    case SX5_MODE_SRC_USB_VIDEO:
        itempmode = E_MODE_SRC_USB_VIDEO;
        break;
    case SX5_MODE_SRC_HDD_AUDIO:
        itempmode = E_MODE_SRC_HDD_AUDIO;
        break;
    case SX5_MODE_SRC_HDD_VIDEO:
        itempmode = E_MODE_SRC_HDD_VIDEO;
        break;
    case SX5_MODE_SRC_IPOD:
        itempmode = E_MODE_SRC_IPOD;
        break;
    case SX5_MODE_SRC_AUX:
        itempmode = E_MODE_SRC_AUX;
        break;
    case SX5_MODE_SRC_BTAUDIO:
        itempmode = E_MODE_SRC_BTAUDIO;
        break;
    case SX5_MODE_SRC_MIRRORLINK:
        itempmode = E_MODE_SRC_MIRRORLINK;
        break;
    case SX5_MODE_SRC_MIRACAST:
        itempmode = E_MODE_SRC_MIRACAST;
        break;
    case SX5_MODE_SRC_CARPLAY:
        itempmode = E_MODE_SRC_CARPLAY;
        break;
    case SX5_MODE_INT_PHONE:
        itempmode = E_MODE_INT_PHONE;
        break;
    case SX5_MODE_INT_RINGTONE:
        itempmode = E_MODE_INT_RINGTONE;
        break;
    case SX5_MODE_INT_ECALL:
        itempmode = E_MODE_INT_ECALL;
        break;
    case SX5_MODE_INT_VR:
        itempmode = E_MODE_INT_VR;
        break;
    case SX5_MODE_MIX_NAVI:
        itempmode = E_MODE_MIX_NAVI;
        break;
    case SX5_MODE_MIX_CARPLAY_ALT:
        itempmode = E_MODE_MIX_CARPLAY_ALT;
        break;
    default:
        break;
    }

    return itempmode;
}

void ModemanagerClass::concreteEvent(QEvent *e)
{
    //appClass->customEvent(e);

    if (NULL == e)
    {
        HMI_WARNING("NULL == e");
    }

}

void ModemanagerClass::sendEvent(int eventID)
{
    Frm_FuncEvent* FuncEvent = new Frm_FuncEvent((static_cast<QEvent::Type>(Frm_AbstractEvent::EN_EV_APPTOHMI)), eventID);

    QGuiApplication::postEvent(frameWorkClass, static_cast<QEvent*> (FuncEvent));

    return;
}

void ModemanagerClass::sendToFrmModeManager(int i_eventID)
{
    Frm_FuncEvent* FuncEvent = new Frm_FuncEvent((static_cast<QEvent::Type>(Frm_AbstractEvent::EN_EV_AUDIOMODE)), i_eventID);

    QGuiApplication::postEvent(frameWorkClass, static_cast<QEvent*> (FuncEvent));

    return;
}


void ModemanagerClass::set_diagnose_status(int i_var)
{
    m_idiagnose_status = i_var;
    if( HMI_MODE_DIAGNOSE_STATUS_ON == m_idiagnose_status )
    {
        sendEvent(D_HMIAPI_EV_MODE_DIAGNOSE_IN);
    }
    else
    {
        sendEvent(D_HMIAPI_EV_MODE_DIAGNOSE_OUT);
    }
    return;
}

int  ModemanagerClass::get_diagnose_status()
{
    return m_idiagnose_status;
}

bool ModemanagerClass::is_AVM_available()
{
    return true; // dummy
}

bool ModemanagerClass::is_Electricvehicle()
{
    //return true; // dummy
    return false; // dummy
}

bool ModemanagerClass::is_TBox_available()
{
    //return true; // dummy
    return false; // dummy
}

void hmi_mode_obtain(HProxy proxy, enum SX5Mode mode)
{
    ModemanagerClassIns->on_mode_obtain(proxy, mode);
    return;
}

void hmi_signal_status_change(HProxy proxy, enum SX5Mode mode, enum ModeStatus status)
{
    ModemanagerClassIns->on_signal_status_change(proxy, mode, status);
    return;
}

void hmi_signal_video_pause(HProxy proxy, enum SX5Mode mode)
{
    ModemanagerClassIns->on_signal_video_pause(proxy, mode);
    return;
}

void hmi_signal_mode_resume(HProxy proxy, enum SX5Mode mode)
{
    ModemanagerClassIns->on_signal_mode_resume(proxy, mode);
    return;
}

void hmi_signal_video_forbid(HProxy proxy)
{
    ModemanagerClassIns->on_signal_video_forbid(proxy);
    return;
}

void hmi_signal_diagnose_in(HProxy proxy)
{
	HMI_TRACE_IN;
    if(NULL == proxy)
    {
        HMI_WARNING("NULL == proxy");
    }

    ModemanagerClassIns->set_diagnose_status(HMI_MODE_DIAGNOSE_STATUS_ON);
    return;
}
void hmi_signal_diagnose_out(HProxy proxy)
{
	HMI_TRACE_IN;
    if(NULL == proxy)
    {
        HMI_WARNING("NULL == proxy");
    }
    ModemanagerClassIns->set_diagnose_status(HMI_MODE_DIAGNOSE_STATUS_OFF);
    return;
}

void hmi_signal_already_focused(HProxy proxy, enum SX5Mode mode)
{
    //ModemanagerClassIns->on_signal_status_change(proxy, mode, MODE_STATUS_FOCUSED);
}


void hmi_signal_pdc_rvc_avm_show(HProxy proxy, unsigned int type)
{
    ModemanagerClassIns->on_signal_pdc_rvc_avm_show(proxy, type);
}

/*void hmi_signal_pm_state_changed(HProxy proxy, enum PowerMode state)
{
    //ModemanagerClassIns->on_signal_pm_state_changed(proxy, state);
}*/

void hmi_signal_screen_showing(HProxy proxy, unsigned int type)
{
    ModemanagerClassIns->on_signal_screen_showing(proxy, type);
}

void hmi_get_opm_authority(HProxy proxy, unsigned int authority)
{
    ModemanagerClassIns->on_get_opm_authority(proxy, authority);
}

void hmi_signal_opm_authority_changed(HProxy proxy, unsigned int authority)
{
    ModemanagerClassIns->on_get_opm_authority(proxy, authority);
}
void hmi_signal_navigation_ready(HProxy proxy)
{
    ModemanagerClassIns->on_signal_navigation_ready(proxy);
}

void hmi_signal_hide_volume_bar(HProxy proxy)
{
    ModemanagerClassIns->on_signal_hide_volume_bar(proxy);
    return;

}

void hmi_signal_show_colorbar(HProxy proxy, unsigned int side, unsigned int show)
{
    ModemanagerClassIns->on_signal_show_colorbar(proxy, side, show);
    return;
}
/*
void hmi_signal_ign_state_changed(HProxy proxy, enum IgnState state)
{
   // ModemanagerClassIns->on_signal_ign_state_changed(proxy, state);
}*/

void hmi_signal_car_state_change(HProxy proxy, unsigned int state)
{
    ModemanagerClassIns->on_signal_car_state_change(proxy, state);
}

void ModemanagerClass::set_isFirstTimePowerOn_Status(bool bIsFirstPowerOnFlag)
{
    m_IsFirstPowerOn = bIsFirstPowerOnFlag;
}

bool ModemanagerClass::get_is_firstTimePowerOn()
{
    return m_IsFirstPowerOn;
}

bool ModemanagerClass::is_navishow()
{
    HMI_LOG("m_bnaviisshow = %d", m_bnaviisshow);
    return m_bnaviisshow;
}

bool ModemanagerClass::is_carplayshow()
{
    HMI_LOG("m_bcarplayisshow = %d", m_bcarplayisshow);
    return m_bcarplayisshow;
}
void ModemanagerClass::set_carPlayOnOff(int iCarPlayOnOff)
{
    HMI_LOG("Set iCarPlayOnOff = %d", iCarPlayOnOff);
    sendEvent(D_HMIAPI_EV_MODE_CARPLAYCHANGE);
    return;
}

int ModemanagerClass::get_carPlayOnOff()
{
    uint8_t icarPlaysts = 0;

    if(m_bCarplaySwitch)
    {
       if(icarPlaysts != 1)
        {
           icarPlaysts = 0;
        }
    }
    m_iCarPlayOnOff = icarPlaysts;
    HMI_LOG("Get m_iCarPlayOnOff = %d", m_iCarPlayOnOff);

    return m_iCarPlayOnOff;
}

int ModemanagerClass::Get_SourceMode()
{
    HMI_LOG("m_isourcemode=%d",m_isourcemode);
    return m_isourcemode;
}

bool ModemanagerClass::Get_isnaviready()
{
    HMI_LOG("m_isnaviok=%d",m_isnaviok);
    return m_isnaviok;
}
void ModemanagerClass::timerEvent(QTimerEvent* event)
{
    HMI_LOG("IN");

    if(event->timerId() == m_timerid)
    {
        if( 0 == m_firstshowflag )
        {
            killTimer(m_timerid);
            m_timerid = 0;
            m_firstshowflag = 1;
             HMI_LOG("IN call hmiisready");
            sendEvent(D_HMIAPI_EV_MODE_HMIISREADY);
        }
    }
    else if(m_KeyTouchTimer == event->timerId())
    {
        HMI_LOG("m_KeyTouchTimer triger timerEvent");
        killTimer(m_KeyTouchTimer);
        m_KeyTouchTimer = 0;
        m_hardkey = 1;
        m_touch_enable = 1;
    }
    return;
}

int ModemanagerClass::Get_VideoPaused()
{
    HMI_LOG("14504 Get_VideoPaused m_videopaused = %d", m_videopaused);
    return m_videopaused;
}

void ModemanagerClass::Set_VideoPaused()
{
    HMI_LOG("14504 Set_VideoPaused m_videopaused = 0");
    m_videopaused = 0;

    return;
}
KeyToneClass::KeyToneClass(QObject *parent)
{
    UNUSED_PARAMETER(parent);
}

KeyToneClass::~KeyToneClass()
{

}

void KeyToneClass::req_keytone()
{
        HMI_LOG("IN");
        return;
}
int ModemanagerClass::Get_befoerVRsource()
{
    HMI_LOG("m_beforevrsource = %d", m_beforevrsource);
    return m_beforevrsource;
}

int ModemanagerClass::Get_Carspeed()
{
    HMI_LOG("m_carspeed = %d", m_carspeed);
    return m_carspeed;
}

int ModemanagerClass::Get_preMode()
{
    HMI_LOG("m_premode = %d", m_premode);
    return m_premode;
}

int ModemanagerClass::Get_beforeUsbvSource()
{
    HMI_LOG("m_beforeusbvsource = %d", m_beforeusbvsource);
    return m_beforeusbvsource;
}

void ModemanagerClass::Set_quitAVMRVCDVR_flag()
{
    HMI_LOG("1m_avmrvcdvr_flag=%d",m_avmrvcdvr_flag );
    m_avmrvcdvr_flag = 1;
    HMI_LOG("2m_avmrvcdvr_flag=%d",m_avmrvcdvr_flag );
    return;
}

void ModemanagerClass::Req_transferred_over()
{
    HMI_LOG("IN m_avmrvcdvr_flag=%d",m_avmrvcdvr_flag );
    if( 1 == m_avmrvcdvr_flag )
    {
        m_avmrvcdvr_flag = 0;
    }

    return;
}

//juanjuan add to check if the source is available,the return value is the available source or default source tuner
int ModemanagerClass::return_available_Source(int mode)
{
    bool iret = true;
    int i_caps = 0;
    return iret;
}

void ModemanagerClass::startKeyTouchTimer()
{
    if(m_KeyTouchTimer == 0)
    {
        HMI_LOG("startKeyTouchTimer");
        m_KeyTouchTimer = startTimer(1000);
        m_hardkey = 0;
        m_touch_enable = 0;
    }
}
void ModemanagerClass::killKeyTouchTimer()
{
    if(0 != m_KeyTouchTimer)
    {
        HMI_LOG("killKeyTouchTimer");
        killTimer(m_KeyTouchTimer);
        m_KeyTouchTimer = 0;
        m_hardkey = 1;
        m_touch_enable = 1;
    }
}

void ModemanagerClass::Ope_ShowWelink()
{
#if 0
    HMI_LOG("ENTER");
    sx5_modemanager_call_show_welink(PROXY_MODE);
    return;
#endif
}
void ModemanagerClass::Ope_HideWelink()
{
#if 0
    HMI_LOG("ENTER");
    sx5_modemanager_call_hide_welink(PROXY_MODE);
    return;
#endif
}
//wangtingting add for SX5-477 start
void ModemanagerClass::set_usbVideoswitchOnOff(int iUsbVideoswitchOnOff)
{
    HMI_LOG("Set iUsbVideoswitchOnOff = %d", iUsbVideoswitchOnOff);
    sendEvent(D_HMIAPI_EV_MODE_USBPLAYCHANGE);
    return;
}

int ModemanagerClass::get_usbVideoswitchOnOff()
{
    char iusbVideoswitchOnOffsts = 0;

    if(iusbVideoswitchOnOffsts != 1)
    {
       iusbVideoswitchOnOffsts = 0;
    }

    m_iUsbVideoswitchOnOff = iusbVideoswitchOnOffsts;
    HMI_LOG("Get m_iUsbVideoswitchOnOff = %d", m_iUsbVideoswitchOnOff);

    return m_iUsbVideoswitchOnOff;
}
//wangtingting add for SX5-477 end
