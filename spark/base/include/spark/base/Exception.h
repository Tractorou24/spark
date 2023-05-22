#pragma once

#include "spark/base/Export.h"
#include "spark/base/Macros.h"
#include "spark/base/details/Exception.h"

#include "fmt/core.h"

#include <stdexcept>

/**
* \brief Macro used to define and implement a derived exception class.
* \param DerivedClass The name of the derived exception to create.
*/
#define SPARK_BASE_DEFINE_EXCEPTION(DerivedClass)                                                               \
    class SPARK_BASE_EXPORT DerivedClass : public spark::base::Exception                                        \
    {                                                                                                           \
    public:                                                                                                     \
        DerivedClass(const char* file_name, const int line_number, const char* additional_message)              \
            : base::Exception(file_name, line_number, additional_message, typeid(DerivedClass).name())   \
        {}                                                                                                      \
    };

/**
 * \brief The macro used to throw a spark exception.
 */
#define SPARK_BASE_THROW_EXCEPTION(...) SPARK_BASE_DETAILS_THROW_EXCEPTION_MACRO_CHOOSER(__VA_ARGS__)

namespace spark::base
{
    SPARK_WARNING_PUSH
    SPARK_DISABLE_MSVC_WARNING(4275) // non dll-interface class used as base for dll-interface class -> disabled since class is an stl class

    /**
     * \brief The base class for all exceptions in Spark. Should be used as a base class only.
     */
    class SPARK_BASE_EXPORT Exception : public std::runtime_error
    {
    public:
        explicit Exception(const char* file_name, const int line_number, const char* additional_message, const char* class_name)
            : std::runtime_error(FormatMessage(file_name, line_number, additional_message, class_name).c_str()) {}

        ~Exception() override = default;

        Exception(const Exception& other) = default;
        Exception(Exception&& other) noexcept = default;
        Exception& operator=(const Exception& other) = default;
        Exception& operator=(Exception&& other) noexcept = default;

    protected:
        static std::string FormatMessage(const char* file_name, const int line_number, const char* additional_message, const char* class_name)
        {
            return fmt::format("Exception of type {0} has been thrown.\nError happen in {1} at line number {2}.\nException contains the message:\n{3}",
                               class_name, file_name, line_number, additional_message);
        }
    };

    SPARK_WARNING_POP

    /**
     * \brief Exception thrown when an argument does not have the expected type.
     */
    SPARK_BASE_DEFINE_EXCEPTION(TypeMismatchException)

    /**
     * \brief Exception thrown when an argument is not valid.
     */
    SPARK_BASE_DEFINE_EXCEPTION(BadArgumentException)

    /**
     * \brief Exception thrown when a parameter already exist.
     */
    SPARK_BASE_DEFINE_EXCEPTION(DuplicatedParameterException)

    /**
     * \brief Exception thrown when a parameter is missing.
     */
    SPARK_BASE_DEFINE_EXCEPTION(CouldNotFindParameterException)

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
}

#undef SPARK_BASE_DEFINE_EXCEPTION
