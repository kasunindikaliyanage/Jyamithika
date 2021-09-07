#include "pch.h"
#include "Core\Primitives\Point.h"
#include "Core\GeoUtils.h"
#include "Core\Primitives\PolygonDCEL.h"
#include "Core\Base\Vector.h"
#include "Trees\BinarySearchTree.h"

#include <functional>

using namespace jmk;

TEST(BST_Construction_with_list1, BST)
{
	float value = 9.0f;
	jmk::BST<float> bst_test(value);
	EXPECT_TRUE(true);
}


TEST(BST_Construction_with_list2, BST)
{
	EXPECT_TRUE(true);
}