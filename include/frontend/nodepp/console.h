/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_CONSOLE
#define NODEPP_CONSOLE

/*────────────────────────────────────────────────────────────────────────────*/

#include <emscripten/console.h>
#include "conio.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace console {

    template< class... T >
    void err( const T&... args ){ emscripten_console_error( conio::join( args... ).get() ); }

    template< class... T >
    void log( const T&... args ){ emscripten_console_log( conio::join( args... ).get() ); }

    template< class... T >
    void pout( const T&... args ){ emscripten_console_log( conio::join( args... ).get() ); }

    template< class... T >
    int scan( const T&... args ){ return conio::scan( args... ); }

    void wait(){ char x; conio::scan("%c",&x); }

    void clear(){ conio::clear(); }
    
    /*─······································································─*/

    template< class... T >
    void error( const T&... args ){ 
        emscripten_console_error( conio::join( args... ).get() );
    }

    template< class... T >
    void warning( const T&... args ){ 
        emscripten_console_warn( conio::join( args... ).get() );
    }

    template< class... T >
    void success( const T&... args ){ 
        emscripten_console_log( conio::join( args... ).get() );
    }

    template< class... T >
    void done( const T&... args ){ 
        emscripten_console_log( conio::join( args... ).get() );
    }

    template< class... T >
    void info( const T&... args ){ 
        emscripten_console_dbg( conio::join( args... ).get() );
    }

}}

/*────────────────────────────────────────────────────────────────────────────*/

#endif