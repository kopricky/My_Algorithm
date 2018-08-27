#include "../header.hpp"

// "データ構造をマージする一般的なテク"と呼ばれるもの
// 大きい方に小さい方をmergeすれば全体でO(nlog(n))という話

class UF {
private:
    int sz;
public:
    vector<int> kind;   //所属しているグループ番号
    vector<vector<int> > inc;   //グループのメンバー
    UF(){}
    UF(int node_size) : sz(node_size), kind(sz), inc(sz){ rep(i,sz){ kind[i] = i; inc[i].push_back(i); } }
    bool same(int a,int b){ return kind[a] == kind[b]; }
    void unite(int a,int b)
    { if(same(a,b)) return; if(inc[kind[a]].size() < inc[kind[b]].size()) swap(a,b);
        int ga = kind[a],gb = kind[b]; rep(i,inc[gb].size()) kind[inc[gb][i]] = ga;
        inc[ga].insert(inc[ga].end(),inc[gb].begin(),inc[gb].end()); inc[gb].clear(); }
};
