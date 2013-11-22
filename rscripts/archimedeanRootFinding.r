## The purpose of this script is to try Archimedean sampling by hand to see why numerically unstable
theta = 100
phi0 = function(t) {(t * theta + 1.0) ^ (-1.0 / theta)}
phi1 = function(t) {-(t * theta + 1.0) ^ (-1.0 / theta - 1.0)}
invphi = function(t) {(t^(-theta)-1.0)/theta}

phi1invphi = function(t) { -((t^(-theta)-1.0) + 1.0) ^ (-1.0 / theta - 1.0) }
phi1invphi2 = function(t) { -t^(1 + theta) }
logPhi1invphi = function(t) { (1 + theta) * log(t) }
logPhi1invphi2 = function(t) { log(-phi1invphi2(t)) }

k = 0.51953080273233354
#u1 = 4.4316984713077545e-006
u1 = 0.0000001

#u1 = 0.017392401350662112
#u2 = 0.96065282914787531

g = function(u2) {phi1(invphi(u1) + invphi(u2)) / phi1(invphi(u1)) - k }
g2 = function(u2) {phi1(invphi(u1) + invphi(u2)) / phi1(invphi(u1))}  #/ phi1(invphi(u1)) - k

logSumExp = function(x)
{
  x <- sort(x) # mainly so max(x)==x[length(x)]
  n <- length(x)
  log1p(sum(exp(x[-n] - x[n]))) + x[n]
}

# This approach doesn't work!
gStable = function(u2)
{
  logU1 = rep(-theta * log(u1), length(u2))
  logU2 = -theta * log(u2)
  
  #maxU = max(logU1, logU2)
  #minU = min(logU1, logU2)
  
  print(c(logU1, logU2))
  
  #logSumExp = maxU + log1p(exp(minU - maxU))
  
  logSum = logSumExp(c(logU1, logU2, 0))
  
  print(logSum == logU1)
  
  print(logSum - logU1)
  
  #return(logSumExp)
}

# Trying another tact
gStable2 = function(u2)
{
  return (-(1/theta + 1) * log(1 + (u1 / u2)^theta - u1^theta))
}

g3 = function(u3)
{
  #return ((theta * (1/theta + 1) * (u1^-theta + u2^-theta + u3^-theta - 2)^(-1/theta - 2)) / (u1^-theta * u2^-theta)^(-1/theta - 1))
  return (((u1^-theta + u2^-theta + u3^-theta - 2) / (u1^-theta + u2^-theta - 1))^(-1/theta - 2))
}

#print(g(0.5))

#u2 = seq(0, 1, 0.01)
#plot(u2, sapply(u2, gStable))
#gStable(0.2)

#plot(u2, exp(sapply(u2, gStable2)), type = "l")
#plot(u2, sapply(u2, g2), type = "l")

#print(phi1(invphi(u1)) == phi1invphi(u1))

g5 = function(u1, u2)
{
  #1 / (u1^theta + u2^theta - (u1 * u2)^theta)
  1 / (u1^-theta + u2^-theta - 1)
}

g4 = function(u3)
{
  #1 / (u1^theta + u2^theta - (u1 * u2)^theta)
  return ((1 + 1 / ((u3/u1)^theta + (u3/u2)^theta - u3^theta) - 1 / (u1^-theta + u2^-theta - 1))^(-1/theta - 2))
}

g1 = function(u1, u2)
{
  return (log(u1^-theta + u2^-theta - 1))
}

g2 = function(u1, u2)
{
  minU = min(u1, u2)
  maxU = max(u1, u2)
  
  print(c("numerator", (minU/maxU)^theta))
  
  return (log1p((minU/maxU)^theta - minU^theta) - theta * log(min(u1, u2)))
}

theta = 10

print(g1(0.7, 0.8))
print(g2(0.7, 0.8))

u1 = 0.0001
u2 = 0.0001
u3 = seq(0, 1, 0.01)
#plot(u3, sapply(u3, g4), type = "l")

#print(c(g3(0.1), g4(0.0001)))