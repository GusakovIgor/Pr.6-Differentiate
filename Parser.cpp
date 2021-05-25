#include "Diff.h"


Node* Parser ()
{
    string* str = StringConstruct (str);

    scanf ("%s", str->pointer);

    Node* value = GetG (str);

    bool error = str->fuckedup;
    str = StringDestruct (str);
    
    if (error)
        assert (!"OK");

    return value;
}


// Rules for Parser
Node* GetG (string* str)
{
    Node* value = GetE (str);
    if (str->fuckedup)
        return NULL;
    
    SyntaxRequire (str, '\0');

    return value;
}

Node* GetE (string* str)
{
    Node* value = GetT (str);

    while (str->pointer[str->offset] == '+' || str->pointer[str->offset] == '-')
    {
        int sign = str->pointer[str->offset];
        str->offset++;

        Node* temp = GetT (str);
        
        if (str->fuckedup)
            return NULL;
        
        if (sign == '+')
            value = CreateNode (TYPE_OP, (double) OP_ADD, value, temp);
        else
            value = CreateNode (TYPE_OP, (double) OP_SUB, value, temp);
        
        value->left->parent  = value;
        value->right->parent = value;

    }

    return value;
}

Node* GetT (string* str)
{
    Node* value = GetD (str);

    while (str->pointer[str->offset] == '*' || str->pointer[str->offset] == '/')
    {
        int sign = str->pointer[str->offset];
        str->offset++;

        Node* temp = GetD (str);

        if (str->fuckedup)
            return NULL;

        if (sign == '*')
            value = CreateNode (TYPE_OP, (double) OP_MUL, value, temp);
        else
            value = CreateNode (TYPE_OP, (double) OP_DIV, value, temp);

        value->left->parent  = value;
        value->right->parent = value;
    }

    return value;
}

Node* GetD (string* str)
{
    Node* value = GetP (str);

    while (str->pointer[str->offset] == '^')
    {
        str->offset++;
        Node* temp = GetP (str);
        
        value = CreateNode (TYPE_OP, (double) OP_DEG, value, temp);
        value->left->parent  = value;
        value->right->parent = value;
    }
    
    if (str->fuckedup)
        return NULL;
    else
        return value;
}

Node* GetP (string* str)
{
    SkipSpaces (str);

    Node* value = 0;
    if (str->pointer[str->offset] == '(')
    {
        str->offset++;
        value = GetE (str);
        SyntaxRequire (str, ')');
    }
    else if (isalpha(str->pointer[str->offset]))
    {
        value = GetVAR_OR_FUNC (str);
    }
    else
    {
        value = GetN  (str);
    }

    SkipSpaces (str);
    
    if (str->fuckedup)
        return NULL;
    else
        return value;
}

Node* GetVAR_OR_FUNC (string* str)
{
    Node* value = GetVAR (str);
    
    SkipSpaces (str);
    
    if (str->pointer[str->offset] == '(')
    {
        str->offset++;
        Node* arg  = GetE (str);
        SyntaxRequire (str, ')');
        
        if (str->fuckedup)
            return NULL;
        
        value->right = arg;
        value->right->parent = value;
    }

    if (str->fuckedup)
        return NULL;
    else
        return value;
}

Node* GetVAR (string* str)
{
    char* variable = (char*) calloc (MAX_STRING_LEN, sizeof (char));
    int i = 0;
    while (
            (i > 0 && isalnum (str->pointer[str->offset]))
                   || isalpha (str->pointer[str->offset])
                   || str->pointer[str->offset] == '_' 
          )
    {
        variable[i] = str->pointer[str->offset];
        str->offset++;
        i++;
    }
    
    double const_value = ConstantValue (variable);
    
    if (str->pointer[str->offset] == '(')
    {
        return CreateNode (TYPE_FUNC,  variable, NULL, NULL);
    }
    else if (!const_value)
    {
        return CreateNode (TYPE_VAR,   variable, NULL, NULL);
    }
    else
    {
        return CreateNode (TYPE_CONST, const_value, variable, NULL, NULL);
    }
}

Node* GetN (string* str)
{
    double number = 0;
    char*  end;
    number = strtod (str->pointer + str->offset, &end);

    if (str->offset == end - str->pointer)
        ErrorReport (str, WRONG_SYMBOL);
    else
        str->offset = end - str->pointer;

    if (str->fuckedup)
        return NULL;

    Node* value = CreateNode (TYPE_NUM, number, NULL, NULL);
    
    if (isalpha(str->pointer[str->offset]))
    {
        Node* temp = GetVAR (str);
        value = CreateNode (TYPE_OP, (double) OP_MUL, value, temp);
        value->left->parent  = value;
        value->right->parent = value;
    }
    
    return value;
}
//----------------------


// Help for parsing
void SkipSpaces (string* str)
{
    while (str->pointer[str->offset] == ' ')
        str->offset++;
}

double ConstantValue (char* name)
{
    if (strcmp (name, "e") == 0)
    {
        return M_E;
    }
    else if (strcmp (name, "pi") == 0)
    {
        return M_PI;
    }
    else
    {
        return NULL;
    }
}
//----------------------
