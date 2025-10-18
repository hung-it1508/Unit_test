#include <gtest/gtest.h>
#include "k_shortest_path.h"

// Test đơn giản cho đồ thị nhỏ
TEST(KShortestPathTest, SimpleGraph) {
    n = 4; m = 5; K = 3;
    G.assign(n + 1, {});

    // Đồ thị có trọng số dương
    G[1].push_back({2, 1});
    G[1].push_back({3, 5});
    G[2].push_back({3, 1});
    G[2].push_back({4, 2});
    G[3].push_back({4, 1});

    // Tìm 3 đường đi ngắn nhất từ 1 → 4
    vector<Path> paths = yenKShortestPaths(1, 4, K);

    ASSERT_FALSE(paths.empty());
    EXPECT_EQ(paths[0].cost, 3);  // đường đi ngắn nhất: 1→2→4 (chi phí 3)
    EXPECT_EQ(paths[0].nodes, (vector<int>{1, 2, 4}));
}

// Test khi không có đường đi
TEST(KShortestPathTest, NoPath) {
    n = 3; m = 1; K = 2;
    G.assign(n + 1, {});
    G[1].push_back({2, 5});
    // Không có cạnh đến 3

    vector<Path> paths = yenKShortestPaths(1, 3, K);
    EXPECT_TRUE(paths.empty());
}
