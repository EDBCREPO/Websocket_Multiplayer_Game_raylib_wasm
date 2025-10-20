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
#include <emscripten/websocket.h>

namespace nodepp { queue_t<void*> user; }

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class ws_t {
protected:

    bool is_state( uchar value ) const noexcept {
        if( obj->state & value ){ return true; }
    return false; }

    void set_state( uchar value ) const noexcept {
    if( obj->state & KILL ){ return; }
        obj->state = value;
    }

    enum FILE_STATE {
        UNKNOWN = 0b00000000,
        OPEN    = 0b00000001,
        CLOSE   = 0b00000010,
        KILL    = 0b00000100,
        REUSE   = 0b00001000,
        DISABLE = 0b00001110
    };

protected:

    struct NODE {
        char state = FILE_STATE::CLOSE;
        int     fd =-1;
    };  ptr_t<NODE> obj;

private:

    static EM_BOOL WS_EVENT_MESSAGE( int /*unused*/, const EmscriptenWebSocketMessageEvent* ev, void* userData ) {
        if( userData == nullptr ){ return EM_FALSE; }  auto user = type::cast<queue_t<void*>>( userData );
        auto x = user->first(); while( x != nullptr ){ auto z = type::cast<ws_t>( x->data ); auto y = x->next;
            if( z->obj->fd==ev->socket ){ z->onData.emit( string_t( (char*)ev->data, ev->numBytes ) ); break; }
        x = y; } return EM_TRUE;
    }

    static EM_BOOL WS_EVENT_CLOSE( int /*unused*/, const EmscriptenWebSocketCloseEvent* ev, void* userData ) {
        if( userData == nullptr ){ return EM_FALSE; }  auto user = type::cast<queue_t<void*>>( userData );
        auto x = user->first(); while( x != nullptr ){ auto z = type::cast<ws_t>( x->data ); auto y = x->next;
            if( z->obj->fd==ev->socket ){ user->erase(x); z->close(); break; }
        x = y; } return EM_TRUE;
    }

    static EM_BOOL WS_EVENT_OPEN( int /*unused*/, const EmscriptenWebSocketOpenEvent* ev, void* userData ) {
        if( userData == nullptr ){ return EM_FALSE; }  auto user = type::cast<queue_t<void*>>( userData );
        auto x = user->first(); while( x != nullptr ){ auto z = type::cast<ws_t>( x->data ); auto y = x->next;
            if( z->obj->fd==ev->socket ){ z->set_state( FILE_STATE::OPEN );
                z->onOpen.emit(*z); /*-*/ z->onConnect.emit(*z); break; }
        x = y; } return EM_TRUE;
    }

public: ws_t() noexcept : obj( new NODE() ){}

    event_t<ws_t>      onConnect;
    event_t<>          onDrain;
    event_t<except_t>  onError;
    event_t<>          onClose;
    event_t<ws_t>      onOpen;
    event_t<string_t>  onData;

    /*─······································································─*/

    virtual ~ws_t() noexcept { if( obj.count() > 1 ) { return; } free(); }

    /*─······································································─*/

    ws_t( const string_t& url ) noexcept : obj( new NODE() ) {
        auto self = type::bind( this );

        if( !emscripten_websocket_is_supported() ){
            onError.emit( "WS not Supported" );
            close(); return;
        }

        function_t<void> clb = [=](){
            ptr_t<EmscriptenWebSocketCreateAttributes> attr ;
            attr = new EmscriptenWebSocketCreateAttributes();

            attr->url                = url.c_str();
            attr->protocols          = nullptr;
            attr->createOnMainThread = EM_TRUE;

            self->obj->fd = emscripten_websocket_new( &attr ); user.push( &self );
            emscripten_websocket_set_onopen_callback   ( self->obj->fd, &user, WS_EVENT_OPEN    );
            emscripten_websocket_set_onclose_callback  ( self->obj->fd, &user, WS_EVENT_CLOSE   );
            emscripten_websocket_set_onmessage_callback( self->obj->fd, &user, WS_EVENT_MESSAGE );
        };

        process::add( coroutine::add( COROUTINE(){
        coBegin ; clb(); coNext;

            do { ushort wait = 0; 
                 emscripten_websocket_get_ready_state( self->obj->fd, &wait );
            if( wait==0 ){ return 1; } elif( wait!=1 ) {
                self->onError.emit( "Something Went Wrong" ); return -1;
            }} while(0);

            coWait( self->is_state( FILE_STATE::OPEN ) ); self->close();

        coFinish
        }));

    }

    /*─······································································─*/

    void free()  const noexcept { if( !is_available() ){ return; } 
        emscripten_websocket_delete( obj->fd );
        set_state( FILE_STATE::CLOSE ); 
        onConnect.clear(); 
        onError  .clear();
        onData   .clear(); onClose.emit();
    }

    void close() const noexcept { if( !is_available() ){ return; }
         onDrain.emit(); free();
    }

    /*─······································································─*/

    bool is_closed()    const noexcept { return is_state( FILE_STATE::DISABLE ); }
    bool is_available() const noexcept { return !is_closed(); }
    int  get_fd()       const noexcept { return obj->fd; }

    /*─······································································─*/

    string_t read( ulong /*unused*/ ) const noexcept { return nullptr; }

    int write( string_t msg ) const noexcept {
        if( msg.empty() ){ return 0; } if( is_closed() ){ return -1; }
        return emscripten_websocket_send_binary( obj->fd, msg.get(), msg.size() );
    }

}; using tcp_t = ws_t; }

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { namespace ws {
    ws_t  client( const string_t& url ) { return ws_t(url); }
    ws_t connect( const string_t& url ) { return ws_t(url); }
}}

/*────────────────────────────────────────────────────────────────────────────*/
