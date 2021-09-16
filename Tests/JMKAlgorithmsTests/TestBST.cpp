#include "pch.h"
#include "Core\Primitives\Point.h"
#include "Core\GeoUtils.h"
#include "Core\Primitives\PolygonDCEL.h"
#include "Core\Base\Vector.h"
#include "Trees\BinarySearchTree.h"

#include <functional>

TEST(BST_Construction_with_list1, BSTTest)
{
	float value = 9.0f;
	float value_2 = 10.f;
	jmk::BST<float,float> bst_test(value, value_2);
	EXPECT_TRUE(true);
}


TEST(BST_Construction_with_list2, BSTTest)
{
	EXPECT_TRUE(true);
}