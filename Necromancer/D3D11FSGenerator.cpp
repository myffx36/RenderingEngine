#include "stdafx.h"
#include "D3D11FSGenerator.h"

#include "FSProcess.h"
#include "FSDeclaration.h"

namespace Necromancer{
	namespace{
		FSProcess* create_vertex_color_fs_process(){
			FSProcess* fs_process = new FSProcess();

			fs_process->set_uid(vertex_color_fs_uid);
			fs_process->set_input_format("TC");
			fs_process->attach_process_string("TC",
				"\tresult.rt0 = input.color;\n");
			fs_process->set_output_num(1);

			return fs_process;
		}

		FSDeclaration* create_diffuse_texture_desc(){
			String str = "";
			str += "Texture2D<float4> texture_slot0 : register(t0); \n";
			str += "SamplerState default_sampler : register(s0); \n";

			FSDeclaration* fs_declaration = new FSDeclaration();
			fs_declaration->set_declaration_string(str);
			fs_declaration->set_uid(diffuse_texture_desc_uid);
			fs_declaration->set_name("DIFFUSE_TEXTURE_DESC");

			return fs_declaration;
		}

		FSProcess* create_diffuse_texture_process(){
			String str = "";
			str += "\tresult.rt0 = texture_slot0.Sample(default_sampler, input.tex);\n";
			FSProcess* fs_process = new FSProcess();
			fs_process->attach_uid(texture_maping_fs_uid);
			fs_process->set_input_format("P2");
			fs_process->attach_process_string("P2", str);
			fs_process->set_output_num(1);

			return fs_process;
		}
	}

	D3D11FSGenerator::D3D11FSGenerator(){
		init_buid_in_fs();
	}

	D3D11FSGenerator::~D3D11FSGenerator(){

	}

	String D3D11FSGenerator::generate_input_text(const String& input){
		String result = "struct PSInput{\n";

		if (input.find('P') != -1){
			result += "\tfloat3 position:POSITION;\n";
		}

		if (input.find('N') != -1){
			result += "\tfloat3 normal:NORMAL;\n";
		}

		if (input.find('A') != -1){
			result += "\tfloat3 tangent:TANGENT;\n";
		}

		if (input.find('2') != -1){
			result += "\tfloat2 tex:TEXCOORD0;\n";
		}

		if (input.find('T') != -1){
			result += "\tfloat4 t_pos:SV_POSITION;\n";
		}

		if (input.find('C') != -1){
			result += "\tfloat4 color:COLOR;\n";
		}

		result += "};\n";

		return result;
	}

	String D3D11FSGenerator::generate_output_text(U32 output_num){
		String result = "struct PSOutput{\n";
		for (U32 i = 0; i < output_num; ++i){
			String num_str = std::to_string(i);
			result += "\tfloat4 rt" + num_str + ":SV_TARGET" + num_str + ";\n";
		}
		result += "};\n";
		return result;
	}

	String D3D11FSGenerator::generate_header_text(const String& input){
		String result;
		result += "PSOutput main(PSInput input){\n";
		result += "\tPSOutput result = (PSOutput)0;\n";

		if (input.find('P') != -1){
			result += "\tfloat3 position = input.position;\n";
		}

		if (input.find('N') != -1){
			result += "\tfloat3 normal = input.normal;\n";
		}

		if (input.find('A') != -1){
			result += "\tfloat3 tangent = input.tangent;\n";
		}

		if (input.find('2') != -1){
			result += "\tfloat2 tex = input.tex;\n";
		}

		if (input.find('T') != -1){
			result += "\tfloat4 t_pos = input.t_pos;\n";
		}

		if (input.find('C') != -1){
			result += "\tfloat4 color = input.color;\n";
		}
		return result;
	}

	String D3D11FSGenerator::generate_tail_text(const String& input){
		String result = "\treturn result;\n}\n";
		return result;
	}

	void D3D11FSGenerator::init_buid_in_fs(){
		FSDeclaration* fs_diffuse_texture_desc = create_diffuse_texture_desc();
		attach_fs_declaration(fs_diffuse_texture_desc);

		FSProcess* texture_maping_fs = create_diffuse_texture_process();
		FSProcess* white_fs = new FSProcess();
		FSProcess* red_fs = new FSProcess();

		texture_maping_fs->set_uid(texture_maping_fs_uid);
		white_fs->set_uid(white_fs_uid);
		white_fs->set_input_format("T");
		white_fs->attach_process_string("T",
			"\tresult.rt0 = float4(1.0f, 1.0f, 1.0f, 1.0f);\n");
		white_fs->set_output_num(1);
		red_fs->set_uid(red_fs_uid);
		red_fs->set_input_format("T");
		red_fs->attach_process_string("T",
			"\tresult.rt0 = float4(1.0f, 0.0f, 0.0f, 1.0f);\n");
		red_fs->set_output_num(1);

		attach_fs_process(texture_maping_fs);
		//attach_fs_process(lighting_fs);
		attach_fs_process(white_fs);
		attach_fs_process(red_fs);
	}


	String D3D11FSGenerator::save_debug_file(const String& fs_code, const String& fs_text){
		String file_name = "shaders\\" + fs_code + "_fs.hlsl";

		std::ofstream out_file(file_name);
		out_file << fs_text << std::endl;
		out_file.close();


		return file_name;
	}
}