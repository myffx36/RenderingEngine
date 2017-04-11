#pragma once

#include "RenderingResource.h"
#include "CommonUtility.h"

namespace Necromancer{
	class D3D11BOPointer:public BufferObjectPointer{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(ID3D11Buffer, d3d11_buffer);
	public:
		virtual ~D3D11BOPointer();
	};

	class D3D11VSPointer:public VertexShaderPointer{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(ID3D11VertexShader, d3d11_vs);
	public:
		virtual ~D3D11VSPointer();
	};

	class D3D11FSPointer:public FragmentShaderPointer{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(ID3D11PixelShader, d3d11_fs);
	public:
		virtual ~D3D11FSPointer();
	};

	class D3D11TexturePointer:public TexturePointer{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(
			ID3D11ShaderResourceView, d3d11_texture_pointer);
	public:
		virtual ~D3D11TexturePointer();
	};

	class D3D11Texture2DPointer:public Texture2DPointer{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(ID3D11Texture2D, 
			d3d11_texture_2d_pointer);
	public:
		virtual ~D3D11Texture2DPointer();
	};

	class D3D11TextureCubePointer:public TextureCubePointer{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(ID3D11Texture2D,
			d3d11_texture_cube_pointer);
	public:
		virtual ~D3D11TextureCubePointer();
	};

	class D3D11RenderTargetPointer:public RenderTargetPointer{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(ID3D11RenderTargetView,
			d3d11_render_target_pointer);
	public:
		virtual ~D3D11RenderTargetPointer();
	};

	class D3D11DepthStencilPointer:public DepthStencilPointer{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(ID3D11DepthStencilView,
			d3d11_depth_stencil_pointer);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(ID3D11Texture2D,
			d3d11_texture);
	public:
		virtual ~D3D11DepthStencilPointer();
	};

	class D3D11RDSPointer:public ReadableDepthStencilPointer{
	public:
		virtual ~D3D11RDSPointer();
	private:
	};

	class D3D11WRTPointer:public WindowRenderTargetPointer{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(IDXGISwapChain, swap_chian);
	public:
		virtual ~D3D11WRTPointer();
	};

	class D3D11GraphicStatePointer:public GraphicStatePointer{
		DEF_PTR_MEMBER_AND_GETTER_SETTER(ID3D11BlendState, as_pointer);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(ID3D11DepthStencilState, ds_state_pointer);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(ID3D11RasterizerState, rs_state_pointer);
		DEF_PTR_MEMBER_AND_GETTER_SETTER(ID3D11SamplerState, ss_state_pointer);
	public:
		virtual ~D3D11GraphicStatePointer();
	};

	class D3D11GSPointer :public GeometryShaderPointer{
	public:
		ID3D11GeometryShader* d3d11_gs() const;
		void set_d3d11_gs(ID3D11GeometryShader* d3d11_gs);

		virtual ~D3D11GSPointer();
	private:
		ID3D11GeometryShader* m_d3d11_gs;
	};

	class D3D11RWBufferPointer :public RWBufferPointer{
	public:
		virtual ~D3D11RWBufferPointer();

		ID3D11UnorderedAccessView* d3d11_uav();
		void set_d3d11_uav(ID3D11UnorderedAccessView*);

		ID3D11ShaderResourceView* d3d11_srv();
		void set_d3d11_srv(ID3D11ShaderResourceView*);
	private:
		ID3D11UnorderedAccessView* m_d3d11_uav;
		ID3D11ShaderResourceView* m_d3d11_srv;
	};

	class D3D11RWTexture2DPointer :public RWTexture2DPointer{
	public:
		virtual ~D3D11RWTexture2DPointer();

		ID3D11UnorderedAccessView* d3d11_uav();
		void set_d3d11_uav(ID3D11UnorderedAccessView*);
	private:
		ID3D11UnorderedAccessView* m_d3d11_uav;
	};

	class D3D11CSPointer :public ComputeShaderPointer{
	public:
		ID3D11ComputeShader* d3d11_cs() const;
		void set_d3d11_cs(ID3D11ComputeShader* d3d11_cs);

		virtual ~D3D11CSPointer();
	private:
		ID3D11ComputeShader* m_d3d11_cs;
	};
}

