#include "Diff.h"

Node* SimplifyNode (Node* node)
{
    if (L)
    {
        L = SimplifyNode (L);
        L->parent = node;
    }
    if (R)
    {
        R = SimplifyNode (R);
        R->parent = node;
    }
    
    switch (node->type)
    {
        case (TYPE_CONST):  return node;
        
        case (TYPE_NUM):    return node;
        
        case (TYPE_VAR):    return node;
        
        case (TYPE_OP):     return SimplifyOperation (node);
        
        case (TYPE_FUNC):   return SimplifyFunction  (node);
    }
}

Node* SimplifyOperation (Node* node)
{
    int operation = (int) node->value;
    switch (operation)
    {
        case (OP_ADD):      return SimplifyAdd (node);
        
        case (OP_SUB):      return SimplifySub (node);
        
        case (OP_MUL):      return SimplifyMul (node);
        
        case (OP_DIV):      return SimplifyDiv (node);
        
        case (OP_DEG):      return SimplifyDeg (node);
    }
}


Node* SimplifyAdd (Node* node)
{    
    if (L->type == TYPE_NUM && R->type == TYPE_NUM)
    {
        return BothNumCase (node);
    }
    else if (L->type == TYPE_NUM && Lval == 0.0)
    {
        return AnotherBranch (L);
    }
    else if (R->type == TYPE_NUM && Rval == 0.0)
    {
        return AnotherBranch (R);
    }
    
    return node;
}

Node* SimplifySub (Node* node)
{    
    if (L->type == TYPE_NUM && R->type == TYPE_NUM)
    {
        return BothNumCase (node);
    }
    else if (L->type == TYPE_NUM && Lval == 0.0)
    {
        return SimplifyOperation (MUL (NUM(-1), AnotherBranch (L)));
    }
    else if (R->type == TYPE_NUM && Rval == 0.0)
    {
        return AnotherBranch (R);
    }
    
    return node;
}

Node* SimplifyMul (Node* node)
{
    if (L->type == TYPE_NUM && R->type == TYPE_NUM)
    {
        return BothNumCase (node);
    }
    else if (L->type == TYPE_NUM)
    {
        if (Lval == 1.0 || Lval == -1.0)
        {
            return AnotherBranch (L);
        }
        else if (Lval == 0.0)
        {
            return ZeroCase (node);
        }
    }
    else if (R->type == TYPE_NUM)
    {
        if (Rval == 1.0 || Rval == -1.0)
        {
            return AnotherBranch (R);
        }
        else if (Rval == 0.0)
        {
            return ZeroCase (node);
        }
    }
    
    return node;
}

Node* SimplifyDiv (Node* node)
{   
    if (L->type == TYPE_NUM && R->type == TYPE_NUM)
    {
        return ReduceFraction (node);
    }
    else if (L->type == TYPE_NUM)
    {
        if (Lval == 0.0)
        {
            return ZeroCase (node);
        }
    }
    else if (R->type == TYPE_NUM && Rval == 1.0)
    {
        return AnotherBranch (R);
    }
    
    return node;
}

Node* SimplifyDeg (Node* node)
{
    if (L->type == TYPE_NUM && R->type == TYPE_NUM)
    {
        return BothNumCase (node);
    }
    else if (L->type == TYPE_NUM)
    {   
        if (Lval == 1.0)
        {
            node = NodeDestruct (node);
            return NUM (1.0);
        }
        else if (Lval == 0.0)
        {
            return ZeroCase (node);
        }
    }
    else if (R->type == TYPE_NUM)
    {
        if (Rval == 1.0)
        {
            return AnotherBranch (R);
        }
        else if (Rval == 0.0)
        {
            node = NodeDestruct (node);
            return NUM (1.0);
        }
    }
    
    return node;
}

// Help Functions

Node* AnotherBranch (Node* side)
{
    Node* node = side->parent;
    
    if (side == R)
    {
        if (side->value < 0 && L->type == TYPE_NUM)
            Lval *= -1;
        
        return MakeNodeRootless (L);
    }
    else
    {
        if (side->value < 0 && R->type == TYPE_NUM)
            Rval *= -1;
        
        return MakeNodeRootless (R);
    }
}

Node* MakeNodeRootless (Node* side)
{
    Node* temp = side;
    
    if (side == side->parent->left)
    {
        side->parent->left  = NULL;
    }
    else
    {
        side->parent->right = NULL;
    }
    
    side->parent = NodeDestruct (side->parent);
    
    return temp;
}

Node* ZeroCase         (Node* node)
{
    node = NodeDestruct (node);
    return NUM(0.0);
}

Node* BothNumCase      (Node* node)
{
    double value = OperationResult (node);
    
    node = NodeDestruct (node);
    
    return NUM (value);
}

double OperationResult (Node* node)
{
    int operation = (int) node->value;
    switch (operation)
    {
        case (OP_ADD):  return (Lval + Rval);
        
        case (OP_SUB):  return (Lval - Rval);
        
        case (OP_MUL):  return (Lval * Rval);
        
        case (OP_DIV):  return (Lval / Rval);
        
        case (OP_DEG):  return (pow (Lval, Rval));
        
    }
}

Node* ReduceFraction   (Node* node)
{
    int l_value = (int) Lval;
    int r_value = (int) Rval;
    
    if (l_value % r_value == 0 || r_value % l_value == 0)
    {
        node = NodeDestruct (node);
        return NUM(1.0);
    }
    else
    {
        int gcd = FindGCD ( l_value, r_value);
        Lval /= gcd;
        Rval /= gcd;
        
        return node;
    }
}

int FindGCD (int num_1, int num_2)
{
    int rem = num_1 % num_2;
    while (rem)
    {
        num_1 = num_2;
        num_2 = rem;
        rem = num_1 % num_2;    
    }
    
    return num_2;
}
//----------------------


// Functions
Node* SimplifyFunction (Node* node)
{
    R = SimplifyNode(R);
    
    char* function = node->string;
    if (strcmp(function, "ln") == 0)
    {
        node = SimplifyLn (node);
    }

    return node;
}

Node* SimplifyLn (Node* node)
{
    if (R->type == TYPE_CONST && Rval == M_E)
    {
        node = NodeDestruct (node);
        return NUM(1.0);
    }
    
    return node;
}
//----------------------




