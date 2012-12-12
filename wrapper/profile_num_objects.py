import pyshiva as ps


def main():
	win = ps.Window('Boring Objects')
	count = 0
	data = []
	for i in range(4500):
		t0 = win.s_since_open()
		r = ps.Rect(count/10, count/10, 40, 40, (1,1,1,.5))
		win.add(r)
		count += 1;
		t1 = win.s_since_open()
		win.refresh()
		t2 = win.s_since_open()
		data.append('%s, %s, %s'%(i,t1-t0, t2-t1))

	return data

if __name__ == '__main__':
	data = main()
	with open('time_v_objects2.txt','w') as f:
		for d in data:
			f.write(d+'\n')

