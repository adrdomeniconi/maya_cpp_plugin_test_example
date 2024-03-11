
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <maya/MVector.h>

#include "../CenterPointNode.h"


TEST(TestCenterPointNode, TestFindCenterPoint)
{
    CenterPointNode centerPointNode;

    std::vector<MVector> inputPositions = {
        MVector(0.0, 0.0, 0.0),
        MVector(0.5, 10.3, 50.8),
        MVector(-15.5, -8.0, -30.0),
        MVector(34.4, 56.66, -45.0),
    };

    MVector expectedResult(4.850, 14.740, -6.050);
    MVector actualResult = centerPointNode.findCenterPoint(inputPositions);

    ASSERT_NEAR(expectedResult.x, actualResult.x, 0.01);
    ASSERT_NEAR(expectedResult.y, actualResult.y, 0.01);
    ASSERT_NEAR(expectedResult.z, actualResult.z, 0.01);

}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
