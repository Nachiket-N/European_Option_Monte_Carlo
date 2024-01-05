# Monte Carlo method for Option Pricing

### About Monte Carlo:

- Applicable to a wide range of option types
- Is easy to apply
- Slow convergence - reduced accuracy as compared to FDM
- Has some difficulty with computation of the option sensitivities (deltas) and early exercise
- Can be used as a 'second opinion' for other methods like FDM

### Building Blocks of Monte Carlo Simulation

- Random Number Generators
- Data structures for the Stochastic Differential Equations (SDEs) and Finite Difference Method (FDM)
- Vectors & Matrices
- Payoff Function

### Some History on Monte Carlo

- Invented during WWII by John Voh Neumann
- It was first applied to option pricing by Phelim Boyle in 1976
- Applicable to n-factor problems

### Basic idea:

- Simulate the underlying’s Stochastic Differential Equation for $`t = 0`$ (now) to $`t = T`$ (expiry) NSIM times

- We normally have to simulate the SDE using the Finite Difference Method (FDM)

- Compute the payoff at $` t = T `$ for each simulation; average all the payoffs over NSIM

- Apply discounting from $`t = T`$ to $`t = 0`$ to the average

### Stochastic Differential Equations

For this project, we are focusing on just one-factor linear and non-linear Geometric Brownian Motion (GBM)

1.

$$ dS_t = (r-D)S_tdt + \sigma S_t dW_t \hspace{15pt} \ \left( S_t \equiv S(t) \right) $$

$` r = `$ constant interest rate \
$` D = `$ constant dividend \
$` \sigma = `$ constant volatility \
$` dW_t = `$ increments of the Wiener(Brownian) process

2.

$$ dr = \kappa(\theta - r)dt + \sigma r^{\beta} dw $$

$` r = r(t) = `$ level of short rate at time $`t`$ \
$` dW = `$ increment of a Wiener process \
$` \theta = `$ long-term level of $`r`$ \
$` \kappa = `$ speed of mean reversion \
$` \sigma = `$ volatility of the short rate \
$` \beta = 0 `$ for Vasicek model, $`0.5`$ for CIR model

###

### The Algorithm

For each $j = 1,...,M$ calculate

$$ C*{T,j} = max( 0 , S*{T,j} - K ) $$

$and$

$$ \hat{C} = exp(-rT) \dfrac{1}{M} \sum*{j=1}^{M} max( 0 , S*{T,j} - K ) $$

$\hat{C}$ is the desired call price

####

##

In this project, we focus on a linear, constant coefficient, scalar (one-factor) problem. Specifically, we examine the case of a one-factor European Call Option using the assumptions of the original Black-Scholes equation.

We know the option price at expiry (t=T) as a function of the current stock price and the strike price. We carry out a simulation experiment by finding the solution of a stochastic differential equation (SDE) from time t=0 to t=T.

This process allows us to compute the stock price at t=T and then the option price using the payoff function.

We carry out M simulations - in each one, we find the solution of the SDE and then calculate the option price at t=T. Finally, we calculate the discounted average of the simulated payoff.

####

The process can be simplified to the following steps:

- Perform the following 3 steps, M times:
  - Construct a simulated path of the underlying stock
  - Calculate the stock price at t = T
  - Calculate the call option price at t = T (using the payoff function)
- Calculate the averaged call price at t = T
- Discount the price found in step 4 to t = 0

####

We start by replacing continuous time with discrete time. So, we divide the interval $[0,T]$ into a number of subintervals and define $N+1$ mesh points as follows:

$$ 0 = t_0 < t_1 < ... < t_N = T $$

For a subinterval $(t_n, t_{n+1})$ its size is $` \Delta t_n \ , `$ $` 0 \leq n \leq N-1 `$

Since we are considering a uniform mesh, all the subintervals are of the same size, i.e. $` \Delta t_n = T/N `$

Now, we look at the scalar linear SDE with constant coefficients:

$` dX = aXdt + bXdW \ , \ a,b \ constant `$ \
$` X(0) = A `$

We can write the above SDE as an SIE (Stochastic Integral Equation) between times $`s`$ and $`t`$ as follows:

$`X(t) = X(s) + \int_s^t aX(y) \, dy + \int_s^t bX(y) \, dW(y), \quad s < t`$

To evaluate the above equation at two consecutive mesh points, we get the following using the fact that $a$ and $b$ are constants:

$` X(t) = X(s) + a \int_{t_n}^{t_{n+1}} X(y)\,dy \ + b \int_{t_n}^{t_{n+1}} X(y) \, dW(y) `$

To approximate the above equation, we replace the solution $X$ by a new discrete function $Y$ (defined at mesh points) by assuming that it is constant on each subinterval.

$` Y\_{n+1} = Y_n + a Y_n \Delta t_n + b Y_n \Delta W_n `$

where $` \Delta W_n = W(t_{n+1}) - W(t_n) $ , $ 0 \leq n \leq N-1 `$

This is called the (explicit) Euler-Maruyama scheme and it is a popular method for approximating the solution of SDEs.

We can write the discrete equation as:

$` \left\{
\begin{array}{l}
X_{n+1} = X_n + aX_n\Delta t_n + bX_n \Delta W_n \\
X_0 = A
\end{array}
\right. `$

In this project we have used a constant Mesh size and the Wiener increments are well known computable quantities:

$` \left\{
\begin{array}{l}
\Delta t_n = \Delta t = T/N \ , \ 0\leq n \leq N-1 \\
\Delta W_n = \sqrt{\Delta t} z_n \ , \ where \ z_n \sim N(0,1)
\end{array}
\right. `$

For this project we generate the random numbers using Boost Random. And we deal almost exclusively with one-step marching schemes; i.e. the unknown solution at time level $n + 1$ is calculated in terms of known values at time level $n$

##

####

### Standard Deviation and Standard Error of Call Option Price

$` SD = \sqrt{ \dfrac{\Sigma C_{T,j}^2 - \frac{1}{M} (\Sigma C_{T,j})^2 }{M-1} } \times exp(-rT) `$

$` SE = \dfrac{SD}{\sqrt{M}} `$

where

$` C_{T,j} = `$ call option price at $` t=T `$ for the $` j^{th} `$ simulation , $` 1 \leq j \leq M `$

$` M = `$ number of simulations
