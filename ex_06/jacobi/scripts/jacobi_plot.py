import matplotlib.pyplot as plt
import pandas as pd

plt.figure(figsize=(8,8))

plt.title('Cache Blocking Performance for AVX')
plt.xlabel('ArraySize')
plt.ylabel('MegaUpdatesPerSecond')
plt.xscale('log',base=2)

#cache blocking
data_cb=pd.read_csv('result_cb.csv')
x=data_cb['ArraySize']
y=data_cb['MegaUpdatesPerSecond']
plt.plot(x,y,label='Cache blocking',marker='.')

#no cache blocking
data_ncb=pd.read_csv('result_no_cb.csv')
x=data_ncb['ArraySize']
y=data_ncb['MegaUpdatesPerSecond']
plt.plot(x,y,label='Pure AVX',marker='.')

plt.legend()
plt.savefig('jacobi_cb.png')
plt.show()