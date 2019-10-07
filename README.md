This repository is adapted from the svn repository at https://valelab4.ucsf.edu/svn/3rdpartypublic/

It was forked from revision 195 and was then modified since the original svn repository had files that were too big for github.

Notable changes in initial commit:
-Boost has been updated from 1.55 to 1.71. Rather than manage the source ourself we just use Boostorgs repository as a submodule. No binary has been included. The user will have to build this themselves. Fortunately this is a straightforward process.
-Many files that are not used in building micromanager were removed.