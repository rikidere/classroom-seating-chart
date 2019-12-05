# classroom-seating-chart
## Problem Description:
There is one row to seat N students based on M preferences of each student.
Optimize the seating such that the preferences met as best as possible.

## Deterministic:
NP-HARD problem lol.
### DP-meme
```
DP[*][*] = -1;
DP[{j}][j] = j;
for i in 0..2^n:
  s = subset denoted by i
  if |s| < 2: continue                // we've initialized these already
  for j in 0..n:
    if j not in s: continue
    DP[i][j] = -1;
    for neighbor in prefd[j]:
      if neighbor not in s: continue
      if DP[s \ j][neighbor] >= 0:
        DP[i][j] = neighbor
        break;
```

## Randomized Solutions:
### Simple
```
clang randomized.cpp [-03 | -O2 | -O1 | -O0 |]
```
has various parameters that can be used such as a range of score to be searched in, amount of those, output to file etc.
Output is the arrangement.

### Randomized2wide:
```
clang randomized2wide.cpp [-03 | -O2 | -O1 | -O0 |]
```
same as Randomized, but this time the neighbours of the neighbours are valid for preference as well
