try:
	import pyshiva as ps
except ImportError as e:
	print("""For now, this program must be run from the same 
directory in which pyshiva.so is located! - Julian""")
	raise e

if __name__ == "__main__":
	win = ps.Window("My First pyShiva Program", width = 500, height = 500)