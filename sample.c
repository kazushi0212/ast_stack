define a;
define b;

array n[3];

a=0; 
b=1;
n[0]=3;
n[1]=5;
n[2]=7;

a=a+2;
b=a*n[0]-b;
n[0]=(n[0]+n[1])/2;
n[1]=n[2]%n[1];
