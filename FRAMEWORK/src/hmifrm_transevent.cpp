/*******************************************************
File Name   : Frm_TransEvent

Created by  :  Shi.Jianhang(Neusoft)

Created Date:  2015/07/15

Description   :   event management(page trans)
*********************************************************/

/****************Include  Section Begin***************************************************/
#include "../inc/hmifrm_transevent.h"
/****************Include  Section End***************************************************/

/******************************************************************************
Function Name : Frm_TransEvent
Description   : Constructor
Date          : 2015/07/15
Parameter     :
para1         : Type type
para2         : quint16 screenid
para3         : quint16 pattern
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_TransEvent::Frm_TransEvent(Type type, quint16 modeid,quint16 screenid,quint16 pattern ): Frm_AbstractEvent(type)
{
    m_modeID = modeid;//wangtingting add on 0509
    m_screenID = screenid;
    m_pattern = pattern;

}

/******************************************************************************
Function Name : Frm_TransEvent
Description   : Destructor
Date          : 2015/07/15
Parameter     :
para1         : Type type
para2         : quint16 screenid
para3         : quint16 pattern
Author        : Shi.Jianhang(Neusoft)
******************************************************************************/
Frm_TransEvent::~Frm_TransEvent()
{
}

