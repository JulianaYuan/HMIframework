#ifndef HMIVIEW_HOME_H
#define HMIVIEW_HOME_H
#include "hmifrm_viewclass.h"


#define HMIVIEW_HOME_BTN_RELEASE            100
#define HMIVIEW_HOME_BTN_RADIO              101
#define HMIVIEW_HOME_BTN_MEDIA              102
#define HMIVIEW_HOME_BTN_NAVI               103
#define HMIVIEW_HOME_BTN_LINK               104
#define HMIVIEW_HOME_BTN_PHONE              105
#define HMIVIEW_HOME_BTN_AIR                106
#define HMIVIEW_HOME_BTN_SETTING            107
#define HMIVIEW_HOME_BTN_CANSETTING         108
#define HMIVIEW_HOME_BTN_CHARGE             109
#define HMIVIEW_HOME_BTN_PHONE_PRESS        110
#define HMIVIEW_HOME_BTN_DVR                111
#define HMIVIEW_HOME_BTN_SEND2CAR           112
//new button in second homepage[add][by jia.jl][start]
#define HMIVIEW_HOME_BTN_MESSAGEPUSH        113
#define HMIVIEW_HOME_BTN_PERIPHERALSERVICE  114
#define HMIVIEW_HOME_BTN_WEATHER            115
#define HMIVIEW_HOME_BTN_NEWS               116
#define HMIVIEW_HOME_BTN_MAINTENANCE        117
#define HMIVIEW_HOME_BTN_FAULTALERT         118
//new button in second homepage[add][by jia.jl][end]
//ren air manage add start 0523
#define HMIVIEW_HOME_BTN_AIRMANAGESYSTEM         119
//ren air manage add end 0523
#define HMIVIEW_HOME_BTN_BACKMODEL_RADIO         200
#define HMIVIEW_HOME_BTN_BACKMODEL_USB_MUSIC     201
#define HMIVIEW_HOME_BTN_BACKMODEL_IPOD          203
#define HMIVIEW_HOME_BTN_BACKMODEL_BT_MUSIC      202
#define HMIVIEW_HOME_BTN_BACKMODEL_CARPLAY       204

#define HMIVIEW_HOME_BTN_CURSOR_RADIO           301
#define HMIVIEW_HOME_BTN_CURSOR_MEDIA           302
#define HMIVIEW_HOME_BTN_CURSOR_NAVI            303
#define HMIVIEW_HOME_BTN_CURSOR_LINK            304
#define HMIVIEW_HOME_BTN_CURSOR_PHONE           305
#define HMIVIEW_HOME_BTN_CURSOR_AIR             306
#define HMIVIEW_HOME_BTN_CURSOR_SETTING         307
#define HMIVIEW_HOME_BTN_CURSOR_CANSETTING      308
#define HMIVIEW_HOME_BTN_CURSOR_CHARGE          309
#define HMIVIEW_HOME_BTN_CURSOR_PHONE_PRESS     310
#define HMIVIEW_HOME_BTN_CURSOR_DVR             311
#define HMIVIEW_HOME_BTN_CURSOR_SEND2CAR        312
//cursor of new button in second homepage[add][by jia.jl][start]
#define HMIVIEW_HOME_BTN_CURSOR_MESSAGEPUSH            313
#define HMIVIEW_HOME_BTN_CURSOR_PERIPHERALSERVICE      314
#define HMIVIEW_HOME_BTN_CURSOR_WEATHER                315
#define HMIVIEW_HOME_BTN_CURSOR_NEWS                   316
#define HMIVIEW_HOME_BTN_CURSOR_MAINTENANCE            317
#define HMIVIEW_HOME_BTN_CURSOR_FAULTALERT             318
//cursor of new button in second homepage[add][by jia.jl][end]
//ren air manage add start 0523
#define HMIVIEW_HOME_BTN_CURSOR_AIRMANAGESYSTEM             319
//ren air manage add end 0523
#define UNUSED_PARAMETER(x) (void)x

class View_Home : public Frm_AbstractViewClass,public CursorFeature
{
    Q_OBJECT
private:
    void init(void *info);
    bool hardKeyEventProc(QEvent *e);
    void knobPressAvtivePress();
    void knobPressAvtiveRelease();
    CursorCtrlClass *pCursorCtrl;
    PageDate_t st_pageDate;
    int iCurosrIndex;
    int chargeflag;
    int m_imagecreatlist;   //16603	SX7-2428-2
public:
    ~View_Home();
    View_Home(QObject* parent = 0, quint16 screenid = 0,quint16 attr = 0);
    static Frm_AbstractViewClass* constructer(QObject* parent = 0, quint16 screenid = 0,quint16 attr = 0)
    {
        return new View_Home(parent,screenid,attr);
    }

    void touchEventProc(QEvent *e);
    bool concreteEvent(QEvent *);
    bool concreteVisible();
    void Start() ;
    void End() ;
    bool concreteInvisible();
    bool hmiEventProc(QEvent *e);
    void modEventProc(QEvent *e);

    virtual void moveCursorTo(int i_cursorIndex);
    virtual void hideCursorFrom();
    virtual void TabBtnFoucsed(CursorTypeEnum en_type, int i_pos);
    virtual void TabBtnUnfoucsed(CursorTypeEnum en_type);
    virtual int get_totalNum();
    virtual int get_totalNumOfTabButton();
    virtual int get_curStartIndex();
    virtual int get_btn_sta_by_index(CursorTypeEnum en_type,int index);

    void update_cursor_recycle();
signals:
    void sig_getbackmodelv();
    void sig_getdata();
    void sig_gettime();
    void sig_getGPSstatus();

//    void sig_mouse_mid_Press();
//    void sig_mouse_mid_Releas();

    void sigCurrentStateReady();
    void sigChangeType();
    void sigUsbMusicReady();
    void sigTemperatureChanged();
    void sigLinkStatusChanged();
    void sig_Timechanged();
    void sig_TimeModechanged();
    void sigStationListReady();
    void sigAutoConIsExist();
    void sigVehicelType();
    void sigChargebuttonStatus();
    void sigCpsDeflectionChanged();
    void sigAutoSearchChange();
    void sig_canNodeStatusDVRChange();
    void sig_home_invisible();
    //[add][by jia.jl][start]
    void sig_TboxStatusChanged();
    //[add][by jia.jl][end]
    //ren air manage add start 0523
    void sig_AirManageSystemStatusChanged();
    //ren air manage add end 0523
};

#endif // FVIEW_HOME_H
