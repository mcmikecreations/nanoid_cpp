find_package(OpenSSL)
if (OPENSSL_FOUND)
    set(EXAMPLE_NAME example_openssl)
    add_executable(${EXAMPLE_NAME} openssl.cpp)
    # Add the include directories for compiling
    target_include_directories(${EXAMPLE_NAME} PUBLIC ${OPENSSL_INCLUDE_DIR})
    # Add the static lib for linking
    target_link_libraries(example_openssl OpenSSL::SSL OpenSSL::Crypto nanoid)
    message(STATUS "Found OpenSSL ${OPENSSL_VERSION}, building ${EXAMPLE_NAME}")
else()
    message(STATUS "OpenSSL Not Found, skipping ${EXAMPLE_NAME}")
endif()