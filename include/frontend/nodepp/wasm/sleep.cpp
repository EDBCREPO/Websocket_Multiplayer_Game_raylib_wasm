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
#include <emscripten/emscripten.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    inline ulong get_new_timeval() { char res [32];

        auto size = EM_ASM_INT({
            let data = Date.now() + ""; /*-----------------*/
            stringToUTF8( data, $0, $1 ); return data.length;
        }, res, 32 );

        return string::to_ullong( string_t( res, size ) );
    }

    ulong  micros(){ return get_new_timeval() / 1000000; }

    ulong seconds(){ return get_new_timeval() / 1000; }

    ulong  millis(){ return get_new_timeval(); }

}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace process {

    void delay( ulong time ){ emscripten_sleep( time ); }

    void yield(){ emscripten_sleep(TIMEOUT); }

    ulong now(){ return millis(); }

}}

/*────────────────────────────────────────────────────────────────────────────*/
