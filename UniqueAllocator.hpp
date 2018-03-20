#ifndef HPP_UNIQUE_ALLOCATOR_0x00
#define HPP_UNIQUE_ALLOCATOR_0x00

#include <memory>
#include <type_traits>
#include <memory_resource>

template<typename T, typename Alloc = std::pmr::polymorphic_allocator<T> /**/>
class UniqueAllocator {
public:
	static_assert(std::is_array_v<T>/**/ ==/**/false);
	static_assert(std::is_reference_v<T>/**/ ==/**/false);
public:
	Alloc member_allocator;
	UniqueAllocator() = default;
	UniqueAllocator(UniqueAllocator&&) = default;
	UniqueAllocator(const UniqueAllocator&) = default;
	UniqueAllocator&operator=(UniqueAllocator&&) = default;
	UniqueAllocator&operator=(const UniqueAllocator&) = default;
	~UniqueAllocator() = default;
public:
	inline void operator()(T*arg) {
		if (arg == nullptr) { return; }
		constexpr const bool varIsNeedDestory = std::has_virtual_destructor_v<T> || (
			std::is_class_v<T> && (!std::is_trivially_destructible_v<T>));
		if constexpr(varIsNeedDestory) {
			std::destroy_at(arg);
		}
		this->member_allocator.deallocate(arg, 1);
	}
public:
	using value_type = std::unique_ptr<T, UniqueAllocator<T, Alloc>/**/>;
public:
	template<typename ...Args>
	inline static value_type make_unique(Args&&...args) {
		UniqueAllocator<T, Alloc> varAlloc;
		T * varAns = varAlloc.member_allocator.allocate(1);
		if (varAns == nullptr) { return{}; }
		if constexpr(std::is_constructible_v<T, Args&&...>) {
			::new (varAns) T(std::forward<Args>(args)...);
		}
		else {
			::new (varAns) T{ std::forward<Args>(args)... };
		}
		return value_type{ varAns, std::move(varAlloc) };
	}
};

#endif
