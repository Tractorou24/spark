#pragma once

#include "spark/base/Macros.h"

SPARK_WARNING_PUSH
SPARK_DISABLE_MSVC_WARNING(4661) // no suitable definition provided for explicit template instantiation request

namespace spark::patterns
{
    /**
     * \brief A class used to implement the Singleton pattern.
     * \tparam T The type of the class to make a singleton.
     */
    template <typename T, typename Context = T>
    class Singleton
    {
    public:
        Singleton(const Singleton& other) = delete;
        Singleton(Singleton&& other) noexcept = default;
        Singleton& operator=(const Singleton& other) = delete;
        Singleton& operator=(Singleton&& other) noexcept = default;

        /**
         * \brief Gets the instance of the singleton.
         * \return A pointer to the instance of the singleton (class T).
         */
        static T* Instance();

    protected:
        Singleton() = default;
        virtual ~Singleton() = default;

    private:
        static T* s_instance;
    };
}

template <typename T, typename Context>
T* spark::patterns::Singleton<T, Context>::s_instance = nullptr;

#include "spark/patterns/impl/Singleton.h"

SPARK_WARNING_POP
