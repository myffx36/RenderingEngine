#include "stdafx.h"

#include "Terrain.h"

const float height_base = 200.0f;

namespace Necromancer {


	Terrain::Terrain(const char* file_name, ID3D11Device* device)
	{
		std::fstream terrain_file;
		terrain_file.open(file_name, std::ios::binary | std::ios::in);
		terrain_file.seekg(0, std::ios::beg);
		auto start_point = terrain_file.tellg();
		terrain_file.seekg(0, std::ios::end);
		auto end_point = terrain_file.tellg();
		unsigned long file_size = end_point;
		m_terrain_size = sqrt(file_size / sizeof(float));
		unsigned long data_size = file_size / sizeof(float);
		m_data = new float[data_size];
		terrain_file.seekg(0, std::ios::beg);
		terrain_file.read(reinterpret_cast<char*>(m_data), file_size);

		terrain_file.close();

		//CImage image;
		//const size_t cSize = strlen(file_name) + 1;
		//wchar_t* wc = new wchar_t[cSize];
		//mbstowcs(wc, file_name, cSize);
		//image.Load(wc);
		//delete[] wc;
		//m_terrain_size = image.GetHeight();
		//m_data = new float[image.GetHeight() * image.GetWidth()];
		//for (int i = 0; i < image.GetWidth(); ++i) {
		//	for (int j = 0; j < image.GetHeight(); ++j) {
		//		int index = i * image.GetHeight() + j;
		//		m_data[index] = (GetRValue(image.GetPixel(i, j))) / 255.0f;
		//	}
		//}

		m_position = nullptr;
		m_normal = nullptr;
		m_indices = nullptr;

		generate_position();
		generate_indices();
		generate_normal();
		generate_vb_ib(device);
	}


	Terrain::~Terrain()
	{
		delete[] m_data;
		if (m_position) delete[] m_position;
		if (m_normal) delete[] m_normal;
		if (m_indices) delete[] m_indices;
	}

	void Terrain::generate_position() {
		m_position = new Vec3[m_terrain_size * m_terrain_size];
		for (unsigned long i = 0; i < m_terrain_size; ++i) {
			for (unsigned long j = 0; j < m_terrain_size; ++j) {
				unsigned long offset = (i * m_terrain_size + j);
				m_position[offset][0] = static_cast<float>(i);
				m_position[offset][1] = height_base * m_data[i * m_terrain_size + j];
				m_position[offset][2] = static_cast<float>(j);
			}
		}
	}

	void Terrain::generate_normal() {
		m_normal = new Vec3[m_terrain_size * m_terrain_size];
		for (unsigned long i = 0; i < m_terrain_size * m_terrain_size; ++i) {
			m_normal[i] = Vec3(0.0f, 0.0f, 0.0f);
		}
		unsigned long triangle_num = (m_terrain_size - 1) * (m_terrain_size - 1) * 2;
		for (unsigned long i = 0; i < triangle_num; ++i) {
			unsigned long index_offset = i * 3;
			unsigned long i1 = m_indices[index_offset];
			unsigned long i2 = m_indices[index_offset + 1];
			unsigned long i3 = m_indices[index_offset + 2];

			Vec3 p1 = m_position[i1];
			Vec3 p2 = m_position[i2];
			Vec3 p3 = m_position[i3];

			Vec3 l1 = p2 - p1;
			Vec3 l2 = p3 - p2;
			Vec3 normal = normalize(cross(l2, l1));

			m_normal[i1] = m_normal[i1] + normal;
			m_normal[i2] = m_normal[i2] + normal;
			m_normal[i3] = m_normal[i3] + normal;
		}
		for (unsigned long i = 0; i < m_terrain_size * m_terrain_size; ++i) {
			m_normal[i] = normalize(m_normal[i]);
		}
	}

	void Terrain::generate_indices() {
		unsigned long triangle_num = (m_terrain_size - 1) * (m_terrain_size - 1) * 2;
		m_index_number = triangle_num * 3;
		m_indices = new unsigned long[m_index_number];
		for (unsigned long i = 0; i < m_terrain_size - 1; ++i) {
			for (unsigned long j = 0; j < m_terrain_size - 1; ++j) {
				unsigned long offset = (i * (m_terrain_size - 1) + j) * 6;
				unsigned long left_bottom_index = i * m_terrain_size + j;
				unsigned long left_top_index = left_bottom_index + 1;
				unsigned long right_bottom_index = left_bottom_index + m_terrain_size;
				unsigned long right_top_index = right_bottom_index + 1;
				m_indices[offset + 0] = left_bottom_index;
				m_indices[offset + 1] = right_bottom_index;
				m_indices[offset + 2] = left_top_index;
				m_indices[offset + 3] = left_top_index;
				m_indices[offset + 4] = right_bottom_index;
				m_indices[offset + 5] = right_top_index;
			}
		}
	}

	void Terrain::generate_vb_ib(ID3D11Device* d3d_device)
	{
		float *vertices = new float[8 * m_terrain_size * m_terrain_size];
		for (unsigned long i = 0; i < m_terrain_size * m_terrain_size; ++i) {
			unsigned long vertices_offset = i * 8;
			vertices[vertices_offset + 0] = m_position[i].x;
			vertices[vertices_offset + 1] = m_position[i].y;
			vertices[vertices_offset + 2] = m_position[i].z;
			vertices[vertices_offset + 3] = m_normal[i].x;
			vertices[vertices_offset + 4] = m_normal[i].y;
			vertices[vertices_offset + 5] = m_normal[i].z;
			vertices[vertices_offset + 6] = 0.0f;
			vertices[vertices_offset + 7] = 0.0f;
		}

		D3D11_BUFFER_DESC vb_desc;
		ZeroMemory(&vb_desc, sizeof(vb_desc));
		vb_desc.ByteWidth = 8 * m_terrain_size * m_terrain_size * sizeof(float);
		vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vb_desc.CPUAccessFlags = 0;
		vb_desc.Usage = D3D11_USAGE_DEFAULT;
		vb_desc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA vb_init_data;
		vb_init_data.pSysMem = vertices;
		vb_init_data.SysMemPitch = 0;
		vb_init_data.SysMemSlicePitch = 0;
		HRESULT hr = d3d_device->CreateBuffer(&vb_desc, &vb_init_data, &m_vertex_buffer);
		delete[]vertices;

		D3D11_BUFFER_DESC ib_desc;
		ZeroMemory(&vb_desc, sizeof(ib_desc));
		ib_desc.ByteWidth = m_index_number * sizeof(unsigned long);
		ib_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ib_desc.CPUAccessFlags = 0;
		ib_desc.Usage = D3D11_USAGE_DEFAULT;
		ib_desc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA ib_init_data;
		ib_init_data.pSysMem = m_indices;
		ib_init_data.SysMemPitch = 0;
		ib_init_data.SysMemSlicePitch = 0;
		hr = d3d_device->CreateBuffer(&ib_desc, &ib_init_data, &m_index_buffer);

		char* vs_data;
		unsigned long vs_data_size;
		std::ifstream vs_file("VertexShader.cso", std::ios::binary | std::ios::ate);
		vs_data_size = static_cast<unsigned long>(vs_file.tellg());
		vs_file.seekg(std::ios::beg);
		vs_data = new char[vs_data_size];
		vs_file.read(vs_data, vs_data_size);
		vs_file.close();

		hr = d3d_device->CreateVertexShader(vs_data, vs_data_size, nullptr, &m_vertex_shader);

		char* ps_data;
		unsigned long ps_data_size;
		std::ifstream ps_file("PixelShader.cso", std::ios::binary | std::ios::ate);
		ps_data_size = static_cast<unsigned long>(ps_file.tellg());
		ps_file.seekg(std::ios::beg);
		ps_data = new char[ps_data_size];
		ps_file.read(ps_data, ps_data_size);
		ps_file.close();

		hr = d3d_device->CreatePixelShader(ps_data, ps_data_size, nullptr, &m_pixel_shader);


		D3D11_INPUT_ELEMENT_DESC input_layout_desc[3];

		input_layout_desc[0].SemanticName = "POSITION";
		input_layout_desc[0].SemanticIndex = 0;
		input_layout_desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		input_layout_desc[0].InputSlot = 0;
		input_layout_desc[0].AlignedByteOffset = 0;
		input_layout_desc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		input_layout_desc[0].InstanceDataStepRate = 0;

		input_layout_desc[1].SemanticName = "NORMAL";
		input_layout_desc[1].SemanticIndex = 0;
		input_layout_desc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		input_layout_desc[1].InputSlot = 0;
		input_layout_desc[1].AlignedByteOffset = 3 * sizeof(float);
		input_layout_desc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		input_layout_desc[1].InstanceDataStepRate = 0;

		input_layout_desc[2].SemanticName = "TEXCOORD";
		input_layout_desc[2].SemanticIndex = 0;
		input_layout_desc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		input_layout_desc[2].InputSlot = 0;
		input_layout_desc[2].AlignedByteOffset = 6 * sizeof(float);
		input_layout_desc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		input_layout_desc[2].InstanceDataStepRate = 0;

		hr = d3d_device->CreateInputLayout(input_layout_desc, 3,
			vs_data, vs_data_size, &m_input_layout);
	}
}