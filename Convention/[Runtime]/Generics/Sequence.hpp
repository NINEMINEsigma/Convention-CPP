#pragma once
#ifndef Convention_Runtime_Generics_Sequence_hpp
#define Convention_Runtime_Generics_Sequence_hpp

#include"Config.hpp"

namespace Convention
{
	namespace Generics
	{

		template<typename Element, size_t size>
		using Array = std::array<Element, size>;

		template<typename Element, template<typename> class Allocator>
		using Vector = std::vector<Element, Allocator<Element>>;

	}
}

#endif Convention_Runtime_Generics_Sequence_hpp