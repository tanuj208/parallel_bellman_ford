import random

n=1000
m=100000
with open('input', 'w+') as f:
	f.write(str(n))
	f.write(" ")
	f.write(str(m))
	f.write('\n')

	for i in range(m):
		u = random.randrange(1, n, 1)
		v = random.randrange(1, n, 1)
		while v==u:
			v = random.randrange(1, n, 1)
		w = random.randrange(1, 100, 1)
		f.write(str(u))
		f.write(' ')
		f.write(str(v))
		f.write(' ')
		f.write(str(w))
		f.write('\n')

	f.write('2\n')