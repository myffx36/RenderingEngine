#pragma once

#include "BasicMath.h"

namespace Necromancer {
	class Terrain
	{
	public:
		Terrain(const char* file_name,
			ID3D11Device* device);
		~Terrain();
	private:
		void generate_position();
		void generate_normal();
		void generate_indices();
		void generate_vb_ib(ID3D11Device* d3d_device);
	private:
		float* m_data;
		Necromancer::Vec3* m_position;
		Necromancer::Vec3* m_normal;
		unsigned long m_terrain_size;
		unsigned long* m_indices;
		unsigned long m_index_number;
		ID3D11Buffer* m_vertex_buffer;
		ID3D11Buffer* m_index_buffer;
		ID3D11InputLayout* m_input_layout;
		ID3D11VertexShader* m_vertex_shader;
		ID3D11PixelShader* m_pixel_shader;
	};

}