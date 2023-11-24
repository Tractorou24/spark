#include "spark/patterns/Factory.h"

#include "gtest/gtest.h"

namespace spark::patterns::testing
{
    class A
    {
    public:
        explicit A(const int) {}
        virtual ~A() = default;

        A(const A& other) = default;
        A(A&& other) noexcept = default;
        A& operator=(const A& other) = default;
        A& operator=(A&& other) noexcept = default;
    };

    class B final : public A
    {
    public:
        explicit B(const int nb) : A(nb) {}
    };

    class C : public A
    {
    public:
        explicit C(int&) : A(2) {}
    };

    class D final : public C
    {
    public:
        explicit D(int& nb) : C(nb) { nb--; }
    };

    TEST(FactoryShould, createARegisteredTypeWithCreate)
    {
        // Given a factory with a given registered type
        spark::patterns::Factory<std::string, A, int> factory;
        factory.registerType<B>("B");

        // When creating this type
        auto ptr = factory.create("B", 2);

        // Then, the created pointer is not null and is of the correct type
        EXPECT_NE(ptr, nullptr);
        EXPECT_EQ(typeid(decltype(ptr)), typeid(std::unique_ptr<A>));
        EXPECT_NO_THROW(std::ignore = dynamic_cast<B*>(ptr.get()));
    }

    TEST(FactoryShould, createARegisteredTypeWithCreateOrFail)
    {
        // Given a factory with a given registered type
        spark::patterns::Factory<std::string, A, int> factory;
        factory.registerType<B>("B");

        // When creating this type
        auto ptr = factory.createOrFail("B", 2);

        // Then, the created pointer is not null and is of the correct type
        EXPECT_NE(ptr, nullptr);
        EXPECT_EQ(typeid(decltype(ptr)), typeid(std::unique_ptr<A>));
        EXPECT_NO_THROW(std::ignore = dynamic_cast<B*>(ptr.get()));
    }

    TEST(FactoryShould, perfectlyForwardArgumentsToConstructor)
    {
        // Given a factory with a given registered type
        spark::patterns::Factory<std::string, C, int&> factory;
        factory.registerType<D>("D");

        // When calling this type with an argument that should be modified
        int value = 2;
        const auto ptr = factory.create("D", value);

        // Then, the value is correctly modified by the object
        EXPECT_EQ(value, 1);
    }

    TEST(FactoryShould, failWhenCreatingUnregisteredType)
    {
        // Given a factory without any registered types
        const spark::patterns::Factory<std::string, A, int> factory;

        // When creating a non registered type, the pointer is null or an exception is thrown depending on the create method
        EXPECT_EQ(factory.createOrFail("not_registered", 2), nullptr);
        EXPECT_THROW(SPARK_UNUSED(factory.create("not_registered", 2)), spark::base::BadArgumentException);
    }

    TEST(FactoryShould, throwWhenRegisteringTheSameTypeMultipleTimes)
    {
        // Given a factory with a single registered type
        spark::patterns::Factory<std::string, A, int> factory;
        factory.registerType<B>("B");

        // When re registering the same type, an exception is thrown
        EXPECT_THROW(factory.registerType<B>("B"), spark::base::BadArgumentException);
    }

    TEST(FactoryShould, returnTheRegisteredTypes)
    {
        // Given a factory with two registered types
        spark::patterns::Factory<std::string, A, int> factory;
        factory.registerType<A>("A");
        factory.registerType<B>("B");

        // When retrieving the registered types
        const auto registered_types = factory.registeredTypes();

        // Then, all registered types are in the vector
        EXPECT_EQ(registered_types.size(), 2);
        EXPECT_TRUE(std::ranges::find(registered_types, "A") != registered_types.end());
        EXPECT_TRUE(std::ranges::find(registered_types, "B") != registered_types.end());
    }
}
