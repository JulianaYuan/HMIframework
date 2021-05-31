#include "../inc/hmifrm_keyclass.h"
#include "hmi_log.h"
#define     KEY_TIMER_FOR_LONG_PRESS            (100)   //100ms
#define     KEY_TIMER_FOR_ROTARY_DELAY            (200)   //100ms

Frm_KeyClass* Frm_KeyClass::m_instance = NULL;

static KEY_TABLE_TYPE key_table[] =
{
    //keycode1                  //keycode2          //keycode3          //keycode4   //b_sendrelease_event  //status                //timeout1  //timeout2  //timeout3       //event1                       //event2                            //event3              //press event                 //release event            // release when long press
    {KEY_CODE_POWER,            KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE, true, KEY_STATUS_OFFTIME,     1500,          0,          0,            KEYEVENT_POWER_LONGPRESS,      KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_POWER_PRESS,       KEYEVENT_POWER_RELEASE,    KEYEVENT_POWER_LONGRELEASE},
    {KEY_CODE_EQ,               KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE, true, KEY_STATUS_PRESS,       0,          0,          0,               KEYEVENT_NONE,                 KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_EQ_PRESS,          KEYEVENT_EQ_RELEASE,       KEYEVENT_NONE},
    {KEY_CODE_BAND,             KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE, true, KEY_STATUS_PRESS,       0,          0,          0,               KEYEVENT_NONE,                 KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_BAND_PRESS,        KEYEVENT_BAND_RELEASE,     KEYEVENT_NONE},
    {KEY_CODE_MUTE,             KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE, true, KEY_STATUS_PRESS,       0,          0,          0,               KEYEVENT_NONE,                 KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_MUTE_PRESS,        KEYEVENT_MUTE_RELEASE,     KEYEVENT_NONE},
    {KEY_CODE_USB,              KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE, true, KEY_STATUS_PRESS,       0,          0,          0,               KEYEVENT_NONE,                 KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_USB_PRESS,         KEYEVENT_USB_RELEASE,      KEYEVENT_NONE},
    {KEY_CODE_NAVI,             KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE, true, KEY_STATUS_PRESS,       0,          0,          0,               KEYEVENT_NONE,                 KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_NAVI_PRESS,        KEYEVENT_NAVI_RELEASE,     KEYEVENT_NONE},
    {KEY_CODE_MENU,             KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE, true, KEY_STATUS_PRESS,       0,          0,          0,               KEYEVENT_NONE,                 KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_HOME_PRESS,        KEYEVENT_HOME_RELEASE,     KEYEVENT_NONE},
    {KEY_CODE_BACK,             KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE, true, KEY_STATUS_PRESS,       0,          0,          0,               KEYEVENT_NONE,                 KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_BACK_PRESS,        KEYEVENT_BACK_RELEASE,     KEYEVENT_NONE},
    {KEY_CODE_ENCODER_LEFT,     KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE, true, KEY_STATUS_ROTARY,      0,          0,          0,               KEYEVENT_NONE,                 KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_NONE,              KEYEVENT_ENCODER_LEFT,     KEYEVENT_NONE},
    {KEY_CODE_ENCODER_RIGHT,    KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE, true, KEY_STATUS_ROTARY,      0,          0,          0,               KEYEVENT_NONE,                 KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_NONE,              KEYEVENT_ENCODER_RIGHT,    KEYEVENT_NONE},
    {KEY_CODE_ENCODER_ENTER,    KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE, true, KEY_STATUS_PRESS,       0,          0,          0,               KEYEVENT_NONE,                 KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_ENCODER_PRESS,     KEYEVENT_ENCODER_RELEASE,  KEYEVENT_NONE},
    {KEY_CODE_PHONE,            KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,     true, KEY_STATUS_OFFTIME,     2000,       0,          0,               KEYEVENT_PHONE_LONGPRESS,      KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_PHONE_PRESS,       KEYEVENT_PHONE_RELEASE,    KEYEVENT_PHONE_LONGRELEASE},
    {KEY_CODE_SETUP,            KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,     true, KEY_STATUS_PRESS,       0,          0,          0,               KEYEVENT_NONE,                 KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_SETUP_PRESS,       KEYEVENT_SETUP_RELEASE,    KEYEVENT_NONE},
    {KEY_CODE_VOL_UP,           KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,     true, KEY_STATUS_OFFTIME,     500,       0,          0,                KEYEVENT_VOL_UP_LONGPRESS,     KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_VOL_UP_PRESS,      KEYEVENT_VOL_UP_RELEASE,   KEYEVENT_VOL_UP_LONGRELEASE},
    {KEY_CODE_VOL_DN,           KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,     true, KEY_STATUS_OFFTIME,     500,       0,          0,                KEYEVENT_VOL_DN_LONGPRESS,     KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_VOL_DN_PRESS,      KEYEVENT_VOL_DN_RELEASE,   KEYEVENT_VOL_DN_LONGRELEASE},
    {KEY_CODE_SEEK_UP,          KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,     true, KEY_STATUS_OFFTIME,     1500,       2000,       0,               KEYEVENT_SEEK_UP_LONGPRESS,    KEYEVENT_SEEK_UP_LONGPRESS_2S,      KEYEVENT_NONE,        KEYEVENT_SEEK_UP_PRESS,     KEYEVENT_SEEK_UP_RELEASE,  KEYEVENT_SEEK_UP_LONGRELEASE},
    {KEY_CODE_SEEK_DN,          KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,     true, KEY_STATUS_OFFTIME,     1500,       2000,       0,               KEYEVENT_SEEK_DN_LONGPRESS,    KEYEVENT_SEEK_DN_LONGPRESS_2S,      KEYEVENT_NONE,        KEYEVENT_SEEK_DN_PRESS,     KEYEVENT_SEEK_DN_RELEASE,  KEYEVENT_SEEK_DN_LONGRELEASE},
    {KEY_CODE_LINK,             KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,     true, KEY_STATUS_PRESS,       0,          0,          0,               KEYEVENT_NONE,                 KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_LINK_PRESS,        KEYEVENT_LINK_RELEASE,     KEYEVENT_NONE},
    {KEY_CODE_DISP,             KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,     true, KEY_STATUS_PRESS,       0,          0,          0,               KEYEVENT_NONE,                 KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_DISP_PRESS,        KEYEVENT_DISP_RELEASE,     KEYEVENT_NONE},
    // Steering wheel keys
    {KEY_CODE_STW_MUTE,         KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,     true, KEY_STATUS_PRESS,       0,          0,          0,               KEYEVENT_NONE,                 KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_STW_MUTE_PRESS,    KEYEVENT_STW_MUTE_RELEASE, KEYEVENT_NONE},
    {KEY_CODE_STW_MODE,         KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,     true, KEY_STATUS_PRESS,       0,          0,          0,               KEYEVENT_NONE,                 KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_STW_MODE_PRESS,    KEYEVENT_STW_MODE_RELEASE, KEYEVENT_NONE},
    {KEY_CODE_STW_VOL_UP,       KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,     true, KEY_STATUS_OFFTIME,     1500,       0,          0,               KEYEVENT_VOL_UP_LONGPRESS,     KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_VOL_UP_PRESS,      KEYEVENT_VOL_UP_RELEASE,   KEYEVENT_VOL_UP_LONGRELEASE},
    {KEY_CODE_STW_VOL_DN,       KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,     true, KEY_STATUS_OFFTIME,     1500,       0,          0,               KEYEVENT_VOL_DN_LONGPRESS,     KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_VOL_DN_PRESS,      KEYEVENT_VOL_DN_RELEASE,   KEYEVENT_VOL_DN_LONGRELEASE},
    //zwb change for SX5M-1204 and SX5M-1205 start
	//15471	SX7-821
    // {KEY_CODE_STW_SEEK_UP,      KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_STATUS_OFFTIME,     1500,       2000,          0,            KEYEVENT_STW_SEEK_UP_LONGPRESS,    KEYEVENT_SEEK_UP_LONGPRESS_2S,  KEYEVENT_NONE,        KEYEVENT_STW_SEEK_UP_PRESS,     KEYEVENT_STW_SEEK_UP_RELEASE,  KEYEVENT_STW_SEEK_UP_LONGRELEASE},
    // {KEY_CODE_STW_SEEK_DN,      KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_STATUS_OFFTIME,     1500,       2000,          0,            KEYEVENT_STW_SEEK_DN_LONGPRESS,    KEYEVENT_SEEK_DN_LONGPRESS_2S,  KEYEVENT_NONE,        KEYEVENT_STW_SEEK_DN_PRESS,     KEYEVENT_STW_SEEK_DN_RELEASE,  KEYEVENT_STW_SEEK_DN_LONGRELEASE},
    {KEY_CODE_STW_SEEK_UP,      KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,     true, KEY_STATUS_OFFTIME,     1500,       0,          0,               KEYEVENT_STW_SEEK_UP_LONGPRESS,    KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_STW_SEEK_UP_PRESS,     KEYEVENT_STW_SEEK_UP_RELEASE,  KEYEVENT_STW_SEEK_UP_LONGRELEASE},
    {KEY_CODE_STW_SEEK_DN,      KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE, true,     KEY_STATUS_OFFTIME,     1500,       0,          0,               KEYEVENT_STW_SEEK_DN_LONGPRESS,    KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_STW_SEEK_DN_PRESS,     KEYEVENT_STW_SEEK_DN_RELEASE,  KEYEVENT_STW_SEEK_DN_LONGRELEASE},
    //zwb change for SX5M-1204 and SX5M-1205 end
    {KEY_CODE_STW_PHONE,        KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,  true,    KEY_STATUS_OFFTIME,     2000,       0,          0,               KEYEVENT_PHONE_LONGPRESS,      KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_PHONE_PRESS,       KEYEVENT_PHONE_RELEASE,    KEYEVENT_PHONE_LONGRELEASE},
    {KEY_CODE_STW_ASR,          KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE,  true,    KEY_STATUS_OFFTIME,     500,       0,          0,               KEYEVENT_ASR_LONGPRESS,        KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_ASR_PRESS,         KEYEVENT_ASR_RELEASE,        KEYEVENT_ASR_LONGRELEASE},
    // MultiKey
    {KEY_CODE_MENU,             KEY_CODE_MUTE,      KEY_CODE_NONE,      KEY_CODE_NONE, true,     KEY_STATUS_OFFTIME,     5000,       0,          0,               KEYEVENT_ETM_ENTRY,            KEYEVENT_NONE,                      KEYEVENT_NONE,       KEYEVENT_NONE,    KEYEVENT_NONE,             KEYEVENT_NONE},
    {KEY_CODE_NAVI,             KEY_CODE_MENU,        KEY_CODE_EQ,      KEY_CODE_NONE,  true,    KEY_STATUS_OFFTIME,     500,       0,          0,               KEYEVENT_FACTORY_MODE_ENTRY,   KEYEVENT_NONE,                      KEYEVENT_NONE,       KEYEVENT_NONE,    KEYEVENT_NONE,             KEYEVENT_NONE},
    {KEY_CODE_NAVI,             KEY_CODE_MENU,        KEY_CODE_USB,     KEY_CODE_NONE, true,     KEY_STATUS_PRESS,       0,          0,          0,               KEYEVENT_CARPLAY_TEST,   KEYEVENT_NONE,                      KEYEVENT_NONE,       KEYEVENT_NONE,    KEYEVENT_NONE,             KEYEVENT_NONE},

    {KEY_CODE_NONE,             KEY_CODE_NONE,      KEY_CODE_NONE,      KEY_CODE_NONE, true,     KEY_STATUS_INVALID,     0,          0,          0,               KEYEVENT_NONE,                 KEYEVENT_NONE,                      KEYEVENT_NONE,        KEYEVENT_NONE,   KEYEVENT_NONE,             KEYEVENT_NONE}
};

Frm_KeyClass::Frm_KeyClass():QObject()
{
    key_slot[0] = KEY_CODE_NONE;
    key_slot[1] = KEY_CODE_NONE;
    key_slot[2] = KEY_CODE_NONE;
    key_slot[3] = KEY_CODE_NONE;
    key_time_cnt = 0;
    keytbl_index = (-1);
    key_count = 0;
    ui_timer_id = 0;
    ui_rotary_timer_id = 0;
}

void Frm_KeyClass::init(QObject *parent)
{
    frameWorkClass = dynamic_cast<Frm_CommonClass*>(parent);

    keytbl_count = sizeof(key_table)/sizeof(KEY_TABLE_TYPE);

    connect(this, SIGNAL(sigPostKeyStatus(KEY_CODE, KEY_STATUS)), this, SLOT(onPostKeyStatus(KEY_CODE, KEY_STATUS)));
}

Frm_KeyClass* Frm_KeyClass::getInstance()
{
    if( !m_instance )
    {
        m_instance = new Frm_KeyClass();
    }

    return m_instance;
}

void Frm_KeyClass::DestroyObject()
{
    if( m_instance )
    {
        delete m_instance;
        m_instance = NULL;
    }
}
void Frm_KeyClass::inithardkey()
{
    key_count = 0;
    memset(key_slot,KEY_CODE_NONE,4);
    keytbl_index = (-1);
    if( 0 != ui_timer_id )
    {
        killTimer(ui_timer_id);
        ui_timer_id = 0;
    }
}

void Frm_KeyClass::ResetHardkeyTbl(KEY_CODE key_code, KEY_STATUS key_status)
{
    if( KEY_STATUS_RELEASE == key_status )
    {
        if(( 1 == key_count )&&(key_code == key_slot[0]))
        {
            if( KEY_STATUS_OFFTIME == key_table[keytbl_index].e_status )
            {
                if( 0 != ui_timer_id )
                {
                    killTimer(ui_timer_id);
                    ui_timer_id = 0;
                }

                if( (key_table[keytbl_index].i_time1 != 0 )
                        && (key_table[keytbl_index].i_time1 <= key_time_cnt ))
                {
                    //e_LongRelease_event
                    if (KEYEVENT_NONE != key_table[keytbl_index].e_LongRelease_event)
                    {
                        if(key_table[keytbl_index].b_send_longrelease) //wangtingting add on 0402
                        {
                            sendKeyEventToFw(key_table[keytbl_index].e_LongRelease_event);
                        }
                        else
                        {
							key_table[keytbl_index].b_send_longrelease = true;
                         	HMI_LOG("set b_send_longrelease true");
						}
                    }
                    else
                    {
                        //do nothing
                    }
                }
                else
                {
                    //default event
                }
            }
            key_count = 0;
            key_slot[0] = KEY_CODE_NONE;
            keytbl_index = (-1);
        }
        else
        {
            for( int i = 0; i < key_count; i++ )
            {
                if( key_code == key_slot[i] )
                {
                    key_slot[i] = KEY_CODE_NONE;
                    key_count--;
                }
            }
            for( int j = 0; j < 3; j++ )
            {
                if( ( KEY_CODE_NONE == key_slot[j] ) && ( KEY_CODE_NONE != key_slot[j+1]) )
                {
                    key_slot[j] = key_slot[j+1];
                    key_slot[j+1] = KEY_CODE_NONE;
                }
            }

            if( 0 != ui_timer_id )
            {
                killTimer(ui_timer_id);
                ui_timer_id = 0;
            }
        }
    }
    return;
}

void Frm_KeyClass::PostKeyStatus(KEY_CODE key_code, KEY_STATUS key_status)
{
    HMI_LOG("key_code=%d,key_status=%d,key_count=%d",key_code,key_status,key_count);
    if( ( key_code <= KEY_CODE_NONE ) ||( KEY_CODE_MAX <= key_code ) )
    {
        //invalid key code
        return;
    }

    if( ( KEY_STATUS_RELEASE != key_status ) && ( KEY_STATUS_PRESS != key_status ) && ( KEY_STATUS_ROTARY != key_status ) )
    {
        //invalid key status
        return;
    }

    if( ( 0 == key_count ) && ( KEY_STATUS_RELEASE == key_status) )
    {
        //no need to handle
        return;
    }

    emit sigPostKeyStatus(key_code, key_status);

}

void Frm_KeyClass::onPostKeyStatus(KEY_CODE key_code, KEY_STATUS key_status)
{
    int index = (-1);

    if( KEY_STATUS_RELEASE == key_status )
    {
        if( 1 == key_count )
        {
            if( key_code == key_slot[0] )    //one key release
            {
                if( KEY_STATUS_OFFTIME == key_table[keytbl_index].e_status )
                {
                    //stop timer
                    if( 0 != ui_timer_id )
                    {
                        killTimer(ui_timer_id);
                        ui_timer_id = 0;
                    }
                    if( (key_table[keytbl_index].i_time1 != 0 )
                            && (key_table[keytbl_index].i_time1 <= key_time_cnt ))
                    {
                        //e_LongRelease_event
                        if (KEYEVENT_NONE != key_table[keytbl_index].e_LongRelease_event)
                        {
                            if(key_table[keytbl_index].b_send_longrelease) //wangtingting add on 0402
                            {
                                sendKeyEventToFw(key_table[keytbl_index].e_LongRelease_event);
                            }
                            else
                            {
                                key_table[keytbl_index].b_send_longrelease = true;
                                HMI_LOG("set b_send_longrelease true");
                            }

                        }
                        else
                        {
                            //do nothing
                        }
                    }
                    else
                    {
                        //default event
                        sendKeyEventToFw(key_table[keytbl_index].e_Release_event);
                    }

                    key_count = 0;
                    key_slot[0] = KEY_CODE_NONE;
                    keytbl_index = (-1);
                }
                else if( KEY_STATUS_PRESS == key_table[keytbl_index].e_status )
                {
                    sendKeyEventToFw(key_table[keytbl_index].e_Release_event);
                }
                key_count = 0;
                key_slot[0] = KEY_CODE_NONE;
                keytbl_index = (-1);
            }
            else
            {
                //invalid
                key_count = 0;
                memset(key_slot,KEY_CODE_NONE,4);
                keytbl_index = (-1);
            }
        }
        else
        {
            //clear slot
            for( int i = 0; i < key_count; i++ )
            {
                if( key_code == key_slot[i] )
                {
                    key_slot[i] = KEY_CODE_NONE;
                    key_count--;
                }
            }
            for( int j = 0; j < 3; j++ )
            {
                if( ( KEY_CODE_NONE == key_slot[j] ) && ( KEY_CODE_NONE != key_slot[j+1]) )
                {
                    key_slot[j] = key_slot[j+1];
                    key_slot[j+1] = KEY_CODE_NONE;
                }
            }

            if( 0 != ui_timer_id )
            {
                killTimer(ui_timer_id);
                ui_timer_id = 0;
            }
        }
    }
    else if( KEY_STATUS_PRESS == key_status )
    {
        if( key_count <= 0 )
        {
            //find long press table
            index = findKeyLongPressTable(key_code);
            if( 0 <= index )
            {
                //long press timer start
                if( 0 != ui_timer_id )
                {
                    killTimer(ui_timer_id);
                }
                key_time_cnt = 0;
                ui_timer_id = startTimer(KEY_TIMER_FOR_LONG_PRESS);
                sendKeyEventToFw(key_table[index].e_Press_event);
                //reset send long release
                if(false == key_table[index].b_send_longrelease)
                {
                    key_table[index].b_send_longrelease = true;
                }
            }
            else
            {
                index = findKeyPressTable(key_code);
                if( 0 <= index )
                {
                    sendKeyEventToFw(key_table[index].e_Press_event);
                }
                else
                {
                    //invalid key code
                    return;
                }
            }
            key_count = 1;
            key_slot[0] = key_code;
            keytbl_index = index;
        }
        else if( key_count <= 3 )
        {
            if( 0 != ui_timer_id )
            {
                killTimer(ui_timer_id);
                ui_timer_id = 0;
            }

            index = findKeyMulPressTable(key_code);
            if( 0 <= index )
            {
                //wait multi key
                key_slot[key_count++] = key_code;
                if( true == checkMultiKey(index) )
                {
                    if( KEY_STATUS_OFFTIME == key_table[index].e_status )
                    {
                        key_time_cnt = 0;
                        ui_timer_id = startTimer(KEY_TIMER_FOR_LONG_PRESS);
                        keytbl_index = index;
                    }
                    else
                    {
                        //send multi key event and clear slot
                        sendKeyEventToFw(key_table[index].e_out_event1);
                        keytbl_index = (-1);
                    }
                }
                else
                {
                    //else of checkMultiKey, do nothing
                }
            }
            else
            {
                //( 0 > index ), ignore the key
            }
        }
        else
        {
            //nothing to do
        }
    }
    if(KEY_STATUS_ROTARY == key_status)
    {
        index = findKeyRotaryTable(key_code);
        if( 0 <= index )
        {
            CursorstatusEnum en_status = EN_CURSORCTRL_STATUS_DISAPPEAR;
            Frm_CommonClassIns->getCursorStatus(&en_status);

            if (EN_CURSORCTRL_STATUS_SHOW == en_status)
            {
                sendKeyEventToFw(key_table[index].e_Release_event);
                if( 0 != ui_rotary_timer_id )
                {
                    killTimer(ui_rotary_timer_id);
                    ui_rotary_timer_id = 0;
                }
                keytbl_index = (-1);
            }
            else
            {
                if( 0 != ui_timer_id )
                {
                    killTimer(ui_timer_id);
                    ui_timer_id =0;
                }

                if( 0 != ui_rotary_timer_id )
                {
                    killTimer(ui_rotary_timer_id);
                }
                ui_rotary_timer_id = startTimer(KEY_TIMER_FOR_ROTARY_DELAY);
                keytbl_index = index;
            }

        }
        else
        {
            //invalid key code
            return;
        }
    }
    else
    {
        //key_count> 3 ,nothing to do
    }
}

void Frm_KeyClass::timerEvent(QTimerEvent *evt)
{
    if( ui_timer_id == evt->timerId() )
    {
        key_time_cnt += KEY_TIMER_FOR_LONG_PRESS;

        if( key_table[keytbl_index].i_time3 != 0 )
        {
            if( key_table[keytbl_index].i_time3 == key_time_cnt )
            {
                sendKeyEventToFw(key_table[keytbl_index].e_out_event3);
                killTimer(ui_timer_id);
                ui_timer_id = 0;
            }
        }

        if( key_table[keytbl_index].i_time2 != 0 )
        {
            if( key_table[keytbl_index].i_time2 == key_time_cnt )
            {
                sendKeyEventToFw(key_table[keytbl_index].e_out_event2);
                if ( 0 == key_table[keytbl_index].i_time3)
                {
                    killTimer(ui_timer_id);
                    ui_timer_id = 0;
                }
            }
        }

        if( key_table[keytbl_index].i_time1 != 0 )
        {
            if( key_table[keytbl_index].i_time1 == key_time_cnt )
            {
                sendKeyEventToFw(key_table[keytbl_index].e_out_event1);
                if ( 0 == key_table[keytbl_index].i_time2)
                {
                    killTimer(ui_timer_id);
                    ui_timer_id = 0;
                }
            }
        }
    }
    else if (evt->timerId() == ui_rotary_timer_id)
    {
        sendKeyEventToFw(key_table[keytbl_index].e_Release_event);
        if( 0 != ui_rotary_timer_id )
        {
            killTimer(ui_rotary_timer_id);
            ui_rotary_timer_id = 0;
        }
        keytbl_index = (-1);
    }
    else
    {
        //do nothing
    }
}

int Frm_KeyClass::findKeyLongPressTable(KEY_CODE key_code)
{
    int i_ret = (-1);

    for( int i = 0; i < keytbl_count; i++ )
    {
        if( ( key_code == key_table[i].e_key1 ) && ( key_table[i].e_status == KEY_STATUS_OFFTIME )
                &&( KEY_CODE_NONE == key_table[i].e_key2 ))
        {
            i_ret = i;
            break;
        }
    }
    return i_ret;
}

int Frm_KeyClass::findKeyMulPressTable(KEY_CODE key_code)
{
    int i_ret = (-1);

    int i_index[10] = {-1};
    int i_count = 0;

    int i = 0;

    if( 4 <= key_count )
    {
        return i_ret;
    }

    for( i = 0; i < keytbl_count; i++ )
    {
        if( ( key_code == key_table[i].e_key1 ) || ( key_code == key_table[i].e_key2 ) ||
                ( key_code == key_table[i].e_key3 ) || ( key_code == key_table[i].e_key4 ) )
        {
            i_index[i_count++] = i;
        }
    }

    for( i = 0; i < i_count; i++ )
    {
        if( KEY_CODE_NONE != key_slot[2] )
        {
            if( ( ( key_slot[2] == key_table[i_index[i]].e_key1 ) || ( key_slot[2] == key_table[i_index[i]].e_key2 ) ||
                  ( key_slot[2] == key_table[i_index[i]].e_key3 ) || ( key_slot[2] == key_table[i_index[i]].e_key4 ) ) &&
                    ( ( key_slot[1] == key_table[i_index[i]].e_key1 ) || ( key_slot[1] == key_table[i_index[i]].e_key2 ) ||
                      ( key_slot[1] == key_table[i_index[i]].e_key3 ) || ( key_slot[1] == key_table[i_index[i]].e_key4 ) ) &&
                    ( ( key_slot[0] == key_table[i_index[i]].e_key1 ) || ( key_slot[0] == key_table[i_index[i]].e_key2 ) ||
                      ( key_slot[0] == key_table[i_index[i]].e_key3 ) || ( key_slot[0] == key_table[i_index[i]].e_key4 ) ) )
            {
                i_ret = i_index[i];
                break;
            }
        }
        else if( KEY_CODE_NONE != key_slot[1] )
        {
            if( ( ( key_slot[1] == key_table[i_index[i]].e_key1 ) || ( key_slot[1] == key_table[i_index[i]].e_key2 ) ||
                  ( key_slot[1] == key_table[i_index[i]].e_key3 ) || ( key_slot[1] == key_table[i_index[i]].e_key4 ) ) &&
                    ( ( key_slot[0] == key_table[i_index[i]].e_key1 ) || ( key_slot[0] == key_table[i_index[i]].e_key2 ) ||
                      ( key_slot[0] == key_table[i_index[i]].e_key3 ) || ( key_slot[0] == key_table[i_index[i]].e_key4 ) ) )
            {
                i_ret = i_index[i];
                break;
            }
        }
        else
        {
            if( ( ( key_slot[0] == key_table[i_index[i]].e_key1 ) || ( key_slot[0] == key_table[i_index[i]].e_key2 ) ||
                  ( key_slot[0] == key_table[i_index[i]].e_key3 ) || ( key_slot[0] == key_table[i_index[i]].e_key4 ) ) )
            {
                i_ret = i_index[i];
                break;
            }
        }
    }
    return i_ret;
}

int Frm_KeyClass::findKeyPressTable(KEY_CODE key_code)
{
    int i_ret = (-1);

    for( int i = 0; i < keytbl_count; i++ )
    {
        if( ( key_code == key_table[i].e_key1 ) && ( key_table[i].e_status == KEY_STATUS_PRESS ) &&
                ( KEY_CODE_NONE == key_table[i].e_key2 ) )
        {
            i_ret = i;
            break;
        }
    }
    return i_ret;
}

bool Frm_KeyClass::checkMultiKey(int index)
{
    bool b_ret = true;

    int i_count = 4;

    if( KEY_CODE_NONE == key_table[index].e_key4 )
    {
        i_count--;
    }

    if( KEY_CODE_NONE == key_table[index].e_key3 )
    {
        i_count--;
    }

    if( KEY_CODE_NONE == key_table[index].e_key2 )
    {
        i_count--;
    }

    if( KEY_CODE_NONE == key_table[index].e_key1 )
    {
        i_count--;
    }

    if( i_count != key_count )
    {
        b_ret = false;
    }
    else
    {
        for( int i = 0; i < key_count; i++ )
        {
            if( ( key_slot[i] != key_table[index].e_key1 ) && ( key_slot[i] != key_table[index].e_key2 ) &&
                    ( key_slot[i] != key_table[index].e_key3 ) && ( key_slot[i] != key_table[index].e_key4 ) )
            {
                b_ret = false;
                break;
            }
        }
    }

    return b_ret;
}

void Frm_KeyClass::sendKeyEventToFw(KeyEvent event)
{
    Frm_KeyEvent* e = new Frm_KeyEvent(static_cast<QEvent::Type>(Frm_AbstractEvent::EN_EV_KEYEVENT), event);

    QGuiApplication::sendEvent(frameWorkClass, e);
}

int Frm_KeyClass::findKeyRotaryTable(KEY_CODE key_code)
{
    int i_ret = (-1);

    for( int i = 0; i < keytbl_count; i++ )
    {
        if( ( key_code == key_table[i].e_key1 ) && ( key_table[i].e_status == KEY_STATUS_ROTARY ) &&
                ( KEY_CODE_NONE == key_table[i].e_key2 ) )
        {
            i_ret = i;
            break;
        }
    }
    return i_ret;
}

void Frm_KeyClass::setkeycount(int count) //14277 by xubing
{
    key_count = count;
}
//wangtingting add on 0402
void Frm_KeyClass::ignoreLongRelease(KEY_CODE key_code)
{
    HMI_LOG("IN key_code =%d,keytbl_index =%d",key_code,keytbl_index);
    for(int i = 0; i < keytbl_count; i++)
    {
        if ((key_table[i].e_key1 == key_code) && (key_table[keytbl_index].e_key1 == key_code))
        {
            key_table[i].b_send_longrelease = false;
            HMI_LOG("key_code = %d",key_code);
        }
    }
}
