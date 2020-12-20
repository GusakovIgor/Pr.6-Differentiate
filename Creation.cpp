#include "Diff.h"


// Construct-Destruct
    // String
string* StringConstruct (string* str)
{
    str = (string*) calloc (1, sizeof (string));

    str->pointer = (char*) calloc (MAX_STRING_LEN, sizeof(char));
    str->offset  = 0;

    return str;
}

string* StringDestruct  (string* str)
{
    free (str->pointer);
    free (str);

    return NULL;
}

    // Tree & Node
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
//----------------------


// Creation of Node
Node* CreateNode (int type, ... )
{
    Node* node = NodeConstruct (node);
    
    va_list ap;
    va_start (ap, type);
    
    if (type == TYPE_FUNC || type == TYPE_VAR)
    {
        *node = {type, NULL,  va_arg (ap, char*), NULL, va_arg (ap, Node*), va_arg (ap, Node*)};
    }
    else if (type == TYPE_NUM || type == TYPE_OP)
    {
        *node = {type, va_arg (ap, double), NULL, NULL, va_arg (ap, Node*), va_arg (ap, Node*)};
    }
    else if (type == TYPE_CONST)
    {
        *node = {type, va_arg (ap, double), va_arg (ap, char*), NULL, va_arg (ap, Node*), va_arg (ap, Node*)};
    }
    
    va_end (ap);
    
    
    return node;
}
//----------------------
