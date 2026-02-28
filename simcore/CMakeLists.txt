cmake_minimum_required(VERSION 3.20)

# Library name: simcore
set(SIMCORE_TARGET simcore)

file(GLOB_RECURSE SIMCORE_HEADERS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*/*.hpp"
)

file(GLOB_RECURSE SIMCORE_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Choose shared vs static for Unity plugin builds
if (SIMCORE_BUILD_SHARED)
  add_library(${SIMCORE_TARGET} SHARED ${SIMCORE_SOURCES} ${SIMCORE_HEADERS})
else()
  add_library(${SIMCORE_TARGET} STATIC ${SIMCORE_SOURCES} ${SIMCORE_HEADERS})
endif()

target_include_directories(${SIMCORE_TARGET}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(${SIMCORE_TARGET} PUBLIC cxx_std_20)

# Export symbols cleanly (esp. Windows)
if (WIN32 AND SIMCORE_BUILD_SHARED)
  target_compile_definitions(${SIMCORE_TARGET} PRIVATE SIMCORE_BUILD_DLL=1)
endif()

# Optional: make the output name consistent across platforms for Unity
set_target_properties(${SIMCORE_TARGET} PROPERTIES
  OUTPUT_NAME "simcore"
)

# If you have a C API wrapper for Unity, keep it in e.g. src/c_api.cpp
# and ensure functions are marked extern "C" + export macro.