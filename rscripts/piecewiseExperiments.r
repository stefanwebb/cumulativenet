path = "C:\\Users\\Stefan\\Documents\\Visual Studio 2012\\Projects\\honours\\Cwd\\experiments\\"  # experiments\\
setwd(path)

source("..\\..\\rscripts\\piecewiseLoadData.r")

#hist(clLoop$mse, breaks = 120)
#hist(clLoop$time, breaks = 5)

claytonTreeBounds = c(min(log(errorMseLbfgsClaytonTree1000[,2])), max(log(errorMseClClaytonTree10[,4])))

par(mfrow=c(1, 1), omi = c(0, 0, 0, 0), mai = c(1, 1.0, 0.25, 0.25))

plot(errorMseClClaytonTree10[,1], log(errorMseClClaytonTree10[,3]), xaxt = "n", xlab = "n", ylab = "ln(mean squared error)", ylim = claytonTreeBounds)
apply(errorMseClClaytonTree10, 1, function(x) { lines(rep(x[1],2), log(x[c(2,4)])) })
axis(1, at = errorMseClClaytonTree10[,1], labels = errorMseClClaytonTree10[,1])

plot(errorMseClClaytonTree100[,1], log(errorMseClClaytonTree100[,3]), xaxt = "n", xlab = "n", ylab = "ln(mean squared error)", ylim = claytonTreeBounds)
apply(errorMseClClaytonTree100, 1, function(x) { lines(rep(x[1],2), log(x[c(2,4)])) })
axis(1, at = errorMseClClaytonTree100[,1], labels = errorMseClClaytonTree100[,1])

plot(errorMseClClaytonTree1000[,1], log(errorMseClClaytonTree1000[,3]), xaxt = "n", xlab = "n", ylab = "ln(mean squared error)", ylim = claytonTreeBounds)
apply(errorMseClClaytonTree1000, 1, function(x) { lines(rep(x[1],2), log(x[c(2,4)])) })
axis(1, at = errorMseClClaytonTree1000[,1], labels = errorMseClClaytonTree1000[,1])