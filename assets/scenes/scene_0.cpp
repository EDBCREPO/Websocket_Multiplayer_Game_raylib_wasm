#pragma once

/*────────────────────────────────────────────────────────────────────────────*/

#include "../script/player.cpp"
#include "../script/npc.cpp"

/*────────────────────────────────────────────────────────────────────────────*/

namespace rl { namespace scene {

    void scene_0( ptr_t<Scene> self ) {

        event_t<rl::game::player_t> event;

        self->SetAttr( "onSend", event );
        self->AppendItem( "npc", rl::game::npc );
        self->AppendItem( "player", rl::game::player );

      //self->onDraw([](){ ClearBackground( BLACK ); });

    }

}}

/*────────────────────────────────────────────────────────────────────────────*/