import math
coefficients = 512
step = 43   # freq_resolution
start = 43
end = coefficients * step

print 'calculating ', coefficients, ' coefficients with ', step, 'Hz resolution...'
print 'start: ', start, ' end: ', end, 'step: ', step

i = 0
for f in range(start, end+1, step):
    # formula taken from: https://en.wikipedia.org/wiki/A-weighting
    coeff = (12194**2 * f**4) / ( (f**2 + 20.6**2) * math.sqrt( (f**2 + 107.7**2) * (f**2 + 737.9**2) ) * (f**2 + 12194**2) )
    print 'i = ',i, 'f= ', f, ' coeff= ', coeff
    i = i + 1
