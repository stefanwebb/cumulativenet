# Script to plot figures for inference experiments
readVals = function(fileName) {
  #workingDir = "C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/experiments/"
  workingDir = "c:/experiments/"
  vals = scan(paste(workingDir, fileName, sep = ""))
}

vals = readVals("inferenceNormalChain4Samples10000Repeats100.txt")
hist(vals)
print(summary(vals))
#plot(1, smp$X2, xlab = "u", ylab = "v", main = bquote(theta == .(r)))