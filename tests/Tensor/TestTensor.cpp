#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <GraphEngine/Tensor/Tensor.hpp>
#include <numeric>

namespace gTest = ::testing;
using gTest::NotNull;
using StorageTypes = gTest::Types<float, double, int>;  // Typed Tests
TYPED_TEST_SUITE(TestTensor, StorageTypes);

template <typename T>
class TestTensor : public gTest::Test {
public:
	using size_type = std::size_t;

protected:
	auto SetUp() -> void override {
		smallShape = {10, 10};
		largeShape = {1000, 1000};

		small_size = compute_total_size(smallShape);
		large_size = compute_total_size(largeShape);

		smallStorage = std::make_shared<GraphEngine::Storage<T>>(small_size);
		largeStorage = std::make_shared<GraphEngine::Storage<T>>(large_size);
	}

	auto compute_total_size(const std::vector<size_type>& shape) -> size_type {
		return std::accumulate(shape.begin(), shape.end(), size_type{1}, std::multiplies<>());
	}

	size_type small_size;
	size_type large_size;
	std::vector<size_type> smallShape;
	std::vector<size_type> largeShape;

	std::shared_ptr<GraphEngine::Storage<T>> smallStorage;
	std::shared_ptr<GraphEngine::Storage<T>> largeStorage;
};

// === Basic Tests ===

TYPED_TEST(TestTensor, ConstructorAllocatesStorage) {
	const auto shape = this->smallShape;

	GraphEngine::Tensor<TypeParam> tensor{shape};

	EXPECT_THAT(tensor.data(), NotNull());
}

TYPED_TEST(TestTensor, sShapeIsConstructedCorrectly) {
	const auto shape = this->smallShape;

	GraphEngine::Tensor<TypeParam> tensor{shape};
	auto tensorShape = tensor.extents();

	EXPECT_EQ(tensorShape, shape);
}

// === View Tests ===

using TestTensorInt = TestTensor<int>;
TEST_F(TestTensorInt, ReadViaView) {
	const auto shape = this->largeShape;
	const auto size = this->large_size;

	GraphEngine::Tensor<int> tensor{shape};
	// Initialize and fill vector, copy data to the tensors storage
	std::vector<int> buffer(size);
	std::iota(buffer.begin(), buffer.end(), 0);
	std::copy(buffer.begin(), buffer.end(), tensor.data());
	// Create view of the tensor
	auto view = tensor.view<2>();
	auto value_two_zero = view[2, 0];
	auto value_five_ten = view[5, 10];

	EXPECT_EQ(value_two_zero, 2000);
	EXPECT_EQ(value_five_ten, 5010);
}

TEST_F(TestTensorInt, WriteViaView) {
	const auto shape = this->largeShape;
	const auto size = this->large_size;

	GraphEngine::Tensor<int> tensor{shape};
	auto view = tensor.view<2>();
	// Write to element [999, 999]
	view[999, 999] = 42;
	// Get element in flat buffer
	auto element = *(tensor.data() + size - 1);

	EXPECT_EQ(element, 42);
}

TEST_F(TestTensorInt, MultipleViewsOnOneTensor) {
	const auto shape = this->largeShape;

	GraphEngine::Tensor<int> tensor{shape};
	auto viewRightLayout = tensor.view<2>();
	auto viewLeftLayout = tensor.view<2, std::layout_left>();

	// Write to [0, 100] and verify on other view on [100, 0]
	viewRightLayout[0, 100] = 42;
	auto element = viewLeftLayout[100, 0];

	EXPECT_EQ(element, 42);
}

// === Non-Allocating Constructor Tests

TYPED_TEST(TestTensor, MultipleTensorsOnOneStorage) {
	auto sharedStoragePtr = this->largeStorage;
	const auto shape = this->largeShape;

	GraphEngine::Tensor<TypeParam> tensor1(sharedStoragePtr, shape);
	GraphEngine::Tensor<TypeParam> tensor2(sharedStoragePtr, shape);
	auto viewTensor1 = tensor1.template view<2>();
	auto viewTensor2 = tensor2.template view<2>();
	// Modify via tensor/view one, and check via tensor two
	viewTensor1[100, 100] = 42;
	auto element = viewTensor2[100, 100];

	EXPECT_EQ(element, 42);
	EXPECT_TRUE(sharedStoragePtr->is_shared());
}