if(USE_PRECOMPILED_HEADERS AND COMMAND target_precompile_headers)
	file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/precompiled_hyperhdr_headers.cpp "void precompiledHeadersHyperHDR(){};")
	add_library(precompiled_hyperhdr_headers ${CMAKE_CURRENT_BINARY_DIR}/precompiled_hyperhdr_headers.cpp)
	target_link_libraries(precompiled_hyperhdr_headers Qt${Qt_VERSION}::Gui Qt${Qt_VERSION}::Network)

    target_precompile_headers(precompiled_hyperhdr_headers PRIVATE ${CMAKE_SOURCE_DIR}/include/base/AllHeaders_pch.h)
endif()

# Define the current source locations
SET(CURRENT_HEADER_DIR ${CMAKE_SOURCE_DIR}/include)
SET(CURRENT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/sources)

add_subdirectory(base)
add_subdirectory(commandline)
add_subdirectory(blackborder)
add_subdirectory(jsonserver)
add_subdirectory(flatbufserver)
if (ENABLE_PROTOBUF)
	add_subdirectory(proto-nano-server)
endif()
add_subdirectory(ssdp)
if(ENABLE_BOBLIGHT)
	add_subdirectory(boblightserver)
endif()
add_subdirectory(leddevice)
add_subdirectory(utils)
add_subdirectory(effectengine)
add_subdirectory(grabber)
add_subdirectory(webserver)
add_subdirectory(db)
add_subdirectory(api)

if (ENABLE_CEC)
	add_subdirectory(cec)
endif()

if(ENABLE_BONJOUR)
	add_subdirectory(bonjour)
endif()

if(ENABLE_MQTT)
	add_subdirectory(mqtt)
endif()

# Executables
add_subdirectory(hyperhdr)
if (NOT APPLE)
	add_subdirectory(hyperhdr-remote)
endif()


