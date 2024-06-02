#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_TEMPLATE_REPO_B_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_TEMPLATE_REPO_B_TREE_H

#include <search_tree.h>
#include <allocator_global_heap.h>


template<
    typename tkey,
    typename tvalue>
class b_tree final:
    public search_tree<tkey, tvalue>
{
private:
    struct Node
    {
        std::vector<tkey> keys;
        std::vector<tvalue> values;
        std::vector<Node*> children;
        bool leaf;

        Node(bool isLeaf) : leaf(isLeaf){}

        ~Node() {
            for (auto child : children)
            {
                delete child;
            }
        }

    };

    Node* root;                    // Указатель на корневой узел
    size_t t;             // Максимальная степень B-дерева
    std::function<int(tkey const &, tkey const &)> keys_comparer; // Функция сравнения ключей
    allocator* allocator;              // Указатель на аллокатор
    logger* logger;

    void insert_non_full(Node *x, tkey const &key, tvalue const &value);
    void split_child(Node *x, int i, Node *y);
    void copy_tree(Node* & thisNode, Node* const & otherNode);
    void free_tree(Node* node);

public:

    class infix_iterator final
    {
    private:
        Node* current_node; //текущий узел
        size_t key_index; //индекс текущего узла
        std::stack<std::pair<Node*, size_t>> path; //путь до узла

    public:
        infix_iterator(Node* start_node = nullptr);

        bool operator==(
            infix_iterator const &other) const noexcept;

        bool operator!=(
            infix_iterator const &other) const noexcept;

        infix_iterator &operator++();

        infix_iterator operator++(
            int not_used);

        std::tuple<size_t, size_t, tkey const &, tvalue &> operator*() const; //индексы ключа и узла, ссылки на ключ и на узел

    };

    class infix_const_iterator final
    {
    private:
        Node* current_node; //текущий узел
        size_t key_index; //индекс текущего узла
        std::stack<std::pair<Node*, size_t>> path; //путь до узла

    public:
        infix_const_iterator(Node* start_node = nullptr);

        bool operator==(
            infix_const_iterator const &other) const noexcept;

        bool operator!=(
            infix_const_iterator const &other) const noexcept;

        infix_const_iterator &operator++();

        infix_const_iterator operator++(
            int not_used);

        std::tuple<size_t, size_t, tkey const &, tvalue const &> operator*() const;

    };

public:

    void insert(
        tkey const &key,
        tvalue const &value) override;

    void insert(
        tkey const &key,
        tvalue &&value) override;

    tvalue const &obtain(
        tkey const &key) override;

    tvalue dispose(
        tkey const &key) override;

    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) override;

public:

    explicit b_tree(
        size_t t,
        std::function<int(tkey const &, tkey const &)> keys_comparer = std::less<tkey>(),
        allocator *allocator = nullptr,
        logger *logger = nullptr);

    b_tree(
        b_tree<tkey, tvalue> const &other);

    b_tree<tkey, tvalue> &operator=(
        b_tree<tkey, tvalue> const &other);

    b_tree(
        b_tree<tkey, tvalue> &&other) noexcept;

    b_tree<tkey, tvalue> &operator=(
        b_tree<tkey, tvalue> &&other) noexcept;

    ~b_tree();

public:

    infix_iterator begin_infix() const noexcept;

    infix_iterator end_infix() const noexcept;

    infix_const_iterator cbegin_infix() const noexcept;

    infix_const_iterator cend_infix() const noexcept;

};

template<typename tkey, typename tvalue>
b_tree<tkey, tvalue>::infix_iterator::infix_iterator(Node* start_node)
        : current_node(start_node), key_index(0)
{
    if (start_node != nullptr)
    {
        while (!current_node->is_leaf)
        {
            path.push({current_node, 0});
            current_node = current_node->children[0];
        }
    }
}

template<
    typename tkey,
    typename tvalue>
bool b_tree<tkey, tvalue>::infix_iterator::operator==(
    typename b_tree::infix_iterator const &other) const noexcept
{
    return current_node = other.current_node && key_index == other.key_index;
}

template<
    typename tkey,
    typename tvalue>
bool b_tree<tkey, tvalue>::infix_iterator::operator!=(
    typename b_tree::infix_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_iterator &b_tree<tkey, tvalue>::infix_iterator::operator++()
{
  /*  if(current_node == nullptr)
    {
        return *this;
    }

    if(key_index + 1 < current_node->keys.size())
    {
        ++current_node;
    }
    else
    {
        if(current_node->is_leaf)
        {
            while (!path.empty() && key_index >= current_node->keys.size() - 1)
            {
                auto [parent_node, parent_index] = path.top();
                path.pop();
                current_node = parent_node;
                key_index = parent_index + 1;
            }
            if (key_index >= current_node->keys.size())
            {
                current_node = nullptr;
            }
        }
        else
        {
            current_node = current_node->children[key_index + 1];
            while (!current_node->is_leaf)
            {
                path.push({current_node, 0});
                current_node = current_node->children[0];
            }
            key_index = 0;
        }
    }
    return *this;*/
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_iterator b_tree<tkey, tvalue>::infix_iterator::operator++(
    int not_used)
{
    /*infix_iterator temp = *this;
    ++(*this);
    return temp;*/
}

template<
    typename tkey,
    typename tvalue>
std::tuple<size_t, size_t, tkey const &, tvalue &> b_tree<tkey, tvalue>::infix_iterator::operator*() const
{
    throw not_implemented("template<typename tkey, typename tvalue> std::tuple<size_t, size_t, tkey const &, tvalue &> b_tree<tkey, tvalue>::infix_iterator::operator*() const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool b_tree<tkey, tvalue>::infix_const_iterator::operator==(
    b_tree::infix_const_iterator const &other) const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> bool b_tree<tkey, tvalue>::infix_const_iterator::operator==(b_tree::infix_const_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
bool b_tree<tkey, tvalue>::infix_const_iterator::operator!=(
    b_tree::infix_const_iterator const &other) const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> bool b_tree<tkey, tvalue>::infix_const_iterator::operator!=(b_tree::infix_const_iterator const &) const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator &b_tree<tkey, tvalue>::infix_const_iterator::operator++()
{
    throw not_implemented("template<typename tkey, typename tvalue> typename b_tree<tkey, tvalue>::infix_const_iterator &b_tree<tkey, tvalue>::infix_const_iterator::operator++()", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::infix_const_iterator::operator++(
    int not_used)
{
    throw not_implemented("template<typename tkey, typename tvalue> typename b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::infix_const_iterator::operator++(int)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
std::tuple<size_t, size_t, tkey const &, tvalue const &> b_tree<tkey, tvalue>::infix_const_iterator::operator*() const
{
    throw not_implemented("template<typename tkey, typename tvalue> std::tuple<size_t, size_t, tkey const &, tvalue const &> b_tree<tkey, tvalue>::infix_const_iterator::operator*() const", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
void b_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue const &value)
{
    if(!root)
    {
        root = new Node(true);
        root->keys.push_back(key);
        root->values.push_back(value);
    }
    else
    {
        if(root->keys.size() == 2 * t - 1)
        {
            Node *s = new Node(false);
            s->children.push_back(root);
            split_child(s, 0, root);
            root = s;
        }
        insert_non_full(root, key, value);
    }
}

template<
    typename tkey,
    typename tvalue>
void b_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue &&value)
{
    insert(key, static_cast<tvalue const &>(value));
}

template<
    typename tkey,
    typename tvalue>
tvalue const &b_tree<tkey, tvalue>::obtain(
    tkey const &key)
{
    throw not_implemented("template<typename tkey, typename tvalue> tvalue const &b_tree<tkey, tvalue>::obtain(tkey const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
tvalue b_tree<tkey, tvalue>::dispose(
    tkey const &key)
{
    throw not_implemented("template<typename tkey, typename tvalue> tvalue b_tree<tkey, tvalue>::dispose(tkey const &)", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> b_tree<tkey, tvalue>::obtain_between(
    tkey const &lower_bound,
    tkey const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
    throw not_implemented("template<typename tkey, typename tvalue> std::vector<typename associative_container<tkey, tvalue>::key_value_pair> b_tree<tkey, tvalue>::obtain_between(tkey const &, tkey const &, bool, bool)", "your code should be here...");
}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::copy_tree(Node* & thisNode, Node* const & otherNode) {
    if (otherNode) {
        thisNode = new Node(otherNode->leaf);
        thisNode->keys = otherNode->keys;
        thisNode->values = otherNode->values;
        thisNode->children.resize(otherNode->children.size());
        for (size_t i = 0; i < otherNode->children.size(); ++i) {
            copy_tree(thisNode->children[i], otherNode->children[i]);
        }
    }
}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::insert_non_full(Node *x, tkey const &key, tvalue const &value) {
    int i = x->keys.size() - 1;
    if (x->leaf) {
        x->keys.push_back(key);
        x->values.push_back(value);
        while (i >= 0 && keys_comparer(key, x->keys[i]) < 0) {
            x->keys[i + 1] = x->keys[i];
            x->values[i + 1] = x->values[i];
            i--;
        }
        x->keys[i + 1] = key;
        x->values[i + 1] = value;
    } else {
        while (i >= 0 && keys_comparer(key, x->keys[i]) < 0) {
            i--;
        }
        i++;
        if (x->children[i]->keys.size() == 2 * t - 1) {
            split_child(x, i, x->children[i]);
            if (keys_comparer(key, x->keys[i]) > 0) {
                i++;
            }
        }
        insert_non_full(x->children[i], key, value);
    }
}

template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::split_child(Node *x, int i, Node *y) {
    Node *z = new Node(y->leaf);
    for (int j = 0; j < t - 1; j++) {
        z->keys.push_back(y->keys[j + t]);
        z->values.push_back(y->values[j + t]);
    }
    if (!y->leaf) {
        for (int j = 0; j < t; j++) {
            z->children.push_back(y->children[j + t]);
        }
    }
    y->keys.resize(t - 1);
    y->values.resize(t - 1);
    y->children.resize(t);
    x->children.insert(x->children.begin() + i + 1, z);
    x->keys.insert(x->keys.begin() + i, y->keys[t - 1]);
    x->values.insert(x->values.begin() + i, y->values[t - 1]);
}


template<typename tkey, typename tvalue>
void b_tree<tkey, tvalue>::free_tree(Node* node) {
    if (node) {
        for (auto child : node->children) {
            free_tree(child);
        }
        delete node;
    }
}


template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::b_tree(
    size_t t,
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    allocator *allocator,
    logger *logger) : t(t), root(nullptr), keys_comparer(keys_comparer), allocator(allocator), logger(logger)
{
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::b_tree(
    b_tree<tkey, tvalue> const &other) : t(other.t), root(nullptr), keys_comparer(other.keys_comparer), allocator(other.allocator), logger(other.logger)
{
    if (other.root)
    {
        copy_tree(root, other.root);
    }
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue> &b_tree<tkey, tvalue>::operator=(b_tree<tkey, tvalue> const &other)
{
    if(this != &other) {
        free_tree(root);
        t = other.t;
        keys_comparer = other.keys_comparer;
        allocator = other.allocator;
        logger = other.logger;
        root = nullptr;
        if (other.root) {
            copy_tree(root, other.root);
        }
    }
    return *this;
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::b_tree(
    b_tree<tkey, tvalue> &&other) noexcept:
    t(other.t), root(other.root), keys_comparer(std::move(other.keys_comparer)), allocator(other.allocator), logger(other.logger)
{
    other.root = nullptr;

}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue> &b_tree<tkey, tvalue>::operator=(
    b_tree<tkey, tvalue> &&other) noexcept
{
    if (this != other) {
        free_tree(root);
        t = other.t;
        keys_comparer = std::move(other.keys_comparer);
        allocator = other.allocator;
        logger = other.logger;
        root = other.root;
        other.root = nullptr;
    }
    return *this;
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::~b_tree()
{
    free_tree(root);
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_iterator b_tree<tkey, tvalue>::begin_infix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> typename b_tree<tkey, tvalue>::infix_iterator b_tree<tkey, tvalue>::begin_infix() const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_iterator b_tree<tkey, tvalue>::end_infix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> typename b_tree<tkey, tvalue>::infix_iterator b_tree<tkey, tvalue>::end_infix() const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::cbegin_infix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> typename b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::cbegin_infix() const noexcept", "your code should be here...");
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::cend_infix() const noexcept
{
    throw not_implemented("template<typename tkey, typename tvalue> typename b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::cend_infix() const noexcept", "your code should be here...");
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_TEMPLATE_REPO_B_TREE_H