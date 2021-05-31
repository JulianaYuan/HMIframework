/*******************************************************
 File Name   : hmifrm_abstractimportview.cpp

 Created by  :  Shi.Jianhang(Neusoft)

 Created Date:  2015/07/15

Description   :   Import QML view
*********************************************************/

/****************Include  Section Begin***************************************************/
#include "../inc/hmifrm_abstractimportview.h"
/****************Include  Section End***************************************************/

/******************************************************************************
Function Name : Frm_AbstractImportView
Description   : constructor
Date          : 2015/07/15
Parameter     : QObject* parent
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_AbstractImportView::Frm_AbstractImportView(QObject* parent): QObject(parent)
{
	view = NULL;
}


/******************************************************************************
Function Name : setAccessClass
Description   : set Access Class
Date          : 2015/07/15
Parameter     : Frm_AbstractViewClass* viewP
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractImportView::setAccessClass(Frm_AbstractViewClass* viewP)
{
    view = viewP;

    setConcreteAccessClass();
    return;
}

/******************************************************************************
Function Name : reqTrans
Description   : view trans
Date          : 2015/07/15
Parameter     : quint16 screenid
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractImportView::reqTrans(quint16 screenid)
{
	if (NULL != view)
	{
    	view->reqTrans(screenid);
	}
    return;
}

/******************************************************************************
Function Name : reqReturn
Description   : view return
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractImportView::reqReturn()
{
	if (NULL != view)
	{
    	view->reqReturn();
	}
    return;
}

/******************************************************************************
Function Name : reqChange
Description   : view return
Date          : 2015/07/15
Parameter     : quint16 screenid
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractImportView::reqChange(quint16 screenid)
{
	if (NULL != view)
	{
    	view->reqChange(screenid);
	}
    return;
}

/******************************************************************************
Function Name : touchButton
Description   : touch button
Date          : 2015/07/15
Parameter     : quint16 screenid
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
void Frm_AbstractImportView::touchButton(quint16 buttonid)
{
	if (NULL != view)
	{
    	view->touchButton(buttonid);
	}
    return;
}

/******************************************************************************
Function Name : modeID
Description   : get modeID
Date          : 2015/07/15
Parameter     : quint16 screenid
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
int Frm_AbstractImportView::modeID()
{
    if( NULL != view )
    {
        Frm_AbstractModeClass* modeClass = view->getModeClass();

        if( NULL != modeClass )
        {
            return (int)modeClass->getmodeID();
        }
    }

    return 0;
}

/******************************************************************************
Function Name : ~Frm_AbstractImportView
Description   : Destructor
Date          : 2015/07/15
Parameter     : quint16 screenid
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_AbstractImportView::~Frm_AbstractImportView()
{
    if( NULL != view )
    {
        view->noticeCleareImportClass();
    }
}
