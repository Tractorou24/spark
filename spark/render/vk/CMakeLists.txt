find_package(Vulkan QUIET REQUIRED)

set (TARGET_NAME ${SPARK_NAME}_render_vk)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME}
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/Conversions.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/Helpers.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanGraphicsAdapter.h
    CXX_SOURCES
        ${SOURCE_DIR}/VulkanGraphcisAdapter.cpp
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_log
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_render
    PRIVATE
        Vulkan::Vulkan
)
