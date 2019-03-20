add_library(tcaxLib SHARED
        common.h
        pix.cpp
        pix.h
        python/tcaxLib.cpp)

#boost
find_package(Boost 1.69.0 REQUIRED COMPONENTS python3 locale)
LIST(APPEND tcaxLib_includes ${Boost_INCLUDE_DIRS})
LIST(APPEND tcaxLib_libs ${Boost_PYTHON3_LIBRARY} ${Boost_LOCALE_LIBRARY})

#python
find_package(Python 3.7 REQUIRED COMPONENTS Development)
LIST(APPEND tcaxLib_includes ${Python_INCLUDE_DIRS})
LIST(APPEND tcaxLib_libs ${Python_LIBRARIES})

target_link_libraries(tcaxLib ${tcaxLib_libs})
target_include_directories(tcaxLib PUBLIC ${tcaxLib_includes})
set_target_properties(tcaxLib PROPERTIES PREFIX "")