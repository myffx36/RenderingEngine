#include "stdafx.h"
#include "Configuration.h"

#include "Container.hpp"

namespace Necromancer{
	int Configuration::get_integer_value(
		const String& section_name, const String& name)
	{
		String str_value = get_value(section_name, name);
		int value = atoi(str_value.c_str());
		return value;
	}

	String Configuration::get_value(
		const String& section_name, const String& name)
	{
		return m_section_value_map[section_name][name];
	}

	void Configuration::load_file(const String& file_name){
		std::ifstream in_file(file_name);
		String line_str;
		String current_section_name = "";
		while(!in_file.eof()){
			std::getline(in_file, line_str);
			String::size_type comment_pos = line_str.find_first_of(';');
			if(comment_pos != String::npos){
				line_str = line_str.substr(0, comment_pos);
			}
			String::size_type section_begin_pos = line_str.find_first_of('[');
			if (section_begin_pos != String::npos){
				current_section_name = read_section(line_str);
			}else{
				String::size_type equal_pos = line_str.find_first_of('=');
				if (equal_pos != String::npos){
					read_value(current_section_name, line_str);
				}
			}
		}
	}

	String Configuration::read_section(const String& line_str){
		String::size_type section_begin_pos = line_str.find_first_of('[');
		String::size_type section_end_pos = line_str.find_first_of(']');
		String section_name = line_str.substr(
			section_begin_pos + 1, section_end_pos - 1);
		m_section_value_map[section_name] = ValueMapType();
		return section_name;
	}

	void Configuration::read_value(
		const String& sectio_name, const String& line_str)
	{
		ValueMapType& value_map = m_section_value_map[sectio_name];
		String::size_type equal_pos = line_str.find('=');
		String value_name = line_str.substr(0, equal_pos);
		String value = line_str.substr(equal_pos + 1, line_str.size() - 1);
		value_map[value_name] = value;
	}
}