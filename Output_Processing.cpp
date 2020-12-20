#include "Diff.h"

void FormulaPrint (Tree* tree)
{
    static int call = 0;
    FILE* LaTeX = fopen ("LaTeX\\LaTeX_expression.latex", "w");
    
    PrintHeader (LaTeX);
    
    fprintf (LaTeX, "\\begin{document}\n");
    fprintf (LaTeX, "\\[ ");
    PrintNode (tree->root, LaTeX);
    fprintf (LaTeX, " \\]\n");
    fprintf (LaTeX, "\\end{document}");
    
    fclose (LaTeX);
    
    CompileLaTeX (call);
    
    call++;
}

void CompileLaTeX (int call)
{
    char* command_1 = (char*) calloc (MAX_COMMAND_LEN, sizeof (char));
    char* command_2 = (char*) calloc (MAX_COMMAND_LEN, sizeof (char));
    
    sprintf (command_1, "pdflatex --output-directory=./LaTeX --output-format=pdf --jobname LaTeX_expression_%d ./LaTeX/LaTeX_expression.latex", call);
    sprintf (command_2, "start LaTeX\\LaTeX_expression_%d.pdf", call);
    
    system (command_1);
    system (command_2);
    
    free (command_1);
    free (command_2);
}

void PrintHeader (FILE* LaTeX)
{
    fprintf (LaTeX, "\\documentclass[a4paper,12pt]{article}\n");
    fprintf (LaTeX, "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}\n");
}

void PrintNode (Node* node, FILE* LaTeX)
{
    switch (node->type)
    {
        case (TYPE_CONST):  fprintf (LaTeX, "%s" , node->string);
                            break;
        
        case (TYPE_NUM):    fprintf (LaTeX, "%lg", node->value);
                            break;
                            
        case (TYPE_OP):     PrintOperation (node, LaTeX);
                            break;
        
        case (TYPE_VAR):    fprintf (LaTeX, "%s",  node->string);
                            break;
        
        case (TYPE_FUNC):   fprintf (LaTeX, "\\%s", node->string);
                            if (node->parent->type == TYPE_OP && node->parent->value == OP_DEG)
                            {
                                PrintSign (OP_DEG, LaTeX);
                                PrintSubNode (node->parent->right, LaTeX, OP_DEG);
                            }
                            PrintFuncArg (node, LaTeX);
                            break;
    }
}

void PrintOperation (Node* node, FILE* LaTeX)
{
    int operation = (int) node->value;
    
    if (operation == OP_DIV)
    {
        PrintSign (operation, LaTeX);
        PrintSubNode (node->left,  LaTeX, operation);
        PrintSubNode (node->right, LaTeX, operation);
    }
    else if (operation == OP_DEG && node->left->type == TYPE_FUNC)
    {
        PrintSubNode (node->left,  LaTeX, operation);
    }
    else
    {
        PrintSubNode (node->left,  LaTeX, operation);
        PrintSign (operation, LaTeX);
        PrintSubNode (node->right, LaTeX, operation);
    }
}

void PrintSubNode (Node* sub_node, FILE* LaTeX, int operation)
{
    fprintf (LaTeX, "{");
    if (sub_node->type == TYPE_OP && (operation - sub_node->value >= 2 || (operation == OP_SUB && sub_node == sub_node->parent->right && sub_node->value == OP_ADD)))
        fprintf (LaTeX, "\\left(");
    
    PrintNode (sub_node,  LaTeX);
    
    if (sub_node->type == TYPE_OP && (operation - sub_node->value >= 2 || (operation == OP_SUB && sub_node == sub_node->parent->right && sub_node->value == OP_ADD)))
        fprintf (LaTeX, "\\right)");
    fprintf (LaTeX, "}");
}

void PrintSign (int operation, FILE* LaTeX)
{
    switch (operation)
    {
        case (OP_ADD):      fprintf (LaTeX, " + ");
                            break;
        
        case (OP_SUB):      fprintf (LaTeX, " - ");
                            break;
        
        case (OP_MUL):      fprintf (LaTeX, " \\cdot ");
                            break;
        
        case (OP_DIV):      fprintf (LaTeX, " \\frac ");
                            break;
        
        case (OP_DEG):      fprintf (LaTeX, " ^ ");
                            break;
    }
}

void PrintFuncArg (Node* node, FILE* LaTeX)
{
    fprintf (LaTeX, "\\left(");
    PrintNode (node->right, LaTeX);
    fprintf (LaTeX, "\\right)");
}
