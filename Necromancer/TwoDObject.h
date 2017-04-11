#pragma once

#include "CommonUtility.h"

#include "RenderingResource.h"
#include "BasicMath.h"

namespace Necromancer{
	class TwoDObject{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Tex2DPtr, texture);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(VBPtr, vertex_buffer);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(IBPtr, index_buffer);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(VSPtr, vertex_shader);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(FSPtr, fragment_shader);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Mat4, projection);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Mat4, world);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Vec2, position);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Vec2, size);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(CBPtr, constant_buffer);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(GraphicStatePtr, graphic_state);
	public:
		TwoDObject(const Tex2DPtr&, 
			const VBPtr& vb = nullptr, const IBPtr& ib = nullptr,
			const VSPtr& vs = nullptr, const FSPtr& fs = nullptr);

		virtual ~TwoDObject();

		virtual void enable();
		virtual void draw();
		virtual void disable();

		virtual void update();
	private:
		void calculate_world_matrix();
	private:
		static WeakPtr<VertexShader>::Type s_vertex_shader;
		static WeakPtr<FragmentShader>::Type s_fragment_shader;
		static WeakPtr<VertexBuffer>::Type s_vertex_buffer;
		static WeakPtr<IndexBuffer>::Type s_index_buffer;
		static WeakPtr<ConstantBuffer>::Type s_constant_buffer;
		static WeakPtr<GraphicState>::Type s_graphic_state;

		static const U32 ks_texture_slot = 0;
		static const U32 ks_cb_slot = 0;
	};
}

