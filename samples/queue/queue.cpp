#include <stdio.h>  
#include <stdlib.h>  
#include <math.h>  
  
#define QUEEN 8 
#define INITIAL -10000  
  
int a[QUEEN]; 
  
void init()
{  
    int *p;  
    for (p = a; p < a + QUEEN; ++p)   
    {  
        *p = INITIAL;  
    }  
}   
  
int valid(int row, int col)
{  
    int i;  
    for (i = 0; i < QUEEN; ++i)
    {  
        if (a[i] == col || abs(i - row) == abs(a[i] - col))
            return 0;  
    }  
    return 1;  
}   
  
void print()
{  
    int i, j;  
    for (i = 0; i < QUEEN; ++i)  
    {  
        for (j = 0; j < QUEEN; ++j)  
        {  
            if (a[i] != j)
                printf("%c ", '.');  
            else
                printf("%c ", '#');  
        }  
        printf("\n");  
    }  
    for (i = 0; i < QUEEN; ++i)  
        printf("%d ", a[i]);  
    printf("\n");  
    printf("--------------------------------\n");  
}  
  
void queen()
{  
    int n = 0;  
    int i = 0, j = 0;  
    while (i < QUEEN)  
    {  
        while (j < QUEEN)
        {  
            if(valid(i, j))
            {  
                a[i] = j;
                j = 0;
                break;  
            }  
            else
            {  
                ++j;
            }
        }  
        if(a[i] == INITIAL)
        {  
            if (i == 0)
                break;  
            else
            {  
                --i;  
                j = a[i] + 1;
                a[i] = INITIAL;
                continue;
            }  
        }  
        if (i == QUEEN - 1)
        {  
            printf("answer %d : \n", ++n);  
            print();  
            j = a[i] + 1;
            a[i] = INITIAL;
            continue;  
        }  
        ++i;
    }  
}  
  
int main(void)  
{  
    init();  
    queen(); 
    return 0;  
}  

