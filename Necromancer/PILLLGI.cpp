#include "stdafx.h"
#include "PILLLGI.h"

namespace Necromancer{

	namespace{
		String load_text_file(const char* file_name){
			std::ifstream file(file_name);
			String line;
			String result;
			while(!file.eof()){
				std::getline(file, line);
				result += line + "\n";
			}
			return result;
		}

		void load_binary_file(const char* file_name, void* data, U32& size){

		}
	}

	PILLLGI::~PILLLGI(){
	}

	VSPtr PILLLGI::create_vertex_shader_from_text_file(
		const char* file_name)
	{
		String text = load_text_file(file_name);
		return create_vertex_shader(text.c_str(), file_name);
	}

	FSPtr PILLLGI::create_fragment_shader_from_text_file(
		const char* file_name)
	{
		String text = load_text_file(file_name);
		return create_fragment_shader(text.c_str(), file_name);
	}

	VSPtr PILLLGI::create_vertex_shader_from_binary_file(
		const char* file_name)
	{
		const int max_char_num = 10240;
		char data[max_char_num];
		U32 size = 0;
		load_binary_file(file_name, data, size);
		return create_vertex_shader_from_binary(data, size, file_name);
	}

	FSPtr PILLLGI::create_fragment_shader_from_binary_file(
		const char* file_name)
	{
		const int max_char_num = 10240;
		char data[max_char_num];
		U32 size = 0;
		load_binary_file(file_name, data, size);
		return create_fragment_shader_from_binary(data, size, file_name);
	}

	GSPtr PILLLGI::create_geometry_shader_from_text_file(
		const char* file_name)
	{
		String text = load_text_file(file_name);
		return create_geometry_shader(text.c_str(), file_name);
	}

	CSPtr PILLLGI::create_compute_shader_from_text_file(
		const char* file_name)
	{
		String text = load_text_file(file_name);
		return create_compute_shader(text.c_str(), file_name);
	}
}