/*******************************************************
 File Name   : hmifrm_viewclass.cpp

 Created by  :  Shi.Jianhang(Neusoft)

 Created Date:  2015/07/15

Description   :   about page display and change
*********************************************************/


/****************Include  Section Begin***************************************************/

#include "../inc/hmifrm_viewclass.h"
#include "../inc/hmifrm_abstractimportview.h"
#include "hmiapi_modemanager.h"
/****************Include  Section End***************************************************/


/******************************************************************************
Function Name : Frm_AbstractViewClass
Description   : constructor
Date          : 2015/07/15
Parameter     :
para1         : QObject* parent
para2         : quint16 screenid
para3         : quint16 attr
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_AbstractViewClass::Frm_AbstractViewClass(QObject* parent, quint16 screenid, quint16 attr)
{
   // setParent(parent);

    screenID = screenid;
    m_attr = attr;
    b_visible = false;
    qmlViewComponent = NULL;
    m_qmlcacheflag = false;
    m_timerId = 0;
    Frm_AbstractModeClass* mode = dynamic_cast<Frm_AbstractModeClass*>(parent);


    if( NULL != mode )
    {
        modeClass = mode;
        Frm_CommonClass* frameworkclass = modeClass->getFrm_CommonClass();
        //parent_qmlView = frameworkclass->getMainQMLView()->rootObject();

    }
    else
    {
        Frm_AbstractViewClass* parentView = dynamic_cast<Frm_AbstractViewClass*>(parent);

        if( NULL != parentView )
        {
            modeClass = parentView->modeClass;

            switch (m_attr) {
            case 1:
               // parent_qmlView = parentView->qmlView;
              //  child_parent_qmlView = parentView->parent_qmlView;
                break;
            default:
                if (parentView->m_attr == 1)
                {
                   // parent_qmlView = parentView->child_parent_qmlView;
                }
                else
                {
                   // parent_qmlView = parentView->parent_qmlView;
                }
                break;
            }

        }
    }
}

/******************************************************************************
Function Name : customEvent
Description   : Recieve evnet
Date          : 2015/07/15
Parameter     : QEvent *e
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::customEvent(QEvent *e)
{
    viewEventProc(e);

    return;
}

/******************************************************************************
Function Name : customEvent
Description   : Recieve view evnet
Date          : 2015/07/15
Parameter     : QEvent *e
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
int Frm_AbstractViewClass::viewEventProc(QEvent *e)
{
    bool b_rc = false;
    int rc = 0;

    b_rc = concreteEvent(e);

    if(b_rc)
    {
        rc = 1;
    }

    return rc;
}

/******************************************************************************
Function Name : setvisible
Description   : Set page visible
Date          : 2015/07/15
Parameter     : bool visible
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::setvisible(bool visible)
{
    if(visible)
    {
        HMI_LOG("screen id:%d mode id:%d true",this->screenID,this->getModeClass()->getmodeID());
    }
    else
    {
        HMI_LOG("screen id:%d mode id:%d false",this->screenID,this->getModeClass()->getmodeID());
    }
    b_visible = visible;
}

/******************************************************************************
Function Name : visible
Description   : visible
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::visible()
{
    HMI_LOG("Enter");
    bool visible = concreteVisible();
    ModemanagerClassIns->Ope_ShowlayerManager(this->getModeClass()->getmodeID(),this->getscreenID());
    this->getModeClass()->printfAllView();
    if (visible)
    {
        HMI_LOG("setVisible");
        setvisible(visible);
        showView();
    }
    return;
}

/******************************************************************************
Function Name : showView
Description   : Show view
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::showView()
{
    HMI_LOG("IN");
    /*if (qmlView)
    {
        HMI_LOG("setVisible");
        qmlView->setVisible(true);
        if(0 != m_timerId)
        {
            killTimer(m_timerId);
            m_timerId = 0;
        }
        m_timerId = startTimer(100);
    }*/
    ModemanagerClassIns->send_lastscreen_save();
    ModemanagerClassIns->Req_call_hmi_is_ready();
    emit sigPropertyChange();
    HMI_LOG("setVisible");
    setvisible(true);
    return;
}

/******************************************************************************
Function Name : invisible
Description   : invisible
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::invisible()
{
    //if (qmlView)
    {
     //   qmlView->setVisible(false);
    }

    switch (m_attr) {
    case 1:
    {
        /*
        QQuickItem* rootObjectComponent = dynamic_cast<QQuickItem*>(qmlViewComponent);
        if (rootObjectComponent)
        {
            rootObjectComponent->setVisible(false);
        }
        */

        break;
    }
    default:
        break;
    }
    concreteInvisible();// SX7-630 revert

    setvisible(false);
    return;
}

/******************************************************************************
Function Name : setQml
Description   : Set page QML
Date          : 2015/07/15
Parameter     : QLatin1String QML
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::setQml(QLatin1String QML)
{    
    QObject* rootObject = NULL;
    QObject* p_temp = NULL;
    Frm_CommonClass* frameworkclass = modeClass->getFrm_CommonClass();

    if( true == m_qmlcacheflag )
    {
        p_temp = Frm_CommonClassIns->get_qmlcomponet(QString(QML));
    }

    switch (m_attr) {
    case 1:
    {
        if( NULL == p_temp )
        {
           /* QQmlComponent component(frameworkclass->getMainQMLView()->engine(), QUrl(QML));
            qmlViewComponent = component.create();
            if( true == m_qmlcacheflag )
            {
                Frm_CommonClassIns->set_qmlcomponet(QString(QML),qmlViewComponent);
            }*/
        }
        else
        {
            qmlViewComponent = p_temp;
        }
        rootObject = qmlViewComponent;

        //QQuickItem *item = qobject_cast<QQuickItem *>(rootObject);

        //item->setParentItem(parent_qmlView);

        //qmlView = NULL;//item;

        break;
    }
    default:
        if( NULL == p_temp )
        {
            //QQmlComponent component(frameworkclass->getMainQMLView()->engine(), QUrl(QML));
            //rootObject = component.create();
            if( true == m_qmlcacheflag )
            {
                Frm_CommonClassIns->set_qmlcomponet(QString(QML),rootObject);
            }
        }
        else
        {
            rootObject = p_temp;
        }
        //QQuickItem *item = qobject_cast<QQuickItem *>(rootObject);

        //item->setParentItem(parent_qmlView);

        //qmlView = NULL;//item;
        break;
    }

    QList<Frm_AbstractImportView*> allViewWidget = rootObject->findChildren<Frm_AbstractImportView*>();
    for(int i=0; i<allViewWidget.count();i++)
    {

        Frm_AbstractImportView* fImportView = allViewWidget.at(i);
        fImportView->setAccessClass(this);

        //QObject::connect( this, SIGNAL(sigPropertyChange()), fImportView, SIGNAL(sigAbstractPropertyChange()));
    }

}
/******************************************************************************
Function Name : getViewObject
Description   : Get View Object
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
QObject* Frm_AbstractViewClass::getViewObject()
{
    if (m_attr)
    {
        return qmlViewComponent;
    }
    else
    {
        return NULL;//dynamic_cast<QObject *>(qmlView);
    }
}

/******************************************************************************
Function Name : showMessage
Description   : Show Message
Date          : 2015/07/15
Parameter     : message
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::showMessage(QString message)
{
    Frm_CommonClassIns->showMessage(message);
    return;
}

void Frm_AbstractViewClass::showmusicMessage(QString message)
{
    Frm_CommonClassIns->showmusicMessage(message);

    return;
}

/******************************************************************************
Function Name : showMessage_noTimer
Description   : Show Message
Date          : 2015/07/15
Parameter     : message
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::showMessage_noTimer(QString message)
{
    Frm_CommonClassIns->showMessage(message);
    return;
}
/******************************************************************************
Function Name : showMessagenoTimer
Description   : Show Message
Date          : 2015/07/15
Parameter     : message
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::destroyshowMessage()
{
    Frm_CommonClassIns->dequeueMessage_byUser();
    return;
}

void Frm_AbstractViewClass::destroyshowMusicMessage()
{
    Frm_CommonClassIns->dequeueMusicMessage_byUser();
    return;
}
/******************************************************************************
Function Name : reqTrans
Description   : page change(same mode)
Date          : 2015/07/15
Parameter     : quint16 screenid
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::reqTrans(quint16 screenid)
{
    HMI_LOG("reqTransscreenid =%d",screenid);
    quint16 modeid = this->getModeClass()->getmodeID(); //wangtingting add on 0509
    HMI_LOG("modeid=%d",modeid);
    Frm_TransEvent* transEvent = new Frm_TransEvent(static_cast<QEvent::Type>(Frm_AbstractEvent::EN_EV_REQTRANS),modeid,screenid);

    Frm_CommonClass* frameworkclass = modeClass->getFrm_CommonClass();
    QCoreApplication::postEvent( frameworkclass, transEvent );
    return;
}

/******************************************************************************
Function Name : reqModeTrans
Description   : mode change
Date          : 2015/07/15
Parameter     :
para1         : quint16 modeid
para2         : quint16 screenid
para3         : ModeReqType reqtype
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::reqModeTrans(ViewModeEnum modeid , quint16 screenid , ModeReqType reqtype)
{
    HMI_LOG("@@@@@@reqModeTrans modeid =%d",modeid);
    HMI_LOG("@@@@@@reqModeTrans screenid =%d",screenid);
    HMI_LOG("@@@@@@reqModeTrans reqtype =%d",reqtype);
    Frm_ModeEvent* modeOnEvent = new Frm_ModeEvent(static_cast<QEvent::Type>(Frm_AbstractEvent::EN_EV_MODETRANS_ON),modeid, screenid ,reqtype);
    Frm_CommonClass* frameworkclass = Frm_CommonClassIns;
    QCoreApplication::postEvent( frameworkclass, modeOnEvent );
    return;
}

/******************************************************************************
Function Name : reqReturn
Description   : return prvious page
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::reqReturn()
{
    HMI_LOG("IN");
    quint16 modeid = this->getModeClass()->getmodeID(); //wangtingting add on 0509
    HMI_LOG("modeid=%d",modeid);
    Frm_TransEvent* transEvent = new Frm_TransEvent(static_cast<QEvent::Type>(Frm_AbstractEvent::EN_EV_REQTRANS),modeid,0, TRANS_PATTERN_RETURN);

    Frm_CommonClass* frameworkclass = modeClass->getFrm_CommonClass();
    QCoreApplication::postEvent( frameworkclass, transEvent );
    return;
}

/******************************************************************************
Function Name : reqChange
Description   : change current page to the top(same mode)
Date          : 2015/07/15
Parameter     : quint16 screenid
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::reqChange(quint16 screenid)
{
    HMI_LOG("reqChangescreenid =%d",screenid);
    quint16 modeid = this->getModeClass()->getmodeID(); //wangtingting add on 0509
    HMI_LOG("modeid=%d",modeid);
    Frm_TransEvent* transEvent = new Frm_TransEvent(static_cast<QEvent::Type>(Frm_AbstractEvent::EN_EV_REQTRANS),modeid,screenid,TRANS_PATTERN_CHANGE);

    Frm_CommonClass* frameworkclass = modeClass->getFrm_CommonClass();
    QCoreApplication::postEvent( frameworkclass, transEvent );
    return;
}

/******************************************************************************
Function Name : touchButton
Description   : button touched announce
Date          : 2015/07/15
Parameter     : quint16 buttonid
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::touchButton(quint16 buttonid)
{
    Frm_FuncEvent* funcEvent = new Frm_FuncEvent(static_cast<QEvent::Type>(Frm_AbstractEvent::EN_EV_REQFUNC),buttonid);

    //QCoreApplication::postEvent( this, funcEvent );
    return;
}

/******************************************************************************
Function Name : RegisterQmlInsView
Description   : RegisterQmlInsView
Date          : 2015/07/15
Parameter     :
para1         : Frm_AbstractViewClass* view
para2         : QLatin1String QML
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::RegisterQmlInsView(Frm_AbstractViewClass* view, QLatin1String QML)
{
    Frm_CommonClass* frameworkclass = modeClass->getFrm_CommonClass();
    //frameworkclass->getMainQMLView()->rootContext()->setContextProperty(QML, view);
    return;

}

/******************************************************************************
Function Name : getViewPara
Description   : Get View Para
Date          : 2015/07/15
Parameter     : quint16 screenid
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void* Frm_AbstractViewClass::getViewPara( quint16 screenid )
{
    return modeClass->getViewPara(screenid); //there seens something wrong,
}

/******************************************************************************
Function Name : setViewPara
Description   : Set View Para
Date          : 2015/07/15
Parameter     :
para1         : quint16 screenid
para2         : void* para
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::setViewPara( quint16 screenid, void* para )
{
    modeClass->setViewPara( screenid, para );
    return;
}

/******************************************************************************
Function Name : noticeCleareImportClass
Description   : Cleare import class
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::noticeCleareImportClass()
{
   // qmlView = NULL;
    return;
}

/******************************************************************************
Function Name : ~Frm_AbstractViewClass
Description   : Destructor
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_AbstractViewClass::~Frm_AbstractViewClass()
{
    switch (m_attr) {
    case 1:
        if(NULL != qmlViewComponent)
        {
            if( true == m_qmlcacheflag )
            {
                qmlViewComponent = NULL;
            }
            else
            {
                delete qmlViewComponent;
            }
        }
        break;
    default:
        /*if( NULL != qmlView )
        {
            if( true == m_qmlcacheflag )
            {
                qmlView = NULL;
            }
            else
            {
                delete qmlView;
            }
        }*/
        break;
    }
}

/******************************************************************************
Function Name : hideVolumeBar
Description   : hide Volume Bar
Date          : 2015/10/19
Parameter     :
Author        : MaoTianliang(Neusoft)
******************************************************************************/
void Frm_AbstractViewClass::hideVolumeBar()
{
    Frm_CommonClass* frameworkclass = Frm_CommonClassIns;

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

   // QMetaObject::invokeMethod(rootObject, "hideVolumeBar");

    return;
}

void Frm_AbstractViewClass::setqmlcachefalg(bool b_flag)
{
    m_qmlcacheflag = b_flag;
    return;
}

void Frm_AbstractViewClass::timerEvent(QTimerEvent *event)
{
    if(m_timerId == event->timerId())
    {
        ModemanagerClassIns->Req_transferred_over();
        //killTimer(m_timerId);
        m_timerId = 0;
    }
    return;
}
