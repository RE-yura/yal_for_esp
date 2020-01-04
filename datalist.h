#ifndef __YALDATALIST__
#define __YALDATALIST__

#include <stdio.h>
#include <vector>

namespace Yal{

template<typename T = double> class Datalist{
    private:
        std::vector<T> data;

    public:
        Datalist(){}

        T &operator[](unsigned int i){ return data[i]; }
        const T operator[](unsigned int i) const { return data[i]; }

        template<typename U>
        void append(U add_data){
            data.emplace_back(add_data);
        }

        void size(){ return data.size(); }
        void clear(){ data.clear(); }
        void release(){ std::vector<T>().swap(data); }

        void print(){
            if(data.size() > 0){
                for(int i=0; i<data.size(); i++){
                    printf("%f", data[i]);
                }
            }
        }

      
};


template<typename T = double> class Datalist2D{
    private:
        int number_of_data;
        // number_of_data行(個)のデータ，列は動的に確保
        std::vector<std::vector<T>> data;

    public:
        Datalist2D(int num){
            number_of_data = num;   // std::vector<型>(要素数,埋める数));
            data = std::vector<std::vector<T>>(number_of_data, std::vector<T>(0,0));
        }

        std::vector<T> & operator[](unsigned int i){ return data[i]; }

        void append(int index, T add_data){
            data[index].emplace_back(add_data);
        }

        void size(){ return data.size(); }
        void clear(){ return data.clear(); }
        void release(){ std::vector<T>().swap(data); }

};

}

#endif