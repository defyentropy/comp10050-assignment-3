# [AA Assignment 3](https://csgitlab.ucd.ie/hari-mohan/aa-assignment-3)

## Design Spec

### How does our application read files?

The app uses CSV files to store kanban board data to disk. These CSV files are
constructed almost exactly to the 
[RFC spec](https://www.ietf.org/rfc/rfc4180.txt): two small changes were made, 
however, because they suit the pruposes of our application better. First, a 
parsed field cannot be longer than 80 characters, because that is the length of the array 
reserved in each list node struct to hold the names of the lists and items. 
Second, while the spec allows for newlines within fields, the app treats all 
newlines as line terminators, partially because it's not possible to enter a 
field with a newline through the app anyway and also because it conflicted with
the formatting when printing out a list.

A robust CSV parser constitutes one half of the File I/O functionality. The 
CSV parser is able to gracefully handle any formatting error in the CSV
file supplied by the user and report back to them the number of the first
malformed line it found, so that they may manually inspect and correct the file
before trying again. It does this by reading the file one character at a time
using `fgetc()` and then runs checks to ensure that the addition of the newest
character does not cause the parsed field to become invalid. For example, any
fields with spaces in them must be wrapped in double quotes, and a line
can't end without having closing quotes to match the opening quotes. The parser
ignores quotes used to wrap fields, and those used to escape quotes in the
field itself.

Though the use of `strtok()` was considered, the current approach was chosen
because it allowed for the use of commas within list and list item names. With
`strtok()` commas within fields would not be treated any differently from those
used as delimiters.

### How does our application edit lists and items?
To edit lists and items in our program, we created a selection of functions 
specifically tailored to traverse and interact with the doubly linked list 
structure efficiently. These functions provide allow to manipulate their boards. 
The search functions, searchByListName() and searchByListItemName(), iterate 
through the doubly linked list structure, comparing the names of each list or 
item with the specified search query. For instance, searchByListName() traverses 
the board's structure, comparing the name of each list with the provided list 
name until a match is found or the end of the structure is reached. Similarly, 
searchByListItemName() performs a similar process within individual lists, 
comparing the names of each item until a match is found or the end of the list 
is reached. If a match isn't found info messages are displayed using our 
function printLog(), notifying the user that their input may be misspelt. Once a 
match is found, the function returns a pointer to the corresponding list or item, 
enabling further actions such as editing or removal. Functions like 
insertListItem() and removeListItem() allow for precise manipulation of the 
structure, whether it's adding or removing items from lists. These functions 
meticulously manage the pointers within the doubly linked lists, ensuring the 
integrity of the structure is maintained throughout the editing process. 
Moreover, the displayList() and displayBoard() functions offer users visual 
feedback, enabling them to observe changes in real-time. Overall, these 
functions collectively provide a seamless editing experience, allowing users 
to insert, delete, or transfer items between lists with ease.

### How does our application save a board to a file?

To save a board to a file, the app actually follows the pointers to the end of
each list first, and then traverses the list backwards, printing each item to 
the file in the format `<list name>,<item name>`; this preserves the order of
the lists because when the items are read back from the file, they are always
inserted at the beginning of the lists. Though storing the list name once for
each item is redundant, a relational system is tricky to implement with a flat-
file format like CSV. I had some ideas, but they all made the stored files
less easy for humans to read and write manually. All fields are properly 
normalised before being written to file. Fields with spaces or double quotes 
are wrapped in double quotes, and each double quote in the text is escaped by 
inserting another double quote right before it.
