#ifndef Frm_AbstractViewClass_H
#define Frm_AbstractViewClass_H

#include "hmifrm_modeclass.h"
#include "hmifrm_keyevent.h"
#include "hmi_log.h"
#include <QString>

#define D_HMIAPI_EV_COMMON_BUTTON_PRESSED  (0xF0FF)

class Frm_AbstractViewClass : public QObject
{
    Q_OBJECT

    private:
        quint16               screenID;
        quint16               m_attr; // 0: SW Menu   1:child/ons
        bool b_visible;
        //QQuickItem* qmlView;
        //QQuickItem* parent_qmlView;
        //QQuickItem* child_parent_qmlView;
        QObject* qmlViewComponent;
        Frm_AbstractModeClass*   modeClass;
        bool m_qmlcacheflag;
        int m_timerId;
    public:
        typedef struct
        {
            int timerId;
            int count;
        }S_Timer;
        Frm_AbstractViewClass(QObject* parent = 0, quint16 screenid = 0, quint16 attr = 0);
        ~Frm_AbstractViewClass();
        char* QML;
        char* QMLVIEW;
        quint16 getscreenID(){return screenID;}
        quint16 getscreenAttr(){return m_attr;}
        inline bool getvisible(){return b_visible;}
        void setvisible(bool visible);
        void showView();
        void reqReturn();
        void reqChange(quint16 screenid);
        void showMessage(QString message);
        void showmusicMessage(QString message);
        void showMessage_noTimer(QString message);
        void destroyshowMessage();
        void destroyshowMusicMessage();
        void RegisterQmlInsView(Frm_AbstractViewClass* view, QLatin1String QML);
        QObject* getViewObject();
        Frm_AbstractModeClass* getModeClass(){return modeClass;}
        void *getViewPara(quint16 screenid);
        void setViewPara(quint16 screenid, void *para);
        void customEvent(QEvent *);
        virtual bool concreteEvent(QEvent *) = 0;
        virtual void Start() = 0;
        virtual void End() = 0;
        int viewEventProc(QEvent *e);
        void setQml(QLatin1String QML);
        Q_INVOKABLE void reqTrans(quint16 screenid);
        void reqModeTrans(ViewModeEnum modeid , quint16 screenid , ModeReqType reqtype);
        void touchButton(quint16 buttonid);
        void visible();
        virtual bool concreteVisible() = 0;
        void invisible();
        virtual bool concreteInvisible() = 0;
        void noticeCleareImportClass();
        void reqDraw(){sigPropertyChange();}
        void hideVolumeBar();
        void setqmlcachefalg(bool b_flag);
        void timerEvent(QTimerEvent *event);
    signals:

        void sigPropertyChange();
};

#endif // Frm_AbstractViewClass_H

