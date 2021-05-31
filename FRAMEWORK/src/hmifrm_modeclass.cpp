/*******************************************************
File Name   : hmifrm_keyevent.cpp

Created by  :  Shi.Jianhang(Neusoft)

Created Date:  2015/07/15

Description   :   event management(key)
*********************************************************/

/****************Include  Section Begin***************************************************/
#include "../inc/hmifrm_modeclass.h"
#include "hmifrm_viewclass.h"
#include "hmiapi_modemanager.h"
/****************Include  Section End***************************************************/
const Frm_AbstractModeClass::S_VIEWTABLE viewnullTable[] =
{
   {"NULL",   0, 0, (void*)(NULL),NULL},
};
/******************************************************************************
Function Name : Frm_AbstractModeClass
Description   : constructor
Date          : 2015/07/15
Parameter     :
para1         : QObject* Framework
para2         : QObject* parent
para3         : quint16 modeid
para4         : quint16 screenid
para4         : quint16 modeattr
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/

/******************************************************************************
Function Name : Frm_AbstractModeClass
Description   : constructor
Date          : 2015/07/15
Parameter     :
para1         : QObject* Framework
para2         : QObject* parent
para3         : quint16 modeid
para4         : quint16 screenid
para5         : quint16 modeattr
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_AbstractModeClass::Frm_AbstractModeClass(QObject* Framework,QObject* parent, quint16 modeid, quint16 screenid, quint16 modeattr )
{
    setParent(parent);

    modeID = modeid;
    screenID = screenid;
    modeAttr = modeattr;

    frameWorkClass = dynamic_cast<Frm_CommonClass*>(Framework);
}
/******************************************************************************
Function Name : changeParent
Description   : changeParent
Date          : 2017/10/24
Parameter     :
para2         : QObject* parent
Author        : juanjuan (visteon)
******************************************************************************/
void Frm_AbstractModeClass::changeParent(QObject* parent)
{
    setParent(parent);
    HMI_LOG("setParent");

}
/******************************************************************************
Function Name : modeEventProc
Description   : mode event management
Date          : 2015/07/15
Parameter     : QEvent *e
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
int Frm_AbstractModeClass::modeEventProc(QEvent *e)
{
    int rc = 0;
    Frm_AbstractEvent* fAbstractEvent = dynamic_cast<Frm_AbstractEvent*>(e);

    if( NULL != fAbstractEvent )
    {
        switch( fAbstractEvent->getEventType() )
        {
        case Frm_AbstractEvent::EN_TYPE_REQTRANS:
            reqTransEvent(e);
            rc = 1;
            break;
        default:
            QList<Frm_AbstractViewClass*> allView = this->findChildren<Frm_AbstractViewClass*>();
            int count = allView.count();
            HMI_LOG("view count = %d",count);

            for(int i=count-1; i>=0; i--)
            {
                Frm_AbstractViewClass* view = allView.at(i);
                HMI_LOG("screen id[%d] name [%s]",view->getscreenID(),getscreenName(view->getscreenID()));

                if ((NULL != view) && view->getvisible() && (rc == 0))
                {
                    HMI_LOG("screen [%s] is visible",getscreenName(view->getscreenID()));
                    rc |= view->viewEventProc(e);
                }
                else
                {
                    HMI_LOG("screen [%s] is invisible",getscreenName(view->getscreenID()));
                    break;
                }
            }

            if (rc == 0)
            {
                rc = this->concreteEvent(e);
            }
            break;
        }
    }

    return rc;
}

/******************************************************************************
Function Name : customEvent
Description   : receive event
Date          : 2015/07/15
Parameter     : QEvent *e
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractModeClass::customEvent(QEvent *e)
{
    modeEventProc(e);
}

/******************************************************************************
Function Name : reqTransEvent
Description   : trans event management
Date          : 2015/07/15
Parameter     : QEvent *e
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractModeClass::reqTransEvent(QEvent *e)
{
    Frm_TransEvent* fTransEvent = dynamic_cast<Frm_TransEvent*>(e);

    if( NULL != fTransEvent )
    {
        if(fTransEvent->getModeID() != this->getmodeID()) //wangtingting add on 0509
        {
            HMI_LOG("return");
            return;
        }
        switch (fTransEvent->getPattern()) {
        case TRANS_PATTERN_NORMAL:
            HMI_LOG("constructView");
            constructView(fTransEvent->getScreenID());
            break;
        case TRANS_PATTERN_RETURN:
            returnView();
            break;
        case TRANS_PATTERN_CHANGE:
            changeView(fTransEvent->getScreenID());
            break;
        default:
            break;
        }


    }
}
/******************************************************************************
Function Name : printfAllView
Description   : printf all screen id and name of this mode
Date          : 2017/10/10
Parameter     : quint16 screenid
Author        : juanjuan(Visteon)
******************************************************************************/
void Frm_AbstractModeClass::printfAllView(void)
{
    QList<Frm_AbstractViewClass*> allView = this->findChildren<Frm_AbstractViewClass*>();

    int icount = allView.count();
    for(int i=0; i<icount; i++)
    {
        Frm_AbstractViewClass* pViewClass = allView.at(i);
        HMI_LOG("modeId:%d screenid:%d [%s]",pViewClass->getModeClass()->getmodeID(),pViewClass->getscreenID(),getscreenName(pViewClass->getscreenID()));
    }
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
void Frm_AbstractModeClass::reqModeTrans(ViewModeEnum modeid , quint16 screenid , ModeReqType reqtype)
{
    HMI_LOG("@@@@@@reqModeTrans mode %s[%d]",Frm_CommonClassIns->getModeName(modeid),modeid);
    HMI_LOG("@@@@@@reqModeTrans screenid =%d",screenid);
    HMI_LOG("@@@@@@reqModeTrans reqtype =%d",reqtype);

    Frm_ModeEvent* modeOnEvent = new Frm_ModeEvent(static_cast<QEvent::Type>(Frm_AbstractEvent::EN_EV_MODETRANS_ON),modeid, screenid ,reqtype);
    Frm_CommonClass* frameworkclass = Frm_CommonClassIns;
    QCoreApplication::postEvent( frameworkclass, modeOnEvent );
    return;
}

/******************************************************************************
Function Name : visible
Description   : Mode visible
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractModeClass::visible()
{
    concreteVisible();

    QList<Frm_AbstractViewClass*> allView = this->findChildren<Frm_AbstractViewClass*>();
    int count = allView.count();
    HMI_LOG("count =%d",count);
#if 1 // juanjuan add start
    if(count == 0)
    {
        HMI_LOG("ViewTableNum =%d",ViewTableNum);
        for(int i=0; i<ViewTableNum; i++)
        {
            HMI_LOG("this->getscreenID() =%d,ViewTable[%d].screenid =%d",this->getscreenID(),i,ViewTable[i].screenid);
            if( this->getscreenID() == ViewTable[i].screenid )
            {
                constructView(this->getscreenID(), MODE_ON_PERCENT_100);
                break;
            }
            else
            {
                HMI_LOG("this mode haven't a screen");
            }
        }
    }
    else
#endif // juanjuan add end
    {
       HMI_LOG("mode have created view");
        Frm_AbstractViewClass* pCurView = NULL;
        for(int i=count-1; i>=0; i--)
        {
            Frm_AbstractViewClass* view = allView.at(i);
            if(view->getscreenID() == this->getscreenID())
            {
                HMI_LOG("mode want show screen exist");
                pCurView = view;
                break;
            }
            else
            {
                HMI_LOG("delete dismatch view");
               if(view->getModeClass()->getmodeID() == this->getmodeID())
               {
                   HMI_LOG("===delete screen id [%d],screen name [%s]",view->getscreenID(),getscreenName(view->getscreenID()));
                   view->End();
                   delete view;
               }
            }
        }

        if(NULL != pCurView)
        {
            pCurView->visible();
        }
        else
        {
            HMI_LOG("mode want show screen doesn't exist");
            for(int j=0; j<ViewTableNum; j++)
            {
                HMI_LOG("this->getscreenID() =%d,ViewTable[%d].screenid =%d",this->getscreenID(),j,ViewTable[j].screenid);
                if( this->getscreenID() == ViewTable[j].screenid )
                {
                    constructView(this->getscreenID(), MODE_ON_PERCENT_100);
                    break;
                }
                else
                {
                    HMI_LOG("this mode haven't a screen");
                }
            }
        }
    }
      ModemanagerClassIns->Ope_ShowlayerManager(this->getmodeID(),this->getscreenID());
}

/******************************************************************************
Function Name : invisible
Description   : Mode invisible
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractModeClass::invisible()
{
    QList<Frm_AbstractViewClass*> allView = this->findChildren<Frm_AbstractViewClass*>();

    int count = allView.count();

    for(int i=count-1; i>=0; i--)
    {
        Frm_AbstractViewClass* view = allView.at(i);
        if (view->getvisible())
        {
            view->invisible();
        }
        else
        {
            break;
        }
    }

    concreteInvisible();
}

/******************************************************************************
Function Name : start
Description   : mode start
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractModeClass::Start()
{
    concreteStart();
}

/******************************************************************************
Function Name : End
Description   : mode end
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractModeClass::End()
{
    QList<Frm_AbstractViewClass*> allView = this->findChildren<Frm_AbstractViewClass*>();
    int icount = allView.count();
    HMI_LOG("icount = %d",icount);
    for(int i=icount-1; i>=0; i--)
    {
        Frm_AbstractViewClass* pview = NULL;

        pview = allView.at(i);
        if(this == pview->getModeClass())//juanjuan add,only delete the screen belong to this mode
        {
            HMI_LOG("this view belong to this mode");
            if( NULL != pview )
            {
                pview->End();

                delete pview;
            }
        }
        else
        {
            HMI_LOG("this view not belong to this mode");
        }
    }

    concreteEnd();
}

/******************************************************************************
Function Name : getView
Description   : get view handle
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_AbstractViewClass* Frm_AbstractModeClass::getView(quint16 screenid)
{
    Frm_AbstractViewClass *View       = NULL;

    QList<Frm_AbstractViewClass*> allView = this->findChildren<Frm_AbstractViewClass*>();

    for(int i=0; i<allView.count();i++)
    {
        View = allView.at(i);

        if( screenid == View->getscreenID())
        {
            break;
        }
    }

    return View;

}

/******************************************************************************
Function Name : constructView
Description   : construct View
Date          : 2015/07/15
Parameter     :
para1         : quint16 screenid
para2         : quint16 modeSts
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractModeClass::constructView(quint16 screenid, quint16 modeSts)
{
    Frm_AbstractViewClass *childView       = NULL;
    Frm_AbstractViewClass *thisView        = NULL;
    Frm_AbstractViewClass *currentView     = NULL;
    screenID = screenid;
    HMI_LOG("constructView");
    if (0 == screenID)
    {
        screenID = getscreenID();
        if(0 == screenID)screenID = 1;//avoid crash
    }

    QList<Frm_AbstractViewClass*> allView = this->findChildren<Frm_AbstractViewClass*>();

    int i=0;
    int icount = allView.count();

    for(i=0; i<allView.count();i++)
    {
        Frm_AbstractViewClass* view = allView.at(i);

        if( NULL != thisView )
        {
            childView = view;
            break;
        }
        else if( screenid == view->getscreenID())
        {
            thisView = view;
        }
        else
        {
            currentView = view;
        }
    }

    if( NULL != thisView )
    {
        if( NULL != childView )
        {
            bool visible_flg = true;
            Frm_AbstractViewClass* pview = NULL;
            for(int j=icount-1; j>=i; j--)
            {
                pview = allView.at(j);

                if( NULL != pview )
                {
                    if (MODE_ON_PERCENT_100 == this->getmodeStatus() && visible_flg)
                    {
                        pview->invisible();
                        HMI_LOG("mode id =%d,view id =%d is invisible.",pview->getModeClass()->getmodeID(),pview->getscreenID());
                        if (0 == pview->getscreenAttr())
                        {
                            visible_flg = false;
                        }
                    }

                    pview->End();

                    delete pview;
                }
                else
                {
                    break;
                }
            }
        }
        HMI_LOG("modeSts = %d, viewSts = %d",this->getmodeStatus(),modeSts);
        if ((MODE_ON_PERCENT_100 == this->getmodeStatus()) ||
                (MODE_ON_PERCENT_100 == modeSts))
        {
            //Draw
            thisView->reqDraw();

            //display
            thisView->visible();
        }
    }
    else
    {
        HMI_LOG("modeSts = %d, viewSts = %d",this->getmodeStatus(),modeSts);
        for(int i=0; i<ViewTableNum; i++)
        {
            HMI_LOG("modeSts = %d, viewSts = %d, %d %d",this->getmodeStatus(),modeSts,screenid,ViewTable[i].screenid);
            if( screenid == ViewTable[i].screenid )
            {
                Frm_AbstractViewClass* view = NULL;
                if(currentView != NULL)
                {
                    if (MODE_ON_PERCENT_100 == this->getmodeStatus() && ViewTable[i].attr != 1)
                    {
                        currentView->invisible();

                        if (1 == currentView->getscreenAttr())
                        {
                            Frm_AbstractViewClass* pview = NULL;//dynamic_cast<Frm_AbstractViewClass*>(currentView->parent());
                            if (pview)
                            {
                                pview->invisible();
                            }
                        }
                    }

                    view = ((Frm_AbstractViewClass*(*)(QObject*,quint16 ,quint16))ViewTable[i].func)(currentView, screenid, ViewTable[i].attr);
                }
                else
                {
                    view = ((Frm_AbstractViewClass*(*)(QObject*,quint16,quint16 ))ViewTable[i].func)(this, screenid, ViewTable[i].attr);
                }

                view->Start();
                HMI_LOG("modeSts = %d, viewSts = %d",this->getmodeStatus(),modeSts);
                if ((MODE_ON_PERCENT_100 == this->getmodeStatus()) ||
                        (MODE_ON_PERCENT_100 == modeSts))
                {
                    view->visible();
                    HMI_LOG("mode id =%d,view id =%d is visible.",view->getModeClass()->getmodeID(),view->getscreenID());
                }

                break;
            }
        }
    }
}

/******************************************************************************
Function Name : returnView
Description   : return View
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractModeClass::returnView()
{
    HMI_LOG("IN");
    Frm_AbstractViewClass *thisView        = NULL;
    Frm_AbstractViewClass *currentView     = NULL;

    QList<Frm_AbstractViewClass*> allView = this->findChildren<Frm_AbstractViewClass*>();

    int icount = allView.count();
    HMI_LOG("view count = %d",icount);
    if (icount <= 1)
    {
        return ;
    }

    currentView = allView.at(icount - 1);
    thisView = allView.at(icount - 2);

    if( NULL != currentView && NULL != thisView)
    {
        screenID = thisView->getscreenID(); //wangtingting add for SX5VA-268

        if (MODE_ON_PERCENT_100 == this->getmodeStatus())
        {
            currentView->invisible();
            HMI_LOG("mode id =%d,view id =%d is invisible.",currentView->getModeClass()->getmodeID(),currentView->getscreenID());
        }

        currentView->End();

        delete currentView;

        if (MODE_ON_PERCENT_100 == this->getmodeStatus())
        {
            //Draw
            thisView->reqDraw();

            if (1 == thisView->getscreenAttr())
            {
                Frm_AbstractViewClass* pview = dynamic_cast<Frm_AbstractViewClass*>(thisView->parent());
                if (pview)
                {
                    pview->visible();
                }
            }

            //display
            thisView->visible();
            HMI_LOG("mode id =%d,view id =%d is visible.",thisView->getModeClass()->getmodeID(),thisView->getscreenID());
        }
    }
}

/******************************************************************************
Function Name : changeView
Description   : change View
Date          : 2015/07/15
Parameter     : quint16 screenid
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractModeClass::changeView(quint16 screenid)
{
    HMI_LOG("IN");
    Frm_AbstractViewClass *thisView        = NULL;
    Frm_AbstractViewClass *currentView     = NULL;

    QList<Frm_AbstractViewClass*> allView = this->findChildren<Frm_AbstractViewClass*>();

    int icount = allView.count();
    bool b_idavailable = false;
    if( 1 >= ViewTableNum )
    {
        return;
    }

    if (icount < 1)
    {
        return ;
    }

    for(int i=0; i<ViewTableNum; i++)
    {
        if( screenid == ViewTable[i].screenid )
        {
            screenID = screenid;
            b_idavailable = true;
        }
    }

    if ( false == b_idavailable )
    {
        return;
    }

    currentView = allView.at(icount - 1);
    if (icount > 1)
    {
        thisView = allView.at(icount - 2);
    }

    if( NULL != currentView )
    {
        if (1 == currentView->getscreenAttr())
        {
            if (thisView && (thisView->getscreenAttr() != 1))
            {
                if (MODE_ON_PERCENT_100 == this->getmodeStatus())
                {
                    thisView->invisible();
                }
            }
        }

        if (MODE_ON_PERCENT_100 == this->getmodeStatus())
        {
            currentView->invisible();
            HMI_LOG("mode id =%d,view id =%d is invisible.",currentView->getModeClass()->getmodeID(),currentView->getscreenID());
        }

        currentView->End();

        delete currentView;

        for(int i=0; i<ViewTableNum; i++)
        {
            if( screenid == ViewTable[i].screenid )
            {
                Frm_AbstractViewClass* view = NULL;
                if(thisView != NULL)
                {
                    view = ((Frm_AbstractViewClass*(*)(QObject*,quint16 ,quint16))ViewTable[i].func)(thisView, screenid, ViewTable[i].attr);
                }
                else
                {
                    HMI_LOG("base view is NULL");
                    view = ((Frm_AbstractViewClass*(*)(QObject*,quint16,quint16 ))ViewTable[i].func)(this, screenid, ViewTable[i].attr);
                }

                view->Start();

                if (MODE_ON_PERCENT_100 == this->getmodeStatus())
                {
                    view->visible();
                    HMI_LOG("mode id =%d,view id =%d is visible.",view->getModeClass()->getmodeID(),view->getscreenID());
                }

                break;
            }
        }
    }
    return;
}
/******************************************************************************
Function Name : getscreenName
Description   : get screen name
Date          : 2017/09/30
Parameter     :int screenid
Author        : juanjuan
******************************************************************************/
const char *Frm_AbstractModeClass::getscreenName(int screenid)
{
    const char *name = NULL;
    for(int i=0; i < ViewTableNum; i++)
    {
        if( screenid == ViewTable[i].screenid )
        {
             name =  ViewTable[i].screenname;
             HMI_LOG("name: %s ",name);
        }
    }
    if(NULL == name)
    {
        name = viewnullTable[0].screenname;
    }
    return name;
}
/******************************************************************************
Function Name : getscreenName
Description   : get screen name
Date          : 2017/09/30
Parameter     :int screenid
Author        : juanjuan
******************************************************************************/
Frm_AbstractModeClass* Frm_AbstractModeClass::get_parentmode()//juanjuan add to check if this mode have parent except the common mode
{
    Frm_AbstractModeClass* pModeClass = NULL;
    pModeClass = (Frm_AbstractModeClass*)this->parent();
    return pModeClass;
}
/******************************************************************************
Function Name : getscreenName
Description   : get screen name
Date          : 2017/09/30
Parameter     :int screenid
Author        : juanjuan
******************************************************************************/
Frm_AbstractModeClass* Frm_AbstractModeClass::get_childmode()//juanjuan add to check if this mode have child
{
    Frm_AbstractModeClass* pModeClass = NULL;
    pModeClass =(Frm_AbstractModeClass*)this->findChild<Frm_AbstractModeClass*>();
    return pModeClass;
}
/******************************************************************************
Function Name : getViewPara
Description   : get View para
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void* Frm_AbstractModeClass::getViewPara( quint16 screenid )
{
    for(int i=0; i < ViewTableNum; i++)
    {
        if( screenid == ViewTable[i].screenid )
        {
             return ViewTable[i].para;
        }
    }

    return NULL;
}

/******************************************************************************
Function Name : setViewPara
Description   : set View para
Date          : 2015/07/15
Parameter     :
para1         : quint16 screenid
para2         : void* para
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractModeClass::setViewPara( quint16 screenid, void* para )
{
    for(int i=0; i < ViewTableNum; i++)
    {
        if( screenid == ViewTable[i].screenid )
        {
            if (NULL != ViewTable[i].para)
            {
                delete ViewTable[i].para;
            }

            ViewTable[i].para = para;
        }
    }
}

/******************************************************************************
Function Name : ~Frm_AbstractModeClass
Description   : Destructor
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_AbstractModeClass::~Frm_AbstractModeClass()
{
    for(int i=0; i < ViewTableNum; i++)
    {
        if (NULL != ViewTable[i].para)
        {
            delete ViewTable[i].para;
        }
    }
}

/******************************************************************************
Function Name : showMessage
Description   : Show Message
Date          : 2015/07/15
Parameter     : message
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractModeClass::showMessage(QString message)
{
    int i_viewcurmode = Frm_CommonClassIns->get_current_view_mode();
    if( 0 == i_viewcurmode )
    {
        return;
    }
    Frm_CommonClassIns->dequeueMusicMessage_byUser();//16387	SX7-1895
    Frm_CommonClassIns->showMessage(message);
    return;
}

void Frm_AbstractModeClass::showmusicMessage(QString message)
{
    int i_viewcurmode = Frm_CommonClassIns->get_current_view_mode();
    if( 0 == i_viewcurmode )
    {
        return;
    }
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "show_musiconscreen",Q_ARG(QVariant, message));
    return;
}

/******************************************************************************
Function Name : showMessage_noTimer
Description   : Show Message
Date          : 2015/07/15
Parameter     : message
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractModeClass::showMessage_noTimer(QString message)
{
    int i_viewcurmode = Frm_CommonClassIns->get_current_view_mode();
    if( 0 == i_viewcurmode )
    {
        return;
    }
    Frm_CommonClassIns->showMessage(message);
    return;
}

void Frm_AbstractModeClass::showTalkons()
{
    int i_viewcurmode = Frm_CommonClassIns->get_current_view_mode();
    if( 0 == i_viewcurmode )
    {
        return;
    }
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "show_talkons");
    return;
}

void Frm_AbstractModeClass::showPhoneUpdatePopup()
{
    HMI_LOG("IN");
    int i_viewcurmode = Frm_CommonClassIns->get_current_view_mode();
    if( 0 == i_viewcurmode )
    {
        return;
    }
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "show_PhoneUpdatePopup");
    return;
}

void Frm_AbstractModeClass::showPDCkons()
{
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "show_PDCons");
    return;
}

void Frm_AbstractModeClass::destroyPDCkons()
{
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "destroy_PDCons");
    return;
}

void Frm_AbstractModeClass::shownavitbtons()
{
    HMI_LOG("IN");
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "show_navitbtons");
    return;
}

void Frm_AbstractModeClass::destroynavitbtons()
{
    HMI_LOG("IN");
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "destroy_navitbtons");
    return;
}

void Frm_AbstractModeClass::destroyTalkons()
{
    HMI_LOG("IN");
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "destroy_talkons");
    return;
}

void Frm_AbstractModeClass::destroyPhoneUpdatePopup()
{
    HMI_LOG("IN");
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "destroy_PhoneUpdatePopup");
    return;
}

void Frm_AbstractModeClass::show_dvr_ons(QString message)
{
    HMI_LOG("[DVR]IN");
    int i_viewcurmode = Frm_CommonClassIns->get_current_view_mode();
    if( 0 == i_viewcurmode )
    {
        return;
    }
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "showDVRons", Q_ARG(QVariant, message));
    return;
}

void Frm_AbstractModeClass::destroy_dvr_ons()
{
    HMI_LOG("[DVR]IN");
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "destroyDVRons");
    return;
}

void Frm_AbstractModeClass::showDiagons()
{
    HMI_LOG("IN");
    if(HMI_MODE_DIAGNOSE_STATUS_OFF == ModemanagerClassIns->get_diagnose_status())
    {
        return;
    }
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "show_diagons");
    return;
}

void Frm_AbstractModeClass::destroyDiagons()
{
    HMI_LOG("IN");
    if(HMI_MODE_DIAGNOSE_STATUS_ON == ModemanagerClassIns->get_diagnose_status())
    {
        if( 0 == ModemanagerClassIns->Get_avm_rvc_pdc() )
        {
            return;
        }
    }

    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "destroy_diagons");
    return;
}

void Frm_AbstractModeClass::touch_notpermit()
{
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "touch_notpermit");
    return;
}

void Frm_AbstractModeClass::touch_permit()
{
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "touch_permit");
    return;
}

void Frm_AbstractModeClass::showCarplayons()
{
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "show_Carplayons");
    return;
}

void Frm_AbstractModeClass::destroyCarplayons()
{
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "destroy_Carplayons");
    return;
}
/******************************************************************************
Function Name : destroyshowMessage
Description   : hide Message
Date          : 2015/07/15
Parameter     : message
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractModeClass::destroyshowMessage()
{
    Frm_CommonClassIns->dequeueMessage_byUser();
    return;
}

void Frm_AbstractModeClass::destroyshowMusicMessage()
{
    Frm_CommonClassIns->dequeueMusicMessage_byUser();
    return;
}

void Frm_AbstractModeClass::destroyshowMessageNoanimation()
{
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    //QMetaObject::invokeMethod(rootObject, "destroy_onscreenNoanimation");
    return;
}

/******************************************************************************
Function Name : showVolumeBar
Description   : Show Volume Bar
Date          : 2015/10/19
Parameter     : value
Author        : MaoTianliang(Neusoft)
******************************************************************************/
void Frm_AbstractModeClass::showVolumeBar(qint16 i_value)
{
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();
    HMI_LOG("i_value = %d", i_value);
    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

    if(0 > i_value)
    {
        i_value = 0;
    }

    if (HMI_VOLUME_MAX < i_value)
    {
        i_value = HMI_VOLUME_MAX;
    }
    if( 12 == Frm_CommonClassIns->get_current_view_mode() )
    {
        ModemanagerClassIns->Ope_ctrlvolbar(1);
    }

   // QMetaObject::invokeMethod(rootObject, "show_volumebar",
    //                          Q_ARG(QVariant, i_value));
    return;
}

/******************************************************************************
Function Name : hideVolumeBar
Description   : hide Volume Bar
Date          : 2015/10/19
Parameter     :
Author        : MaoTianliang(Neusoft)
******************************************************************************/
void Frm_AbstractModeClass::hideVolumeBar()
{
    HMI_LOG("ENTER");
    Frm_CommonClass* frameworkclass = getFrm_CommonClass();

    //QQuickItem* rootObject = frameworkclass->getMainQMLView()->rootObject();

   // QMetaObject::invokeMethod(rootObject, "hideVolumeBar");
    return;
}
