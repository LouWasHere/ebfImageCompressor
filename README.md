# Assignment 1

## Quick Links
- [Key Information](#key-information)
- [The Task](#the-task)
- [File Formats](#file-format-details)
    - [ebf](#ebf-files)
    - [ebu](#ebu-files)
    - [ebc](#ebc-files)
- [Running the Code](#using-the-files)
- [Hints and Tips](#hints-and-tips)
    - [Where to Start](#where-do-i-begin)
    - [Branching in Git](#branching-in-git)
- [Common Errors](#common-issues)
    - [Compilation Errors](#compilation-errors)
    - [Runtime Errors](#runtime-errors)

## Key Information
### Basic Submission Rules
This repository is your base code for the Assignment 1.
You can edit and add any files that you want, but there are some basic submission requirements:

- You c, h and makefile should be in this directory, not inside any subfolders.
- Your executables should be created in this directory
- Your makefile and code must compile on the lab machines.

**Submission is via GradeScope**.

### Academic Integrity

You should already have completed the University's Academic Integrity course - but to clarify:
- Any code you submit, other than code provided by the module leader, must be **your own work** - not that of your peers, any external person, or any code generation tool.
- You should not share any code you write with people other than module staff until final module marks are returned at the end of the academic year.
- You can discuss ideas with your peers, but do not write any code together
- Your work **will** be passed through an academic integrity tool

### Marking

This coursework is worth 50% of the final grade, divided between the sub-tasks as shown. However, **code quality** is also a key aspect of this coursework and these marks are allocated separately.

**Please note that the testing script gives you a count of tests passed which is *not equivalent* to your expected mark.**

# The Task

Your tasks are:

1. (15 marks) Improve ebfEcho.c and ebfComp.c
    - Consider how they could be improved in terms of modularity, defensiveness and reuse.
    - Rewrite these files to improve them.
    - Remove any logic errors in the files.
2. (10 marks) Modify ebfEcho and ebfComp to use 2D arrays.
3. (10 marks)Create two new utilities ebuEcho and ebuComp to handle the format ebu (detailed below), and create utilities ebf2ebu and ebu2ebf to convert between the two formats.
4. (15 marks) Create two new utilities ebcEcho and ebcComp to handle the format ebc (detailed below), and create utilities ebc2ebu and ebu2ebc to convert between the two formats.

## Additional Tasks
Along with these tasks, you will also need to:

- Extend the provided shell script to test all possible errors on all the executables you have created.
- Extend the makefile provided to compile your new executables and any additional files created in the improvement process.

## Testing

You will be provided with a basic shellscript which tests the tools up to the end of Task 3. You should extend this to test Task 4, as well as adding any additional tests which you think appropriate. When your code is submitted, we will test it using our own comprehensive test script. We may test things that you haven't though of.
All programmes should return 0 in the case of successful exit or non-0 where the code fails, with an appropriate string to describe the result. This is a Unix convention- 0 means success because there is only one way in which a program can succeed, but there are many different ways to fail.

The messages and return codes which you should use are:


| Value  | String | Condition |
| ------------- | ------------- | ------------- |
| 1  | ERROR: Bad Argument Count | Program given wrong # of arguments |
| 2 | ERROR: Bad File Name (fname) | Program fails to open file |
| 3 | ERROR: Bad Magic Number (fname) | File has an illegal magic number |
| 4 | ERROR: Bad Dimensions (fname) | File has illegal dimensions |
| 5 | ERROR: Image Malloc Failed | Malloc failed to allocate memory |
| 6 | ERROR: Bad Data (fname) | Reading in data failed |
| 7 | ERROR: Output Failed (fname) | Writing out data failed |
| 100 | ERROR: Miscellaneous (describe) | Any other error which is detected |

In all cases, fname should be replaced by the name of the file which caused the error.
In the case of Miscellaneous errors, in 100 chars or less please describe what the error was.

All other print statements will be ignored by the final test script.

You should also use these standard success messages, for which the value is always 0.

| String | Condition |
| ------------- | ------------- |
| Usage: executablename file1 file2 | Program run with no arguments |
| ECHOED | xEcho file successfully completes |
| IDENTICAL | xComp file successfully completes and files are identical |
| DIFFERENT | xComp file successfully completes and files are not identical |
| CONVERTED | x2y file successfully completes |

Usage messages are also a Unix convention - when an executable is run with no arguments, it is assumed that the user does not know the correct syntax and wishes to see a prompt. As this is not an error state, the return value is 0.

# File Format Details

The three file formats you will be using are ebf, ebu and ebc - these are all **new file formats** which have been created for this assignment - you **will not** find any useful information about them online. They are loosely based on the PGM image format which you can find out more about [here](https://netpbm.sourceforge.net/doc/pgm.html).

You have some sample ebf files available in the sample_images folder.

## ebf files

ebf files are a basic greyscale image file. They contain a short header of metadata, followed by pixel values. The basic format is as follows:

```
eb              - this is the magic number - for ebf files this is always eb.
height width    - two space-separated integers which give the height and width of the image.
p1 p2 p3 ...    - the pixel data which is separated by spaces. This is usually (but not always) separated into rows by a newline.
```

ebf files have a maximum pixel value of 31.

## ebu files

ebu (ebf uncompressed) are the binary equivalent of ebf files. They have a very similar header (written in ASCII) followed by the pixel data written out as **binary**. If you open an ebu file in a text editor, you will generally see a random string of data as it is not encoded in a human-readable format.

```
eu
height width
p1 p2 p3 ...
```

ebc files have a maximum pixel value of 31.

## ebc files

ebc (ebf compressed) are compressed binary ebf files. They also have a similar header, but the pixel data is written out in **compressed** binary. As ebf files have a maximum pixel value of 31, each pixel can be represented by **5 bits of binary**. C can only write out blocks of 8 bits (1 byte) at once, so you will need to design an algorithm to pack your pixel data into the minimum number of bytes. This should compress an ebu file to just over 0.6 times its original size.

```
ec
height width
packed binary pixel data
```

# Using The Files

You can compile the provided files by using:
```
make all
```
Executables can then be run the standard way.

To run the testing script, the first time:
```
chmod +x tests.sh
```
This gives you permission to execute the file - you only need to do this once.

You can then run the test suite using:
```
./tests.sh
```

# Hints and Tips

## Where do I begin?

Start by reading through ebfEcho and ebfComp. Both files are extensively commented, and you should be able to follow the logic quite easily through the files. Look for places with bad code quality- some key things to look out for are:
- blocks of repeated code
- complicated/hard to follow code
- unclear variable names
- very long functions

You can also run the testing suite as you will find a number of tests are failing - some of these are easy to fix.

Make yourself a list (to_do.md for example - this allows you to use [readme-style markup](https://docs.github.com/en/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax) which is a useful skill) and write down all the issues you see - add line numbers so you can find them again easily later.

Once you have a list, read through it and put the changes into a sensible order - you should usually start at the top of the code and work down. If you find yourself making exactly the same change in 2 different places, consider if this might be moved into a function somewhere else!

## What is binary data?

A common issue which students encounter in these tasks is working with binary data.

When we talk about binary vs ASCII data in programming, we are essentially talking about how data is encoded. ASCII data is encoded in a human-readable format, and each character takes up 8 bits (technically, 7 bits and 1 for parity). Binary data is encoded in a smaller but non-human-readable format - while the number '255' would take 3 bytes of ASCII, it is 1 byte of binary. However, when a human tries to open a binary-encoded file, you will not be able to read it:
```
eu
2 2

```
This is slice0a.ebf, converted into an ebu - it's smaller, but no longer readable! However, the exact same numbers are there - just in a more compressed format.

You will find that there are binary read and write functions built into C - you do not need to do **any** conversion.

## Branching in git

Your use of git is assessed in this module, but more importantly it is a very useful tool when you're working on a large, complex set of files. Many of you will never have used git before, so there are a few useful things you should know:
 
- the 'main' branch should only ever contain code which compiles and passes the tests (we call this 'clean code')
- in industry, this is usually protected so that you cannot push to main directly
- developers usually work on branches and only merge in their working, tested code

These conventions are because the majority of software development happens on code which is *already deployed* - therefore if you were to push code which doesn't work onto main, you run the risk of this code going live and affecting your **paying customers**. There is even a widely used git command to find out who last edited a line of code (`git blame`) in order to find and discipline developers who break the deployed code.

As the main branch should always be 'clean', you should get used to making and working on new branches. A branch is a separate copy of the code- it is in the same repository, but is stored separately on the git server so changes to one branch do not affect other branches. You merge your branch into main once you are happy that the code works and does not break the tests. You can also abandon a branch if you break something!

This means that you will **always have a version of the code which runs** - if you use branching properly, you will never get into a situation where you need to start again from scratch.

To create and move to a new branch, make sure all your changes are committed and pushed, and use:
```
git branch new_branch_name
git checkout new_branch_name
```
or
```
git checkout -b new_branch_name
```
which is equivalent to the 2 line version.

`git branch` creates a new branch, and `git checkout` moves you onto it. The `-b` option means 'create this branch'.

From this point on, when you want to push your changes you will need to make sure you use:
```
git push origin new_branch_name
```
Once you are happy with your code, you will merge it into your 'main' branch - make sure you have commited and pushed everything.
```
git checkout main
git merge new_branch_name
```
If you have changed any files in main since your last push to main, you may have merge conflicts - you just need to select the version of your code which you want to keep. You can refer to this [very useful guide](https://git-scm.com/book/en/v2/Git-Branching-Basic-Branching-and-Merging) which explains merging and merge conflicts in more detail.

# Common Issues

## Compilation Errors

Usually GCC will give you a relatively helpful error when you compile your code, telling you what and where your error is. **However** sometimes you will get an error produced by your makefile itself. The most common issue is:

```
make: *** No rule to make target 'file.o', needed by 'file'.  Stop.
```

This is almost always caused by 'file.c' not existing in your folder - you should double check spelling and capital letters and ensure that your files are in the right place.

Another common issue is:
```
multiple definition of 'main'
```
You can only ever have one 'main' function per executable - if you have tried to compile two .c files into one and they both have a main() function, you will get this error. If you want two files which both have their own main function to share some other utility function, the shared functions should be put in a different .c file.

## Runtime Errors

These errors don't prevent your code from compiling, but make your code crash while you are running it.

The one which most students will get at some point is:

```
Segmentation Fault (Core Dumped)
```

This means that you are trying to access some memory which you do not have permissions to access. There are a wide variety of things which can cause it, but most often it is incorrect use of malloc/calloc, trying to access NULL, or going out of bounds in an array.

To find the cause of your segfault, GDB is the best tool.

```
gdb executableName
set args arg1 arg2
run
```
Replace executableName, arg1 and arg2 with the executable name and arguments which gave you the segfault when you ran your code manually.

This should cause another segfault, but this time *inside GDB* where we can find out what happened.
```
backtrace
```

Backtrace lets you see what steps led to the segmentation fault - usually, this will point you to the file and line where the segfault happened. You can then set up a breakpoint and use the debugging tools you learned in Week 2 to trace your variables and work out what is giving you your segfault!

[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-f4981d0f882b2a3f0472912d15f9806d57e124e0fc890972558857b51b24a6f9.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=10555135)
# Assignment 2

- [Introduction](#introduction)
  * [Testing](#testing)
  * [The Algorithms](#the-algorithms)
    + [ebcBlock](#ebcblock)
      - [EBCC Images](#ebcc-images)
    + [ebcR32 and ebcR128](#ebcr32-and-ebcr128)
      - [The EBC Random Algorithm](#the-ebc-random-algorithm)
      - [EBCC Images](#ebcc-images-1)
- [This Repository](#this-repository)
- [Submission](#submission)

# Introduction

The hand-in date for this assignment will be the **10am on the 19th May**. Late submissions are permitted but with the standard University 5% per day penalty.

For assignment 2, the assignment is split into two parts as follows:

**1. 25 Marks - Improve Implementation of Assignment 1**


You should continue to work through the tasks in Assignment 1, using your existing repository and test script. You will receive some feedback on your code quality which should give you some idea of how to further improve this aspect of your code. Note that feedback will be provided within the University's standard 3 week time frame.

All the tasks and weighting remain the same as assignment 1, however this will be scaled down to 25 marks.

This must be submitted via GradeScope - Assignment 2 Part 1


**2. 25 Marks - Stretch Activity - Additional Compression Algorithms**

**This task relies on you having completed the code for ebc handling**.

Now that you have handled some simple compression (bit packing for ebc files), you will implement some more interesting and effective algorithms.

1. 20 Marks - ebcBlock - block-based compression using mean averages.
2. 15 Marks - ebcR32 - block-based compression using 32 random paradigm blocks.
3. 15 Marks - ebcR128 - block-based compression using 128 random paradigm blocks.

Each of these algorithms will be explained in their own sections below.

This must be submitted via GradeScope - Assignment 2 Part 2.


Again, 50% of the marks for the total assignment are for code quality - you can see the rubric which will be used to assess this on GradeScope.

# Testing

There will still be marks available for error checking, and you should ensure that your code is able to locate errors in the code. However, since your code will be working only with ebc files and data cannot be too high or low (as it is limited to 0-31 due to being written out in blocks of 5), these data tests can be ignored/removed.
For functionality, we will be looking at:
* Your code's performance on a variety of different sized images.
* How well you have implemented the algorithms
* The quality of your outputted images

For this assignment, you will not be provided with any test data as how you implement the algorithm may lead to minor differences between our final images - instead, you will be provided with a variety of final uncompressed images which have been converted to PGM image files which can be opened, which should give you some idea of what you should expect your own images to look like.

Your code quality will be assessed using the same rubric as in Assignment 1.

There are some tests set as visible in the Autograder which will allow you to check that your code has compiled correctly. If these are failing, please refer to the 'submission' section below for potential causes and fixes.

# The Algorithms

There are three algorithms for you to implement - two of them are quite similar and have been designed to test your ability to modularise and re-use your code.

For each algorithm, you should produce two executables - one to compress, and one to decompress the data.

The names of these executables should be:

| Algorithm      | Compression Executable | Decompression Executable |
|----------------|------------------------|--------------------------|
| ebcBlock       | ebcBlock               | ebcUnblock               |
| ebc random 32  | ebcR32                 | ebcU32                   |
| ebc random 128 | ebcR128                | ebcU128                  |

## ebcBlock

In this algorithm, your image must be split into 3x3 blocks. Each block should have a mean average value calculated from all existing pixels within the block.
Here is an example:

|   | col 0  | col 1  | col 2  |
|----|----|----|----|
| row 0 | 3  | 5  | 4 |
| row 1 | 7  | 5  | 11 |
| row 2 | 15 | 16 | 16 |

This 3x3 block has an average value of 82/9 = 9.11 - we will round this **down** to 9. You should use standard rounding - anything with a decimal part equal to or greater than 0.5 should be rounded up, anything less than 0.5 should be rounded down.

Therefore, this block will be represented by the single value 9 in our final image. When we decompress the file, all the pixels associated with the block will become 9.

Note that it is possible to have a block with fewer than 9 values - for example, on the edge of the image.

Once you have calculated these mean averages, you must write your data using packed binary - as with ebc images.

### EBCC Images

If you want to use file extensions to differentiate your different files, you can use .ebcc (ebc-compressed).

These compressed images will have the standard ebf-file-family header of:

```
magic number
height width (of the uncompressed image)
compressed image data
```

For files compressed using ebcBlock, the magic number should be **EC**.

## ebcR32 and ebcR128

As both of these algorithms use the same basic principle, they are explained together.

### The EBC Random Algorithm

This algorithm also requires you to create 3x3 blocks from your image.

In this family of algorithms, a set number of 3x3 'paradigm' blocks are chosen from the image. R32 takes 32 paradigm blocks, while R128 takes 128 paradigm blocks. These blocks will be used to represent every other block in the image and therefore it is important that you do not have any duplicate paradigm blocks. In cases where there are fewer than 32/128 blocks in the image, you can use fewer - it is up to you to decide how to best handle this in your code.

Each paradigm block should have an index assigned to it which should be between 0-31 for R32, and 0-127 for R128.

When you have chosen your paradigm blocks, you should iterate through the image and assign each 3x3 block the index (between 0-31, or 0-127) of its nearest paradigm block. You should decide on the closest block by the total difference between the blocks (again, how you do this is up to you, and you may want to try a few different things to see which gives you the best result in your final outputted image).

Your compressed file will contain both the paradigm blocks and your final image data, both written in bit-packed binary. ebcR32 uses 5 bits, but ebcR128 uses **7 bits** and therefore you will need to adapt your existing ebc-writing code to be able to handle this.

#### Randomness and Seeding

Because this algorithm relies on randomisation, it will have an additional argument - you must pass in the integer **seed** for the randomiser, and then use this to seed the randomiser in your code. You are not expected to develop any randomiser yourself, you can use the existing `rand()` function which is part of the standard C library. Seeding your code allows you to consistently reproduce errors or files, so will make testing significantly easier.

Therefore, an example of the use of this function is:
```
./ebcR32 my_file.ebc my_compressed_output.ebcc 8732
```

### EBCC Images

You can also use the .ebcc file extension for the outputs of ebcR32/ebcR128 - or use whatever extension you prefer to differentiate these files in your directory.

These compressed images will have the same header data as in all other ebf-family:

```
magic number
height width (of the uncompressed image)
paradigm blocks
compressed image data
```

#### Magic Numbers

| Algorithm | magic number |
|-----------|--------------|
| ebcR32    | E5           |
| ebcR128   | E7           |

# This Repository

For Part 1, you should continue to work in your Assignment 1 repository.

For Part 2, you may wish to take only the files you need from your Assignment 1 repository and work in this 'clean' repository, or you can continue working in Assignment 1 if you prefer - you will find that you will need some parts of your assignment 1 code.

In this repository, I am providing a few things which you may find useful:

 * Object files which convert ebf files to pgm files
 * Some example decompressed files which have been passed through the compression algorithms once.

The object files can be used to convert your ebf files into images which you can open using standard image manipulation software - this will be useful for visual debugging of your code. There are two objects - one compiled on Feng, one compiled on Codespace. I do not guarantee that these will work on other platforms (i.e. on Apple or Windows devices).

Note that the code includes almost no error checking- you should only be using it for valid ebf images.

The example images are provided to give you a sample of what an outputted image should look like- particularly in terms of how much visual noise you can expect.


# Submission

Two GradeScope assessments have been set up - one for your part 1 code, and one for part 2. Both allow multiple submissions.

You must submit:
* The .c and .h files required to create your executables.
* A working makefile which contains commands for 'clean' and 'all'.

To create a submission zip, you can use the following terminal command:
```
zip submission.zip *.c *.h makefile
```

This should create a 'submission.zip' which can be uploaded to GradeScope, alternatively you can use the drag and drop tool to select all of the files for your submission.

## 'My code won't compile on GradeScope'

The most common cause of this is having a subfolder in your submission. If this is the case, in your 'code' tab in GradeScope you will see 'folder_name/myfile.c' rather than just 'myfile.c'. You can fix this by following the instructions above and using the `zip` command.

Double check that your code compiles on Feng linux - if you have any compilation errors, these should be fixed prior to submission.

If you do not have any compilation errors, add the flag '-Wextra' to CFLAGS (or equivalent in your makefile) and recompile - this should highlight any additional compilation issues.

If this does not resolve your problem, please contact me via Teams (scsabr) during normal working hours (8am-6pm Mon-Fri) or speak to a lab demonstrator.

