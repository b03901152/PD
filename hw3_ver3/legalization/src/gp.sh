gprof ./legalizer > FSA.gprof
gprof2dot FSA.gprof | dot -Tpng -o FSA.png

