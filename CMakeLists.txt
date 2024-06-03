cmake_minimum_required(VERSION 3.17)
project(SkibidiToilet C)


set(CMAKE_C_STANDARD 99)

#add_executable(SkibidiToilet
#        ALA1/matrix.c
#        ALA1/mat.c
#        ALA1/AiSD1.c
#) # Первая лаба
#add_executable(SkibidiToilet
#        ALA2/queue_list.c
#        ALA2/functions.c
#        ALA2/AiSD2.c
#) # Вторая Лаба
#add_executable(SkibidiToilet
#        ALA3/table.c
#        ALA3/AiSD3.c
#) # Третья Лаба A
#add_executable(SkibidiToilet
#        ALA3/hash_table.c
#        ALA3/AiSD3b.c
#) # Третья Лаба B
#add_executable(SkibidiToilet
#        ALA4/bitree.c
#        ALA4/AiSD4a.c
#        ALA4/ffile.h
#        ALA4/from_file.c
#) # Четвёртая Лаба A
add_executable(SkibidiToilet
        ALA5/graph.h
        ALA5/graph.c
        ALA5/AiSD5.c
        ALA5/queue_priority.c
        ALA5/queue_prority.h
        ALA5/from_file.c
        ALA5/from_file.h
) # Пятая лаба
#find_program(VALGRIND "valgrind")
#if(VALGRIND)
#    add_custom_target(valgrind COMMAND "${VALGRIND}" --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes)
#endif()