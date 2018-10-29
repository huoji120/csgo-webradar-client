add_executable(WebRadar
    main.cpp
    features/fbase.cpp
    features/fradar.cpp
    json/jsoncpp.cpp
    process/process.cpp
    sdk/centitylist.cpp
    engine.cpp
    offsets.cpp
)

target_link_libraries(WebRadar
    Threads::Threads
    ${LIBCURL_LIBRARIES}
)

target_include_directories(WebRadar PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)
