#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include <nodepp/map.h>
#include <nodepp/ws.h>

/*────────────────────────────────────────────────────────────────────────────*/

namespace rl { namespace game {

    void npc( ptr_t<Item> self ) {

        struct NODE {
            Texture img = LoadTexture( "assets/sprites/tilemap.png" );
            map_t<string_t,player_t> list;
        };  ptr_t<NODE> obj = new NODE();
    
    /*─······································································─*/

        ws::connect( "ws://localhost:8000" ).onConnect([=]( ws_t cli ){

            auto name= string::format( "player_%d", rand()%10000 );

            cli.onData([=]( string_t data ){

                auto pos = data.find('\n'); if( pos == nullptr )
                { obj->list.erase( name ); return; }

                auto name= data.slice( 0, pos[0] ); player_t mem;
                auto info= data.slice( pos[1] );

                memmove( &mem, info.get(), sizeof( player_t ) );
                obj->list[name] = mem;

            });

            auto ids = GetScene().GetAttr("onSend")
            .as<event_t<player_t>>().on([=]( player_t data ){
                cli.write( string_t( 
                    (char*) &data, sizeof(player_t) 
                )); 
            });

            cli.onClose([=](){
                console::log( "Disconnected" );
                process::clear( ids );
                process::exit(1);
            });

            console::log( "Connected" );

        });
    
    /*─······································································─*/

        self->onDraw([=](){ for( auto &x: obj->list.get() ){
            auto z = x.second; DrawTexturePro( obj->img, 
              { z.frm.x*16, z.frm.y*16, 16*(z.flip?1.0f:-1.0f), 16 },
              { z.pos.x, z.pos.y, 32, 32 }, { 16, 16 }, 0, z.col
            );
        }});
    
    /*─······································································─*/

        self->onLoop([=]( float delta ){ [=](){
            static uchar y = 0; static Vector2 prev;
        coStart; coDelay(100);

            do { for( auto &x: obj->list.get() ){ auto z = x.second;
                  if( z.state == 1 ){ z.frm.x = 1+(y%4); }
                elif( z.state == 0 ){ z.frm.x = 0; }
            }} while(0); y++;

        coStop
        }();});
    
    /*─······································································─*/

        self->onRemove([=](){
            if( IsTextureValid(obj->img) ){ UnloadTexture( obj->img ); }
        });

    }

}}

/*────────────────────────────────────────────────────────────────────────────*/