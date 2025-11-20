#pragma once
#include <cstddef>
#include <vector>

namespace GraphEngine::Tensor {

// Forward Decl.
template <typename T>
class Tensor;

template <typename T>
class Storage {
	using size_type = std::size_t;
	using container = std::vector<T>;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;

public:
	explicit Storage(size_type size) : data_(size), size_(size) {}

	// Use custom allocator later
	template <typename Allocator>
	explicit Storage(size_type size, Allocator alloc) : data_(size, T{}, alloc), size_(size) {}

	auto data() -> pointer { return data_.data(); }
	auto data() const -> const_pointer { return data_.data(); }
	[[nodiscard]] auto size() const -> size_type { return size_; }

	[[nodiscard]] auto is_shared() const -> bool { return shared_; }

private:
	container data_;
	size_type size_;
	bool shared_ = false;

	friend class Tensor<T>;
};

}  // namespace GraphEngine::Tensor