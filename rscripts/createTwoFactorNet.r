# This is a script that calculates the density of a simple network manually.
# The network has two factors. One is over {a, b} and the other is over {b, c}.
# This results in two cliques in the clique tree, each with factor.

# Define marginal parameters
mus = c(0, 0, 0)
sigmas = c(1, 1, 1)

# Define copula parameters
#corr1 = diag(2)
corr1 = diag(2)/2 + 1/2
corr2 = diag(2)*0.3 + 0.7

# Function for calculating derivative of probit
dProbit = function(x) { 1 / dnorm(qnorm(x)) }

# Sample
s = rep(0.1234, 3)
#s = c(0, 1, 2)

# Parameters for conditional distributions
muBgivenA = mus[2] + corr1[1, 2] / corr1[1, 1] * (s[1] - mus[1])
muBgivenC = mus[2] + corr2[1, 2] / corr2[2, 2] * (s[3] - mus[3])

# Note: Invert, remove rows/columns corresponding to vars being conditioned upon, then reinvert
sigmaBgivenA = 1 / solve(corr1)[2,2]
sigmaBgivenC = 1 / solve(corr2)[1,1]

# Calculate factors
marginalFactor = prod(dnorm(s, mus, sqrt(sigmas)))
cdfVars = pnorm(s, mus, sqrt(sigmas))
transformFactor = 0.5 * cdfVars[2] ^ -0.5
cdfVars[2] = sqrt(cdfVars[2])
probitFactor = prod(dProbit(cdfVars))
copVars = qnorm(cdfVars)
copulaFactor = dnorm(copVars[1]) * dnorm(copVars[3]) * (dnorm(copVars[2], muBgivenA, sqrt(sigmaBgivenA)) * pnorm(copVars[2], muBgivenC, sqrt(sigmaBgivenC)) + pnorm(copVars[2], muBgivenA, sqrt(sigmaBgivenA)) * dnorm(copVars[2], muBgivenC, sqrt(sigmaBgivenC)))

answer = marginalFactor * transformFactor * probitFactor * copulaFactor
print("Marginal Factor")
print(marginalFactor)
print("Transform Factor")
print(transformFactor)
print("Probit Factor")
print(probitFactor)
print("Copula Factor")
print(copulaFactor)
print("Answer")
print(answer)