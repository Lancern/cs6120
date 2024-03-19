cmake_minimum_required(VERSION 3.23)
project(cs6120)

if (NOT PROJECT_IS_TOP_LEVEL)
    message(FATAL_ERROR "cs6120 can only be built as the top-level project")
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================================================
# Add cs6120 project targets.
# =========================================================

add_subdirectory(bril)
