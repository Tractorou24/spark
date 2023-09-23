#pragma once

#include "spark/patterns/details/Connection.h"

#include <functional>

namespace spark::patterns
{
    template <typename... Args>
    class Signal;

    /**
     * \brief A slot is a connection between a signal and a callback.
     * \tparam Args The types of the arguments of the callback.
     */
    template <typename... Args>
    class Slot final
    {
        friend class details::Connection<Args...>;
        friend class Signal<Args...>;

    public:
        explicit Slot();

        /**
         * \brief Instantiates a slot with a callback.
         * \param f The callback as a function object.
         */
        explicit Slot(const std::function<void(Args...)>& f);

        /**
         * \brief Instantiates a slot with a callback.
         * \param f The callback as a function object.
         */
        explicit Slot(std::function<void(Args...)>&& f);

        /**
         * \brief Instantiates a slot from a non const method pointer.
         * \tparam T The class containing the method.
         * \param target The instance of the class containing the method.
         * \param method The address of the method to use as callback.
         */
        template <class T>
        Slot(T* target, void (T::*method)(Args...));

        /**
         * \brief Instantiates a slot from a const method pointer.
         * \tparam T The class containing the method.
         * \param target The instance of the class containing the method.
         * \param method The address of the method to use as callback.
         */
        template <class T>
        Slot(T* target, void (T::*method)(Args...) const);

        ~Slot();

        Slot(const Slot& slot);
        Slot(Slot&& slot) noexcept;
        Slot& operator=(const Slot& slot);
        Slot& operator=(Slot&& slot) noexcept;

        /**
         * \brief Checks if this slot is connected to a signal.
         * \return True if this slot is connected to a signal, false otherwise.
         */
        [[nodiscard]] bool isConnected() const;

        /**
         * \brief Disconnects this slot from the signal.
         */
        void disconnect();

    private:
        /**
         * \brief Moves the data from the given slot to this slot.
         * \param slot The slot to take data from.
         */
        void move(Slot* slot);

    private:
        std::function<void(Args...)> m_callback;
        details::Connection<Args...>* m_connection = nullptr;
    };
}

#include "spark/patterns/impl/Slot.h"
