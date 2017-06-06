files <- c("0gradicompr.txt", "10gradicompr.txt", "20gradicompr.txt", "25gradicompr.txt", "30gradicompr.txt", "35gradcompr.txt", "5gradicompr.txt", "0gradidil.txt", "10gradidil.txt", "20gradidil.txt", "25gradidil.txt", "30gradidil.txt", "35gradidil.txt", "5gradidil.txt")
for(i in 
data <- scan(files[i], list(0,0,0))
P = data[[1]]
V = data[[2]]
T = data[[3]]
Q <- 1/P
v <- prcomp(cbind(Q,V))$rotation
beta <- v[2,1]/v[1,1]
alpha <- mean(V) - beta * mean (Q)
