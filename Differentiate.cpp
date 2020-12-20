#include "Diff.h"

int main ()
{
    Tree* diff_tree = TreeConstruct (diff_tree);
    Tree* tree      = TreeConstruct (tree);
    
    tree->root = Parser ();
    
    DiffTreeLogs (tree->root);
    FormulaPrint (tree);
    
    diff_tree->root = Differentiate (tree->root);
    MakeParents  (diff_tree->root);
    diff_tree->root = SimplifyNode  (diff_tree->root);
    
    DiffTreeLogs (diff_tree->root);
    FormulaPrint (diff_tree);
    
    tree      = TreeDestruct (tree);
    diff_tree = TreeDestruct (diff_tree);
    
    return 0;
}


// Differentiate
Node* Differentiate (Node* node)
{
    switch (node->type)
    {
        case (TYPE_CONST):  return CreateNode (TYPE_NUM, 0, NULL, NULL);
        
        case (TYPE_NUM):    return CreateNode (TYPE_NUM, 0, NULL, NULL);
        
        case (TYPE_VAR):    return CreateNode (TYPE_NUM, (strcmp(node->string, "x") == 0) ? 1.0 : 0.0, NULL, NULL);
        
        case (TYPE_OP):     return OperationDiff (node);
        
        case (TYPE_FUNC):   return FunctionDiff  (node);
    }
}

Node* OperationDiff (Node* node)
{
    int operation = (int) node->value;
    
    switch (operation)
    {
        case (OP_ADD):      return ADD(d(L), d(R));
        
        case (OP_SUB):      return SUB(d(L), d(R));
        
        case (OP_MUL):      return ADD(MUL(d(L), c(R)), MUL(c(L), d(R)));
        
        case (OP_DIV):      return DIV(SUB(MUL(d(L), c(R)), MUL(c(L), d(R))), MUL(c(R), c(R)));
        
        case (OP_DEG):      return DegreeDiff (node);
    }
}

Node* DegreeDiff (Node* node)
{
    if (IsNumber(R))
    {
        return MUL (d(L), MUL(c(R), DEG (c(L), SUB (c(R), NUM(1.0)))));
    }
    else if (IsNumber(L))
    {
        return MUL (LN(c(L)), MUL (c(node), d(c(R))));
    }
    else
    {
        return d (DEG (EUL, MUL (LN(c(L)), c(R))));
    }
}

Node* FunctionDiff (Node* node)
{
    char* function = node->string;
    
    if (strcmp (function, "ln") == 0)
    {
        return MUL (d(R), DIV (NUM(1.0), c(R)));
    }
    else if (strcmp (function, "sin") == 0)
    {
        return MUL (d(R), COS(c(R)));
    }
    else if (strcmp (function, "cos") == 0)
    {
        return MUL (NUM(-1.0), MUL (d(R), SIN(c(R))));
    }
}
//----------------------


// Help for differentiate
Node* d (Node* node)
{
    return Differentiate (node);
}

Node* c (Node* node)
{
    if (!node)
        return NULL;
    
    if (node->type == TYPE_FUNC || node->type == TYPE_VAR)
    {
        return CreateNode (node->type, node->string, c(node->left), c(node->right));
    }
    else if (node->type == TYPE_NUM || node->type == TYPE_OP)
    {
        return CreateNode (node->type, node->value,  c(node->left), c(node->right));
    }
    else if (node->type == TYPE_CONST)
    {
        return CreateNode (node->type, node->value, node->string, c(node->left), c(node->right));
    }
}

bool IsNumber (Node* node)
{   
    switch (node->type)
    {
        case (TYPE_NUM):    return true;
        
        case (TYPE_CONST):  return true;
    
        case (TYPE_OP):     return (IsNumber (node->left) && IsNumber (node->right));
        
        case (TYPE_VAR):    return (strcmp(node->string, "x") == 0) ? false : true;
        
        case (TYPE_FUNC):   return IsNumber (node->right);
    }
}

void MakeParents (Node* root)
{
    if (root->left)
    {
        root->left->parent = root;
        MakeParents (root->left);
    }
    
    if (root->right)
    {
        root->right->parent = root;
        MakeParents (root->right);
    }
}
//----------------------
