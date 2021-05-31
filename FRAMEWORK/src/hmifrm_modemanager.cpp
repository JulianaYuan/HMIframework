/*******************************************************
 File Name   : hmifrm_modemanager.cpp

 Created by  :  Mao.Tianliang(Neusoft)

 Created Date:  2015/08/03

Description   :   interacts with Modemanager, pairing between viewmode and audiomode
*********************************************************/

/****************Include  Section Begin***************************************************/
#include "../inc/hmifrm_modemanager.h"
#include "hmiapi_modemanager.h"
#include "hmifrm_keyclass.h"
/****************Include  Section End***************************************************/

/******************************************************************************
Function Name : Frm_modemanager
Description   : constructor
Date          : 2015/08/03
para1         : QObject* parent
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
Frm_modemanager::Frm_modemanager(QObject *parent) :
    QObject(parent)
{
    i_reqViewmode = 0;
    i_reqSource = 0;
    i_curMode = 0;
    i_ViewModeOfSource = -1;
    i_reqScreenid = 1;

    en_SourceStep = EN_MODEMANAGER_STEP_IDLE;
    en_DeviceEject = EN_MODEMANAGER_EJECT_NONE;
    i_movieforbid_transtype = -1;
    is_View_change = true;
    m_starttimer = 0;
    m_homeStartTimer = 0;
    m_sourceStartTimer = 0;
    m_screentype = (HMI_ScreenType)ModemanagerClassIns->Get_ScreenType();
    HMI_LOG("ModemanagerClassIns->Get_ScreenType() = %d",m_screentype);
}

/******************************************************************************
Function Name : isReqCurrentMode
Description   : Is current view mode is the same as request view mode
Date          : 2015/08/03
para1         : QEvent *p_event : event of Screen transition
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
bool Frm_modemanager::isReqCurrentMode(QEvent *p_event)
{
    bool b_ret = false;
    int i_mode = 0;

    Frm_ModeEvent* fModeEvent = dynamic_cast<Frm_ModeEvent*>(p_event);

    i_mode = fModeEvent->getModeID();
    i_curMode = ModemanagerClassIns->Get_CurrentMode();
    pairAudioMode2ViewMode(&i_ViewModeOfSource, ModemanagerClassIns->Get_BaseSrcMode().mode/*i_curMode*/);
    HMI_LOG("i_mode = %d", i_mode);
    HMI_LOG("i_ViewModeOfSource = %d", i_ViewModeOfSource);
    HMI_LOG("i_curMode = %d", i_curMode);
    HMI_LOG("fModeEvent->getReqType() = %d", fModeEvent->getReqType());

    HMI_LOG(" return = %d", b_ret);
    return b_ret;

}

/******************************************************************************
Function Name : reqModeTrans
Description   : According to View mode ,request to change auido mode.
Date          : 2015/08/03
para1         : QEvent *p_event : event of Screen transition
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_modemanager::reqModeTrans(QEvent *p_event)
{
    int i_mode = 0;
    if (NULL == p_event)
    {
        HMI_WARNING("NULL == p_event  return");
        return;
    }
    //backup the mode trans event
    memcpy(&m_ModeTransEvent,p_event,sizeof(QEvent));

    Frm_ModeEvent* fModeEvent = dynamic_cast<Frm_ModeEvent*>(p_event);
    i_reqViewmode = fModeEvent->getModeID();
    i_reqScreenid = fModeEvent->getScreenID();

    pairViewMode2AudioMode(&i_mode, i_reqViewmode);

    HMI_LOG(" modeid = %d. ", i_reqViewmode);

    switch(fModeEvent->getReqType())
    {
    case EN_MODE_OFF:
        en_SourceStep = EN_MODEMANAGER_STEP_MODEOFF_TRANS_REQ;
		HMI_LOG("en_SourceStep = %d", en_SourceStep);
        i_ViewModeOfSource = -1;
        break;
    default:

        ModemanagerClassIns->Req_ModeRequest((enum HMI_Mode)i_mode);
        break;
    }

    return;
}

/******************************************************************************
Function Name : customEvProc
Description   : receive the event frome API
Date          : 2015/08/03
para1         : QEvent *p_event
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_modemanager::customEvProc(QEvent *p_event)
{
    if(NULL == p_event)
    {
        return;
    }

    Frm_FuncEvent* fFuncEvent = dynamic_cast<Frm_FuncEvent*>(p_event);

    if( NULL != fFuncEvent )
    {
        switch(fFuncEvent->getFuncID())
        {
        case D_HMIAPI_EV_FRM_FIRST_BASE_SRC_ACTIVE:
            HMI_LOG("D_HMIAPI_EV_FRM_FIRST_BASE_SRC_ACTIVE");
            if(Frm_CommonClassIns->getTopModeAttr()!=2)
            {
                ActiveBaseMode_getted();
            }
            break;
        case D_HMIAPI_EV_MODE_CYCLE_NEXT_SOURCE:
            {
                CycleNextSource(ModemanagerClassIns->Get_BaseSrcMode().mode/*i_curMode*/);
            }
            break;
        default:
            break;
        }
    }
    return;
}

/******************************************************************************
Function Name : pairAudioMode2ViewMode
Description   : get the view mode by audio mode
Date          : 2015/08/03
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_modemanager::pairAudioMode2ViewMode(int * p_curViewMode, int i_curMode)
{
    switch (i_curMode)
    {
    case E_MODE_SRC_TUNER:

    default:
        *p_curViewMode = EN_VIEW_MODE_HOME;
        break;
    }
//    HMI_LOG("*p_curViewMode = %d", *p_curViewMode);
}

/******************************************************************************
Function Name : pairViewMode2AudioMode
Description   : get the audio mode by view mode
Date          : 2015/08/03
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_modemanager::pairViewMode2AudioMode(int *pi_Mode, int i_ViewMode)
{

    switch (i_ViewMode)
    {

    default:
        *pi_Mode  = -1;
        break;
    }
}

/******************************************************************************
Function Name : backFromNaviMode
Description   : change the view mode from Navi to current  mode
Date          : 2015/10/12
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_modemanager::backFromNaviMode()
{
    i_curMode = ModemanagerClassIns->Get_CurrentMode();
    pairAudioMode2ViewMode(&i_ViewModeOfSource, ModemanagerClassIns->Get_BaseSrcMode().mode/*i_curMode*/);


}

/******************************************************************************
Function Name : Init_AudioSource
Description   : change the view mode from Navi to current Entertainment mode
Date          : 2015/10/12
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_modemanager::Init_AudioSource()
{
    HMI_TRACE_IN;
    HMI_LOG("enter");
    en_SourceStep = EN_MODEMANAGER_STEP_INIT;
	HMI_LOG("en_SourceStep = %d", en_SourceStep);
    en_DeviceEject = EN_MODEMANAGER_EJECT_NONE;
    ModemanagerClassIns->Req_GetPmState();
    lastMode_getted();
}

/******************************************************************************
Function Name : USB_Eject
Description   :
Date          : 2015/11/20/
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_modemanager::USB_Eject()
{

}

/******************************************************************************
Function Name : IPOD_Eject
Description   :
Date          : 2015/11/20/
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_modemanager::IPOD_Eject()
{

}

/******************************************************************************
Function Name : BTA_Eject
Description   :
Date          : 2015/11/20/
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_modemanager::BTA_Eject()
{

}


/******************************************************************************
Function Name : AUX_Eject
Description   :
Date          : 2015/11/20/
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_modemanager::AUX_Eject()
{

}
/******************************************************************************
Function Name : Link_Eject
Description   :
Date          : 2015/11/20/
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_modemanager::LINK_Eject()
{

}

void Frm_modemanager::timerEvent(QTimerEvent* event)
{
    HMI_LOG("Enter");
    if(m_starttimer == event->timerId())
    {


    }
    else if(m_homeStartTimer == event->timerId())
    {

    }
    else if(m_sourceStartTimer == event->timerId())
    {

    }
    return;
}
/******************************************************************************
Function Name : is_Source_available
Description   :
Date          : 2015/11/20/
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
bool Frm_modemanager::is_Source_available(int mode)
{
    bool iret = true;
    switch(mode)
    {
    case E_MODE_SRC_TUNER:
        break;
    case E_MODE_SRC_USB_AUDIO:
        break;
    case E_MODE_SRC_IPOD:
        break;
    case E_MODE_SRC_USB_VIDEO:
        break;
    case E_MODE_SRC_HDD_AUDIO:
    case E_MODE_SRC_HDD_VIDEO:
        iret = false; // HDD  T.B.D
        break;
    case E_MODE_SRC_AUX:
        iret = false; // HDD  T.B.D
        break;
    case E_MODE_SRC_BTAUDIO:
        break;
    case E_MODE_INT_PHONE:
    case E_MODE_INT_VR:
    case E_MODE_MIX_NAVI:
        HMI_LOG("Mode is not Entertainment source");
        iret = false;
        break;
    default:
        HMI_ERROR("Mode is out of scope");
        iret = false;
        break;
    }

    HMI_LOG("iret = %d", iret);
    return iret;
}

/******************************************************************************
Function Name : lastMode_getted
Description   :
Date          : 2015/11/20/
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_modemanager::lastMode_getted()
{
    int i_lastMode = ModemanagerClassIns->Get_LastMode();
    HMI_LOG("Last Mode  i_lastMode = %d", i_lastMode);


    // when system boot, HMI change to top screen of Last mode
    if (EN_MODEMANAGER_STEP_INIT == en_SourceStep)
    {
        //check the last source vailable or not
        if (0 > i_lastMode || (E_MODE_MAX <= i_lastMode))
        {

        }
        else if(E_MODE_SRC_CARPLAY == i_lastMode)
        {

        }
        else if(E_MODE_SRC_USB_AUDIO == i_lastMode || E_MODE_SRC_USB_VIDEO == i_lastMode || E_MODE_SRC_IPOD == i_lastMode )
        {

        }
        else
        {

        }
    }
    else
    {
        HMI_LOG("Do nothing en_SourceStep = %d", en_SourceStep);
    }
}

/******************************************************************************
Function Name : ActiveBaseMode_getted
Description   :
Date          : 2018/01/10/
Author        : Yuanjuanjuan(visteon)
******************************************************************************/

void Frm_modemanager::ActiveBaseMode_getted()
{
    Frm_ModeEvent* modeOnEvent =NULL;
    Frm_CommonClass* frameworkclass = Frm_CommonClassIns;

    if ((HMI_MODE_DIAGNOSE_STATUS_ON == ModemanagerClassIns->get_diagnose_status())||(E_SCREEN_SOURCE == m_screentype)||(E_SCREEN_HOME == m_screentype))
    {
        HMI_LOG("m_sourceStartTimer");
        m_sourceStartTimer = startTimer(3000);
    }
    else if(E_SCREEN_NAVI == m_screentype)
    {
        if( true == ModemanagerClassIns->Get_isnaviready() )
        {

        }
        else
        {
            HMI_LOG("MODETRANS ");

        }
    }
}

/******************************************************************************
Function Name : FocusedMode_getted
Description   :
Date          : 2015/11/20/
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_modemanager::FocusedMode_getted()
{
    bool b_viewChange = false;
    int PopUpEventID = 0;
    int i_tempViewMode = 0;
    int i_curView = Frm_CommonClassIns->get_current_view_mode();
    enum ModeReqType en_modetype = EN_MODE_NONE;
    Frm_ModeEvent* modeOnEvent  = NULL;
    Frm_CommonClass* frameworkclass = Frm_CommonClassIns;

    HMI_LOG("i_curView = %d", i_curView);
    HMI_LOG("en_SourceStep = %d", en_SourceStep);
    HMI_LOG("en_DeviceEject = %d", en_DeviceEject);
    HMI_LOG("i_reqViewmode = %d", i_reqViewmode);

    i_curMode = ModemanagerClassIns->Get_CurrentMode();
    pairAudioMode2ViewMode(&i_tempViewMode, ModemanagerClassIns->Get_BaseSrcMode().mode/*i_curMode*/);
    if(i_tempViewMode == i_curView)
    {
        HMI_LOG("the view mode of this focus source is equeal to i_curView,no need to change view mode ");
        return;
    }
	if((E_MODE_SRC_USB_VIDEO == i_curMode)
        || (E_MODE_SRC_USB_AUDIO == i_curMode)
        || (E_MODE_SRC_IPOD == i_curMode)
        || (E_MODE_SRC_BTAUDIO == i_curMode)
        || (E_MODE_SRC_MIRRORLINK == i_curMode)
        || (E_MODE_SRC_MIRACAST == i_curMode)
        || (E_MODE_SRC_CARPLAY == i_curMode))
    {

    }

    if (HMI_MODE_DIAGNOSE_STATUS_ON == ModemanagerClassIns->get_diagnose_status())
    {
        b_viewChange = true;
        i_reqScreenid = 1;
    }
    else if(EN_MODEMANAGER_STEP_LAST_MODE_REQ == en_SourceStep)
    {
        HMI_LOG("m_screentype = %d",m_screentype);
        if((E_SCREEN_SOURCE == m_screentype) || (E_MODE_SRC_USB_VIDEO == i_curMode))
        {
            b_viewChange = true;
            HMI_LOG("b_viewChange = true");
            i_reqScreenid = 1;
        }
    }
    else if (EN_MODEMANAGER_STEP_NORMAL_TRANS_REQ == en_SourceStep)
    {
        HMI_LOG("m_screentype = %d en_SourceStep =%d",m_screentype,en_SourceStep);
        if ( E_MODE_SRC_USB_AUDIO == i_curMode
             || E_MODE_SRC_USB_VIDEO == i_curMode
             || E_MODE_SRC_IPOD == i_curMode
             || E_MODE_SRC_TUNER == i_curMode )
        {

        }

        if ( E_MODE_SRC_USB_AUDIO == i_curMode
             || E_MODE_SRC_USB_VIDEO == i_curMode )
        {

        }

        if ( E_MODE_SRC_USB_AUDIO == i_curMode
             || E_MODE_SRC_USB_VIDEO == i_curMode )
        {

        }

        if (i_reqViewmode == i_tempViewMode)
        {
            HMI_LOG("m_screentype = %d en_SourceStep =%d",m_screentype,en_SourceStep);
            b_viewChange = true;
        }
        else
        {
            HMI_ERROR("NORMAL_TRANS_REQ But i_reqViewmode[%d] != i_tempViewMode[%d]",i_reqViewmode,i_tempViewMode);
        }
    }
    else if (EN_MODEMANAGER_STEP_MODEOFF_TRANS_REQ == en_SourceStep)
    {
        HMI_LOG("m_screentype = %d en_SourceStep =%d",m_screentype,en_SourceStep);
        //do nothing
    }
    else if (EN_MODEMANAGER_STEP_VIDEO_FORBID_TANS_REQ == en_SourceStep)
    {
        HMI_LOG("m_screentype = %d en_SourceStep =%d",m_screentype,en_SourceStep);
        if (0 == i_movieforbid_transtype)
        {
            // trans to Media usb
           i_reqScreenid = 2; // HMIMODE_MEDIA_VIEW_USB
            b_viewChange = true;
        }
        else if (1 == i_movieforbid_transtype)
        {
            //trans to current source playing
            i_reqScreenid = 1;
            b_viewChange = true;
        }
        else
        {
            HMI_ERROR("i_movieforbid_transtype ERROR = %d", i_movieforbid_transtype);
        }
        i_movieforbid_transtype = -1;

    }
    else
    {
        int itempmode = -1;
        HMI_LOG("shijianhang debug i_curView=%d",i_curView);
        pairViewMode2AudioMode(&itempmode, i_curView);
        HMI_LOG("shijianhang debug itempmode=%d",itempmode);
        HMI_LOG("shijianhang debug i_curMode=%d",i_curMode);
        if((E_MODE_SRC_TUNER == i_curMode) || (E_MODE_SRC_USB_AUDIO == i_curMode)
          || (E_MODE_SRC_USB_VIDEO == i_curMode) || (E_MODE_SRC_IPOD == i_curMode)
          || (E_MODE_SRC_BTAUDIO == i_curMode))
        {
            if((itempmode != i_curMode) && (-1 != itempmode) && (E_MODE_INT_PHONE != itempmode) && (E_MODE_INT_RINGTONE != itempmode))
            {
                HMI_LOG("m_screentype = %d en_SourceStep =%d",m_screentype,en_SourceStep);
                b_viewChange = true;
            }
            else
            {
                HMI_LOG("m_screentype = %d en_SourceStep =%d",m_screentype,en_SourceStep);
            }
        }
        else
        {
            HMI_LOG("m_screentype = %d en_SourceStep =%d",m_screentype,en_SourceStep);
        }

    }

    i_ViewModeOfSource = i_tempViewMode;

   


    if (EN_MODEMANAGER_STEP_EJECT_TRANS_REQ == en_SourceStep)
    {
        HMI_LOG("sendEvent %x",PopUpEventID);
        ModemanagerClassIns->sendEvent(PopUpEventID);
    }
    en_SourceStep = EN_MODEMANAGER_STEP_IDLE;
    en_DeviceEject = EN_MODEMANAGER_EJECT_NONE;
}

/******************************************************************************
Function Name : CycleNextSource
Description   :
Date          : 2015/12/21/
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_modemanager::CycleNextSource(int i_curSource)
{
    int i_band = 0;
    int i_curView = Frm_CommonClassIns->get_current_view_mode();
    int i_curmode = ModemanagerClassIns->Get_CurrentMode();
    i_reqViewmode = 0;
    int ipreMode = ModemanagerClassIns->Get_PreviousMode();
    switch (i_curSource)
    {
    case E_MODE_SRC_TUNER:
        if (true == is_Source_available(E_MODE_SRC_TUNER))
        {
            ModemanagerClassIns->Req_ModeRequest((enum HMI_Mode)E_MODE_SRC_TUNER);
        }else
        {
            CycleNextSource(E_MODE_SRC_USB_AUDIO);
        }
        break;
    case E_MODE_SRC_USB_AUDIO:
        if (true == is_Source_available(E_MODE_SRC_IPOD))
        {
            ModemanagerClassIns->Req_ModeRequest((enum HMI_Mode)E_MODE_SRC_IPOD);
        }
        else
        {
            CycleNextSource(E_MODE_SRC_IPOD);
        }
        break;
    case E_MODE_SRC_IPOD:
        if (true == is_Source_available(E_MODE_SRC_BTAUDIO))
        {
            ModemanagerClassIns->Req_ModeRequest((enum HMI_Mode)E_MODE_SRC_BTAUDIO);
        }
        else
        {
            CycleNextSource(E_MODE_SRC_BTAUDIO);
        }
        break;
    case E_MODE_SRC_BTAUDIO:
        ModemanagerClassIns->Req_ModeRequest((enum HMI_Mode)E_MODE_SRC_TUNER);
        break;
    case E_MODE_SRC_USB_VIDEO:
        if (true == is_Source_available(ipreMode))
        {
            ModemanagerClassIns->Req_ModeRequest((enum HMI_Mode)ipreMode);
        }
        else
        {
            ModemanagerClassIns->Req_ModeRequest((enum HMI_Mode)E_MODE_SRC_TUNER);
        }
        break;
    case E_MODE_SRC_MIRRORLINK:
    case E_MODE_SRC_MIRACAST:
        if (true == is_Source_available(ipreMode))
        {
            pairAudioMode2ViewMode( &i_reqViewmode, ipreMode);
            ModemanagerClassIns->Req_ModeRequest((enum HMI_Mode)ipreMode);
        }
        else
        {
            ModemanagerClassIns->Req_ModeRequest((enum HMI_Mode)E_MODE_SRC_TUNER);
        }
        break;
    default:  
        ModemanagerClassIns->Req_ModeRequest((enum HMI_Mode)E_MODE_SRC_TUNER);
        break;
    }
}

void Frm_modemanager::USB_TransToPrevSource()
{
    int ipreMode = ModemanagerClassIns->Get_PreviousMode();

    //check the Previous source vailable or not
    if (true == is_Source_available(ipreMode))
    {
        i_reqSource = ipreMode;
    }
    else
    {
        i_reqSource = E_MODE_SRC_TUNER;
    }

    Frm_ModeEvent* modeOnEvent  = NULL;
    QCoreApplication::postEvent( Frm_CommonClassIns, modeOnEvent );

    return;
}
