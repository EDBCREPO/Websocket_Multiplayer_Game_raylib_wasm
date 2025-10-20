#include <nodepp/nodepp.h>
#include <nodepp/timer.h>
#include <express/http.h>
#include <nodepp/path.h>
#include <nodepp/ws.h>
#include <nodepp/fs.h>

using namespace nodepp;

queue_t<ws_t> list; 

void onMain(){

    auto app = express::http::add(); 
    auto srv = app.listen( "localhost", 8000, [=](socket_t){
        console::log("server started at http://localhost:8000");
    }); ws::server( srv );

    app.ALL([=]( express_http_t cli ){
        cli.header( "Cross-Origin-Opener-Policy"  , "same-origin"  );
        cli.header( "Cross-Origin-Embedder-Policy", "require-corp" );
    });

    app.USE( express::http::file( "./www" ) );
    
    srv.onConnect([=]( ws_t cli ){

        list.push( cli ); auto ID = list.last();

        cli.onData([=]( string_t data ){
            auto msg = regex::format("player_${0}\n${1}",cli.get_fd(),data);
            for( auto &x: list.data() ){
                if( x.get_fd() == cli.get_fd() )
                  { continue; } x.write( msg ); 
            }
        });

        cli.onClose([=](){
            auto msg = regex::format("player_${0}",cli.get_fd());
            console::log( "Disconnected" ); list.erase( ID );
            for( auto &x: list.data() ){
                if( x.get_fd() == cli.get_fd() )
                  { continue; } x.write( msg ); 
            }
        });

        console::log( "Connected" );

    });

}