#ifndef FUNCS_H
#define FUNCS_H

#include <vector>

template<class T>
void clean_mem(std::vector<std::vector<T*>> vec){
    for(size_t i=0; i<vec.size(); ++i)
        for(size_t j=0; j<vec[i].size(); ++j)
            delete vec[i][j];
}

#endif // FUNCS_H
