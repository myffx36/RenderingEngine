#pragma once

#include "CommonUtility.h"
#include "Light.h"
#include "Camera.h"

namespace Necromancer{
	class GBuffer;
	class DeferredShader{
	public:
		virtual void deferred_shading(
			const Light* lights, U32 light_number,
			GBuffer* g_buffer, bool draw_to_frame_buffer,
			CameraPtr camera) = 0;

		virtual ~DeferredShader();
	private:
	};
}

