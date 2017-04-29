gprof ./hw2 > hw2.gprof
gprof2dot hw2.gprof | dot -Tpng -o flow2.png