brents = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/experiments/brentsMethod.txt", header = FALSE)
newtons = read.table("C:/Users/Stefan/Documents/Visual Studio 2012/Projects/darwin/projects/tutorial/cumulativenet/experiments/safeNewtonsMethod.txt", header = FALSE)

#plot(c(1:length(times$safeNewtons), 1:length(times$safeNewtons)), c(times$safeNewtons, times$Brents), type = "n", xlab = "n", ylab = "samples per second", main = "Comparing Root Finding Methods")

#lines(1:length(times$safeNewtons), times$safeNewtons, lty = 2)
#lines(1:length(times$safeNewtons), times$Brents, lty = 3)

brents = as.matrix(brents)
newtons = as.matrix(newtons)

#sd(unlist(brents[1,]))

# Plot small multiple of sampling speeds
par(mfrow = c(1, 2))

plot(c(1:nrow(brents), 1:nrow(brents), 1:nrow(brents), 1:nrow(brents), 1:nrow(brents), 1:nrow(brents)), c(apply(brents, MARGIN = 1, mean), apply(brents, MARGIN = 1, mean) - 2 * apply(brents, MARGIN = 1, sd), apply(brents, MARGIN = 1, mean) + 2 * apply(brents, MARGIN = 1, sd), apply(newtons, MARGIN = 1, mean), apply(newtons, MARGIN = 1, mean) - 2 * apply(newtons, MARGIN = 1, sd), apply(newtons, MARGIN = 1, mean) + 2 * apply(newtons, MARGIN = 1, sd)), type = "n", xlab = "n", ylab = "samples per second", main = "Safe Newton's Method")

lines(1:nrow(newtons), apply(newtons, MARGIN = 1, mean), type = "b")
lines(1:nrow(newtons), apply(newtons, MARGIN = 1, mean) - 2 * apply(newtons, MARGIN = 1, sd), type = "l", lty = 2)
lines(1:nrow(newtons), apply(newtons, MARGIN = 1, mean) + 2 * apply(newtons, MARGIN = 1, sd), type = "l", lty = 2)

#plot(c(1:nrow(brents), 1:nrow(brents), 1:nrow(brents)), c(), type = "n", xlab = "n", ylab = "samples per second", main = "Brent's Method")
plot(c(1:nrow(brents), 1:nrow(brents), 1:nrow(brents), 1:nrow(brents), 1:nrow(brents), 1:nrow(brents)), c(apply(brents, MARGIN = 1, mean), apply(brents, MARGIN = 1, mean) - 2 * apply(brents, MARGIN = 1, sd), apply(brents, MARGIN = 1, mean) + 2 * apply(brents, MARGIN = 1, sd), apply(newtons, MARGIN = 1, mean), apply(newtons, MARGIN = 1, mean) - 2 * apply(newtons, MARGIN = 1, sd), apply(newtons, MARGIN = 1, mean) + 2 * apply(newtons, MARGIN = 1, sd)), type = "n", xlab = "n", ylab = "samples per second", main = "Brent's Method")

lines(1:nrow(brents), apply(brents, MARGIN = 1, mean), type = "b")
lines(1:nrow(brents), apply(brents, MARGIN = 1, mean) - 2 * apply(brents, MARGIN = 1, sd), type = "l", lty = 2)
lines(1:nrow(brents), apply(brents, MARGIN = 1, mean) + 2 * apply(brents, MARGIN = 1, sd), type = "l", lty = 2)

par(mfrow = c(1, 1))

# Plot ratio of speeds
par(mfrow = c(1, 2))

plot(1:nrow(newtons), apply(newtons, MARGIN = 1, mean) / apply(brents, MARGIN = 1, mean), type = "b", ylab = "ratio of mean sampling speeds of Newton's to Brent's method", xlab = "n", main = "Comparing Mean Sampling Speeds")
abline(h = 1, lty = 2)

plot(1:nrow(newtons), apply(newtons, MARGIN = 1, sd) / apply(brents, MARGIN = 1, sd), type = "b", ylab = "ratio of s.d. of sampling speeds of Newton's to Brent's method", xlab = "n", main = "Comparing Uncertainty in Sampling Speeds")
abline(h = 1, lty = 2)

par(mfrow = c(1, 1))