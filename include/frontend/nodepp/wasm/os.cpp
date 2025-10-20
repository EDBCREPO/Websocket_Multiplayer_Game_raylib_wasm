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
#include <cerrno>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace os {

    void exec( string_t cmd ){ ::emscripten_run_script( cmd.get() ); }

    void call( string_t cmd ){ ::emscripten_run_script( cmd.get() ); }

    /*─······································································─*/
    
    string_t tmp(){ return "/tmp"; }

    string_t cwd(){ return "/"; }

    /*─······································································─*/

    uint error(){ return errno; }

}}

/*────────────────────────────────────────────────────────────────────────────*/