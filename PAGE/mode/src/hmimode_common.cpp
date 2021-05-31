#include "hmimode_common.h"
#include "../../../FRAMEWORK/inc/hmifrm_keyevent.h"
#include "hmiapi_audiomanager.h"
#include "hmiapi_modemanager.h"
#include "hmifrm_keyclass.h"


const Frm_AbstractModeClass::S_VIEWTABLE viewTable[] =
{
    {"NULL",   0, 0, (void*)(NULL),NULL},
};

Mode_Common::Mode_Common(QObject* Framework,QObject* parent, quint16 modeid, quint16 screenid,quint16 modeattr )
    : Frm_AbstractModeClass(Framework,parent,modeid,screenid, modeattr)
{
    ViewTable = (S_VIEWTABLE*)(&viewTable);
    ViewTableNum = sizeof(viewTable) / sizeof(S_VIEWTABLE);

    m_iCurrentVolme = 0;
    m_iVolmeBeforeMute = -1;
    m_iCurrentEQType = 0;
    m_iCurrentLoudType = 0;

    m_iVolLongPressTimer = 0;
    m_isVolUpPressed = -1;
    m_isPausebyMute = 0;
    m_isHKSeekRelease = 0;
    m_muteflagbefore = 0;
    m_powerpressflag = 0;
	m_tunerlongpressUpTimer = 0;
    m_tunerlongpressDnTimer = 0;
    m_tbtonsshow = 0;
    m_usbLoading = 0;
    m_ipodejectTimer = 0;
    m_usbloadons = 0;
    m_itunerstatusbk = 0;
    message_sent_num = 0;//add by yulei
    m_checkSrcViewtimer = startTimer(1000);//check view and audio src every 1 second
    b_flag = false; //add by yulei
    // m_dvrfirstflag = 0;  //zhangwenbin delete 1123
}

bool Mode_Common::concreteEvent(QEvent *e)
{
    bool b_ret = false;
    Frm_AbstractEvent* fAbstractEvent = dynamic_cast<Frm_AbstractEvent*>(e);
    if( NULL != fAbstractEvent )
    {
        switch( fAbstractEvent->getEventType() )
        {
        case Frm_AbstractEvent::EN_TYPE_KEYEVENT:
        {
            HMI_LOG("11111Mode_Common::concreteEvent(QEvent *e)");
            b_ret = hardKeyEventProc(e);
        }
            break;
        case Frm_AbstractEvent::EN_TYPE_APPTOHMI:
        {
//             HMI_LOG("EN_TYPE_APPTOHMI");
            hmiEventProc(e);
        }
            break;
		case Frm_AbstractEvent::EN_TYPE_AUDIOMODE:
			HMI_LOG("EN_TYPE_AUDIOMODE");
            modEventProc(e);
            break;
        default:
            break;
        }
    }

    return b_ret;
}
/******************************************************************************
Function Name : consumeEvent
Description   : concrete the event
Date          : 2017/09/28
Parameter     :
para1         : QEvent *e
Author        : juanjuan(visteon)
******************************************************************************/
int Mode_Common::consumeEvent(QEvent *e)
{
    int i_ret = 0;
    Frm_AbstractEvent* fAbstractEvent = dynamic_cast<Frm_AbstractEvent*>(e);
    HMI_LOG("consumeEvent");
    if( NULL != fAbstractEvent )
    {
        switch( fAbstractEvent->getEventType() )
        {
        case Frm_AbstractEvent::EN_TYPE_APPTOHMI:
            //HMI_LOG("event ID %d",fAbstractEvent->getFuncID());
            i_ret = hmiEventProcess(e);
            break;
        case Frm_AbstractEvent::EN_TYPE_KEYEVENT:
            //HMI_LOG("event ID %d",fAbstractEvent->getFuncID());
            break;
        default:
            break;
        }
    }
    return i_ret;
}

int Mode_Common::hmiEventProcess(QEvent *e)
{
    int i_ret = 0;
    bool b_ret = false;
    int modeId = 0;//if remove this mode,modeId should be this mode,if replace by other mode,modeId should be the mode you want to when you exit interrupt mode.

    Frm_FuncEvent* fNaviEvent = dynamic_cast<Frm_FuncEvent*>(e);
    if( NULL != fNaviEvent )
    {
        switch(fNaviEvent->getFuncID())
        {
        case D_HMIAPI_EV_MODE_FIRST_BASE_SRC_ACTIVE: //juanjuan add
            HMI_LOG("D_HMIAPI_EV_MODE_FIRST_BASE_SRC_ACTIVE");
            if(NULL == this->get_childmode()) //only one mode
            {
                HMI_LOG("only common mode exist, it's impossible");
                break;
            }
            if(NULL == this->get_childmode()->get_childmode())// child mode haven't a child mode
            {
                HMI_LOG("there is no interrupt mode");
                break;
            }
            modeId = Frm_CommonClassIns->viewModeIdOfBaseSrcMode();
            break;
        case D_HMIAPI_EV_MODE_STATUS_ABOLISH:
            HMI_LOG("D_HMIAPI_EV_MODE_STATUS_ABOLISH");
            if(NULL == this->get_childmode()) //only one mode
            {
                HMI_LOG("only common mode exist, it's impossible");
                break;
            }
            if(NULL == this->get_childmode()->get_childmode())// child mode haven't a child mode
            {
                HMI_LOG("there is no interrupt mode");
                break;
            }
            /*child mode source is not the abolish source, maybe something wrong*/
            b_ret = true;
            break;
        case D_HMIAPI_EV_MODE_NEED_CHANGE2SRCVIEW:
            if(2 == Frm_CommonClassIns->getTopModeAttr())
            {
                modeId = Frm_CommonClassIns->viewModeIdOfBaseSrcMode();
            }
            else
            {
                ViewModeEnum mode = (ViewModeEnum)Frm_CommonClassIns->viewModeIdOfBaseSrcMode();
                reqModeTrans(mode, 1, EN_MODE_ON_BASE);
            }
            b_ret = true;
            break;
        default:
            break;
        }
    }
    i_ret = modeId<<1;
    i_ret |=b_ret;
    return i_ret;
}
void Mode_Common::hmiEventProc(QEvent *e)
{
    Frm_FuncEvent* fNaviEvent = dynamic_cast<Frm_FuncEvent*>(e);
    int i_deviceType = 0;
    int i_curMode = ModemanagerClassIns->Get_CurrentMode();
    int i_viewcurmode = Frm_CommonClassIns->get_current_view_mode();
    HMI_LOG("i_viewcurmode = %d", i_viewcurmode);
//    enum CMUCarPlayEventType en_carplayevent = CMU_CARPLAY_EVENT_SCREEN_STOPPED;
    if( NULL != fNaviEvent )
    {
        switch(fNaviEvent->getFuncID())
        {

//CM7C-3918 Bug is modified by zhoudan 0830
        case D_HMIAPI_EV_MODE_DIAGNOSE_IN:
            showDiagons();
            break;
        case D_HMIAPI_EV_MODE_DIAGNOSE_OUT:
            destroyDiagons();
            break;
        case D_HMIAPI_EV_MODE_USB_EJECT_COMPLETE:
            HMI_LOG("D_HMIAPI_EV_MODE_USB_EJECT_COMPLETE");
            //i_deviceType = DevicesClassIns->Get_DeviceType();
            if ( 9 == i_deviceType )
            {
                showMessage(QObject::tr("MTP设备已断开"));
            }
            else
            {
                showMessage(QObject::tr("USB设备已断开"));
            }
            break;
        case D_HMIAPI_EV_MODE_IPOD_EJECT_COMPLETE:
            HMI_LOG("D_HMIAPI_EV_MODE_IPOD_EJECT_COMPLETE");
            m_ipodejectTimer = startTimer(1000);
            break;
        case D_HMIAPI_EV_MODE_BTA_EJECT_COMPLETE:
        {

        }
            break;
        case D_HMIAPI_EV_MODE_AUX_EJECT_COMPLETE:
            HMI_LOG("D_HMIAPI_EV_MODE_AUX_EJECT_COMPLETE");
            showMessage(QObject::tr("AUX设备已断开"));
            break;
        case D_HMIAPI_EV_MODE_LINK_EJECT_COMPLETE:
            HMI_LOG("D_HMIAPI_EV_MODE_LINK_EJECT_COMPLETE");
            break;

        case D_HMIAPI_EV_MODE_HMIISREADY:
            break;
        case D_HMIAPI_EV_AM_UNMUTE_BY_TOUCH:
            m_iCurrentVolme = AudioManagerClassIns->Get_VolumeValue();
            if (0 == m_iCurrentVolme)
            {
                m_iVolmeBeforeMute = 1;
            }
            else
            {
                m_iCurrentVolme = m_iVolmeBeforeMute;
                m_iVolmeBeforeMute = -1;
            }
            emit sigMute();
            break;
        case D_HMIAPI_EV_AM_VOLUME_CHANGED:
            emit sigMute();
            break;
            //Start for seek up down when navi/setting show
        case D_HMIAPI_EV_AM_DIAGONS_CHANGED:
        case D_HMIAPI_EV_MODE_DIAGONS_CHANGED:
			break;
        case D_HMIAPI_EV_MODE_SHOW_AVM:
        case D_HMIAPI_EV_MODE_SHOW_RVC:
            HMI_LOG("D_HMIAPI_EV_MODE_RVC_IN");
            destroyPDCkons();
            if( 1 == m_tbtonsshow )
            {
                emit sigtbtchange();
            }
            hideVolumeBar();
            if (0 != m_iVolLongPressTimer)
            {
                killTimer(m_iVolLongPressTimer);
                m_iVolLongPressTimer = 0;
            }
            m_isVolUpPressed = -1;
            destroyPhoneUpdatePopup();
            destroyshowMessage();
            destroynavitbtons();
            m_tbtonsshow = 0;
            destroyDiagons();
            //reqModeTrans(EN_VIEW_MODE_INT_AVM,1,EN_MODE_ON_INTERRUPT);
            break;

        case D_HMIAPI_EV_MODE_SHOW_DVR:
            HMI_LOG("D_HMIAPI_EV_MODE_SHOW_DVR");
            hideVolumeBar();
            if (0 != m_iVolLongPressTimer)
            {
                killTimer(m_iVolLongPressTimer);
                m_iVolLongPressTimer = 0;
            }
            m_isVolUpPressed = -1;
            destroyPhoneUpdatePopup();
            destroyshowMessage();
            destroyDiagons();
            //reqModeTrans(EN_VIEW_MODE_INT_DVR,1,EN_MODE_ON_INTERRUPT);
            break;

        case D_HMIAPI_EV_MODE_NAVISPEAK:
            HMI_LOG("D_HMIAPI_EV_MODE_NAVISPEAK");
            hideVolumeBar();
            if (0 != m_iVolLongPressTimer)
            {
                killTimer(m_iVolLongPressTimer);
                m_iVolLongPressTimer = 0;
            }
            m_isVolUpPressed = -1;
            break;

        case D_HMIAPI_EV_MODE_SAVE_LASTSCREEN:
        {
            HMI_LOG("D_HMIAPI_EV_MODE_SAVE_LASTSCREEN");
            if (0 != m_iVolLongPressTimer)
            {
                killTimer(m_iVolLongPressTimer);
                m_iVolLongPressTimer = 0;
            }
            m_isVolUpPressed = -1;
            enum HMI_ScreenType type = E_SCREEN_HOME;

            break;
        }
        case D_HMIAPI_EV_MODE_SAVE_EVLASTSCREEN:
        {
            HMI_LOG("D_HMIAPI_EV_MODE_SAVE_EVLASTSCREEN");
            enum HMI_ScreenType type = E_SCREEN_HOME;

            ModemanagerClassIns->Ope_save_last_screent(type);
            break;
        }
        case D_HMIAPI_EV_AM_SCREEN_UNMUTE:
            HMI_LOG("D_HMIAPI_EV_AM_SCREEN_UNMUTE");
            m_iCurrentVolme = m_iVolmeBeforeMute;
            m_iVolmeBeforeMute = -1;
            emit sigMute();
            break;
        case D_HMIAPI_EV_MODE_STATUS_CHANGED:
            if((E_MODE_INT_PHONE == i_curMode)
              || (E_MODE_INT_RINGTONE == i_curMode)
              || (E_MODE_INT_ECALL == i_curMode)
              || (E_MODE_INT_CARPLAY_TEL == i_curMode)
              || (E_MODE_INT_CARPLAY_RING == i_curMode))
            {
                if (0 != m_iVolLongPressTimer)
                {
                    killTimer(m_iVolLongPressTimer);
                    m_iVolLongPressTimer = 0;
                }
                hideVolumeBar();
            }
            HMI_LOG("D_HMIAPI_EV_MODE_STATUS_CHANGED");
            emit sigModeStatusChanged();
            break;
        case D_HMIAPI_EV_MODE_VIEDIO_FORBID:
            HMI_LOG("D_HMIAPI_EV_MODE_VIEDIO_FORBID");
            break;
        case D_HMIAPI_EV_MODE_NAVI_VOLBAR_HIDE:
        case D_HMIAPI_EV_MODE_COLORBARSHOW:
        case D_HMIAPI_EV_AM_KEYMUTESTATUS_CHANGED:
            HMI_LOG("D_HMIAPI_EV_AM_KEYMUTESTATUS_CHANGED");
            if( 1 == AudioManagerClassIns->Get_muteflag() )
            {
                m_iVolmeBeforeMute = AudioManagerClassIns->Get_VolumeValue();
                m_iCurrentVolme = AudioManagerClassIns->Get_VolumeValue();
                m_isPausebyMute = 1;
                HMI_LOG("111m_iVolmeBeforeMute=%d,m_iCurrentVolme=%d",m_iVolmeBeforeMute,m_iCurrentVolme);

            }
            else
            {
                m_iCurrentVolme = AudioManagerClassIns->Get_VolumeValue();
                m_iVolmeBeforeMute = -1;
                HMI_LOG("0000m_iVolmeBeforeMute=%d,m_iCurrentVolme=%d",m_iVolmeBeforeMute,m_iCurrentVolme);

            }
            emit sigMute();
            break;
        case D_HMIAPI_EV_MODE_HMISHOWING:

            break;
        case D_HMIAPI_EV_MODE_IGN_STATUS:
            HMI_LOG("D_HMIAPI_EV_MODE_IGN_STATUS");
            emit sigIgnStatusChanged();
            break;
        default:
            break;
        }
    }

    return;
}

void Mode_Common::modEventProc(QEvent *e)
{
    Frm_FuncEvent* pNaviEvent = dynamic_cast<Frm_FuncEvent*>(e);
    if( NULL != pNaviEvent )
    {
        switch(pNaviEvent->getFuncID())
        {
		case D_HMIAPI_EV_MODE_STATUS_FOCUSED:
			HMI_LOG("D_HMIAPI_EV_MODE_STATUS_FOCUSED");
			break;
        default:
            break;
        }
    }
}
void Mode_Common::set_cleartbtonsflag()
{
    HMI_LOG("IN");
    m_tbtonsshow = 0;
    return;
}

void Mode_Common::concreteVisible()
{
    HMI_LOG("concreteVisible");
}

void Mode_Common::concreteInvisible()
{
    HMI_LOG("concreteInvisible");
}

Mode_Common::~Mode_Common()
{

}

void Mode_Common::concreteStart()
{
    HMI_LOG("concreteStart");
}
void Mode_Common::concreteEnd()
{
    HMI_LOG("concreteEnd");
}

/******************************************************************************
Function Name : hardkeyEventProc
Description   : Hardkey proc
Date          : 2015/09/11
Parameter     :
para1         : QEvent *e
Author        : Maotianliang(Neusoft)
******************************************************************************/
bool Mode_Common::hardKeyEventProc(QEvent *e)
{
    bool b_ret = true;
    int volumeflag = 0;
    // int i_usbState = 0;
    Frm_KeyEvent* fHKEvent = dynamic_cast<Frm_KeyEvent*>(e);
    Frm_FuncEvent* FuncEvent = NULL;
    Frm_CommonClass* frameworkclass = Frm_CommonClassIns;
    int i_curMode = ModemanagerClassIns->Get_CurrentMode();
    int i_viewcurmode = Frm_CommonClassIns->get_current_view_mode();
    if( NULL != fHKEvent )
    {
        HMI_LOG("fHKEvent->getFuncID() = %d", fHKEvent->getFuncID());
        switch(fHKEvent->getFuncID())
        {
        case KEYEVENT_HOME_RELEASE:
        {
            HMI_LOG("KEYEVENT_HOME_RELEASE");
            reqModeTrans(EN_VIEW_MODE_HOME,1,EN_MODE_ON_BASE);
            break;
        }
        case KEYEVENT_POWER_PRESS:
            HMI_LOG("KEYEVENT_POWER_PRESS");
            break;
        case KEYEVENT_POWER_RELEASE:
            HMI_LOG("KEYEVENT_POWER_RELEASE m_muteflagbefore=%d",m_muteflagbefore);
            HMI_LOG("KEYEVENT_POWER_RELEASE m_iCurrentVolme=%d",m_iCurrentVolme);
            ModemanagerClassIns->Ope_PowerPress(1);
            break;
        case KEYEVENT_POWER_LONGPRESS:
            HMI_LOG("KEYEVENT_POWER_LONGPRESS");
            break;
        case KEYEVENT_POWER_LONGRELEASE:
            HMI_LOG("KEYEVENT_POWER_LONGRELEASE");
            ModemanagerClassIns->Ope_PowerPress(1);
            break;
        case KEYEVENT_USB_RELEASE:
        {
            HMI_LOG("KEYEVENT_USB_RELEASE");
       }
            break;
        case KEYEVENT_SETUP_RELEASE:
            HMI_LOG("KEYEVENT_SETUP_RELEASE");
            break;
        case KEYEVENT_SEEK_UP_PRESS:
        case KEYEVENT_STW_SEEK_UP_PRESS:
            HMI_LOG("KEYEVENT_SEEK_UP_PRESS");
            break;
        case KEYEVENT_SEEK_UP_LONGPRESS:
            HMI_LOG("KEYEVENT_SEEK_UP_LONGPRESS");
            break;
        case KEYEVENT_STW_SEEK_UP_LONGPRESS:
            HMI_LOG("KEYEVENT_SEEK_UP_LONGPRESS");
            break;
        case KEYEVENT_SEEK_UP_LONGPRESS_2S:
            HMI_LOG("KEYEVENT_SEEK_UP_LONGPRESS_2S");
            break;
        case KEYEVENT_SEEK_UP_LONGRELEASE:
            HMI_LOG("KEYEVENT_SEEK_UP_LONGRELEASE");

            break;
        case KEYEVENT_STW_SEEK_UP_LONGRELEASE:
            HMI_LOG("KEYEVENT_STW_SEEK_UP_LONGRELEASE");
            break;
        case KEYEVENT_SEEK_DN_PRESS:
        case KEYEVENT_STW_SEEK_DN_PRESS:
            HMI_LOG("KEYEVENT_SEEK_DN_PRESS");
            break;
        case KEYEVENT_SEEK_DN_RELEASE:
            HMI_LOG("KEYEVENT_SEEK_DN_RELEASE");
            break;
        case KEYEVENT_STW_SEEK_DN_RELEASE:
            HMI_LOG("KEYEVENT_STW_SEEK_DN_RELEASE");

            break;
        case KEYEVENT_SEEK_DN_LONGPRESS:
            HMI_LOG("KEYEVENT_SEEK_DN_LONGPRESS");

            break;
        case KEYEVENT_STW_SEEK_DN_LONGPRESS:
            HMI_LOG("KEYEVENT_STW_SEEK_DN_LONGPRESS");

            break;
        case KEYEVENT_SEEK_DN_LONGPRESS_2S:
            HMI_LOG("KEYEVENT_SEEK_DN_LONGPRESS_2S");
            break;
        case KEYEVENT_SEEK_DN_LONGRELEASE:
            HMI_LOG("KEYEVENT_SEEK_DN_LONGRELEASE");

            break;
        case KEYEVENT_STW_SEEK_DN_LONGRELEASE:
            HMI_LOG("KEYEVENT_STW_SEEK_DN_LONGRELEASE");

            break;
        case KEYEVENT_DISP_RELEASE:
            HMI_LOG("KEYEVENT_DISP_RELEASE");        
            break;
        case KEYEVENT_NAVI_RELEASE:
		{
            HMI_LOG("KEYEVENT_NAVI_RELEASE");
            if(true == ModemanagerClassIns->is_carplayshow())
            {
                ModemanagerClassIns->Ope_Hidecarplay();
            }
            break;
		}
        case KEYEVENT_EQ_RELEASE:
            break;
        case KEYEVENT_STW_MUTE_PRESS:
        case KEYEVENT_MUTE_PRESS:
            HMI_LOG("KEYEVENT_MUTE_PRESS");
            if(1 == ModemanagerClassIns->Get_NavispekStatus())
            {
                break;
            }
            volumeflag = 1;
            break;
        case KEYEVENT_MUTE_RELEASE:
        case KEYEVENT_STW_MUTE_RELEASE:
            HMI_LOG("KEYEVENT_MUTE_RELEASE");

            if(1 == ModemanagerClassIns->Get_NavispekStatus())
            {
                break;
            }
            m_iCurrentVolme = AudioManagerClassIns->Get_VolumeValue();
            if (-1 != m_iVolmeBeforeMute)
            {
                AudioManagerClassIns->Ope_UnMute();
                m_iCurrentVolme = m_iVolmeBeforeMute;
                m_iVolmeBeforeMute = -1;
            }
            else
            {
                m_iVolmeBeforeMute = m_iCurrentVolme;
                AudioManagerClassIns->Ope_Mute();
            }
            showVolumeBar(m_iCurrentVolme);
            volumeflag = 1;
            emit sigMute();
            break;
        case KEYEVENT_VOL_UP_RELEASE:
              HMI_LOG("KEYEVENT_VOL_UP_RELEASE COMMON");
            if(1 == ModemanagerClassIns->Get_NavispekStatus())
            {
                break;
            }

            if(10 == ModemanagerClassIns->Get_volummode())
            {
                m_iCurrentVolme = AudioManagerClassIns->Get_CarPlayVolumeValue() + 1;
                AudioManagerClassIns->Ope_VolumeUp_CarplayAlt();
                showVolumeBar(m_iCurrentVolme);
                volumeflag = 1;
                break;
            }

            m_iCurrentVolme = AudioManagerClassIns->Get_VolumeValue();
            if (-1 != m_iVolmeBeforeMute)
            {
                if ((0 == m_iCurrentVolme)&& (0 == m_iVolmeBeforeMute))
                {
                    m_iVolmeBeforeMute = 0;
                }

                AudioManagerClassIns->Ope_UnMute();

                //unmute
                m_iCurrentVolme = m_iVolmeBeforeMute + 1;
                m_iVolmeBeforeMute = -1;

            }
            else
            {
                m_iCurrentVolme = AudioManagerClassIns->Get_VolumeValue() + 1;
            }
            AudioManagerClassIns->Ope_VolumeUp();
            HMI_LOG("m_iCurrentVolme=%d",m_iCurrentVolme);
            showVolumeBar(m_iCurrentVolme);
            volumeflag = 1;
            emit sigMute();
            break;
        case KEYEVENT_VOL_UP_LONGPRESS:
             HMI_LOG("KEYEVENT_VOL_UP_LONGPRESS COMMON");
            if(1 == ModemanagerClassIns->Get_NavispekStatus())
            {
                break;
            }
            if(10 == ModemanagerClassIns->Get_volummode())
            {
                m_iCurrentVolme = AudioManagerClassIns->Get_CarPlayVolumeValue() + 1;
                AudioManagerClassIns->Ope_VolumeUp_CarplayAlt();
                showVolumeBar(m_iCurrentVolme);

                m_isVolUpPressed = 1;
                if (0 != m_iVolLongPressTimer)
                {
                    killTimer(m_iVolLongPressTimer);
                }
                m_iVolLongPressTimer = startTimer(HMI_VOLUME_LONGPRESS_INTERVAL);
                volumeflag = 1;
                break;
            }

            m_iCurrentVolme = AudioManagerClassIns->Get_VolumeValue();
            if (-1 != m_iVolmeBeforeMute)
            {
                //unmute
                if ((0 == m_iCurrentVolme)&& (0 == m_iVolmeBeforeMute))
                {
                    m_iVolmeBeforeMute = 0;
                }

                AudioManagerClassIns->Ope_UnMute();

                m_iCurrentVolme = m_iVolmeBeforeMute + 1;
                m_iVolmeBeforeMute = -1;

            }
            else
            {
                m_iCurrentVolme = AudioManagerClassIns->Get_VolumeValue() + 1;
            }
            AudioManagerClassIns->Ope_VolumeUp();
            showVolumeBar(m_iCurrentVolme);
            m_isVolUpPressed = 1;
            if (0 != m_iVolLongPressTimer)
            {
                killTimer(m_iVolLongPressTimer);
            }
            m_iVolLongPressTimer = startTimer(HMI_VOLUME_LONGPRESS_INTERVAL);
            volumeflag = 1;
            emit sigMute();
            break;
        case KEYEVENT_VOL_DN_RELEASE:
            HMI_LOG("KEYEVENT_VOL_DN_RELEASE COMMON");
            if(1 == ModemanagerClassIns->Get_NavispekStatus())
            {
                break;
            }

            if(10 == ModemanagerClassIns->Get_volummode())
            {
                if ( 0 == AudioManagerClassIns->Get_CarPlayVolumeValue() )
                {
                    m_iCurrentVolme = 0;
                }
                else
                {
                    m_iCurrentVolme = AudioManagerClassIns->Get_CarPlayVolumeValue() - 1;
                }
                AudioManagerClassIns->Ope_VolumeDown_CarplayAlt();
                showVolumeBar(m_iCurrentVolme);
                volumeflag = 1;
                break;
            }

            if (-1 != m_iVolmeBeforeMute)
            {
                //unmute
                if ( 0 == m_iVolmeBeforeMute )
                {
                    m_iCurrentVolme = 0;
                }
                else
                {
                    m_iCurrentVolme = m_iVolmeBeforeMute - 1;
                }

                m_iVolmeBeforeMute = -1;
                AudioManagerClassIns->Ope_UnMute();
            }
            else
            {
                if ( 0 == AudioManagerClassIns->Get_VolumeValue() )
                {
                    m_iCurrentVolme = 0;
                }
                else
                {
                    m_iCurrentVolme = AudioManagerClassIns->Get_VolumeValue() - 1;
                }
            }

            if(/*(true == PhoneControlIns->Is_calling())*/ //wangtingting add for SX5VA-252
                    E_MODE_INT_PHONE == ModemanagerClassIns->Get_CurrentMode()
                    ||E_MODE_INT_RINGTONE == ModemanagerClassIns->Get_CurrentMode()
                    ||E_MODE_INT_CARPLAY_TEL == ModemanagerClassIns->Get_CurrentMode()
                    ||E_MODE_INT_CARPLAY_RING == ModemanagerClassIns->Get_CurrentMode())
            {
                if( m_iCurrentVolme >= 5 )
                {
                    AudioManagerClassIns->Ope_VolumeDown();
                }
                else
                {
                    m_iCurrentVolme = 5;
                }
            }
            else
            {
                    AudioManagerClassIns->Ope_VolumeDown();
            }
            showVolumeBar(m_iCurrentVolme);
            volumeflag = 1;
            emit sigMute();
            break;
        case KEYEVENT_VOL_DN_LONGPRESS:
             HMI_LOG("KEYEVENT_VOL_DN_LONGPRESS COMMON");
            if(1 == ModemanagerClassIns->Get_NavispekStatus())
            {
                break;
            }

            if(10 == ModemanagerClassIns->Get_volummode())
            {
                if ( 0 == AudioManagerClassIns->Get_CarPlayVolumeValue() )
                {
                    m_iCurrentVolme = 0;
                }
                else
                {
                    m_iCurrentVolme = AudioManagerClassIns->Get_CarPlayVolumeValue() - 1;
                }
                AudioManagerClassIns->Ope_VolumeDown_CarplayAlt();
                showVolumeBar(m_iCurrentVolme);

                m_isVolUpPressed = 0;
                if (0 != m_iVolLongPressTimer)
                {
                    killTimer(m_iVolLongPressTimer);
                }
                m_iVolLongPressTimer = startTimer(HMI_VOLUME_LONGPRESS_INTERVAL);
                volumeflag = 1;
                break;
            }

            if (-1 != m_iVolmeBeforeMute)
            {
                //unmute
                if ( 0 == m_iVolmeBeforeMute )
                {
                    m_iCurrentVolme = 0;
                }
                else
                {
                    m_iCurrentVolme = m_iVolmeBeforeMute - 1;
                }

                m_iVolmeBeforeMute = -1;
                AudioManagerClassIns->Ope_UnMute();
            }
            else
            {
                if ( 0 == AudioManagerClassIns->Get_VolumeValue() )
                {
                    m_iCurrentVolme = 0;
                }
                else
                {
                    m_iCurrentVolme = AudioManagerClassIns->Get_VolumeValue() - 1;
                }
            }

            if(/*(true == PhoneControlIns->Is_calling())*/ //wangtingting add for SX5VA-252
                    E_MODE_INT_PHONE == ModemanagerClassIns->Get_CurrentMode()
                    ||E_MODE_INT_RINGTONE == ModemanagerClassIns->Get_CurrentMode()
                    ||E_MODE_INT_CARPLAY_TEL == ModemanagerClassIns->Get_CurrentMode()
                    ||E_MODE_INT_CARPLAY_RING == ModemanagerClassIns->Get_CurrentMode())
            {
                if( m_iCurrentVolme >= 5 )
                {
                    AudioManagerClassIns->Ope_VolumeDown();
                }
                else
                {
                    m_iCurrentVolme = 5;
                }
            }
            else
            {
                    AudioManagerClassIns->Ope_VolumeDown();
            }
            showVolumeBar(m_iCurrentVolme);
            m_isVolUpPressed = 0;
            if (0 != m_iVolLongPressTimer)
            {
                killTimer(m_iVolLongPressTimer);
            }
            m_iVolLongPressTimer = startTimer(HMI_VOLUME_LONGPRESS_INTERVAL);
            volumeflag = 1;
            emit sigMute();
            break;
        case KEYEVENT_VOL_UP_LONGRELEASE:
        case KEYEVENT_VOL_DN_LONGRELEASE:
            HMI_LOG("KEYEVENT_VOL_DN_LONGRELEASE COMMON");
            if (0 != m_iVolLongPressTimer)
            {
                killTimer(m_iVolLongPressTimer);
                m_iVolLongPressTimer = 0;
            }
            m_isVolUpPressed = -1;
            if(1 == ModemanagerClassIns->Get_NavispekStatus())
            {
                break;
            }

            volumeflag = 1;
            break;
        case KEYEVENT_PHONE_PRESS:
            HMI_LOG("KEYEVENT_PHONE_PRESS");
            break;
        case KEYEVENT_NAVI2PHONE_PRESS:
            break;
        case KEYEVENT_NAVI2PHONE_RELEASE:

            HMI_LOG("KEYEVENT_NAVI2PHONE_RELEASE");

            break;
        case KEYEVENT_PHONE_RELEASE:
            HMI_LOG("KEYEVENT_PHONE_RELEASE Received");


            break;
        case KEYEVENT_PHONE_LONGPRESS:
            HMI_LOG("KEYEVENT_PHONE_LONGPRESS Received");
            break;
        case KEYEVENT_PHONE_LONGRELEASE:
            HMI_LOG("KEYEVENT_PHONE_LONGRELEASE Received");

            break;
        case KEYEVENT_LINK_RELEASE:
        {
            HMI_LOG("KEYEVENT_LINK_RELEASE");
        }
            break;
        case KEYEVENT_STW_MODE_RELEASE:
            HMI_LOG("KEYEVENT_STW_MODE_RELEASE Received");
            FuncEvent = new Frm_FuncEvent((static_cast<QEvent::Type>(Frm_AbstractEvent::EN_EV_AUDIOMODE)), D_HMIAPI_EV_MODE_CYCLE_NEXT_SOURCE);
            QCoreApplication::postEvent( frameworkclass, FuncEvent);
            break;
        case KEYEVENT_ETM_ENTRY:
            HMI_LOG("KEYEVENT_ETM_ENTRY Received");             
            break;
        case KEYEVENT_FACTORY_MODE_ENTRY:
            HMI_LOG("KEYEVENT_FACTORY_MODE_ENTRY Received");
            break;
        case KEYEVENT_ASR_PRESS:
            HMI_LOG("KEYEVENT_ASR_PRESS Received");
           break;
        case KEYEVENT_ASR_RELEASE:
        {
            HMI_LOG("KEYEVENT_ASR_RELEASE Received");
        }
            break;
        case KEYEVENT_ASR_LONGPRESS:
            HMI_LOG("KEYEVENT_ASR_LONGPRESS Received");
           break;
        case KEYEVENT_ASR_LONGRELEASE:
            HMI_LOG("KEYEVENT_ASR_LONGRELEASE Received");
            break;
		case KEYEVENT_VOL_UP_PRESS:
        case KEYEVENT_VOL_DN_PRESS:
               HMI_LOG("KEYEVENT_VOL_DN_PRESS COMMON");
            volumeflag = 1;
            break;
        case KEYEVENT_CARPLAY_TEST:
            HMI_LOG("KEYEVENT_CARPLAY_TEST Received");
            break;
        default:
            b_ret = false;
            break;
        }
    }
    else
    {
        b_ret = false;
    }
    if(0 == volumeflag)
    {
        HMI_LOG("0 == volumeflag");
        hideVolumeBar();
    }
    return b_ret;
}

void Mode_Common::checkBaseViewmodeAndBaseSourcemode()
{
    Frm_AbstractModeClass* pMode = (Frm_AbstractModeClass*)(this->get_childmode());
    if(NULL != pMode)
    {
        quint16 basemodeid = pMode->getmodeID();
        int srcMode = Frm_CommonClassIns->getModeSource(basemodeid);
        int baseSrcMode = ModemanagerClassIns->Get_BaseSrcMode().mode;
        int reqBaseSrcMode = ModemanagerClassIns->Get_ReqBaseSrcMode();
        int transModeid = 0;//Frm_CommonClassIns->Get_transModeId();
        int transModeSrc = Frm_CommonClassIns->getModeSource(transModeid);
        int powerMode = ModemanagerClassIns->Get_PmState();
        HMI_LOG("basemodeid = %d,srcMode = %d,baseSrcMode = %d,reqBaseSrcMode = %d,transModeid = %d,transModeSrc = %d,powerMode = %d",\
                basemodeid,srcMode,baseSrcMode,reqBaseSrcMode,transModeid,transModeSrc,powerMode);
        if((0 != transModeid) && (baseSrcMode == transModeSrc)&&(baseSrcMode == reqBaseSrcMode))
        {
            HMI_LOG("req Mode:%s [src: %d], baseSrcMode is%d",Frm_CommonClassIns->getModeName(transModeid),srcMode,baseSrcMode);
            HMI_LOG("changeview to source view by user");
            reqModeTrans((ViewModeEnum)transModeid,1,EN_MODE_ON_BASE);
        }
        else
        {
            if(SX5_MODE_MAX != srcMode)
            {
                if(srcMode != baseSrcMode)
                {
                    if(baseSrcMode == reqBaseSrcMode)
                    {
                        HMI_LOG("req Mode:%s [src: %d], baseSrcMode is%d",Frm_CommonClassIns->getModeName(basemodeid),srcMode,baseSrcMode);
                        if(Frm_CommonClassIns->getTopModeAttr()!=2)
                        {
                            HMI_LOG("source and view is unpaired,need change view");
                            switch((SX5Mode)baseSrcMode)
                            {
                            case SX5_MODE_SRC_USB_AUDIO:
                                //reqModeTrans(EN_VIEW_MODE_USBM,1,EN_MODE_ON_BASE);
                                break;
                            case SX5_MODE_SRC_USB_VIDEO:
                                //reqModeTrans(EN_VIEW_MODE_USBV,1,EN_MODE_ON_BASE);
                                break;
                            case SX5_MODE_SRC_HDD_AUDIO:
                                reqModeTrans(EN_VIEW_MODE_HOME,1,EN_MODE_ON_BASE);
                                break;
                            case SX5_MODE_SRC_HDD_VIDEO:
                                reqModeTrans(EN_VIEW_MODE_HOME,1,EN_MODE_ON_BASE);
                                break;
                            case SX5_MODE_SRC_IPOD:
                                //reqModeTrans(EN_VIEW_MODE_IPOD,1,EN_MODE_ON_BASE);
                                break;
                            case SX5_MODE_SRC_AUX:
                                reqModeTrans(EN_VIEW_MODE_HOME,1,EN_MODE_ON_BASE);
                                break;
                            case SX5_MODE_SRC_BTAUDIO:
                                //reqModeTrans(EN_VIEW_MODE_BTMUSIC,1,EN_MODE_ON_BASE);
                                break;
                            case SX5_MODE_SRC_MIRRORLINK:
                                //reqModeTrans(EN_VIEW_MODE_WELINK,1,EN_MODE_ON_BASE);
                                break;
                            case SX5_MODE_SRC_MIRACAST:
                                reqModeTrans(EN_VIEW_MODE_HOME,1,EN_MODE_ON_BASE);
                                break;
                            case SX5_MODE_SRC_CARPLAY:
                                reqModeTrans(EN_VIEW_MODE_HOME,1,EN_MODE_ON_BASE);
                                break;
                            default:
                                break;
                            }
                        }
                        else
                        {
                            HMI_LOG("send D_HMIAPI_EV_MODE_NEED_CHANGE2SRCVIEW");
                            ModemanagerClassIns->sendEvent(D_HMIAPI_EV_MODE_NEED_CHANGE2SRCVIEW);
                        }
                    }
                    else
                    {
                        HMI_LOG("the request source is not active,wait next time");
                    }
                }
            }
            else
            {
                if(baseSrcMode == reqBaseSrcMode)//request source 与active source 一致，表明source base source 即将切换为此source。
                {
                    if(SX5_MODE_SRC_USB_VIDEO == baseSrcMode)
                    {
                        if(Frm_CommonClassIns->getTopModeAttr()!=2)
                        {
                            HMI_LOG("change to video mode");
                            //reqModeTrans(EN_VIEW_MODE_USBV,1,EN_MODE_ON_BASE);
                        }
                        else
                        {
                            HMI_LOG("send D_HMIAPI_EV_MODE_NEED_CHANGE2SRCVIEW");
                            ModemanagerClassIns->sendEvent(D_HMIAPI_EV_MODE_NEED_CHANGE2SRCVIEW);
                        }
                    }
                }
                else
                {
                    HMI_LOG("the request source is not active,wait next time");
                }
            }
         }
    } else {
        HMI_LOG("NULL == pMode");
    }
}

void Mode_Common::timerEvent(QTimerEvent *event)
{
//    int i_usbState = 0;
//    int i_curMode = ModemanagerClassIns->Get_CurrentMode();

    if (-1 == m_isVolUpPressed)
    {
        killTimer(m_iVolLongPressTimer);
        m_iVolLongPressTimer = 0;
    }

    if(event->timerId() == m_iVolLongPressTimer)
    {
        killTimer(m_iVolLongPressTimer);

        if(10 == ModemanagerClassIns->Get_volummode())
        {
            m_iCurrentVolme = AudioManagerClassIns->Get_CarPlayVolumeValue();
        }
        else
        {
            m_iCurrentVolme = AudioManagerClassIns->Get_VolumeValue();
        }

        if(1 == m_isVolUpPressed)
        {
            m_iCurrentVolme++;

            if(10 == ModemanagerClassIns->Get_volummode())
            {
                AudioManagerClassIns->Ope_VolumeUp_CarplayAlt();
            }
            else
            {
                AudioManagerClassIns->Ope_VolumeUp();
            }
        }
        else
        {
            if (1 == m_iCurrentVolme)
            {
                m_iVolmeBeforeMute = 1;
            }

            if (0 == m_iCurrentVolme)
            {
                m_iVolmeBeforeMute = -1;
                m_iCurrentVolme = 0;
            }
            else
            {
                m_iCurrentVolme--;
                if(/*(true == PhoneControlIns->Is_calling())*/ //wangtingting add for SX5VA-252
                        E_MODE_INT_PHONE == ModemanagerClassIns->Get_CurrentMode()
                        ||E_MODE_INT_RINGTONE == ModemanagerClassIns->Get_CurrentMode()
                        ||E_MODE_INT_CARPLAY_TEL == ModemanagerClassIns->Get_CurrentMode()
                        ||E_MODE_INT_CARPLAY_RING == ModemanagerClassIns->Get_CurrentMode())
                {
                    if( m_iCurrentVolme >= 5 )
                    {
                        AudioManagerClassIns->Ope_VolumeDown();
                    }
                    else
                    {
                        m_iCurrentVolme = 5;
                    }
                }
                else
                {
                    if(10 == ModemanagerClassIns->Get_volummode())
                    {
                        AudioManagerClassIns->Ope_VolumeDown_CarplayAlt();
                    }
                    else
                    {
                        AudioManagerClassIns->Ope_VolumeDown();
                    }
                }
            }
        }
        showVolumeBar(m_iCurrentVolme);
        //emit sigMute();

        m_iVolLongPressTimer = startTimer(HMI_VOLUME_LONGPRESS_INTERVAL_2);
    }
    else if(m_tunerlongpressUpTimer == event->timerId())
    {
        killTimer(m_tunerlongpressUpTimer);
        m_tunerlongpressUpTimer = 0;
        //EntertainmentClassIns->Ope_ChangeHez(E_SEEK_UP);
        m_tunerlongpressUpTimer = startTimer(300);
    }
    else if(m_tunerlongpressDnTimer == event->timerId())
    {
        killTimer(m_tunerlongpressDnTimer);
        m_tunerlongpressDnTimer = 0;
        //EntertainmentClassIns->Ope_ChangeHez(E_SEEK_DOWN);
        m_tunerlongpressDnTimer = startTimer(300);
    }
    else if(m_ipodejectTimer  == event->timerId())
    {
        HMI_LOG("SHOW IPOD EJECT");
        killTimer(m_ipodejectTimer);
        m_ipodejectTimer = 0;
    }
    else if(m_checkSrcViewtimer == event->timerId())
    {
        checkBaseViewmodeAndBaseSourcemode();
         //TestScreen();
    }
}

void Mode_Common::TestScreen()
{
}
