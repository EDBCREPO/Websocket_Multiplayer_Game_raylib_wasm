/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#pragma once
#include <emscripten.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class date_t {
protected:

    struct NODE {
        bool utc;
        uint day;
        uint year;
        uint hour;
        uint month;
        uint minute;
        uint second;
    };  ptr_t<NODE> obj;

    /*─······································································─*/

    void set_time( ulong time, bool utc ) const noexcept {

        obj->second = EM_ASM_INT({ let date = new Date($1);
            if( $0 ){ return date.getUTCSeconds(); }
            else    { return date.getSeconds(); } 
        }, utc, time );
        
        obj->month = EM_ASM_INT({ let date = new Date($1);
            if( $0 ){ return date.getUTCMonth(); }
            else    { return date.getMonth(); } 
        }, utc, time );
    
        obj->minute = EM_ASM_INT({ let date = new Date($1);
            if( $0 ){ return date.getUTCMinutes(); }
            else    { return date.getMinutes(); } 
        }, utc, time );

        obj->year = EM_ASM_INT({ let date = new Date($1);
            if( $0 ){ return date.getUTCFullYear(); }
            else    { return date.getFullYear(); } 
        }, utc, time );
        
        obj->hour = EM_ASM_INT({ let date = new Date($1);
            if( $0 ){ return date.getUTCHours(); }
            else    { return date.getHours(); } 
        }, utc, time );
        
        obj->day = EM_ASM_INT({ let date = new Date($1);
            if( $0 ){ return date.getUTCDay(); }
            else    { return date.getDay(); } 
        }, utc, time );

        obj->utc = utc;

    }

    void get_time( bool utc ) const noexcept {

        obj->second = EM_ASM_INT({ let date = new Date();
            if( $0 ){ return date.getUTCSeconds(); }
            else    { return date.getSeconds(); } 
        }, utc );
    
        obj->minute = EM_ASM_INT({ let date = new Date();
            if( $0 ){ return date.getUTCMinutes(); }
            else    { return date.getMinutes(); } 
        }, utc );
        
        obj->month = EM_ASM_INT({ let date = new Date();
            if( $0 ){ return date.getUTCMonth(); }
            else    { return date.getMonth(); } 
        }, utc );
        
        obj->hour = EM_ASM_INT({ let date = new Date();
            if( $0 ){ return date.getUTCHours(); }
            else    { return date.getHours(); } 
        }, utc );

        obj->year = EM_ASM_INT({ let date = new Date();
            if( $0 ){ return date.getUTCFullYear(); }
            else    { return date.getFullYear(); } 
        }, utc );
        
        obj->day = EM_ASM_INT({ let date = new Date();
            if( $0 ){ return date.getUTCDay(); }
            else    { return date.getDay(); } 
        }, utc );

        obj->utc = utc;
        
    }
    
public:

    template< class... V >
    date_t( const V&... args ) noexcept : obj( new NODE() ) { set_date( args... ); }

    date_t() noexcept : obj( new NODE() ) { set_date( false ); }
    
    virtual ~date_t() noexcept {}

    /*─······································································─*/

    bool operator==( const date_t& other ) const noexcept { return get_stamp()==other.get_stamp(); }
    bool operator<=( const date_t& other ) const noexcept { return get_stamp()<=other.get_stamp(); }
    bool operator>=( const date_t& other ) const noexcept { return get_stamp()>=other.get_stamp(); }
    bool operator< ( const date_t& other ) const noexcept { return get_stamp()< other.get_stamp(); }
    bool operator> ( const date_t& other ) const noexcept { return get_stamp()> other.get_stamp(); }

    /*─······································································─*/

    void operator+=( const date_t& other ) const noexcept {
         set_stamp( get_stamp() + other.get_stamp(), obj->utc );
    }

    void operator-=( const date_t& other ) const noexcept {
         set_stamp( get_stamp() - other.get_stamp(), obj->utc );
    }

    void operator*=( const date_t& other ) const noexcept {
         set_stamp( get_stamp() * other.get_stamp(), obj->utc );
    }

    void operator/=( const date_t& other ) const noexcept {
         set_stamp( get_stamp() / other.get_stamp(), obj->utc );
    }

    /*─······································································─*/

    void set_stamp( const time_t& time, const bool& utc ) const noexcept {
         set_time ( time, utc );
   }
    
    /*─······································································─*/

    void set_date( const bool& utc ) const noexcept {
         set_utc(utc); get_time( utc );
    }

    void set_date( const uint& year, const bool& utc ) const noexcept {
         set_utc(utc); set_year(year);
    }

    void set_date( const uint& year, const uint& month, const bool& utc ) const noexcept {
         set_utc(utc); set_year(year); set_month(month);
    }

    void set_date( const uint& year, const uint& month, const uint& day, const bool& utc ) const noexcept {
         set_utc(utc); set_year(year); set_month(month); set_day(day);
    }

    void set_date( const uint& year, const uint& month, const uint& day, const uint& hour, const bool& utc ) const noexcept {
         set_utc(utc); set_year(year); set_month(month); set_day(day); set_hour(hour);
    }

    void set_date( const uint& year, const uint& month, const uint& day, const uint& hour, const uint& min, const bool& utc ) const noexcept {
         set_utc(utc); set_year(year); set_month(month); set_day(day); set_hour(hour); set_minute(min);
    }

    void set_date( const uint& year, const uint& month, const uint& day, const uint& hour, const uint& min, const uint& second, const bool& utc ) const noexcept {
         set_utc(utc); set_year(year); set_month(month); set_day(day); set_hour(hour); set_minute(min); set_second(second);
    }
    
    /*─······································································─*/
    
    void set_year  ( uint year  ) const noexcept { obj->year   = year;  }
    void set_month ( uint month ) const noexcept { obj->month  = month; }
    void set_second( uint sec   ) const noexcept { obj->second = sec;   }
    void set_minute( uint min   ) const noexcept { obj->minute = min;   }
    void set_hour  ( uint hour  ) const noexcept { obj->hour   = hour;  }
    void set_day   ( uint day   ) const noexcept { obj->day    = day;   }
    void set_utc   ( bool utc   ) const noexcept { obj->utc    = utc;   }

    /*─······································································─*/

    string_t get_fulltime() const noexcept { 
        char res [UNBFF_SIZE]; auto size = EM_ASM_INT({ 
            let data = new Date();
            if( $0 ){
                data.setUTCDate($1);
                data.setUTCYear($2);
                data.setUTCHour($3);
                data.setUTCMonth($4);
                data.setUTCMinutes($5);
                data.setUTCSeconds($6);
            } else {
                data.setDate($1);
                data.setYear($2);
                data.setHour($3);
                data.setMonth($4);
                data.setMinutes($5);
                data.setSeconds($6);
            }
            stringToUTF8( data, $7, $8 ); return date.length;
        }, obj->utc, obj->day, obj->year, obj->hour, obj->month, obj->minute, obj->second, res, UNBFF_SIZE ); 
        
        return string_t( res, size );
    }
    
    uint get_year() const noexcept { 
        return EM_ASM_INT({ 
            let data = new Date();
            if( $0 ){
                data.setUTCDate($1);
                data.setUTCYear($2);
                data.setUTCHour($3);
                data.setUTCMonth($4);
                data.setUTCMinutes($5);
                data.setUTCSeconds($6);
                return data.getUTCFullYear(); 
            } else {
                data.setDate($1);
                data.setYear($2);
                data.setHour($3);
                data.setMonth($4);
                data.setMinutes($5);
                data.setSeconds($6);
                return data.getFullYear(); 
            }
        }, obj->utc, obj->day, obj->year, obj->hour, obj->month, obj->minute, obj->second );
    }
    
    uint get_month() const noexcept {
        return EM_ASM_INT({ 
            let data = new Date();
            if( $0 ){
                data.setUTCDate($1);
                data.setUTCYear($2);
                data.setUTCHour($3);
                data.setUTCMonth($4);
                data.setUTCMinutes($5);
                data.setUTCSeconds($6);
                return data.getUTCMonth(); 
            } else {
                data.setDate($1);
                data.setYear($2);
                data.setHour($3);
                data.setMonth($4);
                data.setMinutes($5);
                data.setSeconds($6);
                return data.getMonth(); 
            }
        }, obj->utc, obj->day, obj->year, obj->hour, obj->month, obj->minute, obj->second );
    }

    uint get_hour() const noexcept { 
        return EM_ASM_INT({ 
            let data = new Date();
            if( $0 ){
                data.setUTCDate($1);
                data.setUTCYear($2);
                data.setUTCHour($3);
                data.setUTCMonth($4);
                data.setUTCMinutes($5);
                data.setUTCSeconds($6);
                return data.getUTCHours(); 
            } else {
                data.setDate($1);
                data.setYear($2);
                data.setHour($3);
                data.setMonth($4);
                data.setMinutes($5);
                data.setSeconds($6);
                return data.getHours(); 
            }
        }, obj->utc, obj->day, obj->year, obj->hour, obj->month, obj->minute, obj->second );
    }
    
    uint get_day() const noexcept {
        return EM_ASM_INT({ 
            let data = new Date();
            if( $0 ){
                data.setUTCDate($1);
                data.setUTCYear($2);
                data.setUTCHour($3);
                data.setUTCMonth($4);
                data.setUTCMinutes($5);
                data.setUTCSeconds($6);
                return data.getUTCDay(); 
            } else {
                data.setDate($1);
                data.setYear($2);
                data.setHour($3);
                data.setMonth($4);
                data.setMinutes($5);
                data.setSeconds($6);
                return data.getDay(); 
            }
        }, obj->utc, obj->day, obj->year, obj->hour, obj->month, obj->minute, obj->second );
    }

    uint get_minute() const noexcept {
        return EM_ASM_INT({ 
            let data = new Date();
            if( $0 ){
                data.setUTCDate($1);
                data.setUTCYear($2);
                data.setUTCHour($3);
                data.setUTCMonth($4);
                data.setUTCMinutes($5);
                data.setUTCSeconds($6);
                return data.getUTCMinutes(); 
            } else {
                data.setDate($1);
                data.setYear($2);
                data.setHour($3);
                data.setMonth($4);
                data.setMinutes($5);
                data.setSeconds($6);
                return data.getMinutes(); 
            }
        }, obj->utc, obj->day, obj->year, obj->hour, obj->month, obj->minute, obj->second );
    }
    
    uint get_second() const noexcept {
        return EM_ASM_INT({ 
            let data = new Date();
            if( $0 ){
                data.setUTCDate($1);
                data.setUTCYear($2);
                data.setUTCHour($3);
                data.setUTCMonth($4);
                data.setUTCMinutes($5);
                data.setUTCSeconds($6);
                return data.getUTCSeconds(); 
            } else {
                data.setDate($1);
                data.setYear($2);
                data.setHour($3);
                data.setMonth($4);
                data.setMinutes($5);
                data.setSeconds($6);
                return data.getSeconds(); 
            }
        }, obj->utc, obj->day, obj->year, obj->hour, obj->month, obj->minute, obj->second );
    }

    ulong get_stamp() const noexcept { 
        char raw  [UNBFF_SIZE]; 
        auto size = EM_ASM_INT({ 
            let data = new Date();
            if( $0 ){
                data.setUTCDate($1);
                data.setUTCYear($2);
                data.setUTCHour($3);
                data.setUTCMonth($4);
                data.setUTCMinutes($5);
                data.setUTCSeconds($6);
            } else {
                data.setDate($1);
                data.setYear($2);
                data.setHour($3);
                data.setMonth($4);
                data.setMinutes($5);
                data.setSeconds($6);
            }
            let info = data.getTime().toString(); stringToUTF8( data, $7, $8 ); return info.length;
        }, obj->utc, obj->day, obj->year, obj->hour, obj->month, obj->minute, obj->second, raw, UNBFF_SIZE ); return string::to_ullong( string_t( raw, size ) );
    }

};}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp {

    date_t operator+( const date_t& A, const date_t& B ){
        date_t C; C.set_stamp( A.get_stamp() + B.get_stamp(), false );
        return C;
    }

    date_t operator-( const date_t& A, const date_t& B ){
        date_t C; C.set_stamp( A.get_stamp() - B.get_stamp(), false );
        return C;
    }

    date_t operator*( const date_t& A, const date_t& B ){
        date_t C; C.set_stamp( A.get_stamp() * B.get_stamp(), false );
        return C;
    }

    date_t operator/( const date_t& A, const date_t& B ){
        date_t C; C.set_stamp( A.get_stamp() / B.get_stamp(), false );
        return C;
    }

}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace date {

    uint now(){ return date_t().get_stamp(); }

    string_t fulltime(){ return date_t().get_fulltime(); }

    uint day( const bool& utc ){ return date_t(utc).get_day(); }

    uint year( const bool& utc ){ return date_t(utc).get_year(); }

    uint hour( const bool& utc ){ return date_t(utc).get_hour(); }

    uint month( const bool& utc ){ return date_t(utc).get_month(); }

    uint minute( const bool& utc ){ return date_t(utc).get_minute(); }

    uint second( const bool& utc ){ return date_t(utc).get_second(); }

}}

/*────────────────────────────────────────────────────────────────────────────*/
