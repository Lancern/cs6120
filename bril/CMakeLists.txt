set(bril_headers
    include/bril/Type.h
)
set(bril_sources
    src/Type.cpp
    src/support/Check.cpp
    src/support/Check.h
)

add_library(bril STATIC)
target_include_directories(bril PUBLIC include)
target_sources(bril
    PUBLIC FILE_SET HEADERS FILES ${bril_headers}
    PRIVATE ${bril_sources}
)
