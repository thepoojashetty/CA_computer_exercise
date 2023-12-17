import matplotlib.pyplot as plt
import pandas as pd

plt.figure(figsize=(8,8))

plt.title('Profiling Performance for Jacobian')
plt.xlabel('ArraySize')
plt.ylabel('HitRatio')
plt.xscale('log',base=2)

#column_wise
data_colwise=pd.read_csv('result_col_hit_ratio.csv')
x=data_colwise['ARRAY_SIZE']
y=data_colwise['HIT_RATIO']
plt.plot(x,y,label='ColumnWise Array Access',marker='.')

#row_wise
data_rowwise=pd.read_csv('result_row_hit_ratio.csv')
x=data_rowwise['ARRAY_SIZE']
y=data_rowwise['HIT_RATIO']
plt.plot(x,y,label='RowWise Array Access',marker='.')


plt.legend()
plt.savefig('jacobi_performance.png')
plt.show()