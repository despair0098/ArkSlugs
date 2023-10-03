int a = 16807;
int m = 2147483647;
int q = 127773;
int r = 2836;
int seed = 1001;

int randomInt(){
   int num = 0;
   int high = seed / q;
   int low = seed - (q * high);
   int test = (a * low) - (r * high);
   if(test > 0){
      seed = test; 
   } else {
      seed = test + m; 
   }
   num = seed / m;
   return num;
}
