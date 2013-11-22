# Experimenting with how to validly parameterize a normal factor

rho = -0.25
n = 5
cov = diag(n) + rho * (matrix(1, n, n) - diag(n))

#print(eigen(cov)$values)

n = 6
print(eigen(2 * matrix(1, n, n))$values)