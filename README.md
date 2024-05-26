
## DOS and Notepad: Data Structures and Algorithms Project

### Overview

The "DOS and Notepad" project is a comprehensive implementation of a DOS shell and a text editor similar to Vim. Developed as part of a Data Structures and Algorithms course, the project aims to demonstrate proficiency in linked lists, vectors, and generalized trees. It consists of two main parts: the implementation of a DOS shell and a text editor resembling Vim.

### Part 1: DOS Shell

The DOS shell component replicates the functionality of a command-line interface similar to that of the DOS operating system. It provides various commands for file and directory manipulation, file editing, and system navigation. Key features include:

- **File and Directory Operations**: Commands such as `CD`, `DIR`, `MKDIR`, `RMDIR`, and `TREE` allow users to navigate through directories, create and delete directories, and list directory contents.
- **File Manipulation**: Commands like `CREATE`, `EDIT`, `COPY`, `MOVE`, `DEL`, and `RENAME` enable users to create, edit, copy, move, delete, and rename files.
- **Text File Editing**: The `EDIT` command loads a text file into a linked list, allowing users to edit the file contents interactively. Features include text insertion, deletion, backspacing, appending, multiline editing, cursor movement, redo, and undo operations.
- **Printing Queues**: Files can be added to regular and priority-based printing queues using commands such as `PRINT`, `PPRINT`, `QUEUE`, and `PQUEUE`.
- **Help and Information**: The `HELP` command provides detailed information about available commands and their usage.

### Part 2: Notepad Vim

The Notepad Vim component implements a text editor similar to Vim, focusing on efficient text editing capabilities using linked lists. Key features include:

- **Linked List Representation**: The entire document is represented as a linked list of characters, allowing for efficient editing and manipulation.
- **Text Editing Features**: Users can insert, delete, append, and modify text with support for cursor movement, undo, and redo operations.
- **Interactive Editing**: Text editing is performed interactively, with support for multiline editing and navigation.
- **Vim-like Interface**: The text editor interface is designed to resemble Vim, providing a familiar environment for users familiar with the Vim text editor.

### Usage

1. **Compilation**: Compile the source code using a C++ compiler.
2. **Execution**: Run the compiled executable file to launch the DOS shell or text editor.
3. **Interface**: Interact with the command-line interface of the DOS shell or the text editor interface to perform various operations.

### Deadline and Acknowledgments

The project was completed as part of a Data Structures and Algorithms course, with a deadline of 18/12/2023. Special thanks to Dr. Zeeshan Rana and Dr. Atif Mehdi for their contributions and guidance in developing this project.

### License

This project is licensed under the MIT License. See the `LICENSE` file for more details.

### Author

- [Muhammad Taha Saleem](https://github.com/twonum)
