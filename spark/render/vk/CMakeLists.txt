find_package(Vulkan QUIET REQUIRED)
find_package(VulkanMemoryAllocator QUIET REQUIRED)
find_package(unofficial-spirv-reflect QUIET REQUIRED)

set (TARGET_NAME ${SPARK_NAME}_render_vk)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME}
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/Conversions.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/Formatters.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/Helpers.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanBackend.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanBuffer.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanGraphicsAdapter.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanCommandBuffer.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanDescriptorLayout.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanDescriptorSet.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanDescriptorSetLayout.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanDevice.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanFactory.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanFrameBuffer.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanImage.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanIndexBuffer.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanIndexBufferLayout.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanInputAssembler.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanInputAttachmentMapping.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanPipeline.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanPipelineLayout.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanPushConstantsLayout.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanPushConstantsRange.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanQueue.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanRasterizer.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanRenderPass.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanRenderPipeline.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanSampler.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanSurface.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanShaderModule.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanShaderProgram.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanSwapChain.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanVertexBuffer.h
        ${HEADER_DIR}/${SPARK_NAME}/render/vk/VulkanVertexBufferLayout.h
    CXX_SOURCES
        ${SOURCE_DIR}/VulkanBackend.cpp
        ${SOURCE_DIR}/VulkanBuffer.cpp
        ${SOURCE_DIR}/VulkanCommandBuffer.cpp
        ${SOURCE_DIR}/VulkanDescriptorLayout.cpp
        ${SOURCE_DIR}/VulkanDescriptorSet.cpp
        ${SOURCE_DIR}/VulkanDescriptorSetLayout.cpp
        ${SOURCE_DIR}/VulkanDevice.cpp
        ${SOURCE_DIR}/VulkanFactory.cpp
        ${SOURCE_DIR}/VulkanFrameBuffer.cpp
        ${SOURCE_DIR}/VulkanGraphicsAdapter.cpp
        ${SOURCE_DIR}/VulkanImage.cpp
        ${SOURCE_DIR}/VulkanIndexBuffer.cpp
        ${SOURCE_DIR}/VulkanIndexBufferLayout.cpp
        ${SOURCE_DIR}/VulkanInputAssembler.cpp
        ${SOURCE_DIR}/VulkanInputAttachmentMapping.cpp
        ${SOURCE_DIR}/VulkanPipeline.cpp
        ${SOURCE_DIR}/VulkanPipelineLayout.cpp
        ${SOURCE_DIR}/VulkanPushConstantsLayout.cpp
        ${SOURCE_DIR}/VulkanPushConstantsRange.cpp
        ${SOURCE_DIR}/VulkanQueue.cpp
        ${SOURCE_DIR}/VulkanRasterizer.cpp
        ${SOURCE_DIR}/VulkanRenderPass.cpp
        ${SOURCE_DIR}/VulkanRenderPipeline.cpp
        ${SOURCE_DIR}/VulkanSampler.cpp
        ${SOURCE_DIR}/VulkanSurface.cpp
        ${SOURCE_DIR}/VulkanShaderModule.cpp
        ${SOURCE_DIR}/VulkanShaderProgram.cpp
        ${SOURCE_DIR}/VulkanSwapChain.cpp
        ${SOURCE_DIR}/VulkanVertexBuffer.cpp
        ${SOURCE_DIR}/VulkanVertexBufferLayout.cpp
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_log
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_render
    PRIVATE
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_patterns
        unofficial::spirv-reflect
        Vulkan::Vulkan
        GPUOpen::VulkanMemoryAllocator
)
