#ifndef Mode_Common_H
#define Mode_Common_H

#include "hmifrm_modeclass.h"
#include "hmi_log.h"
#include <QDebug>
class Mode_Common : public Frm_AbstractModeClass
{
    Q_OBJECT

private:
    int m_iCurrentVolme;
    int m_iVolmeBeforeMute;
    int m_iCurrentEQType;
    int m_iCurrentLoudType;

    int m_iVolLongPressTimer;
    int m_isVolUpPressed;
    int m_isPausebyMute;
    int m_isHKSeekRelease;
    int m_muteflagbefore;
    int m_powerpressflag;
	int m_tunerlongpressUpTimer;
    int m_tunerlongpressDnTimer;
    int m_tbtonsshow;
    int m_usbLoading;
    int m_ipodejectTimer;
    int m_usbloadons;
    int m_itunerstatusbk;
    int m_checkSrcViewtimer;// this timer never be kill, will check the view and the source every 1seconds
    int message_sent_num ;          //add by yulei messagesent popup
    bool b_flag; //add by yulei messagesent popup
    // int m_dvrfirstflag;   //zhangwenbin delete 1123
public:

    Mode_Common(QObject* Framework=0,QObject* parent = 0, quint16 modeid = 0, quint16 screenid = 0,quint16 modeattr = 0);
    ~Mode_Common();

    static Frm_AbstractModeClass* constructer(QObject* Framework=0,QObject* parent = 0, quint16 modeid = 0, quint16 screenid = 0,quint16 modeattr = 0)
    {
        return new Mode_Common(Framework,parent,modeid,screenid,modeattr);
    }

    bool concreteEvent(QEvent *);
    int consumeEvent(QEvent *e);

    void concreteVisible();

    void concreteInvisible();

    void concreteStart();
    void concreteEnd();

    bool hardKeyEventProc(QEvent *e);
    int hmiEventProcess(QEvent *e);
    void hmiEventProc(QEvent *e);
	void modEventProc(QEvent *e);
    void TestScreen();
public slots:
    void set_cleartbtonsflag();
signals:
    void sigPower();
    void sigSingle();
    void sigWifi();
    void sigBT();
//ren add start 0619
    void sigAirQualitySts();
    void sigAirIONSts();
//ren add end 0619
    void sigHID();
    void sigUSBIPOD();
    void sigBtPairListReadyUser();
    void sigBtdeviceDeleteError();
    void sigAUX();
    void sigMessage();
    void sigTbox();
    void sigMute();
    void sigTimemode();
    void sigTemperatureStatus();
    void sigAirModeChanged();
    void sigStatusbartemprChanged();
    void sigAirDriverTempr();
    void sigAirPassengerTempr();
    void sigAirBlowerSpeed();
    void sigAutoModeDisplay();
    void sigACOffModeDisplay();
    void sigDualModeDisplay();
    void sigConfigureDualSingleStsChg();
    void sigModeStatusChanged();

    void sigtalkingtimeupdate();
    void sigtalktypeChanged();
    void sigdestroyTalkons();
    void sigshowTalkons();
    void sigphoneupdate();
    //SX7-977
      void sigphoneupdateend();
    void sigTimeChg();
    void sigParkSensor();

    void sigPDCFaultSensorFL();
    void sigPDCFaultSensorFML();
    void sigPDCFaultSensorFMR();
    void sigPDCFaultSensorFR();
    void sigPDCFaultSensorRL();
    void sigPDCFaultSensorRML();
    void sigPDCFaultSensorRMR();
    void sigPDCFaultSensorRR();
    void sigPDCFaultSensorMID();
    void sigPDCStatus_FaultSystem();
    void sigPDCDistanceLvFL();
    void sigPDCDistanceLvFML();
    void sigPDCDistanceLvFMR();
    void sigPDCDistanceLvFR();
    void sigPDCDistanceLvRL();
    void sigPDCDistanceLvRML();
    void sigPDCDistanceLvRMR();
    void sigPDCDistanceLvRR();
    void sigPDCDistanceLvMID();
    void sigPDCPadarNumber();
    void sigPDCVehicleTypeChange();
//    void sigDtcBusoff();
//    void sigDtcPdcTimeout();
    void sigCanPdcTimeout();
    void sigCanPdcNormalStop();
    void sigCanPdcNormalDisplay();
    void sigMaintenanceDisp();
    void sigAutoConIsExist();
    void sigUpdatatime();
    void sigLanguageType();
    void sigtbtchange();
    void sigSpeedchange();
    void sigchargestatus();
    void sigDvrFaultStatus();
    void sigCarplayAltChanged();
    //[SX5][SX5-6278][Neusoft][dong_b][2017/5/17][ADD][START]
    void sigIgnStatusChanged();
    //[SX5][SX5-6278][Neusoft][dong_b][2017/5/17][ADD][END]
private:
    void checkBaseViewmodeAndBaseSourcemode();
protected:
    void timerEvent(QTimerEvent *event);
};

#endif // Mode_Common_H

