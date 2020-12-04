#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int error_t;

const int MAX_STRING_LEN = 100;


struct string
{
    bool fuckedup;
    
    char* pointer;
    int   offset;  
};

struct Node
{
    int type;
    double value;
    
    Node* parent;
    Node* left;
    Node* right;
};

struct Tree
{
    Node* root;
};

// Enums
enum diff_errors { WRONG_OPERATION = 1,
                   WRONG_SYMBOL    = 2
};

enum diff_debug_modes { LEFT    = 1,
                        CURRENT = 2,
                        RIGHT   = 3
};

enum diff_node_types { TYPE_OP  = 1,
                       TYPE_VAR = 2,
                       TYPE_NUM = 3
};
//----------------------



// Construct-Destruct
string* StringConstruct (string* str);
string* StringDestruct  (string* str);

Tree* TreeConstruct (Tree* tree);
Tree* TreeDestruct  (Tree* tree);

Node* NodeConstruct (Node* node);
Node* NodeDestruct  (Node* node);

Node* CreateNode    (int type, double value, Node* parent, Node* left, Node* right);
//----------------------


// Parser
double Calculator ();
//----------------------


// Rules for calculator
double GetG (string* str);
double GetE (string* str);
double GetT (string* str);
double GetD (string* str);

double GetP  (string* str);
double GetTR (string* str);
double GetVAR (string* str);
double GetN  (string* str);

double GetFunctionResult (int name, double arg);

void SkipSpaces (string* str);
//----------------------


// Logs
void SyntaxRequire  (string* str, char request);
void ErrorReport    (string* str, error_t err_code);
void CalculatorLogs (string* str, FILE* logs, int mode);

void DiffTreeLogs (Node* root);
void PrintSubTree (Node* root, FILE* GraphicLogs);
//----------------------
