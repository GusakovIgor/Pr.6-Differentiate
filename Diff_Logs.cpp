#include "Diff.h"

// Errors Processing
void SyntaxRequire (string* str, char request)
{
    if (str->pointer[str->offset++] != request)
        ErrorReport (str, WRONG_SYMBOL);
}

void ErrorReport (string* str, error_t err_code)
{
    FILE* logs = NULL;
    
    #ifdef SUPERDEBUG
        logs = stderr;
    #else
        logs = fopen ("TextLogs_Diff.txt", "w");
    #endif
    
    switch (err_code)
    {
        case (WRONG_SYMBOL):        fprintf  (logs, "\nThere is wrong symbol in string (above pointer):\n");
                                    ParserLogs (str, logs, CURRENT);
                                    break;
        
        case (WRONG_OPERATION):     fprintf  (logs, "\nThere is wrong operation in string (before pointer):\n");
                                    ParserLogs (str, logs, LEFT);
                                    break;
    }
        
    fclose (logs);
    
    str->fuckedup = true;
}

void ParserLogs  (string* str, FILE* logs, int mode)
{
    fprintf (logs, "%s\n", str->pointer);
    for (int i = 0; i < str->offset; i++)
        fprintf (logs, " ");
    
    switch (mode)
    {
        case (LEFT):    fprintf (logs, "<-\n\n");
                        break;
        
        case (CURRENT): fprintf (logs, "^ \n\n");
                        break;
    }
}
//----------------------



// Graphic Logs
void DiffTreeLogs (Node* root)
{
    FILE* GraphicLogs = fopen ("Logs\\DiffGraphicLogs.txt", "w");
    static int call = 0;
    
    fprintf (GraphicLogs, "digraph %d {\n", call);
    fprintf (GraphicLogs, "node [shape = \"record\", style = \"filled\", color = \"#000800\", fillcolor = \" #ED96EC\"]\n");
    
    if (root)
    {
        PrintNodeLogs (root, GraphicLogs);
        
        if (root->left)
        {
            PrintSubTree (root->left, GraphicLogs);
            PrintArrow   (root, root->left, GraphicLogs);
        }

        if (root->right)
        {
            PrintSubTree (root->right, GraphicLogs);
            PrintArrow   (root, root->right, GraphicLogs);
        }
    }
    
    
    fprintf (GraphicLogs, "}\n");
    
    fclose (GraphicLogs);
    
    MakeOpenPhoto (call);
    
    call++;
}

void PrintSubTree (Node* root, FILE* GraphicLogs)
{
    if (root)
    {
        PrintNodeLogs (root, GraphicLogs);
        
        if (root->parent)
            PrintArrow (root, root->parent, GraphicLogs);
        
        if (root->left)
        {
            PrintSubTree (root->left, GraphicLogs);
            PrintArrow   (root, root->left, GraphicLogs);
        }
        
        if (root->right)
        {
            PrintSubTree (root->right, GraphicLogs);
            PrintArrow   (root, root->right, GraphicLogs); 
        }
    }
}

void PrintNodeLogs  (Node* node, FILE* GraphicLogs)
{
    if (node)
    {
        switch (node->type)
        {
            case (TYPE_CONST):  fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = turquoise,    label = \"{%s}\"]\n", 
                                                                                    node, node->string);
                                break;
                       
            case (TYPE_NUM):    fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = turquoise,    label = \"{%lg}\"]\n", 
                                                                                    node, node->value);
                                break;
                                
            case (TYPE_OP):     fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = \" #ed96ec\", label = \"{%c}\"]\n",
                                                                                    node, FindOperation (node));
                                break;
            
            case (TYPE_VAR):    fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = turquoise,    label = \"{%s}\"]\n",
                                                                                    node, node->string);
                                break;
            
            case (TYPE_FUNC):   fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = \"#8264ff\",  label = \"{%s}\"]\n",
                                                                                    node, node->string);
                                break;
        }
    }
    else
    {
        static int num_of_nulls = 1;
        fprintf (GraphicLogs, "\"%d\" [color = \"#000800\", fillcolor = blueviolet, label = \"{NULL}\"]\n", 
                                                                                0 - num_of_nulls);
        num_of_nulls++;
    }
}

void PrintArrow (Node* beginning, Node* ending, FILE* GraphicLogs)
{
    if (ending)
    {
        if (beginning->parent == ending)
            fprintf (GraphicLogs, "\"%p\" -> \"%p\" [color = black]\n", beginning, ending);
        else
            fprintf (GraphicLogs, "\"%p\" -> \"%p\"\n", beginning, ending);
    }
    else
    {
        static int num_of_nulls = 1;
        fprintf (GraphicLogs, "\"%p\" -> \"%d\"\n", beginning, 0 - num_of_nulls);
        
        num_of_nulls++;
    }
}

void MakeOpenPhoto (int call)
{
    char* command_1 = (char*) calloc (MAX_COMMAND_LEN, sizeof(char));
    char* command_2 = (char*) calloc (MAX_COMMAND_LEN, sizeof(char));
    
    sprintf (command_1, "dot -Tjpeg Logs\\DiffGraphicLogs.txt > Logs\\DiffGraphicLogs_%d.png", call);
    sprintf (command_2, "start Logs\\DiffGraphicLogs_%d.png", call);
    
    system (command_1);
    system (command_2);
    
    free (command_1);
    free (command_2);
}

char FindOperation (Node* node)
{
    int operation = (int) node->value;
    
    switch (operation)
    {
        case (OP_ADD):      return '+';
                            break;
        
        case (OP_SUB):      return '-';
                            break;
        
        case (OP_MUL):      return '*';
                            break;
        
        case (OP_DIV):      return '/';
                            break;
        
        case (OP_DEG):      return '^';
                            break;
    }
}
//----------------------
