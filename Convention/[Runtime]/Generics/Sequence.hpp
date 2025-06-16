/**
* @brief 序列
* @contains SequenceTrait 序列性状
* @contains SequenceIndicator 指示序列动作
* @contains Array 数组
* @contains BoolArray bool数组
* @contains Vector 可变数组
*/

#pragma once
#ifndef Convention_Runtime_Generics_Sequence_hpp
#define Convention_Runtime_Generics_Sequence_hpp

#include"Config.hpp"

namespace Convention
{
	namespace Generics
	{

		namespace Collections
		{

			/**
			* @brief 支持可枚举类型的迭代
			* @tparam ReadValueType 读出元素的类型
			* @version BS 0.0.1
			*/
			template<typename ReadValueType>
			struct IEnumerator
			{
				virtual ~IEnumerator() {}
				/**
				* @brief 获取位于枚举器当前位置中的元素
				* @return The element in the collection at the current position of the enumerator.
				*/
				virtual ReadValueType ReadValue() const abstract;

				/**
				* @brief 将枚举器前进到下一个元素
				*
				* @return	如果枚举器成功前进到下一个元素返回true,
				*			否则枚举器到达无效的位置或枚举器已失效
				*/
				virtual bool MoveNext() abstract;
			};

			/**
			* @brief 可枚举对象
			* @tparam Enumerator 枚举器
			* @version BS 0.0.1
			*/
			template<typename Enumerator>
			struct IEnumerable
			{
				virtual Enumerator begin() abstract;
				virtual Enumerator end() abstract;
			};

		}

		/**
		* @brief 序列性状, 用于静态判断
		* @tparam Sequence 序列类型
		*/
		template<typename Sequence>
		struct SequenceTrait
		{
			constexpr static bool _IsSequence()
			{
				if_exists(Sequence::begin)
				{
					if_exists(Sequence::end)
					{
						if_exists(Sequence::cbegin)
						{
							if_exists(Sequence::cend)
							{
								if_exists(Sequence::operator[])
								{
									return true;
								}
							}
						}
					}
				}
				return false;
			};
			constexpr static bool IsSequence = _IsSequence();
		};

		/**
		* @brief 序列指示器, 序列动作的适配器
		* @tparam Sequence 序列类型
		*/
		template<typename Sequence>
		struct SequenceIndicator
		{
			static_assert(SequenceTrait<Sequence>::IsSequence,
				"Current template type is not a Sequence");
			using iterator = decltype(std::declval<Sequence>().begin());
			using const_iterator = decltype(std::declval<Sequence>().cbegin());
			using element = std::decay_t<decltype(std::declval<Sequence>()[0])>;

			static iterator begin(Sequence& self)
			{
				return self.begin();
			}
			static iterator end(Sequence& self)
			{
				return self.end();
			}
			static const_iterator begin(const Sequence& self)
			{
				return self.cbegin();
			}
			static const_iterator end(const Sequence& self)
			{
				return self.cend();
			}
			static element& at(Sequence& self, int index)
			{
				return self[index];
			}
		};

		/**
		* @brief 栈上静态数组
		* @tparam Element 内容物类型
		* @tparam ElementSize 内容物数量
		* @version BS 0.0.1
		*/
		template<typename Element, size_t ElementSize>
		using Array = std::array<Element, ElementSize>;

		namespace internal
		{
			template <size_t _Size>
			class _BoolArray_const_iterator
#if _ITERATOR_DEBUG_LEVEL != 0
				: private _Iterator_base12_compatible
#endif // _ITERATOR_DEBUG_LEVEL != 0
			{
			public:
				using _Ty = bool;
#if _HAS_CXX20
				using iterator_concept = contiguous_iterator_tag;
#endif // _HAS_CXX20
				using iterator_category = random_access_iterator_tag;
				using value_type = _Ty;
				using difference_type = ptrdiff_t;
				using pointer = const _Ty*;
				using reference = const _Ty&;

				enum { _EEN_SIZE = _Size }; // helper for expression evaluator

#if _ITERATOR_DEBUG_LEVEL == 0
				_CONSTEXPR17 _Array_const_iterator() noexcept : _Ptr() {}

				_CONSTEXPR17 explicit _Array_const_iterator(pointer _Parg, size_t _Off = 0) noexcept : _Ptr(_Parg + _Off) {}

				_NODISCARD _CONSTEXPR17 reference operator*() const noexcept {
					return *_Ptr;
				}

				_NODISCARD _CONSTEXPR17 pointer operator->() const noexcept {
					return _Ptr;
				}

				_CONSTEXPR17 _Array_const_iterator& operator++() noexcept {
					++_Ptr;
					return *this;
				}

				_CONSTEXPR17 _Array_const_iterator operator++(int) noexcept {
					_Array_const_iterator _Tmp = *this;
					++_Ptr;
					return _Tmp;
				}

				_CONSTEXPR17 _Array_const_iterator& operator--() noexcept {
					--_Ptr;
					return *this;
				}

				_CONSTEXPR17 _Array_const_iterator operator--(int) noexcept {
					_Array_const_iterator _Tmp = *this;
					--_Ptr;
					return _Tmp;
				}

				_CONSTEXPR17 _Array_const_iterator& operator+=(const ptrdiff_t _Off) noexcept {
					_Ptr += _Off;
					return *this;
				}

				_CONSTEXPR17 _Array_const_iterator& operator-=(const ptrdiff_t _Off) noexcept {
					_Ptr -= _Off;
					return *this;
				}

				_NODISCARD _CONSTEXPR17 ptrdiff_t operator-(const _Array_const_iterator& _Right) const noexcept {
					return _Ptr - _Right._Ptr;
				}

				_NODISCARD _CONSTEXPR17 reference operator[](const ptrdiff_t _Off) const noexcept {
					return _Ptr[_Off];
				}

				_NODISCARD _CONSTEXPR17 bool operator==(const _Array_const_iterator& _Right) const noexcept {
					return _Ptr == _Right._Ptr;
				}

#if _HAS_CXX20
				_NODISCARD constexpr strong_ordering operator<=>(const _Array_const_iterator& _Right) const noexcept {
					return _Ptr <=> _Right._Ptr;
				}
#else // ^^^ _HAS_CXX20 / !_HAS_CXX20 vvv
				_NODISCARD _CONSTEXPR17 bool operator<(const _Array_const_iterator& _Right) const noexcept {
					return _Ptr < _Right._Ptr;
				}
#endif // ^^^ !_HAS_CXX20 ^^^

				using _Prevent_inheriting_unwrap = _Array_const_iterator;

				_NODISCARD constexpr pointer _Unwrapped() const noexcept {
					return _Ptr;
				}

				static constexpr bool _Unwrap_when_unverified = true;

				constexpr void _Seek_to(pointer _It) noexcept {
					_Ptr = _It;
				}

			private:
				pointer _Ptr; // beginning of array

#else // ^^^ _ITERATOR_DEBUG_LEVEL == 0 / _ITERATOR_DEBUG_LEVEL != 0 vvv
				_CONSTEXPR17 _Array_const_iterator() noexcept : _Ptr(), _Idx(0) {}

				_CONSTEXPR17 explicit _Array_const_iterator(pointer _Parg, size_t _Off = 0) noexcept : _Ptr(_Parg), _Idx(_Off) {}

				_NODISCARD _CONSTEXPR17 reference operator*() const noexcept {
					return *operator->();
				}

				_NODISCARD _CONSTEXPR17 pointer operator->() const noexcept {
					_STL_VERIFY(_Ptr, "cannot dereference value-initialized array iterator");
					_STL_VERIFY(_Idx < _Size, "cannot dereference out of range array iterator");
					return _Ptr + _Idx;
				}

				_CONSTEXPR17 _Array_const_iterator& operator++() noexcept {
					_STL_VERIFY(_Ptr, "cannot increment value-initialized array iterator");
					_STL_VERIFY(_Idx < _Size, "cannot increment array iterator past end");
					++_Idx;
					return *this;
				}

				_CONSTEXPR17 _Array_const_iterator operator++(int) noexcept {
					_Array_const_iterator _Tmp = *this;
					++*this;
					return _Tmp;
				}

				_CONSTEXPR17 _Array_const_iterator& operator--() noexcept {
					_STL_VERIFY(_Ptr, "cannot decrement value-initialized array iterator");
					_STL_VERIFY(_Idx != 0, "cannot decrement array iterator before begin");
					--_Idx;
					return *this;
				}

				_CONSTEXPR17 _Array_const_iterator operator--(int) noexcept {
					_Array_const_iterator _Tmp = *this;
					--*this;
					return _Tmp;
				}

				constexpr void _Verify_offset(const ptrdiff_t _Off) const noexcept {
					if (_Off != 0) {
						_STL_VERIFY(_Ptr, "cannot seek value-initialized array iterator");
					}

					if (_Off < 0) {
						_STL_VERIFY(_Idx >= size_t{ 0 } - static_cast<size_t>(_Off), "cannot seek array iterator before begin");
					}

					if (_Off > 0) {
						_STL_VERIFY(_Size - _Idx >= static_cast<size_t>(_Off), "cannot seek array iterator after end");
					}
				}

				_CONSTEXPR17 _Array_const_iterator& operator+=(const ptrdiff_t _Off) noexcept {
					_Verify_offset(_Off);
					_Idx += static_cast<size_t>(_Off);
					return *this;
				}

				_CONSTEXPR17 _Array_const_iterator& operator-=(const ptrdiff_t _Off) noexcept {
					return *this += -_Off;
				}

				_NODISCARD _CONSTEXPR17 ptrdiff_t operator-(const _Array_const_iterator& _Right) const noexcept {
					_Compat(_Right);
					return static_cast<ptrdiff_t>(_Idx - _Right._Idx);
				}

				_NODISCARD _CONSTEXPR17 reference operator[](const ptrdiff_t _Off) const noexcept {
					return *(*this + _Off);
				}

				_NODISCARD _CONSTEXPR17 bool operator==(const _Array_const_iterator& _Right) const noexcept {
					_Compat(_Right);
					return _Idx == _Right._Idx;
				}

#if _HAS_CXX20
				_NODISCARD constexpr strong_ordering operator<=>(const _Array_const_iterator& _Right) const noexcept {
					_Compat(_Right);
					return _Idx <=> _Right._Idx;
				}
#else // ^^^ _HAS_CXX20 / !_HAS_CXX20 vvv
				_NODISCARD _CONSTEXPR17 bool operator<(const _Array_const_iterator& _Right) const noexcept {
					_Compat(_Right);
					return _Idx < _Right._Idx;
				}
#endif // ^^^ !_HAS_CXX20 ^^^

				_CONSTEXPR17 void _Compat(const _Array_const_iterator& _Right) const noexcept { // test for compatible iterator pair
					_STL_VERIFY(_Ptr == _Right._Ptr, "array iterators incompatible");
				}

				using _Prevent_inheriting_unwrap = _Array_const_iterator;

				_NODISCARD constexpr pointer _Unwrapped() const noexcept {
					return _Ptr + _Idx;
				}

				constexpr void _Verify_with(const _Array_const_iterator& _Last) const noexcept {
					// note _Compat check inside operator<=
					_STL_VERIFY(*this <= _Last, "array iterator range transposed");
				}

				constexpr void _Seek_to(pointer _It) noexcept {
					_Idx = static_cast<size_t>(_It - _Ptr);
				}

			private:
				pointer _Ptr; // beginning of array
				size_t _Idx; // offset into array
#endif // ^^^ _ITERATOR_DEBUG_LEVEL != 0 ^^^

			public:
				_NODISCARD _CONSTEXPR17 _Array_const_iterator operator+(const ptrdiff_t _Off) const noexcept {
					_Array_const_iterator _Tmp = *this;
					_Tmp += _Off;
					return _Tmp;
				}

				_NODISCARD _CONSTEXPR17 _Array_const_iterator operator-(const ptrdiff_t _Off) const noexcept {
					_Array_const_iterator _Tmp = *this;
					_Tmp -= _Off;
					return _Tmp;
				}

				_NODISCARD friend _CONSTEXPR17 _Array_const_iterator operator+(
					const ptrdiff_t _Off, _Array_const_iterator _Next) noexcept {
					_Next += _Off;
					return _Next;
				}

#if !_HAS_CXX20
				_NODISCARD _CONSTEXPR17 bool operator!=(const _Array_const_iterator& _Right) const noexcept {
					return !(*this == _Right);
				}

				_NODISCARD _CONSTEXPR17 bool operator>(const _Array_const_iterator& _Right) const noexcept {
					return _Right < *this;
				}

				_NODISCARD _CONSTEXPR17 bool operator<=(const _Array_const_iterator& _Right) const noexcept {
					return !(_Right < *this);
				}

				_NODISCARD _CONSTEXPR17 bool operator>=(const _Array_const_iterator& _Right) const noexcept {
					return !(*this < _Right);
				}
#endif // !_HAS_CXX20
			};
		}

		/**
		* @brief 栈上静态Bool数组
		* @tparam size 元素大小
		* @version BS 0.0.1
		*/
		template<size_t capacity>
		class BoolArray
			: private Array<
			std::conditional_t<PlatformIndicator::IsPlatformx64, int64_t, int32_t>,
			capacity / sizeof(std::conditional_t<PlatformIndicator::IsPlatformx64, int64_t, int32_t>) + (capacity % sizeof(std::conditional_t<PlatformIndicator::IsPlatformx64, int64_t, int32_t>) ? 1 : 0)
			>
		{
		private:
			constexpr static const char* out_of_range_message = "out of range, capacity: ";
			using _MyElementTy = std::conditional_t<PlatformIndicator::IsPlatformx64, int64_t, int32_t>;
			constexpr static size_t _MyCapacity = capacity / sizeof(_MyElementTy) + (capacity % sizeof(_MyElementTy) ? 1 : 0);
			using _Mybase = Array<_MyElementTy, _MyCapacity>;
			constexpr _Mybase& _GetBaseArray() noexcept
			{
				return *this;
			}
			constexpr const _Mybase& _GetBaseArray() const noexcept
			{
				return *this;
			}
		public:
			inline constexpr size_t size() const noexcept
			{
				return capacity;
			}
			int ReadValue(size_t index) const
			{
				if (index < capacity)
				{
					size_t i = index / sizeof(_MyElementTy), offset = index % sizeof(_MyElementTy);
					return _GetBaseArray()[i] & (1ll << offset);
				}
				throw std::out_of_range(StringIndicator::Combine<std::string>(out_of_range_message, capacity));
			}
			bool operator[](int index) const
			{
				return ReadValue(index < 0 ? capacity + index : index);
			}
			void WriteValue(size_t index, bool value)
			{
				if (index < capacity)
				{
					size_t i = index / sizeof(_MyElementTy), offset = index % sizeof(_MyElementTy);
					if (value)
						_GetBaseArray()[i] |= 1ll << offset;
					else
						_GetBaseArray()[i] &= ~(1ll << offset);
				}
				throw std::out_of_range(StringIndicator::Combine<std::string>(out_of_range_message, capacity));
			}
		};

		/**
		* @brief 堆上动态数组
		* @tparam Element 内容物类型
		* @tparam Allocator 内存分配器
		* @version BS 0.0.1
		*/
		template<typename Element, template<typename> class Allocator>
		using Vector = std::vector<Element, Allocator<Element>>;

	}
}

#endif Convention_Runtime_Generics_Sequence_hpp
