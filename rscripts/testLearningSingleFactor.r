# This script tests the gradient ascent algorithm

smp = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/factor.txt", header = TRUE)
library(mnormt)

# Calculate log-likelihood
logLikelihood = function(x, rho)
{
  n = 2
  cov = diag(n) + rho * (matrix(1, n, n) - diag(n))
  den = dmnorm(x, varcov = cov) # Calculate all densities
  logLike = sum(log(den))
  return (logLike)
}

# Calculate log-gradient
logGradient = function(x, rho)
{
  n = 2
  cov = diag(n) + rho * (matrix(1, n, n) - diag(n))
  precisionZ = solve(cov) %*% rbind(x[,1], x[,2]) # Calculate precision times parameter vector matrix
  logGrad = sum(precisionZ[1,] * precisionZ[2,] - solve(cov)[1,2])
  return (logGrad)
}

#print(c(den, -precisionZ[1] * den, -precisionZ[2] * den, den * (precisionZ[1] * precisionZ[2] - solve(cov)[1,2])))

# Empirical covariance (biased MLE estimate)
rhohat = cor(smp$X1, smp$X2) * (length(smp$X1) - 1) / length(smp$X1)

# Initial random rho
rho = 0.692837

print(c("log-Likelihood", logLikelihood(smp, rho), "log-gradient", logGradient(smp, rho)))
print(c("log-Likelihood", logLikelihood(smp, rhohat), "log-gradient", logGradient(smp, rhohat)))

