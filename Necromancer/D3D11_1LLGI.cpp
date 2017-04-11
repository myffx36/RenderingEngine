#include "stdafx.h"
#include "D3D11_1LLGI.h"

#include "Exception.h"
#include "D3D11Pointers.h"
#include "Container.hpp"

namespace Necromancer{
	D3D11_1LLGI::D3D11_1LLGI(Configuration* configuration, HWND h_wnd){
		m_width = configuration->get_integer_value("Screen", "width");
		m_height = configuration->get_integer_value("Screen", "height");

		bool full_screen;
		if (configuration->get_value("Screen", "fullscreen") == "yes"){
			full_screen = true;
		}
		else{
			full_screen = false;
		}

		HRESULT hr = S_OK;
		RECT rc;
		GetClientRect(h_wnd, &rc);
		U32 width = rc.right - rc.left;
		U32 height = rc.bottom - rc.top;
		m_width = width;
		m_height = height;

		U32 create_device_flags = 0;
#ifdef _DEBUG
		create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		create_device_flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

		D3D_DRIVER_TYPE driver_types [] = {
			D3D_DRIVER_TYPE_HARDWARE
		};
		U32 driver_type_number = ARRAYSIZE(driver_types);

		D3D_FEATURE_LEVEL feature_levels [] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		U32 feature_level_number = ARRAYSIZE(feature_levels);

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT |
			DXGI_USAGE_UNORDERED_ACCESS;
		sd.OutputWindow = h_wnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		for (U32 i = 0; i < driver_type_number; ++i){
			hr = D3D11CreateDeviceAndSwapChain(nullptr, driver_types[i], nullptr,
				create_device_flags, feature_levels, feature_level_number,
				D3D11_SDK_VERSION, &sd, &m_swap_chain, &m_device,
				&m_feature_level, &m_context);

			if (SUCCEEDED(hr)) break;
		}

		//if (full_screen){
		//	IDXGIOutput* dxgi_output;
		//	hr = m_swap_chain->GetContainingOutput(&dxgi_output);

		//	UINT num;
		//	DXGI_MODE_DESC mode_descs[128];
		//	dxgi_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		//		0, &num, mode_descs);
		//	UINT x = 0;
		//	for (U32 i = 0; i < num; ++i){
		//		if ((mode_descs[i].Width == width) &&
		//			(mode_descs[i].Height == height))
		//		{
		//			x = i;
		//			break;
		//		}
		//	}

		//	m_swap_chain->ResizeTarget(&mode_descs[x]);
		//	m_swap_chain->SetFullscreenState(TRUE, dxgi_output);
		//	m_swap_chain->ResizeBuffers(0, width, height,
		//		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		//	dxgi_output->Release();
		//}

		//if (FAILED(hr))
		//	throw Exception("can't create d3d11 device");

		switch (m_feature_level){
		case D3D_FEATURE_LEVEL_11_1:
			m_vs_target = "vs_5_0";
			m_fs_target = "ps_5_0";
			m_gs_target = "gs_5_0";
			m_cs_target = "cs_5_0";
			break;
		case D3D_FEATURE_LEVEL_11_0:
			m_vs_target = "vs_5_0";
			m_fs_target = "ps_5_0";
			m_gs_target = "gs_5_0";
			m_cs_target = "cs_5_0";
			break;
		case D3D_FEATURE_LEVEL_10_1:
			m_vs_target = "vs_4_1";
			m_fs_target = "ps_4_1";
			m_gs_target = "gs_4_1";
			m_cs_target = "cs_4_1";
			break;
		case D3D_FEATURE_LEVEL_10_0:
			m_vs_target = "vs_4_0";
			m_fs_target = "ps_4_0";
			m_gs_target = "gs_4_0";
			m_cs_target = "cs_4_0";
			break;
		}

		ID3D11Texture2D* back_buffer = nullptr;
		hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D),
			(LPVOID*) &back_buffer);
		if (FAILED(hr))
			throw Exception("failed to create d3d11 back buffer");

		hr = m_device->CreateRenderTargetView(back_buffer,
			nullptr, &m_default_render_target);
		if (FAILED(hr)){
			throw Exception("failed to create d3d11 default RTV");
			back_buffer->Release();
		}
		hr = m_device->CreateUnorderedAccessView(back_buffer,
			nullptr, &m_rt_uav);
		if (FAILED(hr)){
			throw Exception("failed to create d3d11 default UAV");
			back_buffer->Release();
		}

		// Now we set up the Direct2D render target bitmap linked to the swapchain. 
		// Whenever we render to this bitmap, it is directly rendered to the 
		// swap chain associated with the window.

		back_buffer->Release();

		m_default_depth_stencil = create_depth_stencil(width, height);
		set_to_default_render_target();

		D3D11_SAMPLER_DESC sampler_desc;
		ZeroMemory(&sampler_desc, sizeof(sampler_desc));
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.MaxAnisotropy = 0;
		hr = m_device->CreateSamplerState(
			&sampler_desc, &m_default_sampler_state);
		if (FAILED(hr))
			throw Exception("failed to create default sampler state");
		m_context->VSSetSamplers(0, 1, &m_default_sampler_state);
		m_context->PSSetSamplers(0, 1, &m_default_sampler_state);
		m_context->GSSetSamplers(0, 1, &m_default_sampler_state);
		m_context->CSSetSamplers(0, 1, &m_default_sampler_state);

		//start to init dwrite

#ifdef _DEBUG
		m_device->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			11, "d3d_device");
		m_context->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			12, "d3d_context");
		m_swap_chain->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			15, "d3d_swap_chain");
		m_default_render_target->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			26, "d3d_default_render_target");
		m_default_sampler_state->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			26, "d3d_default_sampler_state");
#endif
	}

	D3D11_1LLGI::~D3D11_1LLGI(){
		/*IDXGIOutput* dxgi_output;
		m_swap_chain->GetContainingOutput(&dxgi_output);
		m_swap_chain->SetFullscreenState(FALSE, dxgi_output);*/


		static ID3D11SamplerState* null_array [] = { nullptr };
		static ID3D11RenderTargetView* null_rt_array [] = { nullptr };

		m_context->VSSetSamplers(0, 1, null_array);
		m_context->GSSetSamplers(0, 1, null_array);
		m_context->PSSetSamplers(0, 1, null_array);
		m_context->OMSetRenderTargets(1, null_rt_array, nullptr);
		m_default_depth_stencil = DSPtr(nullptr);

		std::for_each(m_input_layout_map.begin(), m_input_layout_map.end(),
			[](const std::pair<String, ID3D11InputLayout*>& pair)
		{
			pair.second->Release();
		});

		m_default_sampler_state->Release();
		m_default_render_target->Release();
		m_rt_uav->Release();
		m_context->Release();
		m_device->Release();

		m_swap_chain->Release();
	}

	void D3D11_1LLGI::enable_vertex_buffer(const VBPtr& vertex_buffer){
		ID3D11Buffer* d3d11_vertex_buffer = get_d3d11_pointer(vertex_buffer);
		U32 stride = vertex_buffer->stride();
		U32 offset = vertex_buffer->offset();
		m_context->IASetVertexBuffers(0, 1,
			&d3d11_vertex_buffer, &stride, &offset);
		set_vertex_format(vertex_buffer->format());
	}

	void D3D11_1LLGI::enable_index_buffer(const IBPtr& index_buffer){
		ID3D11Buffer* d3d11_index_buffer = get_d3d11_pointer(index_buffer);
		m_index_number = index_buffer->index_number();
		m_context->IASetIndexBuffer(d3d11_index_buffer,
			DXGI_FORMAT_R32_UINT, 0);
	}

	void D3D11_1LLGI::enable_constant_buffer(U32 slot,
		const CBPtr& constant_buffer, BindFlag flag)
	{
		ID3D11Buffer* d3d11_buffer = get_d3d11_pointer(constant_buffer);
		if (flag & BIND_TO_VS)
			m_context->VSSetConstantBuffers(slot,
			1, &d3d11_buffer);
		if (flag & BIND_TO_FS)
			m_context->PSSetConstantBuffers(slot,
			1, &d3d11_buffer);
		if (flag & BIND_TO_GS)
			m_context->GSSetConstantBuffers(slot,
			1, &d3d11_buffer);
		if (flag & BIND_TO_CS)
			m_context->CSSetConstantBuffers(slot,
			1, &d3d11_buffer);
		if (flag & BIND_TO_DS)
			m_context->DSSetConstantBuffers(slot,
			1, &d3d11_buffer);
		if (flag & BIND_TO_HS)
			m_context->HSSetConstantBuffers(slot,
			1, &d3d11_buffer);
	}

	void D3D11_1LLGI::disable_vertex_buffer(const VBPtr&){
		static U32 stride = 0;
		static U32 offset = 0;
		static ID3D11Buffer* array_buffer [] = {
			nullptr
		};
		m_context->IASetVertexBuffers(
			0, 1, array_buffer, &stride, &offset);
	}

	void D3D11_1LLGI::disable_index_buffer(const IBPtr&){
		m_context->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	}

	void D3D11_1LLGI::disable_constant_buffer(U32 slot,
		const CBPtr&, BindFlag flag)
	{
		static ID3D11Buffer* buffer_array [] = {
			nullptr
		};
		if (flag & BIND_TO_VS)
			m_context->VSSetConstantBuffers(slot,
			1, buffer_array);
		if (flag & BIND_TO_FS)
			m_context->PSSetConstantBuffers(slot,
			1, buffer_array);
		if (flag & BIND_TO_GS)
			m_context->GSSetConstantBuffers(slot,
			1, buffer_array);
		if (flag & BIND_TO_CS)
			m_context->CSSetConstantBuffers(slot,
			1, buffer_array);
		if (flag & BIND_TO_DS)
			m_context->DSSetConstantBuffers(slot,
			1, buffer_array);
		if (flag & BIND_TO_HS)
			m_context->HSSetConstantBuffers(slot,
			1, buffer_array);
	}

	void D3D11_1LLGI::release_vertex_buffer(const VBPtr& vertex_buffer){
		ID3D11Buffer* d3d11_buffer = get_d3d11_pointer(vertex_buffer);
		d3d11_buffer->Release();
	}

	void D3D11_1LLGI::release_index_buffer(const IBPtr& index_buffer){
		ID3D11Buffer* d3d11_buffer = get_d3d11_pointer(index_buffer);
		d3d11_buffer->Release();
	}

	void D3D11_1LLGI::release_constant_buffer(const CBPtr& constant_buffer){

		ID3D11Buffer* d3d11_buffer = get_d3d11_pointer(constant_buffer);
		d3d11_buffer->Release();
	}

	VBPtr D3D11_1LLGI::create_vertex_buffer(
		U32 size, U32 stride,
		U32 offset, const String& format,
		void *data,
		BufferUsage buffer_usage)
	{
		ID3D11Buffer* d3d11_buffer;
		D3D11_BUFFER_DESC buffer_desc;
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.ByteWidth = size;
		buffer_desc.CPUAccessFlags =
			to_d3d11_cpu_access_flag(buffer_usage);
		buffer_desc.Usage = to_d3d11_usage(buffer_usage);
		D3D11_SUBRESOURCE_DATA init_data;
		ZeroMemory(&init_data, sizeof(init_data));
		init_data.pSysMem = data;
		HRESULT hr = m_device->CreateBuffer(
			&buffer_desc, &init_data, &d3d11_buffer);
		if (FAILED(hr)) return nullptr;

		D3D11BOPointer*
			buffer_object_pointer =
			new D3D11BOPointer();
		buffer_object_pointer->set_d3d11_buffer(d3d11_buffer);
		VBPtr vertex_buffer(
			new VertexBuffer(size, stride, offset,
			format, buffer_usage));
		vertex_buffer->set_pointer(buffer_object_pointer);

		return vertex_buffer;
	}

	IBPtr D3D11_1LLGI::create_index_buffer(
		U32 index_number, void * data,
		BufferUsage buffer_usage)
	{
		int size = index_number * sizeof(unsigned long);
		ID3D11Buffer* d3d11_buffer;
		D3D11_BUFFER_DESC buffer_desc;
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.ByteWidth = size;
		buffer_desc.CPUAccessFlags =
			to_d3d11_cpu_access_flag(buffer_usage);
		buffer_desc.Usage = to_d3d11_usage(buffer_usage);
		D3D11_SUBRESOURCE_DATA init_data;
		ZeroMemory(&init_data, sizeof(init_data));
		init_data.pSysMem = data;
		HRESULT hr = m_device->CreateBuffer(
			&buffer_desc, &init_data, &d3d11_buffer);
		if (FAILED(hr)) return nullptr;

		D3D11BOPointer*
			buffer_object_pointer =
			new D3D11BOPointer();
		buffer_object_pointer->set_d3d11_buffer(d3d11_buffer);
		IBPtr index_buffer(new IndexBuffer(
			index_number, buffer_usage));
		index_buffer->set_pointer(buffer_object_pointer);
		return index_buffer;
	}

	CBPtr D3D11_1LLGI::create_constant_buffer(
		U32 size, const void *data)
	{
		ID3D11Buffer* d3d11_buffer;
		D3D11_BUFFER_DESC buffer_desc;
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.ByteWidth = size;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		if (data != nullptr){
			D3D11_SUBRESOURCE_DATA init_data;
			ZeroMemory(&init_data, sizeof(init_data));
			init_data.pSysMem = data;
			HRESULT hr = m_device->CreateBuffer(
				&buffer_desc, &init_data, &d3d11_buffer);
			if (FAILED(hr)) return nullptr;
		}
		else{
			HRESULT hr = m_device->CreateBuffer(
				&buffer_desc, nullptr, &d3d11_buffer);
			if (FAILED(hr)) return CBPtr(nullptr);
		}

		D3D11BOPointer*
			buffer_object_pointer =
			new D3D11BOPointer();
		buffer_object_pointer->set_d3d11_buffer(d3d11_buffer);
		CBPtr constant_buffer(
			new ConstantBuffer(size));
		constant_buffer->set_pointer(buffer_object_pointer);

		return constant_buffer;
	}

	void D3D11_1LLGI::update_constant_buffer_object(
		const CBPtr& cb, const void* data)
	{
		ID3D11Buffer* d3d11_buffer =
			get_d3d11_pointer(cb);
		m_context->UpdateSubresource(d3d11_buffer,
			0, nullptr, data, 0, 0);
	}

	void* D3D11_1LLGI::map(const VBPtr& vertex_buffer){
#ifdef _DEBUG
		if (vertex_buffer->buffer_usage() !=
			NM_BUFFER_USAGE_DYNAMIC)
			throw Exception("can't map resource");
#endif
		ID3D11Buffer* d3d11_buffer =
			get_d3d11_pointer(vertex_buffer);
		D3D11_MAP map_type =
			D3D11_MAP_WRITE_DISCARD;
		UINT map_flags = 0;
		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		HRESULT hr = m_context->Map(d3d11_buffer, 0,
			map_type, map_flags, &mapped_resource);
		if (FAILED(hr)) return nullptr;
		return mapped_resource.pData;
	}

	void* D3D11_1LLGI::map(const IBPtr& index_buffer){
#ifdef _DEBUG
		if (index_buffer->buffer_usage() !=
			NM_BUFFER_USAGE_DYNAMIC)
			throw Exception("can't map resource");
#endif
		ID3D11Buffer* d3d11_buffer =
			get_d3d11_pointer(index_buffer);
		D3D11_MAP map_type =
			D3D11_MAP_WRITE_DISCARD;
		UINT map_flags = 0;
		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		HRESULT hr = m_context->Map(d3d11_buffer, 0,
			map_type, map_flags, &mapped_resource);
		if (FAILED(hr)) return nullptr;
		return mapped_resource.pData;
	}

	void D3D11_1LLGI::unmap(const VBPtr& vertex_buffer){
		ID3D11Buffer* d3d11_buffer =
			get_d3d11_pointer(vertex_buffer);
		m_context->Unmap(d3d11_buffer, 0);
	}

	void D3D11_1LLGI::unmap(const IBPtr& index_buffer){
		ID3D11Buffer* d3d11_buffer =
			get_d3d11_pointer(index_buffer);
		m_context->Unmap(d3d11_buffer, 0);
	}

	void D3D11_1LLGI::enable_vertex_shader(const VSPtr& vertex_shader){
		ID3D11VertexShader* d3d11_vertex_shader =
			get_d3d11_pointer(vertex_shader);
		m_context->VSSetShader(d3d11_vertex_shader, nullptr, 0);
	}

	void D3D11_1LLGI::enable_fragment_shader(const FSPtr& fs){
		ID3D11PixelShader* d3d11_fragment_shader =
			get_d3d11_pointer(fs);
		m_context->PSSetShader(d3d11_fragment_shader, nullptr, 0);
	}

	void D3D11_1LLGI::disable_vertex_shader(const VSPtr&){
		m_context->VSSetShader(nullptr, nullptr, 0);
	}

	void D3D11_1LLGI::disable_fragment_shader(const FSPtr&){
		m_context->PSSetShader(nullptr, nullptr, 0);
	}

	void D3D11_1LLGI::release_vertex_shader(const VSPtr& vertex_shader){
		ID3D11VertexShader* d3d11_vertex_shader =
			get_d3d11_pointer(vertex_shader);
		d3d11_vertex_shader->Release();
	}

	void D3D11_1LLGI::release_fragment_shader(const FSPtr& fs){
		ID3D11PixelShader* d3d11_fragment_shader =
			get_d3d11_pointer(fs);
		d3d11_fragment_shader->Release();
	}

	VSPtr D3D11_1LLGI::create_vertex_shader(
		const char* shader_text,
		const char* file_name)
	{
		ID3D11VertexShader* d3d11_vertex_shader;
		ID3DBlob* shader_blob = nullptr;
		ID3DBlob* error_blob = nullptr;
		DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		shader_flags |= D3DCOMPILE_DEBUG;
#endif
		HRESULT hr = D3DCompile(shader_text,
			strlen(shader_text), file_name,
			nullptr, nullptr, "main", m_vs_target,
			shader_flags, 0, &shader_blob, &error_blob);
		if (error_blob != nullptr){
			OutputDebugStringA((char*) error_blob->GetBufferPointer());
			error_blob->Release();
		}
		if (FAILED(hr)) return nullptr;

		hr = m_device->CreateVertexShader(shader_blob->GetBufferPointer(),
			shader_blob->GetBufferSize(), nullptr, &d3d11_vertex_shader);
		shader_blob->Release();
		if (FAILED(hr)) return nullptr;

		D3D11VSPointer* vertex_shader_pointer =
			new D3D11VSPointer();
		vertex_shader_pointer->set_d3d11_vs(d3d11_vertex_shader);
		VSPtr vertex_shader(new VertexShader());
		vertex_shader->set_pointer(vertex_shader_pointer);
		return vertex_shader;
	}

	FSPtr D3D11_1LLGI::create_fragment_shader(
		const char* shader_text,
		const char* file_name)
	{
		ID3D11PixelShader* d3d11_fragment_shader;
		ID3DBlob* shader_blob = nullptr;
		ID3DBlob* error_blob = nullptr;
		DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		shader_flags |= D3DCOMPILE_DEBUG;
#endif
		HRESULT hr = D3DCompile(shader_text,
			strlen(shader_text), file_name,
			nullptr, nullptr, "main", m_fs_target,
			shader_flags, 0, &shader_blob, &error_blob);
		if (error_blob != nullptr){
			OutputDebugStringA((char*) error_blob->GetBufferPointer());
			error_blob->Release();
		}
		if (FAILED(hr)) return FSPtr((FragmentShader*)nullptr);

		hr = m_device->CreatePixelShader(
			shader_blob->GetBufferPointer(),
			shader_blob->GetBufferSize(), nullptr,
			&d3d11_fragment_shader);
		shader_blob->Release();
		if (FAILED(hr)) return FSPtr((FragmentShader*)nullptr);

		D3D11FSPointer* pointer = new D3D11FSPointer();
		pointer->set_d3d11_fs(d3d11_fragment_shader);
		FSPtr fragment_shader(new FragmentShader());
		fragment_shader->set_pointer(pointer);

		return fragment_shader;
	}

	VSPtr D3D11_1LLGI::create_vertex_shader_from_binary(
		void* binary_code, U32 binary_code_size,
		const char* file_name)
	{
		ID3D11VertexShader* d3d11_shader;
		HRESULT hr = m_device->CreateVertexShader(
			binary_code, binary_code_size, nullptr, &d3d11_shader);
		if (FAILED(hr)){
			return nullptr;
		}
		D3D11VSPointer* pointer = new D3D11VSPointer();
		pointer->set_d3d11_vs(d3d11_shader);
		VSPtr result(new VertexShader());
		result->set_pointer(pointer);
		return result;
	}

	FSPtr D3D11_1LLGI::create_fragment_shader_from_binary(
		void* binary_code, U32 binary_code_size,
		const char* file_name)
	{
		ID3D11PixelShader* d3d11_shader;
		HRESULT hr = m_device->CreatePixelShader(
			binary_code, binary_code_size, nullptr, &d3d11_shader);
		if (FAILED(hr)) return nullptr;
		D3D11FSPointer* pointer = new D3D11FSPointer();
		pointer->set_d3d11_fs(d3d11_shader);
		FSPtr result(new FragmentShader());
		result->set_pointer(pointer);

		return result;
	}

	VSPtr D3D11_1LLGI::create_vertex_shader_from_header(
		const void* data, U32 size,
		const char* file_name)
	{
		ID3D11VertexShader* d3d11_shader;
		HRESULT hr = m_device->CreateVertexShader(
			data, size, nullptr, &d3d11_shader);
		if (FAILED(hr)){
			return nullptr;
		}
		D3D11VSPointer* pointer = new D3D11VSPointer();
		pointer->set_d3d11_vs(d3d11_shader);
		VSPtr result(new VertexShader());
		result->set_pointer(pointer);

		return result;
	}

	FSPtr D3D11_1LLGI::create_fragment_shader_from_header(
		const void* data, U32 size,
		const char* file_name)
	{
		ID3D11PixelShader* d3d11_shader;
		HRESULT hr = m_device->CreatePixelShader(
			data, size, nullptr, &d3d11_shader);
		if (FAILED(hr)) return nullptr;
		D3D11FSPointer* pointer = new D3D11FSPointer();
		pointer->set_d3d11_fs(d3d11_shader);
		FSPtr result(new FragmentShader());
		result->set_pointer(pointer);

		return result;
	}

	void D3D11_1LLGI::enable_texture(U32 slot,
		const TexPtr& texture, BindFlag flag)
	{
		ID3D11ShaderResourceView* shader_resource_view =
			get_d3d11_pointer(texture);

		if (flag & BIND_TO_VS)
			m_context->VSSetShaderResources(slot, 1,
			&shader_resource_view);
		if (flag & BIND_TO_FS)
			m_context->PSSetShaderResources(slot, 1,
			&shader_resource_view);
		if (flag & BIND_TO_GS)
			m_context->GSSetShaderResources(slot, 1,
			&shader_resource_view);
		if (flag & BIND_TO_CS)
			m_context->CSSetShaderResources(slot, 1,
			&shader_resource_view);
		if (flag & BIND_TO_DS)
			m_context->DSSetShaderResources(slot, 1,
			&shader_resource_view);
		if (flag & BIND_TO_HS)
			m_context->HSSetShaderResources(slot, 1,
			&shader_resource_view);
	}

	void D3D11_1LLGI::disable_texture(U32 slot,
		const TexPtr& texture, BindFlag flag)
	{
		ID3D11ShaderResourceView* null_arr [] = {
			nullptr
		};
		if (flag & BIND_TO_VS)
			m_context->VSSetShaderResources(slot, 1,
			null_arr);
		if (flag & BIND_TO_FS)
			m_context->PSSetShaderResources(slot, 1,
			null_arr);
		if (flag & BIND_TO_GS)
			m_context->GSSetShaderResources(slot, 1,
			null_arr);
		if (flag & BIND_TO_CS)
			m_context->CSSetShaderResources(slot, 1,
			null_arr);
		if (flag & BIND_TO_DS)
			m_context->DSSetShaderResources(slot, 1,
			null_arr);
		if (flag & BIND_TO_HS)
			m_context->HSSetShaderResources(slot, 1,
			null_arr);
	}

	void D3D11_1LLGI::release_texture_view(const TexPtr& texture){
		ID3D11ShaderResourceView* texture_view =
			get_d3d11_pointer(texture);
		texture_view->Release();
	}

	void D3D11_1LLGI::release_texture_2D(const Tex2DPtr& texture){
		ID3D11Texture2D* texture_2d =
			get_d3d11_pointer(texture);
		texture_2d->Release();
	}

	void D3D11_1LLGI::release_texture_cube(const TexCubePtr& texture){
		ID3D11Texture2D* texture_cube =
			get_d3d11_pointer(texture);
		texture_cube->Release();
	}

	Tex2DPtr D3D11_1LLGI::create_texture_2d(
		U32 width, U32 height,
		TextureElementType element_type, void* data)
	{
		ID3D11Texture2D* d3d11_texture;
		ID3D11ShaderResourceView* d3d11_view;
		D3D11_TEXTURE2D_DESC texture_desc;
		ZeroMemory(&texture_desc, sizeof(texture_desc));
		texture_desc.ArraySize = 1;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.Format = to_dxgi_format(element_type);
		texture_desc.Height = height;
		texture_desc.Width = width;
		texture_desc.MipLevels = 1;;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		HRESULT hr = S_OK;
		if (data != nullptr){
			D3D11_SUBRESOURCE_DATA init_data;
			ZeroMemory(&init_data, sizeof(init_data));
			init_data.pSysMem = data;
			init_data.SysMemPitch =
				get_byte_width_from_element_type(element_type) * width;
			hr = m_device->CreateTexture2D(
				&texture_desc, &init_data, &d3d11_texture);
		}
		else{
			hr = m_device->CreateTexture2D(
				&texture_desc, nullptr, &d3d11_texture);
		}
		if (FAILED(hr)) return nullptr;

		D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
		view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		view_desc.Format = to_dxgi_format(element_type);
		view_desc.Texture2D.MipLevels = 1;;
		view_desc.Texture2D.MostDetailedMip = 0;
		hr = m_device->CreateShaderResourceView(
			d3d11_texture, &view_desc, &d3d11_view);
		if (FAILED(hr)){
			d3d11_texture->Release();
			return nullptr;
		}

		D3D11Texture2DPointer* pointer =
			new D3D11Texture2DPointer;
		pointer->set_d3d11_texture_2d_pointer(d3d11_texture);
		D3D11TexturePointer* view =
			new D3D11TexturePointer;
		view->set_d3d11_texture_pointer(d3d11_view);
		Tex2DPtr texture_2d(new
			Texture2D(element_type, width, height, 1));
		texture_2d->set_texture_2d_pointer(pointer);
		texture_2d->set_texture_pointer(view);

		return texture_2d;
	}

	TexCubePtr D3D11_1LLGI::create_texture_cube(
		U32 width, U32 height,
		TextureElementType element_type, void** data)
	{
		ID3D11Texture2D* d3d11_texture;
		ID3D11ShaderResourceView* d3d11_view;
		D3D11_TEXTURE2D_DESC texture_desc;
		ZeroMemory(&texture_desc, sizeof(texture_desc));
		texture_desc.ArraySize = 6;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.Format = to_dxgi_format(element_type);
		texture_desc.Height = height;
		texture_desc.Width = width;
		texture_desc.MipLevels = 1;;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		HRESULT hr = S_OK;
		if (data != nullptr){
			D3D11_SUBRESOURCE_DATA init_data;
			ZeroMemory(&init_data, sizeof(init_data));
			init_data.pSysMem = data;
			init_data.SysMemPitch =
				get_byte_width_from_element_type(element_type) * width;
			init_data.SysMemSlicePitch =
				init_data.SysMemPitch * height;
			hr = m_device->CreateTexture2D(
				&texture_desc, &init_data, &d3d11_texture);
		}
		else{
			hr = m_device->CreateTexture2D(
				&texture_desc, nullptr, &d3d11_texture);
		}
		if (FAILED(hr)) return nullptr;

		D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
		view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		view_desc.Format = to_dxgi_format(element_type);
		view_desc.TextureCube.MipLevels = 1;;
		view_desc.TextureCube.MostDetailedMip = 0;
		hr = m_device->CreateShaderResourceView(
			d3d11_texture, &view_desc, &d3d11_view);
		if (FAILED(hr)){
			d3d11_texture->Release();
			return nullptr;
		}

		D3D11TextureCubePointer* pointer =
			new D3D11TextureCubePointer;
		pointer->set_d3d11_texture_cube_pointer(d3d11_texture);
		D3D11TexturePointer* view = new D3D11TexturePointer;
		view->set_d3d11_texture_pointer(d3d11_view);
		TexCubePtr texture_cube(new
			TextureCube(element_type, width, height));
		texture_cube->set_texture_cube_pointer(pointer);
		texture_cube->set_texture_pointer(view);

		return texture_cube;
	}

	Tex2DPtr D3D11_1LLGI::create_texture_2d_with_mip_map(
		U32 mip_map_level, U32 width [], U32 height [],
		TextureElementType elment_type, void** data)
	{
		return nullptr;
	}

	void D3D11_1LLGI::update_texture_2d(const Tex2DPtr& texture, void* data){
		D3D11_BOX box;
		box.left = 0;
		box.right = static_cast<U32>(texture->width());
		box.top = 0;
		box.bottom = static_cast<U32>(texture->height());
		box.front = 0;
		box.back = 1;
		U32 row_pitch =
			get_byte_width_from_element_type(texture->element_type())
			* texture->width();
		ID3D11Texture2D* d3d11_texture =
			get_d3d11_pointer(texture);
		m_context->UpdateSubresource(d3d11_texture,
			0, &box, data, row_pitch, 0);
	}

	void D3D11_1LLGI::texture_subimage(const Tex2DPtr& texture,
		U32 xmin, U32 xmax, U32 ymin, U32 ymax, void* data)
	{
		D3D11_BOX box;
		box.left = xmin;
		box.right = xmax;
		box.top = ymin;
		box.bottom = ymax;
		box.front = 0;
		box.back = 1;
		U32 width = xmax - xmin;
		U32 row_pitch =
			get_byte_width_from_element_type(texture->element_type())
			* width;
		ID3D11Texture2D* d3d11_texture =
			get_d3d11_pointer(texture);
		m_context->UpdateSubresource(d3d11_texture,
			0, &box, data, row_pitch, 0);
	}

	void D3D11_1LLGI::set_render_targets(U32 num,
		RTPtr render_targets [], DSPtr depth_stencil)
	{
		static ID3D11RenderTargetView* d3d11_render_targets[8];
		ID3D11DepthStencilView* d3d11_depth_stencil = nullptr;
		if (depth_stencil)
			d3d11_depth_stencil = get_d3d11_pointer(depth_stencil);

		for (U32 i = 0; i < num; ++i){
			d3d11_render_targets[i] = get_d3d11_pointer(render_targets[i]);
		}
		m_context->OMSetRenderTargets(num,
			d3d11_render_targets, d3d11_depth_stencil);
		set_viewport(render_targets[0]->rt_width(),
			render_targets[0]->rt_height());
	}

	void D3D11_1LLGI::set_to_default_render_target(){
		ID3D11DepthStencilView* depth_stencil_view =
			get_d3d11_pointer(m_default_depth_stencil);
		m_context->OMSetRenderTargets(1,
			&m_default_render_target,
			depth_stencil_view);
		set_viewport(m_width, m_height);
	}

	void D3D11_1LLGI::release_render_target(const RTPtr& render_target){
		ID3D11RenderTargetView* d3d11_render_target =
			get_d3d11_pointer(render_target);
		d3d11_render_target->Release();
	}

	void D3D11_1LLGI::release_depth_stencil(const DSPtr& depth_stencil){
		ID3D11DepthStencilView* d3d11_depth_stencil_view =
			get_d3d11_pointer(depth_stencil);
		ID3D11Texture2D* d3d11_depth_stencil =
			get_d3d11_texture(depth_stencil);
		d3d11_depth_stencil_view->Release();
		d3d11_depth_stencil->Release();
	}

	void D3D11_1LLGI::release_window_render_target(const WRTPtr& wrt){
		auto pointer = wrt->wrt_ptr();
		auto d3d11_pointer = dynamic_cast<D3D11WRTPointer*>(pointer);
		IDXGISwapChain *swap_chain = d3d11_pointer->swap_chian();

		swap_chain->Release();

		release_render_target(wrt);
	}

	DSPtr D3D11_1LLGI::create_depth_stencil(
		U32 width, U32 height)
	{
		HRESULT hr = S_OK;
		ID3D11Texture2D* d3d11_depth_stencil_texture;
		ID3D11DepthStencilView* d3d11_depth_stencil_view;

		D3D11_TEXTURE2D_DESC depth_desc;
		ZeroMemory(&depth_desc, sizeof(depth_desc));
		depth_desc.Width = width;
		depth_desc.Height = height;
		depth_desc.MipLevels = 1;
		depth_desc.ArraySize = 1;
		depth_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_desc.SampleDesc.Count = 1;
		depth_desc.SampleDesc.Quality = 0;
		depth_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depth_desc.CPUAccessFlags = 0;
		depth_desc.MiscFlags = 0;
		hr = m_device->CreateTexture2D(&depth_desc,
			nullptr, &d3d11_depth_stencil_texture);
		if (FAILED(hr))
			return nullptr;//error process

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
		ZeroMemory(&depth_stencil_view_desc,
			sizeof(depth_stencil_view_desc));
		depth_stencil_view_desc.Format = depth_desc.Format;
		depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depth_stencil_view_desc.Texture2D.MipSlice = 0;
		hr = m_device->CreateDepthStencilView(d3d11_depth_stencil_texture,
			&depth_stencil_view_desc, &d3d11_depth_stencil_view);
		if (FAILED(hr)){
			d3d11_depth_stencil_texture->Release();
			return nullptr;
		}

		D3D11DepthStencilPointer* pointer = new D3D11DepthStencilPointer;
		pointer->set_d3d11_depth_stencil_pointer(d3d11_depth_stencil_view);
		pointer->set_d3d11_texture(d3d11_depth_stencil_texture);
		DSPtr depth_stencil(new DepthStencil(width, height));
		depth_stencil->set_depth_stencil_pointer(pointer);

		return depth_stencil;
	}

	DSPtr D3D11_1LLGI::create_depth_stencil_cube(
		U32 width, U32 height)
	{
		HRESULT hr = S_OK;
		ID3D11Texture2D* d3d11_depth_stencil_texture;
		ID3D11DepthStencilView* d3d11_depth_stencil_view;

		D3D11_TEXTURE2D_DESC depth_desc;
		ZeroMemory(&depth_desc, sizeof(depth_desc));
		depth_desc.Width = width;
		depth_desc.Height = height;
		depth_desc.MipLevels = 1;
		depth_desc.ArraySize = 6;
		depth_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_desc.SampleDesc.Count = 1;
		depth_desc.SampleDesc.Quality = 0;
		depth_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depth_desc.CPUAccessFlags = 0;
		depth_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		hr = m_device->CreateTexture2D(&depth_desc,
			nullptr, &d3d11_depth_stencil_texture);
		if (FAILED(hr))
			return nullptr;//error process

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
		ZeroMemory(&depth_stencil_view_desc,
			sizeof(depth_stencil_view_desc));
		depth_stencil_view_desc.Format = depth_desc.Format;
		depth_stencil_view_desc.ViewDimension =
			D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		depth_stencil_view_desc.Texture2DArray.ArraySize = 6;
		depth_stencil_view_desc.Texture2DArray.FirstArraySlice = 0;
		depth_stencil_view_desc.Texture2DArray.MipSlice = 0;
		hr = m_device->CreateDepthStencilView(d3d11_depth_stencil_texture,
			&depth_stencil_view_desc, &d3d11_depth_stencil_view);
		if (FAILED(hr)){
			d3d11_depth_stencil_texture->Release();
			return nullptr;
		}

		D3D11DepthStencilPointer* pointer = new D3D11DepthStencilPointer;
		pointer->set_d3d11_depth_stencil_pointer(d3d11_depth_stencil_view);
		pointer->set_d3d11_texture(d3d11_depth_stencil_texture);
		DSPtr depth_stencil(new DepthStencil(width, height));
		depth_stencil->set_depth_stencil_pointer(pointer);

		return depth_stencil;
	}

	RDSPtr D3D11_1LLGI::create_readable_depth_stencil(
		U32 width, U32 height)
	{
		HRESULT hr = S_OK;
		ID3D11Texture2D* d3d11_depth_stencil_texture;
		ID3D11DepthStencilView* d3d11_depth_stencil_view;
		ID3D11ShaderResourceView* d3d11_shader_resource_view;

		D3D11_TEXTURE2D_DESC depth_desc;
		ZeroMemory(&depth_desc, sizeof(depth_desc));
		depth_desc.Width = width;
		depth_desc.Height = height;
		depth_desc.MipLevels = 1;
		depth_desc.ArraySize = 1;
		depth_desc.Format = DXGI_FORMAT_R32_TYPELESS;
		depth_desc.SampleDesc.Count = 1;
		depth_desc.SampleDesc.Quality = 0;
		depth_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL |
			D3D11_BIND_SHADER_RESOURCE;
		depth_desc.CPUAccessFlags = 0;
		depth_desc.MiscFlags = 0;
		hr = m_device->CreateTexture2D(&depth_desc,
			nullptr, &d3d11_depth_stencil_texture);
		if (FAILED(hr))
			return nullptr;//error process

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
		ZeroMemory(&depth_stencil_view_desc,
			sizeof(depth_stencil_view_desc));
		depth_stencil_view_desc.Format = DXGI_FORMAT_D32_FLOAT;
		depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depth_stencil_view_desc.Texture2D.MipSlice = 0;
		hr = m_device->CreateDepthStencilView(d3d11_depth_stencil_texture,
			&depth_stencil_view_desc, &d3d11_depth_stencil_view);
		if (FAILED(hr)){
			d3d11_depth_stencil_texture->Release();
			return nullptr;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc;
		ZeroMemory(&shader_resource_view_desc,
			sizeof(shader_resource_view_desc));
		shader_resource_view_desc.Format = DXGI_FORMAT_R32_FLOAT;
		shader_resource_view_desc.ViewDimension =
			D3D11_SRV_DIMENSION_TEXTURE2D;
		shader_resource_view_desc.Texture2D.MipLevels = 1;;
		shader_resource_view_desc.Texture2D.MostDetailedMip = 0;
		hr = m_device->CreateShaderResourceView(
			d3d11_depth_stencil_texture,
			&shader_resource_view_desc,
			&d3d11_shader_resource_view);
		if (FAILED(hr)){
			d3d11_depth_stencil_texture->Release();
			d3d11_depth_stencil_view->Release();
			return nullptr;
		}

		D3D11DepthStencilPointer* pointer = new D3D11DepthStencilPointer;
		pointer->set_d3d11_depth_stencil_pointer(d3d11_depth_stencil_view);
		pointer->set_d3d11_texture(d3d11_depth_stencil_texture);
		D3D11TexturePointer* texture_view = new D3D11TexturePointer;
		texture_view->set_d3d11_texture_pointer(d3d11_shader_resource_view);
		RDSPtr result(
			new ReadableDepthStencil(width, height));
		result->set_depth_stencil_pointer(pointer);
		result->set_texture_pointer(texture_view);

		return result;
	}

	RDSPtr D3D11_1LLGI::create_readable_depth_stencil_cube(
		U32 width, U32 height)
	{
		HRESULT hr = S_OK;
		ID3D11Texture2D* d3d11_depth_stencil_texture;
		ID3D11DepthStencilView* d3d11_depth_stencil_view;
		ID3D11ShaderResourceView* d3d11_shader_resource_view;

		D3D11_TEXTURE2D_DESC depth_desc;
		ZeroMemory(&depth_desc, sizeof(depth_desc));
		depth_desc.Width = width;
		depth_desc.Height = height;
		depth_desc.MipLevels = 1;
		depth_desc.ArraySize = 6;
		depth_desc.Format = DXGI_FORMAT_R32_TYPELESS;
		depth_desc.SampleDesc.Count = 1;
		depth_desc.SampleDesc.Quality = 0;
		depth_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL |
			D3D11_BIND_SHADER_RESOURCE;
		depth_desc.CPUAccessFlags = 0;
		depth_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		hr = m_device->CreateTexture2D(&depth_desc,
			nullptr, &d3d11_depth_stencil_texture);
		if (FAILED(hr))
			return nullptr;//error process

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
		ZeroMemory(&depth_stencil_view_desc,
			sizeof(depth_stencil_view_desc));
		depth_stencil_view_desc.Format = DXGI_FORMAT_D32_FLOAT;
		depth_stencil_view_desc.ViewDimension =
			D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		depth_stencil_view_desc.Texture2DArray.ArraySize = 6;
		depth_stencil_view_desc.Texture2DArray.FirstArraySlice = 0;
		depth_stencil_view_desc.Texture2DArray.MipSlice = 0;
		hr = m_device->CreateDepthStencilView(d3d11_depth_stencil_texture,
			&depth_stencil_view_desc, &d3d11_depth_stencil_view);
		if (FAILED(hr)){
			d3d11_depth_stencil_texture->Release();
			return nullptr;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc;
		ZeroMemory(&shader_resource_view_desc,
			sizeof(shader_resource_view_desc));
		shader_resource_view_desc.Format = DXGI_FORMAT_R32_FLOAT;
		shader_resource_view_desc.ViewDimension =
			D3D11_SRV_DIMENSION_TEXTURECUBE;
		shader_resource_view_desc.TextureCube.MipLevels = 1;;
		shader_resource_view_desc.TextureCube.MostDetailedMip = 0;
		hr = m_device->CreateShaderResourceView(
			d3d11_depth_stencil_texture,
			&shader_resource_view_desc,
			&d3d11_shader_resource_view);
		if (FAILED(hr)){
			d3d11_depth_stencil_texture->Release();
			d3d11_depth_stencil_view->Release();
			return nullptr;
		}

		D3D11DepthStencilPointer* pointer = new D3D11DepthStencilPointer;
		pointer->set_d3d11_depth_stencil_pointer(d3d11_depth_stencil_view);
		pointer->set_d3d11_texture(d3d11_depth_stencil_texture);
		D3D11TexturePointer* texture_view = new D3D11TexturePointer;
		texture_view->set_d3d11_texture_pointer(d3d11_shader_resource_view);
		RDSPtr result(
			new ReadableDepthStencil(width, height));
		result->set_depth_stencil_pointer(pointer);
		result->set_texture_pointer(texture_view);

		return result;
	}

	RTex2DPtr D3D11_1LLGI::create_renderable_texture_2d(
		U32 width, U32 height,
		TextureElementType element_type)
	{
		ID3D11Texture2D* d3d11_texture;
		ID3D11ShaderResourceView* d3d11_texture_view;
		ID3D11RenderTargetView* d3d11_render_target_view;

		D3D11_TEXTURE2D_DESC texture_desc;
		ZeroMemory(&texture_desc, sizeof(texture_desc));
		texture_desc.ArraySize = 1;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE |
			D3D11_BIND_RENDER_TARGET;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.Format = to_dxgi_format(element_type);
		texture_desc.Height = height;
		texture_desc.Width = width;
		texture_desc.MipLevels = 1;;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		HRESULT hr = m_device->CreateTexture2D(
			&texture_desc, nullptr, &d3d11_texture);
		if (FAILED(hr)) return nullptr;

		D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
		ZeroMemory(&view_desc, sizeof(view_desc));
		view_desc.Format = to_dxgi_format(element_type);
		view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MipLevels = 1;;
		view_desc.Texture2D.MostDetailedMip = 0;
		hr = m_device->CreateShaderResourceView(
			d3d11_texture, &view_desc, &d3d11_texture_view);
		if (FAILED(hr)){
			d3d11_texture->Release();
			return nullptr;
		}

		D3D11_RENDER_TARGET_VIEW_DESC render_target_desc;
		ZeroMemory(&render_target_desc, sizeof(render_target_desc));
		render_target_desc.Format = to_dxgi_format(element_type);
		render_target_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		render_target_desc.Texture2D.MipSlice = 0;
		hr = m_device->CreateRenderTargetView(
			d3d11_texture, &render_target_desc, &d3d11_render_target_view);
		if (FAILED(hr)){
			d3d11_texture->Release();
			d3d11_texture_view->Release();
			return nullptr;
		}

		D3D11Texture2DPointer* texture_pointer = new D3D11Texture2DPointer;
		texture_pointer->set_d3d11_texture_2d_pointer(d3d11_texture);
		D3D11TexturePointer* texture_view = new D3D11TexturePointer;
		texture_view->set_d3d11_texture_pointer(d3d11_texture_view);
		D3D11RenderTargetPointer* render_target_pointer =
			new D3D11RenderTargetPointer;
		render_target_pointer->set_d3d11_render_target_pointer(
			d3d11_render_target_view);
		RTex2DPtr result(
			new RenderableTexture2D(element_type, width, height));
		result->set_texture_pointer(texture_view);
		result->set_texture_2d_pointer(texture_pointer);
		result->set_render_target_pointer(render_target_pointer);

		return result;
	}

	RTexCubePtr D3D11_1LLGI::create_renderable_texture_cube(
		U32 width, U32 height,
		TextureElementType element_type)
	{
		ID3D11Texture2D* d3d11_texture;
		ID3D11ShaderResourceView* d3d11_texture_view;
		ID3D11RenderTargetView* d3d11_render_target_view;

		D3D11_TEXTURE2D_DESC texture_desc;
		ZeroMemory(&texture_desc, sizeof(texture_desc));
		texture_desc.ArraySize = 6;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE |
			D3D11_BIND_RENDER_TARGET;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.Format = to_dxgi_format(element_type);
		texture_desc.Height = height;
		texture_desc.Width = width;
		texture_desc.MipLevels = 1;;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		HRESULT hr = m_device->CreateTexture2D(
			&texture_desc, nullptr, &d3d11_texture);
		if (FAILED(hr)) return nullptr;

		D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
		ZeroMemory(&view_desc, sizeof(view_desc));
		view_desc.Format = to_dxgi_format(element_type);
		view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		view_desc.TextureCube.MipLevels = 1;;
		view_desc.TextureCube.MostDetailedMip = 0;
		hr = m_device->CreateShaderResourceView(
			d3d11_texture, &view_desc, &d3d11_texture_view);
		if (FAILED(hr)){
			d3d11_texture->Release();
			return nullptr;
		}

		D3D11_RENDER_TARGET_VIEW_DESC render_target_desc;
		ZeroMemory(&render_target_desc, sizeof(render_target_desc));
		render_target_desc.Format = to_dxgi_format(element_type);
		render_target_desc.ViewDimension =
			D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		render_target_desc.Texture2DArray.ArraySize = 6;
		render_target_desc.Texture2DArray.FirstArraySlice = 0;
		render_target_desc.Texture2DArray.MipSlice = 0;
		hr = m_device->CreateRenderTargetView(
			d3d11_texture, &render_target_desc, &d3d11_render_target_view);
		if (FAILED(hr)){
			d3d11_texture->Release();
			d3d11_texture_view->Release();
			return nullptr;
		}

		D3D11TextureCubePointer* texture_pointer = new D3D11TextureCubePointer;
		texture_pointer->set_d3d11_texture_cube_pointer(d3d11_texture);
		D3D11TexturePointer* texture_view = new D3D11TexturePointer;
		texture_view->set_d3d11_texture_pointer(d3d11_texture_view);
		D3D11RenderTargetPointer* render_target_pointer =
			new D3D11RenderTargetPointer;
		render_target_pointer->set_d3d11_render_target_pointer(
			d3d11_render_target_view);
		RTexCubePtr result(
			new RenderableTextureCube(element_type, width, height));
		result->set_texture_pointer(texture_view);
		result->set_texture_cube_pointer(texture_pointer);
		result->set_render_target_pointer(render_target_pointer);

		return result;
	}

	//WRTPtr D3D11_1LLGI::create_window_render_target(
	//	PILWindow* window)
	//{
	//	//TODO
	//	IDXGIDevice * dxgi_device;
	//	HRESULT hr = m_device->QueryInterface(
	//		__uuidof(IDXGIDevice), (void **) &dxgi_device);

	//	IDXGIAdapter * dxgi_adapter;
	//	hr = dxgi_device->GetParent(
	//		__uuidof(IDXGIAdapter), (void **) &dxgi_adapter);

	//	IDXGIFactory * dxgi_factory;
	//	dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void **) &dxgi_factory);


	//	WindowsWindow* windows_window =
	//		dynamic_cast<WindowsWindow*>(window);
	//	U32 width = windows_window->width();
	//	U32 height = windows_window->height();
	//	HWND h_wnd = windows_window->get_handle();

	//	IDXGISwapChain *swap_chian;
	//	DXGI_SWAP_CHAIN_DESC desc;
	//	ZeroMemory(&desc, sizeof(desc));
	//	desc.BufferCount = 1;
	//	desc.BufferDesc.Width = width;
	//	desc.BufferDesc.Height = height;
	//	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//	desc.BufferDesc.RefreshRate.Numerator = 60;
	//	desc.BufferDesc.RefreshRate.Denominator = 1;
	//	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//	desc.OutputWindow = h_wnd;
	//	desc.SampleDesc.Count = 1;
	//	desc.SampleDesc.Quality = 0;
	//	desc.Windowed = TRUE;
	//	hr = dxgi_factory->CreateSwapChain(m_device, &desc, &swap_chian);

	//	ID3D11Texture2D* back_buffer = nullptr;
	//	hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D),
	//		(LPVOID*) &back_buffer);
	//	if (FAILED(hr))
	//		throw Exception("failed to create d3d11 back buffer");

	//	ID3D11RenderTargetView* render_target;
	//	hr = m_device->CreateRenderTargetView(back_buffer,
	//		nullptr, &render_target);
	//	if (FAILED(hr)){
	//		throw Exception("failed to create d3d11 RTV");
	//		back_buffer->Release();
	//	}

	//	back_buffer->Release();

	//	D3D11WRTPointer* wrt_pointer = new D3D11WRTPointer();
	//	wrt_pointer->set_swap_chian(swap_chian);
	//	D3D11RenderTargetPointer* rt_pointer = new D3D11RenderTargetPointer();
	//	rt_pointer->set_d3d11_render_target_pointer(render_target);
	//	WRTPtr result(new WindowRenderTarget(width, height));
	//	result->set_render_target_pointer(rt_pointer);
	//	result->set_wrt_ptr(wrt_pointer);

	//	return result;
	//}

	void D3D11_1LLGI::enable_graphic_state(const GraphicStatePtr& state){
		static const F32 color [] = {
			1.0f, 1.0f, 1.0f, 1.0f
		};
		ID3D11BlendState* as = get_d3d11_as(state);
		m_context->OMSetBlendState(as, color, 0xffffffff);

		ID3D11DepthStencilState* ds = get_d3d11_ds(state);
		m_context->OMSetDepthStencilState(ds, 0);

		ID3D11RasterizerState* rs = get_d3d11_rs(state);
		m_context->RSSetState(rs);

		ID3D11SamplerState* ss = get_d3d11_ss(state);
		m_context->PSSetSamplers(0, 1, &ss);
		m_context->VSSetSamplers(0, 1, &ss);
	}

	void D3D11_1LLGI::disable_graphic_state(const GraphicStatePtr&){
		static const F32 color [] = {
			1.0f, 1.0f, 1.0f, 1.0f
		};
		m_context->OMSetBlendState(nullptr, color, 0xffffffff);
		m_context->OMSetDepthStencilState(nullptr, 0);
		m_context->RSSetState(nullptr);
		m_context->PSSetSamplers(0, 1, &m_default_sampler_state);
		m_context->VSSetSamplers(0, 1, &m_default_sampler_state);
	}

	void D3D11_1LLGI::release_graphic_state(const GraphicStatePtr& state){
		ID3D11BlendState* as = get_d3d11_as(state);
		ID3D11DepthStencilState* ds = get_d3d11_ds(state);
		ID3D11RasterizerState* rs = get_d3d11_rs(state);
		ID3D11SamplerState* ss = get_d3d11_ss(state);

		as->Release();
		ds->Release();
		rs->Release();
		ss->Release();
	}

	GraphicStatePtr D3D11_1LLGI::create_graphic_state(SamplerStateDesc ss_desc,
		RasterizerStateDesc rs_desc, DepthStencilStateDesc ds_desc, AlphaStateDesc as_desc)
	{
		ID3D11BlendState* as = create_as(as_desc);
		ID3D11DepthStencilState* ds = create_ds(ds_desc);
		ID3D11RasterizerState* rs = create_rs(rs_desc);
		ID3D11SamplerState* ss = create_ss(ss_desc);

		GraphicStatePtr state(new GraphicState());
		state->set_alpha_state_desc(as_desc);
		state->set_depth_stencil_state_desc(ds_desc);
		state->set_rasterizer_state_desc(rs_desc);
		state->set_sampler_state_desc(ss_desc);
		D3D11GraphicStatePointer* pointer = new D3D11GraphicStatePointer();
		pointer->set_as_pointer(as);
		pointer->set_ds_state_pointer(ds);
		pointer->set_rs_state_pointer(rs);
		pointer->set_ss_state_pointer(ss);
		state->set_pointer(pointer);

		return state;
	}

	void D3D11_1LLGI::draw(){
		m_context->DrawIndexed(m_index_number, 0, 0);
	}

	void D3D11_1LLGI::swap_buffer(const WRTPtr& wrt){
		if (wrt){
			auto pointer = wrt->wrt_ptr();
			auto d3d11_pointer = dynamic_cast<D3D11WRTPointer*>(pointer);
			IDXGISwapChain *swap_chain = d3d11_pointer->swap_chian();
			swap_chain->Present(0, 0);
		}
		else{
			m_swap_chain->Present(1, 0);
		}
	}

	void D3D11_1LLGI::clear_default_render_target(F32* color){
		ID3D11DepthStencilView* depth_stencil_view =
			get_d3d11_pointer(m_default_depth_stencil);
		m_context->ClearDepthStencilView(
			depth_stencil_view,
			D3D11_CLEAR_DEPTH, 1.0f, 0);
		m_context->ClearRenderTargetView(
			m_default_render_target, color);
	}

	void D3D11_1LLGI::clear_render_target(const RTPtr& render_target, F32* color){
		static F32 black [] = {
			0.0f, 0.0f, 0.0f, 1.0f
		};

		ID3D11RenderTargetView* render_target_view =
			get_d3d11_pointer(render_target);
		if (color != nullptr)
			m_context->ClearRenderTargetView(
			render_target_view, color);
		else
			m_context->ClearRenderTargetView(
			render_target_view, black);
	}

	void D3D11_1LLGI::clear_depth_stencil(const DSPtr& depth_stencil, float depth){
		ID3D11DepthStencilView* depth_stencil_view =
			get_d3d11_pointer(depth_stencil);
		m_context->ClearDepthStencilView(depth_stencil_view,
			D3D11_CLEAR_DEPTH, depth, 0);
	}

	void D3D11_1LLGI::set_geometry_type(GeometryType type){
		switch (type){
		case NECROMANCER_GEOMETRY_POINTS:
			m_context->IASetPrimitiveTopology(
				D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			break;
		case NECROMANCER_GEOMETRY_LINES:
			m_context->IASetPrimitiveTopology(
				D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		case NECROMANCER_GEOMETRY_MESH:
			m_context->IASetPrimitiveTopology(
				D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		}
	}

	void D3D11_1LLGI::set_viewport(U32 width, U32 height){
		D3D11_VIEWPORT vp;
		vp.Width = (F32) width;
		vp.Height = (F32) height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_context->RSSetViewports(1, &vp);
	}

	ID3D11BlendState* D3D11_1LLGI::create_as(AlphaStateDesc s_desc){
		ID3D11BlendState* d3d11_state;
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		if (s_desc.enable){
			desc.RenderTarget[0].BlendEnable = TRUE;
			desc.RenderTarget[0].RenderTargetWriteMask =
				D3D11_COLOR_WRITE_ENABLE_ALL;
			desc.RenderTarget[0].SrcBlend = to_d3d11_blend(s_desc.src);
			desc.RenderTarget[0].DestBlend = to_d3d11_blend(s_desc.dest);
			desc.RenderTarget[0].SrcBlendAlpha =
				to_d3d11_blend(s_desc.src_alpha);
			desc.RenderTarget[0].DestBlendAlpha =
				to_d3d11_blend(s_desc.dest_alpha);
			desc.RenderTarget[0].BlendOp =
				to_d3d11_blend_op(s_desc.blend_op);
			desc.RenderTarget[0].BlendOpAlpha =
				to_d3d11_blend_op(s_desc.alpha_op);
		}
		else{
			desc.RenderTarget[0].BlendEnable = FALSE;
			desc.RenderTarget[0].RenderTargetWriteMask =
				D3D11_COLOR_WRITE_ENABLE_ALL;
		}

		HRESULT hr = m_device->CreateBlendState(
			&desc, &d3d11_state);
		if (FAILED(hr)) return nullptr;
		else return d3d11_state;
	}

	ID3D11DepthStencilState* D3D11_1LLGI::create_ds(
		DepthStencilStateDesc s_desc)
	{
		ID3D11DepthStencilState* d3d11_state;
		D3D11_DEPTH_STENCIL_DESC desc;

		desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

		if (s_desc.depth_testing) desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		else desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		if (s_desc.depth_write) desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		else desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		switch (s_desc.comparison){
		case NECROMANCER_DEPTH_COMPARISON_NEVER:
			desc.DepthFunc = D3D11_COMPARISON_NEVER;
			break;
		case NECROMANCER_DEPTH_COMPARISON_LESS:
			desc.DepthFunc = D3D11_COMPARISON_LESS;
			break;
		case NECROMANCER_DEPTH_COMPARISON_EQUAL:
			desc.DepthFunc = D3D11_COMPARISON_EQUAL;
			break;
		case NECROMANCER_DEPTH_COMPARISON_LESS_EQUAL:
			desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			break;
		case NECROMANCER_DEPTH_COMPARISON_GREATER:
			desc.DepthFunc = D3D11_COMPARISON_GREATER;
			break;
		case NECROMANCER_DEPTH_COMPARISON_NOT_EQUAL:
			desc.DepthFunc = D3D11_COMPARISON_NOT_EQUAL;
			break;
		case NECROMANCER_DEPTH_COMPARISON_GREATER_EQUAL:
			desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
			break;
		case NECROMANCER_DEPTH_COMPARISON_ALWAYS:
			desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
			break;
		}

		if (s_desc.stencil_testing) desc.StencilEnable = TRUE;
		else desc.StencilEnable = FALSE;

		HRESULT hr = m_device->CreateDepthStencilState(&desc, &d3d11_state);
		if (FAILED(hr)) return nullptr;
		else return d3d11_state;
	}

	ID3D11RasterizerState* D3D11_1LLGI::create_rs(
		RasterizerStateDesc s_desc)
	{
		ID3D11RasterizerState* d3d11_state;
		D3D11_RASTERIZER_DESC desc;
		desc.DepthBias = 0;
		desc.SlopeScaledDepthBias = 0.0f;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = TRUE;
		desc.ScissorEnable = FALSE;
		switch (s_desc.fill_mode){
		case NECROMANCER_FILL_SOLID:
			desc.FillMode = D3D11_FILL_SOLID;
			break;
		case NECROMANCER_FILL_WIREFRAME:
			desc.FillMode = D3D11_FILL_WIREFRAME;
		}
		switch (s_desc.cull_mode){
		case NECROMANCER_CULL_NONE:
			desc.CullMode = D3D11_CULL_NONE;
			break;
		case NECROMANCER_CULL_FRONT:
			desc.CullMode = D3D11_CULL_FRONT;
			break;
		case NECROMANCER_CULL_BACK:
			desc.CullMode = D3D11_CULL_BACK;
			break;
		}
		if (s_desc.multisample) desc.MultisampleEnable = TRUE;
		else desc.MultisampleEnable = FALSE;
		if (s_desc.antialiased_line) desc.AntialiasedLineEnable = TRUE;
		else desc.AntialiasedLineEnable = FALSE;

		HRESULT hr = m_device->CreateRasterizerState(&desc, &d3d11_state);
		if (FAILED(hr)) return nullptr;
		else return d3d11_state;
	}

	ID3D11SamplerState* D3D11_1LLGI::create_ss(SamplerStateDesc s_desc){
		D3D11_TEXTURE_ADDRESS_MODE d3d_address_mode;
		switch (s_desc.texture_address_mode){
		case NECROMANCER_TEXTURE_ADDRESS_WRAP:
			d3d_address_mode = D3D11_TEXTURE_ADDRESS_WRAP;
			break;
		case NECROMANCER_TEXTURE_ADDRESS_MIRROR:
			d3d_address_mode = D3D11_TEXTURE_ADDRESS_MIRROR;
			break;
		case NECROMANCER_TEXTURE_ADDRESS_CLAMP:
			d3d_address_mode = D3D11_TEXTURE_ADDRESS_CLAMP;
			break;
		case NECROMANCER_TEXTURE_ADDRESS_BORDER:
			d3d_address_mode = D3D11_TEXTURE_ADDRESS_BORDER;
			break;
		}
		D3D11_FILTER d3d_filter;
		switch (s_desc.sampler_filter){
		case NECROMANCER_TEXTURE_FILTER_POINT:
			d3d_filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		case NECROMANCER_TEXTURE_FILTER_LINEAR:
			d3d_filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			break;
		case NECROMANCER_TEXTURE_FILTER_ANISOTROPIC:
			d3d_filter = D3D11_FILTER_ANISOTROPIC;
			break;
		}
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Filter = d3d_filter;
		desc.AddressU = d3d_address_mode;
		desc.AddressV = d3d_address_mode;
		desc.AddressW = d3d_address_mode;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;
		desc.MaxAnisotropy = s_desc.max_anisotropic;
		ID3D11SamplerState* d3d11_sampler_state;
		HRESULT hr = m_device->CreateSamplerState(&desc,
			&d3d11_sampler_state);
		if (FAILED(hr)) return nullptr;
		else return d3d11_sampler_state;
	}

	void D3D11_1LLGI::set_vertex_format(const String& format){
		ID3D11InputLayout* layout = m_input_layout_map[format];

		if (layout == nullptr){
			ID3DBlob* shader_blob;
			ID3DBlob* error_blob;

			String shader_text = make_format_vs_text(format);
			HRESULT hr = D3DCompile(
				shader_text.c_str(), shader_text.length(),
				nullptr, nullptr, nullptr, "main",
				m_vs_target, 0, 0, &shader_blob, &error_blob);
			if (error_blob != nullptr){
				OutputDebugStringA((char*) error_blob->GetBufferPointer());
				error_blob->Release();
			}
			if (FAILED(hr))
				throw  Exception("failed compiling d3d11 generating code");

			D3D11_INPUT_ELEMENT_DESC elements[32];
			U32 num = 0;
			make_input_element_desc(format, elements, num);

			hr = m_device->CreateInputLayout(elements, num,
				shader_blob->GetBufferPointer(),
				shader_blob->GetBufferSize(), &layout);
			if (FAILED(hr))
				throw Exception("failed to create d3d11 input layout");

			m_input_layout_map[format] = layout;
		}

		m_context->IASetInputLayout(layout);
	}

	DXGI_FORMAT D3D11_1LLGI::to_dxgi_format(TextureElementType element_type){
		DXGI_FORMAT format;
		switch (element_type){
		case NECROMANCER_TEXTURE_R8:
			format = DXGI_FORMAT_R8_UINT;
			break;
		case NECROMANCER_TEXTURE_R32:
			format = DXGI_FORMAT_R32_FLOAT;
			break;
		case NECROMANCER_TEXTURE_R32_UINT:
			format = DXGI_FORMAT_R32_UINT;
			break;
		case NECROMANCER_TEXTURE_R32G32:
			format = DXGI_FORMAT_R32G32_FLOAT;
			break;
		case NECROMANCER_TEXTURE_R32G32B32:
			format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		case NECROMANCER_TEXTURE_R32G32B32A32:
			format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		case NECROMANCER_TEXTURE_D24S8:
			format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			break;
		case NECROMANCER_TEXTURE_R8G8B8A8:
			format = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case NECROMANCER_TEXTURE_R8G8B8A8I:
			format = DXGI_FORMAT_R8G8B8A8_UINT;
			break;
		}

		return format;
	}

	U32 D3D11_1LLGI::get_byte_width_from_element_type(
		TextureElementType element_type)
	{
		U32 byte_width;
		switch (element_type){
		case NECROMANCER_TEXTURE_R8:
			byte_width = 1;
			break;
		case NECROMANCER_TEXTURE_R32:
			byte_width = 4;
			break;
		case NECROMANCER_TEXTURE_R32G32:
			byte_width = 8;
			break;
		case NECROMANCER_TEXTURE_R32G32B32:
			byte_width = 12;
			break;
		case NECROMANCER_TEXTURE_R32G32B32A32:
			byte_width = 16;
			break;
		case NECROMANCER_TEXTURE_D24S8:
			byte_width = 4;
			break;
		case NECROMANCER_TEXTURE_R8G8B8A8:
			byte_width = 4;
			break;
		case NECROMANCER_TEXTURE_R8G8B8A8I:
			byte_width = 4;
			break;
		}

		return byte_width;
	}

	D3D11_BLEND D3D11_1LLGI::to_d3d11_blend(BlendState state){
		switch (state){
		case NECROMANCER_BLEND_ZERO:
			return D3D11_BLEND_ZERO;
		case NECROMANCER_BLEND_ONE:
			return D3D11_BLEND_ONE;
		case NECROMANCER_BLEND_SRC:
			return D3D11_BLEND_SRC_ALPHA;
		case NECROMANCER_BLEND_SRC_COLOR:
			return D3D11_BLEND_SRC_COLOR;
		case NECROMANCER_BLEND_INV_SRC:
			return D3D11_BLEND_INV_SRC_ALPHA;
		case NECROMANCER_BLEND_INV_SRC_COLOR:
			return D3D11_BLEND_INV_SRC_COLOR;
		case NECROMANCER_BLEND_DEST:
			return D3D11_BLEND_DEST_ALPHA;
		case NECROMANCER_BLEND_DEST_COLOR:
			return D3D11_BLEND_DEST_COLOR;
		case NECROMANCER_BLEND_INV_DEST:
			return D3D11_BLEND_INV_DEST_ALPHA;
		case NECROMANCER_BLEND_INV_DEST_COLOR:
			return D3D11_BLEND_INV_DEST_COLOR;
		default:
			return D3D11_BLEND_ZERO;
		}
	}

	D3D11_BLEND_OP D3D11_1LLGI::to_d3d11_blend_op(BlendOperation operation){
		switch (operation){
		case NECROMANCER_BLEND_OP_ADD:
			return D3D11_BLEND_OP_ADD;
		case NECROMANCER_BLEND_OP_SUBTRACT:
			return D3D11_BLEND_OP_SUBTRACT;
		case NECROMANCER_BLEND_OP_REV_SUBTRACT:
			return D3D11_BLEND_OP_REV_SUBTRACT;
		case NECROMANCER_BLEND_OP_MAX:
			return D3D11_BLEND_OP_MAX;
		case NECROMANCER_BLEND_OP_MIN:
			return D3D11_BLEND_OP_MIN;
		default:
			return D3D11_BLEND_OP_ADD;
		}
	}

	D3D11_USAGE D3D11_1LLGI::to_d3d11_usage(BufferUsage buffer_usage){
		switch (buffer_usage){
		case NM_BUFFER_USAGE_DEFAULT:
			return D3D11_USAGE_DEFAULT;
		case NM_BUFFER_USAGE_DYNAMIC:
			return D3D11_USAGE_DYNAMIC;
		case NM_BUFFER_USAGE_IMMUTABLE:
			return D3D11_USAGE_IMMUTABLE;
		default:
			return D3D11_USAGE_DEFAULT;
		}
	}

	UINT D3D11_1LLGI::to_d3d11_cpu_access_flag(
		BufferUsage buffer_usage)
	{
		switch (buffer_usage){
		case NM_BUFFER_USAGE_DEFAULT:
			return 0;
		case NM_BUFFER_USAGE_DYNAMIC:
			return D3D11_CPU_ACCESS_WRITE;
		case NM_BUFFER_USAGE_IMMUTABLE:
			return 0;
		default:
			return 0;
		}
	}

	ID3D11Buffer* D3D11_1LLGI::get_d3d11_pointer(BOPtr buffer_object){
		BufferObjectPointer* bo_pointer = buffer_object->pointer();
		D3D11BOPointer* d3d11_bo_pointer =
			dynamic_cast<D3D11BOPointer*>(bo_pointer);
		if (d3d11_bo_pointer == nullptr)
			throw Exception("wrong type of resource");
		ID3D11Buffer* d3d11_buffer = d3d11_bo_pointer->d3d11_buffer();
		return d3d11_buffer;
	}

	ID3D11VertexShader* D3D11_1LLGI::get_d3d11_pointer(VSPtr vs){
		D3D11VSPointer* pointer = dynamic_cast<D3D11VSPointer*>(
			vs->pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->d3d11_vs();
	}

	ID3D11PixelShader* D3D11_1LLGI::get_d3d11_pointer(FSPtr fs){
		D3D11FSPointer* pointer = dynamic_cast<D3D11FSPointer*>(
			fs->pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->d3d11_fs();
	}

	ID3D11ShaderResourceView* D3D11_1LLGI::get_d3d11_pointer(TexPtr tex){
		D3D11TexturePointer* pointer = dynamic_cast<D3D11TexturePointer*>(
			tex->texture_pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->d3d11_texture_pointer();
	}

	ID3D11RenderTargetView* D3D11_1LLGI::get_d3d11_pointer(RTPtr rt){
		D3D11RenderTargetPointer* pointer =
			dynamic_cast<D3D11RenderTargetPointer*>(
			rt->render_target_pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->d3d11_render_target_pointer();
	}

	ID3D11DepthStencilView* D3D11_1LLGI::get_d3d11_pointer(DSPtr ds){
		D3D11DepthStencilPointer* pointer =
			dynamic_cast<D3D11DepthStencilPointer*>(
			ds->depth_stencil_pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->d3d11_depth_stencil_pointer();
	}

	ID3D11Texture2D* D3D11_1LLGI::get_d3d11_pointer(Tex2DPtr tex){
		D3D11Texture2DPointer* pointer =
			dynamic_cast<D3D11Texture2DPointer*>(
			tex->texture_2d_pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->d3d11_texture_2d_pointer();
	}

	ID3D11Texture2D* D3D11_1LLGI::get_d3d11_pointer(TexCubePtr tex){
		D3D11TextureCubePointer* pointer =
			dynamic_cast<D3D11TextureCubePointer*>(
			tex->texture_cube_pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->d3d11_texture_cube_pointer();
	}

	ID3D11Texture2D* D3D11_1LLGI::get_d3d11_texture(DSPtr ds){
		D3D11DepthStencilPointer* pointer =
			dynamic_cast<D3D11DepthStencilPointer*>(
			ds->depth_stencil_pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->d3d11_texture();
	}

	ID3D11BlendState* D3D11_1LLGI::get_d3d11_as(GraphicStatePtr state){
		D3D11GraphicStatePointer* pointer =
			dynamic_cast<D3D11GraphicStatePointer*>(state->pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->as_pointer();
	}

	ID3D11DepthStencilState* D3D11_1LLGI::get_d3d11_ds(GraphicStatePtr state){
		D3D11GraphicStatePointer* pointer =
			dynamic_cast<D3D11GraphicStatePointer*>(state->pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->ds_state_pointer();
	}

	ID3D11RasterizerState* D3D11_1LLGI::get_d3d11_rs(GraphicStatePtr state){
		D3D11GraphicStatePointer* pointer =
			dynamic_cast<D3D11GraphicStatePointer*>(state->pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->rs_state_pointer();
	}

	ID3D11SamplerState* D3D11_1LLGI::get_d3d11_ss(GraphicStatePtr state){
		D3D11GraphicStatePointer* pointer =
			dynamic_cast<D3D11GraphicStatePointer*>(state->pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->ss_state_pointer();
	}

	void D3D11_1LLGI::make_input_element_desc(const String& format,
		D3D11_INPUT_ELEMENT_DESC* elements, U32& num)
	{
		num = 0;
		U32 offset = 0;
		U32 texture_number = 0;
		U32 str_len = static_cast<U32>(format.length());
		for (U32 i = 0; i < str_len; ++i){
			char ch = format[i];

			D3D11_INPUT_ELEMENT_DESC desc;
			switch (ch){
			case 'P':
				desc.SemanticName = "POSITION";
				desc.SemanticIndex = 0;
				desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				desc.InputSlot = 0;
				desc.AlignedByteOffset = offset;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;
				offset += 3 * sizeof(F32);
				elements[num++] = desc;
				break;
			case 'T':
				desc.SemanticName = "POSITION";
				desc.SemanticIndex = 0;
				desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				desc.InputSlot = 0;
				desc.AlignedByteOffset = offset;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;
				offset += 4 * sizeof(F32);
				elements[num++] = desc;
				break;
			case 'C':
				desc.SemanticName = "COLOR";
				desc.SemanticIndex = 0;
				desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				desc.InputSlot = 0;
				desc.AlignedByteOffset = offset;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;
				offset += 4 * sizeof(F32);
				elements[num++] = desc;
				break;
			case 'N':
				desc.SemanticName = "NORMAL";
				desc.SemanticIndex = 0;
				desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				desc.InputSlot = 0;
				desc.AlignedByteOffset = offset;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;
				offset += 3 * sizeof(F32);
				elements[num++] = desc;
				break;
			case '1':
				desc.SemanticName = "TEXCOORD";
				desc.SemanticIndex = texture_number;
				desc.Format = DXGI_FORMAT_R32_FLOAT;
				desc.InputSlot = 0;
				desc.AlignedByteOffset = offset;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;
				++texture_number;
				offset += sizeof(F32);
				elements[num++] = desc;
				break;
			case '2':
				desc.SemanticName = "TEXCOORD";
				desc.SemanticIndex = texture_number;
				desc.Format = DXGI_FORMAT_R32G32_FLOAT;
				desc.InputSlot = 0;
				desc.AlignedByteOffset = offset;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;
				++texture_number;
				offset += 2 * sizeof(F32);
				elements[num++] = desc;
				break;
			case '3':
				desc.SemanticName = "TEXCOORD";
				desc.SemanticIndex = texture_number;
				desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				desc.InputSlot = 0;
				desc.AlignedByteOffset = offset;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;
				++texture_number;
				offset += 3 * sizeof(F32);
				elements[num++] = desc;
				break;
			case 'A':
				desc.SemanticName = "TANGENT";
				desc.SemanticIndex = 0;
				desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				desc.InputSlot = 0;
				desc.AlignedByteOffset = offset;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;
				offset += 3 * sizeof(F32);
				elements[num++] = desc;
				break;
			case 'S':
				desc.SemanticName = "SKELETON";
				desc.SemanticIndex = 0;
				desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				desc.InputSlot = 0;
				desc.AlignedByteOffset = offset;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;
				offset += 4 * sizeof(F32);
				elements[num++] = desc;

				desc.SemanticName = "WEIGHTS";
				desc.SemanticIndex = 0;
				desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				desc.InputSlot = 0;
				desc.AlignedByteOffset = offset;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;
				offset += 4 * sizeof(F32);
				elements[num++] = desc;
				break;
			}
		}
	}

	String D3D11_1LLGI::make_format_vs_text(const String& format){
		String front = "struct PS_INPUT{ \n"; 
		front += "	float4 vPos : SV_POSITION; \n";
		front += "	float4 Pos : POSITION; \n";
		front += "	float3 Nor : NORMAL; \n";
		front += "	float2 Tex : TEXCOORD0; \n";
		front += "};\n";
		front += "struct VS_INPUT{\n";

		String back = "}; \n"; 
		back += "PS_INPUT main(VS_INPUT input)\n";
		back += "{\n";
		back += "	PS_INPUT result;\n";
		back += "	result.vPos = float4(0.0, 0.0, 0.0, 1.0);\n";
		back += "	result.Pos = float4(0.0, 0.0, 0.0, 1.0);\n";
		back += "	result.Nor = float3(0.0, 0.0, 0.0);\n";
		back += "	result.Tex = float2(0.0, 0.0);\n";
		back += "	return result;\n";
		back += "}";


		U32 str_len = static_cast<U32>(format.length());
		String center;
		U32 texture_number = 0;
		for (U32 i = 0; i < str_len; ++i){
			char ch = format[i];
			switch (ch){
			case 'P':
				center += "float3 Pos : POSITION; \n";
				break;
			case 'T':
				center += "float4 Pos : POSITION; \n";
				break;
			case 'C':
				center += "float4 Col : COLOR; \n";
				break;
			case 'N':
				center += "float3 Nor : NORMAL; \n";
				break;
			case '1':
				center += "float Tex";
				center += char('0' + texture_number);
				center += " : TEXCOORD";
				center += char('0' + texture_number);
				center += "; \n";
				++texture_number;
				break;
			case '2':
				center += "float2 Tex";
				center += char('0' + texture_number);
				center += " : TEXCOORD";
				center += char('0' + texture_number);
				center += "; \n";
				++texture_number;
				break;
			case '3':
				center += "float3 Tex";
				center += char('0' + texture_number);
				center += " : TEXCOORD";
				center += char('0' + texture_number);
				center += "; \n";
				++texture_number;
				break;
			case 'A':
				center += "float3 tangent : TANGENT; \n";
				break;
			case 'S':
				center += "uint4 skeleton_id : SKELETON; \n";
				center += "float4 skeleton_weights : WEIGHTS; \n";
				break;
			}
		}

		String shader_text(front);
		shader_text += center.c_str();
		shader_text += back;

		return shader_text;
	}

	ID3D11GeometryShader* D3D11_1LLGI::get_d3d11_pointer(GSPtr gs){
		D3D11GSPointer* pointer = dynamic_cast<D3D11GSPointer*>(
			gs->pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->d3d11_gs();
	}

	void D3D11_1LLGI::enable_geometry_shader(const GSPtr& gs){
		ID3D11GeometryShader* d3d11_gs = get_d3d11_pointer(gs);
		m_context->GSSetShader(d3d11_gs, nullptr, 0);
	}

	void D3D11_1LLGI::disable_geometry_shader(const GSPtr& gs){
		m_context->GSSetShader(nullptr, nullptr, 0);
	}

	void D3D11_1LLGI::release_geometry_shader(const GSPtr& gs){
		ID3D11GeometryShader* d3d11_gs = get_d3d11_pointer(gs);
		d3d11_gs->Release();
	}

	GSPtr D3D11_1LLGI::create_geometry_shader(
		const char* shader_text,
		const char* file_name)
	{
		ID3D11GeometryShader* d3d11_geometry_shader;
		ID3DBlob* shader_blob = nullptr;
		ID3DBlob* error_blob = nullptr;
		DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		shader_flags |= D3DCOMPILE_DEBUG;
#endif
		HRESULT hr = D3DCompile(shader_text,
			strlen(shader_text), file_name,
			nullptr, nullptr, "main", m_gs_target,
			shader_flags, 0, &shader_blob, &error_blob);
		if (error_blob != nullptr){
			OutputDebugStringA((char*) error_blob->GetBufferPointer());
			error_blob->Release();
		}
		if (FAILED(hr)) return nullptr;

		hr = m_device->CreateGeometryShader(
			shader_blob->GetBufferPointer(),
			shader_blob->GetBufferSize(), nullptr,
			&d3d11_geometry_shader);
		shader_blob->Release();
		if (FAILED(hr)) return nullptr;

		D3D11GSPointer* pointer = new D3D11GSPointer();
		pointer->set_d3d11_gs(d3d11_geometry_shader);
		GSPtr geometry_shader(new GeometryShader());
		geometry_shader->set_pointer(pointer);

#ifdef _DEBUG
		std::string name("UID:");
		name += " type:";
		name += "geometry_shader";
		d3d11_geometry_shader->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			static_cast<UINT>(name.length()), name.c_str());
#endif

		return geometry_shader;
	}

	GSPtr D3D11_1LLGI::create_geometry_shader_from_binary(
		void* binary_code, U32 binary_code_size,
		const char* file_name)
	{
		ID3D11GeometryShader* d3d11_shader;
		HRESULT hr = m_device->CreateGeometryShader(
			binary_code, binary_code_size, nullptr, &d3d11_shader);
		if (FAILED(hr)) return nullptr;
		D3D11GSPointer* pointer = new D3D11GSPointer();
		pointer->set_d3d11_gs(d3d11_shader);
		GSPtr geometry_shader(new GeometryShader());
		geometry_shader->set_pointer(pointer);

#ifdef _DEBUG
		std::string name("UID:");
		name += " type:";
		name += "geometry_shader";
		d3d11_shader->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			static_cast<UINT>(name.length()), name.c_str());
#endif

		return geometry_shader;
	}

	GSPtr D3D11_1LLGI::create_geometry_shader_from_header(
		const void* data, U32 size,
		const char* file_name)
	{
		ID3D11GeometryShader* d3d11_shader;
		HRESULT hr = m_device->CreateGeometryShader(
			data, size, nullptr, &d3d11_shader);
		if (FAILED(hr)) return nullptr;
		D3D11GSPointer* pointer = new D3D11GSPointer();
		pointer->set_d3d11_gs(d3d11_shader);
		GSPtr geometry_shader(new GeometryShader());
		geometry_shader->set_pointer(pointer);

#ifdef _DEBUG
		std::string name("UID:");
		name += " type:";
		name += "geometry_shader";
		d3d11_shader->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			static_cast<UINT>(name.length()), name.c_str());
#endif

		return geometry_shader;
	}

	//compute shaders:
	void D3D11_1LLGI::enable_compute_shader(CSPtr cs){
		ID3D11ComputeShader* d3d11_cs = get_d3d11_pointer(cs);
		m_context->CSSetShader(d3d11_cs, nullptr, 0);
	}

	void D3D11_1LLGI::disable_compute_shader(CSPtr cs){
		m_context->CSSetShader(nullptr, nullptr, 0);
	}

	void D3D11_1LLGI::release_compute_shader(CSPtr cs){
		ID3D11ComputeShader* d3d11_cs = get_d3d11_pointer(cs);
		d3d11_cs->Release();
	}

	CSPtr D3D11_1LLGI::create_compute_shader(
		const char* shader_text, const char* file_name)
	{
		ID3D11ComputeShader* d3d11_compute_shader;
		ID3DBlob* shader_blob = nullptr;
		ID3DBlob* error_blob = nullptr;
		DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		shader_flags |= D3DCOMPILE_DEBUG;
#endif
		HRESULT hr = D3DCompile(shader_text,
			strlen(shader_text), file_name,
			nullptr, nullptr, "main", m_cs_target,
			shader_flags, 0, &shader_blob, &error_blob);
		if (error_blob != nullptr){
			OutputDebugStringA((char*) error_blob->GetBufferPointer());
			error_blob->Release();
		}
		if (FAILED(hr)) return nullptr;

		hr = m_device->CreateComputeShader(
			shader_blob->GetBufferPointer(),
			shader_blob->GetBufferSize(), nullptr,
			&d3d11_compute_shader);
		shader_blob->Release();
		if (FAILED(hr)) return nullptr;

		D3D11CSPointer* pointer = new D3D11CSPointer();
		pointer->set_d3d11_cs(d3d11_compute_shader);
		CSPtr compute_shader(new ComputeShader());
		compute_shader->set_pointer(pointer);

#ifdef _DEBUG
		char uid_str[100];
		std::string name("UID:");
		name += uid_str;
		name += " type:";
		name += "compute_shader";
		d3d11_compute_shader->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			static_cast<UINT>(name.length()), name.c_str());
#endif

		return compute_shader;
	}

	CSPtr D3D11_1LLGI::create_compute_shader_from_binary(
		void* binary_code, U32 binary_code_size,
		const char* file_name)
	{
		ID3D11ComputeShader* d3d11_shader;
		HRESULT hr = m_device->CreateComputeShader(
			binary_code, binary_code_size, nullptr, &d3d11_shader);
		if (FAILED(hr)) return nullptr;
		D3D11CSPointer* pointer = new D3D11CSPointer();
		pointer->set_d3d11_cs(d3d11_shader);
		CSPtr result(new ComputeShader);
		result->set_pointer(pointer);

#ifdef _DEBUG
		char uid_str[100];
		std::string name("UID:");
		name += uid_str;
		name += " type:";
		name += "compute_shader";
		d3d11_shader->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			static_cast<UINT>(name.length()), name.c_str());
#endif

		return result;
	}

	CSPtr D3D11_1LLGI::create_compute_shader_from_header(
		const void* data, U32 size, const char* file_name)
	{
		ID3D11ComputeShader* d3d11_shader;
		HRESULT hr = m_device->CreateComputeShader(
			data, size, nullptr, &d3d11_shader);
		if (FAILED(hr)) return nullptr;
		D3D11CSPointer* pointer = new D3D11CSPointer();
		pointer->set_d3d11_cs(d3d11_shader);
		CSPtr result (new ComputeShader);
		result->set_pointer(pointer);

#ifdef _DEBUG
		char uid_str[100];
		std::string name("UID:");
		name += uid_str;
		name += " type:";
		name += "compute_shader";
		d3d11_shader->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			static_cast<UINT>(name.length()), name.c_str());
#endif

		return result;
	}

	ID3D11ComputeShader* D3D11_1LLGI::get_d3d11_pointer(CSPtr cs){
		D3D11CSPointer* pointer = dynamic_cast<D3D11CSPointer*>(
			cs->pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->d3d11_cs();
	}

	//rw-buffers & textures
	void* D3D11_1LLGI::map(RWBufferPtr rw_buffer){
#ifdef _DEBUG
		if (rw_buffer->buffer_usage() !=
			NM_BUFFER_USAGE_DYNAMIC)
			throw Exception("can't map resource");
#endif
		ID3D11Buffer* d3d11_buffer =
			get_d3d11_pointer(rw_buffer);
		D3D11_MAP map_type =
			D3D11_MAP_WRITE_DISCARD;
		UINT map_flags = 0;
		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		HRESULT hr = m_context->Map(d3d11_buffer, 0,
			map_type, map_flags, &mapped_resource);
		if (FAILED(hr)) return nullptr;
		return mapped_resource.pData;
	}

	void D3D11_1LLGI::unmap(RWBufferPtr rw_buffer){
		ID3D11Buffer* d3d11_buffer =
			get_d3d11_pointer(rw_buffer);
		m_context->Unmap(d3d11_buffer, 0);
	}

	void D3D11_1LLGI::bind_rw_buffer_to_read(U32 slot, RWBufferPtr buffer){
		ID3D11ShaderResourceView* d3d11_srv = nullptr;
		if (nullptr != buffer)
			d3d11_srv = get_srv(buffer);
		m_context->CSSetShaderResources(slot, 1, &d3d11_srv);
	}

	void D3D11_1LLGI::bind_rw_buffer_to_write(U32 slot, RWBufferPtr buffer){
		ID3D11UnorderedAccessView* d3d11_uav = nullptr;
		if (nullptr != buffer)
			d3d11_uav = get_uav(buffer);
		m_context->CSSetUnorderedAccessViews(slot, 1, &d3d11_uav, nullptr);
	}

	void D3D11_1LLGI::bind_rw_texture_to_write(U32 slot, RWTex2DPtr texture){
		ID3D11UnorderedAccessView* d3d11_uav = nullptr;
		if (nullptr != texture)
			d3d11_uav = get_uav(texture);
		m_context->CSSetUnorderedAccessViews(slot, 1, &d3d11_uav, nullptr);
	}

	void D3D11_1LLGI::release_rw_buffer(RWBufferPtr buffer){
		ID3D11UnorderedAccessView* d3d11_uav = get_uav(buffer);
		ID3D11ShaderResourceView* d3d11_srv = get_srv(buffer);
		ID3D11Buffer* d3d11_buffer = get_d3d11_pointer(buffer);
		d3d11_uav->Release();
		d3d11_srv->Release();
		d3d11_buffer->Release();
	}

	void D3D11_1LLGI::release_rw_texture(RWTex2DPtr texture){
		ID3D11UnorderedAccessView* d3d11_uav = get_uav(texture);
		d3d11_uav->Release();
		release_texture_2D(texture);
	}

	RWBufferPtr D3D11_1LLGI::create_rw_buffer(
		U32 element_size, U32 element_number,
		BufferUsage buffer_usage)
	{
		ID3D11Buffer* d3d11_buffer;
		ID3D11UnorderedAccessView* d3d11_uav = nullptr;
		ID3D11ShaderResourceView* d3d11_srv;
		bool dynamic = (buffer_usage == NM_BUFFER_USAGE_DYNAMIC);
		D3D11_BUFFER_DESC buffer_desc;
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		buffer_desc.ByteWidth = element_size * element_number;
		buffer_desc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
		buffer_desc.Usage =
			dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		buffer_desc.StructureByteStride = element_size;
		HRESULT hr = m_device->CreateBuffer(
			&buffer_desc, nullptr, &d3d11_buffer);
		if (FAILED(hr)) return nullptr;

		/*D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
		uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uav_desc.Buffer.FirstElement = 0;
		uav_desc.Buffer.NumElements = element_number;
		m_device->CreateUnorderedAccessView(d3d11_buffer,
		&uav_desc, &d3d11_uav);
		if(FAILED(hr)){
		d3d11_buffer->Release();
		return nullptr;
		}*/

		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
		ZeroMemory(&srv_desc, sizeof(srv_desc));
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srv_desc.Buffer.FirstElement = 0;
		srv_desc.Buffer.NumElements = element_number;
		srv_desc.Buffer.ElementOffset = 0;
		srv_desc.Buffer.ElementWidth = element_number;
		hr = m_device->CreateShaderResourceView(d3d11_buffer,
			&srv_desc, &d3d11_srv);
		if (FAILED(hr)){
			d3d11_buffer->Release();
			d3d11_uav->Release();
			return nullptr;
		}

		D3D11BOPointer* buffer_object_pointer =
			new D3D11BOPointer();
		D3D11RWBufferPointer* rw_pointer = new D3D11RWBufferPointer();
		rw_pointer->set_d3d11_uav(d3d11_uav);
		rw_pointer->set_d3d11_srv(d3d11_srv);
		buffer_object_pointer->set_d3d11_buffer(d3d11_buffer);
		RWBufferPtr rw_buffer(
			new RWBuffer(element_size, element_number, buffer_usage));
		rw_buffer->set_pointer(buffer_object_pointer);
		rw_buffer->set_rw_pointer(rw_pointer);

#ifdef _DEBUG
		char uid_str[100];
		std::string name("UID:");
		name += uid_str;
		name += " type:";
		std::string srv_name = name + "srv";
		std::string uav_name = name + "uav";
		name += "rw_buffer";
		d3d11_buffer->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			static_cast<UINT>(name.length()), name.c_str());
		d3d11_srv->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			static_cast<UINT>(srv_name.length()), srv_name.c_str());
		/*d3d11_uav->SetPrivateData(
		WKPDID_D3DDebugObjectName,
		uav_name.length(), uav_name.c_str());*/
#endif

		return rw_buffer;
	}

	RWTex2DPtr D3D11_1LLGI::create_rw_texture(
		U32 width, U32 height, TextureElementType element_type)
	{
		ID3D11Texture2D* d3d11_texture;
		ID3D11ShaderResourceView* d3d11_view;
		ID3D11UnorderedAccessView* d3d11_uav;
		D3D11_TEXTURE2D_DESC texture_desc;
		ZeroMemory(&texture_desc, sizeof(texture_desc));
		texture_desc.ArraySize = 1;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE |
			D3D11_BIND_UNORDERED_ACCESS;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.Format = to_dxgi_format(element_type);
		texture_desc.Height = height;
		texture_desc.Width = width;
		texture_desc.MipLevels = 1;;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		HRESULT hr = S_OK;
		hr = m_device->CreateTexture2D(
			&texture_desc, nullptr, &d3d11_texture);
		if (FAILED(hr)) return nullptr;

		D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
		view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		view_desc.Format = to_dxgi_format(element_type);
		view_desc.Texture2D.MipLevels = 1;;
		view_desc.Texture2D.MostDetailedMip = 0;
		hr = m_device->CreateShaderResourceView(
			d3d11_texture, &view_desc, &d3d11_view);
		if (FAILED(hr)){
			d3d11_texture->Release();
			return nullptr;
		}

		D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
		uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uav_desc.Format = to_dxgi_format(element_type);
		uav_desc.Texture2D.MipSlice = 0;
		hr = m_device->CreateUnorderedAccessView(
			d3d11_texture, &uav_desc, &d3d11_uav);
		if (FAILED(hr)){
			d3d11_texture->Release();
			d3d11_view->Release();
			return nullptr;
		}

		D3D11Texture2DPointer* pointer =
			new D3D11Texture2DPointer;
		pointer->set_d3d11_texture_2d_pointer(d3d11_texture);
		D3D11RWTexture2DPointer* rw_pointer =
			new D3D11RWTexture2DPointer;
		rw_pointer->set_d3d11_uav(d3d11_uav);
		D3D11TexturePointer* view =
			new D3D11TexturePointer;
		view->set_d3d11_texture_pointer(d3d11_view);
		RWTex2DPtr texture_2d(new
			RWTexture2D(element_type, width, height, 1));
		texture_2d->set_texture_2d_pointer(pointer);
		texture_2d->set_texture_pointer(view);
		texture_2d->set_rw_pointer(rw_pointer);

#ifdef _DEBUG
		char uid_str[100];
		std::string name("UID:");
		name += uid_str;
		name += " type:";
		std::string texture_name =
			name + "texture";
		std::string view_name =
			name + "texture_view";
		std::string uav_name =
			name + "texture_uav";
		d3d11_texture->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			static_cast<UINT>(texture_name.length()), texture_name.c_str());
		d3d11_view->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			static_cast<UINT>(view_name.length()), view_name.c_str());
		d3d11_uav->SetPrivateData(
			WKPDID_D3DDebugObjectName,
			static_cast<UINT>(view_name.length()), view_name.c_str());
#endif

		return texture_2d;
	}

	void D3D11_1LLGI::bind_rt_to_rw(U32 slot){
		ID3D11RenderTargetView* null = nullptr;
		m_context->OMSetRenderTargets(1,
			&null, nullptr);
		m_context->CSSetUnorderedAccessViews(
			slot, 1, &m_rt_uav, nullptr);
	}

	void D3D11_1LLGI::unbind_rt_from_rw(U32 slot){
		ID3D11UnorderedAccessView* null_arr = nullptr;
		ID3D11DepthStencilView* depth_stencil_view =
			get_d3d11_pointer(m_default_depth_stencil);
		m_context->CSSetUnorderedAccessViews(
			slot, 1, &null_arr, nullptr);
		m_context->OMSetRenderTargets(1,
			&m_default_render_target, depth_stencil_view);
	}

	void D3D11_1LLGI::dispatch(U32 x, U32 y, U32 z){
		m_context->Dispatch(x, y, z);
	}

	ID3D11ShaderResourceView* D3D11_1LLGI::get_srv(RWBufferPtr buffer){
		D3D11RWBufferPointer* pointer =
			dynamic_cast<D3D11RWBufferPointer*>(buffer->rw_pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->d3d11_srv();
	}

	ID3D11UnorderedAccessView* D3D11_1LLGI::get_uav(RWBufferPtr buffer){
		D3D11RWBufferPointer* pointer =
			dynamic_cast<D3D11RWBufferPointer*>(buffer->rw_pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->d3d11_uav();
	}

	ID3D11UnorderedAccessView* D3D11_1LLGI::get_uav(RWTex2DPtr texture){
		D3D11RWTexture2DPointer* pointer =
			dynamic_cast<D3D11RWTexture2DPointer*>(texture->rw_pointer());
		if (pointer == nullptr) throw Exception("wrong type of resource");
		return pointer->d3d11_uav();
	}
}