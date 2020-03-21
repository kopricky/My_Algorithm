#include <bits/stdc++.h>
#define ll long long
#define INF 1000000005
#define MOD 1000000007
#define EPS 1e-10
#define rep(i,n) for(int i=0;i<(int)(n);++i)
#define rrep(i,n) for(int i=(int)(n)-1;i>=0;--i)
#define srep(i,s,t) for(int i=(int)(s);i<(int)(t);++i)
#define each(a,b) for(auto& (a): (b))
#define all(v) (v).begin(),(v).end()
#define len(v) (int)(v).size()
#define zip(v) sort(all(v)),v.erase(unique(all(v)),v.end())
#define cmx(x,y) x=max(x,y)
#define cmn(x,y) x=min(x,y)
#define fi first
#define se second
#define pb push_back
#define show(x) cout<<#x<<" = "<<(x)<<endl
#define sar(a,n) {cout<<#a<<":";rep(pachico,n)cout<<" "<<a[pachico];cout<<endl;}

using namespace std;

template<typename S,typename T>auto&operator<<(ostream&o,pair<S,T>p){return o<<"{"<<p.fi<<","<<p.se<<"}";}
template<typename T>auto&operator<<(ostream&o,set<T>s){for(auto&e:s)o<<e<<" ";return o;}
template<typename S,typename T,typename U>
auto&operator<<(ostream&o,priority_queue<S,T,U>q){while(!q.empty())o<<q.top()<<" ",q.pop();return o;}
template<typename K,typename T>auto&operator<<(ostream&o,map<K,T>&m){for(auto&e:m)o<<e<<" ";return o;}
template<typename T>auto&operator<<(ostream&o,vector<T>v){for(auto&e:v)o<<e<<" ";return o;}
void ashow(){cout<<endl;}template<typename T,typename...A>void ashow(T t,A...a){cout<<t<<" ";ashow(a...);}
template<typename S,typename T,typename U>
struct TRI{S fi;T se;U th;TRI(){}TRI(S f,T s,U t):fi(f),se(s),th(t){}
bool operator<(const TRI&_)const{return(fi==_.fi)?((se==_.se)?(th<_.th):(se<_.se)):(fi<_.fi);}};
template<typename S,typename T,typename U>
auto&operator<<(ostream&o,TRI<S,T,U>&t){return o<<"{"<<t.fi<<","<<t.se<<","<<t.th<<"}";}

typedef pair<int, int> P;
typedef pair<ll, ll> pll;
typedef TRI<int, int, int> tri;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<vi> vvi;
typedef vector<vl> vvl;
typedef vector<P> vp;
typedef vector<double> vd;
typedef vector<string> vs;

const int MAX_N = 100005;

template<typename T> class Node {
public:
    pair<T, int> val, al;
    T lazy;
    Node *left, *right, *par;
    Node() : val((T)0, -1), al((T)0, -1),
        lazy(0), left(nullptr), right(nullptr), par(nullptr){}
    void reset(){
        left = right = par = nullptr;
    }
    bool isRoot() const {
        return (!par) || (par->left != this && par->right != this);
    }
    void push(){
        if(lazy){
            val.first += lazy, al.first += lazy;
            if(left) left->lazy += lazy;
            if(right) right->lazy += lazy;
            lazy = 0;
        }
    }
    void eval(){
        al = val;
        if(left){
            left->push();
            if((left->al).first < al.first) al = left->al;
        }
        if(right){
            right->push();
            if((right->al).first <= al.first) al = right->al;
        }
    }
    void rotate(bool right){
        Node *p = this->par, *g = p->par;
        if(right){
            if((p->left = this->right)) this->right->par = p;
            this->right = p, p->par = this;
        }else{
            if((p->right = this->left)) this->left->par = p;
            this->left = p, p->par = this;
        }
        p->eval(), this->eval(), this->par = g;
        if(!g) return;
        if(g->left == p) g->left = this;
        if(g->right == p) g->right = this;
        g->eval();
    }
    void splay(){
        while(!(this->isRoot())){
            Node *p = this->par, *gp = p->par;
            if(p->isRoot()){
                p->push(), this->push();
                this->rotate((this == p->left));
            }else{
                gp->push(), p->push(), this->push();
                bool flag = (this == p->left);
                if((this == p->left) == (p == gp->left)){
                    p->rotate(flag), this->rotate(flag);
                }else{
                    this->rotate(flag), this->rotate(!flag);
                }
            }
        }
        this->push();
    }
};

template<typename T> class Dinic2 {
private:
    const int V;
    vector<int> que, level, iter, used;
	vector<Node<T> > arr;
    void access(Node<T> *u){
        Node<T> *last = nullptr;
        for(Node<T>* v = u; v; v = v->par){
            if((v->val).second == -1){
                last->par = nullptr;
                break;
            }
            v->splay();
            v->right = last;
            v->eval();
            last = v;
        }
        u->splay();
    }
    void link(Node<T> *u, Node<T> *v){
        access(u), u->right = v, u->eval(), v->par = u;
    }
    void set(Node<T> *u, const T x, const int id){
        u->val = {x, id}, u->eval();
    }
    int prev(Node<T> *u){
        // if(!u->right){
            // show(u->val);
            // assert(false);
        // }
        (u = u->right)->push();
        while(u->left){
            u = u->left;
            u->push();
        }
        u->splay();
        return (u->val).second;
    }
    int top(Node<T> *u){
        while(u->left){
            u = u->left;
            u->push();
        }
        u->splay();
        return (u->val).second;
    }
    void range(Node<T> *u, const T x){
        u->lazy += x, u->push();
    }
    void cut_and_reflect(const int ver){
        Node<T> *u = &arr[ver];
        u->splay();
        if(u->left) u->left->par = nullptr, u->left = nullptr, u->eval();
        edge& e = G[ver][iter[ver]++];
        G[e.to][e.rev].rev_cap += e.rev_cap - (u->val).first, e.rev_cap = (u->val).first;
    }
	void bst_build(vector<int>& index){
        // show(index);
        const int last = index.back();
        index.pop_back();
        reverse(index.begin(), index.end());
        int i = 2, n = (int)index.size(), st = 2, isolate = ((n%4==1) ? (n-1) : -1);
        while(st <= n){
            for(i = st-1; i < n; i += 2*st){
                arr[index[i]].left = &arr[index[i-st/2]], arr[index[i-st/2]].par = &arr[index[i]];
                if(i+st/2 < n){
                    arr[index[i]].right = &arr[index[i+st/2]], arr[index[i+st/2]].par = &arr[index[i]];
                }else if(isolate >= 0){
                    arr[index[i]].right = &arr[index[isolate]], arr[index[isolate]].par = &arr[index[i]];
                }
                arr[index[i]].eval();
            }
            isolate = ((n % (4*st) >= st && (n % (4*st) < 2*st)) ? (i-2*st) : isolate);
            st <<= 1;
        }
        // assert(!arr[index[i-st]].par);
        // show(used[index[i - st]]);
        // cout << last << " " << index[i - st] << endl;
        arr[last].right = &arr[index[i-st]], arr[last].eval(), arr[index[i-st]].par = &arr[last];
		return;
	}
	void bfs(const int s, const int t){
        level[s] = 0, used[s] = 1;
		int qh = 0, qt = 0;
        for(que[qt++] = s; qh < qt;){
            int v = que[qh++];
            for(edge& e : G[v]){
                if(level[e.to] < 0 && G[e.to][e.rev].rev_cap > 0){
					level[e.to] = level[v] + 1, used[e.to] = 1, que[qt++] = e.to;
                }
            }
        }
    }
	T block_flow_naive(const int s, int cur, T f){
		if(s == cur) return f;
		T flow = 0;
		for(int& i = iter[cur]; i < (int)G[cur].size(); ++i){
			edge& e = G[cur][i];
			if(e.rev_cap > 0 && level[e.to] < level[cur]){
				T d = block_flow_naive(s, e.to, min(f, e.rev_cap));
				if(d > 0){
					e.rev_cap -= d, G[e.to][e.rev].rev_cap += d;
					f -= d, flow += d;
					if(f == 0) break;
				}
			}
		}
		return flow;
	}
    T block_flow(const int s, const int t){
        T flow = 0;
        bool find = false;
        int cur = t;
		vector<int> index = {t};
        while(true){
            // 先頭が一番上に来てないとどこがめんどくさくなる？
            if(find){
                const pair<T, int>& res = arr[cur].al;
                flow += res.first;
                range(&arr[cur], -res.first);
                cut_and_reflect(cur = res.second);
                find = false;
            }
            used[cur] = 2;
            bool update = false;
            for(int& i = iter[cur]; i < (int)G[cur].size(); ++i){
                edge& e = G[cur][i];
                if(e.rev_cap > 0 && level[e.to] < level[cur] && used[e.to] >= 1){
                    update = true;
                    set(&arr[cur], e.rev_cap, cur);
                    if(e.to == s){
                        find = true;
						if(index.size() > 1) bst_build(index);
                        index.clear();
                        break;
                    }
                    if(used[e.to] == 1){
						if(index.empty()){
							arr[e.to].right = &arr[cur], arr[e.to].eval();
							arr[cur].par = &arr[e.to];
						}
                        index.push_back(cur = e.to);
                        break;
                    }else{
						if(index.size() > 1) bst_build(index);
                        index.clear();
					}
					link(&arr[e.to], &arr[cur]);
                    const pair<T, int>& res = arr[e.to].al;
                    if(res.first == 0){
                        cut_and_reflect(cur = res.second);
                    }else{
                        if(level[cur = top(&arr[e.to])] == 1) find = true;
                        else cut_and_reflect(cur);
                    }
                    break;
                }
            }
            if(update) continue;
            used[cur] = -1, (arr[cur].val).second = -1;
            if(cur == t) break;
            if(index.size() > 1){
                index.pop_back(), cur = index.back();
                if((int)index.size() > 1) bst_build(index);
                iter[cur]++;
            }else{
                cut_and_reflect(cur = prev(&arr[cur]));
            }
            index.clear();
        }
        return flow;
    }
    void update_dfs(Node<T>* u, auto& G, const vector<int>& iter, vector<int>& used){
        u->push();
        if(u->left) update_dfs(u->left, G, iter, used);
        if(u->right) update_dfs(u->right, G, iter, used);
        const int id = (u->val).second;
        if(iter[id] < (int)G[id].size()){
            auto& e = G[id][iter[id]];
            G[e.to][e.rev].rev_cap += e.rev_cap - (u->val).first;
            e.rev_cap = (u->val).first;
        }
		used[id] = 0;
        u->reset();
    }
    void update_and_clear(auto& G, const vector<int>& iter, vector<int>& used){
        for(int i = 0; i < V; ++i){
            if(used[i] == 2 && arr[i].isRoot()) update_dfs(&arr[i], G, iter, used);
			else if(used[i] == -1) arr[i].reset();
        }
    }

public:
    struct edge {
        const int to, rev;
        T rev_cap;
        edge(const int _to, const int _rev, const T _rev_cap) :
            to(_to), rev(_rev), rev_cap(_rev_cap){}
    };
    vector<vector<edge> > G;
    Dinic2(const int node_size)
        : V(node_size), que(V), level(V), iter(V), used(V), arr(V), G(V){}
    void add_edge(const int from, const int to, const T cap){
        G[from].emplace_back(to, (int)G[to].size(), (T)0);
        G[to].emplace_back(from, (int)G[from].size() - 1, cap);
    }
    T solve(const int s, const int t){
        T flow = 0;
        while(true){
            fill(level.begin(), level.end(), -1);
            fill(used.begin(), used.end(), 0);
            bfs(s, t);
            if(level[t] < 0) break;
            fill(iter.begin(), iter.end(), 0);
			flow += block_flow(s, t);
			update_and_clear(G, iter, used);
        }
        return flow;
    }
};

int main()
{
    cin.tie(0);
    ios::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    Dinic2<int> dn(n);
    rep(i,m){
        int a, b, c;
        cin >> a >> b >> c;
        dn.add_edge(a, b, c);
    }
    cout << dn.solve(0, n-1) << "\n";
    return 0;
}
