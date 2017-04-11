#pragma once

namespace Necromancer{
	class SceneNode;
	class NodeObserver{
	public:
		virtual ~NodeObserver();

		virtual void notify_update(SceneNode*) = 0;
	private:
	};
}

