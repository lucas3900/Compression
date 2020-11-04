all: compress uncompress

uncompress: compress
	ln -s compress uncompress

compress: compress.cc pq.cc pq.h
	g++ -o compress compress.cc

clean:
	rm -f *~ compress uncompress *.o
