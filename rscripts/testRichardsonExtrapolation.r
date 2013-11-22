# This script tests the indexing for Richardson extrapolation
M = 10
idx = function(i, j)
{
  return (j * (M + 1) - j * (j + 1) / 2 + i)
}

for (j in 1:5) {
  for (i in 1:(M - j + 1)) {
    print(idx(i - 1, j - 1))
  }
}