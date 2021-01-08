 // 00_var_def.c
// a c not define
// int main(){
//     int b;
//     b=a+c;
//     return 0;
// }

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
int a;
int b;
bool c;
bool d;
int main(){
    c=c&&d;
    a = 9;
    b = 0;
    a=a+b;
    printf("test:%d\n",a);
    return 0;
}