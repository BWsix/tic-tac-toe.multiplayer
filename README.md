# Tic-Tac-Toe.multiplayer

I somehow managed to build a _barely functional_ multiplayer game :o

## How to build this

```bash
cd /path/to/tic-tac-toe.multiplayer
cmake -S . -B build
cmake --build build
```

After building, both client and server executable can now be found in
`./build/tic-tac-toe/`

## How to play this

- `server`
  - usage: `server <port number>`
  - run server before client
- `client` \* 2
  - usage: `client <server address> <port number>`

## Stuff

- Raylib: https://www.raylib.com
- Hands-On Network Programming with C Learn socket programming in C and write
  secure and optimized network code (Lewis Van Winkle)
