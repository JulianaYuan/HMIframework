#include "../inc/hmiview_home.h"
#include "API/inc/hmiapi_modemanager.h"
#include "hmifrm_keyclass.h"

View_Home::~View_Home()
{
    if (NULL != pCursorCtrl)
    {
        delete pCursorCtrl;
        pCursorCtrl = NULL;
    }
}

View_Home::View_Home(QObject* parent, quint16 screenid ,quint16 attr)
    : Frm_AbstractViewClass(parent,screenid, attr)
{
    QML=(char*)"qrc:/qml/Srn_Home_Main.qml";
    pCursorCtrl = new CursorCtrlClass(this);
    setQml((QLatin1String)QML);
    QMLVIEW = (char*)"Home";
    RegisterQmlInsView(this,(QLatin1String)QMLVIEW);
    iCurosrIndex = -2;
    init( getViewPara(screenid));

    chargeflag = 0;
}

bool View_Home::concreteEvent(QEvent *e)
{
    bool b_ret = false;

    Frm_AbstractEvent* pAbstractEvent = dynamic_cast<Frm_AbstractEvent*>(e);
    if( NULL != pAbstractEvent )
    {
        switch( pAbstractEvent->getEventType() )
        {
        case Frm_AbstractEvent::EN_TYPE_REQFUNC:
            touchEventProc(e);
            break;
        case Frm_AbstractEvent::EN_TYPE_APPTOHMI:
            b_ret = hmiEventProc(e);
            break;
        case Frm_AbstractEvent::EN_TYPE_KEYEVENT:
            b_ret = hardKeyEventProc(e);
            break;
        case Frm_AbstractEvent::EN_TYPE_AUDIOMODE:
            modEventProc(e);
            break;
        default:
            break;
        }
    }


    return b_ret;
}
bool View_Home::hmiEventProc(QEvent *e)
{
    bool b_ret = false;
    Frm_FuncEvent* pNaviEvent = dynamic_cast<Frm_FuncEvent*>(e);
    if( NULL != pNaviEvent )
    {
        switch(pNaviEvent->getFuncID())
        {
        case D_HMIAPI_EV_MODE_OBTAIN_GETTED:
        case D_HMIAPI_EV_MODE_STATUS_FOCUSED:// this event seems never come here
            HMI_LOG("Home : D_HMIAPI_EV_MODE_ACTIVEMODE_GETTED Get back model success !!");
            emit sig_getbackmodelv();
            emit sigUsbMusicReady();
            break; 
        case D_HMIAPI_EV_COMMON_BUTTON_PRESSED:
            pCursorCtrl->updateListCursr(EN_CURSORCTRL_OPE_TYPE_HIDE);
            break;
        default:
            break;
        }
    }
    return b_ret;
}

void View_Home::modEventProc(QEvent *e)
{
    Frm_FuncEvent* pNaviEvent = dynamic_cast<Frm_FuncEvent*>(e);
    if( NULL != pNaviEvent )
    {
        switch(pNaviEvent->getFuncID())
        {
        case D_HMIAPI_EV_MODE_STATUS_FOCUSED:
        case D_HMIAPI_EV_MODE_LASTMODE_GETTED:
            HMI_LOG("Home : D_HMIAPI_EV_MODE_ACTIVEMODE_GETTED Get back model success !!");
            emit sig_getbackmodelv();
            emit sigUsbMusicReady();
            break;
        default:
            break;
        }
    }
}

void View_Home::init(void *info)
{
    if (NULL == info)
    {
        HMI_LOG("info is NULL !!!");
    }

    update_cursor_recycle();

    return;
}

void View_Home::touchEventProc(QEvent *e)
{
    int i_FuncID = 0;

    int i_mediaNum = 0;

    Frm_FuncEvent* pFuncEvent = dynamic_cast<Frm_FuncEvent*>(e);
    if( NULL != pFuncEvent )
    {
        i_FuncID = pFuncEvent->getFuncID();

        switch( i_FuncID )
        {
        case HMIVIEW_HOME_BTN_RELEASE:
            break;
        case HMIVIEW_HOME_BTN_CURSOR_MEDIA:
        case HMIVIEW_HOME_BTN_MEDIA:
        case HMIVIEW_HOME_BTN_NAVI:
            HMI_LOG("HMIVIEW_HOME_BTN_NAVI");
        case HMIVIEW_HOME_BTN_CURSOR_NAVI:
            HMI_LOG("HMIVIEW_HOME_BTN_CURSOR_NAVI");
            Frm_CommonClassIns->setHomecursor(2); //SX7-1148 zwb merge for SX5M-660
            break;

        case HMIVIEW_HOME_BTN_LINK:
        case HMIVIEW_HOME_BTN_CURSOR_LINK:
              Frm_CommonClassIns->setHomecursor(3); //SX7-1148 zwb merge for SX5M-660
            break;
        case HMIVIEW_HOME_BTN_PHONE_PRESS:
            break;
        case HMIVIEW_HOME_BTN_PHONE:
        case HMIVIEW_HOME_BTN_CURSOR_PHONE:
        case HMIVIEW_HOME_BTN_AIR:
        case HMIVIEW_HOME_BTN_CURSOR_AIR:
        case HMIVIEW_HOME_BTN_SETTING:
        case HMIVIEW_HOME_BTN_CURSOR_SETTING:

        case HMIVIEW_HOME_BTN_CANSETTING:


        case HMIVIEW_HOME_BTN_CURSOR_CANSETTING:


        case HMIVIEW_HOME_BTN_CHARGE:

            break;

        case HMIVIEW_HOME_BTN_CURSOR_CHARGE:


        case HMIVIEW_HOME_BTN_DVR:


        case HMIVIEW_HOME_BTN_CURSOR_DVR:

        case HMIVIEW_HOME_BTN_SEND2CAR:
            HMI_LOG("HMIVIEW_HOME_BTN_SEND2CAR");

        case HMIVIEW_HOME_BTN_CURSOR_SEND2CAR:
            HMI_LOG("HMIVIEW_HOME_BTN_CURSOR_SEND2CAR");

            break;
        case HMIVIEW_HOME_BTN_BACKMODEL_USB_MUSIC:
        case HMIVIEW_HOME_BTN_BACKMODEL_IPOD:
        case HMIVIEW_HOME_BTN_BACKMODEL_BT_MUSIC:
        case HMIVIEW_HOME_BTN_BACKMODEL_CARPLAY:
        case HMIVIEW_HOME_BTN_AIRMANAGESYSTEM:
        case HMIVIEW_HOME_BTN_CURSOR_AIRMANAGESYSTEM:
        case HMIVIEW_HOME_BTN_MESSAGEPUSH:
        case HMIVIEW_HOME_BTN_CURSOR_MESSAGEPUSH:
            break;
        case HMIVIEW_HOME_BTN_PERIPHERALSERVICE:
        case HMIVIEW_HOME_BTN_CURSOR_PERIPHERALSERVICE:
        case HMIVIEW_HOME_BTN_WEATHER:
        case HMIVIEW_HOME_BTN_CURSOR_WEATHER:

        case HMIVIEW_HOME_BTN_NEWS:
        case HMIVIEW_HOME_BTN_CURSOR_NEWS:

        case HMIVIEW_HOME_BTN_MAINTENANCE:
        case HMIVIEW_HOME_BTN_CURSOR_MAINTENANCE:
        default:
            break;
        }
    }
}
bool View_Home::concreteVisible()
{
    HMI_LOG("[HOME]");
    update_cursor_recycle();
    emit sig_getbackmodelv();
    emit sigAutoConIsExist();
    emit sigUsbMusicReady();
    emit sigCurrentStateReady();
    emit sigVehicelType();
//    emit sigCpsDeflectionChanged();
    emit sigLinkStatusChanged();
    emit sig_canNodeStatusDVRChange();
    emit sigChargebuttonStatus();
    //[add][by jia.jl][start]
    emit sig_TboxStatusChanged();
    //[add][by jia.jl][end]
    //ren air manage add start 0523
    emit sig_AirManageSystemStatusChanged();
	//ren air manage add end 0523
    return true;
}
bool View_Home::concreteInvisible()
{
    HMI_LOG("[HOME]");
    emit sig_home_invisible();
    return true;
}
void View_Home::Start(){
    m_imagecreatlist = 0;   //16603	SX7-2428-2
    return;
}
void View_Home::End(){
    m_imagecreatlist = 0;   //16603	SX7-2428-2
    return;
}

/******************************************************************************
Function Name : moveCursorTo
Description   : show the cursor on function button
Date          : 2015/09/02
Parameter     :
para1         :
Author        : Maotianliang(Neusoft)
******************************************************************************/
void View_Home::moveCursorTo( int i_cursorIndex)
{
    QObject* p_object = NULL;
    p_object = getViewObject();
    QVariant returnedValue;
    QVariant msg = i_cursorIndex;
    iCurosrIndex = i_cursorIndex;
    QMetaObject::invokeMethod(p_object, "changeCursorIndex",Q_RETURN_ARG(QVariant, returnedValue),Q_ARG(QVariant, msg));

}
/******************************************************************************
Function Name : TabBtnFoucsed
Description   : hide the cursor on function button
Date          : 2015/09/02
Parameter     :
para1         :
Author        : Maotianliang(Neusoft)
******************************************************************************/
void View_Home::hideCursorFrom()
{
    QObject* p_object = NULL;
    p_object = getViewObject();
    QVariant returnedValue;
    QVariant msg = -1;
    iCurosrIndex = -1;
    QMetaObject::invokeMethod(p_object, "changeCursorIndex",Q_RETURN_ARG(QVariant, returnedValue),Q_ARG(QVariant, msg));
}
/******************************************************************************
Function Name : TabBtnFoucsed
Description   : show the cursor on tab button
Date          : 2015/09/02
Parameter     :
para1         :
Author        : Maotianliang(Neusoft)
******************************************************************************/
void View_Home::TabBtnFoucsed(CursorTypeEnum en_type, int i_pos)
{
    HMI_LOG("en_type = %d i_pos = %d",en_type,i_pos);
    return;
}
/******************************************************************************
Function Name : TabBtnUnfoucsed
Description   : hide the cursor on tab button
Date          : 2015/09/02
Parameter     :
para1         :
Author        : Maotianliang(Neusoft)
******************************************************************************/
void View_Home::TabBtnUnfoucsed(CursorTypeEnum en_type)
{
    HMI_LOG("en_type = %d",en_type);
    return;
}
/******************************************************************************
Function Name : get_totalNum
Description   : get total num of list
Date          : 2015/09/02
Parameter     :
para1         :
Author        : Maotianliang(Neusoft)
******************************************************************************/
int View_Home::get_totalNum()
{
    return 0;
}
/******************************************************************************
Function Name : get_totalNumOfTabButton
Description   :
Date          : 2015/09/02
Parameter     :
para1         :
Author        : Maotianliang(Neusoft)
******************************************************************************/
int View_Home::get_totalNumOfTabButton()
{
    return 0;
}
/******************************************************************************
Function Name : get_curStartIndex
Description   : get the start index of current page
Date          : 2015/09/02
Parameter     :
para1         :
Author        : Maotianliang(Neusoft)
******************************************************************************/
int View_Home::get_curStartIndex()
{
    return 0;
}

/******************************************************************************
Function Name : get_btn_sta_by_index
Description   : get the start index of current page
Date          : 2015/09/02
Parameter     :
para1         :
Author        : Maotianliang(Neusoft)
******************************************************************************/
int View_Home::get_btn_sta_by_index(CursorTypeEnum en_type,int index)
{
    int i_ret = 0;
    HMI_LOG("i_ret = %d", i_ret);
    return i_ret;
}

/******************************************************************************
Function Name : hardkeyEventProc
Description   : Hardkey proc
Date          : 2015/09/02
Parameter     :
para1         : QEvent *e
Author        : Maotianliang(Neusoft)
******************************************************************************/
bool View_Home::hardKeyEventProc(QEvent *e)
{
    bool b_ret = false;
    Frm_KeyEvent* fHKEvent = dynamic_cast<Frm_KeyEvent*>(e);
    CursorstatusEnum en_status = EN_CURSORCTRL_STATUS_DISAPPEAR;

    if( NULL != fHKEvent )
    {
        switch(fHKEvent->getFuncID())
        {
        case KEYEVENT_ENCODER_LEFT:
        {
            HMI_LOG("View_Home::KEYEVENT_ENCODER_LEFT");
            pCursorCtrl->updateListCursr(EN_CURSORCTRL_OPE_TYPE_LEFT);
            break;
        }
        case KEYEVENT_ENCODER_RIGHT:
        {
            pCursorCtrl->updateListCursr(EN_CURSORCTRL_OPE_TYPE_RIGHT);
            break;
        }
        case KEYEVENT_ENCODER_PRESS:
            pCursorCtrl->KnobPressed();
            //emit sig_mouse_mid_Press();
            knobPressAvtivePress();
            HMI_LOG("View_Home::KEYEVENT_ENCODER_PRESS");
            break;
        case KEYEVENT_ENCODER_RELEASE:
            pCursorCtrl->KnobReleased();
            //emit sig_mouse_mid_Releas();
            knobPressAvtiveRelease();
            Frm_CommonClassIns->getCursorStatus(&en_status);
            if ((EN_CURSORCTRL_STATUS_HIDE == en_status)||(EN_CURSORCTRL_STATUS_DISAPPEAR == en_status))
            {
                pCursorCtrl->updateListCursr(EN_CURSORCTRL_OPE_TYPE_RIGHT);
            }
            HMI_LOG("View_Home::KEYEVENT_ENCODER_RELEASE");
            break;
        case KEYEVENT_HOME_RELEASE:
            break;

        default:
            break;
        }
    }
    return b_ret;
}

/******************************************************************************
Function Name : knobPressAvtive
Description   :
Date          : 2015/09/02
Parameter     :
para1         :
Author        : Maotianliang(Neusoft)
******************************************************************************/
void View_Home::knobPressAvtivePress()
{

}

void View_Home::knobPressAvtiveRelease()
{
    CursorTypeEnum en_type = EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR;
    int i_curpos = 0;
    QObject* p_object = NULL;
    QVariant returnedValue = 0;
    QVariant msg = 0;

    en_type = pCursorCtrl->getCurosrType();
    i_curpos = pCursorCtrl->getCurosrPostion_Abs();

    switch(en_type)
    {
    case EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN:
        p_object = getViewObject();
        msg = i_curpos;
        QMetaObject::invokeMethod(p_object, "knobReleaseOnFunc",Q_RETURN_ARG(QVariant, returnedValue),Q_ARG(QVariant, msg));
        break;

    default:
        break;
    }
}

void View_Home::update_cursor_recycle()
{

    CursorTypeEnum en_type = EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR;
    int ipos = 0;
    memset(&st_pageDate, 0, sizeof(PageDate_t));

    Frm_CommonClassIns->getCursorType(&en_type, &ipos);
    HMI_LOG("en_type = %d",en_type);
    if (EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR == en_type)
    {
        st_pageDate.en_initstatus = EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR;
        ipos = 0;
    }
    else
    {
        st_pageDate.en_initstatus = EN_CURSORCTRL_CURSOR_TYPE_FUNC_BTN;
        if( -2 == iCurosrIndex )
        {
            HMI_LOG("iCurosrIndex = %d",iCurosrIndex);
            ipos = Frm_CommonClassIns->getHomecursor();
        }
        else
        {
            ipos = iCurosrIndex;
        }
        HMI_LOG("ipos = %d",ipos);
    }
    st_pageDate.icursorPos = ipos;
    st_pageDate.ilayerCount = 1;
    st_pageDate.i_itemPerPage = 8;
    st_pageDate.i_returnBtnExist = 0;

    pCursorCtrl->initCursor(&st_pageDate);

    return;
}

