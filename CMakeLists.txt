cmake_minimum_required(VERSION 3.12)

project(bulk)

#option(WITH_BOOST_TEST "Whether to build Boost test" ON)
# Опция для включения Doxygen
option(WITH_DOXYGEN "Build Doxygen documentation" OFF)

add_executable(bulk main.cpp bulk.cpp)

set_target_properties(bulk PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)


# Тест (если файл test.cpp существует)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp)
    # Ищем установленный gtest
    find_package(GTest REQUIRED)
    
    # Тестовый исполняемый файл
    add_executable(bulk_tests test.cpp bulk.cpp)
    
    # Линкуем с системным gtest
    target_link_libraries(bulk_tests PRIVATE GTest::gtest GTest::gtest_main)
    
    # Тесты
    enable_testing()
    add_test(NAME AllTests COMMAND bulk_tests)
else()
    message(WARNING "test.cpp not found, tests will not be built")
endif()


if (MSVC)
    target_compile_options(bulk PRIVATE
        /W4
    )
else ()
    target_compile_options(bulk PRIVATE
        -Wall -Wextra -pedantic -Werror -Wunused -Wunused-variable -Wunused-parameter
    )
endif()

target_compile_definitions(bulk PRIVATE 
    $<$<CONFIG:Debug>:ENABLE_DEBUG_LOGGING>
    $<$<CONFIG:RelWithDebInfo>:ENABLE_DEBUG_LOGGING>
)


install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_DEBIAN_FILE_NAME "bulk.deb")
set(CPACK_PACKAGE_CONTACT example@example.com)


include(CPack)

