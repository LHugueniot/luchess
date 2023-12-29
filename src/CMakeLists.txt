
set(LUCHESSCORE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR})
set(LUCHESSCORE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/luchess/core)

add_library(
    LuChessCore

    STATIC
    ${LUCHESSCORE_SRC}/board.cpp
    ${LUCHESSCORE_SRC}/util.cpp
    ${LUCHESSCORE_SRC}/chess.cpp
    ${LUCHESSCORE_SRC}/notation.cpp
)

target_include_directories(
    LuChessCore

    PUBLIC
    ${LUCHESSCORE_INCLUDE}
)
