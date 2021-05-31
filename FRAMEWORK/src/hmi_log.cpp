#include <stdio.h>
//#include <pthread.h>
#include <stdarg.h>
#include "hmi_log.h"

#define STR_BUFFER_LEN 1000

//DLT_DECLARE_CONTEXT(con_hmi)

static char ver_str[STR_BUFFER_LEN] = {0};static char str[STR_BUFFER_LEN];

//static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void hmi_dlt_log_init()
{    
    //DLT_REGISTER_APP("HMI","SX5 HMI Service");

    //DLT_REGISTER_CONTEXT(con_hmi,"CON","Singleton");

    snprintf(ver_str, STR_BUFFER_LEN, "[SX5 HMI Test MID][v%s]\n", HMI_VER);

    //dlt_log_string(&con_hmi, DLT_LOG_INFO, ver_str);
}

void hmi_dlt_log(int/*DltLogLevelType*/ level, const char *format, ...)
{    
	va_list var_args;
	va_start (var_args, format);    
	
    //pthread_mutex_lock(&mutex);
	
	vsnprintf(str, STR_BUFFER_LEN, format, var_args);    
	
    //dlt_log_string(&con_hmi, level, str);
	
    //pthread_mutex_unlock(&mutex);

	va_end (var_args);
}


