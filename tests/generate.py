import numpy  as np
import pandas as pd
from scipy import stats as st

# Test parameters
# FRAME     is size of the window
# SIZE      is size of the data

FRAME     = 6
SIZE      = 100

v = np.random.randint(low=10,high=31, size=SIZE)
with open('testdata', 'w') as f:
    print(SIZE, FRAME, file=f)
    for x in v:
        print(x, file=f)

v = np.insert(v, 0,  [v[0]]*(FRAME-1))
v = np.insert(v, -1, [v[-1]]*(FRAME-1))

with open('raw.a', 'w') as f:
    for i in range(SIZE+FRAME-1):
        for j in range(5):
            print(sum(map(lambda i : i**j, v[i:i+FRAME]))/FRAME, end=",", file=f)
        print('', file=f)

with open('moments.a', 'w') as f:
    for i in range(SIZE+FRAME-1):
        for j in range(5):
            print(st.moment(v[i:i+FRAME], moment=j), end=",", file=f)
        print('', file=f)

with open('stats.a', 'w') as f:
    for i in range(SIZE+FRAME-1):
        for q in [0.0, 0.25, 0.5, 0.75, 1.0]:
            print(np.quantile(v[i:i+FRAME], q), end=",", file=f)
        print('', file=f)

with open('means.a', 'w') as f:
    for i in range(SIZE+FRAME-1):
        print(np.mean(v[i:i+FRAME]), end=",", file=f)
        print(st.gmean(v[i:i+FRAME]), end=",", file=f)
        print(st.hmean(v[i:i+FRAME]), end=",", file=f)
        print(st.pmean(v[i:i+FRAME], 2), end=",", file=f)
        print('', file=f)
