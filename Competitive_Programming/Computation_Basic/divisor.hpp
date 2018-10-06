#include "../header.hpp"

template<typename T>
vector<T> divisor(T n)
{
    vector<T> res;
    for(T i=1;(ll)i*i<=n;i++){
        if(n%i==0){
            res.push_back(i);
            if(i != n/i){
                res.push_backs(n/i);
            }
        }
    }
    sort(all(res));
    return res;
}
