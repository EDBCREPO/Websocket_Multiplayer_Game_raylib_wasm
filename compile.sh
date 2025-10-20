export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH

em++ -o ./www/index.html client.cpp   \
     -I./include/frontend -L./lib     \
     -lraylib -lwebsocket.js -lembind \
     -s PTHREAD_POOL_SIZE=8 --bind    \
     -s USE_PTHREADS=1                \
     -s USE_GLFW=3                    \
     -s ASYNCIFY=1                    \
     -s FETCH=1                       \
     -s WASM=1                        \
     -s ERROR_ON_UNDEFINED_SYMBOLS=0  \
     --embed-file ./assets/sprites/tilemap.png

if [ ! $? -eq 0 ]; then
    echo "exit error"; exit;
fi

g++ -o server server.cpp -I./include/backend -L./lib -lssl -lcrypto -lz

if [ ! $? -eq 0 ]; then
    echo "exit error"; exit;
fi

./server