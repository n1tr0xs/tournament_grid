#ifndef FUNCS_H
#define FUNCS_H

#include <QVector>

template<class T>
void clean_mem(QVector<QVector<T*>> vec){
    for(int i=0; i<vec.size(); ++i)
        for(int j=0; j<vec[i].size(); ++j)
            if(vec[i][j])
                delete vec[i][j];
}

#endif // FUNCS_H
