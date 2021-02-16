This repository is adapted from the svn repository at https://valelab4.ucsf.edu/svn/3rdpartypublic/
It provides all publically available dependencies that are needed to build the Micro-Manager project. This repository is used as a submodule of the main project.

It was forked from revision 195 and was then modified since the original svn repository had files that were too big for github. This repository was then updated to match the changes up to revision 199 of the SVN repository.

Notable changes in initial commit:
-Boost has been updated from 1.55 to 1.72. Rather than manage the source ourself we just use Boostorgs repository as a submodule. No binary has been included. The user will have to build this themselves. Fortunately this is a straightforward process.
-Many files that are not used in building micromanager were removed.
