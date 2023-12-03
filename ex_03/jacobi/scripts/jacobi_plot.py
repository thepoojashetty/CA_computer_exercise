import matplotlib.pyplot as plt
import pandas as pd

plt.figure(figsize=(8,8))

plt.title('Vectorization Loop Unrolling Performance for Jacobian')
plt.xlabel('ArraySize')
plt.ylabel('MegaUpdatesPerSecond')
plt.xscale('log',base=2)

#plotting for no unrolling
data_no_unroll=pd.read_csv('result_1.csv')
x=data_no_unroll['ArraySize']
y=data_no_unroll['MegaUpdatesPerSecond']
plt.plot(x,y,label='No Loop Unrolling',marker='.')

#plotting for twofold unrolling
data_twofold=pd.read_csv('result_2.csv')
x=data_twofold['ArraySize']
y=data_twofold['MegaUpdatesPerSecond']
plt.plot(x,y,label='Twofold Unrolling(SSE)',marker='.')

#plotting for fourfold unrolling
data_fourfold=pd.read_csv('result_4.csv')
x=data_fourfold['ArraySize']
y=data_fourfold['MegaUpdatesPerSecond']
plt.plot(x,y,label='Fourfold Unrolling(AVX)',marker='.')

plt.legend()
plt.savefig('jacobi_unroll.png')
plt.show()