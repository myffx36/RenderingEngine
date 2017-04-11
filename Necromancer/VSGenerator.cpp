#include "stdafx.h"
#include "VSGenerator.h"

#include "VSProcess.h"
#include "VSDeclaration.h"

#include "Algorithm.hpp"
#include "PIL.h"
#include "PILLLGI.h"

namespace Necromancer{
	VSPtr VSGenerator::generate_vertex_shader(
		const String& vs_code, const String& format)
	{
		String code = vs_code + "_" + format;
		auto iter = m_vs_map.find(code);
		if(iter == m_vs_map.end()){
			return create_vertex_shader(vs_code, format);
		}else{
			return iter->second;
		}
	}

	VSGenerator::~VSGenerator(){
		for(auto iter = m_uid_map.begin();iter != m_uid_map.end();++ iter){
			delete iter->second;
		}
	}

	void VSGenerator::attach_vs_process(VSProcess* vs_process){
		m_uid_map[vs_process->uid()] = vs_process;
	}

	void VSGenerator::attach_vs_declaration(VSDeclaration* declaration){
		m_declaration_map[declaration->uid()] = declaration;
	}

	VSPtr VSGenerator::create_vertex_shader(
		const String& vs_code, const String& format)
	{
		String str = vs_code;
		String::size_type pos = 0;

		Queue<U32>::Type queue;

		while((pos = str.find_first_of('_')) != String::npos){
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

		Queue<VSProcess*>::Type vs_process_queue;
		while(!queue.empty()){
			U32 uid = queue.front();
			queue.pop();
			VSProcess* vs_process = m_uid_map[uid];
			vs_process_queue.push(vs_process);
		}

		String declaration;
		String body;
		String output;

		List<U32>::Type vs_declaration_list;

		while(!vs_process_queue.empty()){
			VSProcess* process = vs_process_queue.front();
			vs_process_queue.pop();

			//检查声明是否重复 若没有重复则加入链表末尾
			//若重复则不加
			const List<U32>::Type& declaration_list = process->declaration_list();
			for(auto iter = declaration_list.begin();
				iter != declaration_list.end();++ iter)
			{
				auto dec_iter = 
					std::find(vs_declaration_list.begin(), 
					vs_declaration_list.end(), *iter);
				if(dec_iter == vs_declaration_list.end()){
					vs_declaration_list.push_back(*iter);
				}
			}

			body += process->process_string(format);
			body += "\n";
			output = 
				combine_vs_input_and_output(
				process->output_format(format), output);
		}

		//将找出来的声明字符串拼接起来 
		for(auto iter = vs_declaration_list.begin();
			iter != vs_declaration_list.end();++ iter)
		{
			VSDeclaration* vs_declaration = m_declaration_map[*iter];
			declaration += vs_declaration->declaration_string();
			declaration += "\n";
		}

		String vs_string = generate_input_text(format) + generate_output_text(output) + 
			declaration + generate_header_text(format, output) + 
			body + generate_tail_text(format, output);


		const char* file_name = nullptr;
#ifdef _DEBUG
		String file_name_str = save_debug_file(vs_code, vs_string).c_str();
		file_name = file_name_str.c_str();
#endif

		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();
		VSPtr vs = llgi->create_vertex_shader(
			vs_string.c_str(), file_name);
		vs->set_input_format(format);
		vs->set_output_format(output);

		m_vs_map[vs_code + "_" + format] = vs;

		return vs;
	}

#ifdef _DEBUG
	String VSGenerator::save_debug_file(const String& vs_code, const String& vs_text){
		String file_name = "shaders\\" + vs_code + "_vs.hlsl";

		std::ofstream out_file(file_name);
		out_file << vs_text << std::endl;
		out_file.close();


		return file_name;
	}
#endif

	String VSGenerator::combine_vs_input_and_output(const String& str1, const String& str2){
		String result = str1;
		for(U32 i = 0;i < str2.length();++ i){
			if(result.find(str2[i]) == -1){
				result += str2[i];
			}
		}

		return result;
	}


	String VSGenerator::generate_input_text(const String& input){
		String result = m_input_head;
		for (U32 i = 0; i < input.size(); ++i){
			result += m_input_map[input[i]];
		}
		result += m_input_tail;
		return result;
	}

	String VSGenerator::generate_output_text(const String& input){
		String result = m_output_head;
		for (U32 i = 0; i < input.size(); ++i){
			result += m_output_map[input[i]];
		}
		result += m_output_tail;
		return result;
	}

	String VSGenerator::generate_header_text(const String& input, const String& output){
		return m_head;
	}

	String VSGenerator::generate_tail_text(const String& input, const String& output){
		return m_tail;
	}

	void VSGenerator::set_head(const String& s){
		m_head = s;
	}

	void VSGenerator::set_tail(const String& s){
		m_tail = s;
	}

	void VSGenerator::set_input_head(const String& s){
		m_input_head = s;
	}

	void VSGenerator::set_input_tail(const String& s){
		m_input_tail = s;
	}

	void VSGenerator::set_output_head(const String& s){
		m_output_head = s;
	}

	void VSGenerator::set_output_tail(const String& s){
		m_output_tail = s;
	}

	void VSGenerator::set_input_map(char c, const String& s){
		m_input_map[c] = s;
	}

	void VSGenerator::set_output_map(char c, const String& s){
		m_output_map[c] = s;
	}

	void VSGenerator::set_extension(const String& s){
		m_extension = s;
	}
}