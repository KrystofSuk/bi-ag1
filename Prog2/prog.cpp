
#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <fstream>
using namespace std;
#endif /* __PROGTEST__ */

struct Data
{
    unsigned int ID = 0;
    unsigned int ID2 = 0;
    unsigned int Revenue = 0;
    int index = 0;

    bool Compare(Data *x)
    {
        if (Revenue < x->Revenue)
            return true;
        else if (Revenue == x->Revenue)
        {
            if (ID < x->ID)
                return true;
        }
        return false;
    }
};

class MinHeap
{
    Data **arr;
    int capacity;
    int heap_size;

    void SwapData(Data **a, Data **b)
    {
        int i = (*a)->index;

        (*a)->index = (*b)->index;
        (*b)->index = i;

        Data *pSwap = *a;
        *a = *b;
        *b = pSwap;
        //cout << "Swap" << endl;
    }

public:
    MinHeap(const int &cap)
    {
        heap_size = 0;
        capacity = cap;
        arr = new Data *[cap];
    }

    Data *GetMin()
    {
        return arr[0];
    }

    Data *ExtractMin()
    {
        Data *root = new Data();
        root->ID = arr[0]->ID;
        root->Revenue = arr[0]->Revenue;

        if (heap_size != 1)
        {
            SwapData(&arr[0], &arr[heap_size - 1]);
        }
        heap_size--;
        CheckIntegirty(0);

        return root;
    }

    void CheckIntegirty(int i)
    {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int smallest = i;
        if (l < heap_size && arr[l]->Compare(arr[i]))
            smallest = l;
        if (r < heap_size && arr[r]->Compare(arr[smallest]))
            smallest = r;
        if (smallest != i)
        {
            SwapData(&arr[i], &arr[smallest]);
            CheckIntegirty(smallest);
        }
    }

    void CheckIntegirtyComplete(int i)
    {
        while (i != 0 && arr[i]->Compare(arr[(i - 1) / 2]))
        {
            SwapData(&arr[i], &arr[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
        CheckIntegirty(i);
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
            //cout << "FULL" << endl;
            capacity *= 2;
            Data **tmpArr = new Data *[capacity];
            for (int i = 0; i < capacity / 2; i++)
            {
                tmpArr[i] = arr[i];
            }
            arr = tmpArr;
        }
    }

    void Insert(Data *k)
    {
        CheckFull();

        heap_size++;
        int i = heap_size - 1;
        arr[i] = k;
        arr[i]->index = i;

        while (i != 0 && arr[i]->Compare(arr[(i - 1) / 2]))
        {
            SwapData(&arr[i], &arr[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    ~MinHeap()
    {
        for(int i = 0; i < capacity; i++)
            if(arr[i] != nullptr)
                delete arr[i];
        delete[] arr;
    }

    void Print()
    {
        cout << "------" << endl;
        for (int i = 0; i < heap_size; i++)
        {
            cout << arr[i]->ID << "-" << arr[i]->ID2 << "-" << arr[i]->Revenue << " = " << arr[i]->index << endl;
        }
    }
};

class CHolding
{
    MinHeap **_chains;
    Data ** _data;
    int _defaultCapacity = 8;

    MinHeap *_nodes;

public:
    CHolding()
    {
        _nodes = new MinHeap(1);
        _chains = new MinHeap *[10000 + 1];
        _data = new Data *[10000 + 1];
        for (int i = 0; i < 10000 + 1; ++i)
        {
            _data[i] = nullptr;
            _chains[i] = nullptr;
        }
    }
    ~CHolding()
    {
        for (int i = 0; i < 10000 + 1; ++i)
        {
            if (_chains[i] != nullptr)
                delete _chains[i];
            if (_data[i] != nullptr)
                delete _data[i];
        }
        delete _nodes;
        delete[] _data;
        delete[] _chains;
    }
    void Add(int chain, unsigned int id, unsigned int revenue)
    {
        Data *n = new Data();
        n->ID = chain;
        n->ID2 = id;
        n->Revenue = revenue;

        if (_chains[chain] == nullptr)
        {
            _chains[chain] = new MinHeap(1);
            _data[chain] = n;
            _nodes->Insert(n);
        }
        else
        {
            if (_data[chain]->Revenue > n->Revenue)
            {
                _data[chain]->ID2 = n->ID2;
                _data[chain]->Revenue = n->Revenue;
                _nodes->CheckIntegirtyComplete(_data[chain]->index);
            }
            else if (_data[chain]->Revenue == n->Revenue)
            {
                if (_data[chain]->ID2 > n->ID2)
                {
                    _data[chain]->ID2 = n->ID2;
                    _data[chain]->Revenue = n->Revenue;
                    _nodes->CheckIntegirtyComplete(_data[chain]->index);
                }
            }
            delete n;
        }

        Data *d = new Data();
        d->ID = id;
        d->ID2 = id;
        d->Revenue = revenue;

        _chains[chain]->Insert(d);
    }
    bool Remove(int chain, unsigned int &id)
    {
        if (_chains[chain] == nullptr)
        {
            return false;
        }

        if (_chains[chain]->CheckEmpty())
        {
            return false;
        }

        Data *r = _chains[chain]->ExtractMin();
        id = r->ID;
        delete r;

        if (_chains[chain]->CheckEmpty())
        {
            _data[chain]->Revenue = 0;
            _data[chain]->ID = 0;
            _nodes->CheckIntegirtyComplete(_data[chain]->index);
            Data *d = _nodes->ExtractMin();
            delete d;
            delete _chains[chain];
            _chains[chain] = nullptr;
        }
        else
        {
            //cout << "Changing " << _data[chain]->ID2 << " -> " << _chains[chain]->GetMin()->ID2 << endl;
            _data[chain]->ID2 = _chains[chain]->GetMin()->ID2;
            _data[chain]->Revenue = _chains[chain]->GetMin()->Revenue;
            _nodes->CheckIntegirtyComplete(_data[chain]->index);
        }

        return true;
    }
    bool Remove(unsigned int &id)
    {
        if (_nodes->CheckEmpty())
        {
            return false;
        }
        Data *d = _nodes->GetMin();
        bool t = Remove(d->ID, id);
        return t;
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
    bool res = false;
    unsigned int id = 0;
    CHolding f8;
    /*
    std::ofstream outfile;
    outfile.open("test1.txt", std::ios_base::trunc);

    unsigned int x = 0;
    int y = 0;
    unsigned int z = 0;
    unsigned int q = 0;
    for (unsigned int i = 0; i < 10000; i++)
    {
        cin >> x;
        if (x == 0)
        {
            cin >> y >> z >> q;
            f8.Add(y, z, q);
        }
        else if (x == 1)
        {
            cin >> x;
            res = f8.Remove(x, id);
            outfile << res << " " << id << endl;
        }
        else if (x == 2)
        {
            res = f8.Remove(id);
            outfile << res << " " << id << endl;
        }
    }
    for (unsigned int i = 0; i < 10000000; i++)
    {
        f8.Add(1, 1, 1);
    }
    for (unsigned int i = 0; i < 10000000; i++)
    {
        f8.Remove(1, id);
    }
*/
    cout << "ddd" << endl;
    //Ukazkovy vstup #1
    //-----------------
    CHolding f1;
    f1.Add(7, 2, 9);
    f1.Add(12, 4, 4);
    f1.Add(6, 15, 2);
    f1.Add(6, 9, 3);
    res = f1.Remove(12, id);
    assert(res == true && id == 4);
    // res = true, id = 4
    res = f1.Remove(12, id);
    assert(res == false && id == 4);
    // res = false, id = N/A
    res = f1.Remove(6, id);
    assert(res == true && id == 15);
    // res = true, id = 15
    res = f1.Remove(6, id);
    assert(res == true && id == 9);
    // res = true, id = 9
    res = f1.Remove(6, id);
    assert(res == false && id == 9);
    // res = false, id = N/A
    //Ukazkovy vstup #2
    //-----------------
    CHolding f2;
    f2.Add(4, 2, 2);
    f2.Add(1, 4, 3);
    f2.Add(8, 9, 8);
    cout << "----" << endl;
    res = f2.Remove(id);
    assert(res == true && id == 2);
    // res = true, id = 2
    res = f2.Remove(id);
    assert(res == true && id == 4);
    // res = true, id = 4

    //Ukazkovy vstup #3
    //-----------------
    CHolding f3;
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
    assert(res == true && id == 105);
    res = f3.Remove(id);
    cout << id << endl;
    // res = true, id = 205
    assert(res == true && id == 205);
    res = f3.Remove(id);
    // res = true, id = 305
    assert(res == true && id == 305);
    res = f3.Remove(id);
    // res = true, id = 104
    assert(res == true && id == 104);
    res = f3.Remove(id);
    // res = true, id = 204
    assert(res == true && id == 204);
    res = f3.Remove(id);
    // res = true, id = 304
    assert(res == true && id == 304);
    res = f3.Remove(id);
    // res = true, id = 103
    assert(res == true && id == 103);
    res = f3.Remove(id);
    // res = true, id = 203
    assert(res == true && id == 203);
    res = f3.Remove(id);
    // res = true, id = 303
    assert(res == true && id == 303);
    res = f3.Remove(id);
    // res = true, id = 102
    assert(res == true && id == 102);
    res = f3.Remove(id);
    // res = true, id = 202
    assert(res == true && id == 202);
    res = f3.Remove(id);
    // res = true, id = 302
    assert(res == true && id == 302);

    CHolding c2;
    c2.Add(30, 80, 4);
    c2.Add(30, 83, 3);
    c2.Add(30, 82, 2);
    c2.Add(30, 81, 1);
    c2.Add(30, 181, 1);
    c2.Add(20, 90, 4);
    c2.Add(20, 93, 3);
    c2.Add(20, 92, 2);
    c2.Add(20, 291, 1);
    c2.Add(20, 191, 1);
    //---
    res = c2.Remove(id);
    //cout << id << endl;
    assert(res && id == 191);
    res = c2.Remove(id);
    assert(res && id == 291);
    res = c2.Remove(id);
    assert(res && id == 81);
    res = c2.Remove(id);
    assert(res && id == 181);
    res = c2.Remove(id);
    assert(res && id == 92);
    res = c2.Remove(id);
    assert(res && id == 82);
    res = c2.Remove(id);
    assert(res && id == 93);
    res = c2.Remove(id);
    assert(res && id == 83);
    res = c2.Remove(id);
    assert(res && id == 90);
    res = c2.Remove(id);
    assert(res && id == 80);
    res = c2.Remove(id);
    assert(!res && id == 80);

    CHolding c3;
    c3.Add(1, 1, 1);
    c3.Add(1, 2, 1);
    c3.Add(1, 3, 2);
    res = c3.Remove(id);
    assert(res && id == 1);
    res = c3.Remove(id);
    assert(res && id == 2);
    res = c3.Remove(id);
    assert(res && id == 3);
    c3.Add(1, 1, 1);
    c3.Add(1, 2, 1);
    c3.Add(1, 3, 2);
    c3.Add(2, 4, 1);
    c3.Add(2, 5, 1);
    c3.Add(2, 6, 2);
    res = c3.Remove(id);
    assert(res && id == 1);
    res = c3.Remove(id);
    assert(res && id == 2);
    res = c3.Remove(id);
    assert(res && id == 4);
    res = c3.Remove(id);
    assert(res && id == 5);
    res = c3.Remove(id);
    assert(res && id == 3);
    res = c3.Remove(id);
    assert(res && id == 6);
    CHolding a;
    a.Add(10, 10, 10); //1
    a.Add(10, 10, 10); //3
    a.Add(10, 10, 10); //4
    a.Add(10, 10, 10); //5
    a.Add(10, 10, 10); //7

    assert(a.Remove(id) && id == 10); //1

    a.Add(10, 9, 10); //2

    assert(a.Remove(id) && id == 9); //2

    a.Add(10, 9, 11); //8

    assert(a.Remove(10, id) && id == 10); //3
    assert(a.Remove(id) && id == 10);     //4

    a.Add(20, 5, 10); //6

    assert(a.Remove(id) && id == 10); //5

    assert(a.Remove(20, id) && id == 5); //6

    a.Add(20, 20, 20); //11

    cout << id << endl;

    assert(a.Remove(id) && id == 10); //7

    a.Add(20, 19, 20); //10
    a.Add(10, 20, 20); //9

    assert(a.Remove(id) && id == 9);      //8
    assert(a.Remove(10, id) && id == 20); //9
    assert(a.Remove(id) && id == 19);     //10
    assert(a.Remove(20, id) && id == 20); //11
    assert(!a.Remove(id) && id == 20);    //xx
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
    assert(res == true && id == 305);
    // res = true, id = 305
    res = f4.Remove(20, id);
    assert(res == true && id == 205);
    // res = true, id = 205
    res = f4.Remove(10, id);
    assert(res == true && id == 105);
    // res = true, id = 105
    f4.Merge(30, 10);
    res = f4.Remove(10, id);
    assert(res == false && id == 105);
    // res = false, id = N/A
    res = f4.Remove(20, id);
    assert(res == true && id == 204);
    // res = true, id = 204
    res = f4.Remove(30, id);
    assert(res == true && id == 104);
    // res = true, id = 104
    res = f4.Remove(id);
    assert(res == true && id == 304);
    // res = true, id = 304
    res = f4.Remove(id);
    assert(res == true && id == 203);
    // res = true, id = 203
    res = f4.Remove(id);
    assert(res == true && id == 103);
    // res = true, id = 103
    res = f4.Remove(id);
    assert(res == true && id == 303);
    // res = true, id = 303
    res = f4.Remove(id);
    assert(res == true && id == 203);
    // res = true, id = 202
    res = f4.Remove(id);
    assert(res == true && id == 102);
    // res = true, id = 102
    res = f4.Remove(id);
    assert(res == true && id == 302);
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