#pragma once

#include "CommonUtility.h"
#include "RenderingResource.h"

namespace Necromancer{
	class GBuffer{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, width);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, height);
	public:
		GBuffer(U32 width, U32 height);

		virtual ~GBuffer();

		void bind_to_read();
		void bind_to_write();
		void unbind_to_read();
		void unbind_to_write();
	private:
		static const U32 ks_texture_number = 3;
		static const U32 ks_depth_buffer_slot = 5;
		static const U32 ks_texture_start_slot = 2;

		RDSPtr m_depth_buffer;
		RTex2DPtr m_textures[ks_texture_number];
	};
}

