cmake_minimum_required(VERSION 3.17)
project(YSBD C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_EXE_LINKER_FLAGS "-static")
link_directories(/home/Desktop/desktop/YSBD/BF_lib/lib)

add_executable(YSBD
        examples/main_HP.c
        include/BF.h
        src/HP.c include/HP.h
        include/Record.h
        src/HpList.c include/HpList.h src/utilities.c include/utilities.h include/HP_info.h src/HT.c)

add_executable(HT
        examples/main_HT.c include/HT_info.h
        include/NOT.h NOT.c src/HtList.c include/HtList.h)

target_link_libraries (YSBD LINK_PUBLIC BF_64.a)