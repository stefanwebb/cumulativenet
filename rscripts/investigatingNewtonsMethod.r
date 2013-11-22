condcdf = scan("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/condcdf.txt")
x = seq(0, 1, 0.001)
plot(x, condcdf - 0.15066687809303403, type = "l", xlab = "u", ylab = "g(u)", main = "Finding Root of g(u)")
abline(h = 0, lty = 3)
#abline(v = 0.140625, lty = 2)
#abline(v = 0.125, lty = 2)

# Todo: improve titles, put on tangents...

drawTangent = function(i) {
  m = (condcdf[i] - condcdf[i - 1]) / (x[i] - x[i - 1])
  yint = (condcdf[i] - 0.15066687809303403) - m * x[i]
  xint = -yint / m
  abline(coef = c(yint, m), lty = 2)
}

points(x[291], condcdf[291] - 0.15066687809303403)

drawTangent(291)
drawTangent(30)
drawTangent(1001 - 291)