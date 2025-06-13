#pragma once
#ifndef Convention_Runtime_Generics_Sequence_hpp
#define Convention_Runtime_Generics_Sequence_hpp

#include"Config.hpp"

namespace Convention
{
	namespace Generics
	{
		namespace Iterator
		{

			/**
			* @brief 序列迭代器接口
			* @tparam ReadValueType 读出元素类型
			* @version BS 0.0.1
			*/
			template<typename ReadValueType>
			struct ISequenceIterator
			{
				virtual ~ISequenceIterator() {}
				virtual void Next() abstract;
				virtual ReadValueType ReadValue() const abstract;
				ISequenceIterator& operator++()
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

			/**
			* @brief 序列迭代器的默认实现
			* @tparam Sequence 序列类型
			* @tparam Index 索引类型, 必须为具有operator++()的类型
			* @tparam ReadValueType 读出元素类型
			* @version BS 0.0.1
			*/
			template<
				typename Sequence,
				typename Index,
				typename ReadValueType = decltype(std::declval<Sequence>()[std::declval<Index>()])
			>
			class DefaultSequenceIterator
				: public ISequenceIterator<ReadValueType>,
				public IComparable<DefaultSequenceIterator<Sequence, Index, ReadValueType>, ICompare<Index>>
			{
			private:
				Sequence& target;
				Index index;
			public:
				operator const Index&()
				{
					return index;
				}
				DefaultSequenceIterator(Sequence& target, Index index)
					: __init(target), __init(index)
				{
				}
				DefaultSequenceIterator(const DefaultSequenceIterator& other) noexcept
					: target(other.target), index(other.index)
				{
				}
				DefaultSequenceIterator& operator=(const DefaultSequenceIterator& other) noexcept
				{
					target = other.target;
					index = other.index;
					return *this;
				}
				virtual ~DefaultSequenceIterator() {}
				void Next()
				{
					index++;
				}
				ReadValueType ReadValue() const
				{
					return target[index];
				}
				bool operator==(const ISequenceIterator<ReadValueType>& other) const noexcept
				{
					auto ptr = dynamic_cast<const DefaultSequenceIterator* const>(&other);
					if (ptr != nullptr)
						return index == ptr->index;
					return false;
				}
			};

		}

		/**
		* @brief 序列接口
		* @tparam Element 元素
		* @tparam Index 索引类型, 必须为具有operator++()的类型
		* @tparam ReadValueType 读出元素类型
		* @version BS 0.0.1
		*/
		template<
			typename Element,
			typename Index = int,
			typename ReadValueType = Element&
		>
		struct ISequence
		{
			virtual ~ISequence() {}
			virtual ReadValueType operator[](Index index) abstract;
			virtual size_t size() const noexcept abstract;
			Iterator::ISequenceIterator<ReadValueType> begin() abstract;
			Iterator::ISequenceIterator<ReadValueType> end() abstract;
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
