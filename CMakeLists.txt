cmake_minimum_required(VERSION 3.10)
project(FastTransformersUtils VERSION 1.0.0)

# Set C standard
set(CMAKE_C_STANDARD 11)

# Include directory for header files
include_directories(include)

# Dynamically find all source files in the src directory
file(GLOB SOURCES "src/*.c")

# Dynamically find all header files in the include directory
file(GLOB HEADERS "include/*.h")

# Add KISS FFT source files
set(KISSFFT_SRCS kiss/kiss_fft.c  kiss/kiss_fftr.c)

# Create shared library
add_library(fast_transformers_utils SHARED ${SOURCES} ${KISSFFT_SRCS})

# Conditional linking for math library
if (UNIX)
    target_link_libraries(fast_transformers_utils m)
endif()

# Set library version
set_target_properties(fast_transformers_utils PROPERTIES VERSION ${PROJECT_VERSION})

# Set 
if (WIN32)
    target_compile_definitions(fast_transformers_utils PRIVATE FAST_TRANSFORMERS_UTILS_EXPORTS)
endif()

# Install library
install(TARGETS fast_transformers_utils DESTINATION lib)

# Install header file
install(FILES include/fast_transformers_utils.h DESTINATION include)

# Configure CPack
set(CPACK_PACKAGE_NAME "fast_transformers_utils")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "kyrianobikwelu@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Fast Utilities for TransformersPHP")
set(CPACK_PACKAGE_VENDOR "CodeWithKyrian")
set(CPACK_PACKAGE_LICENSE "MIT")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://codewithkyrian.com")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

# Specify package output directory
set(CPACK_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/releases")

# Define the package type(s) you want to create
set(CPACK_GENERATOR "TGZ")

# Include CPack
include(CPack)