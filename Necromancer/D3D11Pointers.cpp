#include "stdafx.h"
#include "D3D11Pointers.h"

namespace Necromancer{
	namespace{
		template<typename T>
		void release_resource(T& ptr){
			if(ptr != nullptr){
				ptr->Release();
				ptr = nullptr;
			}
		}
	}

	D3D11BOPointer::~D3D11BOPointer(){
		release_resource(m_d3d11_buffer);
	}

	D3D11VSPointer::~D3D11VSPointer(){
		release_resource(m_d3d11_vs);
	}

	D3D11FSPointer::~D3D11FSPointer(){
		release_resource(m_d3d11_fs);
	}

	D3D11TexturePointer::~D3D11TexturePointer(){
		release_resource(m_d3d11_texture_pointer);
	}

	D3D11Texture2DPointer::~D3D11Texture2DPointer(){
		release_resource(m_d3d11_texture_2d_pointer);
	}

	D3D11TextureCubePointer::~D3D11TextureCubePointer(){
		release_resource(m_d3d11_texture_cube_pointer);
	}

	D3D11RenderTargetPointer::~D3D11RenderTargetPointer(){
		release_resource(m_d3d11_render_target_pointer);
	}

	D3D11DepthStencilPointer::~D3D11DepthStencilPointer(){
		release_resource(m_d3d11_depth_stencil_pointer);
		release_resource(m_d3d11_texture);
	}

	D3D11RDSPointer::~D3D11RDSPointer(){
	}

	D3D11WRTPointer::~D3D11WRTPointer(){
		release_resource(m_swap_chian);
	}

	D3D11GraphicStatePointer::~D3D11GraphicStatePointer(){
		release_resource(m_as_pointer);
		release_resource(m_ds_state_pointer);
		release_resource(m_rs_state_pointer);
		release_resource(m_ss_state_pointer);
	}

	D3D11GSPointer::~D3D11GSPointer(){
		release_resource(m_d3d11_gs);
	}

	ID3D11GeometryShader* D3D11GSPointer::d3d11_gs() const{
		return m_d3d11_gs;
	}

	void D3D11GSPointer::set_d3d11_gs(ID3D11GeometryShader* d3d11_gs){
		m_d3d11_gs = d3d11_gs;
	}

	D3D11CSPointer::~D3D11CSPointer(){
		release_resource(m_d3d11_cs);
	}

	ID3D11ComputeShader* D3D11CSPointer::d3d11_cs() const{
		return m_d3d11_cs;
	}

	void D3D11CSPointer::set_d3d11_cs(ID3D11ComputeShader* d3d11_cs){
		m_d3d11_cs = d3d11_cs;
	}

	D3D11RWBufferPointer::~D3D11RWBufferPointer(){
		release_resource(m_d3d11_uav);
		release_resource(m_d3d11_srv);
	}

	ID3D11UnorderedAccessView* D3D11RWBufferPointer::d3d11_uav(){
		return m_d3d11_uav;
	}

	void D3D11RWBufferPointer::set_d3d11_uav(
		ID3D11UnorderedAccessView* d3d11_uav)
	{
		m_d3d11_uav = d3d11_uav;
	}

	ID3D11ShaderResourceView* D3D11RWBufferPointer::d3d11_srv(){
		return m_d3d11_srv;
	}

	void D3D11RWBufferPointer::set_d3d11_srv(
		ID3D11ShaderResourceView* d3d11_srv)
	{
		m_d3d11_srv = d3d11_srv;
	}

	D3D11RWTexture2DPointer::~D3D11RWTexture2DPointer(){
		release_resource(m_d3d11_uav);
	}

	ID3D11UnorderedAccessView* D3D11RWTexture2DPointer::d3d11_uav(){
		return m_d3d11_uav;
	}

	void D3D11RWTexture2DPointer::set_d3d11_uav(
		ID3D11UnorderedAccessView* d3d11_uav)
	{
		m_d3d11_uav = d3d11_uav;
	}
}