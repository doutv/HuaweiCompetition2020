#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <array>
#include <cmath>
#include <unordered_map>
using namespace std;

// #define LINUXOUTPUT
// #define OUTPUT
// #define TEST

#ifdef TEST
#include <chrono>
auto time_start = chrono::steady_clock::now();
string test_scale = "9153";
string input_path = "./data/" + test_scale + "/test_data.txt";
string output_path = input_path.substr(0, input_path.rfind('/')) + "/output.txt";
#else
string input_path = "/data/test_data.txt";
string output_path = "/projects/student/result.txt";
#endif

const int MAX_EDGE = 2000005;
typedef long long ll;
const int MAX_IN_DEGREE = 101;
const int MAX_OUT_DEGREE = 201;
int GUV[MAX_EDGE][MAX_OUT_DEGREE][2];
int GVU[MAX_EDGE][MAX_IN_DEGREE][2];
int edge_size;

bool visited[MAX_EDGE];
int flag[MAX_EDGE];
bool is_end[MAX_EDGE];
int node_size;
int node[MAX_EDGE * 2];
unordered_map<int, int> node_hashmap;
float c_prenode_to_node[MAX_EDGE];

typedef array<int, 8> ans_t;
int ans_size;
// const int ANS_MAX = 20000005;
const int ANS_MAX = 4000005;
ans_t ans[ANS_MAX];

int u_arr[MAX_EDGE];
int v_arr[MAX_EDGE];
int c_arr[MAX_EDGE];

namespace IO
{
const int MAXSIZE = 1 << 20;
char buf[MAXSIZE], *p1, *p2;
#define gc() (p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, MAXSIZE, stdin), p1 == p2) ? EOF : *p1++)
inline int rd()
{
    int x = 0;
    int16_t c = gc();
    while (!isdigit(c))
    {
        if (c == EOF)
            return c;
        c = gc();
    }
    while (isdigit(c))
        x = x * 10 + (c ^ 48), c = gc();
    return x;
}
inline void rd_to_line_end()
{
    int16_t c = gc();
    while (c != '\n')
        c = gc();
}
char pbuf[MAXSIZE], *pp = pbuf;
inline void push(const char &c)
{
    if (pp - pbuf == MAXSIZE)
        fwrite(pbuf, 1, MAXSIZE, stdout), pp = pbuf;
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
    freopen(input_path.c_str(), "r", stdin);
    int u, v, c;
    int ch;
    register int i;
    while (1)
    {
        u = IO::rd();
        if (u == EOF)
            break;
        v = IO::rd();
        c = IO::rd();
        node[++node_size] = u;
        node[++node_size] = v;
        ++edge_size;
        u_arr[edge_size] = u;
        v_arr[edge_size] = v;
        c_arr[edge_size] = c;
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
        ++GUV[u][0][0];
        GUV[u][GUV[u][0][0]][0] = v_arr[i];
        GUV[u][GUV[u][0][0]][1] = c_arr[i];
        ++GVU[v][0][0];
        GVU[v][GVU[v][0][0]][0] = u_arr[i];
        GVU[v][GVU[v][0][0]][1] = c_arr[i];
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

void flag_reverse_dfs(int u, int depth, int target, float nxtc)
{
    // 标记倒走4步以内能到达的点
    if (depth <= 4)
    {
        register int i;
        int v;
        float nowc;
        float frac;
        for (i = 1; i <= GVU[u][0][0]; i++)
        {
            v = node_hashmap[GVU[u][i][0]];
            nowc = GVU[u][i][1];
            frac = nxtc / nowc;
            if (frac < 0.2 || frac > 3.0)
                continue;
            if (!visited[v] && GVU[u][i][0] > target)
            {
                visited[v] = 1;
                flag[v] = target;
                flag_reverse_dfs(v, depth + 1, target, nowc);
                visited[v] = 0;
            }
        }
    }
}

void dfs(int u, int depth, ans_t &path, int target, float prec)
{
    // pre--prec-->u--nowc-->v
    register int i;
    int v;
    float nowc, frac;
    for (i = 1; i <= GUV[u][0][0]; i++)
    {
        if (GUV[u][i][0] <= target)
            continue;
        nowc = GUV[u][i][1];
        frac = nowc / prec;
        if (frac < 0.2 || frac > 3.0)
            continue;
        v = node_hashmap[GUV[u][i][0]];
        if (is_end[v] && visited[v] == 0)
        {
            frac = c_prenode_to_node[v] / nowc;
            if (frac >= 0.2 && frac <= 3.0)
            {
                path[0] = depth + 1;
                path[depth + 1] = GUV[u][i][0];
                ans[++ans_size] = path;
            }
        }
        if (flag[v] != target && !is_end[v] && depth >= 4)
            continue;
        if (!visited[v] && depth <= 5)
        {
            visited[v] = 1;
            path[depth + 1] = GUV[u][i][0];
            dfs(v, depth + 1, path, target, nowc);
            visited[v] = 0;
        }
    }
}

inline void iter_st_from_node(int u, int target)
{
    // pre--prec-->u--nowc-->nxt
    ans_t path;
    register int i, j;
    float prec, nowc, frac;
    int pre, nxt;
    for (i = 1; i <= GUV[u][0][0]; i++)
    {
        if (GUV[u][i][0] < target || GVU[u][0][0] == 0)
            continue;
        nowc = GUV[u][i][1];
        memset(is_end, 0, node_size + 5);
        for (j = 1; j <= GVU[u][0][0]; j++)
        {
            if (GVU[u][j][0] < target)
                continue;
            prec = GVU[u][j][1];
            pre = node_hashmap[GVU[u][j][0]];
            frac = nowc / prec;
            if (frac >= 0.2 && frac <= 3.0)
            {
                c_prenode_to_node[pre] = prec;
                is_end[pre] = 1;
                visited[pre] = 1;
                flag_reverse_dfs(pre, 2, target, prec);
                visited[pre] = 0;
            }
        }
        path[1] = target;
        path[2] = GUV[u][i][0];
        nxt = node_hashmap[GUV[u][i][0]];
        visited[nxt] = 1;
        dfs(nxt, 2, path, target, nowc);
        visited[nxt] = 0;
    }
}
inline void work()
{
    int target;
    register int i, j;
    for (i = 1; i <= node_size; i++)
    {
        target = node[i];
        iter_st_from_node(i, target);
    }
}
inline void output_data()
{
    register int i, j;
    freopen(output_path.c_str(), "w", stdout);
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
            IO::push(',');
        }
        IO::write(ans[i][ans[i][0]]);
        IO::push('\n');
    }
    fwrite(IO::pbuf, 1, IO::pp - IO::pbuf, stdout);
#ifdef TEST
#ifdef LINUXOUTPUT
    freopen("/dev/tty", "w", stdout);
#else
    freopen("CON", "w", stdout);
#endif
    auto output_time_end = chrono::steady_clock::now();
    auto output_time_diff = output_time_end - output_time_start;
    cout << "output cost: " << chrono::duration<double, milli>(output_time_diff).count() / 1000 << "s" << endl;
#endif
    return;
}
int main()
{
#ifdef TEST
    cout << "Now running on data " + test_scale << endl;
#endif
    read_data();
    work();
    output_data();
#ifdef TEST
    auto time_end = chrono::steady_clock::now();
    auto diff = time_end - time_start;
#ifdef LINUXOUTPUT
    freopen("/dev/tty", "w", stdout);
#else
    freopen("CON", "w", stdout);
#endif
    printf("ans size is %d\n", ans_size);
    cout << "The program's speed: " << chrono::duration<double, milli>(diff).count() / 1000 << "s" << endl;
#endif
    fclose(stdin);
    fclose(stdout);
    return 0;
}