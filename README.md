# File Explorer Application

A fully functional File Explorer Application, with both Normal and Command modes, built in C++.

## Table of Contents

- [Introduction](#introduction)
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

## Some screenshots

<img width="1069" alt="1" src="https://github.com/rohithooda10/file-explorer/assets/109358642/57ff7cc7-a1ec-4877-b32c-91e75d4f338d">
<img width="1069" alt="2" src="https://github.com/rohithooda10/file-explorer/assets/109358642/8e9841b5-1047-4204-9f96-4919ee5565cf">
<img width="1072" alt="3" src="https://github.com/rohithooda10/file-explorer/assets/109358642/61ac753f-e918-4d71-a9a1-380e55b19f47">
<img width="1069" alt="4" src="https://github.com/rohithooda10/file-explorer/assets/109358642/215c96f9-e6fb-4fd4-82dd-540bb19dcc32">
<img width="1069" alt="5" src="https://github.com/rohithooda10/file-explorer/assets/109358642/04a0c5be-439e-476f-84d8-754dbf2a24bf">
<img width="1069" alt="6" src="https://github.com/rohithooda10/file-explorer/assets/109358642/a0d85cf7-50c1-4076-a20a-065f5ba8164e">

## Features

### Normal Mode

Normal mode is the default mode of your application. It has the following functionalities:

1. **Displays a list of directories and files in the current folder**
   - Every file in the directory is displayed on a new line with the following attributes for each file:
     i. File Name
     ii. File Size
     iii. Ownership (user and group) and Permissions
     iv. Last modified
   - The file explorer shows entries “.” and “..” for the current and parent directory, respectively.
   - The file explorer handles scrolling using the up and down arrow keys.
   - Users can navigate up and down in the file list using the corresponding up and down arrow keys. The up and down arrow keys also handle scrolling during vertical overflow.

2. **Open directories and files**
   - Directory: Clears the screen and navigate into the directory and show the directory contents as specified in point 1.
   - File: Opens the file in an editor.

3. **Traversal**
   - Go back: Left arrow key takes the user to the previously visited directory.
   - Go forward: Right arrow key takes the user to the next directory.
   - Up one level: Backspace key takes the user up one level.
   - Home: "h" key takes the user to the home folder.

### Command Mode

The application can enter the Command mode whenever the colon (":") key is pressed. In the command mode, the user can enter different commands. All commands appear in the status bar at the bottom.

1. **Copy, Move, Rename**
   - Copy: `$ copy <source_file(s)> <destination_directory>`
   - Move: `$ move <source_file(s)> <destination_directory>`
   - Rename: `$ rename <old_filename> <new_filename>`
   - Example:
     - `$ copy foo.txt bar.txt baz.mp4 ~/foobar`
     - `$ move foo.txt bar.txt baz.mp4 ~/foobar`
     - `$ rename foo.txt bar.txt`
   - Copying/Moving directories also be implemented.
   - The file ownership and permissions remains intact.

2. **Create File and Directory**
   - Create File: `$ create_file <file_name> <destination_path>`
   - Create Directory: `$ create_dir <dir_name> <destination_path>`
   - Example:
     - `$ create_file foo.txt ~/foobar`
     - `$ create_dir foo ~/foobar`

3. **Delete File and Directory**
   - Delete File: `$ delete_file <file_path>`
   - Delete Directory: `$ delete_dir <dir_path>`
   - On deleting a directory, you recursively delete all content present inside it.

4. **Goto**
   - `$ goto <location>`
   - Example: `$ goto <directory_path>`

5. **Search**
   - `$ search <file_name>`
   - `$ search <directory_name>`
   - Search for a given file or folder under the current directory recursively.
   - Output is True or False depending on whether the file or folder exists.

6. **Exiting Command Mode**
   - On pressing the "ESC" key, the application goes back to Normal Mode.

7. **Quitting the Application**
   - On pressing "q" key in normal mode, the application closes. Similarly, entering the "quit" command in command mode also closes the application.


## Usage

1. To start the application, compile the `main.cpp` file using the C++ compiler and run:

   ```shell
   $ g++ main.cpp -o file_explorer

    ```shell
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
