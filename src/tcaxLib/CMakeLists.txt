add_library(tcaxLib SHARED
        std.h
        pix.h
        pix.cpp
        file.h
        file.cpp
        text.h
        text.cpp
        image.h
        image.cpp
        utility.h
        utility.cpp
        python/tcaxlib.cpp)

#boost
LIST(APPEND tcaxLib_includes ${Boost_INCLUDE_DIRS})
LIST(APPEND tcaxLib_libs ${Boost_PYTHON3_LIBRARY} ${Boost_LOCALE_LIBRARY})

#python
LIST(APPEND tcaxLib_includes ${Python_INCLUDE_DIRS})
LIST(APPEND tcaxLib_libs ${Python_LIBRARIES})

#freetype2
LIST(APPEND tcaxLib_includes ${FREETYPE_INCLUDE_DIRS})
LIST(APPEND tcaxLib_libs ${FREETYPE_LIBRARIES})

#libpng
find_package(PNG 1.6.36 REQUIRED)
LIST(APPEND tcaxLib_includes ${PNG_INCLUDE_DIRS})
LIST(APPEND tcaxLib_libs ${PNG_LIBRARIES})

target_link_libraries(tcaxLib ${tcaxLib_libs})
target_include_directories(tcaxLib PUBLIC ${tcaxLib_includes})
set_target_properties(tcaxLib PROPERTIES PREFIX "")