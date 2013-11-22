# Testing the example of a reparameterization I give
a1 = 1
a2 = 1
a3 = -1

Sigma = cbind(c(1, a1 * a2, a1 * a3), c(a1 * a2, 1, a2 * a3), c(a1 * a3, a2 * a3, 1))
print(eigen(Sigma)$values)

#rho = -1
#n = 3
#cov = diag(n) + rho * (matrix(1, n, n) - diag(n))
#print(cov)
#print(eigen(cov)$values)