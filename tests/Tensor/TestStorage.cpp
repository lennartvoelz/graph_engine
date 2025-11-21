#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <GraphEngine/Tensor/Storage.hpp>
#include <vector>

using namespace GraphEngine;
namespace gTest = ::testing;
using gTest::NotNull;
using StorageTypes = gTest::Types<float, double, int>;  // Typed Tests
TYPED_TEST_SUITE(TestStorage, StorageTypes);

template <typename T>
class TestStorage : public gTest::Test {
public:
	using size_type = std::size_t;

protected:
	auto SetUp() -> void override {
		small_ = 1e2;
		large_ = 1e6;
	}

	size_type small_;
	size_type large_;
};

// === Basic Tests ===

TYPED_TEST(TestStorage, ConstructorAllocatesSmall) {
	const std::size_t size = this->small_;
	Storage<TypeParam> storage(size);
	EXPECT_EQ(storage.size(), size);
	EXPECT_THAT(storage.data(), NotNull());
}

TYPED_TEST(TestStorage, AllocatesLargeDataModification) {
	const std::size_t size = this->large_;
	Storage<TypeParam> storage(size);
	// Check that data modification works
	storage.data()[10'000] = 42;
	EXPECT_EQ(storage.size(), size);
	EXPECT_THAT(storage.data(), NotNull());
	EXPECT_EQ(storage.data()[10'000], TypeParam{42});
}

TYPED_TEST(TestStorage, DataPointerIsValid) {
	const std::size_t size = this->small_;
	Storage<TypeParam> storage(size);
	std::vector<TypeParam> vector(size, 42);
	auto* dataPtr = storage.data();
	std::copy(vector.begin(), vector.end(), dataPtr);
	EXPECT_EQ(dataPtr[0], 42);
	EXPECT_EQ(dataPtr[10], 42);
}

TYPED_TEST(TestStorage, SizeZeroCreatesEmtpyStorage) {
	const std::size_t size = 0;
	Storage<TypeParam> storage(size);
	EXPECT_EQ(storage.size(), size);
}

// === Shared Flag Tests ===

TYPED_TEST(TestStorage, IsSharedReturnsFalseByDefault) {
	const std::size_t size = this->small_;
	Storage<TypeParam> storage(size);
	EXPECT_FALSE(storage.is_shared());
}