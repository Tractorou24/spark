find_package(SFML COMPONENTS system window graphics QUIET REQUIRED)
find_package(Vulkan QUIET REQUIRED)
find_package(glfw3 QUIET REQUIRED)
find_package(imgui QUIET REQUIRED)

set (TARGET_NAME ${SPARK_NAME}_core)
set (HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

spark_add_library(${TARGET_NAME}
    CXX_SOURCES
        ${SOURCE_DIR}/Application.cpp
        ${SOURCE_DIR}/Component.cpp
        ${SOURCE_DIR}/GameObject.cpp
        ${SOURCE_DIR}/Input.cpp
        ${SOURCE_DIR}/Registries.cpp
        ${SOURCE_DIR}/Scene.cpp
        ${SOURCE_DIR}/SceneManager.cpp
        ${SOURCE_DIR}/Window.cpp
    PUBLIC_HEADERS
        ${HEADER_DIR}/${SPARK_NAME}/core/Application.h
        ${HEADER_DIR}/${SPARK_NAME}/core/ApplicationBuilder.h
        ${HEADER_DIR}/${SPARK_NAME}/core/Component.h
        ${HEADER_DIR}/${SPARK_NAME}/core/EntryPoint.h
        ${HEADER_DIR}/${SPARK_NAME}/core/GameObject.h
        ${HEADER_DIR}/${SPARK_NAME}/core/Input.h
        ${HEADER_DIR}/${SPARK_NAME}/core/Registries.h
        ${HEADER_DIR}/${SPARK_NAME}/core/Renderer2D.h
        ${HEADER_DIR}/${SPARK_NAME}/core/Scene.h
        ${HEADER_DIR}/${SPARK_NAME}/core/SceneManager.h
        ${HEADER_DIR}/${SPARK_NAME}/core/Window.h

        ${HEADER_DIR}/${SPARK_NAME}/core/components/Circle.h
        ${HEADER_DIR}/${SPARK_NAME}/core/components/Collider.h
        ${HEADER_DIR}/${SPARK_NAME}/core/components/Image.h
        ${HEADER_DIR}/${SPARK_NAME}/core/components/Rectangle.h
        ${HEADER_DIR}/${SPARK_NAME}/core/components/Text.h
        ${HEADER_DIR}/${SPARK_NAME}/core/components/Transform.h

        ${HEADER_DIR}/${SPARK_NAME}/core/details/AbstractGameObject.h
        ${HEADER_DIR}/${SPARK_NAME}/core/details/SerializationSchemes.h
        ${HEADER_DIR}/${SPARK_NAME}/core/impl/AbstractGameObject.h
        ${HEADER_DIR}/${SPARK_NAME}/core/impl/ApplicationBuilder.h
        ${HEADER_DIR}/${SPARK_NAME}/core/impl/GameObject.h
        ${HEADER_DIR}/${SPARK_NAME}/core/impl/Renderer2D.h
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_base
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_events
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_imgui
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_lib
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_log
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_math
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_patterns
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_rtti
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_path
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_render
        ${CMAKE_PROJECT_NAME}::${SPARK_NAME}_render_vk
        experimental_ser
    PRIVATE
        glfw
        imgui::imgui
        sfml-system
        sfml-window
        sfml-graphics
        Vulkan::Vulkan
)
