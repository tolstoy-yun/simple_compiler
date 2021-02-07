 // 00_var_def.c
// a c not define
int main(){
    int b;
    b=a+c;
    return 0;
}

// 01_var_re.c
// a redefine
// int main(){
//     int a;
//     a=10;
//     int a;
//     a=20;
// }

//02_bool.c
//a+2 transform to boolean (or just error)
// int main(){
//     int a;
//     a=10;
//     if(a+2){
//         a-=2;
//     }
//     return 0;
// }

// 03_str.c
// 10+'0' may error or transform
// 10 +"0" must error
// int main(){
//     int a;
//     a=10+'0';
//     a=10+"0";
//     return 0;
// }

// 04_var_usebeforedef.c
// var shall not use before defination, wrong type check will cause this mistake.
// int main(){
// 	b=10;
// 	int a=4,b=5;
// 	a = a + 1;
// 	return 0;
// }

// 05_void_return.c
// void func don't need return anything, will cause error (This test is not necessary)
// void main(){
// 	return 0;
// }

//00_main.c
// int main(){
//     return 0;
// }

//01_basic_out.c
// int main(){
//     printf("test\n");
//     return 0;
// }

//02_var_out.c
// int a;
// int main(){
//     a=10;
//     printf("test:%d\n",a);
//     return 0;
// }

//03_basic_in.c
// int a;
// int main(){
//     scanf("%d",&a);
//     printf("test:%d\n",a);
//     return 0;
// }

//04_if.c
// int a;
// int main(){
//     scanf("%d",&a);
//     if(a>0){
//         a-=20;
//     }
//     printf("test:%d\n",a);
//     return 0;
// }

//05_if_notin.c
// int a;
// int main(){
//     scanf("%d",&a);
//     if(a>0){
//         a-=20;
//     }
//     printf("test:%d\n",a);
//     return 0;
// }

//06_if_else.c
// int a;
// int main(){
//     a=10;
//     if(a>9){
//         a-=20;
//     }else{
//         a+=15;
//     }
//     printf("test:%d\n",a);
//     return 0;
// }

//07_if_else_notin.c
// int a;
// int main(){
//     a=10;
//     if(a>16){
//         a-=20;
//     }else{
//         a+=15;
//     }
//     printf("test:%d\n",a);
//     return 0;
// }

//08_expr_add.c
// int a;
// int b;
// int c;
// int main(){
//     scanf("%d",&a);
//     scanf("%d",&b);
//     c=a+b;
//     printf("test:%d\n",c);
//     return 0;
// }

//09_expr_sub.c
// int a;
// int b;
// int c;
// int main(){
//     scanf("%d",&a);
//     scanf("%d",&b);
//     c=a-b;
//     printf("test:%d\n",c);
//     return 0;
// }

//10_expr_mul.c
// int a;
// int b;
// int c;
// int main(){
//     scanf("%d",&a);
//     scanf("%d",&b);
//     c=a*b;
//     printf("test:%d\n",c);
//     return 0;
// }

//11_expr_div.c
// int a;
// int b;
// int c;
// int main(){
//     scanf("%d",&a);
//     scanf("%d",&b);
//     c=a/b;
//     printf("test:%d\n",c);
//     return 0;
// }

//12_expr_mod.c
// int a;
// int b;
// int c;
// int main(){
//     scanf("%d",&a);
//     scanf("%d",&b);
//     c=a%b;
//     printf("test:%d\n",c);
//     return 0;
// }

//13_expr_minus.c
// int a;
// int c;
// int main(){
//     scanf("%d",&a);
//     c=-a;
//     printf("test:%d\n",c);
//     return 0;
// }

//14_expr_eq.c
// int a;
// int b;
// int main(){
//     scanf("%d",&a);
//     scanf("%d",&b);
//     if(a==b){
//         printf("Success\n");
//     }else{
//         printf("Failed\n");
//     }
//     return 0;
// }

//15_expr_gr.c
// int a;
// int b;
// int main(){
//     scanf("%d",&a);
//     scanf("%d",&b);
//     if(a>b){
//         printf("Success\n");
//     }else{
//         printf("Failed\n");
//     }
//     return 0;
// }

//16_expr_lr.c
// int a;
// int b;
// int main(){
//     scanf("%d",&a);
//     scanf("%d",&b);
//     if(a<b){
//         printf("Success\n");
//     }else{
//         printf("Failed\n");
//     }
//     return 0;
// }

//17_expr_ge.c
// int a;
// int b;
// int main(){
//     scanf("%d",&a);
//     scanf("%d",&b);
//     if(a>=b){
//         printf("Success\n");
//     }else{
//         printf("Failed\n");
//     }
//     return 0;
// }

//18_expr_le.c
// int a;
// int b;
// int main(){
//     scanf("%d",&a);
//     scanf("%d",&b);
//     if(a<=b){
//         printf("Success\n");
//     }else{
//         printf("Failed\n");
//     }
//     return 0;
// }

//19_expr_ne.c
// int a;
// int b;
// int main(){
//     scanf("%d",&a);
//     scanf("%d",&b);
//     if(a!=b){
//         printf("Success\n");
//     }else{
//         printf("Failed\n");
//     }
//     return 0;
// }

//20_expr_or.c
// int a;
// int b;
// int main(){
//     scanf("%d",&a);
//     scanf("%d",&b);
//     if((a==b)||(a!=b)){
//         printf("Success\n");
//     }else{
//         printf("Failed\n");
//     }
//     return 0;
// }

//21_expr_and.c
// int a;
// int b;
// int main(){
//     scanf("%d",&a);
//     scanf("%d",&b);
//     if((a<b)&&(a>b)){
//         printf("Failed\n");
//     }else{
//         printf("Success\n");
//     }
//     return 0;
// }

//22_expr_not.c
// int a;
// int b;
// int main(){
//     scanf("%d",&a);
//     scanf("%d",&b);
//     if(!(a!=b)){
//         printf("Success\n");
//     }else{
//         printf("Failed\n");
//     }
//     return 0;
// }

//23_loop_while.c
// int a;
// int main(){
//     scanf("%d",&a);
//     while(a>0){
//         a=a-1;
//         printf("In Loop\n");
//     }
//     printf("End Loop\n");
//     return 0;
// }

//24_loop_for.c
// int a;
// int main(){
//     for(a=0;a<200;a=a+1){
//         printf("In Loop\n");
//     }
//     printf("End Loop\n");
//     return 0;
// }

//25_comment.c
// comment line
// int a;
// int main(){
//     a=2;
//     /**
//      * a=15;
//      * if(a<2){
//      *  a=0;
//      * }else{
//      * printf("Test Comment\n");
//      * }
//      **/
//     printf("Test:%d\n",a);
//     return 0;
// }

//26_multi_var.c不行
// int a,b,c;
// int main(){
//     scanf("%d",&a);
//     scanf("%d",&b);
//     scanf("%d",&c);
//     printf("%d ",a);
//     printf("%d ",b);
//     printf("%d\n",c);
//     return 0;
// }

//27_char.c不行
// char s;
// char t;
// char a;
// char r;
// int main(){
//     s='9';
//     t='\t';
//     a='t';
//     r='\n';
//     t='a';
//     printf("%c",s);
//     printf("%c",t);
//     printf("%c",a);
//     printf("%c",r);
//     printf("%c",t);
//     return 0;
// }

//28_prior_1.c
// int a;
// int b;
// int c;
// int main(){
// 	a = 16;
// 	b = 2;
// 	c = 30;
// 	int d;
// 	d = a+b*c;
//     printf("%d\n",d);
// 	return 0;
// }

//29_prior_2.c不行
// int main() {
//     int a;
//     a = 20;
//     int b;
//     b = 5;
//     int c;
//     c = 6;
//     int d;
//     d = -4;
//     a = a + c * d - b % (a + d) / a;
//     printf("%d",a);
//     return 0;
// }

//30_prior_3.c不行
// int main () {
//     int a;
//     int b;
//     int c;
//     a = 1;
//     b = 4;
//     c = 28;
//     int t;
//     if (c + a != b) {
//         t = c % -b;
//         printf("%d\n",t);
//     }
//     if (b - c == a) {
//         t = c%b+b;
//         printf("%d\n",t);
//     }
//     return 0;
// }

//31_prior_4.c不行
// int main () {
//     int a;
//     int b;
//     int c;
//     int d;
//     a = 3;
//     b = 8;
//     c = -4;
//     d = 15;
//     int t;
//     if (d % (b - a) != 0 && a > 0 || d % 3 == 0 && c > 0) {
//         t = d + c - -b;
//         printf("%d",t);
//     }
//     return 0;
// }

//32_prior_5.c不行
// int main () {
//     int a;
//     int b;
//     a = -2;
//     b = 1;
//     a = a - -b + -(a + b) % -(a - b);
//     printf("%d\n",a);
//     return 0;
// }

//33_expr_multi_assign.c不行
// int main(){
//     int a;
//     int b;
//     a=b=3;
//     printf("%d",a);
//     printf("%d",b);
//     return 0;
// }

//34_if_else_multista.c
// int main(){
//     int a=10;
//     if(a>9){
//         a=a-10;
//         a=a%6;
//     }else{
//         a=2;
//         a=a-30;
//     }
//     printf("Test:%d\n",a);
//     return 0;
// }

//35_while_if.c不行
// int main(){
//     int a;
//     a = 0;
//     int b;
//     b = 0;
//     while (a < 100) {
//     if (a == 5) {
//         b = 25;
//     }else if (a == 10) {
//         b = 42;  
//     }else {
//         b = a * 2;
//     }
//     a = a + 1;
//     } 
//     printf("%d\n",a);
//     printf("%d\n",b); 
//     return 0;
// }

//36_if_complex_expr.c不行
// int main () {
//     int a;
//     int b;
//     int c;
//     int d;
//     int result;
//     a = 5;
//     b = 5;
//     c = 1;
//     d = -2;
//     result = 2;
//     if ((d * 1 / 2) < 0 || (a - b) != 0 && (c + 3) % 2 != 0) {
//         printf("%d\n",result);
//     }
//     if ((d % 2 + 67) < 0 || (a - b) != 0 && (c + 2) % 2 != 0) {
//         result = 4;
//         printf("%d\n",result);
//     }
//     return 0;
// }

//37_neg_div_mod.c
// int main(){
//     int a;
//     int b;
//     a=-100020;
//     b=-3;
//     int c;
//     int d;
//     c=a/b;
//     d=a%b;
//     printf("test:%d %d\n",c,d);
//     return 0;
// }