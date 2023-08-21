find_package(raylib 4.5 REQUIRED)

add_executable(client client.c)
target_compile_options(client PRIVATE -Wall -Wextra -ggdb -O2)
target_link_libraries(client raylib)

add_executable(server server.c)
target_compile_options(server PRIVATE -Wall -Wextra -ggdb -O2)
