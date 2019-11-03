
#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <assert.h>

using namespace std;
#endif /* __PROGTEST__ */

struct LNode
{
    int Val;
    LNode *Next = nullptr;
};

struct Data
{
    unsigned int ID;
    unsigned int Revenue;

    bool Compare(const Data &x)
    {
        if (Revenue <= x.Revenue)
            return true;
        return false;
    }
};

class MinHeap
{
    Data ** arr;
    int capacity;
    int heap_size;

    void SwapData(Data &a, Data &b)
    {
        unsigned int _ID = a.ID;
        unsigned int _Revenue = a.Revenue;
        a.Revenue = b.Revenue;
        b.Revenue = _Revenue;
        a.ID = b.ID;
        b.ID = _ID;
    }

    void CheckIntegirty(int i)
    {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int smallest = i;
        if (l < heap_size && arr[l].Compare(arr[i]))
            smallest = l;
        if (r < heap_size && arr[r].Compare(arr[smallest]))
            smallest = r;
        if (smallest != i)
        {
            SwapData(arr[i], arr[smallest]);
            CheckIntegirty(smallest);
        }
    }

public:
    MinHeap(const int &cap)
    {
        heap_size = 0;
        capacity = cap;
        arr = new Data[cap];
    }

    Data &GetMin()
    {
        Data &root = arr[0];
        return root;
    }

    Data *ExtractMin()
    {
        Data *root = new Data();
        root->ID = arr[0].ID;
        root->Revenue = arr[0].Revenue;

        if (heap_size != 1)
        {
            SwapData(arr[0], arr[heap_size - 1]);
        }

        heap_size--;
        CheckIntegirty(0);

        return root;
    }

    void MergeHeap(MinHeap *o)
    {
    }

    bool CheckEmpty()
    {
        if (heap_size <= 0)
            return true;
        return false;
    }

    void CheckFull()
    {
        if (heap_size == capacity)
        {
            capacity *= 2;
            Data *tmpArr = new Data[capacity];
            for (int i = 0; i < capacity / 2; i++)
                tmpArr[i] = arr[i];
            delete[] arr;
            arr = tmpArr;
        }
    }

    void Insert(Data * k)
    {
        CheckFull();

        heap_size++;
        int i = heap_size - 1;
        arr[i] = k;

        while (i != 0 && arr[i].Compare(arr[(i - 1) / 2]))
        {
            SwapData(arr[i], arr[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    ~MinHeap()
    {
        delete[] arr;
    }
};

class CHolding
{
    MinHeap **_chains;
    int _defaultCapacity = 8;

public:
    CHolding()
    {
        _chains = new MinHeap *[10000 + 1];
        for (int i = 0; i < 10000 + 1; ++i)
        {
            _chains[i] = nullptr;
        }
    }
    ~CHolding()
    {
        for (int i = 0; i < 10000 + 1; ++i)
        {
            if (_chains[i] != nullptr)
                delete _chains[i];
        }
        delete[] _chains;
    }
    void Add(int chain, unsigned int id, unsigned int revenue)
    {
        Data d;
        d.ID = id;
        d.Revenue = revenue;

        if (_chains[chain] == nullptr)
        {
            _chains[chain] = new MinHeap(8);
        }
        

        _chains[chain]->Insert(d);
        //cout << "Adding: " << chain << " - " << d.ID << " / " << d.Revenue << "  " << _revs[chain] << endl;
    }
    bool Remove(int chain, unsigned int &id)
    {
        if (_chains[chain] == nullptr)
        {
            //cout << "N/A" << endl;
            return false;
        }

        if (_chains[chain]->CheckEmpty())
        {
            //cout << "N/A" << endl;
            return false;
        }

        Data *r = _chains[chain]->ExtractMin();
        id = r->ID;
        //cout << r->Revenue << "-" << r->ID << endl;
        delete r;

        return true;
    }
    bool Remove(unsigned int &id)
    {
        int min = 0;
        Data d;
        Data minData;
        minData.ID = 4294967295;
        minData.Revenue = 4294967295;

        for (int i = 0; i < 10000; ++i)
        {
            if (_chains[i] == nullptr)
                continue;
            if (_chains[i]->CheckEmpty())
                continue;

            d = _chains[i]->GetMin();
            if (d.Revenue < minData.Revenue)
            {
                minData = d;
                min = i;
            }
            else
            {
                if (d.Revenue == minData.Revenue)
                {
                    continue;
                }
            }
        }

        if (min == 0)
            return false;

        Data *r = _chains[min]->ExtractMin();
        id = r->ID;
        //cout << r->Revenue << "-" << r->ID << endl;
        delete r;
        return true;
    }
    void Merge(int dstChain, int srcChain)
    {
    }
    bool Merge(void)
    {
        return false;
    }

private:
};

#ifndef __PROGTEST__
int main()
{

    bool res;
    unsigned int id;

    MinHeap * h = new MinHeap(10);
    Data * d = new Data();
    d->ID = 9;
    d->Revenue = 5;

    h->Insert(*d);
    cout << h->GetMin().ID << endl;
    d->ID = 5;
    cout << h->GetMin().ID << endl;
    
    //Ukazkovy vstup #1
    //-----------------
/*
    CHolding f1;
    f1.Add(7, 2, 9);
    f1.Add(12, 4, 4);
    f1.Add(6, 15, 2);
    f1.Add(6, 9, 3);
    res = f1.Remove(12, id);
    // res = true, id = 4
    res = f1.Remove(12, id);
    // res = false, id = N/A
    res = f1.Remove(6, id);
    // res = true, id = 15
    res = f1.Remove(6, id);
    // res = true, id = 9
    res = f1.Remove(6, id);
    // res = false, id = N/A

    //Ukazkovy vstup #2
    //-----------------
    CHolding f2;
    f2.Add(4, 2, 2);
    f2.Add(1, 4, 3);
    f2.Add(8, 9, 8);
    res = f2.Remove(id);
    // res = true, id = 2
    res = f2.Remove(id);
    // res = true, id = 4

    //Ukazkovy vstup #3
    //-----------------
    CHolding f3;
    for (int i = 1; i < 10000000; i++){
        f3.Add(i % 100 + 10, 101, i);
    }
    for (int i = 1; i < 10000000; i++){
        f3.Remove(id);
    }
    f3.Add(10, 101, 9);
    f3.Add(10, 102, 8);
    f3.Add(10, 103, 7);
    f3.Add(10, 104, 6);
    f3.Add(10, 105, 5);
    f3.Add(20, 201, 9);
    f3.Add(20, 202, 8);
    f3.Add(20, 203, 7);
    f3.Add(20, 204, 6);
    f3.Add(20, 205, 5);
    f3.Add(30, 301, 9);
    f3.Add(30, 302, 8);
    f3.Add(30, 303, 7);
    f3.Add(30, 304, 6);
    f3.Add(30, 305, 5);

    res = f3.Remove(id);
    // res = true, id = 105
    res = f3.Remove(id);
    // res = true, id = 205
    res = f3.Remove(id);
    // res = true, id = 305
    res = f3.Remove(id);
    // res = true, id = 104
    res = f3.Remove(id);
    // res = true, id = 204
    res = f3.Remove(id);
    // res = true, id = 304
    res = f3.Remove(id);
    // res = true, id = 103
    res = f3.Remove(id);
    // res = true, id = 203
    res = f3.Remove(id);
    // res = true, id = 303
    res = f3.Remove(id);
    // res = true, id = 102
    res = f3.Remove(id);
    // res = true, id = 202

    /*
    //Ukazkovy vstup #4
    //-----------------
    CHolding f4;
    f4.Add(10, 101, 9);
    f4.Add(10, 102, 8);
    f4.Add(10, 103, 7);
    f4.Add(10, 104, 6);
    f4.Add(10, 105, 5);
    f4.Add(20, 201, 9);
    f4.Add(20, 202, 8);
    f4.Add(20, 203, 7);
    f4.Add(20, 204, 6);
    f4.Add(20, 205, 5);
    f4.Add(30, 301, 9);
    f4.Add(30, 302, 8);
    f4.Add(30, 303, 7);
    f4.Add(30, 304, 6);
    f4.Add(30, 305, 5);
    res = f4.Remove(30, id);
    // res = true, id = 305
    res = f4.Remove(20, id);
    // res = true, id = 205
    res = f4.Remove(10, id);
    // res = true, id = 105
    f4.Merge(30, 10);
    res = f4.Remove(10, id);
    // res = false, id = N/A
    res = f4.Remove(20, id);
    // res = true, id = 204
    res = f4.Remove(30, id);
    // res = true, id = 104
    res = f4.Remove(id);
    // res = true, id = 304
    res = f4.Remove(id);
    // res = true, id = 203
    res = f4.Remove(id);
    // res = true, id = 103
    res = f4.Remove(id);
    // res = true, id = 303
    res = f4.Remove(id);
    // res = true, id = 202
    res = f4.Remove(id);
    // res = true, id = 102
    res = f4.Remove(id);
    // res = true, id = 302
    res = f4.Remove(id);
    // res = true, id = 201
    res = f4.Remove(id);
    // res = true, id = 101
    res = f4.Remove(id);
    // res = true, id = 301
    res = f4.Remove(id);
    // res = false, id = N/A
    res = f4.Remove(id);
    // res = false, id = N/A

    //Ukazkovy vstup #5
    //-----------------
    CHolding f5;
    f5.Add(10, 333, 5);
    f5.Add(20, 444, 2);
    f5.Add(10, 222, 6);
    f5.Add(20, 555, 8);
    res = f5.Remove(10, id);
    // res = true, id = 333
    res = f5.Remove(id);
    // res = true, id = 444
    f5.Merge(20, 10);
    res = f5.Remove(10, id);
    // res = false, id = N/A
    res = f5.Remove(20, id);
    // res = true, id = 222

    //Ukazkovy vstup #6
    //-----------------
    CHolding f6;
    f6.Add(10, 1, 7);
    f6.Add(20, 1, 7);
    f6.Add(30, 1, 7);
    res = f6.Merge();
    // res = true
    res = f6.Remove(20, id);
    // res = false, id = N/A
    res = f6.Remove(30, id);
    // res = true, id = 1
    res = f6.Remove(30, id);
    // res = false, id = N/A
    res = f6.Remove(10, id);
    // res = true, id = 1
    res = f6.Remove(10, id);
    // res = true, id = 1
    res = f6.Remove(10, id);
    // res = false, id = N/A

    //Ukazkovy vstup #7
    //-----------------
    CHolding f7;
    f7.Add(1, 1, 1);
    f7.Add(2, 2, 1);
    f7.Add(3, 3, 1);
    res = f7.Merge();
    // res = true
    res = f7.Merge();
    // res = true
    res = f7.Merge();
    // res = false
    res = f7.Remove(id);
    // res = true, id = 1
    res = f7.Remove(id);
    // res = true, id = 2
    res = f7.Remove(id);
    // res = true, id = 3
    res = f7.Remove(id);
    // res = false, id = N/A
*/
    return 0;
}
#endif