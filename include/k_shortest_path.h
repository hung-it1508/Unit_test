#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;
    long long cost;
};

struct Path {
    vector<int> nodes;
    long long cost;
    bool operator>(const Path &other) const { return cost > other.cost; }
};

extern int n, m, K;
extern vector<vector<Edge>> G;

Path dijkstra(int s, int t,
              const set<pair<int,int>> &bannedEdges,
              const set<int> &bannedNodes);
vector<Path> yenKShortestPaths(int s, int t, int K);
