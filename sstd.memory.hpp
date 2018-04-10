#ifndef SSTD_MEMORY_HPP
#define SSTD_MEMORY_HPP

#include <memory>
#include <type_traits>

namespace sstd {/*namespace sstd*/
	namespace _1_p_sstd_memory_ {/*namespace _1_p_sstd_memory_*/

		template<typename T>
		class alignas(T)MemoryWrapVirtualClass : public T {
			static_assert(std::has_virtual_destructor<T>::value, "T must has a virtual destructor");
			template<typename ... Args >
			using IsCe = std::enable_if_t<std::is_constructible<T, Args ...>::value, int*>;
			template<typename ... Args >
			using IsNotCe = std::enable_if_t<!std::is_constructible<T, Args ...>::value, int**>;
		public:
			MemoryWrapVirtualClass() :T{} {/*fore to init data*/ }
			template<typename A0, typename ... Args, typename = IsCe<A0&&, Args&&...>/**/>
			MemoryWrapVirtualClass(A0&&a0, Args && ... args) : T(std::forward<A0>(a0), std::forward<Args>(args)...) {}
			template<typename A0, typename ... Args, typename = IsNotCe<A0&&, Args&&...>, typename = int *** >
			MemoryWrapVirtualClass(A0&&a0, Args && ... args) : T{ std::forward<A0>(a0), std::forward<Args>(args)... } {}
			virtual ~MemoryWrapVirtualClass() = default;

			T * $get_wrap_pointer() noexcept { return this; }
			T * $get_wrap_pointer() const noexcept { return this; }

			MemoryWrapVirtualClass(const MemoryWrapVirtualClass &) = delete;
			MemoryWrapVirtualClass(MemoryWrapVirtualClass &&) = delete;
			MemoryWrapVirtualClass& operator=(const MemoryWrapVirtualClass &) = delete;
			MemoryWrapVirtualClass& operator=(MemoryWrapVirtualClass &&) = delete;

			/*!!! rewrite to add operator new/delete*/
		};

		template<typename T>
		class alignas(T)MemoryWrap {
			template<typename ... Args >
			using IsCe = std::enable_if_t<std::is_constructible<T, Args ...>::value, float*>;
			template<typename ... Args >
			using IsNotCe = std::enable_if_t<!std::is_constructible<T, Args ...>::value, float**>;
		public:
			std::aligned_storage_t<sizeof(T), alignof(T)> raw_data;

			T * pointer() noexcept { return reinterpret_cast<T*>(&raw_data); }
			T * pointer() const noexcept { return reinterpret_cast<T*>(&raw_data); }

			T * get() noexcept { return pointer(); }
			T * get() const noexcept { return pointer(); }

			T * $get_wrap_pointer() noexcept { return pointer(); }
			T * $get_wrap_pointer() const noexcept { return pointer(); }

			T * operator->() const noexcept { return pointer(); }
			T * operator->() noexcept { return pointer(); }

			T & operator*() noexcept { return *pointer(); }
			T & operator*()const noexcept { return *pointer(); }

			MemoryWrap(const MemoryWrap &) = delete;
			MemoryWrap(MemoryWrap &&) = delete;
			MemoryWrap& operator=(const MemoryWrap &) = delete;
			MemoryWrap& operator=(MemoryWrap &&) = delete;
			MemoryWrap() { ::new(&raw_data) T{}; /*fore to init data*/ }
			template<typename A0, typename ... Args, typename = IsCe<A0&&, Args&&...> /**/ >
			MemoryWrap(A0&&a0, Args&&...args) {
				::new(&raw_data) T(std::forward<A0>(a0), std::forward<Args>(args)...);
			}
			template<typename A0, typename ... Args, typename = IsNotCe<A0&&, Args&&...>, typename = float *** /**/ >
			MemoryWrap(A0&&a0, Args&&...args) {
				::new(&raw_data) T{ std::forward<A0>(a0), std::forward<Args>(args)... };
			}
			~MemoryWrap() { this->pointer()->~T(); }

			/*!!! rewrite to add operator new/delete*/
		};

		template<typename ...> using __void_t = void;
		/*x.get*/
		template<typename T, typename = void>
		class HasRGetSelect : public std::false_type {};

		template<typename T>
		class HasRGetSelect<T, __void_t< decltype(std::declval<T>().$get_wrap_pointer()) >/**/> :public std::true_type {};

		/*x->get*/
		template<typename T, typename = void>
		class HasPGetSelect : public std::false_type {};
		template<typename T>
		class HasPGetSelect<T, __void_t< decltype(std::declval<T>()->$get_wrap_pointer()) >/**/> :public std::true_type {};

		template<bool/*r*/R, bool/*p*/P>class GetPointer {
		public:
			static_assert(P || R, "can not find : x.$get_wrap_pointer() or x->$get_wrap_pointer()");
			template<typename T>
			static inline decltype(nullptr) pointer(T &&) noexcept { return nullptr; }
		};

		template<>class GetPointer<true, true> {
		public:
			template<typename T>
			static inline decltype(auto) pointer(T && arg) noexcept { return arg->$get_wrap_pointer(); }
		};

		template<>class GetPointer<true, false> {
		public:
			template<typename T>
			static inline decltype(auto) pointer(T && arg) noexcept { return arg.$get_wrap_pointer(); }
		};

		template<>class GetPointer<false, true> {
		public:
			template<typename T>
			static inline decltype(auto) pointer(T && arg) noexcept { return arg->$get_wrap_pointer(); }
		};

	}/*namespace _1_p_sstd_memory_*/

	/*memory wrap type*/
	template<typename T, bool = ((std::has_virtual_destructor<T>::value) && (std::is_final<T>::value == false)) >
	class alignas(T)MWType final:public _1_p_sstd_memory_::MemoryWrapVirtualClass<T>{ public:template<typename ... Args>MWType(Args && ... args) : _1_p_sstd_memory_::MemoryWrapVirtualClass<T>(std::forward<Args>(args)...) {} };
	template<typename T  >
	class alignas(T)MWType<T, false> final :public _1_p_sstd_memory_::MemoryWrap<T> {public:template<typename ... Args>MWType(Args && ... args) : _1_p_sstd_memory_::MemoryWrap<T>(std::forward<Args>(args)...) {}};

	template<typename T>
	inline decltype(auto) pointer(T && arg) noexcept {
		return _1_p_sstd_memory_::GetPointer<
			_1_p_sstd_memory_::HasRGetSelect<T&&>::value,
			_1_p_sstd_memory_::HasPGetSelect<T&&>::value>::template pointer(std::forward<T>(arg));
	}

	template<typename T, typename ... Args>
	inline MWType<T> * wnew(Args&&...args) {
		return new MWType<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	inline auto & wr(T && arg) noexcept {
		return *pointer(std::forward<T>(arg));
	}

	template<typename T, typename ... Args>
	std::unique_ptr<MWType<T>/**/> make_unique(Args && ... args) {
		return std::make_unique<MWType<T>/**/>(std::forward<Args>(args)...);
	}/*make unique*/

	template<typename T, typename ... Args>
	std::shared_ptr<MWType<T>> make_shared(Args && ... args) {
		using U = MWType<T>;
		U * varData = wnew<T>(std::forward<Args>(args)...);
		/*!!! set alloc here*/
		return std::shared_ptr<U>(varData, std::default_delete<U>());
	}/*make shared*/

}/*namespace sstd*/

#endif // SSTD_MEMORY

