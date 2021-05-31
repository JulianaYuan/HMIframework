#ifndef Frm_CommonClass_H
#define Frm_CommonClass_H

#include <QtGui/QGuiApplication>
//#include "qtquick2applicationviewer.h"
//#include <QQmlApplicationEngine>
//#include <QQmlComponent>
//#include <QQmlContext>
//#include <QQuickItem>
//#include <QQuickView>
#include <QQueue>

/*
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeComponent>
*/

#include "hmifrm_modevent.h"
#include "../inc/hmifrm_apiclass.h"
#include "hmifrm_modemanager.h"
#include "../../PAGE/base/cursorctrlclass.h"

#include <QTextCodec>
//#include "start.h"

#define HMI_VOLUME_MAX  (30)

typedef enum tagViewModeEnum
{
    EN_VIEW_MODE_NULL,          //--0
    EN_VIEW_MODE_HOME,          //--9
    EN_VIEW_MODE_MAX //44
}ViewModeEnum;

class Frm_AbstractModeClass;
class Frm_AbstractViewClass;
class Frm_ApiClass;

class Frm_CommonClass : public QObject
{
    Q_OBJECT

private:
    Frm_CommonClass(QObject* parent = 0);
    int modeTrans(QEvent *e);
    Frm_AbstractModeClass* modeClass;   /**the top mode you can see on screen**/
    void customEvProc(QEvent *e);
    Frm_AbstractModeClass* CmnmodeClass; /**the Mode_Common, normal mode 's parent mode, it is exist at any time **/
    int ModeClassPopup(quint16 modeid);
    int mode_proc_on_base(Frm_ModeEvent* fModeEvent);
    int mode_proc_on_replace(Frm_ModeEvent* fModeEvent);//juanjuan add
    int mode_proc_on_interrupt(Frm_ModeEvent* fModeEvent);
    int mode_proc_on_off(Frm_ModeEvent* fModeEvent);
    void initCommonMode();
    void initMainQML();
    static Frm_CommonClass* m_instance; /**the instance of Frm_CommonClass**/
    int screenTrans(QEvent *e);

    void special_key_filter(QEvent *e);
    void timerEvent(QTimerEvent* event);

    //QtQuick2ApplicationViewer* main_qmlView; /**QtQuick2ApplicationViewer source is main.qml, the main qml of this application,exist at any time**/
    Frm_ApiClass* m_ApiClass; /*thread for dbus*/
    Frm_modemanager m_ModeManager;/*the instance of class Frm_modemanager*/

    CursorTypeEnum m_enCursorType;/*�������*/
    CursorstatusEnum m_enCursorstatus;/*���״̬*/
    /**��¼���λ��  start**/
    int m_ipos;
    int m_isettingpos;
    int m_home_pos;
    /**��¼���λ��  end**/

    int i_isboot;/*keep applicationStatusSlot run once*/
    int i_avmtimer;/*������ҪHMIͨ��IPC���������������ģ����AVM RVC DVR�ȣ���һ��80ms��timer �Զ�ʱ�������꣬*/
    int i_usbimer;//13720	SX5-5696
    int m_knobpressflag;/*��־hardkey OK�Ƿ�ѹ�£���ѹ����1��������0*/
    /*touch pos need send by IPC start */
    int m_avmposX;
    int m_avmposY;
    int m_avmposX_OFF;
    int m_avmposY_OFF;
    /*touch pos need send by IPC end */
    int m_touchbeforeaccoff;/*only used in eventfilter, ��¼touch begin ��panel disable֮ǰ��touch end��panel disable����HMI is not ready֮�󣬿�����Ϊ�˱���һЩ�������Ļ����touch���ӵ�flag*/
    int m_touchpermitstatus;/*only used in eventfilter,it should be a local variable, ��¼��ǰ������ʹ��״̬*/
    QQueue<QString> m_Popup_queue;/*������Ϣ����*/
    QString m_mesaage_shown;/*��ǰ��ʾ�ĵ�����Ϣ�ַ���*/
    int isLoadingInterrupt;/*usb ipod ý������е�����Ϣ��ʾflag��������������ʾ���Զ���ȥ������������*/
    int m_transModeId;/*��¼������source �Լ�hmi mode*/
    bool isAlreadyReq(QString &message);
    void createMessage(QString message);
    void createMessage_noTimer(QString message);/*no used*/
    QMap<QString,QObject*> m_qmap;/*map for qml path string and QObject of view Class*/
public:
    //Mode Class Table
    typedef struct {
        //Mode ID
        const char *modename;//juanjuan add ,give every view mode a string name
        quint16 modeid;
        quint16 srcMode;//juanjuan add, keep the source for every view mode
        quint16 modeAttr;
        void *func;
        void* para;
    }S_MODECLASS_TABLE;
    ~Frm_CommonClass();
    static Frm_CommonClass* getInstance();
    void init();
    char* QML;
    void customEvent(QEvent *);

    Frm_AbstractModeClass* getModeClass(){return modeClass;}
    Frm_AbstractModeClass* getCmnModeClass(){return CmnmodeClass;}
    //QtQuick2ApplicationViewer* getMainQMLView(){return main_qmlView;}
    void ShowMainQMLView();
    int viewModeIdOfBaseSrcMode();
    void setCursorStatus(CursorstatusEnum en_status);
    void getCursorStatus(CursorstatusEnum *pen_status);
    void setCursorType(CursorTypeEnum en_type, int ipos);
    void getCursorType(CursorTypeEnum *pen_type, int *pi_pos);
    void setSettingCursorpos(int ipos);
    void getSettingCursorpos(int* pi_pos);
    void backFromNaviMode();
    int getHomecursor();
    void setHomecursor(int i_pos);
    //[SX7][SX7-54][Neusoft][dong_b][2017/6/12][ADD][START]
    int getUSBMcursor();
    void setUSBMcursor(int i_pos);
    //[SX7][SX7-54][Neusoft][dong_b][2017/6/12][ADD][START]
    int getMediaUsbcursor();
    void setMediaUsbcursor(int i_pos);
    int getMediaUsbShowcursor();
    void setMediaUsbShowcursor(int i_pos);
    int getMediaMaincursor();
    void setMediaMaincursor(int i_pos);
    int getMediaMainShowcursor();
    void setMediaMainShowcursor(int i_pos);

    void Device_plugOut(int i_devtype);
    int get_current_view_mode();
    int get_current_view_screenid();
    void Movie_forbid_previous_mode(int transtype);
    void init_audioSource();

    void showMessage(QString message);
    void showmusicMessage(QString message);
    void dequeueMessage_byUser();
    void dequeueMusicMessage_byUser();
    void dequeueMessage_auto();
    bool find_inputmode(int mode);
    bool find_nextmode();
    bool fined_nextinputmode(int mode);
    const char *getModeName(quint16 modeid);//juanjuan add to printf the mode name char
    int getModeSource(quint16 modeid);//juanjuan add to printf the mode name char
    int getTopModeAttr();//juanjuan add to find current top mode
    int find_parentmode(Frm_AbstractModeClass* mode);//juanjuan add to check if this mode have parent except the common mode
    int find_childmode(Frm_AbstractModeClass* mode);//juanjuan add to check if this mode have child
    bool eventFilter(QObject * obj,QEvent * e);
    QObject* get_qmlcomponet(QString url);
    void set_qmlcomponet(QString url,QObject* qmlcomponent);
    void delet_qmlcomponet(QString url);

public slots:
    void applicationStatusSlot(Qt::ApplicationState state);

protected:
    S_MODECLASS_TABLE* m_ModeClassTable;
    int ModeClassTableNum;

};

#define Frm_CommonClassIns    Frm_CommonClass::getInstance()

#endif // Frm_CommonClass_H

