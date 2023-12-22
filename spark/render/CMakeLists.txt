set (TARGET_NAME ${SPARK_NAME}_render)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME}
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/render/Buffer.h
        ${HEADER_DIR}/${SPARK_NAME}/render/CommandBuffer.h
        ${HEADER_DIR}/${SPARK_NAME}/render/CommandQueue.h
        ${HEADER_DIR}/${SPARK_NAME}/render/DescriptorBinding.h
        ${HEADER_DIR}/${SPARK_NAME}/render/DescriptorSet.h
        ${HEADER_DIR}/${SPARK_NAME}/render/DeviceMemory.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Format.h
        ${HEADER_DIR}/${SPARK_NAME}/render/FrameBuffer.h
        ${HEADER_DIR}/${SPARK_NAME}/render/GraphicsAdapter.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Image.h
        ${HEADER_DIR}/${SPARK_NAME}/render/IndexBuffer.h
        ${HEADER_DIR}/${SPARK_NAME}/render/InputAssembler.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Mappable.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Pipeline.h
        ${HEADER_DIR}/${SPARK_NAME}/render/PushConstantsLayout.h
        ${HEADER_DIR}/${SPARK_NAME}/render/PushConstantsRange.h
        ${HEADER_DIR}/${SPARK_NAME}/render/RenderTarget.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Resource.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Sampler.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Scissor.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Shader.h
        ${HEADER_DIR}/${SPARK_NAME}/render/ShaderStages.h
        ${HEADER_DIR}/${SPARK_NAME}/render/StateResource.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Surface.h
        ${HEADER_DIR}/${SPARK_NAME}/render/SwapChain.h
        ${HEADER_DIR}/${SPARK_NAME}/render/VertexBuffer.h
        ${HEADER_DIR}/${SPARK_NAME}/render/Viewport.h

        ${HEADER_DIR}/${SPARK_NAME}/render/impl/Resource.h
    CXX_SOURCES
        ${SOURCE_DIR}/Buffer.cpp
        ${SOURCE_DIR}/ExportSymbols.cpp
        ${SOURCE_DIR}/RenderTarget.cpp
        ${SOURCE_DIR}/StateResource.cpp
        ${SOURCE_DIR}/Scissor.cpp
        ${SOURCE_DIR}/Viewport.cpp
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_lib
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_math
)
