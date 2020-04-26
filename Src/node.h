#ifndef NODE_H
#define NODE_H

#include <tuple>

//That's the data structure for storing a single search node.
//You MUST store all the intermediate computations occuring during the search
//incapsulated to Nodes (so NO separate arrays of g-values etc.)

struct Node
{
    int     i, j; //grid cell coordinates
    int time;
    double  F, g, H; //f-, g- and h-values of the search node
    //Node    *parent; //backpointer to the predecessor node (e.g. the node which g-value was used to set the g-velue of the current node)
    std::pair<int, int> parent;

    Node() {
        i = -1;
        j = -1;
        time = -1;
        F = 0;
        g = 0;
        H = 0;
    }

    Node(int _i, int _j, int _time) {
        i = _i;
        j = _j;
        time = _time;
        F = 0;
        g = 0;
        H = 0;
    }

    Node &operator=(Node const &other) {
        if (*this == other && std::tie(this->F, this->g, this->H, this->parent) == std::tie(other.F, other.g, other.H, other.parent)) {
            return *this;
        }
        this->i = other.i;
        this->j = other.j;
        this->time = other.time;
        this->F = other.F;
        this->g = other.g;
        this->H = other.H;
        this->parent = other.parent;
        return *this;
    }

    bool operator==(const Node &b) {
        return (this->i == b.i) && (this->j == b.j) && (this->time == b.time);
    }
};

#endif
