/**
 * @file logger.hpp
 * @brief 簡易ロガー
 * @author 20Niship Nishimiya Tadashi
 * @date 2021-01-17
 * @details 主に、LOGI, LOGD, LOGW, LOGEマクロを使ってログを出力する。MYLOGGER_OUTPUT_FILENAMEが定義されている時はMYLOGGER_OUTPUT_FILENAME(ファイル名)にログテキストを出力し、定義されていない時はコンソールに出力される。
 * @note
 * - LOGI << "string" : 標準色でテキスト出力
 * - LOGD << "string" : 緑色でテキスト出力
 * - LOGW << "string" : 黄色でテキスト出力
 * - LOGE << "string" : 赤色でテキスト出力
 */

#pragma once


#include <iostream>
#include <cstdio>
#include <ctime>
#include <time.h>
#include <string>
#include <sstream>
#include <fstream>
#include <assert.h>


#if defined( __Clang__ ) || defined( __GNUC__ )
#    define MYLOGGER_GET_FUNCTION __PRETTY_FUNCTION__
#else
#    define MYLOGGER_GET_FUNCTION __func__
#endif


#define LOGI ::myLogger::log_intermediate::make_info ( __FILE__, __LINE__ , MYLOGGER_GET_FUNCTION )
#define LOGD ::myLogger::log_intermediate::make_debug( __FILE__, __LINE__ , MYLOGGER_GET_FUNCTION )
#define LOGW ::myLogger::log_intermediate::make_warn ( __FILE__, __LINE__ , MYLOGGER_GET_FUNCTION )
#define LOGE ::myLogger::log_intermediate::make_error( __FILE__, __LINE__ , MYLOGGER_GET_FUNCTION )


namespace myLogger{

struct log_intermediate {
    std::stringstream buffer;

    const char* prefix;
    const char* suffix;
    const char* source;
    const size_t line;
    const char* function;
    const char* type;
    
#ifdef MYLOGGER_OUTPUT_FILENAME
    std::ofstream myFile;
#endif

    struct timespec ts;
    struct tm t;
    char buf[28]; 
    
#ifdef MYLOGGER_OUTPUT_FILENAME
    static constexpr auto prefix_i = "";
    static constexpr auto prefix_d = "";
    static constexpr auto prefix_w = "";
    static constexpr auto prefix_e = "";
    
    static constexpr auto suffix_i = "";
    static constexpr auto suffix_d = "";
    static constexpr auto suffix_w = "";
    static constexpr auto suffix_e = "";
#else
    static constexpr auto prefix_i = "";
    static constexpr auto prefix_d = "\x1b[32m";
    static constexpr auto prefix_w = "\x1b[33m";
    static constexpr auto prefix_e = "\x1b[31m";
    
    static constexpr auto suffix_i = "";
    static constexpr auto suffix_d = "\x1b[0m";
    static constexpr auto suffix_w = "\x1b[0m";
    static constexpr auto suffix_e = "\x1b[0m";
#endif
    
    static constexpr auto type_i = "[  INFO   ]";
    static constexpr auto type_d = "[  DEBUG  ]";
    static constexpr auto type_w = "[ WARNING ]";
    static constexpr auto type_e = "[  ERROR  ]";
    
    
    log_intermediate( log_intermediate&& a )
      : buffer( move( a.buffer ) )
      , prefix( a.prefix )
      , suffix( a.suffix )
      , source( a.source )
      , line( a.line )
      , function( a.function )
      , type(a.type) 
    { }
    
    log_intermediate( const char* p, const char* s, const char *t, const char* source_, const size_t line_, const char* function_ )
      : prefix( p )
      , suffix( s )
      , source( source_ )
      , line( line_ )
      , function( function_ )
      , type(t) 
#ifdef MYLOGGER_OUTPUT_FILENAME
      { if(!myFile.is_open()){ myFile = std::ofstream(MYLOGGER_OUTPUT_FILENAME, std::ios_base::app); } }
#else
      { }
#endif

    static log_intermediate make_info ( const char* s, const size_t l, const char* f ) { return log_intermediate( prefix_i, suffix_i, type_i, s, l, f ); }
    static log_intermediate make_debug( const char* s, const size_t l, const char* f ) { return log_intermediate( prefix_d, suffix_d, type_d, s, l, f ); }
    static log_intermediate make_warn ( const char* s, const size_t l, const char* f ) { return log_intermediate( prefix_w, suffix_w, type_w, s, l, f ); }
    static log_intermediate make_error( const char* s, const size_t l, const char* f ) { return log_intermediate( prefix_e, suffix_e, type_e, s, l, f ); }
    
    template < typename T >
    bool operator<<( const T& in ) noexcept {
      try{  buffer << in; return true; }
      catch ( std::exception& e ){ buffer << "<<<<<exception on " << __PRETTY_FUNCTION__ << " what=" << e.what() << ">>>>>"; return false;}
      catch ( ... ){ buffer << "<<<<<exception on " << __PRETTY_FUNCTION__ << " uknown>>>>>"; return false;}
    }
    
    ~log_intermediate() noexcept {
      try{ 
        // Get epoch time
        auto ret = clock_gettime(CLOCK_REALTIME, &ts);
        assert( ret >= 0);
        // localtime_r(&ts.tv_sec, &t);
        time_t tt;
        time(&tt);
        auto timeinfo = localtime(&tt);

        ret = strftime(buf, 32, "%Y/%m/%d %H:%M:%S", timeinfo);
        assert( ret >= 0);
        const int msec = ts.tv_nsec / 1000000;

        std::stringstream s;
        s << prefix
          << "[ "   << buf << "." << msec << "    "
          << type   << "    \""
          << source << "\" @ " << line << ' ' << function
          << "\t"
          << suffix
          << "\t"
          << buffer.str()
          << std::endl;

#ifdef MYLOGGER_OUTPUT_FILENAME
        myFile << s.str();
#else
        std::cout << s.str() << ::std::flush;
#endif
      }
      catch ( const std::exception& e )
      { std::cerr << "\n\n<<<<<\nexception on " << __PRETTY_FUNCTION__ << "\nwhat=" << e.what() << "\n>>>>>\n\n"; }
      catch ( ... )
      { std::cerr << "\n\n<<<<<\nexception on " << __PRETTY_FUNCTION__ << "\nunknown\n>>>>>\n\n"; }
    }
  };
}
