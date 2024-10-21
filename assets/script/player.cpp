#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

namespace rl { namespace game {

    struct player_t {
        Vector2 pos = { GetRenderWidth()/2.0f, GetRenderHeight()/2.0f };
        Vector2 frm = { 1, 12 };
        Vector2 dir = { 0, 0 };
        float speed = 100.0f;
        Color   col = YELLOW;
        bool  flip  = 0;
        uchar state = 0;
    };

    void player( ptr_t<Item> self ) {

        struct NODE {
            Texture img = LoadTexture( "assets/sprites/tilemap.png" );
            player_t dt ;
        };  ptr_t<NODE> obj = new NODE();
    
    /*─······································································─*/

        obj->dt.col = array_t<Color>({ RED, BLUE, GREEN, YELLOW })[ rand()%4 ];

        self->SetAttr("getPos",function_t<Vector2>([=](){
            return obj->dt.pos;
        }));
    
    /*─······································································─*/

        self->onLoop([=]( float delta ){ [=](){
            static uchar x = 0; static player_t prev;
        coStart; coDelay(100);

            if( obj->dt.state == 1 ){ obj->dt.frm.x = 1+x; x++; x %= 4; }
          elif( obj->dt.state == 0 ){ obj->dt.frm.x = 0; }

            if( memcmp( &prev, &obj->dt, sizeof(player_t) )!=0 ){
                GetScene().GetAttr("onSend").as<event_t<player_t>>()
                          .emit( obj->dt );
            }   prev = obj->dt;

        coStop
        }();});
    
    /*─······································································─*/

        self->onLoop([=]( float delta ){ obj->dt.state=0;

              if( obj->dt.pos.y > GetRenderHeight() ){ obj->dt.pos.y = GetRenderHeight(); }
            elif( obj->dt.pos.y < 0 )                { obj->dt.pos.y = 0; }

              if( obj->dt.pos.x > GetRenderWidth() ){ obj->dt.pos.x = GetRenderWidth(); }
            elif( obj->dt.pos.x < 0 )               { obj->dt.pos.x = 0; }

              if( IsKeyDown( 'W' ) ){ obj->dt.dir.y =-1; obj->dt.state=1; obj->dt.flip=0; }
            elif( IsKeyDown( 'S' ) ){ obj->dt.dir.y = 1; obj->dt.state=1; obj->dt.flip=1; }
            else                    { obj->dt.dir.y = 0; }

              if( IsKeyDown( 'A' ) ){ obj->dt.dir.x =-1; obj->dt.state=1; obj->dt.flip=0; }
            elif( IsKeyDown( 'D' ) ){ obj->dt.dir.x = 1; obj->dt.state=1; obj->dt.flip=1; }
            else                    { obj->dt.dir.x = 0;}
            
            obj->dt.pos.x += obj->dt.dir.x * delta * obj->dt.speed; 
            obj->dt.pos.y += obj->dt.dir.y * delta * obj->dt.speed;

        });
    
    /*─······································································─*/

        self->onDraw([=](){
            DrawTexturePro( obj->img, 
                { obj->dt.frm.x*16, obj->dt.frm.y*16, 16*(obj->dt.flip?1.0f:-1.0f), 16 },
                { obj->dt.pos.x, obj->dt.pos.y, 32, 32 }, { 16, 16 }, 0, obj->dt.col
            );
        });
    
    /*─······································································─*/

        self->onRemove([=](){
            if( IsTextureReady(obj->img) ){ UnloadTexture( obj->img ); }
        });

    }

}}

/*────────────────────────────────────────────────────────────────────────────*/