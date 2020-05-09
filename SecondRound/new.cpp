#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>

using namespace std;

#define TEST
#define LINUXOUTPUT

#ifdef TEST
#include <chrono>
auto time_start = chrono::steady_clock::now();
string scale("9153");
string input_path = "./data/" + scale + "/test_data.txt";
string output_path = "./data/" + scale + "/output.txt";
#else
string input_path = "/data/test_data.txt";
string output_path = "/projects/student/result.txt";
#endif


typedef long long ll;
typedef unordered_map<ll, vector<ll>> unvec;
typedef array<ll, 8> ans_t;
const ll W = 4294967295;
vector<ans_t> ans;
unvec GUV, GVU;
unordered_map<ll, bool> visited;
unordered_map<ll, ll> flag;
int ans_size;

void read_data()
{
    long long u, v;
    int u1, v1, w;
    freopen(input_path.c_str(), "r", stdin);
    while (scanf("%d,%d,%d\n", &u1, &v1, &w) != EOF) {
        u = u1;
        v = v1;
        GUV[u].push_back((v << 32) + w);
        GVU[v].push_back((u << 32) + w);
        // cout << u << ';' << v << endl;
    }
#ifdef TEST
    auto input_time_end = chrono::steady_clock::now();
    auto input_time_diff = input_time_end - time_start;
    cout << "prehandle cost: " << chrono::duration<double, milli>(input_time_diff).count() / 1000 << "s" << endl;
#endif
}

void r_dfs(ll v, int depth, ll target, ll a_amount=0)
{
    if (depth > 2) {
        return;
    }
    ll u, w;
    for (ll &u_w: GVU[v]) {
        u = u_w >> 32;
        w = u_w & W;
        if (u <= target) {
            continue;
        }
        if (visited.find(u) != visited.end()) {
            continue;
        }
        if (a_amount) {
            if (float(a_amount)/w < 0.2 || float(a_amount)/w > 3) {
                continue;
            }
        }
        visited[u] = 1;
        if (!depth) {
            flag[u] = ((target+1) << 32) + w;
        } else {
            if (flag[u] >> 32 != target + 1) {
                flag[u] = target+1;
            }
        }
        r_dfs(u, depth+1, target, w);
        visited.erase(u);
    }
}

void dfs(ll u, int depth, ll target, ans_t &path,ll p_amount=0, ll init_amount=0)
{
    if (depth >= 6) {
        return;
    }
    ll v, w;
    for (ll &v_w: GUV[u]) {
        v = v_w >> 32;
        w = v_w & W;
        if (v <= target) {
            continue;
        }
        if (visited.find(v) != visited.end()) {
            continue;
        }
        if (!depth) {
            init_amount = w;
            p_amount = w;
        } else {
            if (float(w)/p_amount < 0.2 || float(w)/p_amount > 3) {
                continue;
            }
        }
        if (depth > 3 && (flag[v] != target + 1 && (flag[v]>>32) != target + 1)) {
            continue;
        }
        if (flag[v]>>32 == (target + 1)) {
            ll last_amount = flag[v] & W;
            if (depth >= 1) {
                if (float(init_amount)/last_amount >= 0.2
                    && float(init_amount)/last_amount <= 3
                    && float(last_amount)/w >= 0.2
                    && float(last_amount)/w <= 3) {
                    path[++path[0]] = v;
                    ans.push_back(path);
                    path[0]--;
                    ans_size++;

                }

            }
        }
        visited[v] = 1;
        path[++path[0]] = v;
        dfs(v, depth+1, target, path, w, init_amount);
        visited.erase(v);
        path[0]--;
    }
}

void work()
{
    ans_t path;
    for (auto iter = GUV.begin(); iter != GUV.end(); iter++) {
        ll i = iter->first;
        path[0] = 1;
        path[1] = i;
        r_dfs(i, 0, i);
        dfs(i, 0, i, path);
    }
}

bool cmp(ans_t &x, ans_t &y)
{
    int now = 0;
    while (x[now] == y[now])
        ++now;
    return x[now] < y[now];
}

void output()
{
    freopen(output_path.c_str(), "w", stdout);
#ifdef TEST
    auto output_time_start = chrono::steady_clock::now();
#endif
    sort(ans.begin(), ans.end(), cmp);
    printf("%d\n", ans_size);
    for (ans_t &path: ans) {
        for(int i = 1; i < path[0]; i++) {
            printf("%d,", path[i]);
        }
        printf("%d\n", path[path[0]]);
    }
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
}


int main()
{
#ifdef TEST
    cout << "Now running on data " + scale << endl;
#endif
    read_data();
    work();
    output();
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