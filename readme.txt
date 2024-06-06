Information for compiling and running the program.

Example of compiling : 
  [s3344949@csitprdap02 ~]$ ./ftt <foodsfile> <coinsfile>
  [s3344949@csitprdap02 ~]$ ./ftt foods.dat coins.dat

./ftt foods.dat coins.dat < test1.input  > test1.actual_output
diff -w test1.expected_output test1.actual_output
diff -w -y test1.expected_coins test1.actual_coins
