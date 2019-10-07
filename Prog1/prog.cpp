
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <iomanip>

using namespace std;

class Node
{
public:
    Node *front;
    Node *swap;
    Node *back;
    bool visited = false;
    bool cleaned = false;
    int distance = 2147483647;
    int index;

    Node()
    {
        front = nullptr;
        swap = nullptr;
        back = nullptr;
    }
};

class Lunch
{
    int _skip;
    int _lenght;

    Node *_nodes;
    Node *_end;

    list<int> _queue;

public:
    Lunch(const int &l, const int &s, const string &left, const string &right)
    {
        _skip = s;
        _lenght = l;

        _nodes = new Node[l * 2];
        for(int i = 0; i < l*2; i++){
            _nodes[i].index = i;
        }
        _end = new Node();

        //cout << "Loaded: " << l << " " << s << endl;
        //cout << "L: " << left << endl;
        /*
        for(int i = 0; i < l; i++){
            cout << setw(3)<<left[i];
        }
        cout << endl;
        for(int i = 0; i < l; i++){
            cout << setw(3)<< right[i];
        }
        cout << endl;
        for(int i = 0; i < l; i++){
            cout << setw(3)<<_nodes[i].index;
        }
        cout << endl;
        for(int i = 0; i < l; i++){
            cout << setw(3)<< _nodes[i+_lenght].index;
        }
        cout << endl;
        */
        //cout << "R: " << right << endl;
        if(l == 1)
        {
            _nodes[0].front = _end;
        }

        for (int i = 0; i < l - 1; i++)
        {
            //Left
            if (left[i + 1] == '.')
            {
                _nodes[i].front = &_nodes[i + 1];
            }
            if (left[i + 1] == '.' && i == l - 1)
            {
                _nodes[i].front = _end;
                //cout << "End: " << i << endl;
            }
            if (i > 0)
            {
                if (left[i - 1] == '.')
                {
                    _nodes[i].back = &_nodes[i - 1];
                }
            }
            if (i + _skip < _lenght)
            {
                if (right[i + _skip] == '.')
                    _nodes[i].swap = &_nodes[_lenght + i + _skip];
            }
            else
            {
                _nodes[i].swap = _end;
                //cout << "End: " << i << endl;
            }

            //Right
            if (right[i + 1] == '.')
            {
                _nodes[_lenght + i].front = &_nodes[_lenght + i + 1];
            }
            if (right[i + 1] == '.' && i == l - 2)
            {
                _nodes[_lenght + 1 + i].front = _end;
                //cout << "End: " << _lenght+ 1 + i << endl;
            }
            if (i > 0)
            {
                if (right[i - 1] == '.')
                {
                    _nodes[_lenght + i].back = &_nodes[_lenght + i - 1];
                }
            }
            if (i + _skip < _lenght)
            {
                if (left[i + _skip] == '.')
                    _nodes[_lenght + i].swap = &_nodes[i + _skip];
            }
            else
            {
                _nodes[_lenght + i].swap = _end;
                //cout << "End: " << _lenght+i << endl;
            }
        }
    }

    int Process()
    {
        list<Node *> queue;
        Node *now = &_nodes[0];
        now->visited = true;
        now->distance = 0;
        queue.push_back(now);
        int time = 0;
        while (!queue.empty())
        {
            now = queue.front();
            queue.pop_front();
            if(now->distance != time){
                _nodes[time].cleaned = true;
                _nodes[time + _lenght].cleaned = true;
                //cout << "Cleaned: " << time << "/" << time + _lenght << endl << endl;
                time++;
            }
            if (now == _end)
                return now->distance;
            //cout << "Now: " << now->index << endl;
            if(now -> cleaned == false)
            {
                if (now->front != nullptr)
                {
                    if (!now->front->visited)
                    {
                        now->front->visited = true;
                        now->front->distance = now->distance+1;
                        queue.push_back(now->front);
                        //cout << "F " <<  now->front->index<< endl;
                    }
                }
                if (now->back != nullptr)
                {
                    if (!now->back->visited)
                    {
                        now->back->visited = true;
                        now->back->distance = now->distance+1;
                        queue.push_back(now->back);
                        //cout << "B " <<  now->back->index<< endl;
                    }
                }
                if (now->swap != nullptr)
                {
                    if (!now->swap->visited)
                    {
                        now->swap->visited = true;
                        now->swap->distance = now->distance+1;
                        queue.push_back(now->swap);
                        //cout << "S " <<  now->swap->index<< endl;
                    }
                }
            }else{
                //cout<< "Clean " << now-> index << endl;
            }
            
            
        }
        return -1;
    }

    ~Lunch()
    {
        delete[] _nodes;
    }
};

int main()
{
    int l, s;
    string left, right;
    cin >> l >> s >> left >> right;

    Lunch lunch(l, s, left, right);
    cout << lunch.Process() << endl;
    return 0;
}