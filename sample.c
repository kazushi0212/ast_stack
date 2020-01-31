define N;
define i;
define n;
define k;


N = 10;
i = 2;
while( i < N/2) {
       n = 2;
       while(n < 6){
           n=n*2;
           k = i * n;         	       
           n = n + 1;
       }
       i = i + 1;
}
