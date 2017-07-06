/***************************************************************************
 *   libecoteste.c version v0.1.090902.000301                              *
 *                                                                         *
 * This program is used to test and examplify libeco.h library             *
 * Portable common functions for Linux/Windows                             *
 *   Copyright (C) 2009 by Ruben Carlo Benante (aka Dr.Beco)               *
 *   rcb at beco.cc                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 *                                                                         *
 * To contact the author, please write to:                                 *
 * Ruben Carlo Benante.                                                    *
 * email: rcb at beco dot cc                                               *
 * web page:                                                               *
 * http://www.beco.cc                                                      *
 * address:                                                                *
 * FAX: +55 (81) 2126-8438                                                 *
 * PHONE: +55 (81) 2126-8430  x.4067                                       *
 ***************************************************************************/

/* Compile with:
 *     gcc libecoteste.c -o libecoteste -lm
 * be sure libeco.h is in the same directory
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <float.h>
#include "libeco.h"

#define TAM 5

int main(int argc, char *argv[])
{

    int a, b, i, novo=0;
    char c;
    int amax, amin;
    double x2;
    int meuint;
    float meufloat;
    double meudouble;

    int fica=2, die, ydie=1, last=0;
    char *s;
    char str[TAM];
    clock_t clock1, clock2, diffclock;

    if(argc==2)
        if(!strcmp(argv[1],"-n"))
            novo=1;

    printf("\nlibecoteste version v0.1.090902.000301\n\n");

    if(!novo)
    {
        printf("Teste getch()\n");
        c=getch();
        printf("Char: %c ASCII: %d\n", c, c);
        printf("Teste ok\n\n");

        printf("Teste kbhit()\n");
        clock1 = clock() / CLOCKS_PER_SEC;
        clock2 = clock1;
        diffclock = clock2 - clock1;
        printf("Tecle algo no tempo de: ");
        while(diffclock<10)
        {
            if(a=kbhit()) /* kbbuff() deprecated */
                break;
            clock2 = clock() / CLOCKS_PER_SEC;
            diffclock = clock2 - clock1;
            if(diffclock>last)
            {
                printf("%lus ", 10-diffclock);
                last++;
            }
            fflush(stdout);
        }
        if(a)
            printf("\nTecla disponivel no buffer\n");
        else
            printf("\nTecla nao pressionada\n");
        printf("Teste ok\n\n");

        printf("Teste min(3,2)\n");
        printf("%d\n", min(3,2));
        printf("Teste min(-3,-2)\n");
        printf("%d\n", min(-3,-2));
        printf("Teste ok\n\n");


        printf("Teste kbwait(1000)\n");
        //sleep(1);
        printf("Tecle algo 10s\n");
        a=kbwait(1000);
        if(a)
            printf("Tecla disponivel no buffer\n");
        else
            printf("Tecla nao pressionada\n");
        printf("Teste ok\n\n");

        printf("Teste getfrom(\"0a\")\n");
        c=getfrom("0a");
        if(c!='a' && c!='0')
            printf("\nTeste FALHOU!\n\n");
        else
            printf("Teste ok\n\n");



        printf("Teste rand_minmax(0,10) em 10000 iteracoes\n");
        amax = -1;
        amin = 11;
        for(i=0; i<10000; i++)
        {
            a=rand_minmax(0, 10);

            if(a>amax)
                amax=a;
            if(a<amin)
                amin=a;
        }
        printf("max=%d, min=%d\n", amax, amin);
        if(amax<10 && amin>=0)
            printf("\nTeste ok\n\n");
        else
            printf("\nTeste FALHOU!\n\n");


        printf("Teste x2=mudaintervalo(0.0, 50.0, 0.0, 100.0, 25.0)\n");
        x2=mudaintervalo(0.0, 50.0, 0.0, 100.0, 25.0);
        printf("x2: %f\n", x2);
        if(x2!=50.0)
            printf("\nTeste FALHOU!\n\n");
        printf("Teste x2=mudaintervalo(0.0, 100.0, 0.0, 0.4, 50.0)\n");
        x2=mudaintervalo(0.0, 100.0, 0.0, 0.4, 50.0);
        printf("x2: %f\n", x2);
        printf("\nTeste ok\n\n");


        printf("Teste de limites dos tipos\n");
        printf("--------------------------\n");
        printf("unsigned %lu bits %lu bytes\n", sizeof(unsigned)*CHAR_BIT,(sizeof(unsigned)*CHAR_BIT)/8);
        printf("max unsigned %u, min unsigned %u\n\n", UINT_MAX, 0);

        printf("int %lu bits %lu bytes\n", sizeof(int) * CHAR_BIT, (sizeof(int) * CHAR_BIT)/8);
        printf("max int %d, min int %d\n\n", INT_MAX, INT_MIN);

        printf("long int %lu bits %lu bytes\n", sizeof(long) * CHAR_BIT, (sizeof(long) * CHAR_BIT)/8);
        printf("max %ld, min %ld\n\n", (long)LONG_MAX, (long)LONG_MIN);

        printf("long long int %lu bits %lu bytes\n\n", sizeof(long long) * CHAR_BIT, (sizeof(long long) * CHAR_BIT)/8);
    //    printf("max %d, min %d\n\n", LLONG_MAX, LLONG_MIN);

        printf("float %lu bits %lu bytes\n", sizeof(float) * CHAR_BIT, (sizeof(float) * CHAR_BIT)/8);
        printf("max, min float +-%f\n", (double)FLT_MAX);
        printf("max float 10^%f-1 is valid\n", (double)FLT_MAX_10_EXP);
        printf("min float 10^%f-1 is valid\n\n", (double)FLT_MIN_10_EXP);

        printf("double %lu bits %lu bytes\n", sizeof(double) * CHAR_BIT, (sizeof(double) * CHAR_BIT)/8);
        printf("max, min double +-%f\n", (double)DBL_MAX);
        printf("max double 10^%f-1 is valid\n", (double)DBL_MAX_10_EXP);
        printf("min double 10^%f-1 is valid\n\n", (double)DBL_MIN_10_EXP);
        //FLT_MAX_EXP
        //FLT_RADIX^FLT_MAX_EXP-1 is valid
        //DBL_MAX_EXP
        printf("FLT_ROUNDS: %d\n", FLT_ROUNDS);
        printf("--------------------------\n");

        printf("%f+%f=%f\n", 83484516925440.000000, 1.1, 83484516925440.000000+1.1);
        x2=5000000000000.0;
        x2+=1.1;
        printf("x:%.15f\n", x2);

        printf("%f+%f=%f\n", 340282346638528859811704183484516925440.0, 1.1, (double)((double)340282346638528859811704183484516925440.0+(double)1.1));


        printf("\nchar getfrom(\"01923456789.-\\n\");");
        printf("Press ENTER to end, or fill %d chars.\n", TAM-1);
        i=0;
        str[0]=str[TAM-1]='\0';
        while(1)
        {
            a=getfrom("01923456789.-\n");
            if(a>=32&&a<=127)
            {
                str[i]=a;
                str[i+1]='\0';
                printf("%c", a);
                if(i==TAM-2)
                    break;
                i++;
            }
            else if(a=='\n')
                break;
        }
        printf("\ns='%s'\nlength: %lu\n", str, strlen(str));
        printf("sizeof(s): %lu\n", sizeof(str));
        printf("s[%d]:'%d'\n", TAM-1, str[TAM-1]);
        printf("s[%d]:'%d'\n", TAM-2, str[TAM-2]);
        printf("s[%d]:'%d'\n", TAM-3, str[TAM-3]);
        printf("\nTeste ok\n\n");

    }

    printf("Teste chkpipe()\n");
    printf("Teste nao implementado\n\n");



    // printf("Teste ok\n\n");
    printf("Fim dos testes\n");
    printf("Nota: para compilar use: gcc libecoteste.c -o libecoteste\n\n");
}
