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
#include <emscripten/fetch.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { using header_t = map_t< string_t, string_t >; }

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { struct fetch_t {

    string_t  url;
    string_t  body;
    string_t  code;
    uchar     status;
    header_t  headers;
    ulong     timeout= 0;
    string_t  method = "GET";

};  using http_t = fetch_t; }

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace generator { GENERATOR( fetch ) {
protected:

    struct NODE {
        function_t<void,except_t> rej;
        function_t<void,fetch_t>  res;
        array_t<const char*> hdr;
        bool state = false;
        fetch_t ctx;
    }; ptr_t<NODE> obj;
    
    /*─······································································─*/

    static void callback( emscripten_fetch_t* args ) {

        auto self = type::cast<fetch>( args->userData );

        auto rej = self->obj->rej;
        auto res = self->obj->res; self->close();

        if( args->status == 0 ){
            rej( except_t( "Something Went Wrong" ) );
            emscripten_fetch_close( args ); return;
        }

        fetch_t out;
                out.body   = string_t( args->data, args->numBytes );
                out.code   = args->statusText;
                out.status = args->status;
                out.url    = args->url;

        string_t headers ( emscripten_fetch_get_response_headers_length( args ), '\0' );
        emscripten_fetch_get_response_headers( args, headers.get(), headers.size() );
        forEach( x, string::split( headers, '\n' ) ){
            auto y = x.find( ": " ); if( y == nullptr ){ break; }
            out.headers[ x.slice(0,y[0]) ] = x.slice(y[1],-2);
        }

        emscripten_fetch_close( args ); res( out ); 

    }

public:

    fetch( fetch_t ctx ) : obj( new NODE() ) {
        if( !url::is_valid( ctx.url ) ){ throw except_t("invalid URL"); }
        obj->state = true; obj->ctx = ctx;
    }

    fetch() : obj( new NODE() ) { obj->state = false; }

    virtual ~fetch() noexcept {}
    
    /*─······································································─*/

    coEmit(){
    coBegin

        do{ emscripten_fetch_attr_t attr;
            emscripten_fetch_attr_init( &attr );
            memcpy( attr.requestMethod, obj->ctx.method.get(), obj->ctx.method.size() );
    
            if( obj->ctx.timeout != 0 ){ attr.timeoutMSecs = obj->ctx.timeout; }
    
            for( auto& x: obj->ctx.headers.data() ){
                obj->hdr.push( x.first.c_str()  );
                obj->hdr.push( x.second.c_str() );
            }   obj->hdr.push( nullptr );
    
            attr.attributes     = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
            attr.userData       = type::cast<void>( this );
            attr.requestHeaders = (char**) obj->hdr.data();
            attr.requestDataSize= obj->ctx.body.size();
            attr.requestData    = obj->ctx.body.get();
            attr.onsuccess      = callback;
            attr.onerror        = callback;
    
            emscripten_fetch( &attr, obj->ctx.url.get() );

        } while(0); coWait( is_closed()==false );

    coFinish
    }
    
    /*─······································································─*/

    void set_rejected_callback( function_t<void,except_t> callback ) const noexcept {
         obj->rej = callback;
    }

    void set_resolved_callback( function_t<void,fetch_t> callback ) const noexcept {
         obj->res = callback;
    }
    
    /*─······································································─*/

    bool is_closed() const noexcept { return obj->state==false; }

    void     close() const noexcept {        obj->state =false; }

};}}

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace fetch {

    promise_t<fetch_t,except_t> add ( const fetch_t& fetch ) {
    return promise_t<fetch_t,except_t>([=]( function_t<void,fetch_t> res, function_t<void,except_t> rej ){

        if( !url::is_valid( fetch.url ) ){ rej(except_t("invalid URL")); return; }

        auto task = type::bind( new generator::fetch( fetch ) );
             task->set_resolved_callback( res );
             task->set_rejected_callback( rej );

        process::add( task );

    }); }


}}

/*────────────────────────────────────────────────────────────────────────────*/
