#pragma once

#include "Mesh.h"

namespace Necromancer{
	class DynamicLineSetMesh :public Mesh{
	public:
		DynamicLineSetMesh();
		virtual ~DynamicLineSetMesh();

		virtual void enable(RenderContext*);
		virtual void disable(RenderContext*);

		void attach_line(const Vec3& start, 
			const Vec3& end, const Vec4& color);
		void attach_line(const Vec3& start, Vec4& start_color,
			const Vec3& end, const Vec4& end_color);
		void clean_up();
	private:
		void update_vertices();
	private:
		static const U32 ks_line_data_size = 14;
		static const U32 ks_max_line_num = 128;
		float m_data[ks_line_data_size * ks_max_line_num];
		U32 m_current_line_num;
		GraphicStatePtr m_graphic_state;
	};
	typedef SmartPtr<DynamicLineSetMesh>::Type DynamicLineSetMeshPtr;
}

