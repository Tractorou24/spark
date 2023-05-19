#pragma once

namespace spark::patterns
{
    template <typename T, typename Context>
    T* Singleton<T, Context>::Instance()
    {
        static bool init = []
        {
            if constexpr (std::is_default_constructible_v<T>)
                s_instance = new T();
            else
                s_instance = Context::InitSingleton();
            return true;
        }();
        SPARK_UNUSED(init);

        return s_instance;
    }
}
