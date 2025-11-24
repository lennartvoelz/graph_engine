#pragma once
#include <GraphEngine/Tensor/Storage.hpp>
#include <cassert>
#include <mdspan>
#include <memory>
#include <utility>

namespace GraphEngine {

// Forward declaration
template <typename T>
class Storage;

template <typename T>
class Tensor {
	using size_type = std::size_t;
	using pointer = T*;

public:
	// Allocating constructor
	explicit Tensor(const std::vector<size_type>& shape) : rank_(shape.size()), extents_(shape) {
		size_type contigouos_size{1};
		for (const auto dim : extents_) {
			contigouos_size *= dim;
		}
		storage_ = std::make_shared<Storage<T>>(contigouos_size);
	}

	// View constructor for buffer reuse; add functionality later
	explicit Tensor(std::shared_ptr<Storage<T>> storage, const std::vector<size_type>& shape)
	    : rank_(shape.size()), storage_(storage), extents_(shape) {
		storage_->shared_ = true;
	}

	[[nodiscard]] auto rank() const -> size_type { return rank_; }
	[[nodiscard]] auto data() const -> pointer { return storage_->data(); }
	[[nodiscard]] auto extents() const -> const std::vector<size_type>& { return extents_; }

	/*
	  For now stick to static layouts/accessors, later support them dynamically via type erasure
	*/
	template <size_type Rank, typename LayoutPolicy = std::layout_right,
	          typename AccessorPolicy = std::default_accessor<T>>
	auto view() {
		assert(rank_ == Rank);
		return create_view<LayoutPolicy, AccessorPolicy, Rank>(std::make_index_sequence<Rank>{});
	}

private:
	size_type rank_;
	std::shared_ptr<Storage<T>> storage_;
	std::vector<size_type> extents_;

	template <typename LayoutPolicy, typename AccessorPolicy, size_type Rank, size_type... IndexSeq>
	auto create_view(std::index_sequence<IndexSeq...> /*only used for expansion*/) {
		return std::mdspan<T, std::dextents<size_type, Rank>, LayoutPolicy, AccessorPolicy>(storage_->data(),
		                                                                                    extents_[IndexSeq]...);
	}
};

}  // namespace GraphEngine