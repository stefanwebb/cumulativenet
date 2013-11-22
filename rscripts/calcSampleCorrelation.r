smp = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/samplesSingleCopula.txt", header = TRUE)

rhohat = cor(smp$X1, smp$X2) * (length(smp$X1) - 1) / length(smp$X1)
print(rhohat)

dmnorm(smp, c(0, 0), )