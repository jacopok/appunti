library(xtable)
files <- c("0gradicompr.txt", "5gradicompr.txt", "10gradicompr.txt", "20gradicompr.txt", "25gradicompr.txt", "30gradicompr.txt", "35gradcompr.txt",  "0gradidil.txt", "5gradidil.txt", "10gradidil.txt", "20gradidil.txt", "25gradidil.txt", "30gradidil.txt", "35gradidil.txt")
names <- c("0c", "5c", "10c", "20c", "25c", "30c", "35c", "0d", "5d", "10d", "20d", "25d", "30d", "35d")
errorP <- 0.01
errorV <- 0.1
errorT <- 0.1
g104 <- 0.980665
Values <- data.frame()

for(a in 1:length(files)){
data <- scan(files[a], list(0,0,0))
for(i in 1:length(data[[1]])){
    if(abs(data[[1]][1]-data[[1]][i]) < 0.04){
        k <- i
        }
    }
for(i in length(data[[1]]):1){
    if(abs(data[[1]][length(data[[1]])] - data[[1]][i]) < 0.04){
        j <- i
        }
    }
P = data[[1]]/errorP
V = data[[2]]/errorV
T = data[[3]]/errorT
Q <- (1/P) #normalized
index <- c(1:length(data[[1]]))
NormalizedData <- data.frame(index,Q,V,T)
NormalizedData <- subset(x = NormalizedData, subset = index >= k, select= c(index,Q,V,T))
NormalizedData <- subset(x = NormalizedData, subset = index <= j, select= c(Q,V,T))
Data <- data.frame(1/(g104 * errorP/NormalizedData$Q) ,NormalizedData$V * errorV , NormalizedData$T * errorT)
names(Data) <- c("Q","V","T")
print(paste("Rejected data: ", length(data[[1]]) - length(NormalizedData$Q)))
v <- prcomp(cbind(NormalizedData[[1]],NormalizedData[[2]]))$rotation
beta <- v[2,1]/v[1,1]
alpha <- mean(NormalizedData[[2]]) - beta * mean(NormalizedData[[1]])
#print(paste("Dataset: ", a))
#print(paste("Intercept: ", alpha * errorV))
#print(paste("Coefficient: ", beta * errorV * errorP))
Values <- rbind(Values, c(a, alpha * errorV, beta * errorV * errorP * g104, mean(Data$T) + 273.15))

#print(summary(lm(formula = Q ~ V, data = NormalizedData)))
#print(summary(lm(formula = Q ~ V, data = Data)))

distancesPointToLine <- abs(V - (beta * Q + alpha)) / sqrt(1+beta^2)
if(max(distancesPointToLine) > 3){print("There is data to reject")}

imagename <- paste("Norm", names[a], ".png", sep="")
png(imagename) #Change to pdf later
plot(NormalizedData[[1]], NormalizedData[[2]], xlab="1/P (adimensionale, P normalizzata all'errore)", ylab="V (adimensionale, V normalizzata all'errore)")
dev.off()

imagename <- paste("NotNorm", names[a], ".png", sep="")
png(imagename) #Change to pdf later
plot(Data[[1]], Data[[2]], xlab="1/P [(10^5 * Pascal)^-1]", ylab="V [10^-6 m^3]")
dev.off()
}
names(Values) <- c("index", "Intercept", "Coefficient", "Temperature")

print(abs(mean(Values$Intercept[0:7])-mean(Values$Intercept[8:14]))/sqrt(sd(Values$Intercept[0:7] / sqrt(7))^2 + sd(Values$Intercept[8:14] / sqrt(7))^2))
print(mean(Values$Intercept))
print(sd(Values$Intercept) / sqrt(14))
print(sd(Values$Intercept))

PVoverT <- Values$Coefficient / Values$Temperature
