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

auto time_start = chrono::steady_clock::now();

#define OUTPUT

#define TEST

string input_path = "/data/test_data.txt";
string output_path = "/projects/student/result.txt";
#ifdef TEST
string test_scale = "std";
string test_input_path_s = "./data/" + test_scale + "/test_data.txt";
string test_output_path_s = test_input_path_s.substr(0, test_input_path_s.rfind('/')) + "/output.txt";
#endif

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
ans_t ans[4000005];

int u_arr[INF];
int v_arr[INF];

namespace IO
{
const int MAXSIZE = 1 << 20;
char buf[MAXSIZE], *p1, *p2;
#define gc()                                                                 \
    (p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2) \
         ? EOF                                                               \
         : *p1++)
inline int rd()
{
    int x = 0;
    char c = gc();
    if (c == EOF)
        return -1;
    while (!isdigit(c))
    {
        c = gc();
    }
    while (isdigit(c))
        x = x * 10 + (c ^ 48), c = gc();
    return x;
}
char pbuf[1 << 20], *pp = pbuf;
inline void push(const char &c)
{
    if (pp - pbuf == 1 << 20)
        fwrite(pbuf, 1, 1 << 20, stdout), pp = pbuf;
    *pp++ = c;
}
inline void write(int x)
{
    static int sta[35];
    int top = 0;
    do
    {
        sta[top++] = x % 10, x /= 10;
    } while (x);
    while (top)
        push(sta[--top] + '0');
}
} // namespace IO

inline void read_data()
{
    freopen(test_input_path_s.c_str(), "r", stdin);
    int u, v;
    int ch;
    register int i;
    while (1)
    {
        u = IO::rd();
        if (u == -1)
            break;
        v = IO::rd();
        IO::rd();
        node[++node_size] = u;
        node[++node_size] = v;
        ++edge_size;
        u_arr[edge_size] = u;
        v_arr[edge_size] = v;
    }
    sort(node + 1, node + node_size + 1);
    node_size = unique(node + 1, node + node_size + 1) - node - 1;
    for (i = 1; i <= node_size; i++)
    {
        node_hashmap[node[i]] = i;
    }
    for (i = 1; i <= edge_size; i++)
    {
        u = node_hashmap[u_arr[i]];
        v = node_hashmap[v_arr[i]];
        GUV[u].push_back(v_arr[i]);
        GVU[v].push_back(u_arr[i]);
    }
#ifdef TEST
    auto input_time_end = chrono::steady_clock::now();
    auto input_time_diff = input_time_end - time_start;
    cout << "prehandle cost: " << chrono::duration<double, milli>(input_time_diff).count() / 1000 << "s" << endl;
#endif
}
inline bool cmp(ans_t &x, ans_t &y)
{
    int now = 0;
    while (x[now] == y[now])
        ++now;
    return x[now] < y[now];
}
void flag_traverse_dfs(int u, int depth, int target)
{
    if (depth <= 2)
    {
        register int i;
        int v;
        for (i = 0; i < GUV[u].size(); i++)
        {
            v = node_hashmap[GUV[u][i]];
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
        register int i;
        int v;
        for (i = 0; i < GVU[u].size(); i++)
        {
            v = node_hashmap[GVU[u][i]];
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
    register int i;
    int v;
    for (i = 0; i < GUV[u].size(); i++)
    {
        v = node_hashmap[GUV[u][i]];
        if (GUV[u][i] < target)
            continue;
        if (flag[v] == -2 && visited[v] == 0)
        {
            if (depth >= 2)
            {
                path[0] = depth + 1;
                path[depth + 1] = GUV[u][i];
                ans[++ans_size] = path;
            }
        }
        if (GUV[u][i] == target)
            continue;
        if (flag[v] != target && flag[v] != -2)
            continue;
        if (!visited[v] && depth <= 5)
        {
            visited[v] = 1;
            path[depth + 1] = GUV[u][i];
            dfs(v, depth + 1, path, target);
            visited[v] = 0;
        }
    }
}
inline void work()
{
    memset(flag, -1, node_size + 5);
    ans_t path;
    int v, target;
    register int i, j;
    for (i = 1; i <= node_size; i++)
    {
        target = node[i];
        flag_traverse_dfs(i, 0, target);
        flag_reverse_dfs(i, 0, target);
        for (j = 0; j < GVU[i].size(); j++)
        {
            v = node_hashmap[GVU[i][j]];
            flag[v] = -2;
        }
        path[1] = target;
        dfs(i, 1, path, target);
        for (j = 0; j < GVU[i].size(); j++)
        {
            v = node_hashmap[GVU[i][j]];
            flag[v] = target;
        }
    }
}
inline void out(int x)
{
    if (x > 9)
        out(x / 10);
    putchar(x % 10 + '0');
}

inline void output_data()
{
    register int i, j;
    freopen(test_output_path_s.c_str(), "w", stdout);
    sort(ans + 1, ans + ans_size + 1, cmp);
#ifdef TEST
    auto output_time_start = chrono::steady_clock::now();
#endif
    printf("%d\n", ans_size);
    for (i = 1; i <= ans_size; i++)
    {
        for (j = 1; j < ans[i][0]; j++)
        {
            IO::write(ans[i][j]);
            putchar(',');
        }
        IO::write(ans[i][ans[i][0]]);
        putchar('\n');
    }
#ifdef TEST
    freopen("CON", "w", stdout);
    auto output_time_end = chrono::steady_clock::now();
    auto output_time_diff = output_time_end - output_time_start;
    cout << "output cost: " << chrono::duration<double, milli>(output_time_diff).count() / 1000 << "s" << endl;
#endif
    return;
}
int main()
{
    read_data();
    work();
    output_data();
#ifdef TEST
    auto time_end = chrono::steady_clock::now();
    auto diff = time_end - time_start;
    freopen("CON", "w", stdout);
    printf("ans size is %d\n", ans_size);
    cout << "The program's speed: " << chrono::duration<double, milli>(diff).count() / 1000 << "s" << endl;
#endif
    fclose(stdin);
    fclose(stdout);
    return 0;
}