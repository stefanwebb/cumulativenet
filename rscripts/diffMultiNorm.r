## NOTE: This file is for checking that a single normal copula factor is differentiated correctly

library(mnormt)
library(bitops)

# Sample where fails
x = c(0.001, 0.017392401350662112, 0.96065282914787531)
#x = c(0.6, -0.2, 0.1)
#x = c(qnorm(0.960652829147875), qnorm(0.5))
#x = qnorm(x)

n = 3
#n = 2
rho = 0.965215
sigma = diag(n) + rho * (matrix(1, n, n) - diag(n))

# Function to convert subset index to subset
indexToSubset = function(x, n)
{
  result = c()
  for (i in 1:n) {
    if ((bitAnd(x, 2^(i-1))) > 0) {
      result = c(result, i)
    }
  }
  return(result)
}

partialDeriv = function(x, s, sigma)
{
  # If empty subset, then simply return CDF
  if (length(s) == 0) {
    return(pmnorm(x, rep(0, length(x)), sigma)[1])
  }
  
  # If full subset, then return density
  else if (length(s) == length(x)) {
    return(dmnorm(x, rep(0, length(x)), sigma)[1])
  }
  
  # Else return product
  else {
    sigma12 = sigma[-s, s]
    sigma11 = sigma[-s, -s]
    sigma22 = sigma[s, s]

    mu = sigma12 %*% solve(sigma22) %*% x[s]
    
    print(c("mu", s, mu))
        
    sigma = sigma11 - sigma12 %*% solve(sigma22) %*% sigma12
    
    #print(c("sigma", s, sqrt(sigma)))
    
    print(sigma12)
    print(sigma11)
    print(solve(sigma22))
    
    print(sigma)
    
    #if (length(s) == 2) {
    #  print(c("sigma", s, sqrt(sigma)))
    #}
    
    return(dmnorm(x[s], rep(0, length(s)), sigma22) * pmnorm(x[-s], mu, sigma)[1])
  }
}

#print(partialDeriv(x, c(1,2), sigma))

# Loop over subsets of variables
#vars = c("X1", "X2", "X3")
#vars = c("X1", "X2", "X3")
#for (i in 1:(2^length(x))) {
#  s = indexToSubset(i - 1, length(x))
#  print(c(vars[s], partialDeriv(x, s, sigma)))
#}

#x2 = c(0.001, 0.017392401350662112, 1, 0.96065282914787531)

# Values over Submodel
# ProductTransform = 2.0249493310345161
# ProductProbit = 9.8758509237323846
# CopulaDensity = 0.11235565525542995

# Cache Derivatives in passing from 1 to -1, over submodel clique tree
# Factor 1
# 0.24691975860184001
# 0
# 0.31568402012413965
#
# Factor 2
# 0.24691975860184001
# 0.31568402012413965
# 0
# 0

print(0.0826698 * 0.0792006 + 0.00558811 * 0.101257)
