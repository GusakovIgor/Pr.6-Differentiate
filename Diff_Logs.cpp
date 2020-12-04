#include "Diff.h"

void SyntaxRequire (string* str, char request)
{
    if (str->pointer[str->offset] != request)
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
                                    CalculatorLogs (str, logs, CURRENT);
                                    break;
        
        case (WRONG_OPERATION):     fprintf  (logs, "\nThere is wrong operation in string (before pointer):\n");
                                    CalculatorLogs (str, logs, LEFT);
                                    break;
    }
        
    fclose (logs);
    
    str->fuckedup = true;
}


void CalculatorLogs (string* str, FILE* logs, int mode)
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

void DiffTreeLogs (Node* root)
{
    FILE* GraphicLogs = fopen ("Logs\\DiffGraphicLogs.txt", "w");
    static int call = 0;
    
    fprintf (GraphicLogs, "digraph %d {\n", call);
    fprintf (GraphicLogs, "node [shape = \"record\", style = \"filled\", color = \"#000800\", fillcolor = \" #ED96EC\"]\n");
    
    if (root)
    {
        fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", label = \"{%lg}\"]\n", 
                                                                            root, root->value);
        if (root->left)
        {
            PrintSubTree (root->left, GraphicLogs);
            fprintf (GraphicLogs, "\"%p\" -> \"%p\"\n", root, root->left);
        }
        if (root->right)
        {
            PrintSubTree (root->right, GraphicLogs);
            fprintf (GraphicLogs, "\"%p\" -> \"%p\"\n", root, root->right);
        }
    }
    else
    {
        fprintf (GraphicLogs, "\"%d\" [color = \"#000800\", fillcolor = blue, label = \"{NULL}\"]\n", 0);
    }
    
    fprintf (GraphicLogs, "}\n");
    
    fclose (GraphicLogs);
    
    
    char* command_1 = (char*) calloc (100, sizeof(char));
    sprintf (command_1, "dot -Tjpeg Logs\\DiffGraphicLogs.txt > Logs\\DiffGraphicLogs_%d.jpeg", call);
    char* command_2 = (char*) calloc (100, sizeof(char));
    sprintf (command_2, "start Logs\\DiffGraphicLogs_%d.jpeg", call);
    
    system (command_1);
    system (command_2);
    
    free (command_1);
    free (command_2);
    
    call++;
}

void PrintSubTree (Node* root, FILE* GraphicLogs)
{   
    static int check = -1;
    
    if (root->left || root->right)
        fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", label = \"{%lg}\"]\n", 
                                                                                root, root->value);
    else
        fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = turquoise, label = \"{%lg}\"]\n", 
                                                                                root, root->value);
    
    fprintf (GraphicLogs, "\"%p\" -> \"%p\"\n [color = white]", root, root->parent);
    
    if (root->left)
    {
        PrintSubTree (root->left, GraphicLogs);
        fprintf (GraphicLogs, "\"%p\" -> \"%p\"\n", root, root->left);
    }
    else
    {
        fprintf (GraphicLogs, "\"%d\" [color = \"#000800\", fillcolor = blue, label = \"{NULL}\"]\n", check);
        fprintf (GraphicLogs, "\"%p\" -> \"%d\"\n", root, check);
        check--;
    }
    
    if (root->right)
    {
        PrintSubTree (root->right, GraphicLogs);
        fprintf (GraphicLogs, "\"%p\" -> \"%p\"\n", root, root->right);
    }
    else
    {
        fprintf (GraphicLogs, "\"%d\" [color = \"#000800\", fillcolor = blue, label = \"{NULL}\"]\n", check);
        fprintf (GraphicLogs, "\"%p\" -> \"%d\"\n", root, check);
        check--;
    }
}

