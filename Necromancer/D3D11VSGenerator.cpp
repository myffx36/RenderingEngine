#include "stdafx.h"
#include "D3D11VSGenerator.h"

#include "VSProcess.h"
#include "VSDeclaration.h"

namespace Necromancer{
	namespace {
		VSProcess* create_perspective_vs_process(){
			VSProcess* perspective_projection_vs = new VSProcess();

			String perspective_declartion_str = 
				"cbuffer matrices:register(b0){\
				\n\tfloat4x4 world;\n\tfloat4x4 view;\n\tfloat4x4 projection;\n}";

			String pna2_perspective_process_str = "\tresult.t_pos = float4(input.position, 1.0);\n" ;
			pna2_perspective_process_str += "\tresult.t_pos = mul(world, result.t_pos);\n";
			pna2_perspective_process_str += "\tresult.position = result.t_pos.xyz / result.t_pos.w;\n";
			pna2_perspective_process_str += "\tresult.t_pos = mul(view, result.t_pos);\n";
			pna2_perspective_process_str += "\tresult.t_pos = mul(projection, result.t_pos);\n";
			pna2_perspective_process_str += "\tresult.normal = mul(world, float4(input.normal, 0.0f)).xyz;";
			pna2_perspective_process_str += "\tresult.tangent = mul(world, float4(input.tangent, 0.0f)).xyz;";
			pna2_perspective_process_str += "\tresult.tex = input.tex;";
			String pn2_perspective_process_str = "\tresult.t_pos = float4(input.position, 1.0);\n" ;
			pn2_perspective_process_str += "\tresult.t_pos = mul(world, result.t_pos);\n";
			pn2_perspective_process_str += "\tresult.position = result.t_pos.xyz / result.t_pos.w;\n";
			pn2_perspective_process_str += "\tresult.t_pos = mul(view, result.t_pos);\n" ;
			pn2_perspective_process_str += "\tresult.t_pos = mul(projection, result.t_pos);\n";
			pn2_perspective_process_str += "\tresult.normal = mul(world, float4(input.normal, 0.0f)).xyz;";
			pn2_perspective_process_str += "\tresult.tex = input.tex;";
			String pn_perspective_process_str = "\tresult.t_pos = float4(input.position, 1.0);\n" ;
			pn_perspective_process_str += "\tresult.t_pos = mul(world, result.t_pos);\n";
			pn_perspective_process_str += "\tresult.position = result.t_pos.xyz / result.t_pos.w;\n";
			pn_perspective_process_str += "\tresult.t_pos = mul(view, result.t_pos);\n" ;
			pn_perspective_process_str += "\tresult.t_pos = mul(projection, result.t_pos);\n" ;
			pn_perspective_process_str += "\tresult.normal = mul(world, float4(input.normal, 0.0f)).xyz;";
			String p_perspective_process_str = "\tresult.t_pos = float4(input.position, 1.0);\n";
			p_perspective_process_str += "\tresult.t_pos = mul(world, result.t_pos);\n";
			p_perspective_process_str += "\tresult.position = result.t_pos.xyz / result.t_pos.w;\n";
			p_perspective_process_str += "\tresult.t_pos = mul(view, result.t_pos);\n";
			p_perspective_process_str += "\tresult.t_pos = mul(projection, result.t_pos);";
			String pc_perspective_process_str = "\tresult.t_pos = float4(input.position, 1.0);\n";
			pc_perspective_process_str += "\tresult.t_pos = mul(world, result.t_pos);\n";
			pc_perspective_process_str += "\tresult.position = result.t_pos.xyz / result.t_pos.w;\n";
			pc_perspective_process_str += "\tresult.t_pos = mul(view, result.t_pos);\n";
			pc_perspective_process_str += "\tresult.t_pos = mul(projection, result.t_pos);";
			pc_perspective_process_str += "\tresult.color = mul(projection, result.color);";

			perspective_projection_vs->set_uid(perspective_projection_vs_uid);
			perspective_projection_vs->attach_process_string("PNA2", pna2_perspective_process_str);
			perspective_projection_vs->attach_process_string("PN2", pn2_perspective_process_str);
			perspective_projection_vs->attach_process_string("PN", pn_perspective_process_str);
			perspective_projection_vs->attach_process_string("PC", pc_perspective_process_str);
			perspective_projection_vs->attach_process_string("P", p_perspective_process_str);

			perspective_projection_vs->set_output_format("PNA2", "PNA2T");
			perspective_projection_vs->set_output_format("PN2", "PN2T");
			perspective_projection_vs->set_output_format("PN", "PNT");
			perspective_projection_vs->set_output_format("PC", "PTC");
			perspective_projection_vs->set_output_format("P", "PT");

			perspective_projection_vs->attach_uid(matrices_declaration_uid);

			return perspective_projection_vs;
		}

		VSDeclaration* create_matrix_declaration(){

			VSDeclaration* matrices_dec = new VSDeclaration();

			String perspective_declartion_str = 
				"cbuffer matrices:register(b0){\
				\n\tfloat4x4 world;\n\tfloat4x4 view;\n\tfloat4x4 projection;\n}";
			matrices_dec->set_declaration_string(perspective_declartion_str);
			matrices_dec->set_uid(matrices_declaration_uid);
			matrices_dec->set_name("matrices_declaration");

			return matrices_dec;
		}

		VSDeclaration* create_skeleton_declaration(){

			VSDeclaration* matrices_dec = new VSDeclaration();

			String perspective_declartion_str = "";
			perspective_declartion_str += "cbuffer skeleton_matrix:register(b5){\n";
			perspective_declartion_str += "\tfloat4x4 matrices[128];\n";
			perspective_declartion_str += "}\n";
			perspective_declartion_str += "struct SkinedInfo{\n";
			perspective_declartion_str += "\tfloat4 position;\n";
			perspective_declartion_str += "\tfloat3 normal;\n";
			perspective_declartion_str += "\t//float3 tangent;\n";
			perspective_declartion_str += "};\n";
			perspective_declartion_str += "\n";
			perspective_declartion_str += "SkinedInfo caculate_skining(VSInput input){\n";
			perspective_declartion_str += "\tSkinedInfo info = (SkinedInfo)0;\n";
			perspective_declartion_str += "\tfloat4 position = float4(input.position, 1.0f);\n";
			perspective_declartion_str += "\t//position = mul(world, position);\n";
			perspective_declartion_str += "\t//float4 position = float4(0.0f, 0.0f, 0.0f, 1.0f);\n";
			perspective_declartion_str += "\tfloat3 normal = input.normal;\n";
			perspective_declartion_str += "\t//float3 tangent = input.tangent;\n";
			perspective_declartion_str += "\tfloat4x4 mat;\n";
			perspective_declartion_str += "\tfloat3x3 normal_t;\n";
			perspective_declartion_str += "\n";
			perspective_declartion_str += "\tfor(int i = 0;i < 4;++ i){\n";
			//perspective_declartion_str += "\t\tif(input.skeleton_id[i] >= 128) continue;\n";
			//perspective_declartion_str += "\t\tif(input.skeleton_id[i] == 0) continue;\n";
			perspective_declartion_str += "\t\tmat = matrices[input.skeleton_id[i]];\n";
			perspective_declartion_str += "\t\tinfo.position	+= mul(mat, position) * input.skeleton_weights[i];\n";
			perspective_declartion_str += "\t\tnormal_t += (float3x3)mat * input.skeleton_weights[i];\n";
			perspective_declartion_str += "\t}\n";
			perspective_declartion_str += "\n";
			perspective_declartion_str += "\tinfo.normal = normal;\n";
			perspective_declartion_str += "\t//info.tangent = mul(normal_t, tangent);\n";
			perspective_declartion_str += "\n";
			perspective_declartion_str += "\t/*info.position = position;\n";
			perspective_declartion_str += "\tinfo.normal = normal;\n";
			perspective_declartion_str += "\t//info.tangent = tangent;*/\n";
			perspective_declartion_str += "\n";
			perspective_declartion_str += "\treturn info;\n";
			perspective_declartion_str += "}\n";
			matrices_dec->set_declaration_string(perspective_declartion_str);
			matrices_dec->set_uid(skeleton_declaration_uid);
			matrices_dec->set_name("matrices_declaration");

			return matrices_dec;
		}

		VSProcess* create_skinning_vs_process(){
			VSProcess* skining_transform_vs = new VSProcess;

			String process_str = "";
			process_str += "\tSkinedInfo info = caculate_skining(input);\n";
			process_str += "\tinput.position = info.position.xyz / info.position.w;\n";

			skining_transform_vs->set_uid(skining_transform_vs_uid);
			skining_transform_vs->set_output_format("PN2S", "PN2TS");
			skining_transform_vs->attach_process_string("PN2S", process_str);
			skining_transform_vs->attach_uid(skeleton_declaration_uid);

			return skining_transform_vs;
		}
	}

	D3D11VSGenerator::D3D11VSGenerator(){
		init_build_in_vs();
	}

	D3D11VSGenerator::~D3D11VSGenerator(){

	}

	String D3D11VSGenerator::generate_input_text(const String& input){
		String result = "struct VSInput{\n";

		if(input.find('P') != -1){
			result += "\tfloat3 position:POSITION;\n";
		}

		if(input.find('N') != -1){
			result += "\tfloat3 normal:NORMAL;\n";
		}

		if(input.find('A') != -1){
			result += "\tfloat3 tangent:TANGENT;\n";
		}

		if (input.find('C') != -1){
			result += "\tfloat4 color:COLOR;\n";
		}

		if(input.find('2') != -1){
			result += "\tfloat2 tex:TEXCOORD0;\n";
		}

		if(input.find('S') != -1){
			result += "\tuint4 skeleton_id : SKELETON; \n";
			result += "\tfloat4 skeleton_weights : WEIGHTS; \n";
		}

		result += "};\n";

		return result;
	}

	String D3D11VSGenerator::generate_output_text(const String& output){

		String result = "struct PSInput{\n";

		if(output.find('P') != -1){
			result += "\tfloat3 position:POSITION;\n";
		}

		if(output.find('N') != -1){
			result += "\tfloat3 normal:NORMAL;\n";
		}

		if(output.find('A') != -1){
			result += "\tfloat3 tangent:TANGENT;\n";
		}

		if(output.find('2') != -1){
			result += "\tfloat2 tex:TEXCOORD0;\n";
		}

		if(output.find('T') != -1){
			result += "\tfloat4 t_pos:SV_POSITION;\n";
		}

		if (output.find('C') != -1){
			result += "\tfloat4 color:COLOR;\n";
		}

		result += "};\n";

		return result;
	}

	String D3D11VSGenerator::generate_header_text(const String& input, const String& output){
		String result;
		result += "PSInput main(VSInput input){\n";
		result += "\tPSInput result = (PSInput)0;\n";

		return result;
	}

	String D3D11VSGenerator::generate_tail_text(const String& input, const String& output){
		String result = "\treturn result;\n}\n";
		return result;
	}

	void D3D11VSGenerator::init_build_in_vs(){
		VSDeclaration* matrices_dec = create_matrix_declaration();
		VSDeclaration* skeleton_dec = create_skeleton_declaration();
		VSProcess* perspective_projection_vs = create_perspective_vs_process();
		VSProcess* skining_transform_vs = create_skinning_vs_process();

		attach_vs_declaration(matrices_dec);
		attach_vs_declaration(skeleton_dec);
		attach_vs_process(perspective_projection_vs);
		attach_vs_process(skining_transform_vs);
	}

	String D3D11VSGenerator::save_debug_file(const String& vs_code, const String& vs_text){
		String file_name = "shaders\\" + vs_code + "_vs.hlsl";

		std::ofstream out_file(file_name);
		out_file << vs_text << std::endl;
		out_file.close();


		return file_name;
	}
}