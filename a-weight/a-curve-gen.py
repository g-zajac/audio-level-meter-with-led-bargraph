# script generating A-weight curve coefficients based on formula from: https://en.wikipedia.org/wiki/A-weighting
# run: python a-curve-gen.py

import math

coefficients_number = 512
step = 43
start = 43
end = coefficients_number * step
results_array = []

print '// this file is generatet by phyton script a-curve-gen.py'
print '// a-weight curve size ',coefficients_number,' coefficients with ',step, 'Hz resolution'
print '// start: ', start, ' end: ', end, 'step: ', step
print ''

i = 0
for f in range(start, end+1, step):
    coeff = (12194**2 * f**4) / ( (f**2 + 20.6**2) * math.sqrt( (f**2 + 107.7**2) * (f**2 + 737.9**2) ) * (f**2 + 12194**2) )
    results_array.append(coeff)
    i = i + 1

# convert array to string
results_string = ', '.join([str(elem) for elem in results_array])

print 'float a_weighting_curve[512] = {'
print results_string
print '};'
