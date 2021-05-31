#ifndef HMILIB_STATUSBAR_H
#define HMILIB_STATUSBAR_H

#include "hmifrm_abstractimportview.h"
#include "hmimode_common.h"
#include "QObject"
#include <QDateTime>

#define HMILIB_STATUSBAR_AIRMODE_OFF                   ( -1 )
#define HMILIB_STATUSBAR_AIRMODE_FACE                  ( 1 )
#define HMILIB_STATUSBAR_AIRMODE_FACEANDFOOT           ( 2 )
#define HMILIB_STATUSBAR_AIRMODE_FOOT                  ( 3 )
#define HMILIB_STATUSBAR_AIRMODE_SCREENANDFOOT         ( 4 )
#define HMILIB_STATUSBAR_AIRMODE_SCREEN                ( 5 )
#define HMILIB_STATUSBAR_AIRMODE_INVALID               ( 6 )

#define HMILIB_STATUSBAR_AUTOMODE_INACTIVE              ( 1 )
#define HMILIB_STATUSBAR_AUTOMODE_ACTIVE                ( 2 )
#define HMILIB_STATUSBAR_DUALMODE_INACTIVE              ( 1 )
#define HMILIB_STATUSBAR_DUALMODE_ACTIVE                ( 2 )
#define HMILIB_STATUSBAR_OFFACMODE_INACTIVE              ( 1 )
#define HMILIB_STATUSBAR_OFFACMODE_ACTIVE                ( 2 )

#define HMILIB_STATUSBAR_WINDLEVEL_OFF                  ( -1 )
//ren add start 0704
#define HMILIB_STATUSBAR_AQSCYCLE_OFF                  ( 0 )	//ren chg 0712
#define HMILIB_STATUSBAR_AIRION_OFF                  ( 0 )
//ren add end 0704

class Lib_Ctrl_Statusbar : public Frm_AbstractImportView
{
    Q_OBJECT
    Q_PROPERTY(int PowerStatus READ PowerStatus WRITE setPowerStatus NOTIFY PowerStatusChanged)
    Q_PROPERTY(int SingleStatus READ SingleStatus WRITE setSingleStatus NOTIFY SingleStatusChanged)
    Q_PROPERTY(int WifiStatus READ WifiStatus WRITE setWifiStatus NOTIFY WifiStatusChanged)
    Q_PROPERTY(int BtStatus READ BtStatus WRITE setBtStatus NOTIFY BtStatusChanged)
    Q_PROPERTY(int AuxStatus READ AuxStatus WRITE setAuxStatus NOTIFY AuxStatusChanged)
    Q_PROPERTY(int HidStatus READ HidStatus WRITE setHidStatus NOTIFY HidStatusChanged)
    Q_PROPERTY(int UsbIpodStatus READ UsbIpodStatus WRITE setUsbIpodStatus NOTIFY UsbIpodStatusChanged)
    Q_PROPERTY(int MessageStatus READ MessageStatus WRITE setMessageStatus NOTIFY MessageStatusChanged)
    Q_PROPERTY(int TboxStatus READ TboxStatus WRITE setTboxStatus NOTIFY TboxStatusChanged)
    Q_PROPERTY(int MuteStatus READ MuteStatus WRITE setMuteStatus NOTIFY MuteStatusChanged)
    Q_PROPERTY(int currentMode READ currentMode WRITE setCurrentMode NOTIFY currentModeChanged)
    Q_PROPERTY(int parkSensor READ parkSensor WRITE setParkSensor NOTIFY sigParkSensorChanged)
    Q_PROPERTY(bool maintDisp READ maintDisp WRITE setMaintDisp NOTIFY sigMaintDispChanged)
    Q_PROPERTY(bool lowTempWarn READ lowTempWarn WRITE setLowTempWarn NOTIFY sigLowTempWarnChanged)
    Q_PROPERTY(bool statusTime24 READ statusTime24 WRITE setStatusTime24 NOTIFY sigStatusTime24Changed)
    Q_PROPERTY(bool dvrIsError READ dvrIsError WRITE setDvrIsError NOTIFY sigDvrIsErrorChanged)

    Q_PROPERTY(int airModeSts READ airModeSts WRITE setAirModeSts NOTIFY sigAirModeStsChanged)
    Q_PROPERTY(int windLevel READ windLevel WRITE setWindLevel NOTIFY sigWindLevelChanged)
    Q_PROPERTY(int autoSts READ autoSts WRITE setAutoSts NOTIFY sigAutoStsChanged)
    Q_PROPERTY(int dualSts READ dualSts WRITE setDualSts NOTIFY sigDualStsChanged)
    Q_PROPERTY(int offACModeSts READ offACModeSts WRITE setOffACModeSts NOTIFY sigOffACModeStsChanged)
    Q_PROPERTY(int configureDualSingle READ configureDualSingle WRITE setConfigureDualSingle NOTIFY sigConfigureDualSingleChanged)

    Q_PROPERTY(QString leftTemper READ leftTemper WRITE setLeftTemper NOTIFY sigLeftTemperChanged)
    Q_PROPERTY(QString rightTemper READ rightTemper WRITE setRightTemper NOTIFY sigRightTemperChanged)

    Q_PROPERTY(QString temperature READ temperature WRITE setTemperatureStatus NOTIFY sigTemperatureStatusChg)
    Q_PROPERTY(QString statusTime READ statusTime WRITE setStatusTime NOTIFY sigStatusTimeChg)
    Q_PROPERTY(QString statusTimeH READ statusTimeH WRITE setStatusTimeH NOTIFY sigStatusTimeHChg)
    Q_PROPERTY(QString statusTimeMin READ statusTimeMin WRITE setStatusTimeMin NOTIFY sigStatusTimeMinChg)
    Q_PROPERTY(QString statusTimeAP READ statusTimeAP WRITE setStatusTimeAP NOTIFY sigStatusTimeAPChg)
    Q_PROPERTY(int chargestatus READ chargestatus WRITE setchargestatus NOTIFY sigchargestatusChg)
    Q_PROPERTY(int statusbartemprSts READ statusbartemprSts WRITE setstatusbartemprSts NOTIFY sigstatusbartemprStsChanged)
//ren add start 0619
    Q_PROPERTY(int statusbarAqsSts READ statusbarAqsSts WRITE setstatusbarAqsSts NOTIFY sigstatusbarAqsStsChanged)
    Q_PROPERTY(int statusbarAcionSts READ statusbarAcionSts WRITE setstatusbarAcionSts NOTIFY sigstatusbarAcionStsChanged)
//ren add end 0619
public:
    Lib_Ctrl_Statusbar();
    ~Lib_Ctrl_Statusbar();
    //Propeties
    int PowerStatus() const;
    void setPowerStatus(const int &var);

    int SingleStatus() const;
    void setSingleStatus(const int &var);

    int WifiStatus() const;
    void setWifiStatus(const int &var);

    int BtStatus() const;
    void setBtStatus(const int &var);

    int AuxStatus() const;
    void setAuxStatus(const int &var);

    int HidStatus() const;
    void setHidStatus(const int &var);

    int UsbIpodStatus() const;
    void setUsbIpodStatus(const int &var);

    int MessageStatus() const;
    void setMessageStatus(const int &var);

    int TboxStatus() const;
    void setTboxStatus(const int &var);

    int MuteStatus() const;
    void setMuteStatus(const int &var);

    void setConcreteAccessClass();

    int currentMode() const;
    void setCurrentMode(const int &var);

    int  airModeSts() const;
    void setAirModeSts(const int &var);

    int  windLevel() const;
    void setWindLevel(const int &var);

    int  autoSts() const;
    void setAutoSts(const int &var);

    int  dualSts() const;
    void setDualSts(const int &var);

    int  offACModeSts() const;
    void setOffACModeSts(const int &var);

    int parkSensor();
    void setParkSensor(const int &var);

    bool maintDisp();
    void setMaintDisp(const bool &var);

    bool lowTempWarn();
    void setLowTempWarn(const bool &var);

    int chargestatus();
    void setchargestatus(const int &var);

    bool dvrIsError() const;
    void setDvrIsError(const bool &var);

    QString leftTemper()const;
    void setLeftTemper(const QString &var);

    QString rightTemper()const;
    void setRightTemper(const QString &var);

    QString temperature()const;
    void setTemperatureStatus(const QString &var);

    QString statusTime() const;
    void setStatusTime(const QString &var);

    QString statusTimeH() const;
    void setStatusTimeH(const QString &var);

    QString statusTimeMin() const;
    void setStatusTimeMin(const QString &var);

    QString statusTimeAP() const;
    void setStatusTimeAP(const QString &var);

    int  statusTime24() const;
    void setStatusTime24(const int &var);

    int  configureDualSingle() const;
    void setConfigureDualSingle(const int &var);

    int  statusbartemprSts() const;
    void setstatusbartemprSts(const int &var);

//ren add start 0619
    int  statusbarAqsSts() const;
    void setstatusbarAqsSts(const int &var);

    int  statusbarAcionSts() const;
    void setstatusbarAcionSts(const int &var);
//ren add end 0619

public slots:
    void PowerSlot();
    void SingleSlot();
    void WifiSlot();
    void BTSlot();
    void AUXSlot();
    void HIDSlot();
    void USBIPODSlot();
    void MessageSlot();
    void TboxSlot();
    void MuteSlot();
    void temperatureStatusSlot();
    void currentModeSlot();
    void airDistributionModeSlot();
    void driverTemperSlot();        //left
    void passengerTemperSlot();     //right
    void blowerSpeedLvlSlot();
    void autoModeSlot();
    void offACModeSlot();
    void dualModeSlot();
    void timeModeSlot();
    void parkSensorSlot();
    void maintDispSlot();
    void autoConIsExistSlot();
    void updatatimeSlot();
    void configureDualSingleChgSlot();
    void modeStatusChangedSlot();
	void chargestatusslot();
    void dvrStatusSlot();
    void statusbartemprSlot();
//ren add start 0619
    void AirQualitySlot();
    void AirIONSlot();
//ren add end 0619
//zhanghanwu add 20180226
    void zhw_log(QString log);
//zhanghanwu add 20180226
signals:
    void PowerStatusChanged();
    void SingleStatusChanged();
    void WifiStatusChanged();
    void BtStatusChanged();
    void AuxStatusChanged();
    void HidStatusChanged();
    void UsbIpodStatusChanged();
    void MessageStatusChanged();
    void TboxStatusChanged();
    void MuteStatusChanged();
    void sigTemperatureStatusChg();
    void sigStatusTimeChg();
    void currentModeChanged();
    void sigAirModeStsChanged();
    void sigWindLevelChanged();
    void sigLeftTemperChanged();
    void sigRightTemperChanged();
    void sigParkSensorChanged();
    void sigAutoStsChanged();
    void sigDualStsChanged();
    void sigOffACModeStsChanged();
    void sigMaintDispChanged();
    void sigLowTempWarnChanged();
    void sigStatusTimeHChg();
    void sigStatusTimeMinChg();
    void sigStatusTimeAPChg();
    void sigStatusTime24Changed();
    void sigConfigureDualSingleChanged();
    void sigchargestatusChg();
    void sigDvrIsErrorChanged();
    void sigstatusbartemprStsChanged();
//ren add start 0619
    void sigstatusbarAqsStsChanged();
    void sigstatusbarAcionStsChanged();
//ren add end 0619
private:
    bool b_isDvrError;
    bool b_lowTempWarn;
    bool b_maintDisp;
    int i_powerstatus;
    int i_singlestatus;
    int i_wifistatus;
    int i_btstatus;
    int i_auxstatus;
    int i_hidstatus;
    int i_usb_ipod_status;
    int i_messagestatus;
    int i_tboxstatus;
    int i_mutestatus;
    int i_currentMode;
    int i_Timerid;
    int i_HourSystem;
    int i_airModeSts;         //1:face    2:face and foot    3:Foot   4:foot and screen  5:screen    6:Invalid
    int i_windLevel;
    int i_parkSensor;           //0:display
    int i_autoSts;            //0:noaudo      1:audo
    int i_dualSts;            //0:sync        1:dual
    int i_offACModeSts;
    int i_statusTime24;
    int m_configureDualSingle;      //0:single  1:dual
    int m_chargestatus;
    int m_temprstatus;
//ren add start 0619
    int m_aqsstatus;
    int m_acionstatus;
//ren add end 0619
    QDateTime m_time;
    QString strStatusTime;
    QString strStatusTimeH;
    QString strStatusTimeMin;
    QString strStatusTimeAP;

    QString strTemperature;
    QString strLeftTemper;
    QString strRightTemper;

protected:
    void timerEvent(QTimerEvent* event);
    void updateTime();
};
#endif // HMILIB_STATUSBAR_H

