#include <iostream>
using namespace std;


template <typename Key, typename Info>
class TreeNode
{
public:
    Key key;
    Info info;
    int factor;
    TreeNode *left;
    TreeNode *right;
    
    TreeNode() : left(nullptr), factor(0), right(nullptr) {};
    ~TreeNode()  {left=right=nullptr;};
    TreeNode(Key k,Info i) : left(nullptr), right(nullptr), key(k), factor(0),  info(i) {};
};

template <typename Key, typename Info>
class Dictionary
{
    void _Insert(TreeNode<Key,Info> *&x, const Key &key, const Info &info)
    {
        if (x == nullptr)
        {
            x = new TreeNode<Key,Info>(key, info);
            return;
        }
        else if (key < x->key)                                                                            //if turns to the left
        {
            _Insert(x->left, key, info);
            
            if(height(x->left) - height(x->right) == 2)                                                   //if bfactor equals 2
            {
                if(key < x->left->key)
                {
                    right_rot(x);
                }
                
                else
                {
                    Lright_rot(x);
                }
            }
        }
        else                                                                                               //if turn to the right
        {
            _Insert(x->right, key, info);
            
            if(height(x->left) - height(x->right) == -2)
            {
                if(key > x->right->key)
                {
                    left_rot(x);
                }
                else
                {
                    Rleft_rot(x);
                }
            }
        }
    }
    void _Clear(TreeNode<Key, Info> *x)
    {
        if (x!=nullptr)
        {
            _Clear(x->left);
            _Clear(x->right);
            delete x;
        }
    }
    
    int BalanceFactor(TreeNode<Key,Info> *x)
    {
         return height(x->left)-height(x->right);
    }
    
    void setFactor(TreeNode<Key,Info> *x)
    {
        if (x != nullptr)
        {
            setFactor(x->left);
            setFactor(x->right);
            x->factor = BalanceFactor(x);
        }
    }
    
    int height(TreeNode<Key,Info> *x)
    {
        if (x == nullptr)
            return 0;
        
        if (x->left==nullptr && x->right==nullptr)
            return 1;
        
        int heightL = height(x->left);
        int heightR = height(x->right);
        if (heightL>heightR)
        {
            return heightL+1;
        }
        else
        {
            return heightR+1;
        }
    }
    
    void left_rot(TreeNode<Key,Info> *&ptr)        //Left rotation
    {
        TreeNode<Key,Info> *a, *b;
        a = ptr;
        b = a->right;
        ptr = b;
        a->right = b->left;
        b->left = a;
    }
    void right_rot(TreeNode<Key,Info> *&ptr)        //Rignt rotation
    {
        TreeNode<Key,Info> *a, *b;
        a = ptr;
        b = a->left;
        ptr = b;
        a->left = b->right;
        b->right = a;
    }
    void Rleft_rot(TreeNode<Key,Info> *&ptr)        //Right Left rotation
    {
        TreeNode <Key,Info> *a, *b, *c;
        a = ptr;
        b = a->right;
        c = b->left;
        
        a->right = c;
        b->left = c->right;
        c->right = b;
        
        ptr = c;
        a->right = c->left;
        c->left = a;
    }
    void Lright_rot(TreeNode<Key,Info> *&ptr)        //Left Right rotation
    {
        TreeNode <Key,Info> *a, *b, *c;
        a = ptr;
        b = a->left;
        c = b->right;
        
        a->left = c;
        b->right = c->left;
        c->left = b;
        
        ptr = c;
        a->left = c->right;
        c->right = a;
    } 
    void _print(TreeNode<Key,Info> *x)
    {
        if (x != nullptr)
        {
            _print(x->left);
            cout << x->key << " " << x->info << endl;
            _print(x->right);
        }
    }
    
    int balance_tree(TreeNode<Key,Info> *&subtreeRoot)
    {
        if (subtreeRoot == nullptr)
            return 0;
        
        int hightLeft = balance_tree(subtreeRoot->left),
        hightRight = balance_tree(subtreeRoot->right);
        
        subtreeRoot->factor = hightLeft - hightRight;
        
        if (subtreeRoot->factor == -2 && (subtreeRoot->right->factor == -1 || subtreeRoot->right->factor == 0)) {
            left_rot(subtreeRoot);
            return balance_tree(subtreeRoot);
        }
        if (subtreeRoot->factor == 2 && (subtreeRoot->left->factor == 1 || subtreeRoot->left->factor == 0)) {
            right_rot(subtreeRoot);
            return balance_tree(subtreeRoot);
        }
        if (subtreeRoot->factor == -2 && subtreeRoot->right->factor == 1){
            Rleft_rot(subtreeRoot);
            return balance_tree(subtreeRoot);
        }
        if (subtreeRoot->factor == 2 && subtreeRoot->left->factor == -1) {
            Lright_rot(subtreeRoot);
            return balance_tree(subtreeRoot);
        }
        if (hightLeft>hightRight) return hightLeft+1;
        else return hightRight+1;
    }
    
    
public:
    Dictionary(): root_node(nullptr) {}                                                  
    ~Dictionary()                                                                   
    {
        clear();
    }
     
    bool insert(const Key &key, const Info &info)                         //insert TreeNode. balances the tree
    {
        if (root_node == nullptr)
        {
            TreeNode<Key,Info> *new_ptr = new TreeNode<Key,Info>(key,info);
            root_node = new_ptr;
            return true;
        }
        if(find(key))
        {
            cout << " Key : " << key << " exists " << endl;
            return false;
        }
        _Insert(root_node, key, info);
        setFactor(root_node);
        return true;
    } 

    bool remove(const Key &_key)
    {
        if (root_node == nullptr)
        {
            return 0;
        }
        bool found = false;
        TreeNode<Key,Info>  *del_ptr = root_node, * parent = nullptr;     
        while (!found && del_ptr != nullptr) {
            if (_key < del_ptr->key) {
                parent = del_ptr;
                del_ptr = del_ptr->left;
            }
            else if (_key > del_ptr->key) {
                parent = del_ptr;
                del_ptr = del_ptr->right;
            }
            else
                found = true;
        }
        
        if (!found)
            return false;
        
        if (del_ptr->left != nullptr && del_ptr->right != nullptr)                                //TreeNode which has 2 children
        {
            TreeNode<Key,Info> *del_ptr_successor = del_ptr->right;
            parent = del_ptr;
            while(del_ptr_successor->left != nullptr)
            {
                parent = del_ptr_successor;
                del_ptr_successor = del_ptr_successor->left;
            }
            del_ptr->key = del_ptr_successor->key;
            del_ptr->info = del_ptr_successor->info;
            del_ptr = del_ptr_successor;
        }
        
        TreeNode<Key,Info> *subtree = del_ptr->left;
        if (subtree == nullptr) subtree = del_ptr->right;
        if (parent == nullptr) root_node = subtree;
        else if (parent->left == del_ptr) parent->left = subtree;
        else parent->right = subtree;
        
        delete del_ptr;
        balance_tree(root_node);                                                             //after deleting balance the tree
        return true;
    }
    
    void print()
    {
         _print(root_node);
    }
    
    bool find(const Key &key)
    {
        TreeNode<Key,Info> *temp = root_node;
        while(temp != nullptr)
        {
            if (key < temp->key)
            {
                temp = temp->left;
            }
            else if (key > temp->key)
            {
                temp = temp->right;
            }
            else return 1;
        }
        return 0;
    }
    
    bool isempty()
    {
        return (root_node==nullptr);
    }
    
    int height()
    {
        return height(root_node);
        
    }
    
    void clear()
    {
        _Clear(root_node);
        root_node = nullptr;
    }
    
    void print(const Key &findkey)const
    {
        TreeNode <Key,Info> * x = root_node;
        bool found = false;
        
        while (!found && x != nullptr) {
            if (findkey < x->key)
                x = x->left;
            else if (findkey > x->key)
                x = x->right;
            else
                found = true;
        }
        if (found)
        {
            cout<<x->key<<": "<<x->info<<endl;
            return;
        }
        else
        {
            cout  << "not found\n";
            return;
        }
    }
    private:
    TreeNode<Key,Info> *root_node;
};
struct Test
{
    static void run(){
    Dictionary <int, string> A;  
    A.insert(102,"A");
    A.insert(218,"B");
    A.insert(80,"C");
    A.insert(90,"D");
    A.insert(60,"E");
    A.insert(550,"F");
    A.print();
    
    cout << " remove 60 " << endl;
    A.remove(60);
    
    A.print();
    A.insert(5,"Five");
    A.insert(3,"Three");
    A.print();
    
    A.insert(2,"Two");
    cout<<"left rotation"<<endl;
    A.print();
     
    A.insert(6,"Six");
    A.insert(7,"Seven");
    cout<<"right rotation"<<endl;
    A.print();
    cout<<endl;
     
    cout<<" add 8,9,1"<<endl;
    A.insert(8,"Eight");
    A.insert(9,"Nine");
    A.insert(1,"One");
    cout<<endl;
    A.print();
     
    cout<<"removing the root and left-right rotation"<<endl;
    A.remove(6);
    A.print();
    cout<<"Ascending order"<<endl;
    A.print();
    cout<<endl<<endl;
     
   
    cout<<endl<<endl;
     
    A.insert(60,"E");
    A.insert(80,"C");
    A.insert(90,"D");
    A.insert(102,"A");
    A.insert(218,"B");
    A.insert(550,"F");
     
    A.print();
    A.insert(102,"can't be added");
     
    A.print();
    }
};

int main(){
    Test::run();
}
