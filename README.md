# C++ Programming Labs

This repository contains solutions and projects for my C++ programming laboratory classes. Each folder corresponds to a specific lab assignment.

## Table of Contents
* [Lab 01: Inventory System](#lab-01-inventory-system)
* [Lab 02: 2D Image Class](#lab-02-2d-image-class)
* [Lab 03 project: Event Bus System](#lab-03-project-event-bus-system)
* [Lab 03: Event Bus Optimization](#lab-03-event-bus-optimization)
* [Lab 04: TileMap Views and Iterators](#lab-04-tilemap-views-and-iterators)
* [Lab 05: String Comparator Builder](#lab-05-string-comparator-builder)
* [Lab 06: Multi-dimensional Tensor & Views](#lab-06-multi-dimensional-tensor--views)
* [Lab 07 project: Expression Parser & AST](#lab-07-project-expression-parser--ast)
* [Lab 07: AST Optimization & Unary Operators](#lab-07-ast-optimization--unary-operators)
* *More labs coming soon...*

---

### Lab 01: Inventory System
Focuses on object-oriented programming, polymorphism, and manual memory management in C++. It implements an `Inventory` system that stores various derived items (like `Weapon` and `HealthPotion`) using base class pointers, demonstrating the virtual `clone()` pattern and the Rule of Three.

### Lab 02: 2D Image Class
Focuses on advanced class design and move semantics. RGBA pixels are stored in a 1D dynamic array to maximize cache efficiency. Covers the Rule of Five, operator overloading for arithmetic/access, and ANSI escape codes for terminal rendering.

### Lab 03 project: Event Bus System
Focuses on software architecture and the Observer design pattern. Implements a centralized `EventBus` with a polymorphic event queue, demonstrating decoupled communication between game systems using subscriber callbacks.

### Lab 03: Event Bus Optimization
Focuses on "hardware sympathy" and low-level performance. Replaces inheritance with **Tagged Unions** to improve memory locality and introduces **bitwise masks** for high-efficiency event filtering, minimizing pointer indirection.

### Lab 04: TileMap Views and Iterators
Focuses on data abstraction and non-owning interfaces. Implements a `TileMap` with complex terrain properties and a `TileMapView` that provides a windowed interface. Features a custom **STL-compatible Iterator** for traversing 2D sub-grids with coordinate offsets.

### Lab 05: String Comparator Builder
Focuses on the **Builder Pattern** and **Fluent Interface** to construct complex sorting logic. It utilizes a hierarchy of **functors** combined via a **Composite pattern** to support multi-level criteria sorting. Emphasizes memory management where the builder maintains ownership of functor instances in contiguous memory.

### Lab 06: Multi-dimensional Tensor & Views
Focuses on high-performance n-dimensional data representation and coordinate mapping. It implements a `Tensor` class using **strides** for indexing a 1D memory block. Features recursive dimensionality reduction via `TensorView` and supports both index-vector and scalar access.

### Lab 07 project: Expression Parser & AST
Focuses on symbolic computation and the fundamentals of compiler design. It implements a mathematical expression parser that converts **Reverse Polish Notation (RPN)** into an **Abstract Syntax Tree (AST)**. Features a Lexer for tokenization and a stack-based parser to build a polymorphic tree structure for recursive evaluation.

### Lab 07: AST Optimization & Unary Operators
Focuses on tree traversal and symbolic manipulation. This extension introduces **unary negation** (using the `~` symbol to resolve RPN ambiguity) and recursive serialization of the tree back to a human-readable string format. The core feature is a **simplification engine** that performs **constant folding** (pre-calculating static sub-branches) and applies **algebraic identities** (e.g., $x + 0 \rightarrow x$, $x \cdot 0 \rightarrow 0$). It demonstrates how intermediate representations can be optimized by pruning redundant nodes and reducing computational complexity before final evaluation.