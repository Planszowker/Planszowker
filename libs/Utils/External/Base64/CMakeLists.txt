set(LIB_NAME Base64)

add_library(${LIB_NAME} INTERFACE base64.hpp)

target_include_directories(${LIB_NAME}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )