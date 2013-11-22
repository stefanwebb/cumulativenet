path = "C:\\Users\\Stefan\\Documents\\Visual Studio 2012\\Projects\\honours\\Cwd\\experiments\\"

## Read Data

normalChain2 = read.table(paste(path, "inferenceNormalChain2_1.txt", sep =""))
normalChain3 = read.table(paste(path, "inferenceNormalChain3.txt", sep =""))
normalChain4 = read.table(paste(path, "inferenceNormalChain4.txt", sep =""))
normalChain5 = read.table(paste(path, "inferenceNormalChain5.txt", sep =""))
normalChain6 = read.table(paste(path, "inferenceNormalChain6.txt", sep =""))
normalChain7 = read.table(paste(path, "inferenceNormalChain7.txt", sep =""))
normalChain8 = read.table(paste(path, "inferenceNormalChain8.txt", sep =""))
normalChain9 = read.table(paste(path, "inferenceNormalChain9.txt", sep =""))
normalChain10 = read.table(paste(path, "inferenceNormalChain10.txt", sep =""))
normalChain11 = read.table(paste(path, "inferenceNormalChain11.txt", sep =""))
normalChain12 = read.table(paste(path, "inferenceNormalChain12.txt", sep =""))
normalChain13 = read.table(paste(path, "inferenceNormalChain13.txt", sep =""))
normalChain14 = read.table(paste(path, "inferenceNormalChain14.txt", sep =""))
normalChain15 = read.table(paste(path, "inferenceNormalChain15.txt", sep =""))

normalChainMeans = c(mean(normalChain2[,1]), mean(normalChain3[,1]), mean(normalChain4[,1]), mean(normalChain5[,1]), mean(normalChain6[,1]), mean(normalChain7[,1]), mean(normalChain8[,1]), mean(normalChain9[,1]), mean(normalChain10[,1]), mean(normalChain11[,1]), mean(normalChain12[,1]), mean(normalChain13[,1]), mean(normalChain14[,1]), mean(normalChain15[,1]))
normalChainMeansLogSpace = c(mean(normalChain2[,2]), mean(normalChain3[,2]), mean(normalChain4[,2]), mean(normalChain5[,2]), mean(normalChain6[,2]), mean(normalChain7[,2]), mean(normalChain8[,2]), mean(normalChain9[,2]), mean(normalChain10[,2]), mean(normalChain11[,2]), mean(normalChain12[,2]), mean(normalChain13[,2]), mean(normalChain14[,2]), mean(normalChain15[,2]))

normalLoop3 = read.table(paste(path, "inferenceNormalLoop3.txt", sep =""))
normalLoop4 = read.table(paste(path, "inferenceNormalLoop4.txt", sep =""))
normalLoop5 = read.table(paste(path, "inferenceNormalLoop5.txt", sep =""))
normalLoop6 = read.table(paste(path, "inferenceNormalLoop6.txt", sep =""))
normalLoop7 = read.table(paste(path, "inferenceNormalLoop7.txt", sep =""))
normalLoop8 = read.table(paste(path, "inferenceNormalLoop8.txt", sep =""))
normalLoop9 = read.table(paste(path, "inferenceNormalLoop9.txt", sep =""))
normalLoop10 = read.table(paste(path, "inferenceNormalLoop10.txt", sep =""))
normalLoop11 = read.table(paste(path, "inferenceNormalLoop11.txt", sep =""))
normalLoop12 = read.table(paste(path, "inferenceNormalLoop12.txt", sep =""))
normalLoop13 = read.table(paste(path, "inferenceNormalLoop13.txt", sep =""))
normalLoop14 = read.table(paste(path, "inferenceNormalLoop14.txt", sep =""))
normalLoop15 = read.table(paste(path, "inferenceNormalLoop15.txt", sep =""))

normalLoopMeans = c(mean(normalLoop3[,1]), mean(normalLoop4[,1]), mean(normalLoop5[,1]), mean(normalLoop6[,1]), mean(normalLoop7[,1]), mean(normalLoop8[,1]), mean(normalLoop9[,1]), mean(normalLoop10[,1]), mean(normalLoop11[,1]), mean(normalLoop12[,1]), mean(normalLoop13[,1]), mean(normalLoop14[,1]), mean(normalLoop15[,1]))
normalLoopMeansLogSpace = c(mean(normalLoop3[,2]), mean(normalLoop4[,2]), mean(normalLoop5[,2]), mean(normalLoop6[,2]), mean(normalLoop7[,2]), mean(normalLoop8[,2]), mean(normalLoop9[,2]), mean(normalLoop10[,2]), mean(normalLoop11[,2]), mean(normalLoop12[,2]), mean(normalLoop13[,2]), mean(normalLoop14[,2]), mean(normalLoop15[,2]))

normalTree3 = read.table(paste(path, "inferenceNormalTree3.txt", sep =""))
normalTree4 = read.table(paste(path, "inferenceNormalTree4.txt", sep =""))
normalTree5 = read.table(paste(path, "inferenceNormalTree5.txt", sep =""))
normalTree6 = read.table(paste(path, "inferenceNormalTree6.txt", sep =""))
normalTree7 = read.table(paste(path, "inferenceNormalTree7.txt", sep =""))
normalTree8 = read.table(paste(path, "inferenceNormalTree8.txt", sep =""))
normalTree9 = read.table(paste(path, "inferenceNormalTree9.txt", sep =""))
normalTree10 = read.table(paste(path, "inferenceNormalTree10.txt", sep =""))
normalTree11 = read.table(paste(path, "inferenceNormalTree11.txt", sep =""))
normalTree12 = read.table(paste(path, "inferenceNormalTree12.txt", sep =""))
normalTree13 = read.table(paste(path, "inferenceNormalTree13.txt", sep =""))

normalTreeMeans = c(mean(normalTree3[,1]), mean(normalTree4[,1]), mean(normalTree5[,1]), mean(normalTree6[,1]), mean(normalTree7[,1]), mean(normalTree8[,1]), mean(normalTree9[,1]), mean(normalTree10[,1]), mean(normalTree11[,1]), mean(normalTree12[,1]), mean(normalTree13[,1]))
normalTreeMeansLogSpace = c(mean(normalTree3[,2]), mean(normalTree4[,2]), mean(normalTree5[,2]), mean(normalTree6[,2]), mean(normalTree7[,2]), mean(normalTree8[,2]), mean(normalTree9[,2]), mean(normalTree10[,2]), mean(normalTree11[,2]), mean(normalTree12[,2]), mean(normalTree13[,2]))

normalGrid2 = read.table(paste(path, "inferenceNormalGrid2.txt", sep =""))
normalGrid3 = read.table(paste(path, "inferenceNormalGrid3.txt", sep =""))
normalGrid4 = read.table(paste(path, "inferenceNormalGrid4.txt", sep =""))
normalGrid5 = read.table(paste(path, "inferenceNormalGrid5.txt", sep =""))
normalGrid6 = read.table(paste(path, "inferenceNormalGrid6.txt", sep =""))
normalGrid7 = read.table(paste(path, "inferenceNormalGrid7.txt", sep =""))
normalGrid8 = read.table(paste(path, "inferenceNormalGrid8.txt", sep =""))
normalGrid9 = read.table(paste(path, "inferenceNormalGrid9.txt", sep =""))
normalGrid10 = read.table(paste(path, "inferenceNormalGrid10.txt", sep =""))

normalGridMeans = c(mean(normalGrid2[,1]), mean(normalGrid3[,1]), mean(normalGrid4[,1]), mean(normalGrid5[,1]), mean(normalGrid6[,1]), mean(normalGrid7[,1]), mean(normalGrid8[,1]), mean(normalGrid9[,1]), mean(normalGrid10[,1]))
normalGridMeansLogSpace = c(mean(normalGrid2[,2]), mean(normalGrid3[,2]), mean(normalGrid4[,2]), mean(normalGrid5[,2]), mean(normalGrid6[,2]), mean(normalGrid7[,2]), mean(normalGrid8[,2]), mean(normalGrid9[,2]), mean(normalGrid10[,2]))

maxMean = max(c(normalChainMeans, normalLoopMeans))
maxMean2 = max(c(normalGridMeans, normalTreeMeans))

par(mar = c(5.1, 4.1, 2.1, 2.1))

## Normal Chains

plot(rep(1:length(normalChainMeans), 2) + 2, 1 / c(normalChainMeans, normalChainMeansLogSpace), type = "n", xlab = "n", ylab = "seconds per sample", main = "Normal Chains")
lines((1:length(normalChainMeans)) + 2, 1 / normalChainMeans, type = "b")
lines((1:length(normalChainMeansLogSpace)) + 2, 1 / normalChainMeansLogSpace, type = "b", pch = 4, lty = 2)

## Normal Loops

plot(rep(1:length(normalLoopMeans) + 2, 2), 1 / c(normalLoopMeans, normalLoopMeansLogSpace), type = "n", xlab = "n", ylab = "seconds per sample", main = "Normal Loops")
lines((1:length(normalLoopMeans)) + 2, 1 / normalLoopMeans, type = "b")
lines((1:length(normalLoopMeansLogSpace)) + 2, 1 / normalLoopMeansLogSpace, type = "b", pch = 4, lty = 2)

## Normal Trees

plot(2^(rep(1:length(normalTreeMeans), 2) + 2) - 1, 1 / c(normalTreeMeans, normalTreeMeansLogSpace), type = "n", xlab = "n", ylab = "seconds per sample", main = "Normal Trees")
lines(2^((1:length(normalTreeMeans)) + 2) - 1, 1 / normalTreeMeans, type = "b")
lines(2^((1:length(normalTreeMeansLogSpace)) + 2) - 1, 1/ normalTreeMeansLogSpace, type = "b", pch = 4, lty = 2)

## Normal Grids

plot((rep(1:length(normalGridMeans), 2) + 1)^2, 1 / c(normalGridMeans, normalGridMeansLogSpace), type = "n", xlab = "n", ylab = "seconds per sample", main = "Normal Grids")
lines(((1:length(normalGridMeans)) + 1)^2, 1 / normalGridMeans, type = "b")
lines(((1:length(normalGridMeansLogSpace)) + 1)^2, 1 / normalGridMeansLogSpace, type = "b", pch = 4, lty = 2)