import pyshiva as ps
from guppy import hpy



def main():
	win = ps.Window('Boring Objects')
	count = 0
	data = []
	h = hpy()
	for i in range(5):
		r = ps.Rect(count/10, count/10, 40, 40, (1,1,1,.5))
		win.add(r)
		print h.heap()
		count += 1;
		win.refresh()
		print h.heap()
		#data.append(win.s_since_refresh())

	return data

if __name__ == '__main__':
	data = main()
	"""
	with open('time_v_objects3.txt','w') as f:
		for d in data:
			f.write(str(d)+'\n')
	"""

