#pragma once

namespace Necromancer{
	class SceneNode;
	class BoundingVolume{
	public:
		virtual ~BoundingVolume();

		virtual void update_bv(SceneNode*);
	private:
	};
}

