#pragma once

#include "spark/base/Export.h"
#include "spark/base/Macros.h"
#include "spark/base/details/Exception.h"

#include <source_location>

/**
* \brief Macro used to define and implement a derived exception class.
* \param DerivedClass The name of the derived exception to create.
*/
#define SPARK_BASE_DEFINE_EXCEPTION(DerivedClass)                                                                                       \
    class SPARK_BASE_EXPORT DerivedClass : public spark::base::details::Exception                                                       \
    {                                                                                                                                   \
    public:                                                                                                                             \
        DerivedClass(const std::string_view& message, const std::source_location& source_location = std::source_location::current())    \
            : base::details::Exception(#DerivedClass, message, source_location)                                                         \
        {}                                                                                                                              \
    };

namespace spark::base
{
    /**
     * \brief Exception thrown when an argument does not have the expected type.
     */
    SPARK_BASE_DEFINE_EXCEPTION(TypeMismatchException)

    /**
     * \brief Exception thrown when an argument is not valid.
     */
    SPARK_BASE_DEFINE_EXCEPTION(BadArgumentException)

    /**
     * \brief Exception thrown when a functionality which is not implemented yet is called.
     */
    SPARK_BASE_DEFINE_EXCEPTION(NotImplementedException)

    /**
     * \brief Exception thrown when something strange happened and we have no idea of why this is happening. This is an unexpected exception which
     *        means there is a software failure somewhere.
     */
    SPARK_BASE_DEFINE_EXCEPTION(UnknownException)

    /**
     * \brief Exception thrown when an I/O operation on a file could not be achieved.
     */
    SPARK_BASE_DEFINE_EXCEPTION(FileIOException)

    /**
     * \brief Exception thrown when trying to open a file that could not be opened. This can occurs on several situations:
     *    - The file does not exists when trying to open a file for reading.
     *    - The file is locked by another process.
     */
    SPARK_BASE_DEFINE_EXCEPTION(CouldNotOpenFileException)

    /**
     * \brief Exception thrown on failure when trying to obtain a lock on a file. This can occurs on several situations:
     *    - Another process is locking the file.
     */
    SPARK_BASE_DEFINE_EXCEPTION(CouldNotObtainFileLockException)

    /**
     * \brief Exception thrown when a pointer is null and should't be.
     */
    SPARK_BASE_DEFINE_EXCEPTION(NullPointerException)

    /**
     * \brief Exception thrown when trying to manipulate an uninitialized socket.
     */
    SPARK_BASE_DEFINE_EXCEPTION(UninitializedSocketException)

    /**
     * \brief Exception thrown when trying to serialize an object when the serializer is not in writing mode or vice versa.
     */
    SPARK_BASE_DEFINE_EXCEPTION(WrongSerializerMode)

    /**
     * \brief Exception thrown when trying to read/write past the end of a buffer.
     */
    SPARK_BASE_DEFINE_EXCEPTION(OverflowException)

    /**
     * \brief Exception thrown when trying to load a file with an unsupported format.
     */
    SPARK_BASE_DEFINE_EXCEPTION(UnsupportedFileFormatException)

    /**
     * \brief Exception thrown when trying to create multiple instances of an application, which is not allowed.
     */
    SPARK_BASE_DEFINE_EXCEPTION(DuplicatedApplicationException)

    /**
     * \brief Exception thrown when starting a command buffer recording while it is already recording.
     */
    SPARK_BASE_DEFINE_EXCEPTION(CommandBufferAlreadyRecordingException)

    /**
     * \brief Exception thrown when an argument is out of range.
     */
    SPARK_BASE_DEFINE_EXCEPTION(ArgumentOutOfRangeException)

    /**
     * \brief Exception thrown when calling a function which requires a render pass to be ended while it is not.
     */
    SPARK_BASE_DEFINE_EXCEPTION(RenderPassAlreadyStartedException)

    /**
     * \brief Exception thrown when calling a function which requires a render pass to be started while it is not.
     */
    SPARK_BASE_DEFINE_EXCEPTION(RenderPassNotStartedException)

    /**
     * \brief Exception thrown when a descriptor does not have the expected type.
     */
    SPARK_BASE_DEFINE_EXCEPTION(InvalidDescriptorTypeException)

    /**
     * \brief Exception thrown when failing to create a shader.
     */
    SPARK_BASE_DEFINE_EXCEPTION(CouldNotCreateShaderException)

    /**
     * \brief Exception thrown when failing to initialize a window.
     */
    SPARK_BASE_DEFINE_EXCEPTION(BadWindowInitializationException)
}

#undef SPARK_BASE_DEFINE_EXCEPTION
