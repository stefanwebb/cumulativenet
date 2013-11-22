# Testing my formula for the gradient of the Clayton copula
library(numDeriv)
library(bitops)

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

gradClayton = function(u, s, theta)
{
  n = length(u)
  d = length(s)
  
  # Breaking up formula into terms, mainly so can make debugging easier (compare individual terms in C++)
  if (d > 0) {
    firstTerm = -sum(1 / (theta * (1 + theta * (0:(d-1)))))
    secondTerm = -sum(log(u[s]))
  }
  else {
    firstTerm = 0
    secondTerm = 0
  }
    
  thirdTerm = d / theta
  fourthTerm = 1 / theta^2 * log(sum(u^-theta) - n + 1)
  fifthTerm = (1/theta + d) * (sum(log(u) * u^-theta)) / (sum(u^-theta) - n + 1)
  
  #print(c("firstTerm", firstTerm))
  #print(c("secondTerm", secondTerm))
  #print(c("thirdTerm", thirdTerm))
  #print(c("fourthTerm", fourthTerm))
  #print(c("fifthTerm", fifthTerm))
  
  return ((firstTerm + secondTerm + thirdTerm + fourthTerm + fifthTerm) * derivClayton(u, s, theta))
}

derivClayton = function(u, s, theta)
{
  n = length(u)
  d = length(s)
  if (d > 0) {
    firstTerm = prod(1/theta + (0:(d-1)))
  }
  else {
    firstTerm = 1
  }
  
  return (firstTerm * prod(u[s]^-theta)^(1/theta + 1) * theta^d * (sum(u^-theta) - n + 1)^-(1/theta + d))
}

gradClayton2 = function(u, s, theta)
{
  thisDeriv = function(theta) { derivClayton(u, s, theta) }
  h = grad(thisDeriv, theta, method.args=list(eps=1e-4, d=0.00001, zero.tol=sqrt(.Machine$double.eps/7e-7), r=6, v=2, show.details=FALSE)) #
  return(h)
}

logClaytonDensity = function(u, theta)
{
  # Breaking up formula into terms, mainly so can make debugging easier (compare individual terms in C++)
  n = length(u)
  firstTerm = sum(log(1/theta + 0:(n-1)))
  secondTerm = -(1 + theta) * sum(log(u))
  thirdTerm = n * log(theta)
  fourthTerm = -(1 / theta + n) * log(sum(u^-theta) - n + 1)
  
  return (firstTerm + secondTerm + thirdTerm + fourthTerm)
}

u = c(0.2, 0.5, 0.7, 0.2)
theta = 5

den = dCopula(u, claytonCopula(param = theta, dim = length(u)))

#print(c(exp(logClaytonDensity(u, theta)), den))

g = function(theta)
{
  n = length(u)
  #firstTerm = sum(log(1/theta + 0:(n-1)))
  #secondTerm = -(1 + theta) * sum(log(u))
  #thirdTerm = n * log(theta)
  #fourthTerm = -(1 / theta + n) * log(sum(u^-theta) - n + 1)
  
  dCopula(u, claytonCopula(param = theta, dim = length(u)))
}

gDif = function(theta)
{
  n = length(u)
  #firstTerm = -sum(1 / (theta * (1 + theta * (0:(n-1)))))
  #secondTerm = -sum(log(u[s]))
  #thirdTerm = n / theta
  #fourthTerm = 1 / theta^2 * log(sum(u^-theta) - n + 1)
  #fifthTerm = (1/theta + n) * (sum(log(u) * u^-theta)) / (sum(u^-theta) - n + 1)
  #return (fourthTerm + fifthTerm)
  
  # Breaking up formula into terms, mainly so can make debugging easier (compare individual terms in C++)
  firstTerm = -sum(1 / (theta * (1 + theta * (0:(n-1)))))
  secondTerm = -sum(log(u[s]))
  thirdTerm = n / theta
  fourthTerm = 1 / theta^2 * log(sum(u^-theta) - n + 1)
  fifthTerm = (1/theta + n) * (sum(log(u) * u^-theta)) / (sum(u^-theta) - n + 1)
  
  return (firstTerm + secondTerm + thirdTerm + fourthTerm + fifthTerm)
}

#print(gDif(theta) * g(theta))
#print(grad(g, theta))

#print(gDif(theta))
#print(g(theta))
#print(gradClayton(u, c(1, 2), theta))

#print(log(derivClayton(u, c(1), theta)))
#rint(gradClayton(u, c(1), theta))

 print("Partial Derivatives")
 vars = c("U1", "U2", "U3", "U4")
 for (i in 1:(2^length(u))) {
   s = indexToSubset(i - 1, length(u))
   print(c(vars[s], log(derivClayton(u, s, theta))))
 }
 
 print("Formula Gradient")
 for (i in 1:(2^length(u))) {
   s = indexToSubset(i - 1, length(u))
   print(c(vars[s], gradClayton(u, s, theta)))
 }
  
 print("Numerical Gradient")
 for (i in 1:(2^length(u))) {
   s = indexToSubset(i - 1, length(u))
   print(c(vars[s], gradClayton2(u, s, theta)))
 }