#ifndef MY_SET_H
#define MY_SET_H

#include <cstddef>
#include <utility>
#include <iterator>

template <typename T>
class set
{
    typedef T key_type;

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
        friend class set;

    private:
        BaseNode* ptr;

    public:
        Iterator(); // private
        explicit Iterator(BaseNode* ptr);

        template <typename V>
        Iterator(Iterator<V> const& other);

        U& operator*() const;
        U* operator->() const;

        bool operator==(Iterator const& other) const;
        bool operator!=(Iterator const& other) const;

        Iterator& operator=(Iterator const& other);

        Iterator& operator++();
        Iterator operator++(int);
        Iterator& operator--();
        Iterator operator--(int);
    };

public:
    using iterator = Iterator<T const>;
    using const_iterator = Iterator<T const>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    size_t siz;
    BaseNode root;

    const_iterator detach(const_iterator iter);
    BaseNode* get_root_pointer() const;

public:

    set();
    set(set const& other) ;

    ~set();

    set& operator=(set other);


    std::pair<iterator, bool> insert(key_type const& x);
    iterator erase(const_iterator iter);
    const_iterator find(key_type const& x) const;
    const_iterator lower_bound(key_type const& x) const;
    const_iterator upper_bound(key_type const& x) const;

    bool empty() const;
    size_t size() const;
    void clear();

    iterator begin() const;
    iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
    reverse_iterator rbegin() const;
    reverse_iterator rend() const;
    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;

    void swap(set<T> &other);
};


/// BASE NODE IMPLEMENTATION =================================================================

template <typename T>
set<T>::BaseNode::BaseNode()
        : parent(nullptr),
          left_child(nullptr),
          right_child(nullptr)
{}

template <typename T>
set<T>::BaseNode::BaseNode(BaseNode *parent, BaseNode *left, BaseNode *right)
        : parent(parent),
          left_child(left),
          right_child(right)
{}

template <typename T>
set<T>::BaseNode::BaseNode(BaseNode *parent)
        : parent(parent),
          left_child(nullptr),
          right_child(nullptr)
{}

template <typename T>
set<T>::BaseNode::~BaseNode()
{
    if (left_child)
        delete left_child;
    if (right_child)
        delete right_child;
}

// V
template <typename T>
template <typename U>
bool set<T>::Iterator<U>::operator==(Iterator<U> const &other) const
{
    return ptr == other.ptr;
}

// V
template <typename T>
template <typename U>
bool set<T>::Iterator<U>::operator!=(Iterator<U> const &other) const
{
    return ptr != other.ptr;
}

/// NODE IMPLEMENTATION ======================================================================

template <typename T>
set<T>::Node::Node(set::key_type const& key)
        : set::BaseNode(),
          key(key)
{}

template <typename T>
set<T>::Node::Node(set::key_type const& key, BaseNode* parent)
        : set::BaseNode(parent),
          key(key)
{}


template <typename T>
set<T>::Node::Node(set::key_type const& key, BaseNode* parent, BaseNode* left_child, BaseNode* right_child)
        : set::BaseNode(parent, left_child, right_child),
          key(key)
{}

/// ITERATORS IMPLEMENTATION =================================================================

template <typename T>
template <typename U>
set<T>::Iterator<U>::Iterator(BaseNode *ptr) :
        ptr(ptr)
{}

template <typename T>
template <typename U>
template <typename V>
set<T>::Iterator<U>::Iterator(Iterator<V> const &other)
        : ptr(other.ptr)
{}

template <typename T>
template <typename U>
U& set<T>::Iterator<U>::operator*() const
{
    return (dynamic_cast<Node*>(ptr))->key;
}


template <typename T>
template <typename U>
set<T>::Iterator<U>& set<T>::Iterator<U>::operator++()
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
set<T>::Iterator<U>& set<T>::Iterator<U>::operator--()
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
set<T>::Iterator<U> set<T>::Iterator<U>::operator++(int)
{
    auto tmp(*this);
    ++(*this);
    return tmp;
}

template <typename T>
template <typename U>
set<T>::Iterator<U> set<T>::Iterator<U>::operator--(int)
{
    auto tmp(*this);
    --(*this);
    return tmp;
}

template<typename T>
template<typename U>
U *set<T>::Iterator<U>::operator->() const {
    return &(dynamic_cast<Node*>(ptr)->key);
}

template<typename T>
template<typename U>
typename set<T>::template Iterator<U> &set<T>::Iterator<U>::operator=(const set<T>::Iterator<U> &other)
{
    ptr = other.ptr;
    return *this;
}

template<typename T>
template<typename U>
set<T>::Iterator<U>::Iterator() : ptr(nullptr)
{}


/// SET IMPLEMENTATION =======================================================================

template <typename T>
set<T>::set()
        : siz(0),
          root()
{}

template <typename T>
set<T>::set(set const &other)
        : siz(other.siz),
          root()
{
    for (auto x : other)
    {
        insert(x);
    }
}

template <typename T>
set<T>::~set()
{
    delete root.left_child;
    root.left_child = nullptr;
}


template <typename T>
typename set<T>::iterator set<T>::begin() const
{
    BaseNode * cur = get_root_pointer();
    while (cur->left_child)
        cur = cur->left_child;
    return set<T>::iterator(cur);
}

template <typename T>
typename set<T>::iterator set<T>::end() const
{
    return set<T>::iterator(get_root_pointer());
}

template <typename T>
typename set<T>::reverse_iterator set<T>::rbegin() const
{
    return set<T>::reverse_iterator(end());
}

template <typename T>
typename set<T>::const_reverse_iterator set<T>::crend() const
{
    return set<T>::const_reverse_iterator(set<T>::iterator(begin()));
}

template <typename T>
typename set<T>::const_reverse_iterator set<T>::crbegin() const
{
    return set<T>::const_reverse_iterator(end());
}

template <typename T>
typename set<T>::reverse_iterator set<T>::rend() const
{
    return set<T>::reverse_iterator(set<T>::iterator(begin()));
}

template <typename T>
bool set<T>::empty() const
{
    return siz == 0;
}

template <typename T>
size_t set<T>::size() const
{
    return siz;
}

template <typename T>
std::pair<typename set<T>::iterator, bool> set<T>::insert(key_type const &x)
{
    if (!root.left_child)
    {
        root.left_child = new Node(x, &root);
        siz++;
        return { iterator(root.left_child), true };
    }

    set<T>::BaseNode* cur = root.left_child;
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
typename set<T>::const_iterator set<T>::detach(const_iterator iter)
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
typename set<T>::iterator set<T>::erase(set<T>::const_iterator iter)
{
    iterator ret = iter;
    ++ret;

    if (iter.ptr->left_child && iter.ptr->right_child)
    {
        auto next = iter;
        ++next;
        const_iterator cur = detach(next);

        if (iter.ptr->parent->left_child == iter.ptr)
            iter.ptr->parent->left_child = cur.ptr;
        else
            iter.ptr->parent->right_child = cur.ptr;

        if (iter.ptr->right_child)
            iter.ptr->right_child->parent = cur.ptr;
        if (iter.ptr->left_child)
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
    return ret;
}

template <typename T>
typename set<T>::const_iterator set<T>::find(key_type const &x) const
{
    BaseNode* cur = root.left_child;
    while (true)
    {
        if (!cur)
            return end();

        if (dynamic_cast<Node*>(cur)->key == x)
            return set<T>::const_iterator(cur);
        else if (dynamic_cast<Node*>(cur)->key < x)
            cur = cur->right_child;
        else if (dynamic_cast<Node*>(cur)->key > x)
            cur = cur->left_child;
    }
}

template <typename T>
void set<T>::clear()
{
    siz = 0;
    if (root.left_child)
        delete root.left_child;
    root.left_child = nullptr;
}

template <typename T>
typename set<T>::const_iterator set<T>::lower_bound(key_type const &x) const
{
    // Итератор первого >= x

    BaseNode* cur = root.left_child;
    if (!cur)
        return end();

    BaseNode* ans = get_root_pointer();
    while (cur)
    {
        if (dynamic_cast<Node*>(cur)->key < x)
            cur = cur->right_child;
        else
        {
            ans = cur;
            cur = cur->left_child;
        }
    }
    return set<T>::const_iterator(ans);
}

template <typename T>
typename set<T>::const_iterator set<T>::upper_bound(key_type const &x) const  // T?
{
    // Итератор первого > x

    BaseNode* cur = root.left_child;
    if (!cur)
        return end();

    BaseNode* ans = get_root_pointer();
    while (cur)
    {
        if (dynamic_cast<Node*>(cur)->key <= x)
            cur = cur->right_child;
        else
        {
            ans = cur;
            cur = cur->left_child;
        }
    }
    return set<T>::const_iterator(ans);
}

template<typename T>
void set<T>::swap(set<T> &other)
{
    std::swap(siz, other.siz);
    if (root.left_child && other.root.left_child)
        std::swap(root.left_child->parent, other.root.left_child->parent);
    else if (root.left_child)
        root.left_child->parent = other.get_root_pointer();
    else if (other.root.left_child)
        other.root.left_child->parent = get_root_pointer();
    std::swap(root.left_child, other.root.left_child);
}

template<typename T>
typename set<T>::const_iterator set<T>::cbegin() const {
    return set::const_iterator(begin());
}

template<typename T>
typename set<T>::const_iterator set<T>::cend() const {
    return set::const_iterator(end());
}

template<typename T>
set<T>& set<T>::operator=(set<T> other) {
    swap(other);
    return *this;
}

template<typename T>
typename set<T>::BaseNode *set<T>::get_root_pointer() const {
    return const_cast<typename set<T>::BaseNode*>(&root);
}

template <typename T>
void swap(set<T> &a, set<T> &b)
{
    a.swap(b);
}

#endif //MY_SET_H
