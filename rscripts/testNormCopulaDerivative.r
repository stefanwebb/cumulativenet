# This script tests that I am correctly calculating the partial derivatives of the derivative of the normal copula with respect to a single correlation parameter

library(mnormt)

# Bivariate case
rho = 0.810503
n = 2
x = c(-2.11085, 1.75831)

cov = diag(n) + rho * (matrix(1, n, n) - diag(n))
precisionZ = solve(cov) %*% x

den = dmnorm(x, varcov = cov)

print(c(den, -precisionZ[1] * den, -precisionZ[2] * den, den * (precisionZ[1] * precisionZ[2] - solve(cov)[1,2])))

print((precisionZ[1] * precisionZ[2] - solve(cov)[1,2]))

# Trivariate case
#rho = 0.9
#n = 3
#x = c(0.6, -0.2, 0.1)

#cov = diag(n) + rho * (matrix(1, n, n) - diag(n))
#precisionZ = solve(cov) %*% x

#den123 = dmnorm(x, varcov = cov)
#den01 = dmnorm(x[c(0, 1) + 1], varcov = cov[c(0, 1) + 1, c(0, 1) + 1])
#den02 = dmnorm(x[c(0, 2) + 1], varcov = cov[c(0, 2) + 1, c(0, 2) + 1])
#den12 = dmnorm(x[c(1, 2) + 1], varcov = cov[c(1, 2) + 1, c(1, 2) + 1])

#mu0given12 = cov[1, c(1, 2) + 1] %*% solve(cov[c(1, 2) + 1, c(1, 2) + 1]) %*% x[c(1, 2) + 1]
#mu1given02 = cov[2, c(0, 2) + 1] %*% solve(cov[c(0, 2) + 1, c(0, 2) + 1]) %*% x[c(0, 2) + 1]
#mu2given01 = cov[3, c(0, 1) + 1] %*% solve(cov[c(0, 1) + 1, c(0, 1) + 1]) %*% x[c(0, 1) + 1]

#sd0given12 = sqrt(solve(solve(cov)[1, 1]))
#sd1given02 = sqrt(solve(solve(cov)[2, 2]))
#sd2given01 = sqrt(solve(solve(cov)[3, 3]))

#prob0given12 = pnorm(x[1], mu0given12, sd0given12)
#prob1given02 = pnorm(x[2], mu1given02, sd1given02)
#prob2given01 = pnorm(x[3], mu2given01, sd2given01)

#factor0 = -precisionZ[1]
#factor1 = -precisionZ[2]
#factor2 = -precisionZ[3]
#factor01 = precisionZ[1] * precisionZ[2] - solve(cov)[1, 2]
#factor02 = precisionZ[1] * precisionZ[3] - solve(cov)[1, 3]
#factor12 = precisionZ[2] * precisionZ[3] - solve(cov)[2, 3]

#diff012 = den123
#diff01 = den01 * prob2given01
#diff02 = den02 * prob1given02
#diff12 = den12 * prob0given12

#subsetEmpty = diff01 + diff02 + diff12
#subset0 = factor0 * diff01 + factor0 * diff02 + diff012
#subset1 = factor1 * diff01 + diff012 + factor1 * diff12
#subset01 = factor01 * diff01 + factor0 * diff012 + factor1 * diff012
#subset2 = diff012 + factor2 * diff02 + factor2 * diff12
#subset02 = factor0 * diff012 + factor02 * diff02 + factor2 * diff012
#subset12 = factor1 * diff012 + factor2 * diff012 + factor12 * diff12
#subset012 = factor01 * diff012 + factor02 * diff012 + factor12 * diff012
  
# Print used subsets
#print(c("X12", diff01))
#print(c("X13", diff02))
#print(c("X23", diff12))
#print(c("X123", diff012))

# Print partial derivatives of derivative
#print(c(subsetEmpty, subset0, subset1, subset01, subset2, subset02, subset12, subset012))