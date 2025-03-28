#pragma once

#include "spark/patterns/details/Connection.h"

#include <functional>
#include <map>

#include <cstddef>

namespace spark::patterns
{
    /**
     * \brief A signal is a class used to emit events.
     * \tparam Args The types of the arguments emitted in the event.
     */
    template <typename... Args>
    class Signal final
    {
    public:
        explicit Signal();
        ~Signal();

        Signal(const Signal& signal) = delete;
        Signal(Signal&& signal) noexcept;
        Signal& operator=(const Signal& signal) = delete;
        Signal& operator=(Signal&& signal) noexcept;

        /**
         * \brief Connects a slot to the signal.
         * \param slot The address of the slot to connect.
         * \return The key of the slot.
         */
        std::size_t connect(Slot<Args...>* slot);

        /**
         * \brief Connects a slot to the signal.
         * \param slot A reference to the slot to connect.
         * \return The key of the slot.
         */
        std::size_t connect(Slot<Args...>& slot);

        /**
         * \brief Connects a slot to the signal.
         * \param slot The slot to connect.
         * \return The key of the slot.
         */
        std::size_t connect(Slot<Args...>&& slot);

        /**
         * \brief Connects a callback to the signal. This will create an internal slot. Avoids the need to create an slot in place.
         * \param callback A callback to connect.
         * \return The key of the slot.
         */
        std::size_t connect(const std::function<void(Args...)>& callback);

        /**
         * \brief Connects a callback to the signal. This will create an internal slot. Avoids the need to create an slot in place.
         * \param callback A callback to connect.
         * \return The key of the slot.
         */
        std::size_t connect(std::function<void(Args...)>&& callback);

        /**
         * \brief Disconnects a slot from the signal.
         * \param key The key of the slot to disconnect.
         */
        void disconnect(std::size_t key);

        /**
         * \brief Disconnects a slot from the signal.
         * \param slot The address of the slot to disconnect.
         */
        void disconnect(Slot<Args...>* slot);

        /**
         * \brief Disconnects a slot from the signal.
         * \param slot A reference to the slot to disconnect.
         */
        void disconnect(Slot<Args...>& slot);

        /**
         * \brief Disconnects all connected slots from the signal.
         */
        void clear();

        /**
         * \brief Finds if a slot is connected to the signal.
         * \param key The key of the slot to find.
         * \return True if the slot is connected, false otherwise.
         */
        [[nodiscard]] bool isConnected(std::size_t key) const;

        /**
         * \brief Gets all the keys of the connected slots.
         * \return A \ref std::vector containing the keys of the connected slots.
         */
        [[nodiscard]] std::vector<std::size_t> connectedKeys() const;

        /**
         * \brief Gets all the connected slots.
         * \return A \ref std::vector containing pointers to the connected slots.
         */
        [[nodiscard]] std::vector<const Slot<Args...>*> connectedSlots() const;

        /**
         * \brief Emits the signal to all connected slots.
         * \tparam FnArgs The types of the arguments to emit. Must be convertible to the signal arguments.
         * \param args The arguments for the slots.
         */
        template <typename... FnArgs>
        void emit(FnArgs&&... args) const;

        /**
         * \brief Emits the signal to all connected slots. Same as \ref emit.
         * \param args The arguments for the slots.
         */
        void operator()(Args&&... args) const;

    private:
        /**
         * \brief A connect wrapper used by all public connect methods. Avoids code duplication.
         * \param slot The address of the slot to connect.
         * \param managed A boolean indicating if the slot is managed by the signal.
         * \return The key of the slot.
         */
        std::size_t connect(Slot<Args...>* slot, bool managed);

        /**
         * \brief Moves the connections from another signal to this signal. The other signal will be empty after this operation.
         * \param signal The signal to move the connections from.
         */
        void move(Signal* signal);

    private:
        std::map<std::size_t, details::Connection<Args...>> m_connections;
        std::size_t m_sequence = 0;
    };
}

#include "spark/patterns/impl/Signal.h"
