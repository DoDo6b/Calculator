cmake_minimum_required(VERSION 3.5)

project(FALT_Calculator)

find_library(PTHREAD_LIBRARY pthread)
find_library(PCRE_LIBRARY pcre)

include_directories(include)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable(main main.cpp lib/token.cpp lib/token.h lib/variables.cpp lib/variables.h lib/grammary.cpp lib/grammary.h)

target_link_libraries(main ${PTHREAD_LIBRARY} ${PCRE_LIBRARY})

