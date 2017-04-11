#pragma once

#include "CommonUtility.h"
#include "Container.hpp"
#include "RenderingResource.h"

namespace Necromancer{
	class VSProcess;
	class FSProcess;
	class RenderContext{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(VBPtr, vertex_buffer);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(IBPtr, index_buffer);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(GeometryType, geometry_type);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Mat4, world_matrix);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Mat4, view_matrix);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Mat4, projection_matrix);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(VSPtr, vertex_shader);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(FSPtr, fragment_shader);
	public:
		~RenderContext();

		void attach_vs_process(U32 seq, VSProcess*);
		void attach_vs_process(U32 seq, U32 uid);
		void attach_fs_process(U32 seq, FSProcess*);
		void attach_fs_process(U32 seq, U32 uid);

		void set_constant_buffer(U32, const CBPtr&);
		void set_texture(U32, const TexPtr&);

		void generate_vertex_shader();
		void generate_fragment_shader();
		void generate_shaders();

		const Map<U32, CBPtr>::Type& cb_map();
		const Map<U32, TexPtr>::Type& tex_map();
	private:
		Map<U32, U32>::Type m_vs_process_map;
		Map<U32, U32>::Type m_fs_process_map;
		Map<U32, CBPtr>::Type m_cb_map;
		Map<U32, TexPtr>::Type m_tex_map;
	};
}

