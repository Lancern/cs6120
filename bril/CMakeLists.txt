set(bril_headers
    include/bril/CFG.h
    include/bril/Inst.h
    include/bril/Literal.h
    include/bril/Type.h
    include/bril/Value.h
)
set(bril_sources
    src/CFG.cpp
    src/Inst.cpp
    src/Type.cpp
    src/Value.cpp
    src/support/Check.cpp
    src/support/Check.h
)

add_library(bril STATIC)
target_include_directories(bril PUBLIC include)
target_sources(bril
    PUBLIC FILE_SET HEADERS FILES ${bril_headers}
    PRIVATE ${bril_sources}
)
target_link_libraries(bril PUBLIC Boost::boost)
