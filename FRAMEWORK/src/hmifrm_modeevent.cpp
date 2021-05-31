/*******************************************************
File Name   : Frm_ModeEvent

Created by  :  Shi.Jianhang(Neusoft)

Created Date:  2015/07/15

Description   :   event management(mode)
*********************************************************/

/****************Include  Section Begin***************************************************/
#include "../inc/hmifrm_modevent.h"
#include "hmi_log.h"
/****************Include  Section End***************************************************/


/******************************************************************************
Function Name : Frm_AbstractModeClass
Description   : constructor
Date          : 2015/07/15
Parameter     :
para1         : Type type
para2         : quint16 modeid
para3         : quint16 screenid
para4         : ModeReqType reqtype
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_ModeEvent::Frm_ModeEvent(Type type, quint16 modeid,quint16 screenid, ModeReqType reqtype): Frm_AbstractEvent(type)
{
    HMI_LOG("MODETRANS  modeid = %d,screenid = %d,reqtype = %d",modeid,screenid,reqtype);
    modeID = modeid;
    screenID = screenid;
    reqType = reqtype;
}

/******************************************************************************
Function Name : ~Frm_AbstractModeClass
Description   : constructor
Date          : 2015/07/15
Parameter     :
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_ModeEvent::~Frm_ModeEvent()
{
}

