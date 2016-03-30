 set multiplot

 set size 0.33,0.33
 set origin 0.0,0.0
 set xlabel "tiempo"
 set ylabel "accel x"
 plot 'test2.txt' using 2 with lines


 set size 0.33,0.33
 set origin 0.0,0.33
 set xlabel "tiempo"
 set ylabel "accel y"
 load 'config'
 plot 'test2.txt' using 3 with lines

 set size 0.33,0.33
 set origin 0.0,0.66
 set xlabel "tiempo"
 set ylabel "accel y"
 load 'config'
 plot 'test2.txt' using 3 with lines


 set size 0.33,0.33
 set origin 0.33,0.0
 set xlabel "tiempo"
 set ylabel "accel y"
 load 'config'
 plot 'test2.txt' using 3 with lines

 set size 0.33,0.33
 set origin 0.33,0.33 
 set xlabel "tiempo"
 set ylabel "accel y"
 load 'config'
 plot 'test2.txt' using 3 with lines

 set size 0.33,0.33
 set origin 0.33,0.66 
 set xlabel "tiempo"
 set ylabel "accel y"
 load 'config'
 plot 'test2.txt' using 3 with lines

 set size 0.33,0.33
 set origin 0.66,0.0 
 set xlabel "tiempo"
 set ylabel "accel y"
 load 'config'
 plot 'test2.txt' using 3 with lines

 set size 0.33,0.33
 set origin 0.66,0.33 
 set xlabel "tiempo"
 set ylabel "accel y"
 load 'config'
 plot 'test2.txt' using 3 with lines

 set size 0.33,0.33
 set origin 0.66,0.66 
 set xlabel "tiempo"
 set ylabel "accel y"
 load 'config'
 plot 'test2.txt' using 3 with lines


replot
reread

unset multiplot

