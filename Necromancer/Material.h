#pragma once

#include "Container.hpp"
#include "RenderingResource.h"
#include "CommonUtility.h"

#include "Container.hpp"

namespace Necromancer{
	class RenderContext;
	class FSProcess;
	class Material{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(GraphicStatePtr, state);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(CBPtr, cb);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Vec3, ambient);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Vec3, diffuse);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(Vec3, specular);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, shiness);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(String, name);
	public:
		Material();
		virtual ~Material();

		virtual void enable(RenderContext*);
		virtual void disable(RenderContext*);

		void attach_fs_process(U32, U32);
		void attach_texture(U32, const TexPtr&);
	private:
		struct FSProcessBinding{
			U32 uid;
			U32 seq;
		};

		struct TextureBinding{
			U32 slot;
			TexPtr texture;
		};

		List<FSProcessBinding>::Type m_fs_process_bindings;
		List<TextureBinding>::Type m_texture_bindings;

		static const U32 ks_cb_slot = 3;
	};

	typedef SmartPtr<Material>::Type MaterialPtr;
}

