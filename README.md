# Websocket Multiplayer Game - raylib

This is a simple implementation of a real-time Multiplayer game using C++, raylib and nodepp

inspired by: [Create your own online multiplayer Game](https://medium.com/@EDBCBlog/create-your-own-online-multiplayer-small-fast-and-fun-with-raylib-nodepp-and-websockets-190f5c174094)

# Dependencies
```bash
📌 nodepp-wasm: https://github.com/NodeppOficial/nodepp-wasm
📌 nodepp:      https://github.com/NodeppOficial/nodepp
📌 raylib:      https://www.raylib.com/
```

# Preview
[preview](https://github.com/user-attachments/assets/a2675277-807a-4e65-bf94-b5448d270fe8)

# Build & Run The Game
```bash
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
```

# Build & Run The Server
```bash
time g++ -o server server.cpp -I./include -L./lib -lssl -lcrypto ; ./server
```
