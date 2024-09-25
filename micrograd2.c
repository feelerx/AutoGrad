#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100
#define MAX_SIZE 100

struct value {
    double data;
    double grad;
    char op;
    struct value* next_value;
    struct value* prev_value1;
    struct value* prev_value2;
};

// Stack structure for integers used for order of precedence
typedef struct {
    struct value* items[MAX_SIZE];
    int top;
} nums;

typedef struct {
    int items[MAX_SIZE];
    int top;
} ops;
char problem[MAX_LEN];

struct value* create (double);
struct value* visited[MAX_SIZE];

// Function prototypes for operator precedence calculation
// Probably soo unnecessary
void pushop(ops *s, int value);
int popop(ops *s);
int isEmptyop(ops *s);
void push(nums *s, struct value* value);
struct value* pop(nums *s);
int isEmpty(nums *s);
struct value* applyOp(struct value* a, struct value* b, char op);
struct value* evaluateExpression(char *expr, struct value** nodes, char *operators, int *range);

void traverseGraph(struct value*);
void visitAllNodes(struct value*, int* );


int main (int argc, char **argv) {

    //get input from user and store the whole thing as one
    printf("\nenter the expression {without spaces}\nand in brackets examples below\n***ex1: (1+1)\n***ex2: ((6*2)+3)\n***ex3: ((31+(33+13)+(153+6))*3+(3*8))\n\nENTER EXPRESSION:");
    scanf("%s", problem);

    //calculate len of user input, values and operations to be performed
    char k='a';
    int length_of_problem=0;
    int length_of_values=0;
    int length_of_ops=0;
    for (int i=0; k!='\0'; i++) {
        k = problem[i];
        length_of_problem++;
        if (problem[i] == '+' || problem[i] == '*' || problem[i] == '-' || problem[i] == '/') length_of_ops++;
        else if (problem[i]== '(' || problem[i]==')') {}
        else length_of_values++;
    }

    //because of string termination at the end of each string in C
    length_of_problem-=1;
    length_of_values-=1;
    //making an integer to calculate how many string values there are in an integer
    //making an accompanying array to know the count value of individual integer values
    int string_to_int[length_of_ops+1];
    int some_count=0;
    int another_count=0;


    //storing the values of operations and lists in their respective arrays
    //just realized that I am storing individual integers as char. fck
    char value_list[length_of_values];
    char ops_list[length_of_ops];
    int temp_value_list = 0;
    int temp_op_list =0; 
    for (int i=0; i<length_of_problem; i++) {
        if (problem[i] == '+' || problem[i] == '*' || problem[i] == '-' || problem[i] == '/') {
            ops_list[temp_op_list] = problem[i];
            temp_op_list++;
            if (some_count!=0) {
                string_to_int[another_count]=some_count;
                another_count++;
                some_count=0;
            }

        }
        else if(problem[i]== '(' || problem[i]==')') {
            if (some_count!=0) {
                string_to_int[another_count]=some_count;
                another_count++;
                some_count=0;
            }
        }
        else {
            value_list[temp_value_list] = problem[i];
            temp_value_list++;
            some_count++;
        }

    }

    //create another array to store the actual values
    //basically converting the values we will need from str to int and saving to actual_value_list array
    int actual_value_list[length_of_ops+1];
    int start_index = 0;
    for (int i=0; i<length_of_ops+1; i++) {
        int some_length = string_to_int[i];
        char substring[some_length];
        strncpy(substring, &value_list[start_index], string_to_int[i]);
        substring[string_to_int[i]] = '\0';
        actual_value_list[i] = atoi(substring);
        start_index += string_to_int[i];
    }

    //we now have our values independently as 1. actual_value_list 2. ops_list
    //we can now start assigning individual values as nodes
    struct value* nodes[length_of_ops+1];
    for (int i = 0; i < length_of_ops + 1; i++) {
        nodes[i] = create(actual_value_list[i]);
    }
    printf("expression:");
    for (int i=0; i<length_of_problem; i++) printf("%c", problem[i]);
    printf("\nvalues:{");
    for (int i=0; i<length_of_ops+1; i++) printf("%d ", actual_value_list[i]); 
    printf("}\noperations:{");
    for (int i=0; i<length_of_ops; i++) printf("%c", ops_list[i]);
    printf("}\nvalue lenghth:{");
    for (int i=0; i<length_of_ops+1; i++) printf("%d ", string_to_int[i]); 
    printf("}\n");    
    //now, we try evaluating the expression correctly
    struct value* result = evaluateExpression(problem, nodes, ops_list, string_to_int);
    //int result_2 = evaluateExpression(expr, values, operators);
    double result2 = result->data;
    printf("Result: %f\n", result2);
    printf("\n\n");

    // for (int i = 0; i<length_of_ops+1; i++) {
    //     printf("leafnodeaddress:%p\nleafnodevalue:%f\naddressofnextnode:%p\nvalueofgrad:%f\nvalueofoperation:%c\nvalueofprevnode1:%p\nvalueofprevnode2:%p\n", nodes[i], nodes[i]->data, nodes[i]->next_value, nodes[i]->grad, nodes[i]->op, nodes[i]->prev_value1, nodes[i]->prev_value2);
    //     printf("\n");
    // }

    //int i=0;
    // while (nodes[i]!=NULL) {
    //     printf("address:%p\nvalue:%f\ngrad:%f\nnext:%p\nprev1:%p\nprev2:%p\n", nodes[i], nodes[i]->data, nodes[i]->grad, nodes[i]->next_value, nodes[i]->prev_value1, nodes[i]->prev_value2);
    //     printf("\n");
    //     nodes[i] = nodes[i]->next_value;
    //     //i++;
    // }

    traverseGraph(result);
    
    return 0;
}

struct value* create(double data_i) {
    struct value* node = (struct value*) malloc(sizeof(struct value));
    if (node == NULL) {
        // Handle memory allocation failure
        return NULL;
    }
    node->data = data_i;
    node->next_value = NULL;
    node->prev_value1 = NULL;
    node->prev_value2 = NULL;
    node->grad = 0;
    node->op = '\0';
    //printf("%f\n", node->data);
    return node;
}

//functions for order of precedence
void push(nums *s, struct value* value) {
    s->items[++s->top] = value;
}

struct value* pop(nums *s) {
    return s->items[s->top--];
}

int isEmpty(nums *s) {
    return s->top == -1;
}

void pushop(ops *s, int value) {
    s->items[++s->top] = value;
}

int popop(ops *s) {
    return s->items[s->top--];
}

int isEmptyop(ops *s) {
    return s->top == -1;
}
//This part is where the actual operation is carried out
struct value* applyOp(struct value* a, struct value* b, char op) {
    if(op=='+') {
        struct value* node = create(a->data + b->data);
        node->prev_value1 = a;
        node->prev_value2 = b;
        a->next_value = node;
        b->next_value = node;
        node->op = op;
        return node;
    }
    else if(op=='-') {
        struct value* node = create(a->data - b->data);
        node->prev_value1 = a;
        node->prev_value2 = b;
        a->next_value = node;
        b->next_value = node;
        node->grad = 0;
        node->op = op; 
        return node;
    }
    else if(op=='*') {
        struct value* node = create(a->data * b->data);
        node->prev_value1 = a;
        node->prev_value2 = b;
        a->next_value = node;
        b->next_value = node;
        node->grad = 0;
        node->op = op;
        return node;
    }
    else if(op=='/') {
        struct value* node = create(a->data / b->data);
        node->prev_value1 = a;
        node->prev_value2 = b;
        a->next_value = node;
        b->next_value = node;
        node->grad = 0;
        node->op = op;
        return node;
    }
    return NULL;
}

struct value* evaluateExpression(char *expr, struct value** nodes, char *operators, int *range) {
    nums values = {.top = -1};
    ops operations = {.top = -1};
    int val_index = 0;
    int op_index = 0;
    
    for (int i = 0; expr[i]; i++) {
        if (expr[i] == ' ') continue;
        if (expr[i] == '(') {
            pushop(&operations, -1);  // Use -1 to represent '('
        }
        else if (isdigit(expr[i])) {
            push(&values, nodes[val_index]);
            i += range[val_index] - 1;
            val_index++;
        }
        else if (expr[i] == ')') {
            while (!isEmptyop(&operations) && operations.items[operations.top] != -1) {
                struct value* b = pop(&values);
                struct value* a = pop(&values);
                int op = popop(&operations);
                push(&values, applyOp(a, b, operators[op]));
            }
            if (!isEmptyop(&operations) && operations.items[operations.top] == -1) {
                popop(&operations);  // Pop '('
            }
        }
        else {  // Operator
            while (!isEmptyop(&operations) && operations.items[operations.top] != -1) {
                struct value* b = pop(&values);
                struct value* a = pop(&values);
                int op = popop(&operations);
                push(&values, applyOp(a, b, operators[op]));
            }
            pushop(&operations, op_index++);
        }
    }
    
    while (!isEmptyop(&operations)) {
        struct value* b = pop(&values);
        struct value* a = pop(&values);
        int op = popop(&operations);
        push(&values, applyOp(a, b, operators[op]));
    }
    
    return pop(&values);
}

void visitAllNodes(struct value* node, int* visited_count) {
    if (node == NULL || node->data == 0) {
        return;
    }

    // Check if we've already visited this node
    for (int i = 0; i < *visited_count; i++) {
        if (visited[i] == node) {
            return;
        }
    }

    // Mark this node as visited
    if (node->op!='\0') {
        struct value* temp1 = node->prev_value1;
        struct value* temp2 = node->prev_value2;
        if (node->op=='+') {
            temp1->grad = node->grad;
            temp2->grad = node->grad;
        }
        else if (node->op=='*') {
            temp1->grad = temp2->data;
            temp2->grad = temp1->data;
        }
        else if (node->op=='-') {
            temp1->grad = node->grad;
            temp2->grad = -1*(node->grad);
        }
        else if (node->op=='/') {
            temp1->grad = 1/(temp2->data);
            temp2->grad = temp1->data;
        }


    }
    visited[*visited_count] = node;
    (*visited_count)++;

    //Print node information
    printf("Node Address: %p\n", (void*)node);
    printf("Value: %f\n", node->data);
    printf("Grad: %f\n", node->grad);
    printf("Operation: %c\n", node->op);
    printf("\n");

    // Visit previous nodes
    visitAllNodes(node->prev_value1, visited_count);
    visitAllNodes(node->prev_value2, visited_count);
}

// Function to initiate the traversal
void traverseGraph(struct value* result) {
    int visited_count = 0;
    result->grad=1.0;
    visitAllNodes(result, &visited_count);
}
