# This is a script that calculates the density of a simple network manually.
# The network has two factors. Both are over {a, b}. This results in a single clique.

library(mnormt)

# Define marginal parameters
mus = c(0, 0)
sigmas = c(1, 1)

# Define copula parameters
#corr1 = diag(2)
corr1 = diag(2)/2 + 1/2
corr2 = diag(2)*0.3 + 0.7

# Function for calculating derivative of probit
dProbit = function(x) { 1 / dnorm(qnorm(x)) }

# Sample
s = rep(0.1234, 2)
#s = c(0, 1, 2)

# Note: Invert, remove rows/columns corresponding to vars being conditioned upon, then reinvert
sigmaBgivenA1 = 1 / solve(corr1)[2,2]
sigmaAgivenB1 = 1 / solve(corr1)[1,1]
sigmaBgivenA2 = 1 / solve(corr2)[2,2]
sigmaAgivenB2 = 1 / solve(corr2)[1,1]

# Calculate factors
marginalFactor = prod(dnorm(s, mus, sqrt(sigmas)))
cdfVars = pnorm(s, mus, sqrt(sigmas))
transformFactor = 0.25 * prod(cdfVars ^ -0.5)
cdfVars = sqrt(cdfVars)
probitFactor = prod(dProbit(cdfVars))
copVars = qnorm(cdfVars)

# Parameters for conditional distributions
muBgivenA1 = corr1[1, 2] / corr1[1, 1] * (copVars[1])
muAgivenB1 = corr1[1, 2] / corr1[2, 2] * (copVars[2])
muBgivenA2 = corr2[1, 2] / corr2[1, 1] * (copVars[1])
muAgivenB2 = corr2[1, 2] / corr2[2, 2] * (copVars[2])

copulaFactor = pmnorm(copVars, rep(0, 2), corr1) * dmnorm(copVars, rep(0, 2), corr2) + pmnorm(copVars, rep(0, 2), corr2) * dmnorm(copVars, rep(0, 2), corr1) + dnorm(copVars[1]) * dnorm(copVars[2]) * (pnorm(copVars[1], muAgivenB1, sqrt(sigmaAgivenB1)) * pnorm(copVars[2], muBgivenA2, sqrt(sigmaBgivenA2)) + pnorm(copVars[2], muBgivenA1, sqrt(sigmaBgivenA1)) * pnorm(copVars[1], muAgivenB2, sqrt(sigmaAgivenB2)))

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