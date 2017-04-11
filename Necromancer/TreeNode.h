#pragma once

#include "Container.hpp"

namespace Necromancer{
	class TreeNode{
	public:
		TreeNode(TreeNode* parent = nullptr);

		virtual ~TreeNode();
	private:
		Vector<TreeNode*>::Type m_children;
		TreeNode* m_parent;
	};
}

