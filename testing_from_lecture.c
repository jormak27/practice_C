/* Jordan Makansi
Testing for EE 249 */

char *x;
void foo(void) {
    char y[] = "yep";
    x = &y;
    *x = "n";
    printf("y is %d\n", y); // This will print the ASCII value of the string variable 
    return;
}

void s_flow(int x) {
    int b = 0;
    if (x>b) {
        s_flow(x+1);
    return;
    }
}

void main(void) {
    foo();
    char m="s";
    printf("####### %d\n", m);
    s_flow(2);
    // this function doesn't really do much as you can see here
    }