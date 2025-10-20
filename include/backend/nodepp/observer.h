/*
 * Copyright 2023 The Nodepp Project Authors. All Rights Reserved.
 *
 * Licensed under the MIT (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://github.com/NodeppOfficial/nodepp/blob/main/LICENSE
 */

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef NODEPP_OBSERVER
#define NODEPP_OBSERVER

/*────────────────────────────────────────────────────────────────────────────*/

#include "event.h" 
#include "type.h"
#include "map.h"
#include "any.h"

/*────────────────────────────────────────────────────────────────────────────*/

namespace nodepp { class observer_t {
private:

    using V = any_t;
    using U = string_t; 
    using E = event_t<V,V>;
    
    using T = type::pair<V,E>;
    using P = type::pair<U,V>;

    map_t<string_t,T> list;

public: observer_t() noexcept {}
    
    /*─······································································─*/

    template< ulong N >
    observer_t ( const P (&args) [N] ) noexcept {
        ulong x=N; while( x-->0 ){
            T item; // memset( &item, 0, sizeof(T) );
            item.first = args[x].second;
            list[ args[x].first ]= item;
        }
    }

    virtual ~observer_t() noexcept {}
    
    /*─······································································─*/

    void off( const U& name, void* addr ) const noexcept { 
        if( !list.has( name ) ) { return; }
        /**/ list[ name ].second.off( addr );
    }

    template< class F >
    void* once( const U& name, F func ) const noexcept {
        if( !list.has( name ) ){ return nullptr; }
        auto n = list[ name ]; /*--*/
        return n.second.once( func );
    }

    template< class F >
    void* on( const U& name, F func ) const noexcept {
        if( !list.has( name ) ){ return nullptr; }
        auto n = list[ name ]; /**/
        return n.second.on( func );
    }
    
    /*─······································································─*/

    template< class F >
    void set( const U& name, const F& value ) const {
        if( !list.has( name ) ){ throw except_t("field not found:",name); }
        auto n = list[ name ]; n.second.emit( n.first, value );
        /*----*/ list[name].first = value; 
    }

    /*─······································································─*/

    const V get( const U& name ) const { if( !list.has( name ) ){ 
        throw except_t( "field not found:", name ); 
    }   return list[ name ].first; }
    
    /*─······································································─*/

    void clear( string_t name ) const noexcept { 
        if( list.has( name ) ){ list[name].second.clear(); }
    }

    bool empty() const noexcept { return list.empty(); }

    ulong size() const noexcept { return list.size(); }

    void clear() const noexcept { list.clear(); }
    
    /*─······································································─*/

    const V operator[]( const U& name ) const { return get( name ); }
    
};}

/*────────────────────────────────────────────────────────────────────────────*/

#endif

/*────────────────────────────────────────────────────────────────────────────*/
