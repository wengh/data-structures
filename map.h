#ifndef MAP_H
#define MAP_H
namespace cs246e{
    class key_error {};
    template <typename T> class stack {
        struct Node {
            T data;
            Node *next;
            Node(T data, Node *next): data{data}, next{next} {}
            Node(const Node &other): data{other.data}, next{other.next? new Node{*other.next}:nullptr}{}
            Node(Node &&other): data{other.data}, next{other.next} {
                other.next = nullptr;
            }
            Node &operator=(Node other){
                data = other.data;
                std::swap(next, other.next);
                return *this;
            }
            ~Node() {delete next;}
        };

        Node *theStack = nullptr;
        size_t len = 0;

        public:
        stack() {}
        stack(const stack &other): theStack{other.theStack ? new Node{*other.theStack}: nullptr}, len{other.len} {}
        stack(stack &&other): theStack{other.theStack}, len{other.len} {
            other.theStack = nullptr;
            other.len = 0;
        }
        stack &operator=(stack other) {
            std::swap(theStack, other.theStack);
            std::swap(len, other.len);
            return *this;
        }
        size_t size() const {return len;}
        stack &push_front(T n) {
            theStack = new Node{std::move(n), theStack};
            ++len;
            return *this;
        }
        stack &pop_front() {
            if (len) {
                Node *tmp = theStack;
                theStack = theStack->next;
                tmp->next = nullptr;
                delete tmp;
                --len;
            }
            return *this;
        }
        
        T& topElement() const {
            return (this->theStack)->data;
        }

        bool empty() const {
            return len == 0;
        }
        ~stack() {delete theStack;}
    };
//****************************************************************************
    template <typename K, typename V> class map {
        class treeNode{
            treeNode* left;
            treeNode* right;
            K key;
            V value;
            treeNode(K key, V value): left{nullptr}, right{nullptr}, key {key}, value{value} {};
            friend class map;
        };
        
        size_t treeSize;
        treeNode* root;

        //Private helper
        V& lookUp(treeNode*& node, const K& key) {
            if(node == nullptr) {
                node = new treeNode {key, V()};
                ++treeSize;
                return node->value;
            } else if (key < node->key) {
                return lookUp(node->left, key);
            } else if (key > node->key) {
                return lookUp(node->right, key);
            } else {
                return node->value;
            }
        }

        // const V& lookUp(treeNode*& node, const K& key) const {
        //     if(node == nullptr) {
        //         node = new treeNode {key, V()};
        //         ++treeSize;
        //         return node->value;
        //     } else if (key < node->key) {
        //         return lookUp(node->left, key);
        //     } else if (key > node->key) {
        //         return lookUp(node->right, key);
        //     } else {
        //         return node->value;
        //     }
        // }

        int find(treeNode*& node, const K& key) {
            if(node == nullptr) {
                return 0;
            } else if (key < node->key) {
                return find(node->left, key);
            } else if (key > node->key) {
                return find(node->right, key);
            } else {
                return 1;
            }
        }

        void deleteTree(treeNode* node) {
            if (node == nullptr) { return; }
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }

        treeNode* deepCopy(treeNode* node) {
            if (node) {
                treeNode* root = new treeNode{node->key, node->value};
                root->left = deepCopy(node->left);
                root->right = deepCopy(node->right);
                return root;
            } else {
                return nullptr;
            }
        }

        void swap (map& other) {
            using std::swap;
            swap(treeSize, other.treeSize);
            swap(root, other.root);
        }
        public:
        //default constructor
        map(): treeSize{0}, root {nullptr} {}

        //List initializer
        map(std::initializer_list<std::pair<const K, V>> init): treeSize{0}, root {nullptr} {
            for(auto a : init) {
                lookUp(root, a.first) = a.second;
            }
        }

        size_t count(const K& key) { return find(root, key); }

        //copy constructor
        map(const map& other): treeSize {other.treeSize}, root {deepCopy(other.root)} {}

        //move constructor
        map(const map&& other): treeSize {other.treeSize}, root {other.root} { other.root = nullptr; }

        //copy/move assignment
        map& operator=(map other) {
            swap(other);
            return *this;
        }

        ~map() { deleteTree(root); }

        V& at(const K& key) { 
            if(count(key) == 0) {
                throw key_error();
            } else {
                return lookUp(root, key); 
            }
        }

        // const V& at(const K& key) const{ 
        //     if(count(key) == 0) {
        //         throw key_error();
        //     } else {
        //         return lookUp(root, key); 
        //     }
        // }

        V& operator[](const K& key) { return lookUp(root, key); }

        // const V& operator[] (const K& key) const { return lookUp(root, key); }

        bool empty() { return root == nullptr; }

        size_t size() { return treeSize; }

        void clear() { 
            deleteTree(root);
            root = nullptr; 
        }


        private:
        //erase helper
        treeNode* deleteNode(treeNode* curr, const K& key) {
            if(curr) {
                if(key < curr->key) {
                    curr->left = deleteNode(curr->left, key);
                } else if(key > curr->key) {
                    curr->right = deleteNode(curr->right, key);
                } else {
                    if(!curr->left || !curr->right){
                        auto tmp = curr->left ? curr->left : curr->right;
                        delete curr;
                        --treeSize;
                        return tmp; 
                    }
                    treeNode* tmp = curr->left;
                    while(tmp->right != nullptr) tmp = tmp->right;

                    curr->key = tmp->key;
                    curr->value = tmp->value;
                    curr->left = deleteNode(curr->left, tmp->key);
                }
            }
            return curr;
        }
        public:
        size_t erase(const K& key) {
            size_t originalSize = treeSize;
            root = deleteNode(root, key);
            return originalSize - treeSize;
        }

        //iterator implementation
        class iterator {
            stack<treeNode*> theIterator {};
            iterator(treeNode* root) { fillDeque(root); }
            //private helper
            void fillDeque(treeNode* root) {
                while(root) {
                    theIterator.push_front(root);
                    root = root->left;
                }
            }
            public:
            bool operator!=(const iterator& other) const { 
                if(theIterator.empty() && other.theIterator.empty()){ 
                    return false;
                } else if (theIterator.empty() || other.theIterator.empty()){
                    return true;
                } else {
                    return theIterator.topElement() != other.theIterator.topElement(); 
                }
            }
            iterator &operator++(){
                treeNode* tmp = theIterator.topElement();
                theIterator.pop_front();
                fillDeque(tmp->right);
                return *this;
            }
            const std::pair<const K&, V&> operator*(){ 
                std::pair<const K&, V&> tmp (theIterator.topElement()->key, theIterator.topElement()->value);
                return tmp;
            }
            friend class map;
        };

        iterator begin() { return iterator(root); }
        iterator end() { return iterator(nullptr); }

        //const_iterator implementation
        class const_iterator {
            stack<treeNode*> theIterator {};
            const_iterator(treeNode* root) { fillDeque(root); }
            //private helper
            void fillDeque(treeNode* root) {
                while(root) {
                    theIterator.push_front(root);
                    root = root->left;
                }
            }
            public:
            bool operator!=(const const_iterator& other) const { 
                if(theIterator.empty() && other.theIterator.empty()){ 
                    return false;
                } else if (theIterator.empty() || other.theIterator.empty()){
                    return true;
                } else {
                    return theIterator.topElement() != other.theIterator.topElement(); 
                }
            }
            const_iterator &operator++(){
                treeNode* tmp = theIterator.topElement();
                theIterator.pop_front();
                fillDeque(tmp->right);
                return *this;
            }
            const std::pair<const K&, const V&> operator*() const{ 
                std::pair<const K&, const V&> tmp (theIterator.topElement()->key, theIterator.topElement()->value);
                return tmp;
            }
            friend class map;
        };

        const_iterator begin() const { return const_iterator(root); }
        const_iterator end() const { return const_iterator(nullptr); }
    };
}
#endif
