#ifdef DBG_MACRO_NO_WARNING
#include <dbg.h>
#else
#define dbg(...) ((void)0)
#endif

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
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

    friend ostream &operator<<(ostream &output, const XY &o)
    {
        output << "( " << o.x << ", " << o.y;
        if (o.data != -1)
        {
            output << ", " << o.data;
        }
        output << " )";
        return output;
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

    bool contains_in_border(const XY &xy) const
    {
        return this->top_right.x == xy.x && this->bottom_left.y == xy.y;
    }

    XY center() const
    {
        return XY(mid(top_right.x, bottom_left.x), mid(top_right.y, bottom_left.y));
    }

    friend ostream &operator<<(ostream &output, const BBOX &o)
    {
        output << "[ " << o.bottom_left << ", " << o.top_right << " ]";
        return output;
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
        this->children.reserve(QT_NODE_CHILDREN);
        this->points.reserve(QT_NODE_CAPACITY);
    }

    QuadTree(BBOX bbox, vector<XY> points) : bbox(bbox), points(points)
    {
        this->children.reserve(QT_NODE_CHILDREN);
        if (points.size() < QT_NODE_CAPACITY)
        {
            this->points.reserve(QT_NODE_CAPACITY);
        }
    }

    vector<QuadTree *> get_children() const
    {
        return this->children;
    }

    friend ostream &operator<<(ostream &output, const QuadTree &o)
    {
        output << o.bbox.bottom_left.x << " "
               << o.bbox.bottom_left.y << " "
               << o.bbox.top_right.x << " "
               << o.bbox.top_right.y << " ";
        for (auto &point : o.points)
        {
            output << point.x << " "
                   << point.y << " "
                   << point.data;
        }
        return output;
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

    void insert(const XY &point)
    {
        if (this->is_leaf())
        {
            if (this->is_full())
            {
                vector<XY>::iterator it = find_if(
                    this->points.begin(),
                    this->points.end(),
                    [&](XY i)
                    { return i.data == point.data; });
                if (it != points.end())
                {
                    /* Union */
                    return;
                }
                else if (this->bbox.contains_in_border(point))
                {
                    /* Intersection */
                    this->points = {point};
                }
                else
                {
                    /* Divide */
                    divide();
                    this->insert(point);
                }
            }
            else
            {
                this->points.push_back(point);
            }
        }
        else
        {
            vector<QuadTree *>::iterator it = find_if(
                children.begin(),
                children.end(),
                [&](QuadTree *i)
                { return i->contains(point); });

            assert(it != children.end());

            (*it)->insert(point);
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
            new QuadTree(nw, this->points),
            new QuadTree(ne, this->points),
            new QuadTree(sw, this->points),
            new QuadTree(se, this->points)};
        this->points.clear();
    }

    void print()
    {
        cout << "QuadtTree(bbox=" << this->bbox << ", points=[";
        for (auto &point : this->points)
        {
            cout << " " << point;
        }
        cout << " ], children=[" << endl;
        for (auto &child : this->children)
        {
            child->print();
        }
        cout << "])" << endl;
    }
};

struct IO
{
    void write_tree(string filename, QuadTree tree)
    {
        ofstream file(filename, ios::out); // | ios::binary
        assert(file);

        int idx = 0;
        queue<QuadTree *> q;
        q.push(&tree);

        while (!q.empty())
        {
            QuadTree t = *q.front();
            q.pop();
            if (!t.is_leaf())
            {
                for (QuadTree *child : t.get_children())
                {
                    q.push(child);
                }
                for (int i = idx + 1; i <= idx + 4; ++i)
                {
                    file << i << " ";
                }
                idx += 4;
            }
            else
            {
                for (int i = 0; i < 4; ++i)
                {
                    file << "-1 ";
                }
            }
            file << t << endl;
        }
    }

    QuadTree read_tree(string filename)
    {
        BBOX bbox(XY(0, 0), XY(0, 0));
        QuadTree tree(bbox);
        return tree;
    }

    void write_image(string filename, QuadTree tree) {}

    vector<XY> read_image(string filename)
    {
        ifstream file(filename, ios::in);
        assert(file);

        string line;
        file >> line;
        int w, h;
        file >> w >> h;

        vector<XY> points;
        for (int i = 0; i < h; ++i)
        {
            for (int j = 0; j < w; ++j)
            {
                int n;
                file >> n;
                points.push_back(XY(i, j, n));
            }
        }

        file.close();

        return points;
    }
} io;

int main()
{
    vector<XY> points = io.read_image("images/small.pgm");
    dbg(points);

    QuadTree tree(BBOX(XY(0, 0), XY(points.back().x, points.back().y)));
    for (auto &point : points)
    {
        tree.insert(point);
    }
    // tree.print();

    io.write_tree("tree", tree);

    // io.write_image("image.pgm", io.read_tree("tree"));

    return 0;
}