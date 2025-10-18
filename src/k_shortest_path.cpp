#include "k_shortest_path.h"

// ---------------------- Biến toàn cục ----------------------
int n, m, K;           // n: số đỉnh, m: số cạnh, K: số đường ngắn nhất cần tìm
vector<vector<Edge>> G; // danh sách kề của đồ thị (G[u] = danh sách cạnh từ u)

// ---------------------- Dijkstra có loại trừ cạnh/nút ----------------------
Path dijkstra(int s, int t, 
              const set<pair<int,int>> &bannedEdges,  // tập các cạnh bị cấm
              const set<int> &bannedNodes) {          // tập các đỉnh bị cấm

    vector<long long> dist(n+1, LLONG_MAX); // dist[v]: chi phí nhỏ nhất từ s đến v
    vector<int> prev(n+1, -1);              // prev[v]: nút trước v trong đường ngắn nhất
    priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;

    dist[s] = 0;                            // khởi tạo khoảng cách từ s = 0
    pq.push({0, s});                        // bắt đầu từ s

    // Dijkstra tiêu chuẩn
    while (!pq.empty()) {
        auto [du, u] = pq.top(); pq.pop();
        if (du != dist[u]) continue;        // bỏ qua nếu đã có giá trị tốt hơn
        if (u == t) break;                  // dừng sớm nếu đã đến t

        // duyệt tất cả cạnh kề u -> v
        for (auto &e : G[u]) {
            // bỏ qua nếu cạnh hoặc đỉnh bị cấm
            if (bannedEdges.count({u,e.to}) || bannedNodes.count(e.to)) continue;

            // nếu tìm được đường đi ngắn hơn
            if (dist[e.to] > du + e.cost) {
                dist[e.to] = du + e.cost;
                prev[e.to] = u;
                pq.push({dist[e.to], e.to});
            }
        }
    }

    // Nếu không tìm thấy đường đi
    if (dist[t] == LLONG_MAX) return {{}, LLONG_MAX};

    // Khôi phục đường đi bằng cách lần ngược prev[]
    vector<int> path;
    for (int cur = t; cur != -1; cur = prev[cur])
        path.push_back(cur);
    reverse(path.begin(), path.end()); // đảo ngược để có thứ tự từ s → t

    return {path, dist[t]}; // trả về đường đi và chi phí
}

// ---------------------- Yen’s Algorithm ----------------------
vector<Path> yenKShortestPaths(int s, int t, int K) {
    vector<Path> A; // tập A: chứa các đường đi tốt nhất đã tìm thấy
    priority_queue<Path, vector<Path>, greater<Path>> B; // tập B: các ứng viên

    // 1. Tìm đường đi ngắn nhất đầu tiên bằng Dijkstra
    Path first = dijkstra(s, t, {}, {});
    if (first.cost == LLONG_MAX) return {}; // không có đường đi nào

    A.push_back(first); // thêm vào danh sách kết quả

    // 2️. Tìm tiếp các đường đi 2..K
    for (int k = 1; k < K; ++k) {
        const Path &prevPath = A[k-1]; // đường đi trước đó (ngắn nhất thứ k-1)

        // duyệt từng nút trong đường đi trước đó để tạo "spur node"
        for (int i = 0; i + 1 < (int)prevPath.nodes.size(); ++i) {
            int spurNode = prevPath.nodes[i]; // nút chia nhánh
            vector<int> rootPath(prevPath.nodes.begin(), 
                                 prevPath.nodes.begin() + i + 1); // phần đầu (root)

            set<pair<int,int>> bannedEdges; // tập cạnh bị loại
            set<int> bannedNodes;           // tập đỉnh bị loại

            // 3. Loại bỏ các cạnh trùng root path trong các đường đi A
            for (auto &p : A) {
                // Nếu phần đầu của p trùng với root path
                if (p.nodes.size() > i && 
                    equal(rootPath.begin(), rootPath.end(), p.nodes.begin()))
                    bannedEdges.insert({p.nodes[i], p.nodes[i+1]}); // loại cạnh ngay sau đó
            }

            // 4. Cấm các node trong root (trừ spurNode)
            for (int node : rootPath) {
                if (node != spurNode)
                    bannedNodes.insert(node);
            }

            // 5. Tìm spur path từ spurNode → t (tránh các cạnh/nút bị cấm)
            Path spurPath = dijkstra(spurNode, t, bannedEdges, bannedNodes);
            if (spurPath.cost == LLONG_MAX) continue; // không tồn tại spur path

            // 6. Nối root + spur để tạo ứng viên đường mới
            vector<int> totalPath = rootPath;
            totalPath.pop_back(); // bỏ spurNode trùng
            totalPath.insert(totalPath.end(), spurPath.nodes.begin(), spurPath.nodes.end());

            // 7. Tính tổng chi phí của đường đi mới
            long long totalCost = 0;
            for (int j = 0; j + 1 < (int)totalPath.size(); ++j) {
                for (auto &e : G[totalPath[j]])
                    if (e.to == totalPath[j+1])
                        totalCost += e.cost;
            }

            // Thêm vào danh sách ứng viên
            B.push({totalPath, totalCost});
        }

        // 8. Nếu không còn ứng viên thì dừng
        if (B.empty()) break;

        // 9. Chọn đường ngắn nhất trong các ứng viên để thêm vào A
        A.push_back(B.top());
        B.pop();
    }

    return A; // trả về K đường đi ngắn nhất
}

