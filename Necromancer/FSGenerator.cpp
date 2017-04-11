#include "stdafx.h"
#include "FSGenerator.h"

#include "Algorithm.hpp"
#include "FSProcess.h"
#include "PIL.h"
#include "PILLLGI.h"
#include "FSDeclaration.h"

namespace Necromancer{
	FSPtr FSGenerator::generate_fragment_shader(
		const String& fs_code, const String& format)
	{
		String code = fs_code + "_" + format;
		auto iter = m_fs_map.find(code);
		if (iter == m_fs_map.end()){
			return create_fragment_shader(fs_code, format);
		}
		else{
			return iter->second;
		}
	}

	FSGenerator::~FSGenerator(){
		for (auto iter = m_uid_map.begin(); iter != m_uid_map.end(); ++iter){
			delete iter->second;
		}
	}

	void FSGenerator::attach_fs_process(FSProcess* fs_process){
		m_uid_map[fs_process->uid()] = fs_process;
	}

	void FSGenerator::attach_fs_declaration(FSDeclaration* declaration){
		m_declaration_map[declaration->uid()] = declaration;
	}

	FSPtr FSGenerator::create_fragment_shader(
		const String& fs_code, const String& format)
	{
		String str = fs_code;
		String::size_type pos = 0;

		Queue<U32>::Type queue;

		while ((pos = str.find_first_of('_')) != String::npos){
			String token = str.substr(0, pos);
			str = str.substr(pos + 1, str.length());

			std::stringstream sstream;
			sstream << token;
			U32 value;
			sstream >> value;

			queue.push(value);
		}

		std::stringstream sstream;
		sstream << str;
		U32 value;
		sstream >> value;
		queue.push(value);

		Queue<FSProcess*>::Type fs_process_queue;
		while (!queue.empty()){
			U32 uid = queue.front();
			queue.pop();
			FSProcess* fs_process = m_uid_map[uid];
			fs_process_queue.push(fs_process);
		}

		String declaration;
		String body;
		String input = format;
		String output;

		List<U32>::Type fs_declaration_list;
		U32 output_num = 0;

		while (!fs_process_queue.empty()){
			FSProcess* process = fs_process_queue.front();
			fs_process_queue.pop();

			//检查声明是否重复 若没有重复则加入链表末尾
			//若重复则不加
			const List<U32>::Type& declaration_list =
				process->declaration_list();
			for (auto iter = declaration_list.begin();
				iter != declaration_list.end(); ++iter)
			{
				auto dec_iter = std::find(
					fs_declaration_list.begin(),
					fs_declaration_list.end(), *iter);
				if (dec_iter == fs_declaration_list.end()){
					fs_declaration_list.push_back(*iter);
				}
			}

			body += process->process_string(format);
			body += "\n";
			input = combine_vs_input_and_output(process->input_format(), input);

			output_num = std::max(output_num, process->output_num());
		}

		//将找出来的声明字符串拼接起来 
		for (auto iter = fs_declaration_list.begin();
			iter != fs_declaration_list.end(); ++iter)
		{
			FSDeclaration* fs_declaration = m_declaration_map[*iter];
			declaration += fs_declaration->declaration_string();
			declaration += "\n";
		}

		String fs_string = generate_input_text(input) +
			generate_output_text(output_num) +
			declaration + generate_header_text(input) +
			body + generate_tail_text(input);

		const char* file_name = nullptr;
#ifdef _DEBUG
		String file_name_str = save_debug_file(fs_code, fs_string).c_str();
		file_name = file_name_str.c_str();
#endif

		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		FSPtr fs = llgi->create_fragment_shader(
			fs_string.c_str(), file_name);

		m_fs_map[fs_code + "_" + format] = fs;

		return fs;
	}

#ifdef _DEBUG
	String FSGenerator::save_debug_file(
		const String& fs_code, const String& fs_text)
	{
		String file_name = "shaders\\" + fs_code + "_fs.hlsl";

		std::ofstream out_file(file_name);
		out_file << fs_text << std::endl;
		out_file.close();


		return file_name;
	}
#endif

	String FSGenerator::combine_vs_input_and_output(
		const String& str1, const String& str2)
	{
		String result = str1;
		for (U32 i = 0; i < str2.length(); ++i){
			if (result.find(str2[i]) == -1){
				result += str2[i];
			}
		}

		return result;
	}


	String FSGenerator::generate_input_text(const String& format){
		String result = m_input_head;
		for (size_t i = 0; i < format.size(); ++i){
			result += m_input_map[format[i]];
		}
		result += m_input_tail;
		return result;
	}

	String FSGenerator::generate_output_text(U32 num){
		String result = m_output_head;
		for (U32 i = 0; i < num; ++i){
			String tmp = m_output_mode;
			std::replace(tmp.begin(), tmp.end(), '#', char('0' + i));
			result += tmp;
		}
		result += m_output_tail;
		return result;
	}

	String FSGenerator::generate_header_text(const String& input){
		String result = m_head_common;
		for (U32 i = 0; i < input.size(); ++i){
			result += m_head_map[input[i]];
		}
		return result;
	}

	String FSGenerator::generate_tail_text(const String& input){
		return m_tail;
	}

	void FSGenerator::set_head_common(const String& s){
		m_head_common = s;
	}

	void FSGenerator::set_tail(const String& s){
		m_tail = s;
	}

	void FSGenerator::set_head_map(char c, const String& s){
		m_head_map[c] = s;
	}

	void FSGenerator::set_output_mode(const String& s){
		m_output_mode = s;
	}

	void FSGenerator::set_output_head(const String& s){
		m_output_head = s;
	}

	void FSGenerator::set_output_tail(const String& s){
		m_output_tail = s;
	}

	void FSGenerator::set_input_head(const String& s){
		m_input_head = s;
	}

	void FSGenerator::set_input_tail(const String& s){
		m_input_tail = s;
	}

	void FSGenerator::set_input_map(char c, const String& s){
		m_input_map[c] = s;
	}

	void FSGenerator::set_extension(const String& s){
		m_extension = s;
	}
}