#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <array>
#include <chrono>
#include <cmath>
#include <map>
#include <unordered_map>
#include <unordered_set>
using namespace std;

const int INF = 280005;
typedef long long ll;

vector<int> GUV[INF];
vector<int> GVU[INF];
int edge_size;

bool visited[INF];
int flag[INF];
int node_size;
int node[INF * 2];
unordered_map<int, int> node_hashmap;

typedef array<int, 8> ans_t;
int ans_size;
ans_t ans[100005];

unordered_set<ll> ans_hashmap;
// 2^60上一个质数
const ll mod = 1152921504606846883;
int bit_size;

int u_arr[INF];
int v_arr[INF];
void read_data()
{
    string input_path = "/data/test_data.txt";
    freopen(input_path.c_str(), "r", stdin);
    int u, v, c;
    while (scanf("%u,%u,%u", &u, &v, &c) != EOF)
    {
        node[++node_size] = u;
        node[++node_size] = v;
        ++edge_size;
        u_arr[edge_size] = u;
        v_arr[edge_size] = v;
    }
    sort(node + 1, node + node_size + 1);
    node_size = unique(node + 1, node + node_size + 1) - node - 1;
    for (int i = 1; i <= node_size; i++)
    {
        node_hashmap[node[i]] = i;
    }
    bit_size = log(node_size) / log(2) + 1;
    for (int i = 1; i <= edge_size; i++)
    {
        int u = node_hashmap[u_arr[i]];
        int v = node_hashmap[v_arr[i]];
        GUV[u].push_back(v_arr[i]);
        GVU[v].push_back(u_arr[i]);
    }
}
bool cmp(ans_t &x, ans_t &y)
{
    int now = 0;
    while (x[now] == y[now])
        ++now;
    return x[now] < y[now];
}
void add_ans(ans_t path)
{
    ll len = path[0];
    ll hash_key = 1;
    for (ll i = 1; i <= len; i++)
    {
        ll hash_id = node_hashmap[path[i]];
        hash_key ^= hash_id << ((i - 1) * bit_size) % 60;
        hash_key %= mod;
    }
    hash_key += (ll)len << 60;
    if (ans_hashmap.find(hash_key) == ans_hashmap.end())
    {
        ans_hashmap.insert(hash_key);
        ans[++ans_size] = path;
    }
}
void flag_traverse_dfs(int u, int depth, int target)
{
    if (depth <= 2)
    {
        for (int i = 0; i < GUV[u].size(); i++)
        {
            int v = node_hashmap[GUV[u][i]];
            if (!visited[v] && GUV[u][i] > target)
            {
                visited[v] = 1;
                flag[v] = target;
                flag_traverse_dfs(v, depth + 1, target);
                visited[v] = 0;
            }
        }
    }
}
void flag_reverse_dfs(int u, int depth, int target)
{
    if (depth <= 2)
    {
        for (int i = 0; i < GVU[u].size(); i++)
        {
            int v = node_hashmap[GVU[u][i]];
            if (!visited[v] && GVU[u][i] > target)
            {
                visited[v] = 1;
                flag[v] = target;
                flag_reverse_dfs(v, depth + 1, target);
                visited[v] = 0;
            }
        }
    }
}
void dfs(int u, int depth, ans_t &path, int target)
{
    for (int i = 0; i < GUV[u].size(); i++)
    {
        int v = node_hashmap[GUV[u][i]];
        if (flag[v] != target)
            continue;
        if (GUV[u][i] == target)
        {
            if (depth >= 3)
            {
                path[0] = depth;
                add_ans(path);
            }
        }
        else if (!visited[v] && depth <= 6)
        {
            visited[v] = 1;
            path[depth + 1] = GUV[u][i];
            dfs(v, depth + 1, path, target);
            visited[v] = 0;
        }
    }
}
void work()
{
    ans_t path;
    for (int i = 1; i <= node_size; i++)
    {
        int target = node[i];
        visited[i] = 1;
        flag[i] = target;
        flag_traverse_dfs(i, 0, target);
        flag_reverse_dfs(i, 0, target);
        path[1] = target;
        dfs(i, 1, path, target);
        visited[i] = 0;
    }
}
void out(int x)
{
    if (x > 9)
        out(x / 10);
    putchar(x % 10 + '0');
}
void output_data()
{
    string output_path = "/projects/student/result.txt";
    freopen(output_path.c_str(), "w", stdout);
    sort(ans + 1, ans + ans_size + 1, cmp);
    printf("%d\n", ans_size);
    for (int i = 1; i <= ans_size; i++)
    {
        int len = ans[i][0];
        for (int j = 1; j < len; j++)
        {
            out(ans[i][j]);
            putchar(',');
        }
        out(ans[i][len]);
        putchar('\n');
    }
    return;
}
int main()
{
    read_data();
    work();
    output_data();
    fclose(stdin);
    fclose(stdout);
    return 0;
}