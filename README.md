# Automata System

A Dothraki to English dictionary stored in a trie, written in C++ as an undergraduate programming assignment. It reads commands from an input file, one per line, and writes the result of each command to an output file.

## Build and run

```bash
g++ -std=c++11 main.cpp -o dictionary
./dictionary input.txt output.txt
```

The first argument is the command file, the second is the output file.

## Commands

- `insert(word,meaning)`: adds a word with its English equivalent and reports `was added`, or `already exist` if the word is stored.
- `search(word)`: writes the English equivalent. Otherwise it writes `no record` when no stored word starts with the same letter, `incorrect Dothraki word` when the word leaves the stored words after its first letters, or `not enough Dothraki word` when it is only the beginning of a stored word.
- `delete(word)`: removes the word and the nodes that no other word uses.
- `list`: writes every stored word with its meaning in alphabetical order, grouped under its first letter.

## Structure

Words are made of the lower-case letters `a` to `z`. Each trie node has 26 child pointers, and the node that ends a word holds its English meaning.
