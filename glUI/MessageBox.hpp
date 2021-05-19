#pragma once

#ifdef _WIN
    #include<windows.h>
#endif

#include <stdlib.h>

#include "logger.hpp"

inline void AbortMessage(std::string message){
    LOGE << message;
#ifdef _WIN
    MessageBox(NULL , TEXT(message.c_str()), TEXT("AbortMessage"), 
         MB_OK|MB_ICONWARNING );
#endif

    exit(1);
}


inline void showMessage(std::string message){
    LOGI << message;
#ifdef _WIN
    MessageBox(NULL ,  TEXT(message.c_str()), TEXT("AbortMessage") , MB_OK );
#endif
}
