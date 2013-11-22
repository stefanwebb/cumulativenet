# This script creates figures for examples in learning 
path = "C:\\Users\\Stefan\\Documents\\Visual Studio 2012\\Projects\\honours\\Cwd\\experiments\\"
setwd(path)

library(mnormt)

# Output from my learn marginals procedure
# Mean0 = 0.0016943467285658092
# Sd0 = 1.0171318837218919
# Mean1 = 0.0031116424876826928
# Sd1 = 0.99370483849629565
# Mean2 = -0.012929111658082812
# Sd2 = 0.99307273337619817
#print(sd(s))

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

n = 2

# Load sample
smp = read.table("samplesLearnEx2.txt", header = TRUE)
X1 = smp$X1
X2 = smp$X2

U1 = pnorm(X1, mean(X1), sd(X1))
U2 = pnorm(X2, mean(X2), sd(X2))

V1 = sqrt(U1)
V2 = sqrt(U2)

W1 = qnorm(V1)
W2 = qnorm(V2)

logLikelihood = function(x)
{
  rho1 = x[1]
  rho2 = x[2]
  
  cov1 = diag(n) + rho1 * (matrix(1, n, n) - diag(n))
  cov2 = diag(n) + rho2 * (matrix(1, n, n) - diag(n))
  
  f1 = partialDeriv(cbind(W1, W2), c(1, 2), cov1)
  f2 = partialDeriv(cbind(W1, W2), c(1, 2), cov2)
  
  F1 = partialDeriv(cbind(W1, W2), c(), cov1)
  F2 = partialDeriv(cbind(W1, W2), c(), cov2)

  dF1dW1 = partialDeriv(cbind(W1, W2), c(1), cov1)
  dF1dW2 = partialDeriv(cbind(W1, W2), c(2), cov1)
  
  dF2dW1 = partialDeriv(cbind(W1, W2), c(1), cov2)
  dF2dW2 = partialDeriv(cbind(W1, W2), c(2), cov2)

  den = (f1 * F2 + dF1dW1 * dF2dW2 + dF1dW2 * dF2dW1 + F1 * f2)
  
  #print(which(log(den) == min(log(den))))
  #print(den[2716])
  
  #print(dF2dW1)
  
  #precisionZ1 = solve(cov1) %*% rbind(W1, W2)
  #precisionZ2 = solve(cov2) %*% rbind(W2, W3)

  #df1dW1dW2 = f1 * (precisionZ1[1,] * precisionZ1[2,] - solve(cov1)[1,2])
  #df2dW2dW3 = f2 * (precisionZ2[1,] * precisionZ2[2,] - solve(cov2)[1,2])
                  
  #df1dW1 = f1 * -precisionZ1[1,]
  #df2dW3 = f2 * -precisionZ2[2,]

  #dfdRho1 = C * (df1dW1dW2 * dF2dW3 + df1dW1 * f2)
  #dfdRho2 = C * (f1 * df2dW3 + dF1dW1 * df2dW2dW3)

  #grad = cbind((df1dW1dW2 * dF2dW3 + df1dW1 * f2), (f1 * df2dW3 + dF1dW1 * df2dW2dW3))
  
  #dlnfdRho1 = dfdRho1 / f
  #dlnfdRho2 = dfdRho2 / f

  #logGrad = c(sum(dlnfdRho1), sum(dlnfdRho2))
  
  #print(-log(den)[43]/10000)
  #print(den[43])
  #print(c(W1[43], W2[43]))
  
  logLike = sum(log(den)) / length(X1)
  return (logLike)
}

rhoGrid = function(n)
{
  p = seq(-0.99, 0.99, 2 * 0.99 / (n - 1))
  #q = seq(1, -1, 2 / (n - 1))
  
  return (expand.grid(p, p))
}

z = scan(file = "gridLearnEx2.txt")
z = matrix(z, sqrt(length(z)), sqrt(length(z)))

plotContours = function(lab)
{
  n = sqrt(length(z))
  x = seq(-0.99, 0.99, 2 * 0.99 / (n - 1))
  y = seq(-0.99, 0.99, 2 * 0.99 / (n - 1))
  #z = matrix(apply(as.matrix(rhoGrid(n)), 1, function(x) {-logLikelihood(x)}), n, n)
  
  #print(z)
  
  #z = apply(as.matrix(rhoGrid(n)), 1, logLikelihood)
  contour(x, y, z, xlab = expression(rho[1]), ylab = expression(rho[2]), main = lab, levels = c(2.445, 2.455, 2.45, 2.46, 2.48, 2.4425, 2.443, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.3, 3.6, 4, 5, 6, 7), col = gray(0.5))
  #filled.contour(x, y, z)
  
  #print(x)
  #print(y)
  #print(z)
}

logSpaced = function(a, b, n)
{
  return (a * exp(log(b / a) * (((1:n) - 1) / (n - 1))))
  return (((((1:n) / 5)^10 - 1) / (b - a)) + a)
}

printSolution = function(fileName)
{
  prm = read.table(fileName, header = TRUE)
  rho1 = prm$rho0[1:500]
  rho2 = prm$rho1[1:500]
  type = prm$type[1:500]
  
  points(rho1[type == "L"], rho2[type == "L"], pch = 16) # 1
  points(rho1[type == "R"], rho2[type == "R"], pch = 1) # 4
  points(rho1[type == "N"], rho2[type == "N"], pch = 4)  
  
  #lines(rho1, rho2, type = "c", lty = 2, col = colour)
}

#rho = c(-0.998004853725433, 0.25746844522655)

#W1 = W1[1]
#W2 = W2[2]

#print(-logLikelihood(rho)/10000)

#print(-logLikelihood(c(-0.99, -0.99)))

  par(mfrow=c(1,2))
  plotContours("Gradient Descent")
  printSolution("resultsLearnEx2graddescent.txt")

  plotContours("L-BFGS with Barrier Method")
  printSolution("resultsLearnEx2bfgs.txt")
  par(mfrow=c(1,1))