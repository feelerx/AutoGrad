# AutoGrad

A simple automatic differentiation (autograd) program for mathematical expressions implemented in C. AutoGrad enables automatic gradient computation through computational graphs, parsing mathematical expressions and computing derivatives via backpropagation.

## Overview

AutoGrad provides automatic gradient computation capabilities in C by building a computational graph from mathematical expressions and automatically computing gradients through backpropagation. The implementation uses a node-based structure where each value tracks its dependencies and gradients.

## Features

- Automatic gradient computation for mathematical expressions
- Expression parsing with proper operator precedence
- Computational graph construction with node tracking
- Backpropagation through the entire computation graph
- Support for basic arithmetic operations
- Bracketed expression evaluation

## Supported Operations

The library supports the following mathematical operations:
- Addition (`+`)
- Subtraction (`-`)
- Multiplication (`*`)
- Division (`/`)

All operations properly track gradients and support arbitrary nesting through parentheses.

## Requirements

- C compiler (gcc, clang, or similar)
- Standard C libraries (stdio.h, stdlib.h, string.h, ctype.h)

## Installation

Clone the repository:
```bash
git clone https://github.com/feelerx/AutoGrad.git
cd AutoGrad
```

## Usage

### Compilation

Compile the program using your preferred C compiler:
```bash
gcc autograd.c -o autograd
```

### Running the Program

Execute the compiled binary:
```bash
./autograd
```

### Input Format

The program accepts mathematical expressions in the following format:
- Expressions must be enclosed in parentheses
- No spaces between characters
- Use standard arithmetic operators

#### Examples

Valid expressions:
```
(1+1)
((6*2)+3)
((31+(33+13)+(153+6))*3+(3*8))
```

### Program Output

The program outputs:
1. The parsed expression
2. Extracted values and operations
3. The computed result
4. Complete node information including:
   - Node addresses
   - Computed values
   - Gradient values
   - Operations performed

## Implementation Details

### Data Structure

The core data structure is the `value` struct:
```c
struct value {
    double data;           // The computed value
    double grad;           // The gradient
    char op;              // Operation that produced this value
    struct value* next_value;    // Child node
    struct value* prev_value1;   // First parent node
    struct value* prev_value2;   // Second parent node
};
```

### Gradient Computation Rules

- **Addition**: Both operands receive the same gradient
- **Multiplication**: Each operand receives the gradient multiplied by the other operand's value
- **Subtraction**: First operand receives the gradient, second receives negative gradient
- **Division**: Gradients computed using quotient rule

### Algorithm Flow

1. **Expression Parsing**: Parse input string to extract values and operators
2. **Node Creation**: Create value nodes for each number in the expression
3. **Graph Construction**: Build computational graph respecting operator precedence
4. **Forward Pass**: Compute the result following the expression
5. **Backward Pass**: Traverse graph and compute gradients via backpropagation

## How It Works

### Forward Pass
The program uses two stacks (one for values, one for operators) to evaluate expressions while respecting operator precedence and parentheses. As operations are performed, nodes are linked to create the computational graph.

### Backward Pass
Starting from the result node (gradient = 1.0), the program traverses backwards through the graph, computing gradients for each node based on:
- The operation that produced it
- The gradients of child nodes
- The chain rule of calculus

## Limitations

- Maximum expression length: 100 characters
- Maximum stack size: 100 elements
- Input must be properly parenthesized
- No support for unary operations
- No support for advanced mathematical functions (sin, cos, exp, etc.)
- Expressions must not contain spaces

## Use Cases

- Understanding automatic differentiation concepts
- Learning backpropagation algorithms
- Educational demonstrations of computational graphs
- Building simple gradient-based optimization
- Neural network gradient computation (for simple networks)

## Example Session

```
enter the expression {without spaces}
and in brackets examples below
***ex1: (1+1)
***ex2: ((6*2)+3)
***ex3: ((31+(33+13)+(153+6))*3+(3*8))

ENTER EXPRESSION:((6*2)+3)

expression:((6*2)+3)
values:{6 2 3 }
operations:{*+}
value length:{1 1 1 }
Result: 15.000000

Node Address: 0x...
Value: 15.000000
Grad: 1.000000
Operation: +
...
```

## Future Improvements

Potential enhancements:
- Support for additional mathematical functions (exp, log, sin, cos, etc.)
- Power operations
- Unary operations (negation)
- Better error handling and input validation
- Support for variables and symbolic differentiation
- Higher-order derivatives
- Graph visualization
- Memory optimization

## Contributing

Contributions are welcome. Areas for improvement include:
- Additional mathematical operations
- Performance optimizations
- Extended functionality
- Better documentation and examples
- Input validation and error handling

Please feel free to submit issues and pull requests.

---

Note: This is an educational implementation demonstrating automatic differentiation concepts. For production applications requiring automatic differentiation, consider using established frameworks like TensorFlow, PyTorch, or JAX.
