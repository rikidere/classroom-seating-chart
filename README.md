# classroom-seating-chart
## Problem Description:
There is one row to seat N students based on M preferences of each student.
Optimize the seating such that the preferences met as best as possible

## Randomized:
```
clang randomized.cpp [-03 | -O2 | -O1 | -O0 |]
```
has various parameters that can be used such as a range of score to be searched in, amount of those, output to file etc.
Output is the arrangement.

## Randomized2wide:
```
clang randomized2wide.cpp [-03 | -O2 | -O1 | -O0 |]
```
same as Randomized, but this time the neighbours of the neighbours are valid for preference as well
