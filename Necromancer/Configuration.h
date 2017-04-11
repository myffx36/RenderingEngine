#pragma once

#include "Container.hpp"

namespace Necromancer{
	class Configuration{
	public:
		int get_integer_value(
			const String& section_name,
			const String& name);

		String get_value(const String& section_name,
			const String& name);

		virtual void load_file(const String&);
	private:
		String read_section(const String& line_str);
		void read_value(const String& section, const String& line_str);
	private:
		typedef Map<String, String>::Type ValueMapType;
		Map<String, ValueMapType>::Type m_section_value_map;
	};
}

