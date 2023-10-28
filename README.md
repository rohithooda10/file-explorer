# File Explorer Application

A fully functional File Explorer Application, with both Normal and Command modes, built in C++.

## Table of Contents

- [Introduction](#introduction)
- [Prerequisites](#prerequisites)
- [Features](#features)
  - [Normal Mode](#normal-mode)
  - [Command Mode](#command-mode)
- [Usage](#usage)
- [File Structure](#file-structure)
- [Building and Running](#building-and-running)
- [Contributing](#contributing)
- [License](#license)

## Introduction

This File Explorer Application is a project developed as an assignment. It provides two modes: Normal Mode for browsing directories and files and Command Mode for executing various file and directory operations.

## Prerequisites

To build and run this application, you should have the following prerequisites:

- Basic knowledge of C++ programming.
- A C++ compiler (e.g., g++) for code compilation.
- A terminal or command prompt to execute and test the application.

## Features


### Normal Mode

Normal mode is the default mode of your application. It should have the following functionalities:

1. **Display a list of directories and files in the current folder**
   - Every file in the directory should be displayed on a new line with the following attributes for each file:
     i. File Name
     ii. File Size
     iii. Ownership (user and group) and Permissions
     iv. Last modified
   - The file explorer should show entries “.” and “..” for the current and parent directory, respectively.
   - The file explorer should handle scrolling using the up and down arrow keys.
   - Users should be able to navigate up and down in the file list using the corresponding up and down arrow keys. The up and down arrow keys should also handle scrolling during vertical overflow.

2. **Open directories and files**
   - Directory: Clear the screen and navigate into the directory and show the directory contents as specified in point 1.
   - File: Open the file in a vi editor.

3. **Traversal**
   - Go back: Left arrow key should take the user to the previously visited directory.
   - Go forward: Right arrow key should take the user to the next directory.
   - Up one level: Backspace key should take the user up one level.
   - Home: "h" key should take the user to the home folder.

### Command Mode

The application should enter the Command mode whenever the colon (":") key is pressed. In the command mode, the user should be able to enter different commands. All commands appear in the status bar at the bottom.

1. **Copy, Move, Rename**
   - Copy: `$ copy <source_file(s)> <destination_directory>`
   - Move: `$ move <source_file(s)> <destination_directory>`
   - Rename: `$ rename <old_filename> <new_filename>`
   - Example:
     - `$ copy foo.txt bar.txt baz.mp4 ~/foobar`
     - `$ move foo.txt bar.txt baz.mp4 ~/foobar`
     - `$ rename foo.txt bar.txt`
   - Assume that the destination directory exists, and you have write permissions.
   - Copying/Moving directories should also be implemented.
   - The file ownership and permissions should remain intact.

2. **Create File and Directory**
   - Create File: `$ create_file <file_name> <destination_path>`
   - Create Directory: `$ create_dir <dir_name> <destination_path>`
   - Example:
     - `$ create_file foo.txt ~/foobar`
     - `$ create_dir foo ~/foobar`

3. **Delete File and Directory**
   - Delete File: `$ delete_file <file_path>`
   - Delete Directory: `$ delete_dir <dir_path>`
   - On deleting a directory, you must recursively delete all content present inside it.

4. **Goto**
   - `$ goto <location>`
   - Example: `$ goto <directory_path>`

5. **Search**
   - `$ search <file_name>`
   - `$ search <directory_name>`
   - Search for a given file or folder under the current directory recursively.
   - Output should be True or False depending on whether the file or folder exists.

6. **Exiting Command Mode**
   - On pressing the "ESC" key, the application should go back to Normal Mode.

7. **Quitting the Application**
   - On pressing "q" key in normal mode, the application should close. Similarly, entering the "quit" command in command mode should also close the application.


### Command Mode

In Command Mode, you can enter different commands in the status bar at the bottom of the screen, including:

1. Copy files or directories to a destination.
2. Move files or directories to a destination.
3. Rename files or directories.
4. Create new files or directories.
5. Delete files or directories (including recursive deletion of directories).
6. Go to a specific directory location.
7. Search for files or directories recursively.
8. Return to Normal Mode using the 'ESC' key.

## Usage

1. To start the application, compile the `main.cpp` file using the C++ compiler:

   ```shell
   $ g++ main.cpp -o file_explorer
Run the application:
$ ./file_explorer
2. You will be in Normal Mode by default. Use the arrow keys, 'Enter' key, and specific shortcuts to navigate and interact with files and directories.

3. To switch to Command Mode, press the ':' key, enter a command, and press 'Enter' to execute it.

4. To exit the application in Normal Mode, press the 'q' key. In Command Mode, enter the 'quit' command.

## File Structure
1. main.cpp: The main application source code.
2. README.md: This documentation file.

## Building and Running
Follow the instructions in the Usage section to compile and run the application.

## Contributing
If you want to contribute to this project, feel free to submit issues or pull requests on the GitHub repository.

## License
This project is licensed under the MIT License - see the LICENSE file for details.