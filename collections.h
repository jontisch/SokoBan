#ifndef COLLECTION_H
#define COLLECTION_H
#include "global.h"
#include <assert.h>



template <typename T>
struct List
{

public:
    T *E;
    int N;

    void add(T *Item);
    void remove(int Index);
    T *getPointer(int index);
    int indexOf(T *Item);

    bool Allocated;
    int AllocatedElements;

    T operator [](const int Index)
    {
        return this->E[Index];
    }

};

template <typename T>
void Clear(List<T> *List);

template <typename T>
void Copy(List<T> *Destination, List<T> *Source, int Length = -1);


//The static list guarantees that the items stays in the same place in memory
template <typename T>
struct static_list
{
    List<List<T> > Lists;
    int ListSize;

    T *Add(T *Item);
    void Remove(int Index);
    T *At(int Index);

    int N;

    bool Initialized;

    T *operator [](const int Index)
    {
        return this->At(Index);
    }
};
template <typename T>
void Clear(static_list<T> *List);

template <typename T>
void Copy(static_list<T> *Destination, static_list<T> *Source);



template<typename T>
struct queue
{
public:
    queue(int InitialAllocationSize);

    void Push(T Item);
    bool Pull(T *Item);

    ~queue();

private:

    int StartCarret;
    int EndCarret;
    int AllocatedItems;
    T *Items;

};


template<typename T>
struct random_array
{
public:
    int NItems;

    random_array(int InitialAllocationSize);

    int Add(T Item);
    void Remove(int Index);

    /*
      Get
      ---
      Returns the item at the passed index. Note that this could
      be an item that has been removed but is still left in memory.
    */
    T Get(int Index);
    T *GetPointer(int Index);

    bool IndexIsValid(int Index);

    ~random_array();

private:
    int NFreeSpots;
    int *FreeSpots;

    int AllocatedItems;
    struct slot
    {
        T Item;
        bool Valid;
    };
    slot *Items;

    void EnlargeAllocation(int NewAllocationSize);
};









#define MAX(X, Y) ((X > Y) ? X : Y)

template <typename T>
void List<T>::add(T *Item)
{
    int NewAllocatedElements = MAX(AllocatedElements, 1);
    if(!Allocated)
    {
        NewAllocatedElements = MAX(128/sizeof(T), 1);
        Allocated = true;
    }
    while(NewAllocatedElements <= N)
    {
        NewAllocatedElements *= 2;
    }
    if(NewAllocatedElements != AllocatedElements)
    {
        T *OldE = E;
        E = (T *)malloc(sizeof(T)*NewAllocatedElements);
        memcpy(E, OldE, sizeof(T)*AllocatedElements);
        if(OldE) free(OldE);
        AllocatedElements = NewAllocatedElements;
    }

    //memcpy(&E[N], Item, sizeof(T));
    E[N] = *Item;

    N++;
}

template <typename T>
int List<T>::indexOf(T *Item)
{
    for(int i = 0; i < this->N; i++)
    {
        if(this->E[i] == *Item) return i;
    }
    return -1;
}

template <typename T>
void List<T>::remove(int Index)
{
    if(Index < 0 || Index >= N) return;
    if(Index < N - 1)
    {
        memcpy(&E[Index], &E[Index + 1], sizeof(T)*(N-(Index+1)));
    }
    N--;
}

template<typename T>
T *List<T>::getPointer(int index)
{
    return E + index;
}

template <typename T>
void Clear(List<T> *list)
{
    if(list->Allocated)
    {
        free(list->E);
    }

    size_t Size = sizeof(List<T>);
    memset(list, 0, Size);
}


template <typename T>
void Copy(List<T> *Destination, List<T> *Source, int Length/* = -1*/)
{
    if(Length == -1) Length = Source->N;

    *Destination = *Source;
    Destination->E = (T *)malloc(sizeof(T)*Source->AllocatedElements);
    memcpy(Destination->E, Source->E, sizeof(T)*Length);
}





template <typename T>
T *static_list<T>::Add(T *Item)
{
    bool WasInitialized = true;
    if(!Initialized)
    {
        List<T> FirstList = {0};
        Lists.add(&FirstList);
        WasInitialized = false;
        Initialized = true;
    }
    List<T> *ListToAddTo = &Lists.E[Lists.N-1];
    if(ListToAddTo->Allocated && ListToAddTo->N >= ListToAddTo->AllocatedElements)
    {
        List<T> NewList = {0};
        Lists.add(&NewList);
        ListToAddTo = &Lists.E[Lists.N-1];
    }
    ListToAddTo->add(Item);
    N++;

    if(!WasInitialized)
    {
        ListSize = ListToAddTo->AllocatedElements;
    }

    return &ListToAddTo->E[ListToAddTo->N-1];
}

template <typename T>
void static_list<T>::Remove(int Index)
{
    if(!Initialized) return;
    int ListIndex = Index / ListSize;
    int ItemIndex = Index % ListSize;
    Lists.E[ListIndex].remove(ItemIndex);
    N--;
}

template <typename T>
T *static_list<T>::At(int Index)
{
    assert(Index >= 0 && Index < N);
    if(!Initialized){
        assert(false);
        return NULL;
    }
    int ListIndex = Index / ListSize;
    int ItemIndex = Index % ListSize;
    return &Lists.E[ListIndex].E[ItemIndex];
}

template <typename T>
void Clear(static_list<T> *List)
{
    if(!List->Initialized) return;
    for(int L = 0; L < List->Lists.N; L++)
    {
        Clear(&List->Lists.E[L]);
    }
    memset(List, 0, sizeof(static_list<T>));
}


template <typename T>
void Copy(static_list<T> *Destination, static_list<T> *Source)
{
    *Destination = *Source;
    if(!Source->Initialized) return;
    Copy(&Destination->Lists, &Source->Lists);
    for(int L = 0; L < Source->Lists.N; L++)
    {
        Copy(&Destination->Lists.E[L], &Source->Lists.E[L]);
    }
}




template <typename T>
void Move(List<T> *Source, int SourceIndex, List<T> *Destination)
{
    T *Item = &Source->E[SourceIndex];
    Destination->add(Item);
    Source->remove(SourceIndex);
}



template <typename T>
queue<T>::queue(int InitialAllocationSize)
{
    StartCarret = 0;
    EndCarret = 0;
    AllocatedItems = InitialAllocationSize;
    Items = (T *)malloc(sizeof(T)*AllocatedItems);
}

template <typename T>
void queue<T>::Push(T Item)
{
    Items[EndCarret] = Item;
    EndCarret++;
    if(EndCarret >= AllocatedItems)
    {
        EndCarret = 0;
    }

    if(EndCarret == StartCarret)
    {
        int OldAllocatedItems = AllocatedItems;
        T *OldItems = Items;
        AllocatedItems *= 2;
        Items = (T *)malloc(sizeof(T)*AllocatedItems);

        int RightSectionLength = (AllocatedItems-StartCarret);
        int LeftSectionLength = EndCarret;
        memcpy(Items, &OldItems[StartCarret], sizeof(T) * RightSectionLength);
        memcpy(&Items[RightSectionLength], OldItems, sizeof(T) * LeftSectionLength);

        StartCarret = 0;
        EndCarret = OldAllocatedItems;

        Free(OldItems);
    }
}

template <typename T>
bool queue<T>::Pull(T *Item)
{
    if(StartCarret >= EndCarret)
    {
        return false;
    }

    *Item = Items[StartCarret];
    StartCarret++;

    if(StartCarret >= AllocatedItems)
    {
        StartCarret = 0;
    }

    if(StartCarret == EndCarret)
    {
        StartCarret = 0;
        EndCarret = 0;
    }

    return true;
}

template <typename T>
queue<T>::~queue()
{
    Free(Items);
}





template<typename T>
random_array<T>::random_array(int InitialAllocationSize)
{
    AllocatedItems = InitialAllocationSize;
    Items = (slot *)malloc(sizeof(slot)*AllocatedItems);
    FreeSpots = (int *)malloc(sizeof(int)*AllocatedItems);

    NItems = 0;
    NFreeSpots = 0;
}

template<typename T>
void random_array<T>::EnlargeAllocation(int NewAllocationSize)
{
    int OldAllocatedItems = AllocatedItems;

    slot *OldItems = Items;
    Items = (slot *)malloc(sizeof(slot)*NewAllocationSize);
    if(OldItems)
    {
        memcpy(Items, OldItems, sizeof(slot)*OldAllocatedItems);
        FREE(OldItems);
    }

    int *OldFreeSpots = FreeSpots;
    FreeSpots = (int *)malloc(sizeof(int)*NewAllocationSize);
    if(OldFreeSpots)
    {
        memcpy(FreeSpots, OldFreeSpots, sizeof(int)*OldAllocatedItems);
        free(OldFreeSpots);
    }

    AllocatedItems = NewAllocationSize;
}


template <typename T>
int random_array<T>::Add(T Item)
{

    int Index;
    if(NFreeSpots > 0)
    {
        Index = FreeSpots[NFreeSpots-1];
        NFreeSpots--;
    }
    else
    {
        printf("AllocatedItems = %d\n", AllocatedItems);
        int RequiredAllocationSize = AllocatedItems;
        if(NItems >= RequiredAllocationSize)
        {
            RequiredAllocationSize *= 2;
        }
        if(RequiredAllocationSize > AllocatedItems)
        {
            EnlargeAllocation(RequiredAllocationSize);
        }

        Index = NItems;
        NItems++;
    }

    Items[Index].Item = Item;
    Items[Index].Valid = true;

    return Index;
}

template <typename T>
void random_array<T>::Remove(int Index)
{

    if(Index == NItems-1)
    {
        NItems--;
    }
    else
    {
        NFreeSpots++;
        FreeSpots[NFreeSpots] = Index;
    }

    Items[Index].Valid = false;

}

template<typename T>
inline
T random_array<T>::Get(int Index)
{
    T Result;
    Result = Items[Index].Item;
    return Result;
}

template<typename T>
inline
T *random_array<T>::GetPointer(int Index)
{
    T *Result;
    Result = &Items[Index].Item;
    return Result;
}

template<typename T>
bool random_array<T>::IndexIsValid(int Index)
{
    if(Index < 0) return false;
    /*
      if(Index > NItems)
      {
      Mutex.unlock();
      return false;
      }
      for(int I = 0; I < NFreeSpots; I++)
      {
      if(FreeSpots[I] == Index)
      {
      Mutex.unlock();
      return false;
      }
      }
    */

    bool Result = Items[Index].Valid;
    return Result;
}

template<typename T>
random_array<T>::~random_array()
{
    free(Items);
    free(FreeSpots);
}



#endif // COLLECTION_H
