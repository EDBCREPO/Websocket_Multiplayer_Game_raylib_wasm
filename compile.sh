em++ -o www/index.html main.cpp      \
     -I./include -L./lib             \
     -lraylib -lwebsocket.js         \
     -s PTHREAD_POOL_SIZE=8          \
     -s USE_PTHREADS=1               \
     -s USE_GLFW=3                   \
     -s ASYNCIFY=1                   \
     -s FETCH=1                      \
     -s WASM=1                       \
     -s WEBSOCKET_URL=1              \
     -s WEBSOCKET_SUBPROTOCOL=1      \
     -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
     --embed-file ./assets/sprites/tilemap.png