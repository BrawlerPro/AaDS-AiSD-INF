cmake_minimum_required(VERSION 3.17)
project(SkibidiToilet C)


set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)

#add_executable(SkibidiToilet ALA1/matrix.c ALA1/mat.c ALA1/AiSD1.c) # Первая лаба
add_executable(SkibidiToilet
        ALA2/queue_list.c
        ALA2/functions.c
        ALA2/AiSD2.c
) # Вторая Лаба

#find_program(VALGRIND "valgrind")
#if(VALGRIND)
#    add_custom_target(valgrind COMMAND "${VALGRIND}" --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes)
#endif()