#include "Diff.h"

int main ()
{
    double value = Calculator ();
    printf ("value = %lg\n", value);
    
    Tree* tree = TreeConstruct (tree);
    tree->root = CreateNode (TYPE_NUM, value, NULL, NULL, NULL);
    
    DiffTreeLogs (tree->root);
    
    tree = TreeDestruct (tree);
    
    return 0;
}

// Construct-Destruct
Tree* TreeConstruct (Tree* tree)
{
    tree = (Tree*) calloc (1, sizeof (Tree));
    tree->root = NULL;
    
    return tree;
}

Tree* TreeDestruct (Tree* tree)
{
    tree->root = NodeDestruct (tree->root);
    free (tree);
    
    return NULL;
}

Node* NodeConstruct (Node* node)
{
    node = (Node*) calloc (1, sizeof (Node));
    *node = {};
    
    return node;
}

Node* NodeDestruct (Node* node)
{
    if (node->left)
        node->left = NodeDestruct (node->left);
    
    if (node->right)
        node->right = NodeDestruct (node->right);
    
    free (node);
    
    return NULL;
}


Node* CreateNode (int type, double value, Node* parent, Node* left, Node* right)
{
    Node* node = NodeConstruct (node);
    *node = {type, value, parent, left, right};
    
    return node;
}
//----------------------
