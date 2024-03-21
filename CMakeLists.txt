cmake_minimum_required(VERSION 3.23)
project(cs6120)

if (NOT PROJECT_IS_TOP_LEVEL)
    message(FATAL_ERROR "cs6120 can only be built as the top-level project")
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================================================
# Add third-party dependencies to thd build.
# =========================================================

# We want Boost.Intrusive for an intrusive list implementation.
# An older boost version might also work, not tested.
find_package(Boost 1.84 REQUIRED)

# =========================================================
# Add cs6120 project targets.
# =========================================================

add_subdirectory(bril)
