#pragma once
#ifndef Convention_Runtime_Generics_Sequence_hpp
#define Convention_Runtime_Generics_Sequence_hpp

#include"Config.hpp"

namespace Convention
{
	namespace Generics
	{
		template<typename Element, typename Index = int, typename ReadValueType = Element&>
		struct ISequence
		{
			virtual ~ISequence() {}
			virtual ReadValueType operator[](Index index) abstract;
			virtual Index size();
			virtual Index GetBeginIndex() const abstract;
			virtual Index GetEndIndex() const abstract;
			virtual Index GetHeadIndex() const
			{
				return GetBeginIndex();
			}
			virtual Index GetTailIndex() const
			{
				return GetEndIndex() - 1;
			}
		};

		namespace Iterator
		{
			template<typename Element, typename ReadValueType = Element&>
			struct ISequenceIterator
			{
				virtual ~ISequenceIterator() {}
				virtual void Next() abstract;
				virtual ReadValueType ReadValue() const abstract;
				virtual ISequenceIterator& operator++()
				{
					this->Next();
					return *this;
				}
				virtual bool operator==(const ISequenceIterator& other) const noexcept abstract;
				bool operator!=(const ISequenceIterator& other) const noexcept
				{
					return !(*this == other);
				}
				ReadValueType operator*() const
				{
					return ReadValue();
				}
			};

			template<typename Element>
			class DefaultSequenceIterator:public ISequenceIterator<Element>
			{
			public:
				DefaultSequenceIterator() {}
			};
		}

		/**
		* @brief 序列容器枚举
		* @tparam Sequence 序列类型
		* @tparam _Index 索引类型
		* @version BS 0.0.1
		*/
		template<typename Sequence, typename _Index = int>
		class SequenceIterator
		{
		private:
			const Sequence& target;
			_Index index;
		public:
			SequenceIterator(const Sequence& target, _Index index) :__init(target), __init(index) {}
			SequenceIterator(const SequenceIterator& other) : target(other.target), index(other.index) {}
			SequenceIterator& operator=(const SequenceIterator& other)
			{
				target = other.target;
				index = other.index;
				return *this;
			}
			bool operator==(const SequenceIterator& other) noexcept
			{
				return other.index == index;
			}
			bool operator!=(const SequenceIterator& other) noexcept
			{
				return other.index != index;
			}
			SequenceIterator& operator++() noexcept
			{
				++index;
				return *this;
			}
			SequenceIterator operator++(int) noexcept
			{
				++index;
				return SequenceIterator(target, index - 1);
			}
			SequenceIterator& operator--() noexcept
			{
				--index;
				return *this;
			}
			SequenceIterator operator--(int) noexcept
			{
				--index;
				return SequenceIterator(target, index + 1);
			}
		};

		/**
		* @brief 序列容器可枚举类型
		* @tparam Sequence 序列类型
		* @tparam _Index 索引类型
		* @version BS 0.0.1
		*/
		template<typename Sequence, typename _Index = int>
		struct SequenceRange
		{
			using iterator = SequenceIterator<Sequence, _Index>;
			iterator mbegin, mend;
			SequenceRange(const Sequence& target, _Index begin = 0, _Index end = 0ll - 1) :mbegin(target, begin), mend(target, end) {}

			constexpr iterator begin()
			{
				return mbegin;
			}
			constexpr iterator end()
			{
				return mend;
			}
		};


		/**
		* @brief 栈上静态数组
		* @tparam Element 内容物类型
		* @tparam size 元素大小
		* @version BS 0.0.1
		*/
		template<typename Element, size_t size>
		using Array = std::array<Element, size>;

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
