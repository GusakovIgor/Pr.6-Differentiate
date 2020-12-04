#include "Diff.h"

double Calculator ()
{
    string* str = StringConstruct (str);
    
    gets (str->pointer);
    
    double value = GetG (str);
    
    if (str->fuckedup)
    {
        str = StringDestruct (str);
        assert (!"OK");
    }
    
    return value;
}

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

double GetG (string* str)
{
    double value = GetE (str);
    if (str->fuckedup)
        return NULL;
    
    SyntaxRequire (str, '\0');
    
    return value;
}

double GetE (string* str)
{
    double value = GetT (str);
    
    while (str->pointer[str->offset] == '+' || str->pointer[str->offset] == '-')
    {
        int sign = str->pointer[str->offset];
        str->offset++;
        
        double temp = GetT (str);
        if (str->fuckedup)
            return NULL;
        
        if (sign == '+')
            value += temp;
        else
            value -= temp;
    }
    
    return value;
}

double GetT (string* str)
{
    double value = GetD (str);
    
    while (str->pointer[str->offset] == '*' || str->pointer[str->offset] == '/')
    {
        int sign = str->pointer[str->offset];
        str->offset++;
        
        double temp = GetD (str);
        
        if (str->fuckedup)
            return NULL;
        
        if (sign == '*')
            value *= temp;
        else if (temp != 0.0)
            value /= temp;
        else
            ErrorReport (str, WRONG_OPERATION);
    }
    
    return value;
}

double GetD (string* str)
{
    double value = GetP (str);
    
    while (str->pointer[str->offset] == '^')
    {
        str->offset++;
        double temp = GetP (str);
        
        value = pow (value, temp);
    }
    
    if (str->fuckedup)
        return NULL;
    else
        return value;
}

double GetP (string* str)
{
    SkipSpaces (str);       // Пробелы перед primary
    
    double value = 0;
    if (str->pointer[str->offset] == '(')
    {
        str->offset++;
        value = GetE (str);
        SyntaxRequire (str, ')');
        str->offset++;
    }
    else if ('A' < str->pointer[str->offset] && str->pointer[str->offset] < 'z')
    {
        value = GetVAR_OR_FUNC (str);
    }
    else
        value = GetN  (str);
    
    SkipSpaces (str);       // Пробелы после primary
    
    if (str->fuckedup)
        return NULL;
    else
        return value;
}

double GetVAR_OR_FUNC (string* str)
{
    double value = GetVAR (str);
        
    if (str->pointer[str->offset] == '(')
    {
        str->offset++;
        double name = value;
        double arg  = GetE (str);
        SyntaxRequire (str, ')');
        str->offset++;
        
        value = GetFunctionResult (name, arg);
    }
    
    if (str->fuckedup)
        return NULL;
    else
        return value;
}

double GetVAR (string* str)
{
    double value = str->pointer[str->offset];
    str->offset++;
    
    return value;
}

double GetN (string* str)
{    
    double value = 0;
    char*  end;
    value = strtod (str->pointer + str->offset, &end);
    
    if (str->offset == end - str->pointer)
        ErrorReport (str, WRONG_SYMBOL);
    else
        str->offset = end - str->pointer;
    
    if (str->fuckedup)
        return NULL;
    else
        return value;
}


double GetFunctionResult (int name, double arg)
{
    double value = 0;
    
    switch (name)
    {
        case ('s'):     value = sin (arg);
                        break;
        
        case ('c'):     value = cos (arg);
                        break;
    }
    
    return value;
}

void SkipSpaces (string* str)
{
    while (str->pointer[str->offset] == ' ')
        str->offset++;
}
