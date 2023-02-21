#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <graphics.h>
#include <winbgim.h>
#include <libbgi.a>
int main()
{
    float a, b, k;
    float q;
    float x0, y0, x1e, y1e, x2e, y2e;
    float x1, y1, x2, y2, A1, A2;
    float S1=0, S2=0, xc1, xc2, yc1, yc2;
    float Aa, xa, ya;       // aria, respectiv coordonatele centrului, calculate analitic
    float An = 0, xn, yn;   // aria, respectiv coordonatele centrului, calculate numeric
    int be = 600, he = 400; // dimensiunile ferestrei grafice, in pixeli
    int i,n;

//      Citirea datelor de intrare

    printf("Baza si inaltimea placii = ");
    scanf(" %f %f", &a, &b);
    printf("Nr. pasi =");
    scanf(" %d", &n);
    printf("Factor de scara =");
    scanf(" %f", &k);

//      Calcul analitic

        q = (b*b)/a;
        Aa = (2*a*b)/3;
        xa = (3*a)/5;
        ya = (3*b)/8;



//      Calcul numeric si desen

    initwindow(be, he);
    x0 = be/2;
    y0 = he/2;



    for (i = 0; i < n; i++)
    {
        x1 = (i*a)/n;
        x2 = ((i+1)*a)/n;
        y1 = sqrt(q*x1);
        y2 = sqrt(q*x2);
        A1 = (( x2-x1 )*y1)/2;
        A2 = (( x2-x1 )*y2)/2;
        An += A1 + A2;

         xc1 = (2*x1 +  x2)/3;
         xc2 = (2*x2 +  x1)/3;
         yc1 = y1/3;
         yc2 = (y1 + y2)/3;

         S1 += A1*xc1 + A2*xc2;
         S2 += A1*yc1 + A2*yc2;



        x1e = x0 + k*x1;
        y1e = y0 - k*y1;
        x2e=x0 + k*x2;
        y2e=y0 - k*y2;

        line(x1e, y0, x1e, y1e);
        line(x1e, y0, x2e, y0);
        line(x2e, y0, x2e, y2e);
        line(x1e, y1e, x2e, y2e);
        line(x2e, y0, x1e, y1e);
    }

    xn = S1/An;
    yn = S2/An;

    setcolor(YELLOW);
    setlinestyle(SOLID_LINE , 1, 4);
    circle(x1e, y1e, 4);

//      Afisarea rezultatelor

    printf("\n");
    printf("Analitic:  A = %6.3f, xC = %6.3f, yC = %6.3f \n", Aa, xa, ya);
    printf("Numeric :  A = %6.3f, xC = %6.3f, yC = %6.3f \n", An, xn, yn);

    printf("\n");
    printf("   Press any key! \n");
    while (!kbhit()){};
    return 0;
}
