/****************Include  Section Begin***************************************************/
#include "hmilib_statusbar.h"
#include "hmifrm_common.h"
#include "hmiapi_audiomanager.h"
/****************Include  Section End***************************************************/

Lib_Ctrl_Statusbar::Lib_Ctrl_Statusbar()
{
    i_Timerid = 0;
    i_powerstatus = -1;
    i_singlestatus = -1;
    i_wifistatus = 0;
    i_btstatus = 0;
    i_auxstatus = 0;
    i_hidstatus = 0;
    i_usb_ipod_status = 0;
    i_messagestatus = 0;
    i_tboxstatus = 0;
    i_mutestatus = 0;
    i_currentMode = 0;
    i_Timerid = 0;
    m_temprstatus = 0;
//ren add start 0619
    m_aqsstatus = 0;
    m_acionstatus = 0;
//ren add end 0619
    i_dualSts = HMILIB_STATUSBAR_DUALMODE_INACTIVE;       //0: Single     1: Dual Mode
    i_autoSts = HMILIB_STATUSBAR_AUTOMODE_INACTIVE;
    i_airModeSts = HMILIB_STATUSBAR_AIRMODE_OFF;          //-1: don't display 0:face  1:face and foot  2:Foot 3:foot and screen  4:screen  15:Invalid
    i_windLevel = HMILIB_STATUSBAR_WINDLEVEL_OFF;           //-1: don't display 0x0: OFF-No wind      0x1: Level 1 ~ 0x7: Level 7
    i_offACModeSts = HMILIB_STATUSBAR_OFFACMODE_ACTIVE;     //AC OFF
    i_parkSensor = 1;       //0:display
    b_maintDisp = false;
	b_lowTempWarn = false;
    i_statusTime24 = 1;
    b_isDvrError = false;

    strStatusTime.clear();
    strStatusTime = "00:00";
    strStatusTimeAP = "";
    strStatusTimeMin = "00";
    strStatusTimeH = "00";

    strTemperature.clear();
    strTemperature = "";
    strLeftTemper = "";
    strRightTemper = "";

    m_configureDualSingle = 0;
    m_chargestatus = 0;
    updateTime();
}

Lib_Ctrl_Statusbar::~Lib_Ctrl_Statusbar()
{
    if (0 != i_Timerid)
    {
        killTimer(i_Timerid);
        i_Timerid = 0;
    }
}

void Lib_Ctrl_Statusbar::setConcreteAccessClass()
{
    Mode_Common* CommonMode =dynamic_cast<Mode_Common*>(Frm_CommonClassIns->getCmnModeClass());
    if( NULL != CommonMode)
    {
        connect(CommonMode, SIGNAL(sigPower()), this, SLOT(PowerSlot()));
        connect(CommonMode, SIGNAL(sigSingle()), this, SLOT(SingleSlot()));
        connect(CommonMode, SIGNAL(sigWifi()), this, SLOT(WifiSlot()));
        connect(CommonMode, SIGNAL(sigBT()), this, SLOT(BTSlot()));
        connect(CommonMode, SIGNAL(sigHID()), this, SLOT(HIDSlot()));
        connect(CommonMode, SIGNAL(sigUSBIPOD()), this, SLOT(USBIPODSlot()));
        connect(CommonMode, SIGNAL(sigAUX()), this, SLOT(AUXSlot()));
//        connect(CommonMode, SIGNAL(sigMessage()), this, SLOT(MessageSlot()));
//        connect(CommonMode, SIGNAL(sigTbox()), this, SLOT(TboxSlot()));
        connect(CommonMode, SIGNAL(sigMute()), this, SLOT(MuteSlot()));
        connect(CommonMode, SIGNAL(sigTemperatureStatus()), this, SLOT(temperatureStatusSlot()));
        connect(CommonMode, SIGNAL(sigAirModeChanged()), this, SLOT(airDistributionModeSlot()));
        connect(CommonMode, SIGNAL(sigAirDriverTempr()), this, SLOT(driverTemperSlot()));
        connect(CommonMode, SIGNAL(sigAirPassengerTempr()), this, SLOT(passengerTemperSlot()));
        connect(CommonMode, SIGNAL(sigAirBlowerSpeed()), this, SLOT(blowerSpeedLvlSlot()));
        connect(CommonMode, SIGNAL(sigParkSensor()), this, SLOT(parkSensorSlot()));
        connect(CommonMode, SIGNAL(sigAutoModeDisplay()), this, SLOT(autoModeSlot()));
        connect(CommonMode, SIGNAL(sigACOffModeDisplay()), this, SLOT(offACModeSlot()));
        connect(CommonMode, SIGNAL(sigDualModeDisplay()), this, SLOT(dualModeSlot()));
        connect(CommonMode, SIGNAL(sigTimemode()), this, SLOT(timeModeSlot()));
        connect(CommonMode, SIGNAL(sigMaintenanceDisp()), this, SLOT(maintDispSlot()));
        connect(CommonMode, SIGNAL(sigAutoConIsExist()), this, SLOT(autoConIsExistSlot()));
        connect(CommonMode, SIGNAL(sigUpdatatime()), this, SLOT(updatatimeSlot()));
        connect(CommonMode, SIGNAL(sigchargestatus()), this, SLOT(chargestatusslot()));
        connect(CommonMode, SIGNAL(sigConfigureDualSingleStsChg()), this, SLOT(configureDualSingleChgSlot()));
        connect(CommonMode, SIGNAL(sigModeStatusChanged()), this, SLOT(modeStatusChangedSlot()));
        connect(CommonMode, SIGNAL(sigDvrFaultStatus()), this, SLOT(dvrStatusSlot()));
        connect(CommonMode, SIGNAL(sigStatusbartemprChanged()), this, SLOT(statusbartemprSlot()));
//ren add start 0619
        connect(CommonMode, SIGNAL(sigAirQualitySts()), this, SLOT(AirQualitySlot()));
        connect(CommonMode, SIGNAL(sigAirIONSts()), this, SLOT(AirIONSlot()));
//ren add end 0619
    }
    else
    {
        HMI_LOG("Lib_Ctrl_Statusbar::setConcreteAccessClass  ( NULL == CommonMode)");
    }

    currentModeSlot();
    SingleSlot();
    WifiSlot();
    PowerSlot();
    BTSlot();
    HIDSlot();
    USBIPODSlot();
//    MessageSlot();
//    TboxSlot();
    MuteSlot();
    temperatureStatusSlot();
    parkSensorSlot();
    configureDualSingleChgSlot();
    dualModeSlot();
//    blowerSpeedLvlSlot();
    autoModeSlot();
    airDistributionModeSlot();
    driverTemperSlot();        //left
    passengerTemperSlot();     //right
    offACModeSlot();
    autoConIsExistSlot();
    maintDispSlot();
    chargestatusslot();
    dvrStatusSlot();
    statusbartemprSlot();
//ren add start 0619
    AirQualitySlot();
    AirIONSlot();
//ren add end 0619

    i_Timerid = startTimer(2000);

}

void Lib_Ctrl_Statusbar::timeModeSlot()
{
    updateTime();
    return;
}

void Lib_Ctrl_Statusbar::PowerSlot()
{

    return;
}

int Lib_Ctrl_Statusbar::PowerStatus() const
{
    return i_powerstatus;
}

void Lib_Ctrl_Statusbar::setPowerStatus(const int &var)
{
    i_powerstatus = var;
    emit PowerStatusChanged();
    return;
}

void Lib_Ctrl_Statusbar::SingleSlot()
{

    return;
}

int Lib_Ctrl_Statusbar::SingleStatus() const
{
    return i_singlestatus;
}

void Lib_Ctrl_Statusbar::setSingleStatus(const int &var)
{
    i_singlestatus = var;
    emit SingleStatusChanged();
    return;
}

void Lib_Ctrl_Statusbar::WifiSlot()
{

    return;
}

int Lib_Ctrl_Statusbar::WifiStatus() const
{
    return i_wifistatus;
}

void Lib_Ctrl_Statusbar::setWifiStatus(const int &var)
{
    i_wifistatus = var;
    emit WifiStatusChanged();
    return;
}

void Lib_Ctrl_Statusbar::modeStatusChangedSlot()
{
    HMI_LOG("IN");
    if(E_MODE_SRC_BTAUDIO == ModemanagerClassIns->Get_CurrentMode())
    {
        setPowerStatus(-1);
        setSingleStatus(-1);
        setBtStatus(2);
        HMI_LOG("E_MODE_SRC_BTAUDIO");
    }
    else
    {
        PowerSlot();
        SingleSlot();
        BTSlot();
    }
    HMI_LOG("ModemanagerClassIns->Get_CurrentMode()=%d",ModemanagerClassIns->Get_CurrentMode());
    return;
}

void Lib_Ctrl_Statusbar::BTSlot()
{
    int btpower = 0;

    return;
}

int Lib_Ctrl_Statusbar::BtStatus() const
{
    return i_btstatus;
}

void Lib_Ctrl_Statusbar::setBtStatus(const int &var)
{
    i_btstatus = var;
    emit BtStatusChanged();
    return;
}

void Lib_Ctrl_Statusbar::AUXSlot()
{
    setAuxStatus(1);
    return;
}

int Lib_Ctrl_Statusbar::AuxStatus() const
{
    return i_auxstatus;
}

void Lib_Ctrl_Statusbar::setAuxStatus(const int &var)
{
    i_auxstatus = var;
    emit AuxStatusChanged();
    return;
}

void Lib_Ctrl_Statusbar::HIDSlot()
{

    setHidStatus(i_hidstatus);

    return;
}

int Lib_Ctrl_Statusbar::HidStatus() const
{
    return i_hidstatus;
}

void Lib_Ctrl_Statusbar::setHidStatus(const int &var)
{
    i_hidstatus = var;
    emit HidStatusChanged();
    return;
}

void Lib_Ctrl_Statusbar::chargestatusslot()
{

    HMI_LOG("IN");
    m_chargestatus = false;

    return;
}

int Lib_Ctrl_Statusbar::chargestatus()
{
    return m_chargestatus;
}

void Lib_Ctrl_Statusbar::setchargestatus(const int &var)
{
    m_chargestatus = var;
    emit sigchargestatusChg();
    return;
}

void Lib_Ctrl_Statusbar::USBIPODSlot()
{
    int usbconnect = 0;
    int i_deviceType = 0;
    int i_deviceError = 0;

    return;

}

int Lib_Ctrl_Statusbar::UsbIpodStatus() const
{
    return i_usb_ipod_status;
}

void Lib_Ctrl_Statusbar::setUsbIpodStatus(const int &var)
{
    i_usb_ipod_status = var;
    emit UsbIpodStatusChanged();
    return;
}

void Lib_Ctrl_Statusbar::MessageSlot()
{
    setMessageStatus(1);
    return;
}

int Lib_Ctrl_Statusbar::MessageStatus() const
{
    return i_messagestatus;
}

void Lib_Ctrl_Statusbar::setMessageStatus(const int &var)
{
    i_messagestatus = var;
    emit MessageStatusChanged();
    return;
}

void Lib_Ctrl_Statusbar::TboxSlot()
{
    setTboxStatus(1);
    return;
}

int Lib_Ctrl_Statusbar::TboxStatus() const
{
    return i_tboxstatus;
}

void Lib_Ctrl_Statusbar::setTboxStatus(const int &var)
{
    i_tboxstatus = var;
    emit TboxStatusChanged();
    return;
}

void Lib_Ctrl_Statusbar::MuteSlot()
{
    int i_volumeFlag = 0;
    i_volumeFlag = AudioManagerClassIns->Get_muteflag();

    if (1 == i_volumeFlag)
    {
        setMuteStatus(1);
    }
    else
    {
        setMuteStatus(0);
    }
    return;
}

int Lib_Ctrl_Statusbar::MuteStatus() const
{
    return i_mutestatus;
}

void Lib_Ctrl_Statusbar::setMuteStatus(const int &var)
{
    i_mutestatus = var;
    emit MuteStatusChanged();
    return;
}

void Lib_Ctrl_Statusbar::temperatureStatusSlot()
{


    return;
}

QString Lib_Ctrl_Statusbar::temperature()const
{
    return strTemperature;
}

void Lib_Ctrl_Statusbar::setTemperatureStatus(const QString &var)
{
    strTemperature = var;
    emit sigTemperatureStatusChg();
}

QString Lib_Ctrl_Statusbar::statusTime() const
{
    return strStatusTime;
}

void Lib_Ctrl_Statusbar::setStatusTime(const QString &var)
{
    strStatusTime = var;
    emit sigStatusTimeChg();

    return;
}

QString Lib_Ctrl_Statusbar::statusTimeH() const
{
    return strStatusTimeH;
}

void Lib_Ctrl_Statusbar::setStatusTimeH(const QString &var)
{
    strStatusTimeH = var;
    emit sigStatusTimeHChg();

    return;
}

QString Lib_Ctrl_Statusbar::statusTimeMin() const
{
    return strStatusTimeMin;
}

void Lib_Ctrl_Statusbar::setStatusTimeMin(const QString &var)
{
    strStatusTimeMin = var;
    emit sigStatusTimeMinChg();

    return;
}

QString Lib_Ctrl_Statusbar::statusTimeAP() const
{
    return strStatusTimeAP;
}

void Lib_Ctrl_Statusbar::setStatusTimeAP(const QString &var)
{
    strStatusTimeAP = var;
    emit sigStatusTimeAPChg();

    return;
}

int  Lib_Ctrl_Statusbar::statusTime24() const
{
    return i_statusTime24;
}

void Lib_Ctrl_Statusbar::setStatusTime24(const int &var)
{
    i_statusTime24 = var;
    emit sigStatusTime24Changed();

    return;
}


void Lib_Ctrl_Statusbar::timerEvent(QTimerEvent* event)
{
    if (i_Timerid == event->timerId())
    {
        updateTime();
    }
}

void Lib_Ctrl_Statusbar::updateTime()
{
    int i_hour = 0;
    m_time = QDateTime::currentDateTime();
}

void Lib_Ctrl_Statusbar::currentModeSlot()
{
    i_currentMode = Frm_CommonClassIns->get_current_view_mode();
    HMI_LOG("i_currentMode = %d", i_currentMode);
    setCurrentMode(i_currentMode);
    return;
}

int Lib_Ctrl_Statusbar::currentMode() const
{
    return i_currentMode;
}

void Lib_Ctrl_Statusbar::setCurrentMode(const int &var)
{
    i_currentMode = var;
    emit currentModeChanged();
    return;
}

void Lib_Ctrl_Statusbar::offACModeSlot()
{
    unsigned char c_off_mode = 0;

    setOffACModeSts(i_offACModeSts);
}

int  Lib_Ctrl_Statusbar::offACModeSts() const
{
    return i_offACModeSts;
}

void Lib_Ctrl_Statusbar::setOffACModeSts(const int &var)
{
    i_offACModeSts = var;
    emit sigOffACModeStsChanged();
}

void Lib_Ctrl_Statusbar::airDistributionModeSlot()
{
    unsigned char c_air_distri_mode = 0;

    return;
}

int  Lib_Ctrl_Statusbar::airModeSts() const
{
    return i_airModeSts;
}

void Lib_Ctrl_Statusbar::setAirModeSts(const int &var)
{
   emit sigAirModeStsChanged();
}

void Lib_Ctrl_Statusbar::driverTemperSlot()
{

    return;
}

QString Lib_Ctrl_Statusbar::leftTemper()const
{
    return strLeftTemper;
}

void Lib_Ctrl_Statusbar::setLeftTemper(const QString &var)
{
    strLeftTemper = var;
    emit sigLeftTemperChanged();
}

void Lib_Ctrl_Statusbar::passengerTemperSlot()
{
    float f_passenger_set_temp = 0;
    setRightTemper(strRightTemper);

    return;
}

QString Lib_Ctrl_Statusbar::rightTemper()const
{
    return strRightTemper;
}

void Lib_Ctrl_Statusbar::setRightTemper(const QString &var)
{
    strRightTemper = var;
    emit sigRightTemperChanged();
}

void Lib_Ctrl_Statusbar::blowerSpeedLvlSlot()
{
    return;
}

int  Lib_Ctrl_Statusbar::windLevel() const
{
    HMI_LOG("QML i_windLevel = %d", i_windLevel);
    return i_windLevel;
}

void Lib_Ctrl_Statusbar::setWindLevel(const int &var)
{
    i_windLevel = var;
    emit sigWindLevelChanged();
}

void Lib_Ctrl_Statusbar::autoModeSlot()
{
    unsigned char c_auto_mode = 0;
   setAutoSts(i_autoSts);

    return;
}

int  Lib_Ctrl_Statusbar::autoSts() const
{
    HMI_LOG("QML i_autoSts = %d", i_autoSts);
    return i_autoSts;
}

void Lib_Ctrl_Statusbar::setAutoSts(const int &var)
{

    emit sigAutoStsChanged();
}

void Lib_Ctrl_Statusbar::dualModeSlot()
{
    unsigned char c_dual_mode = 0;

    setDualSts(i_dualSts);

    return;
}

int  Lib_Ctrl_Statusbar::dualSts() const
{
    return i_dualSts;
}

void Lib_Ctrl_Statusbar::setDualSts(const int &var)
{
    i_dualSts = var;
    emit sigDualStsChanged();
}

void  Lib_Ctrl_Statusbar::configureDualSingleChgSlot()
{
   setConfigureDualSingle(m_configureDualSingle);

    return;
}

int Lib_Ctrl_Statusbar::configureDualSingle() const
{
    return m_configureDualSingle;
}

void Lib_Ctrl_Statusbar::setConfigureDualSingle(const int &var)
{
    m_configureDualSingle = var;
    emit sigConfigureDualSingleChanged();

    return;
}

void Lib_Ctrl_Statusbar::parkSensorSlot()
{
    setParkSensor(i_parkSensor);
    return;
}

int Lib_Ctrl_Statusbar::parkSensor()
{
    return i_parkSensor;
}

void Lib_Ctrl_Statusbar::setParkSensor(const int &var)
{
    i_parkSensor = var;
    emit sigParkSensorChanged();
    return;
}

void Lib_Ctrl_Statusbar::maintDispSlot()
{
//    i_parkSensor = CanServiceIns->Get_PDC_Status_ParkSensor();
    HMI_LOG("b_maintDisp = %d", b_maintDisp);

    int i_remindSts1 = 0;
    int i_remindSts2 = 0;
    int i_remindSts3 = 0;

    int i_RemindSwitch = 0;
    int i_MaintaingMile = 0;
    int i_setMaintaingMile = 0;

    int i_MaintTireSwitch = 0;
    int i_MaintTireMile = 0;
    int i_setMaintTireMile = 0;
    int i_MaintTireYear = 0;
    int i_MaintTireMonth = 0;
    int i_MaintTireDay = 0;

    int i_MaintOilSwitch = 0;
    int i_MaintOilMile = 0;
    int i_setMaintOilMile = 0;
    int i_MaintOilYear = 0;
    int i_MaintOilMonth = 0;
    int i_MaintOilDay = 0;

    int i_year = 0;
    int i_month = 0;
    int i_day = 0;

    QDateTime m_time = QDateTime::currentDateTime();

    i_year = m_time.date().year();
    i_month = m_time.date().month();
    i_day = m_time.date().day();


    if ( 1 == i_RemindSwitch && 0 == i_MaintaingMile && 0 < i_setMaintaingMile )
    {
        i_remindSts1 = 1;
    }
    else
    {
        i_remindSts1 = 0;
    }

    if ( 1 == i_MaintTireSwitch )
    {
        if ( 0 == i_MaintTireMile && 0 < i_setMaintTireMile )
        {
            i_remindSts2 = 1;
        }
        else
        {
            if ( 0 < i_MaintTireYear )
            {
                if ( i_year > i_MaintTireYear )
                {
                    i_remindSts2 = 1;
                }
                else if ( i_year == i_MaintTireYear && i_month > i_MaintTireMonth )
                {
                    i_remindSts2 = 1;
                }
                else if ( i_year == i_MaintTireYear && i_month == i_MaintTireMonth && i_day >= i_MaintTireDay)
                {
                    i_remindSts2 = 1;
                }
                else
                {
                    i_remindSts2 = 0;
                }
            }
            else
            {
                i_remindSts2 = 0;
            }
        }
    }
    else
    {
        i_remindSts2 = 0;
    }

    if ( 1 == i_MaintOilSwitch )
    {
        if ( 0 == i_MaintOilMile && 0 < i_setMaintOilMile)
        {
            i_remindSts3 = 1;
        }
        else
        {
            if ( 0 < i_MaintOilYear )
            {
                if ( i_year > i_MaintOilYear )
                {
                    i_remindSts3 = 1;
                }
                else if ( i_year == i_MaintOilYear && i_month > i_MaintOilMonth )
                {
                    i_remindSts3 = 1;
                }
                else if ( i_year == i_MaintOilYear && i_month == i_MaintOilMonth && i_day >= i_MaintOilDay )
                {
                    i_remindSts3 = 1;
                }
                else
                {
                    i_remindSts3 = 0;
                }
            }
            else
            {
                i_remindSts3 = 0;
            }
        }
    }
    else
    {
        i_remindSts3 = 0;
    }

    HMI_LOG("i_remindSts1 = %d, i_remindSts2 = %d, i_remindSts3 = %d", i_remindSts1, i_remindSts2, i_remindSts3);
    if ( 1 == i_remindSts1 || 1 == i_remindSts2 || 1 == i_remindSts3 )
    {
        b_maintDisp = true;
    }
    else
    {
        b_maintDisp = false;
    }

    setMaintDisp(b_maintDisp);
    return;
}

bool Lib_Ctrl_Statusbar::maintDisp()
{
    return b_maintDisp;
}

void Lib_Ctrl_Statusbar::setMaintDisp(const bool &var)
{
    b_maintDisp = var;
    emit sigMaintDispChanged();
    return;
}

void Lib_Ctrl_Statusbar::autoConIsExistSlot()
{
    QString temperature = "";

}

void Lib_Ctrl_Statusbar::updatatimeSlot()
{
    HMI_LOG("ENTER");
    updateTime();
}

bool Lib_Ctrl_Statusbar::lowTempWarn()
{
    return b_lowTempWarn;
}

void Lib_Ctrl_Statusbar::setLowTempWarn(const bool &var)
{
    b_lowTempWarn = var;
    emit sigLowTempWarnChanged();
    return;
}

void Lib_Ctrl_Statusbar::dvrStatusSlot()
{
    int iFault_dvr = 0;
      return;
}

bool Lib_Ctrl_Statusbar::dvrIsError() const
{
    return b_isDvrError;
}

void Lib_Ctrl_Statusbar::setDvrIsError(const bool &var)
{
    b_isDvrError = var;
    emit sigDvrIsErrorChanged();
    return;
}

void Lib_Ctrl_Statusbar::statusbartemprSlot()
{
    setstatusbartemprSts(m_temprstatus);
    return;
}

int Lib_Ctrl_Statusbar::statusbartemprSts() const
{
    HMI_LOG(" ren2222 m_actype = %d", m_temprstatus);
    return m_temprstatus;
}
void Lib_Ctrl_Statusbar::setstatusbartemprSts(const int &var)
{
    m_temprstatus = var;
    emit sigstatusbartemprStsChanged();

    return;
}
//ren add start 0619
void Lib_Ctrl_Statusbar::AirQualitySlot()
{
  return;
}

int Lib_Ctrl_Statusbar::statusbarAqsSts() const
{
    HMI_LOG(" ren2222 m_aqsstatus = %d", m_aqsstatus);
    return m_aqsstatus;
}
void Lib_Ctrl_Statusbar::setstatusbarAqsSts(const int &var)
{
    m_aqsstatus = var;
    emit sigstatusbarAqsStsChanged();

    return;
}

void Lib_Ctrl_Statusbar::AirIONSlot()
{
    //int acionSetSts = 0;	//ren remove 0704
    int acionCanSts = 0;
    int AcionshowSts = 0;

    setstatusbarAcionSts(AcionshowSts);
    return;
//ren chg end 0704
}

int Lib_Ctrl_Statusbar::statusbarAcionSts() const
{
    HMI_LOG(" ren2222 m_acionstatus = %d", m_acionstatus);
    return m_acionstatus;
}
void Lib_Ctrl_Statusbar::setstatusbarAcionSts(const int &var)
{
    m_acionstatus = var;
    emit sigstatusbarAcionStsChanged();

    return;
}
//ren add end 0619



//zhanghanwu add 20180226
void Lib_Ctrl_Statusbar:: zhw_log(QString log)
{
    HMI_LOG("%s",log.toLatin1().data());
    return;
}
//zhanghanwu add 20180226

