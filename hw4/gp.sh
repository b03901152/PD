gprof ./Router > Router.gprof
gprof2dot Router.gprof | dot -Tpng -o output/RouterAnalysis.png
eog -f output/RouterAnalysis.png
