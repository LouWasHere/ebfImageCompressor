# Changelog and To-Do List

## ebfEcho.c Changelog from 20/02/23 to 22/02/23

Add check for no arguments (Line 19)

Specify file causing BAD FILE error in error message (Line 48)

Specify file causing Bad Magic Number error in error message (Line 59)

Fix Return Value for what should be a Bad Magic Number error (Line 60)

Specify file causing Bad Dimensions error in error message (Line 70)

Improve error message to be more specific (Line 83)

Fix Bad Data check (Line 92)

Add a test for too much data (Line 102)

Add more detail to error message for output file being unreadable (Line 123)

Check if at the end of the file so as not to add a newline character (Line 142)

## ebfComp.c Changelog from 20/02/23 to 22/02/23

Add check for no arguments (Line 19)

Added more detail to the error message while checking for file names (Line 46) (Line 118)

Added more detail to Bad Magic Number error message (Line 57) (Line 129)

Corrected to Bad Magic Number return value (Line 58) (Line 130)

Added more detail to Bad Dimensions error message (Line 69) (Line 141)

Added more specificity to the error reporting a failed Malloc (Line 81) (Line 153)

Added check for too much data (Line 100) (Line 172)

## The Plan
### ALL BELOW CHANGES TO BE IMPLEMENTED IN EXPERIMENTAL BRANCH AND MERGED UPON COMPLETION

 - Modularize everything that repeats across the files (identical validation, repeated validation etc. don't go crazy.) DONE
 - Implement string constants for error messages for transferability (put in errorHandling) DONE

BELOW NOT TO BE STARTED UNTIL CODE IS FULLY MODULAR (IMPORTANT!)

 - Implement the 2D arrays for storing the ebf data DONE
 - Implement ebu and ebc formats and their conversions (likely also good to be modules, if located in independant files can likely be done. bit twiddling will be needed.)
 - Implement tests for this new functionality to the best of my ability

## Changelog - Starting 23/02/23

Created module for checking command line arguments. Code has been removed from ebfEcho and ebfComp and condensed into a specialized file and header called checkArgs.

Implemented a struct for image data to reduce the repitition of variables and amended all instances of aforementioned variables in both files.

Entirely modularized file reading related processes and any error handling related to it. Created 2 new files, fileReaderWriter and errorHandling for this purpose.

Completed modularization of ebfComp. Created new file compareImages to house comparison functions as well a a function to clear both images as if any difference is flagged the behavior is the same. Additionally, methods created for ebfEcho were re-used where appropriate.

Re-arranged array declaration for imageData to have its own function so when I implement the 2D array it will be modular already.

## Changelog - Starting 28/02/23

Merged changes from experimental to main branch. From now, work on experimental will be focused on new features.

Implemented 2D array with full functionality and additional error checking for relevant memory allocation. Adjusted other methods to work with this new data structure.

Implemented the ebu file format/struct and added the ebuEcho tool to enable file echoing. Also created tests for the ebu file struct as well as the modules required to read/write them in their binary format.

Implemented ebuComp using methods defined for ebuEcho. Began implementing ebf2ebu, refactored methods for distinction between the different methods concerning the different file types.

Completed implementation of ebf2ebu and ebu2ebf. Reused most modules from previous implementations and tools so they are structurally very simple. Began researching ebc ready for implementation.

Began development on ebc2ebu.

Restarted development on ebc2ebu. Much better solution with much better performance found. Implemented over the course of a day.

Began development on ebu2ebc using similar principles to ebc2ebu. Debugged and implemented over a day. Fixed minor bugs with ebc2ebu, tested appropriately. Added new testing scripts for ebc2ebu. Began planning ebcEcho and ebcComp.

Completed ebcEcho and ebcComp. Added further comments and indentation. Merged into main. Signing off for now.

## POSSIBLE AREAS OF IMPROVEMENT FOR CW2

 - Simplify data handling into a single struct, removing need for specialized functions for ebfImage and ebuImage instances.

 - Further testing for robustness. Improvements to be made based off feedback.

 - Fix any errors made in memory management. I probably made some.
   