
#include <nodepp/nodepp.h>
#include <nodepp/date.h>

using namespace nodepp;

#include "assets/script/game.cpp"
#include "assets/scenes/scene_0.cpp"

void onMain() {

    rl::Init( 300, 300, 60, "Multiplayer" );

    rl::AppendScene( rl::scene::scene_0 );

    rl::onClose([](){
        console::log("Closed");
        rl::Close();
    });

}