#ifndef COLLECTION_H
#define COLLECTION_H
#include "global.h"

template <typename T> class Collection
{
public:
    Collection(int n);


    bool isValid(int index);

    void remove(int index);

    int add(T newElement);

    bool get(int index, T *output);

    T *getPointer(int index);

    int size();
    int count();
private:
    struct element{
        T elem;
        bool valid;
    };

    element *elements;
    int _size;
    int _allocatedMem;
    int _nElements;
    int *_freeSlots;
    int _nFreeSlots;
};

template <typename T>
Collection<T>::Collection(int n):_allocatedMem(n),_size(0),_nFreeSlots(0), _nElements(0)
{
    elements = (element*)malloc(sizeof(element)*n);
    _freeSlots = (int*)malloc(sizeof(int)*n);


}

template <typename T>
int Collection<T>::add(T newElement){
    _nElements++;
    if(_nFreeSlots == 0){
        if(_size >= _allocatedMem){
            elements = (element*)EnlargeMemory(sizeof(element)*_allocatedMem, sizeof(element)*_allocatedMem*2, elements);
            _freeSlots = (int*)EnlargeMemory(sizeof(int)*_allocatedMem, sizeof(int)*_allocatedMem*2, _freeSlots);
            _allocatedMem = _allocatedMem*2;
        }
        elements[_size].elem = newElement;
        elements[_size++].valid = true;
        return _size-1;
    }
    else
    {
        int index = _freeSlots[_nFreeSlots-1];
        elements[index].elem = newElement;
        elements[index].valid = true;
        _nFreeSlots--;
        return index;

    }

}

template <typename T>
void Collection<T>::remove(int index){
    _nElements--;
    elements[index].valid = false;
    _freeSlots[_nFreeSlots++] = index;

}

template <typename T>
bool Collection<T>::isValid(int index){
    if(index < _size)
        return elements[index].valid;
    else
        return false;
}

template <typename T>
bool Collection<T>::get(int index, T *output){
    if(isValid(index)){
        *output = elements[index].elem;
        return true;
    } else {
        return false;
    }

}


template <typename T>
T *Collection<T>::getPointer(int index){

    return isValid(index)?&elements[index].elem:NULL;
}

template <typename T>
int Collection<T>::count(){
    return _nElements;
}
template <typename T>
int Collection<T>::size(){
    return _size;
}



#endif // COLLECTION_H
