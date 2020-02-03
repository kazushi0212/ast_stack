define N;
define i;
define n;
define k;
array a[1001];

N = 1000;
i = 1;
while (i <= N) {
      a[i] = 1;
      i = i + 1;
}

i = 2;
while( i <= N/2) {
       n = 2;
       while(n <= N/i){
       	       k = i * n;
               a[k] = 0;             	       
       	       n = n + 1;
       }
       i = i + 1;
}
