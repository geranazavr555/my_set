#ifndef MY_SET_H
#define MY_SET_H

#include <cstddef>
#include <utility>
#include <iterator>

template <typename T>
class Set
{
    typedef T key_type;

    struct BaseNode;

    struct BaseNode
    {
        BaseNode *parent, *left_child, *right_child;

        BaseNode() ;
        BaseNode(BaseNode* parent);
        BaseNode(BaseNode* parent, BaseNode* left, BaseNode* right);

        virtual ~BaseNode();
    };

    struct Node : public BaseNode
    {
        key_type key;

        Node(key_type const& key) ;
        Node(key_type const& key, BaseNode* parent) ;
        Node(key_type const& key, BaseNode* parent, BaseNode* left, BaseNode* right) ;
    };

    template <typename U>
    class Iterator : public std::iterator<std::bidirectional_iterator_tag, U>
    {
        friend class Set;

    private:
        BaseNode* ptr;

    public:
        explicit Iterator(BaseNode* ptr);

        template <typename V>
        Iterator(Iterator<V> const& other);

        U& operator*() const;

        bool operator==(Iterator const& other);
        bool operator!=(Iterator const& other);

        Iterator& operator++();
        Iterator operator++(int);
        Iterator& operator--();
        Iterator operator--(int);

        void up();
    };

public:
    using iterator = Iterator<T>;
    using const_iterator = Iterator<T const>;
    using reverse_iterator = std::reverse_iterator<iterator>;

private:
    size_t siz;
    BaseNode* root;

    const_iterator detach(const_iterator iter);

public:

    Set();
    Set(Set const& other) ;

    ~Set();

    Set& operator=(Set const& other);


    std::pair<iterator, bool> insert(key_type const& x);
    void erase(const_iterator iter);
    const_iterator find(key_type const& x) const;
    const_iterator lower_bound(key_type const& x) const;
    const_iterator upper_bound(key_type const& x) const;

    bool empty() const;
    size_t size() const;
    void clear();

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
};


/// BASE NODE IMPLEMENTATION =================================================================

template <typename T>
Set<T>::BaseNode::BaseNode()
        : left_child(nullptr),
          right_child(nullptr),
          parent(nullptr)
{}

template <typename T>
Set<T>::BaseNode::BaseNode(BaseNode *parent, BaseNode *left, BaseNode *right)
        : parent(parent),
          left_child(left),
          right_child(right)
{}

template <typename T>
Set<T>::BaseNode::BaseNode(BaseNode *parent)
        : parent(parent),
          left_child(nullptr),
          right_child(nullptr)
{}

template <typename T>
Set<T>::BaseNode::~BaseNode()
{
    if (left_child)
        delete left_child;
    if (right_child)
        delete right_child;
}

template <typename T>
template <typename U>
bool Set<T>::Iterator<U>::operator==(Iterator<U> const &other)
{
    return ptr == other.ptr;
}


template <typename T>
template <typename U>
bool Set<T>::Iterator<U>::operator!=(Iterator<U> const &other)
{
    return ptr != other.ptr;
}

/// NODE IMPLEMENTATION ======================================================================

template <typename T>
Set<T>::Node::Node(Set::key_type const& key)
        : Set::BaseNode(),
          key(key)
{}

template <typename T>
Set<T>::Node::Node(Set::key_type const& key, BaseNode* parent)
        : Set::BaseNode(parent),
          key(key)
{}


template <typename T>
Set<T>::Node::Node(Set::key_type const& key, BaseNode* parent, BaseNode* left_child, BaseNode* right_child)
        : Set::BaseNode(parent, left_child, right_child),
          key(key)
{}

/// ITERATORS IMPLEMENTATION =================================================================

template <typename T>
template <typename U>
Set<T>::Iterator<U>::Iterator(BaseNode *ptr) :
        ptr(ptr)
{}

template <typename T>
template <typename U>
template <typename V>
Set<T>::Iterator<U>::Iterator(Iterator<V> const &other)
        : ptr(other.ptr)
{}

template <typename T>
template <typename U>
U& Set<T>::Iterator<U>::operator*() const
{
    return (dynamic_cast<Node*>(ptr))->key;
}

template <typename T>
template <typename U>
void Set<T>::Iterator<U>::up()
{
    ptr = ptr->parent;
}


template <typename T>
template <typename U>
Set<T>::Iterator<U>& Set<T>::Iterator<U>::operator++()
{
    if (ptr->right_child)
    {
        ptr = ptr->right_child;
        while (ptr->left_child)
            ptr = ptr->left_child;
    }
    else
    {
        while (ptr->parent->right_child == ptr)
            ptr = ptr->parent;
        ptr = ptr->parent;
    }
    return *this;
}

template <typename T>
template <typename U>
Set<T>::Iterator<U>& Set<T>::Iterator<U>::operator--()
{
    if (ptr->left_child)
    {
        ptr = ptr->left_child;
        while (ptr->right_child)
            ptr = ptr->right_child;
    }
    else
    {
        while (ptr->parent->left_child == ptr)
            ptr = ptr->parent;
        ptr = ptr->parent;
    }
    return *this;
}

template <typename T>
template <typename U>
Set<T>::Iterator<U> Set<T>::Iterator<U>::operator++(int)
{
    auto tmp(*this);
    ++(*this);
    return tmp;
}

template <typename T>
template <typename U>
Set<T>::Iterator<U> Set<T>::Iterator<U>::operator--(int)
{
    auto tmp(*this);
    --(*this);
    return tmp;
}


/// SET IMPLEMENTATION =======================================================================

template <typename T>
Set<T>::Set()
        : siz(0),
          root(new BaseNode())
{}

template <typename T>
Set<T>::Set(Set const &other)
        : siz(other.siz),
          root(new BaseNode())
{
    for (auto x : other)
    {
        insert(x);
    }
}

template <typename T>
Set<T>::~Set()
{
    delete root;
}

template <typename T>
typename Set<T>::const_iterator Set<T>::begin() const
{
    BaseNode* cur = root;
    while (cur->left_child)
        cur = cur->left_child;
    return Set<T>::const_iterator(cur);
}

template <typename T>
typename Set<T>::const_iterator Set<T>::end() const
{
    return Set<T>::const_iterator(root);
}

template <typename T>
typename Set<T>::iterator Set<T>::begin()
{
    BaseNode* cur = root;
    while (cur->left_child)
        cur = cur->left_child;
    return Set<T>::iterator(cur);
}

template <typename T>
typename Set<T>::iterator Set<T>::end()
{
    return Set<T>::iterator(root);
}

template <typename T>
bool Set<T>::empty() const
{
    return siz == 0;
}

template <typename T>
size_t Set<T>::size() const
{
    return siz;
}

template <typename T>
std::pair<typename Set<T>::iterator, bool> Set<T>::insert(key_type const &x)
{
    if (!root->left_child)
    {
        root->left_child = new Node(x, root);
        siz++;
        return { iterator(root->left_child), true };
    }

    Set<T>::BaseNode* cur = root->left_child;
    while (true)
    {
        if (dynamic_cast<Node*>(cur)->key == x)
            return { iterator(cur), false };
        if (dynamic_cast<Node*>(cur)->key > x)
        {
            if (cur->left_child)
                cur = cur->left_child;
            else
            {
                cur->left_child = new Node(x, cur);
                siz++;
                return { iterator(cur->left_child), true };
            }
        }
        if (dynamic_cast<Node*>(cur)->key < x)
        {
            if (cur->right_child)
                cur = cur->right_child;
            else
            {
                cur->right_child = new Node(x, cur);
                siz++;
                return { iterator(cur->right_child), true };
            }
        }
    }
}

template <typename T>
typename Set<T>::const_iterator Set<T>::detach(const_iterator iter)
{
    if (!iter.ptr->left_child && !iter.ptr->right_child)
    {
        if (iter.ptr->parent->left_child == iter.ptr)
            iter.ptr->parent->left_child = nullptr;
        else
            iter.ptr->parent->right_child = nullptr;
    }
    else if (!iter.ptr->left_child && iter.ptr->right_child)
    {
        if (iter.ptr->parent->left_child == iter.ptr)
            iter.ptr->parent->left_child = iter.ptr->right_child;
        else
            iter.ptr->parent->right_child = iter.ptr->right_child;
        iter.ptr->right_child->parent = iter.ptr->parent;
    }
    else if (iter.ptr->left_child && !iter.ptr->right_child)
    {
        if (iter.ptr->parent->left_child == iter.ptr)
            iter.ptr->parent->left_child = iter.ptr->left_child;
        else
            iter.ptr->parent->right_child = iter.ptr->left_child;
        iter.ptr->left_child->parent = iter.ptr->parent;
    }
    return iter;
}

template <typename T>
void Set<T>::erase(Set<T>::const_iterator iter)
{
    if (iter.ptr->left_child && iter.ptr->right_child)
    {
        auto next = iter;
        ++next;
        const_iterator cur = detach(next);

        if (iter.ptr->parent->left_child == iter.ptr)
            iter.ptr->parent->left_child = cur.ptr;
        else
            iter.ptr->parent->right_child = cur.ptr;

        iter.ptr->right_child->parent = cur.ptr;
        iter.ptr->left_child->parent = cur.ptr;

        cur.ptr->parent = iter.ptr->parent;
        cur.ptr->left_child = iter.ptr->left_child;
        cur.ptr->right_child = iter.ptr->right_child;
    }
    else
    {
        detach(iter);
    }
    --siz;
    iter.ptr->right_child = nullptr;
    iter.ptr->left_child = nullptr;
    delete iter.ptr;
}

template <typename T>
typename Set<T>::const_iterator Set<T>::find(key_type const &x) const
{
    BaseNode* cur = root->left_child;
    while (true)
    {
        if (!cur)
            return end();

        if (dynamic_cast<Node*>(cur)->key == x)
            return Set<T>::const_iterator(cur);

        if (dynamic_cast<Node*>(cur)->key < x)
            cur = cur->right_child;

        cur = cur->left_child;
    }
}

#endif //MY_SET_H
