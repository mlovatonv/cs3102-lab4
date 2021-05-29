#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int QT_NODE_CAPACITY = 1;
const int QT_NODE_CHILDREN = 4;

int ceil(const int &a, const int &b)
{
    return 1 + ((a - 1) / b);
}

int mid(const int &a, const int &b)
{
    int mx = max(a, b), mn = min(a, b);
    return mn + (mx - mn) / 2;
}

struct XY
{
    int x;
    int y;
    int data;

    XY() : x(-1), y(-1), data(-1) {}

    XY(int x, int y) : x(x), y(y), data(-1) {}

    XY(int x, int y, int data) : x(x), y(y), data(data) {}

    bool operator<=(const XY &o) const
    {
        return this->x <= o.x && this->y <= o.y;
    }

    bool operator>=(const XY &o) const
    {
        return this->x >= o.x && this->y >= o.y;
    }
};

struct BBOX
{
    XY bottom_left;
    XY top_right;

    BBOX() {}

    BBOX(XY bottom_left, XY top_right) : bottom_left(bottom_left), top_right(top_right)
    {
    }

    bool contains(const XY &xy) const
    {
        return this->bottom_left <= xy && xy <= this->top_right;
    }

    XY center() const
    {
        return XY(mid(top_right.x, bottom_left.x), mid(top_right.y, bottom_left.y));
    }
};

class QuadTree
{
private:
    BBOX bbox; /* Bounding box */
    vector<QuadTree *> children;
    vector<XY> points;

public:
    QuadTree() {}

    QuadTree(BBOX bbox) : bbox(bbox)
    {
        children.reserve(QT_NODE_CHILDREN);
        points.reserve(QT_NODE_CAPACITY);
    }

    bool is_leaf() const
    {
        return children.empty();
    }

    bool is_full() const
    {
        return points.size() == QT_NODE_CAPACITY;
    }

    bool contains(const XY &xy) const
    {
        return bbox.contains(xy);
    }

    void insert(const XY &xy)
    {
        if (this->is_leaf())
        {
            if (this->is_full())
            {
                divide();
                this->insert(xy);
            }
            else
            {
                this->points.push_back(xy);
            }
        }
        else
        {
            vector<QuadTree *>::iterator it = find_if(
                children.begin(),
                children.end(),
                [&](QuadTree *qt)
                { return qt->contains(xy); });
            assert(it != children.end());
            (*it)->insert(xy);
        }
    }

    void divide()
    {
        XY bottom_left = this->bbox.bottom_left,
           top_right = this->bbox.top_right,
           center = this->bbox.center();
        BBOX nw(XY(bottom_left.x, center.y + 1), XY(center.x, top_right.y)),
            ne(XY(center.x + 1, center.y + 1), top_right),
            sw(bottom_left, center),
            se(XY(center.x + 1, bottom_left.y), XY(top_right.x, center.y));
        this->children = {
            new QuadTree(nw),
            new QuadTree(ne),
            new QuadTree(sw),
            new QuadTree(se)};
    }
};

struct IO
{
    void write_tree(string filename, QuadTree tree)
    {
    }

    QuadTree read_tree(string filename)
    {
        BBOX bbox(XY(0, 0), XY(0, 0));
        QuadTree tree(bbox);
        return tree;
    }

    void write_image(string filename, QuadTree tree) {}

    vector<vector<XY>> read_image(string filename)
    {
        vector<vector<XY>> matrix;
        return matrix;
    }
};

int main()
{
    IO io;

    return 0;
}