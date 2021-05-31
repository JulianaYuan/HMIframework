/*******************************************************
 File Name   : hmifrm_common.cpp

 Created by  :  Shi.Jianhang(Neusoft)

 Created Date:  2015/07/15

Description   :   framework common code
*********************************************************/

/****************Include  Section Begin***************************************************/

#include "../inc/hmifrm_common.h"
#include "hmimode_home.h"
#include "../inc/hmifrm_apiclass.h"
#include "hmimode_common.h"
#include "hmilib_statusbar.h"
#include "hmiapi_audiomanager.h"
#include "hmifrm_keyclass.h"


/****************Include  Section End***************************************************/

Frm_CommonClass* Frm_CommonClass::m_instance = NULL;

const Frm_CommonClass::S_MODECLASS_TABLE ModeClassTable[] =
{
    {"Common",                     EN_VIEW_MODE_NULL,                  SX5_MODE_MAX,       0,   (void*)(Mode_Common::constructer),      NULL},
    {"HOME",                       EN_VIEW_MODE_HOME,                  SX5_MODE_MAX,       1,   (void*)(Mode_Home::constructer),        NULL},
};

/******************************************************************************
Function Name : Frm_CommonClass
Description   : constructor
Date          : 2015/07/15
Parameter     : QObject* parent
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_CommonClass::Frm_CommonClass(QObject* parent)
    : QObject(parent)
{
    m_ModeClassTable = (S_MODECLASS_TABLE*)(&ModeClassTable);
    ModeClassTableNum = sizeof(ModeClassTable) / sizeof(S_MODECLASS_TABLE);

    m_enCursorType = EN_CURSORCTRL_CURSOR_TYPE_NO_CURSOR;
    m_enCursorstatus = EN_CURSORCTRL_STATUS_DISAPPEAR;
    m_ipos = 0;
    m_home_pos = 0;

    modeClass = NULL;
    i_isboot = 0;
    isLoadingInterrupt = 0;

    m_knobpressflag = 0;
    m_mesaage_shown.clear();
    m_Popup_queue.clear();
    m_transModeId = EN_VIEW_MODE_NULL;
    i_avmtimer = 0;
    i_usbimer = 0;  //13720	SX5-5696
    m_touchbeforeaccoff = 0;
    m_touchpermitstatus = 0;
}

/******************************************************************************
Function Name : init
Description   : Init framework class
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_CommonClass::init()
{
#ifndef HMI_API_STUB
    hmi_dlt_log_init();
#endif
    initCommonMode();

    //new Frm_ApiClass(this);
    m_ApiClass = new Frm_ApiClass(this);
    m_ApiClass->init(this);
    m_ApiClass->start();

    m_ModeManager.Init_AudioSource();

    Frm_KeyClassIns->init(this);
    initMainQML();
}

/******************************************************************************
Function Name : getInstance
Description   : Get Instance
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_CommonClass* Frm_CommonClass::getInstance()
{
    if( !m_instance )
    {
        m_instance = new Frm_CommonClass();
    }

    return m_instance;
}

/******************************************************************************
Function Name : customEvent
Description   : Receive event
Date          : 2015/07/15
Parameter     : QEvent *e
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_CommonClass::customEvent(QEvent *e)
{
    CPandaDbusEvent *dbus_event = dynamic_cast<CPandaDbusEvent *>(e);
    if (NULL != dbus_event)
    {
        HMI_LOG("Start to process dbus event");
        dbus_event->relay();
        return;
    }

    HMI_LOG("Receive non-dbus Event");
    Frm_AbstractEvent* fAbstractEvent = dynamic_cast<Frm_AbstractEvent*>(e);
    //Frm_ModeEvent* p_modeOffEvent = NULL;

    if( NULL != fAbstractEvent )
    {
        switch( fAbstractEvent->getEventType() )
        {
        case Frm_AbstractEvent::EN_TYPE_MODETRANS:
        {
            HMI_LOG("Frm_AbstractEvent::EN_TYPE_MODETRANS");
            Frm_ModeEvent* fModeEvent = dynamic_cast<Frm_ModeEvent*>((e));
            int i_mode = fModeEvent->getModeID();
            if(true == m_ModeManager.isReqCurrentMode(e))
            {
                HMI_LOG("Not need to change mode");
                HMI_LOG("isReqCurrentMode = true  i_mode = %d", i_mode);
                HMI_LOG("modeTrans");
                modeTrans(e);
            }
            else
            {
                HMI_LOG("req to change mode");
                m_ModeManager.reqModeTrans(e);
                modeTrans(e);
            }
        }
            break;
        case Frm_AbstractEvent::EN_TYPE_REQTRANS:
            HMI_LOG("Frm_AbstractEvent::EN_TYPE_REQTRANS");
            screenTrans(e);
            break;
        case Frm_AbstractEvent::EN_TYPE_APPTOHMI:
            HMI_LOG("Frm_AbstractEvent::EN_TYPE_APPTOHMI");
            customEvProc(e);
            break;
        case Frm_AbstractEvent::EN_TYPE_KEYEVENT:
            HMI_LOG("Frm_AbstractEvent::EN_TYPE_KEYEVENT");
            special_key_filter(e);
            customEvProc(e);
            break;
        case Frm_AbstractEvent::EN_TYPE_AUDIOMODE:
            HMI_LOG("Frm_AbstractEvent::EN_TYPE_AUDIOMODE");
            m_ModeManager.customEvProc(e);
            customEvProc(e);
            break;
        default:
            HMI_LOG("Frm_AbstractEvent::default");
            customEvProc(e);
            break;
        }
    }
}

/******************************************************************************
Function Name : ModeClassPopup
Description   : Popup mode
Date          : 2015/07/15
Parameter     : quint16 modeid
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
int Frm_CommonClass::ModeClassPopup(quint16 modeid)
{
    QList<Frm_AbstractModeClass*> allModeClass = this->findChildren<Frm_AbstractModeClass*>();

    int i=0;
    int icount = allModeClass.count();

    for(i=icount-1; i>=0; i--)
    {
        Frm_AbstractModeClass* pModeClass = allModeClass.at(i);

        if (modeid == pModeClass->getmodeID())
        {
            break;
        }

        if(NULL != pModeClass)
        {
            if (MODE_ON_PERCENT_100 == pModeClass->getmodeStatus())
            {
                pModeClass->invisible();
            }

            pModeClass->End();

            delete pModeClass;
        }
    }

    return 0;
}

/******************************************************************************
Function Name : ModeClassPopup
Description   : Popup mode
Date          : 2015/07/15
Parameter     : quint16 modeid
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_CommonClass::initCommonMode()
{
    Frm_AbstractModeClass* pmodeclass = NULL;

    pmodeclass = (Frm_AbstractModeClass*)Mode_Common::constructer(
                this, this, 0, 0, m_ModeClassTable[0].modeAttr);

    pmodeclass->Start();

    pmodeclass->setmodeStatus(MODE_ON_PERCENT_0);

    CmnmodeClass = pmodeclass;

    return;
}

/******************************************************************************
Function Name : initMainQML
Description   : Init QML
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_CommonClass::initMainQML()
{
    //main_qmlView = new QtQuick2ApplicationViewer();
   // main_qmlView->rootContext()->setContextProperty("Framework", this);
    //main_qmlView->installEventFilter(this);
    //qmlRegisterType<Ctrl_Common_Bg>("Ctrl_Common_Bg", 1, 0, "Ctrl_Common_Bg");
    //main_qmlView->setSource(QUrl("qrc:/qml/main.qml"));
    //main_qmlView->setColor(QColor(Qt::transparent));
    //main_qmlView->engine()->addImageProvider("myImageProvider", EntertainmentClassIns->get_UsbImageProvider());
    ShowMainQMLView();
    /*
    qmlRegisterType<Ctrl_Radio_Play>("FImportViewRadioPlay", 1, 0, "FImportViewRadioPlay");
    qmlRegisterType<Ctrl_Home>("Ctrl_Home", 1, 0, "Ctrl_Home");
    qmlRegisterType<Ctrl_Radio_PlayList_FM>("Ctrl_Radio_PlayList_FM", 1, 0, "Ctrl_Radio_PlayList_FM");
    qmlRegisterType<Ctrl_Radio_PlayList_AM>("Ctrl_Radio_PlayList_AM", 1, 0, "Ctrl_Radio_PlayList_AM");
    qmlRegisterType<Ctrl_Radio_FavoriteList>("FImportviewFavoriteList", 1, 0, "FImportviewFavoriteList");
    qmlRegisterType<Ctrl_CanSetting_VehicleInfo>("CtrlCanSettingVehicleInfo", 1, 0, "CtrlCanSettingVehicleInfo");
    qmlRegisterType<Ctrl_CanSetting_WarningVoice>("CtrlCanSettingWarningVoice", 1, 0, "CtrlCanSettingWarningVoice");
    qmlRegisterType<Ctrl_CanSetting_VoiceSpeed>("CtrlCanSettingVoiceSpeed", 1, 0, "CtrlCanSettingVoiceSpeed");
    qmlRegisterType<Ctrl_CanSetting_AlarmSpeed>("CtrlCanSettingAlarmSpeed", 1, 0, "CtrlCanSettingAlarmSpeed");
    qmlRegisterType<Ctrl_CanSetting_VehicleSafety>("CtrlCanSetting_VehicleSafty", 1, 0, "CtrlCanSetting_VehicleSafty");
    qmlRegisterType<Ctrl_Usbv_Play>("Ctrl_Usbv_Play", 1, 0, "Ctrl_Usbv_Play");
    qmlRegisterType<Ctrl_Usbv_FileList>("Ctrl_Usbv_FileList", 1, 0, "Ctrl_Usbv_FileList");
    qmlRegisterType<Ctrl_Usbm_Play>("Ctrl_Usbm_Play", 1, 0, "Ctrl_Usbm_Play");
    qmlRegisterType<Ctrl_Usbm_FileList>("Ctrl_Usbm_FileList", 1, 0, "Ctrl_Usbm_FileList");
    qmlRegisterType<Ctrl_Usbm_FolderList>("Ctrl_Usbm_FolderList", 1, 0, "Ctrl_Usbm_FolderList");
    qmlRegisterType<Ctrl_Usbm_ArtistList>("Ctrl_Usbm_ArtistList", 1, 0, "Ctrl_Usbm_ArtistList");
    qmlRegisterType<Ctrl_Usbm_AlbumList>("Ctrl_Usbm_AlbumList", 1, 0, "Ctrl_Usbm_AlbumList");
    qmlRegisterType<Ctrl_Engineer_Main>("CtrlEngineerMain", 1, 0, "CtrlEngineerMain");
    qmlRegisterType<Ctrl_Setting_Sound>("CtrlSettingSound", 1, 0, "CtrlSettingSound");
    qmlRegisterType<Ctrl_Ipod_Play>("CtrlIpodPlay", 1, 0, "CtrlIpodPlay");
    qmlRegisterType<Ctrl_Media_main>("Ctrl_Media_main", 1, 0, "Ctrl_Media_main");

    qmlRegisterType<Ctrl_Weather_CS>("Ctrl_Weather_CS", 1, 0, "Ctrl_Weather_CS");
    qmlRegisterType<Ctrl_Weather_Main>("Ctrl_Weather_Main", 1, 0, "Ctrl_Weather_Main");
    qmlRegisterType<Ctrl_Message_Detail>("Ctrl_Message_Detail", 1, 0, "Ctrl_Message_Detail");
    qmlRegisterType<Ctrl_Message_Send>("Ctrl_Message_Send", 1, 0, "Ctrl_Message_Send");
    qmlRegisterType<Ctrl_Message_nosend>("Ctrl_Message_nosend", 1, 0, "Ctrl_Message_nosend");   // add by yulei
    //zzf end
    qmlRegisterType<Lib_Ctrl_Statusbar>("Lib_Ctrl_Statusbar", 1, 0, "Lib_Ctrl_Statusbar");
    qmlRegisterType<Ctrl_Ipod_Album>("CtrlIpodAlbum", 1, 0, "CtrlIpodAlbum");
    qmlRegisterType<Ctrl_Ipod_AllSong>("CtrlIpodAllSong", 1, 0, "CtrlIpodAllSong");
    qmlRegisterType<Ctrl_Ipod_Artist>("CtrlIpodArtist", 1, 0, "CtrlIpodArtist");
    qmlRegisterType<Ctrl_Ipod_AudioBook>("CtrlIpodAudioBook", 1, 0, "CtrlIpodAudioBook");
    qmlRegisterType<Ctrl_Ipod_Radio>("Ctrl_Ipod_Radio", 1, 0, "Ctrl_Ipod_Radio");
    qmlRegisterType<Ctrl_Ipod_Composer>("CtrlIpodComposer", 1, 0, "CtrlIpodComposer");
    qmlRegisterType<Ctrl_Ipod_Genre>("CtrlIpodGenre", 1, 0, "CtrlIpodGenre");
    qmlRegisterType<Ctrl_Ipod_PlayList>("CtrlIpodPlayList", 1, 0, "CtrlIpodPlayList");
    qmlRegisterType<Ctrl_Ipod_Podcast>("CtrlIpodPodcast", 1, 0, "CtrlIpodPodcast");
    qmlRegisterType<Ctrl_CanSetting_ActiveSafety>("CtrlCanSettingActiveSafety", 1, 0, "CtrlCanSettingActiveSafety");
    qmlRegisterType<Ctrl_CanSetting_CarBodyCtrl>("CtrlCanSettingCarBodyCtrl", 1, 0, "CtrlCanSettingCarBodyCtrl");
    qmlRegisterType<Ctrl_CanSetting_DriveAssistance>("CtrlCanSettingDriveAssistance", 1, 0, "CtrlCanSettingDriveAssistance");
    qmlRegisterType<Ctrl_CanSetting_IntelComfort>("CtrlCanSettingIntelComfort", 1, 0, "CtrlCanSettingIntelComfort");
	//ren add start
    qmlRegisterType<Ctrl_CanSetting_CarBodyAccessory>("CtrlCanSettingCarBodyAccessory", 1, 0, "CtrlCanSettingCarBodyAccessory");
	//ren add end
    qmlRegisterType<Ctrl_CanSetting_Opm>("CtrlCanSettingOpm", 1, 0, "CtrlCanSettingOpm");
    qmlRegisterType<Ctrl_Usb_Loading>("Ctrl_Usb_Loading", 1, 0, "Ctrl_Usb_Loading");
    qmlRegisterType<Ctrl_Usb_LoadFail>("Ctrl_Usb_LoadFail", 1, 0, "Ctrl_Usb_LoadFail");
    qmlRegisterType<Ctrl_Image_show>("Ctrl_Image_show",1,0,"Ctrl_Image_show");
    qmlRegisterType<Ctrl_usbi_list>("Ctrl_usbi_list", 1, 0, "Ctrl_usbi_list");
    qmlRegisterType<Ctrl_SWDL_UpgradeInfo>("Ctrl_SWDL_UpgradeInfo", 1, 0, "Ctrl_SWDL_UpgradeInfo");
    qmlRegisterType<Ctrl_SWDL_UpgradeSelect>("Ctrl_SWDL_UpgradeSelect", 1, 0, "Ctrl_SWDL_UpgradeSelect");
    qmlRegisterType<Ctrl_Media_Usb>("Ctrl_Media_Usb", 1, 0, "Ctrl_Media_Usb");
    qmlRegisterType<Ctrl_Phone_CallBook>("Ctrl_Phone_CallBook", 1, 0, "Ctrl_Phone_CallBook");
    qmlRegisterType<Ctrl_Phone_RecentCall>("Ctrl_Phone_RecentCall", 1, 0, "Ctrl_Phone_RecentCall");
    qmlRegisterType<Ctrl_Phone_Index>("Ctrl_Phone_Index", 1, 0, "Ctrl_Phone_Index");
    qmlRegisterType<Ctrl_Phone_Detail>("Ctrl_Phone_Detail", 1, 0, "Ctrl_Phone_Detail");
    qmlRegisterType<Ctrl_Phone_Setting>("Ctrl_Phone_Setting", 1, 0, "Ctrl_Phone_Setting");
    qmlRegisterType<Ctrl_Phone_Dial>("Ctrl_Phone_Dial", 1, 0, "Ctrl_Phone_Dial");
    qmlRegisterType<Ctrl_Poweroff>("Ctrl_Poweroff", 1, 0, "Ctrl_Poweroff");
    qmlRegisterType<Ctrl_Setting_Main>("Ctrl_Setting_Main", 1, 0,"Ctrl_Setting_Main");
    qmlRegisterType<Ctrl_setting_time>("Ctrl_setting_time", 1 ,0,"Ctrl_setting_time");
    qmlRegisterType<Ctrl_setting_display>("Ctrl_setting_display", 1 ,0,"Ctrl_setting_display");
    qmlRegisterType<Ctrl_BtSettings_DeviceList>("Ctrl_BtSettings_DeviceList", 1, 0, "Ctrl_BtSettings_DeviceList");
    qmlRegisterType<Ctrl_BtSettings_Main>("Ctrl_BtSettings_Main", 1, 0, "Ctrl_BtSettings_Main");
    qmlRegisterType<Ctrl_BtSettings_UserSet>("Ctrl_BtSettings_UserSet", 1, 0, "Ctrl_BtSettings_UserSet");
    qmlRegisterType<Ctrl_BTmusic>("Ctrl_BTmusic",1,0,"Ctrl_BTmusic");
    qmlRegisterType<Ctrl_Incomingcall>("Ctrl_Incomingcall", 1, 0, "Ctrl_Incomingcall");
    qmlRegisterType<Ctrl_Talking>("Ctrl_Talking", 1, 0, "Ctrl_Talking");
    qmlRegisterType<Ctrl_Link_Nodevice>("Ctrl_Link_Nodevice", 1, 0, "Ctrl_Link_Nodevice");
    qmlRegisterType<Ctrl_Link_Usbconfail>("Ctrl_Link_Usbconfail", 1, 0, "Ctrl_Link_Usbconfail");
    qmlRegisterType<Ctrl_Link_Usbconnotice>("Ctrl_Link_Usbconnotice", 1, 0, "Ctrl_Link_Usbconnotice");
    qmlRegisterType<Ctrl_Link_Usbconneting>("Ctrl_Link_Usbconneting", 1, 0, "Ctrl_Link_Usbconneting");
    qmlRegisterType<Ctrl_Link_Wificonfail>("Ctrl_Link_Wificonfail", 1, 0, "Ctrl_Link_Wificonfail");
    qmlRegisterType<Ctrl_Link_Wificonnotice>("Ctrl_Link_Wificonnotice", 1, 0, "Ctrl_Link_Wificonnotice");
    qmlRegisterType<Ctrl_Link_Wificonneting>("Ctrl_Link_Wificonneting", 1, 0, "Ctrl_Link_Wificonneting");
    qmlRegisterType<Ctrl_Link_AppConfirm>("Ctrl_Link_AppConfirm", 1, 0, "Ctrl_Link_AppConfirm");
    qmlRegisterType<Ctrl_Link_AppInstall>("Ctrl_Link_AppInstall", 1, 0, "Ctrl_Link_AppInstall");
    qmlRegisterType<Ctrl_Link_AppInsatallFail>("Ctrl_Link_AppInsatallFail", 1, 0, "Ctrl_Link_AppInsatallFail");
    qmlRegisterType<Ctrl_Link_AppNotOpen>("Ctrl_Link_AppNotOpen", 1, 0, "Ctrl_Link_AppNotOpen");
    qmlRegisterType<Ctrl_Link_MirrorMain>("Ctrl_Link_MirrorMain", 1, 0, "Ctrl_Link_MirrorMain");
    qmlRegisterType<Ctrl_Link_MirrorRun>("Ctrl_Link_MirrorRun", 1, 0, "Ctrl_Link_MirrorRun");
    qmlRegisterType<Ctrl_Link_MirrorStop>("Ctrl_Link_MirrorStop", 1, 0, "Ctrl_Link_MirrorStop");
    qmlRegisterType<Ctrl_Link_SafeNotice01>("Ctrl_Link_SafeNotice01", 1, 0, "Ctrl_Link_SafeNotice01");
    qmlRegisterType<Ctrl_Link_SafeNotice02>("Ctrl_Link_SafeNotice02", 1, 0, "Ctrl_Link_SafeNotice02");
	qmlRegisterType<Ctrl_link_bluetoothwarn>("Ctrl_link_bluetoothwarn", 1, 0, "Ctrl_link_bluetoothwarn");
    qmlRegisterType<Ctrl_Link_CarplayConnect>("Ctrl_Link_CarplayConnect", 1, 0, "Ctrl_Link_CarplayConnect");
    qmlRegisterType<Ctrl_Link_CarplayInsert>("Ctrl_Link_CarplayInsert", 1, 0, "Ctrl_Link_CarplayInsert");
    qmlRegisterType<Ctrl_Usbv_Setting>("Ctrl_Usbv_Setting", 1, 0, "Ctrl_Usbv_Setting");
    qmlRegisterType<Ctrl_WifiSettings_Main>("Ctrl_WifiSettings_Main", 1, 0, "Ctrl_WifiSettings_Main");
    qmlRegisterType<Ctrl_WifiSettings_WifiList>("Ctrl_WifiSettings_WifiList", 1, 0, "Ctrl_WifiSettings_WifiList");
    qmlRegisterType<Ctrl_WifiSettings_ConnectedList>("Ctrl_WifiSettings_ConnectedList", 1, 0, "Ctrl_WifiSettings_ConnectedList");
    qmlRegisterType<Ctrl_WifiSettings_Wifiinfo>("Ctrl_WifiSettings_Wifiinfo", 1, 0, "Ctrl_WifiSettings_Wifiinfo");
    qmlRegisterType<Ctrl_VR_Main>("Ctrl_VR_Main", 1, 0, "Ctrl_VR_Main");
    qmlRegisterType<Ctrl_VR_PickList>("Ctrl_VR_PickList", 1, 0, "Ctrl_VR_PickList");
    qmlRegisterType<Ctrl_VR_Phone>("Ctrl_VR_Phone", 1, 0, "Ctrl_VR_Phone");
    qmlRegisterType<Ctrl_VR_Fault>("Ctrl_VR_Fault", 1, 0, "Ctrl_VR_Fault");
    qmlRegisterType<Ctrl_VR_Confirm>("Ctrl_VR_Confirm", 1, 0, "Ctrl_VR_Confirm");
    qmlRegisterType<Ctrl_Air_Condition>("Ctrl_Air_Condition", 1, 0, "Ctrl_Air_Condition");
    qmlRegisterType<Ctrl_Air_Prompt>("Ctrl_Air_Prompt", 1, 0, "Ctrl_Air_Prompt");
	//ren add start
    qmlRegisterType<Ctrl_Avm_AutoParking>("Ctrl_Avm_AutoParking", 1, 0, "Ctrl_Avm_AutoParking");
	//ren add end
    qmlRegisterType<Ctrl_PowerMode>("Ctrl_PowerMode",1,0,"Ctrl_PowerMode");
    qmlRegisterType<Ctrl_Engineer_Calibration>("CtrlEngineerCalibration", 1, 0, "CtrlEngineerCalibration");
    qmlRegisterType<Ctrl_Usbv_Forbid>("CtrlUsbvForbid", 1, 0, "CtrlUsbvForbid");
    qmlRegisterType<Ctrl_Maintenance_VehicleInfo>("CtrlMaintenanceVehicleInfo", 1, 0, "CtrlMaintenanceVehicleInfo");
    qmlRegisterType<Ctrl_Maintenance_MaintainRemind>("CtrlMaintenanceMaintainRemind", 1, 0, "CtrlMaintenanceMaintainRemind");
    qmlRegisterType<Ctrl_Maintenance_OilChange>("CtrlMaintenanceOilChange", 1, 0, "CtrlMaintenanceOilChange");
    qmlRegisterType<Ctrl_Maintenance_TireChange>("CtrlMaintenanceTireChange", 1, 0, "CtrlMaintenanceTireChange");
    qmlRegisterType<Ctrl_Link_MirrorPhoneLock>("Ctrl_Link_MirrorPhoneLock", 1, 0, "Ctrl_Link_MirrorPhoneLock");
    qmlRegisterType<Ctrl_Pdc_Ons>("Ctrl_Pdc_Ons", 1, 0, "Ctrl_Pdc_Ons");
    qmlRegisterType<Ctrl_Maintenance_MaintainPrompt>("CtrlMaintenanceMaintainPrompt", 1, 0, "CtrlMaintenanceMaintainPrompt");
    qmlRegisterType<Ctrl_Maintenance_TirePrompt>("CtrlMaintenanceTirePrompt", 1, 0, "CtrlMaintenanceTirePrompt");
    qmlRegisterType<Ctrl_Maintenance_OilPrompt>("CtrlMaintenanceOilPrompt", 1, 0, "CtrlMaintenanceOilPrompt");
    qmlRegisterType<Ctrl_Maintenance_MaintainPromptSet>("CtrlMaintenanceMaintainPromptSet", 1, 0, "CtrlMaintenanceMaintainPromptSet");
    qmlRegisterType<Ctrl_Maintenance_TirePromptSet>("CtrlMaintenanceTirePromptSet", 1, 0, "CtrlMaintenanceTirePromptSet");
    qmlRegisterType<Ctrl_Maintenance_OilPromptSet>("CtrlMaintenanceOilPromptSet", 1, 0, "CtrlMaintenanceOilPromptSet");
    qmlRegisterType<Ctrl_Maintenance_MileSigLose>("CtrlMaintenanceMileSigLose", 1, 0, "CtrlMaintenanceMileSigLose");
    qmlRegisterType<Ctrl_Maintenance_MileSigReAction>("CtrlMaintenanceMileSigReAction", 1, 0, "CtrlMaintenanceMileSigReAction");
    qmlRegisterType<Ctrl_NaviStart>("Ctrl_NaviStart", 1, 0, "Ctrl_NaviStart");
    qmlRegisterType<Ctrl_Navi_Tbt>("Ctrl_Navi_Tbt", 1, 0, "Ctrl_Navi_Tbt");
    qmlRegisterType<Ctrl_ChargeMain>("Ctrl_ChargeMain", 1, 0, "Ctrl_ChargeMain");
    qmlRegisterType<Ctrl_ChargeModeselect>("Ctrl_ChargeModeselect", 1, 0, "Ctrl_ChargeModeselect");
	qmlRegisterType<Ctrl_Factory_Mode>("Ctrl_Factory_Mode", 1, 0, "Ctrl_Factory_Mode");
    qmlRegisterType<Ctrl_Factory_Confirm>("Ctrl_Factory_Confirm", 1, 0, "Ctrl_Factory_Confirm");
	qmlRegisterType<Ctrl_SettingSystemhelp>("Ctrl_SettingSystemhelp", 1, 0, "Ctrl_SettingSystemhelp");
    qmlRegisterType<Ctrl_Systemdetail>("Ctrl_Systemdetail", 1, 0, "Ctrl_Systemdetail");
	qmlRegisterType<Ctrl_ChargeStartmode>("Ctrl_ChargeStartmode", 1, 0, "Ctrl_ChargeStartmode");
    qmlRegisterType<Ctrl_ChargeStopmode>("Ctrl_ChargeStopmode", 1, 0, "Ctrl_ChargeStopmode");
    qmlRegisterType<Ctrl_ChargeStartstopmode>("Ctrl_ChargeStartstopmode", 1, 0, "Ctrl_ChargeStartstopmode");
    qmlRegisterType<Ctrl_ChargeSaveok>("Ctrl_ChargeSaveok", 1, 0, "Ctrl_ChargeSaveok");
    qmlRegisterType<Ctrl_Charge_Charging>("Ctrl_Charge_Charging", 1, 0, "Ctrl_Charge_Charging");
    qmlRegisterType<Ctrl_ChargeCancel>("Ctrl_ChargeCancel", 1, 0, "Ctrl_ChargeCancel");
    qmlRegisterType<Ctrl_ChargeCancelok>("Ctrl_ChargeCancelok", 1, 0, "Ctrl_ChargeCancelok");
    qmlRegisterType<Ctrl_ChargeReservationok>("Ctrl_ChargeReservationok", 1, 0, "Ctrl_ChargeReservationok");
    qmlRegisterType<Ctrl_ChargeToStop>("Ctrl_ChargeToStop", 1, 0, "Ctrl_ChargeToStop");
    qmlRegisterType<Ctrl_Weather_Update_Fail>("Ctrl_Weather_Update_Fail", 1, 0, "Ctrl_Weather_Update_Fail");      //add by yulei
    qmlRegisterType<Ctrl_Weather_Gps_Weak>("Ctrl_Weather_Gps_Weak", 1, 0, "Ctrl_Weather_Gps_Weak");               //add by yulei
    qmlRegisterType<Ctrl_MaintenRemind_Reminder>("Ctrl_MaintenRemind_Reminder", 1, 0, "Ctrl_MaintenRemind_Reminder");
	qmlRegisterType<Ctrl_News_Main>("Ctrl_News_Main", 1, 0, "Ctrl_News_Main"); 			//zhang.yy
    //qmlRegisterType<Ctrl_maintenance_appointmentcommitconfirm>("Ctrl_maintenance_appointmentcommitconfirm", 1, 0, "Ctrl_maintenance_appointmentcommitconfirm");   //nan
    qmlRegisterType<Ctrl_MaintenanceAppointment_PhoneAndDate>("Ctrl_MaintenanceAppointment_PhoneAndDate", 1, 0, "Ctrl_MaintenanceAppointment_PhoneAndDate");  //luzhijun
    //qmlRegisterType<Ctrl_MaintenanceAppointment_4ssearch>("Ctrl_MaintenanceAppointment_4ssearch", 1, 0, "Ctrl_MaintenanceAppointment_4ssearch");   //yulei
    qmlRegisterType<Ctrl_MaintenanceAppointment_4sList>("Ctrl_MaintenanceAppointment_4sList", 1, 0, "Ctrl_MaintenanceAppointment_4sList");        //yulei
    //qmlRegisterType<Ctrl_MaintenanceAppointmentsearch4sng>("Ctrl_MaintenanceAppointmentsearch4sng", 1, 0, "Ctrl_MaintenanceAppointmentsearch4sng");        //yulei
    qmlRegisterType<Ctrl_news_order>("Ctrl_news_order", 1, 0, "Ctrl_news_order");
    qmlRegisterType<Ctrl_News_WholeChanList_Getting>("Ctrl_News_WholeChanList_Getting", 1, 0, "Ctrl_News_WholeChanList_Getting");              //add by panmm
    qmlRegisterType<Ctrl_News_WholeChanList_GetFailure>("Ctrl_News_WholeChanList_GetFailure", 1, 0, "Ctrl_News_WholeChanList_GetFailure");              //add by panmm
    qmlRegisterType<Ctrl_News_Detail>("Ctrl_News_Detail", 1, 0, "Ctrl_News_Detail");
	qmlRegisterType<Ctrl_News_Detail_Getting>("Ctrl_News_Detail_Getting", 1, 0, "Ctrl_News_Detail_Getting");
    qmlRegisterType<Ctrl_News_Detail_GettingFail>("Ctrl_News_Detail_GettingFail", 1, 0, "Ctrl_News_Detail_GettingFail");
	qmlRegisterType<Ctrl_Alert_FaultAlert>("Ctrl_Alert_FaultAlert", 1, 0, "Ctrl_Alert_FaultAlert");		//[add by jia.jl]
	//ren air manage add start 0523
    qmlRegisterType<Ctrl_Air_Manage_System>("Ctrl_Air_Manage_System", 1, 0, "Ctrl_Air_Manage_System");	
    qmlRegisterType<Ctrl_Air_Manage_Popup>("Ctrl_Air_Manage_Popup", 1, 0, "Ctrl_Air_Manage_Popup");
	//ren air manage add end 0523
    qmlRegisterType<Ctrl_MaintenanceAppointment_MA_01>("Ctrl_MaintenanceAppointment_MA_01", 1, 0, "Ctrl_MaintenanceAppointment_MA_01");		//[add by yulei]
    qmlRegisterType<Ctrl_Engineer_Info>("CtrlEngineerInfo", 1, 0, "CtrlEngineerInfo"); //jiayl add SX7-827
    qmlRegisterType<Ctrl_Factory_videolist>("Ctrl_Factory_videolist", 1, 0, "Ctrl_Factory_videolist"); //wangtingting add for factroy mode
 */
   return;

}

/******************************************************************************
Function Name : ShowMainQMLView
Description   : Show QML page
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_CommonClass::ShowMainQMLView()
{
    //QQuickItem* rootObject = main_qmlView->rootObject();

    HMI_LOG("setVisible");
    //rootObject->setVisible(true);

   // main_qmlView->showExpanded();

    return;
}
int Frm_CommonClass::viewModeIdOfBaseSrcMode()
{
    int modeId = EN_VIEW_MODE_HOME;
    HMI_LOG("modeId =%d",modeId);
    return modeId;
}
/******************************************************************************
Function Name : mode_proc_on_base
Description   : Mode trans(base)
Date          : 2015/07/15
Parameter     : Frm_ModeEvent* fModeEvent
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
int Frm_CommonClass::mode_proc_on_base(Frm_ModeEvent* fModeEvent)
{
    int rc = 1;
    //Frm_AbstractModeClass *child_ModeClass       = NULL;
    //Frm_AbstractModeClass *this_ModeClass        = NULL;
    //Frm_AbstractModeClass *current_ModeClass    = NULL;

    QList<Frm_AbstractModeClass*> allModeClass = this->findChildren<Frm_AbstractModeClass*>();

    quint16 modeid = fModeEvent->getModeID();
    quint16 screenid = fModeEvent->getScreenID();
    HMI_LOG("modeid = %d ,screenid = %d",modeid,screenid);

    ModeClassPopup(0);

    for(int k=0; k<ModeClassTableNum; k++)
    {
        if( modeid == m_ModeClassTable[k].modeid )
        {
            Frm_AbstractModeClass* common_mode = allModeClass.at(0);

            modeClass = ((Frm_AbstractModeClass*(*)(QObject*,QObject*,quint16,quint16,quint16 ))m_ModeClassTable[k].func)(
                        this, common_mode, modeid, screenid, 1/*m_ModeClassTable[k].modeAttr*/);

            modeClass->Start();

            modeClass->visible();
            modeClass->setmodeStatus(MODE_ON_PERCENT_100);
            HMI_LOG("[constructView]screenid:%d",modeClass->getscreenID());
            modeClass->constructView(modeClass->getscreenID(), MODE_ON_PERCENT_100);
            break;
        }
    }
    return rc;
}

/******************************************************************************
Function Name : mode_proc_on_replace
Description   : Mode trans(interrupt) if this interrupt mode want to replace the current interrupt mode, call this function
Date          : 2017/10/12
Parameter     : Frm_ModeEvent* fModeEvent
Author        : juanjuan(visteon)
******************************************************************************/
int Frm_CommonClass::mode_proc_on_replace(Frm_ModeEvent* fModeEvent)
{
    int rc = 1;
    Frm_AbstractModeClass *child_ModeClass       = NULL;
    Frm_AbstractModeClass *this_ModeClass        = NULL;
    //Frm_AbstractModeClass *current_ModeClass    = NULL;

    QList<Frm_AbstractModeClass*> allModeClass = this->findChildren<Frm_AbstractModeClass*>();

    quint16 modeid = fModeEvent->getModeID();
    quint16 screenid = fModeEvent->getScreenID();
    HMI_LOG("modeid = %d ,screenid = %d",modeid,screenid);
    int i=0;
    int icount = allModeClass.count();
    for(i=0; i<icount; i++)
    {
        Frm_AbstractModeClass* pModeClass = allModeClass.at(i);

        if( NULL != this_ModeClass )
        {
            child_ModeClass = pModeClass;
            break;
        }

        else if( modeid == pModeClass->getmodeID())
        {
            this_ModeClass = pModeClass;
        }
        else
        {
            //current_ModeClass = pModeClass;
        }
    }


    if (NULL != this_ModeClass)
    {
        //Current mode is not Top Mode Class
        if (NULL != child_ModeClass)
        {
            for(int j=icount-1; j>=i; j--)
            {
                Frm_AbstractModeClass* pModeClass = allModeClass.at(j);

                if( NULL != pModeClass )
                {
                    if (MODE_ON_PERCENT_100 == pModeClass->getmodeStatus())
                    {
                        pModeClass->invisible();
                    }

                    pModeClass->End();

                    delete pModeClass;
                    pModeClass = NULL;
                }
            }
        }


        this_ModeClass->visible();
        this_ModeClass->setmodeStatus(MODE_ON_PERCENT_100);
        HMI_LOG("[constructView]screenid=%d",screenid);
        this_ModeClass->constructView(screenid, MODE_ON_PERCENT_100);
        modeClass = this_ModeClass;
    }
    else
    {
        if((NULL != modeClass) && (icount > 2))
        {
                HMI_LOG("if modeClass is not NULL, icount must >2,otherwise,there must be something wrong");
            Frm_AbstractModeClass *parentOfCur_ModeClass        = NULL;
            if(modeClass == allModeClass.at(icount-1))
            {
                parentOfCur_ModeClass = allModeClass.at(icount-2);

                for(int k=0; k<ModeClassTableNum; k++)
                {
                    if( modeid == m_ModeClassTable[k].modeid )
                    {
                         this_ModeClass = ((Frm_AbstractModeClass*(*)(QObject*,QObject*,quint16,quint16,quint16 ))m_ModeClassTable[k].func)(
                                      this, parentOfCur_ModeClass, modeid, screenid, 2/*m_ModeClassTable[k].modeAttr*/);
                         if(MODE_ON_PERCENT_100 == modeClass->getmodeStatus())
                         {
                             modeClass->invisible();
                             modeClass->setmodeStatus(MODE_ON_PERCENT_0);
                         }
                         modeClass->End();
                         delete modeClass;
                         modeClass = NULL;
                         this_ModeClass->Start();
                         this_ModeClass->visible();
                         this_ModeClass->setmodeStatus(MODE_ON_PERCENT_100);
                         HMI_LOG("[constructView]screenid=%d",this_ModeClass->getscreenID());
                         this_ModeClass->constructView(this_ModeClass->getscreenID(), MODE_ON_PERCENT_100);
                         modeClass = this_ModeClass;
                    }
                }
            }
            else
            {
                HMI_LOG("Are you kidding me? modeClass is not the top mode,impossible!!!");
            }

        }
        else
        {
            HMI_LOG("wrong case, maybe you should call mode_proc_on_base() or mode_proc_on_interrupt()");
        }
    }

    return rc;
}

/******************************************************************************
Function Name : mode_proc_on_interrupt
Description   : Mode trans(interrupt)
Date          : 2015/07/15
Parameter     : Frm_ModeEvent* fModeEvent
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
int Frm_CommonClass::mode_proc_on_interrupt(Frm_ModeEvent* fModeEvent)
{
    int rc = 1;
    Frm_AbstractModeClass *child_ModeClass       = NULL;
    Frm_AbstractModeClass *this_ModeClass        = NULL;
    //Frm_AbstractModeClass *current_ModeClass    = NULL;

    QList<Frm_AbstractModeClass*> allModeClass = this->findChildren<Frm_AbstractModeClass*>();

    quint16 modeid = fModeEvent->getModeID();
    quint16 screenid = fModeEvent->getScreenID();
    HMI_LOG("modeid = %d ,screenid = %d",modeid,screenid);
    int i=0;
    int icount = allModeClass.count();
    for(i=0; i<icount; i++)
    {
        Frm_AbstractModeClass* pModeClass = allModeClass.at(i);

        if( NULL != this_ModeClass )
        {
            child_ModeClass = pModeClass;
            break;
        }

        else if( modeid == pModeClass->getmodeID())
        {
            this_ModeClass = pModeClass;
        }
        else
        {
            //current_ModeClass = pModeClass;
        }
    }

    if (NULL != this_ModeClass)
    {
        //Current mode is not Top Mode Class
        if (NULL != child_ModeClass)
        {
            for(int j=icount-1; j>=i; j--)
            {
                Frm_AbstractModeClass* pModeClass = allModeClass.at(j);

                if( NULL != pModeClass )
                {
                    if (MODE_ON_PERCENT_100 == pModeClass->getmodeStatus())
                    {
                        pModeClass->invisible();
                    }

                    pModeClass->End();

                    delete pModeClass;
                }
            }
        }


        this_ModeClass->visible();
        this_ModeClass->setmodeStatus(MODE_ON_PERCENT_100);
        HMI_LOG("[constructView]screenid=%d",screenid);
        this_ModeClass->constructView(screenid, MODE_ON_PERCENT_100);
        modeClass = this_ModeClass;
    }
    else
    {
        if( (NULL != modeClass) && (MODE_ON_PERCENT_100 == modeClass->getmodeStatus()))
        {
            modeClass->invisible();
            modeClass->setmodeStatus(MODE_ON_PERCENT_0);
        }


        if( NULL == modeClass )
        {
            for(int k=0; k<ModeClassTableNum; k++)
            {
                Frm_AbstractModeClass* common_mode = allModeClass.at(0);
                int modeID = viewModeIdOfBaseSrcMode();
                if( modeID == m_ModeClassTable[k].modeid )
                {
                    HMI_LOG("no base mode, we should give a base mode");
                    HMI_LOG("modeId =%d",modeID);
                    modeClass = ((Frm_AbstractModeClass*(*)(QObject*,QObject*,quint16,quint16,quint16 ))m_ModeClassTable[k].func)(
                                this, common_mode,modeID , 1, 1/*m_ModeClassTable[k].modeAttr*/);
                    modeClass->Start();
                    modeClass->setmodeStatus(MODE_ON_PERCENT_0);
                    break;
                }
            }
        }
        if( NULL != modeClass )
        {
            for(int k=0; k<ModeClassTableNum; k++)
            {
                if( modeid == m_ModeClassTable[k].modeid )
                {
                    HMI_LOG("have base mode, this is a normal interrup mode");
                    modeClass = ((Frm_AbstractModeClass*(*)(QObject*,QObject*,quint16,quint16,quint16 ))m_ModeClassTable[k].func)(
                                this, modeClass, modeid, screenid, 2/*m_ModeClassTable[k].modeAttr*/);
                    break;
                }
            }
        }


        modeClass->Start();

        modeClass->visible();
        modeClass->setmodeStatus(MODE_ON_PERCENT_100);
        HMI_LOG("[constructView]screenid=%d",modeClass->getscreenID());
        modeClass->constructView(modeClass->getscreenID(), MODE_ON_PERCENT_100);
    }

    return rc;
}

/******************************************************************************
Function Name : mode_proc_on_off
Description   : Mode trans(off)
Date          : 2015/07/15
Parameter     : Frm_ModeEvent* fModeEvent
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
int Frm_CommonClass::mode_proc_on_off(Frm_ModeEvent* fModeEvent)
{
    int rc = 1;
    Frm_AbstractModeClass *current_ModeClass    = NULL;

    QList<Frm_AbstractModeClass*> allModeClass = this->findChildren<Frm_AbstractModeClass*>();

    quint16 modeid = fModeEvent->getModeID();
    HMI_LOG("modeid = %d ",modeid);
    int icount = allModeClass.count();

    current_ModeClass = allModeClass.at(icount - 1);

    if (NULL != current_ModeClass && (modeid == current_ModeClass->getmodeID()))
    {
        if(icount > 2)
        {
            if (MODE_ON_PERCENT_100 == current_ModeClass->getmodeStatus())
            {
                current_ModeClass->invisible();
            }

            current_ModeClass->End();

            delete current_ModeClass;


            Frm_AbstractModeClass* pModeClass = allModeClass.at(icount - 2);

            if( NULL != pModeClass )
            {
                pModeClass->visible();

                pModeClass->setmodeStatus(MODE_ON_PERCENT_100);

                modeClass = pModeClass;
            }
        }
        else
        {
            rc = 0;
        }
    }
    else
    {
        rc = 0;
    }

    return rc;
}

/******************************************************************************
Function Name : modeTrans
Description   : Mode trans
Date          : 2015/07/15
Parameter     : QEvent *e
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
int Frm_CommonClass::modeTrans(QEvent *e)
{
    Frm_ModeEvent* fModeEvent = dynamic_cast<Frm_ModeEvent*>(e);
    int rc = 0 ;
    HMI_LOG("modeTrans is on");
    switch(fModeEvent->getReqType())
    {
    case EN_MODE_OFF:
        HMI_LOG("EN_MODE_OFF");
    case EN_MODE_OFF_NOSOURCE_CHANGE:
        HMI_LOG("EN_MODE_OFF_NOSOURCE_CHANGE");
        if((false == find_nextmode()) && (get_current_view_mode() == fModeEvent->getModeID()))//when nextmode is null,trans to home
        {
            Frm_ModeEvent* fModeEventTrans = NULL;
            fModeEventTrans  = new Frm_ModeEvent(static_cast<QEvent::Type>(Frm_AbstractEvent::EN_EV_MODETRANS_ON),
                                                EN_VIEW_MODE_HOME, 1 ,EN_MODE_ON_BASE);
            mode_proc_on_base(fModeEventTrans);
        }
        else
        {
            rc = mode_proc_on_off(fModeEvent);
        }
        break;
    case EN_MODE_ON_INTERRUPT:
        HMI_LOG("EN_MODE_ON_INTERRUPT");
        rc = mode_proc_on_interrupt(fModeEvent);
        break;
    case EN_MODE_ON_BASE:
        HMI_LOG("EN_MODE_ON_BASE");
        rc = mode_proc_on_base(fModeEvent);
        break;
    //juanjuan add start
    case EN_MODE_ON_REPLACE:
        HMI_LOG("EN_MODE_ON_REPLACE");
        rc = mode_proc_on_replace(fModeEvent);
        break;
     //juanjuan add end
    default:
        break;
    }

    return rc;
}

/******************************************************************************
Function Name : modeTrans
Description   : Screen trans
Date          : 2015/07/15
Parameter     : QEvent *e
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
int Frm_CommonClass::screenTrans(QEvent *e)
{
    int rc = 0;

    if (NULL != modeClass)
    {
        modeClass->modeEventProc(e);
    }

    return rc;
}

/******************************************************************************
Function Name : customEvProc
Description   : Receive trans event
Date          : 2015/07/15
Parameter     : QEvent *e
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_CommonClass::customEvProc(QEvent *e)
{
    QList<Frm_AbstractModeClass*> allModeClass = this->findChildren<Frm_AbstractModeClass*>();
    Frm_AbstractModeClass *child_ModeClass       = NULL;// the child mode of this mode
    Frm_AbstractModeClass *this_ModeClass        = NULL; // the mode need be remove from the mode chain
    Frm_AbstractModeClass *parent_ModeClass       = NULL;//the parent mode of this mode
    quint16 modeid = 0;// the mode if return by consumeEvent,if the upper mode have returned a modeid,
                            //ignorn modeid return by the lower mode,
                            //because every event should only remove or replace one mode
    int icount = allModeClass.count();
    int ret = 0; // the return value of consumeEvent, the last bit means the event will process by parent mode or not,other bit is the modeid
    bool rc = 0;
    /*process the event from top to buttom,
     * find the mode which is not avaliable,
     * if the mode exist in the chain ,and its parent isn't the common calss,
     *  means the mode can be remove directly,and set the */
    HMI_LOG("customEvProc icount =%d",icount);
    for(int i=icount; i>0; i--)
    {
        Frm_AbstractModeClass* pModeClass = allModeClass.at(i-1);
        HMI_LOG("pModeClass mode:%s[%d]",getModeName(pModeClass->getmodeID()),pModeClass->getmodeID());
        if((NULL != modeClass)&&(modeClass == pModeClass))
        {
            if (2 == pModeClass->getmodeStatus())
            {
                rc |= pModeClass->modeEventProc(e);
            }
        }
        else if(0 == rc)
        {
            ret = pModeClass->consumeEvent(e);
            rc |= (bool)(ret&1);
            if(0 == modeid)modeid = (ret>>1);//
            if(modeid != 0)
            {
                if(NULL == this_ModeClass)
                {
                    if(CmnmodeClass==pModeClass)
                    {
                        HMI_LOG("common mode event");
                        HMI_LOG("need change to mode %d",modeid);
                        if(i < icount-1)
                        {
                            this_ModeClass = allModeClass.at(i);
                            HMI_LOG("this mode:%s[%d]",getModeName(this_ModeClass->getmodeID()),this_ModeClass->getmodeID());
                            child_ModeClass = allModeClass.at(i+1);
                            HMI_LOG("child mode:%s[%d]",getModeName(child_ModeClass->getmodeID()),child_ModeClass->getmodeID());
                        }
                        else
                        {
                            HMI_LOG("the child mode is NULL,this mode is the top mode of the mode chain");

                        }
                        for(int k=0; k<ModeClassTableNum; k++)
                        {
                            if( modeid == m_ModeClassTable[k].modeid )
                            {
                                parent_ModeClass = ((Frm_AbstractModeClass*(*)(QObject*,QObject*,quint16,quint16,quint16 ))m_ModeClassTable[k].func)(
                                            this, CmnmodeClass, modeid, 1, 1);
                                parent_ModeClass->Start();
                                parent_ModeClass->setmodeStatus(MODE_ON_PERCENT_0);
                                //parent_ModeClass->constructView(1, MODE_ON_PERCENT_0);
                                HMI_LOG("paren mode:%s[%d]",getModeName(parent_ModeClass->getmodeID()),parent_ModeClass->getmodeID());
                                break;
                            }
                        }
                        if(NULL == parent_ModeClass)
                        {
                            HMI_ERROR("the parent mode is NULL,you may give me a invalid modeid,maybe you should tell me change to another correct mode");
                        }
                    }
                    else
                    {
                        this_ModeClass = pModeClass;
                        HMI_LOG("get this mode:%s[%d],this case should only process once, if it process more than once, there maybe something wrong",getModeName(this_ModeClass->getmodeID()),this_ModeClass->getmodeID());//
                        if(modeid == this_ModeClass->getmodeID())
                        {
                            if(i < icount)
                            {
                                child_ModeClass = allModeClass.at(i);
                                HMI_LOG("child mode:%s[%d]",getModeName(child_ModeClass->getmodeID()),child_ModeClass->getmodeID());
                            }
                            else
                            {
                                HMI_LOG("the child mode is NULL,this mode is the top mode of the mode chain");
                                HMI_ERROR("this case should never be process,if you see me ,there must be a big bug in the code, check it now");
                                child_ModeClass = NULL;
                            }
                            if(i>2)
                            {
                                parent_ModeClass = allModeClass.at(i-2);
                                HMI_LOG("paren mode:%s[%d]",getModeName(parent_ModeClass->getmodeID()),parent_ModeClass->getmodeID());
                            }
                            else
                            {
                                HMI_ERROR("the parent mode is NULL,this mode is a normal mode base on common,maybe you should tell me change to another correct mode");
                                parent_ModeClass = NULL;
                            }
                        }
                        else
                        {
                            HMI_LOG("need change to mode %d",modeid);
                            if(i < icount)
                            {
                                child_ModeClass = allModeClass.at(i);
                                HMI_LOG("child mode:%s[%d]",getModeName(child_ModeClass->getmodeID()),child_ModeClass->getmodeID());
                            }
                            else
                            {
                                HMI_LOG("the child mode is NULL,this mode is the top mode of the mode chain");
                                HMI_ERROR("this case should never be process,if you see me ,there must be a big bug in the code, check it now");
                                child_ModeClass = NULL;
                            }
                            for(int k=0; k<ModeClassTableNum; k++)
                            {
                                if( modeid == m_ModeClassTable[k].modeid )
                                {
                                    parent_ModeClass = ((Frm_AbstractModeClass*(*)(QObject*,QObject*,quint16,quint16,quint16 ))m_ModeClassTable[k].func)(
                                                this, CmnmodeClass, modeid, 1, 1);
                                    parent_ModeClass->Start();
                                    parent_ModeClass->setmodeStatus(MODE_ON_PERCENT_0);
                                    //parent_ModeClass->constructView(1, MODE_ON_PERCENT_0);
                                    HMI_LOG("paren mode:%s[%d]",getModeName(parent_ModeClass->getmodeID()),parent_ModeClass->getmodeID());
                                    break;
                                }
                            }
                            if(NULL == parent_ModeClass)
                            {
                                HMI_ERROR("the parent mode is NULL,you may give me a invalid modeid,maybe you should tell me change to another correct mode");
                            }

                        }
                    }
                }
            }
        }
    }
    if(NULL != this_ModeClass)
    {
        if((NULL != child_ModeClass)&&(NULL != parent_ModeClass))
        {
            child_ModeClass->changeParent(parent_ModeClass);
            if (MODE_ON_PERCENT_100 == this_ModeClass->getmodeStatus())
            {
                this_ModeClass->invisible();
            }
            this_ModeClass->End();
            delete this_ModeClass;
            this_ModeClass = NULL;
        }
        else
        {
            HMI_LOG("wrong process, check your code");
        }
    }
    else
    {
         HMI_LOG("this mode is NULL, means this is a normal event,we needn't remove or replace a exist but invalid mode");
    }

    if (NULL != modeClass)
    {
        if (0 == rc)
        {
            CmnmodeClass->concreteEvent(e);
        }
    }
    else
    {
//        qDebug("Frm_CommonClass::customEvProc error ");
        CmnmodeClass->concreteEvent(e);
    }

    return;
}

/******************************************************************************
Function Name : ~Frm_CommonClass
Description   : Destructor
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_CommonClass::~Frm_CommonClass()
{
    if(NULL != m_ApiClass)
    {
        m_ApiClass->exit();
        delete m_ApiClass;
        m_ApiClass = NULL;
    }
}

/******************************************************************************
Function Name : setCursorStatus
Description   : Record state of cursor
Date          : 2015/08/31
Parameter     :
Author        : Maotianliang(Neusoft)
******************************************************************************/
void Frm_CommonClass::setCursorStatus(CursorstatusEnum en_status)
{
    m_enCursorstatus = en_status;
}

/******************************************************************************
Function Name : getCursorStatus
Description   : get state of cursor
Date          : 2015/08/31
Parameter     :
Author        : Maotianliang(Neusoft)
******************************************************************************/
void Frm_CommonClass::getCursorStatus(CursorstatusEnum *pen_status)
{
    *pen_status = m_enCursorstatus;
}
/******************************************************************************
Function Name : setCursorType
Description   : Record state of cursor
Date          : 2015/08/31
Parameter     :
Author        : Maotianliang(Neusoft)
******************************************************************************/
void Frm_CommonClass::setCursorType(CursorTypeEnum en_type, int ipos)
{
    m_enCursorType = en_type;
    m_ipos = ipos;
}

/******************************************************************************
Function Name : getCursorType
Description   : get state of cursor
Date          : 2015/08/31
Parameter     :
Author        : Maotianliang(Neusoft)
******************************************************************************/
void Frm_CommonClass::getCursorType(CursorTypeEnum *pen_type, int *pi_pos)
{
    *pen_type = m_enCursorType;
    *pi_pos = m_ipos;
}

void Frm_CommonClass::setSettingCursorpos(int ipos)
{
    m_isettingpos = ipos;
}

void Frm_CommonClass::getSettingCursorpos(int* pi_pos)
{
    *pi_pos = m_isettingpos;
}

/******************************************************************************
Function Name : backFromNaviMode
Description   : change the view mode from Navi to current Entertainment mode
Date          : 2015/10/12
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_CommonClass::backFromNaviMode()
{
    m_ModeManager.backFromNaviMode();
}

void Frm_CommonClass::Device_plugOut(int i_devtype)
{
    HMI_LOG("i_devtype = %d", i_devtype);
    switch (i_devtype)
    {
    case 1:
        m_ModeManager.USB_Eject();
       break;
    case 2:
        m_ModeManager.IPOD_Eject();
        break;
    case 3:
        m_ModeManager.BTA_Eject();
        break;
    case 4:
        m_ModeManager.AUX_Eject();
        break;
    case 5:
        m_ModeManager.LINK_Eject();
        break;
    default:
        break;
    }
}

void Frm_CommonClass::special_key_filter(QEvent *e)
{

    Frm_KeyEvent* fHKEvent = dynamic_cast<Frm_KeyEvent*>(e);

    if( NULL != fHKEvent )
    {
        switch(fHKEvent->getFuncID())
        {
        case KEYEVENT_NAVI_RELEASE:
            modeClass->destroyshowMessage();
            break;
        case KEYEVENT_PHONE_RELEASE:
        case KEYEVENT_PHONE_LONGRELEASE:
        {
             modeClass->destroyshowMessage();
         }
             break;
        case KEYEVENT_HOME_RELEASE:
            modeClass->destroyshowMessage();
            break;
        case KEYEVENT_BAND_PRESS:
        case KEYEVENT_BAND_RELEASE:
             modeClass->destroyshowMessage();
             break;
        case KEYEVENT_SETUP_RELEASE:
            modeClass->destroyshowMessage();
            break;
        case KEYEVENT_LINK_RELEASE:
            modeClass->destroyshowMessage();
            break;
        case KEYEVENT_DISP_RELEASE:
        case KEYEVENT_POWER_RELEASE:
        case KEYEVENT_POWER_LONGRELEASE:
        case KEYEVENT_STW_MODE_RELEASE:
            modeClass->destroyshowMessage();
            break;
        case KEYEVENT_ENCODER_PRESS:
            m_knobpressflag = 1;
            break;
        case KEYEVENT_ENCODER_RELEASE:
            m_knobpressflag = 0;
            break;
        default:
            break;
        }
    }

}

int Frm_CommonClass::get_current_view_mode()
{
    QList<Frm_AbstractModeClass*> allModeClass = this->findChildren<Frm_AbstractModeClass*>();

    Frm_AbstractModeClass* pModeClass = allModeClass.last();

    HMI_LOG("pModeClass->getmodeID() = %d", pModeClass->getmodeID());

    return pModeClass->getmodeID();
}

bool Frm_CommonClass::find_inputmode(int mode)
{
    bool b_ret = false;
    QList<Frm_AbstractModeClass*> allModeClass = this->findChildren<Frm_AbstractModeClass*>();

    Frm_AbstractModeClass* pModeClass = NULL;

    for (int i=0; i < allModeClass.size(); ++i )
    {
       pModeClass = allModeClass.at(i);
       if ( pModeClass->getmodeID() == mode )
       {
            HMI_LOG("find mode %s",getModeName(mode));
            b_ret = true;
       }
    }

    return b_ret;
}

bool Frm_CommonClass::find_nextmode()
{
    bool b_ret = false;
    QList<Frm_AbstractModeClass*> allModeClass = this->findChildren<Frm_AbstractModeClass*>();

    Frm_AbstractModeClass* pModeClass = NULL;

    HMI_LOG("allModeClass.size() = %d", allModeClass.size());
    if(allModeClass.size() > 2)
    {
        pModeClass = allModeClass.at(allModeClass.size() - 2);
        HMI_LOG("pModeClass->getmodeID() = %d", pModeClass->getmodeID());
        if(pModeClass->getmodeID() > EN_VIEW_MODE_NULL)
        {
            b_ret = true;
        }
    }

    return b_ret;
}

bool Frm_CommonClass::fined_nextinputmode(int mode)
{
    bool b_ret = false;
    QList<Frm_AbstractModeClass*> allModeClass = this->findChildren<Frm_AbstractModeClass*>();

    Frm_AbstractModeClass* pModeClass = NULL;
    HMI_LOG("mode = %d", mode);
    HMI_LOG("allModeClass.size() = %d", allModeClass.size());
    if(allModeClass.size() > 2)
    {
        pModeClass = allModeClass.at(allModeClass.size() - 2);
        HMI_LOG("pModeClass->getmodeID() = %d", pModeClass->getmodeID());
        if(pModeClass->getmodeID() == mode)
        {
            b_ret = true;
        }
    }

    return b_ret;
}
//juanjuan add start
const char *Frm_CommonClass::getModeName(quint16 modeid)
{
    const char *name=NULL;
    for(int k=0; k<ModeClassTableNum; k++)
    {
        if( modeid == m_ModeClassTable[k].modeid )
        {
            name = m_ModeClassTable[k].modename;
            break;
        }
    }
    if(NULL == name)
    {
        HMI_LOG("you may give a wrong modeid,the mode is not exist");
        name = m_ModeClassTable[0].modename;
    }
    return name;
}
int Frm_CommonClass::getModeSource(quint16 modeid)
{
    int srcMode = SX5_MODE_MAX;
    for(int k=0; k<ModeClassTableNum; k++)
    {
        if( modeid == m_ModeClassTable[k].modeid )
        {
            srcMode = m_ModeClassTable[k].srcMode;
            break;
        }
    }
    return srcMode;
}

int Frm_CommonClass::getTopModeAttr()
{
    int attr = 0;
    QList<Frm_AbstractModeClass*> allModeClass = this->findChildren<Frm_AbstractModeClass*>();
    int icount = allModeClass.count();
    Frm_AbstractModeClass* pModeClass = NULL;
    if(icount)
    {
        pModeClass = allModeClass.at(icount-1);
        if(NULL != pModeClass)attr = pModeClass->getmodeAttr();
    }
    return attr;
}

int Frm_CommonClass::find_parentmode(Frm_AbstractModeClass* mode)
{
    int i_ret = 0;
    QList<Frm_AbstractModeClass*> allModeClass = this->findChildren<Frm_AbstractModeClass*>();
    Frm_AbstractModeClass* pModeClass = NULL;
    int icount = allModeClass.count();
    if(icount>2)
    {
        for(int i=icount-1; i>0; i--)
        {
            pModeClass = allModeClass.at(i);
            if(mode->getmodeID() == pModeClass->getmodeID())
            {
                if(i>1)
                {
                    i_ret = allModeClass.at(i-1)->getmodeID();
                    HMI_LOG("mode:%s[%d],its parent mode is %s[%d]",getModeName(mode->getmodeID()),mode->getmodeID(),getModeName(allModeClass.at(i-1)->getmodeID()),allModeClass.at(i-1)->getmodeID());
                }
                break;
            }
        }
    }
    else
    {
        HMI_LOG("mode:%s[%d] is a single mode,no parent,no child",getModeName(mode->getmodeID()),mode->getmodeID());
    }

    return i_ret;
}
int Frm_CommonClass::find_childmode(Frm_AbstractModeClass* mode)
{
    int i_ret = 0;
    QList<Frm_AbstractModeClass*> allModeClass = this->findChildren<Frm_AbstractModeClass*>();
    Frm_AbstractModeClass* pModeClass = NULL;
    int icount = allModeClass.count();
    if(icount>2)
    {
        for(int i=icount-1; i>0; i--)
        {
            pModeClass = allModeClass.at(i);
            if(mode->getmodeID() == pModeClass->getmodeID())
            {
                if(i<icount-1)
                {
                    i_ret = allModeClass.at(i+1)->getmodeID();
                    HMI_LOG("mode:%s[%d],its child mode is %s[%d]",getModeName(mode->getmodeID()),mode->getmodeID(),getModeName(allModeClass.at(i+1)->getmodeID()),allModeClass.at(i+1)->getmodeID());
                }
                break;
            }
        }
    }
    else
    {
        HMI_LOG("mode:%s[%d] is a single mode,no parent,no child",getModeName(mode->getmodeID()),mode->getmodeID());
    }

    return i_ret;
}
//juanjuan add end
int Frm_CommonClass::get_current_view_screenid()
{
    QList<Frm_AbstractModeClass*> allModeClass = this->findChildren<Frm_AbstractModeClass*>();

    Frm_AbstractModeClass* pModeClass = allModeClass.last();

    return pModeClass->getscreenID();
}

void Frm_CommonClass::Movie_forbid_previous_mode(int transtype)
{
    //m_ModeManager.back_to_previous_mode_from_Movie(transtype);
}

int Frm_CommonClass::getHomecursor()
{
    return m_home_pos;
}

void Frm_CommonClass::setHomecursor(int i_pos)
{
    m_home_pos = i_pos;
}

void Frm_CommonClass::applicationStatusSlot(Qt::ApplicationState state)
{
    HMI_LOG("ApplicationState = %d", state);
    if ((Qt::ApplicationActive == state) && (0 == i_isboot))
    {
        i_isboot = 1;
        //QQmlComponent component(getMainQMLView()->engine(), QUrl("qrc:/qml/Srn_Radio_Play.qml"));
       // QObject* qmlViewComponent = component.create();
        //set_qmlcomponet(QString("qrc:/qml/Srn_Radio_Play.qml"),qmlViewComponent);
        if (NULL == modeClass)
        {
            HMI_LOG("NULL == modeClass");
        }
    }
}

void Frm_CommonClass::init_audioSource()
{
    m_ModeManager.Init_AudioSource();
}

void Frm_CommonClass::timerEvent(QTimerEvent* event)
{
    if( event->timerId() == i_avmtimer )
    {

    }
    //13720	SX5-5696
    else if( event->timerId() == i_usbimer )
    {
        killTimer(i_usbimer);
        i_usbimer = 0;
        dequeueMessage_auto();
    }
    else
    {

    }
}
/******************************************************************************
Function Name : showMessage
Description   : Show Message
Date          : 2016/01/15
Parameter     : message
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_CommonClass::showMessage(QString message)
{
//    HMI_LOG("Enter");
    if (true == isAlreadyReq(message))
    {
        //Message is requested , do nothing
    }
    else if (m_Popup_queue.isEmpty())
    {
        if (m_mesaage_shown.isEmpty())
        {
            m_mesaage_shown = message;
            createMessage(message);
        }
        else if((0 == m_mesaage_shown.compare(QObject::tr("USB媒体加载中...")))
                || (0 == m_mesaage_shown.compare(QObject::tr("iPod媒体加载中..."))))
        {
            m_mesaage_shown = message;
            createMessage(message);
        }
        else
        {
            m_Popup_queue.enqueue(message);
        }
    }
    else
    {
        m_Popup_queue.enqueue(message);
    }

    if((0 == message.compare(QObject::tr("USB媒体加载中...")))
            || (0 == message.compare(QObject::tr("iPod媒体加载中..."))))
    {
        isLoadingInterrupt = 0;
    }
}

void Frm_CommonClass::showmusicMessage(QString message)
{
    int i_viewcurmode = Frm_CommonClassIns->get_current_view_mode();
    if( 0 == i_viewcurmode )
    {
        return;
    }
    message = message;
    return;
}

/******************************************************************************
Function Name : isAlreadyReq
Description   : 
Date          : 2016/01/15
Parameter     : message
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
bool Frm_CommonClass::isAlreadyReq(QString &message)
{
    HMI_TRACE_IN;
    QString str;
    bool b_ret = false;
    if (0 == m_mesaage_shown.compare(message))
    {
        //13720	SX5-5696
        if((0 == m_mesaage_shown.compare(QObject::tr("USB媒体加载中...")))
                || (0 == m_mesaage_shown.compare(QObject::tr("iPod媒体加载中..."))))
        {
            HMI_LOG("already req");
            b_ret = true;
            return b_ret;
        }
       createMessage(message); // for reset timer
        b_ret = true;
    }
    else if (m_Popup_queue.isEmpty())
    {
        HMI_LOG("m_Popup_queue.isEmpty() is true");
        b_ret = false;
    }
    else
    {
        foreach (str, m_Popup_queue) {
            if (0 == str.compare(message))
            {
                HMI_LOG("finded same");
                b_ret= true;
                break;
            }
        }
    }
    HMI_LOG("count = %d", m_Popup_queue.count());
    return b_ret;
}


/******************************************************************************
Function Name : dequeueMessage_auto
Description   : 
Date          : 2016/01/15
Parameter     : 
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_CommonClass::dequeueMessage_auto()
{
}
/******************************************************************************
Function Name : dequeueMessage_byUser
Description   : 
Date          : 2016/01/15
Parameter     : 
Author        : Mao.Tianliang(Neusoft)
******************************************************************************/
void Frm_CommonClass::dequeueMessage_byUser()
{
    HMI_TRACE_IN;
    if((0 == m_mesaage_shown.compare(QObject::tr("USB媒体加载中...")))
            ||(0 == m_mesaage_shown.compare(QObject::tr("iPod媒体加载中..."))))
    {
        isLoadingInterrupt = 1;
    }

    if (false == m_Popup_queue.isEmpty())
    {
        m_mesaage_shown = m_Popup_queue.dequeue();
        createMessage(m_mesaage_shown);
    }
    else
    {
        m_mesaage_shown.clear();
        //QQuickItem* rootObject = main_qmlView->rootObject();
        //QMetaObject::invokeMethod(rootObject, "destroy_onscreen");
    }
}

void Frm_CommonClass::dequeueMusicMessage_byUser()
{
    HMI_TRACE_IN;
    if((0 == m_mesaage_shown.compare(QObject::tr("USB媒体加载中...")))
            ||(0 == m_mesaage_shown.compare(QObject::tr("iPod媒体加载中..."))))
    {
        isLoadingInterrupt = 1;
    }

    if (false == m_Popup_queue.isEmpty())
    {
        m_mesaage_shown = m_Popup_queue.dequeue();
        createMessage(m_mesaage_shown);
    }
    else
    {
        m_mesaage_shown.clear();
        //QQuickItem* rootObject = main_qmlView->rootObject();
        //QMetaObject::invokeMethod(rootObject, "destroy_musiconscreen");
    }
}

/******************************************************************************
Function Name : createMessage
Description   : Show Message
Date          : 2015/07/15
Parameter     : message
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_CommonClass::createMessage(QString message)
{
    //QQuickItem* rootObject = main_qmlView->rootObject();

   // QMetaObject::invokeMethod(rootObject, "show_onscreen",
   //                           Q_ARG(QVariant, message));

    return;
}


/******************************************************************************
Function Name : createMessage_noTimer
Description   : Show Message
Date          : 2015/07/15
Parameter     : message
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_CommonClass::createMessage_noTimer(QString message)
{
    //QQuickItem* rootObject = main_qmlView->rootObject();

    //QMetaObject::invokeMethod(rootObject, "show_onscreen_noTimer",
    //                          Q_ARG(QVariant, message));
    return;
}

bool Frm_CommonClass::eventFilter(QObject * obj,QEvent * e)
{
    if((QEvent::Timer!= e->type())&&(QEvent::UpdateRequest !=e->type()))
    {
        HMI_LOG("IN");
        HMI_LOG("type=%d",e->type());
    }

    bool b_ret = false;

    if(QEvent::TouchBegin == e->type())
    {
        if((QEvent::Timer!= e->type())&&(QEvent::UpdateRequest !=e->type()))
        {
           HMI_LOG("m_touchpermitstatus=%d",m_touchpermitstatus);
        }

        //if( main_qmlView == obj )
        {
            if((QEvent::Timer!= e->type())&&(QEvent::UpdateRequest !=e->type()))
            {
                 HMI_LOG("### main_qmlView == obj ###");
            }
            switch(e->type())
            {
                case QEvent::TouchBegin:
                case QEvent::TouchEnd:
                case QEvent::TouchUpdate:
                {
                    //QTouchEvent* touchEvent = dynamic_cast<QTouchEvent*>(e);
                   // QList<QTouchEvent::TouchPoint> touchlist = touchEvent->touchPoints();
                    HMI_LOG("touchlist.count()=%d",touchlist.count());
                    if( QEvent::TouchEnd == e->type())
                    {
                    }

                }
                    break;
                case QEvent::KeyPress:
                case QEvent::KeyRelease:
                {
                    //QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(e);
                    HMI_LOG("keycode=%d",keyEvent->key());
                    if(1 == m_knobpressflag)
                    {
                        HMI_LOG("HK disable 1");
                        b_ret = true;
                    }
                    else
                    {
                        b_ret = false;
                    }
                }
                    break;
                case QEvent::MouseButtonPress:
                    break;
                case QEvent::MouseButtonRelease:
                    break;
                case QEvent::Wheel:
                {
                    b_ret = false;
                }
                    break;
                default:
                    b_ret = false;
            }
            return b_ret;
        }
        //else
        {
            //return QObject::eventFilter(obj, e);
        }
    }
}

QObject* Frm_CommonClass::get_qmlcomponet(QString url)
{
    if(m_qmap.contains(url))
    {
        HMI_LOG("%s is created ",qPrintable(url));
        QMap<QString,QObject*>::iterator it = m_qmap.find(url);
        return it.value();
    }
    else
    {
        HMI_LOG("%s is NULL ",qPrintable(url));
        return NULL;
    }
}

void Frm_CommonClass::set_qmlcomponet(QString url,QObject* qmlcomponent)
{
    HMI_LOG("IN");
    m_qmap.insert(url,qmlcomponent);
    return;
}

void Frm_CommonClass::delet_qmlcomponet(QString url)
{
    if(m_qmap.contains(url))
    {
        m_qmap.remove(url);
    }
    return;
}

