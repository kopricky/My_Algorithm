//すべての頂点の出次数が1のグラフ(自己ループはない)
class FunctionalGraph {
public:
    vector<vector<int> > loop; //ループは負数
    vector<int> nx,start,deg,visit; //stはサイクルから生えた鎖のスタートの頂点
    int V;
    FunctionalGraph(int node_size){
        V = node_size;
        nx.resize(V,0),deg.resize(V,0),visit.resize(V,0);
    }
    void add_edge(int u,int v){
        nx[u] = v;
        deg[v]++;
    }
    void make_loop(const int st,int nw,int ch,vector<int>& vec){
        while(nx[nw] != st){
            vec.push_back(nx[nw]);
            visit[nx[nw]] = ch;
            break;
        }
    }
    void dfs(int u,int kind,vector<int>& vec){
        visit[u] = kind;
        int v = nx[u];
        if(visit[v] == kind){
            vec.push_back(u),vec.push_back(v);
            int ch = (int)(loop.size()) + 1;
            visit[u] = visit[v] = -ch;
            make_loop(u,v,-ch,vec);
        }else if(visit[v] == 0){
            dfs(v,kind,vec);
        }
    }
    void solve(){
        int id = 1;
        rep(i,V){
            if(deg[i] == 0){
                start.push_back(i);
            }
            if(visit[i] == 0){
                vector<int> vec;
                dfs(i,id++,vec);
                if((int)vec.size()){
                    loop.push_back(vec);
                }
            }
        }
    }
};
