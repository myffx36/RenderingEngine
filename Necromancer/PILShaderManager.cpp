#include "stdafx.h"
#include "PILShaderManager.h"

#include "VSGenerator.h"
#include "FSGenerator.h"
#include "VSProcess.h"
#include "FSProcess.h"
#include "VSDeclaration.h"
#include "FSDeclaration.h"

namespace Necromancer{
	PILShaderManager::PILShaderManager()
	{
		m_vs_generator = new VSGenerator();
		m_fs_generator = new FSGenerator();


		tinyxml2::XMLDocument doc;
		doc.LoadFile("XMLFile.xml");
		auto fragments_doc = doc.FirstChildElement("fragments");
		
		//vsutility
		auto vs_utility_doc = fragments_doc->FirstChildElement("vsutility");
		auto vs_head_doc = vs_utility_doc->FirstChildElement("vshead");
		m_vs_generator->set_head(vs_head_doc->GetText());
		auto vs_tail_doc = vs_utility_doc->FirstChildElement("vstail");
		m_vs_generator->set_tail(vs_tail_doc->GetText());
		auto vs_inputs_doc = vs_utility_doc->FirstChildElement("vsinputs");
		auto vs_input_head_doc = vs_inputs_doc->FirstChildElement("vsinputhead");
		m_vs_generator->set_input_head(vs_input_head_doc->GetText());
		auto vs_input_tail_doc = vs_inputs_doc->FirstChildElement("vsinputtail");
		m_vs_generator->set_input_tail(vs_input_tail_doc->GetText());
		auto vs_input_doc = vs_inputs_doc->FirstChildElement("vsinput");
		while (vs_input_doc){
			char c = vs_input_doc->Attribute("format")[0];
			m_vs_generator->set_input_map(c, vs_input_doc->GetText());
			vs_input_doc = vs_input_doc->NextSiblingElement("vsinput");
		}
		auto vs_outputs_doc = vs_utility_doc->FirstChildElement("vsoutputs");
		auto vs_output_head_doc = vs_outputs_doc->FirstChildElement("vsoutputhead");
		m_vs_generator->set_output_head(vs_output_head_doc->GetText());
		auto vs_output_tail_doc = vs_outputs_doc->FirstChildElement("vsoutputtail");
		m_vs_generator->set_output_tail(vs_output_tail_doc->GetText());
		auto vs_output_doc = vs_outputs_doc->FirstChildElement("vsoutput");
		while (vs_output_doc){
			char c = vs_output_doc->Attribute("format")[0];
			m_vs_generator->set_output_map(c,
				vs_output_doc->GetText());
			vs_output_doc = vs_output_doc->NextSiblingElement("vsoutput");
		}

		//fsutility
		auto fs_utility_doc = fragments_doc->FirstChildElement("fsutility");
		auto fs_tail_doc = fs_utility_doc->FirstChildElement("fstail");
		m_fs_generator->set_tail(fs_tail_doc->GetText());
		auto fs_head_doc = fs_utility_doc->FirstChildElement("fshead");
		auto fs_head_common_doc = fs_head_doc->FirstChildElement("common");
		m_fs_generator->set_head_common(fs_head_common_doc->GetText());
		auto fs_head_context_doc = fs_head_doc->FirstChildElement("context");
		while (fs_head_context_doc){
			char c = fs_head_context_doc->Attribute("input")[0];
			m_fs_generator->set_head_map(c, fs_head_context_doc->GetText());
			fs_head_context_doc = fs_head_context_doc->NextSiblingElement("context");
		}
		auto fs_output_doc = fs_utility_doc->FirstChildElement("fsoutput");
		auto fs_output_head_doc = fs_output_doc->FirstChildElement("head");
		m_fs_generator->set_output_head(fs_output_head_doc->GetText());
		auto fs_output_tail_doc = fs_output_doc->FirstChildElement("tail");
		m_fs_generator->set_output_tail(fs_output_tail_doc->GetText());
		auto fs_output_context_doc = fs_output_doc->FirstChildElement("context");
		m_fs_generator->set_output_mode(fs_output_context_doc->GetText());
		auto fs_input_doc = fs_utility_doc->FirstChildElement("fsinput");
		auto fs_input_head_doc = fs_input_doc->FirstChildElement("head");
		m_fs_generator->set_input_head(fs_input_head_doc->GetText());
		auto fs_input_tail_doc = fs_input_doc->FirstChildElement("tail");
		m_fs_generator->set_input_tail(fs_input_tail_doc->GetText());
		auto fs_input_context_doc = fs_input_doc->FirstChildElement("context");
		while (fs_input_context_doc){
			char c = fs_input_context_doc->Attribute("input")[0];
			m_fs_generator->set_input_map(c, 
				fs_input_context_doc->GetText());
			fs_input_context_doc = 
				fs_input_context_doc->NextSiblingElement("context");
		}

		//load vs declaration
		auto vs_declarations_doc = fragments_doc->FirstChildElement("vsdeclarations");
		auto vs_declaration_doc = vs_declarations_doc->FirstChildElement("vsdeclaration");
		while (vs_declaration_doc){
			unsigned int id = vs_declaration_doc->UnsignedAttribute("id");
			VSDeclaration* vs_declaration = new VSDeclaration();
			vs_declaration->set_uid(id);
			vs_declaration->set_name(vs_declaration_doc->Attribute("name"));
			vs_declaration->set_declaration_string(
				vs_declaration_doc->GetText());
			m_vs_generator->attach_vs_declaration(vs_declaration);
			vs_declaration_doc = vs_declaration_doc->NextSiblingElement("vsdeclaration");
		}

		//load vs process
		auto vs_fragments_doc = fragments_doc->FirstChildElement("vsfragments");
		auto vs_fragment_doc = vs_fragments_doc->FirstChildElement("vsfragment");
		while (vs_fragment_doc){
			unsigned int id = vs_fragment_doc->UnsignedAttribute("id");
			unsigned int priority = vs_fragment_doc->UnsignedAttribute("priority");
			VSProcess* vs_process = new VSProcess();
			vs_process->set_uid(id);
			vs_process->set_priority(priority);
			auto vs_attach_dec_doc = vs_fragment_doc->FirstChildElement("vsattachdec");
			while (vs_attach_dec_doc){
				vs_process->attach_uid(vs_attach_dec_doc->UnsignedAttribute("id"));
				vs_attach_dec_doc = vs_attach_dec_doc->NextSiblingElement("vsattachdec");
			}
			auto vs_context_doc = vs_fragment_doc->FirstChildElement("vscontext");
			while (vs_context_doc){
				String input(vs_context_doc->Attribute("input"));
				String output(vs_context_doc->Attribute("output"));
				vs_process->set_output_format(input, output);
				vs_process->attach_process_string(input, vs_context_doc->GetText());
				vs_context_doc = vs_context_doc->NextSiblingElement("vscontext");
			}
			m_vs_generator->attach_vs_process(vs_process);
			vs_fragment_doc = vs_fragment_doc->NextSiblingElement("vsfragment");
		}

		//load fs declaration
		auto fs_declarations_doc = fragments_doc->FirstChildElement("fsdeclarations");
		auto fs_declaration_doc = fs_declarations_doc->FirstChildElement("fsdeclaration");
		while (fs_declaration_doc){
			FSDeclaration* fd = new FSDeclaration();
			fd->set_name(fs_declaration_doc->Attribute("name"));
			fd->set_uid(fs_declaration_doc->UnsignedAttribute("id"));
			fd->set_declaration_string(fs_declaration_doc->GetText());
			fs_declaration_doc = fs_declaration_doc->NextSiblingElement("fsdeclaration");
			m_fs_generator->attach_fs_declaration(fd);
		}

		//load fs process
		auto fs_fragments_doc = fragments_doc->FirstChildElement("fsfragments");
		auto fs_fragment_doc = fs_fragments_doc->FirstChildElement("fsfragment");
		while (fs_fragment_doc){
			FSProcess* fp = new FSProcess();
			fp->set_priority(fs_fragment_doc->UnsignedAttribute("priority"));
			fp->set_uid(fs_fragment_doc->UnsignedAttribute("id"));
			fp->set_input_format(fs_fragment_doc->Attribute("input"));
			fp->set_output_num(fs_fragment_doc->UnsignedAttribute("outputnum"));
			auto fs_attach_dec_doc = fs_fragment_doc->FirstChildElement("fsattachdec");
			while (fs_attach_dec_doc){
				fp->attach_uid(fs_attach_dec_doc->UnsignedAttribute("id"));
				fs_attach_dec_doc = fs_attach_dec_doc->NextSiblingElement("fsattachdec");
			}
			auto fs_fragment_context_doc = fs_fragment_doc->FirstChildElement("context");
			fp->attach_process_string(
				fs_fragment_doc->Attribute("input"), 
				fs_fragment_context_doc->GetText());
			fs_fragment_doc = fs_fragment_doc->NextSiblingElement("fsfragment");
			m_fs_generator->attach_fs_process(fp);
		}
	}

	PILShaderManager::~PILShaderManager(){
		delete m_vs_generator;
		delete m_fs_generator;
	}

	VSPtr PILShaderManager::create_vertex_shader(const char* vs_code, const char* format){
		return m_vs_generator->generate_vertex_shader(vs_code, format);
	}

	FSPtr PILShaderManager::create_fragment_shader(const char* fs_code, const char* format){
		return m_fs_generator->generate_fragment_shader(fs_code, format);
	}

	void PILShaderManager::attach_vs_process(VSProcess* vs_process){
		m_vs_generator->attach_vs_process(vs_process);
	}

	void PILShaderManager::attach_fs_process(FSProcess* fs_process){
		m_fs_generator->attach_fs_process(fs_process);
	}

	void PILShaderManager::attach_vs_declaration(
		VSDeclaration* vs_declaration)
	{
		m_vs_generator->attach_vs_declaration(vs_declaration);
	}

	void PILShaderManager::attach_fs_declaration(
		FSDeclaration* fs_declaration)
	{
		m_fs_generator->attach_fs_declaration(fs_declaration);
	}
}