# Koala : A Toy Programming Language 🐨

Made to learn about compilers and programming languages, Koala is heavily inspired by Bob Nystrom's [Crafting Interpreters](https://craftinginterpreters.com/).

---

## Introduction

The project includes:

- **A recursive descent parser** for syntactic analysis.
- **The visitor pattern** for evaluating the abstract syntax tree (AST).
- **Runtime support** for expressions, statements, variables, and function calls.
- **Error reporting** with line and column highlights (Rust-style formatting).

### Tech Stack

- **Language**: C++
- **Build System**: CMake, Make

### Implemented Features

- Arithmetic expressions with operator precedence
- Variable declarations and assignments
- Block-scoped environments
- Conditional statements (`if`, `else`)
- Loops (`for`, `while`)
- Functions and function calls
- Return statements
- Runtime error reporting with file name and line/column tracing

---

## Future Scope

The project is far from done, these are some things off the top of my head that I would like the language to have:

- **Better C++** (my experience with C++ has been limited to DSA problems - this lack of knowledge has led to a lot of hacky and unsafe things which I would like to revisit)
- **Ternary operator and switch statement** (I overuse the ternary operator in every language I touch, it’d be a shame if my own didn’t support it)
- **Syntactic Sugar**: (support for `else if (elif?)`, `do-while`, etc. to make the language feel nicer to write)
- **stdlib functions** (user input, print function, random number generator etc.)
- **Closures** (currently functions work without closures)
- **Support for Object-Oriented Programming** (classes and inheritance)

Currently it's quite jarring to write ```.kol``` files. Since I've been fascinated by LSP, treesitter and related technologies in the past, I'd also like to have:
- **Integration with Tree-sitter** for better syntax parsing
- **LSP Support** to enable editor features like autocomplete, go-to-definition, error detection etc.

---

## Resources & Inspirations

Wouldn't have been able to come even halfway without these reources:

- [Crafting Interpreters](https://craftinginterpreters.com/) by Bob Nystrom
- [Writing A C Compiler](https://norasandler.com/2017/11/29/Write-a-Compiler.html) by Nora Sandler
- [Making A Compiler](https://youtube.com/playlist?list=PLUDlas_Zy_qC7c5tCgTMYq2idyyT241qs\&si=K2s9c6tDGEPZXxgb) - YouTube Playlist

---

## Building the Project

To build the project:

```bash
mkdir build && cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. && make
```

This will generate a binary called ```koala``` in the build directory, which can be invoked in one of two ways:

To drop into a REPL:

```bash
./koala
```

To provide an input file:

```bash
./koala ../tests/fibonacci.kol
```

---

