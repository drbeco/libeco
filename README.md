# Libeco Library 

## Details 

Libeco is a library that provides a set of tools and useful functions to run console programs both in Linux and Windows. Its first intention was to unify C programs compiled using the proprietary old `<conio.h>`, making it easy to run those programs in Linux.

After some point, the Libeco library agregated some very interesting stuff (written in Linux) that was ported back to Windows.

So, using Libeco, you can count on an unified way to express yourself in C and keep compatibility with both OS's.

There are some great stuff there to help your terminal program to be more safe when running iteractively. Validation functions that limit the user input to certain ranges, types or letters, and even a great compiler (`geteval()`) that by itself is worth all of it.

You can even play some basic music (and that was inserted in Libeco to help porting Akernaak from BASIC TRS-80!) with playmml.

Paint and control screen and keyboard, get key names and extended ascii codes, clear screen, and so on. That's just basic stuff.

Some more important functions will help you in your scientific projects, like calculating combinations, factorials, arrangements, permutations, converting numbers from one base to another, converting ranges (that can be used to normalize variables), etc.

For games, you can randomly choose numbers without repetition (with `randnorep()`), pool the keyboard, check pipe communications...

Now that you have being teased, take a good look at the documentation and help yourself!

In appreciation,

Dr. Bèco

## Usage

To use libeco just add:

* `#include "libeco.h"`

at the start of your code and compile it against the C source, linking with the mathematical library (lm) with:

* `$ gcc myprogram.c libeco.c -o myprogram.x -lm`

If you use the function `ballwait()`, you need also to add `-lncurses` as this is the only function that needs the curses library.

If you are under Windows, add `-lconio` (from from Borland ( http://conio.sourceforge.net/). You can find the `conio.h` library on DEV-CPP package 2.0 ( http://conio.sf.net/ )

## Author, Copyright and Warranty

* Author: Prof. Dr. Ruben Carlo Benante (Dr. Bèco)
* Email: rcb@beco.cc
* Home: (http://www.beco.cc)
* Date: 2017-05-04
* Copyright (c) 2009-2019 GNU GPL v2

This program is free software: you can redistribute it
and/or modify it under the terms of the
GNU General Public License as published by
the Free Software Foundation version 2 of the License.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.
If not, write to the Free Software Foundation, Inc.,
59 Temple Place - Suite 330, Boston, MA. 02111-1307, USA.
Or read it online at (http://www.gnu.org/licenses/).

