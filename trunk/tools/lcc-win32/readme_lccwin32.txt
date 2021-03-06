------------------------------------------------------------------------------

README file for LCC-Win32 users

------------------------------------------------------------------------------

There are two separate folders, one for PHP4 and the other for PHP5. Each contains a project file, the makefile, and the export file (.exp extension) for the PHP DLL.

Generating the PHP link library
-------------------------------

Step 1: the export file

You may use one of the included export files (php4ts.exp or php5ts.exp) or build your own. For example, to build php4ts.exp, first create the empty file and type 'php4ts.dll' (without the quotes) in the first line. Then use Microsoft Dependency Walker to obtain the list of exported functions in php4ts.dll. Copy the function list (one per line) to php4ts.exp and add a '_' in front of each function name.

Step 2: build the library

To generate the library, create a console (DOS) window and type

	[LCC]\bin\buildlib.exe php[N]ts.exp [LCC]\lib\php[N]ts.lib

where

	[LCC] = Installation path to LCC-Win32
	[N] = PHP version desired (4 or 5)



Building WinBinder with Wedit
-----------------------------

To build WinBinder in Wedit (LCC-Win32 code editor):

1) Locate the project file at compilers\Lcc-win32\php[N]\winbinder_php[N].prj, where [N] is the PHP version desired (4 or 5).
2) Open it on a text editor. It is in fact an INI file so it is easy to edit.
3) Change the paths in the project file so that they point to your folders.
4) Import the project file into Wedit.
5) Build the project using Build All.


Building WinBinder with another code editor
-------------------------------------------

If you prefer not to go with Wedit, you may want to configure your favorite source code editor to run the makefile generated by LCC-Win32. To do so, follow the steps below:

1) Create a tool that points to the LCC-Win32 Make utility. In my case it is C:\Util\lcc\bin\make.exe.
2) Configure the tool so that its initial folder is where your makefile is. In my case it is Compilers\Lcc-win32\php4.
3) If your text editor supports regular expressions to match compilation errors and errors, enter the following RE in the appropriate field:

^.+ .*([A-Za-z]:[^:]+): ([0-9]+)

In this RE, the file is at register 1 and Line is at register 2.

4) Assign a keyboard shortcut to the newly created tool. Now you can build WinBinder simply by pressing one key.
5) You may have to quit your text editor, start it again and recompile your program.
6) Now you can check for errors or warnings, double-click the error/warning in the Command Results window so the text editor will open the file at the appropriate location.


If you use TextPad, see also tools\textpad\readme_textpad.txt for more configuration tips.


------------------------------------------------------------------------------

I'd very much appreciate if you sent me corrections or updated information about this document, or a procedure for a different tool. Please contact me at:

Rubem Pechansky
http://www.hypervisual.com/winbinder/contact.php
