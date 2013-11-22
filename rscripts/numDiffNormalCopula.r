## This script checks that numerical derivatives of normal copula match formulae
library(mnormt)
library(bitops)
library(numDeriv)
library(tmvtnorm)

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

# Function to calculate normal density and CDF of some variables given others
partialDeriv = function(x, s, sigma)
{
  if (class(x) == "matrix") {
    if (length(s) == 0) {
      return(apply(x, MARGIN = 1, pmnorm, mean = rep(0, dim(x)[2]), varcov = sigma))
    }
    
    else if (length(s) == dim(x)[2]) {
      return(dmnorm(x, rep(0, dim(x)[2]), sigma))
    }
    
    else {
      sigma12 = sigma[-s, s]
      sigma11 = sigma[-s, -s]
      sigma22 = sigma[s, s]
      
      mu = sigma12 %*% solve(sigma22) %*% t(x[,s])
      
      sigma = sigma11 - sigma12 %*% solve(sigma22) %*% sigma12
      
      if (class(x[,-s]) == "matrix") {
        return(dmnorm(x[,s], rep(0, length(s)), sigma22) * apply(x[,-s], MARGIN = 1, pmnorm, mean = mu, varcov = sigma))
      }
      
      else {
        return(dmnorm(x[,s], rep(0, length(s)), sigma22) * pmnorm(x[,-s], mu, sigma))
      }
    }
  }
  
  else {
    if (length(s) == 0) {
      return(pmnorm(x, rep(0, length(x)), sigma)[1])
    }
    
    else if (length(s) == length(x)) {
      return(dmnorm(x, rep(0, length(x)), sigma))
    }
    
    else {
      sigma12 = sigma[-s, s]
      sigma11 = sigma[-s, -s]
      sigma22 = sigma[s, s]
      
      mu = sigma12 %*% solve(sigma22) %*% x[s]
      
      sigma = sigma11 - sigma12 %*% solve(sigma22) %*% sigma12
      
      return(dmnorm(x[s], rep(0, length(s)), sigma22) * pmnorm(x[-s], mu, sigma))
    }
  }
}

partialDerivSingle = function(x, s1, s2, sigma)
{
  if (length(s1) == length(x) && length(s2) == 1) {
    return(dmnorm(x, rep(0, length(x)), sigma) * -(solve(sigma) %*% x)[s2[1]])
  }
    
  else if (length(s2) == 1 && length(s1) > 0) {
    sigma12 = sigma[-s1, s1]
    sigma11 = sigma[-s1, -s1]
    sigma22 = sigma[s1, s1]
      
    mu = sigma12 %*% solve(sigma22) %*% x[s1]
      
    sigmaNew = sigma11 - sigma12 %*% solve(sigma22) %*% sigma12
    
    #print("mu")
    #print(as.vector(mu))
    
    #print("y")
    #print(x[-s1])
    
    #print("sigmaNew")
    #print(sigmaNew)
    
    # Calculate the means of the truncated normal distribution of the undifferentiated variables
    truncMeans = mtmvnorm(as.vector(mu), sigmaNew, upper = x[-s1], doComputeVariance = FALSE)$tmean
    
    print("truncMeans")
    print(truncMeans)
    
    term0 = dmnorm(x[s1], rep(0, length(s1)), sigma22) * pmnorm(x[-s1], mu, sigmaNew)
      
    term1 = -term0 * (solve(sigma)[s2[1],s1] %*% x[s1])
    term2 = -term0 * (solve(sigma)[s2[1],-s1] %*% truncMeans)
    
    print("answer")
    print(term1 + term2)
    return(term1 + term2)
  }
  
  else {
    print("Case in partialDerivSingle not implemented!")
  }
}

# Values for covariance
#n = 4
n = 4
#rho = -0.965215
#rho = -0.473911
rho = -0.310143
cov = diag(n) + rho * (matrix(1, n, n) - diag(n))
#u = c(0.2, 0.6, 0.4, 0.1)
#x = qnorm(u)
#x = c(1.75831, -1.64222)
#x = c(-1.25248, -0.648284, 1.75831)
x = c(-1.19603, 1.75831, -0.683373, -0.345597)

# Loop over subsets of variables
vars = c("X1", "X2", "X3", "X4")
#vars = c("X1", "X2", "X3")
print("Partial Derivatives")
for (i in 1:(2^length(x))) {
  s = indexToSubset(i - 1, length(x))
  print(c(vars[s], log(partialDeriv(x, s, cov))))
}

print("Partial Derivatives w.r.t. rho")
for (i in 1:(2^length(x))) {
  s = indexToSubset(i - 1, length(x))
  thisDeriv = function(rho) {
    cov = diag(n) + rho * (matrix(1, n, n) - diag(n))
    return(partialDeriv(x, s, cov))
  }
  
  h = grad(thisDeriv, rho, method.args=list(eps=1e-4, d=0.00001, zero.tol=sqrt(.Machine$double.eps/7e-7), r=6, v=2, show.details=FALSE))
  
  print(c(vars[s], h))
}

#c1 = (rho * x[2] - x[1]) / (1 - rho^2)
#c2 = (rho * x[1] - x[2]) / (1 - rho^2)
#c3 = ((rho * x[2] - x[1]) * (rho * x[1] - x[2]) + rho*(1 - rho^2)) / (1 - rho^2)^2

#print(partialDerivSingle(x, c(1, 2), c(1), cov))
#print(partialDerivSingle(x, c(1, 3), c(1), cov))
#print(partialDeriv(x, c(1, 2, 3), cov))

#print(c1 * dmnorm(x, c(0, 0), cov))
#print(c2 * dmnorm(x, c(0, 0), cov))
#print(-0.4938372 * dmnorm(x, c(0, 0), cov))