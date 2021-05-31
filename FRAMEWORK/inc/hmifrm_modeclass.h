#ifndef Frm_AbstractModeClass_H
#define Frm_AbstractModeClass_H

//FrameWorkClass
#include "hmifrm_common.h"

#include "hmifrm_transevent.h"
#if 1 //KATO_ADD
#include "hmifrm_funcevent.h"
#include "hmifrm_navitohmievent.h"
//#include "../navi/inc/qthmi.h"
//#include "../navi/inc/qthmitonavi_event.h"
#endif

#define HMI_VOLUME_LONGPRESS_INTERVAL 300
#define HMI_VOLUME_LONGPRESS_INTERVAL_2 100

class Frm_AbstractModeClass : public QObject
{
    Q_OBJECT

    private:

        quint16     modeID;
        quint16     screenID;
        quint16     modeAttr;// 0:common , 1:base , 2:Interrupt
        quint16     modeStatus;// 1:0% ON , 2:100% ON
        //Frm_CommonClass
        Frm_CommonClass* frameWorkClass;
        void reqTransEvent(QEvent *e);
        void returnView();
        void changeView(quint16 screenid);

    public:

        typedef struct {
            const char *screenname;  // give every screen a string name
            quint16 screenid;  // if this mode have screen, the id start from 1,if it haven't screen, give a 0 id.
            quint16 attr;   // 0: SW Menu   1:child/ons
            void *func;
            void* para;

        }S_VIEWTABLE;
        typedef struct
        {
            int timerId;
            int count;
        }S_Timer;
        Frm_AbstractModeClass(QObject* Framework,QObject* parent = 0, quint16 modeid = 0, quint16 screenid = 0, quint16 modeattr = 1);
        ~Frm_AbstractModeClass();
        void changeParent(QObject* parent = 0);//juanjuan add ,to change the mode's parent mode
        quint16 getmodeID(){return modeID;}

        inline quint16 getscreenID(){return screenID;}
        inline void setscreenID(quint16 id){screenID = id;HMI_LOG("screenID = %d",screenID);}

        inline quint16 getmodeAttr(){return modeAttr;}
        inline quint16 getmodeStatus(){return modeStatus;}
        inline void setmodeStatus(quint16 Status){modeStatus = Status;}

        Frm_CommonClass* getFrm_CommonClass(){return frameWorkClass;}


        int modeEventProc(QEvent *);
        void customEvent(QEvent *);

        virtual bool concreteEvent(QEvent *) = 0;
        virtual int consumeEvent(QEvent *e) = 0;//juanjuan add,to redefine the event process flow

        void visible();

        virtual void concreteVisible() = 0;

        void invisible();

        virtual void concreteInvisible() = 0;


        void  Start();
        void  End();

        virtual void concreteStart() = 0;
        virtual void concreteEnd() = 0;

        void constructView(quint16 screenid = 0,quint16 modeSts = MODE_ON_PERCENT_100);
        //void reqViewChange(quint16 screenid);//juanjuan add to change screen in the same mode
        void printfAllView(void);//juanjuan add to printf all the screen name in the mode
        void reqModeTrans(ViewModeEnum modeid , quint16 screenid , ModeReqType reqtype);

        Frm_AbstractViewClass* getView(quint16 screenid);

        const char *getscreenName(int screenid);//juanjuan add to get the screen name by screen id
        Frm_AbstractModeClass* get_parentmode();//juanjuan add to check if this mode have parent except the common mode
        Frm_AbstractModeClass* get_childmode();//juanjuan add to check if this mode have child
        void* getViewPara(quint16 screenid);
        void setViewPara(quint16 screenid, void* para);
        void showMessage(QString message);
        void showmusicMessage(QString message);
        void showMessage_noTimer(QString message);
        void showPhoneUpdatePopup();
        void destroyshowMessage();
        void destroyshowMusicMessage();
        void destroyshowMessageNoanimation();
        void destroyPhoneUpdatePopup();
        void showVolumeBar(qint16 i_value);
        void hideVolumeBar();
        void showTalkons();
        void destroyTalkons();
        void showDiagons();
        void destroyDiagons();
        void touch_notpermit();
        void touch_permit();
		void showPDCkons();
        void destroyPDCkons();
        void showCarplayons();
        void destroyCarplayons();
        void shownavitbtons();
        void destroynavitbtons();
        void show_dvr_ons(QString message);
        void destroy_dvr_ons();
    protected:
        S_VIEWTABLE* ViewTable;
        int ViewTableNum;

};

#endif // Frm_AbstractModeClass_H

