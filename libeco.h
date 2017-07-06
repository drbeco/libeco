/*--------------------------------------------
 File: libeco.h

 By Ruben Carlo Benante (dr.beco@gmail.com)
 2009-08-27

 Acknowlegments:
 Some pieces of code adapted from and thanks to
 DB1, 2004-09-10, 17:30
 kermi3, 2002-01-11, 11:56
 itsme86, 2005-03-17, 21:00
 Nimrod Zimerman, 1998-09-16, 02:26
 pepito, pullinput(), 2004
 Linux Programmer's Manual, select(2), 2007-07-26

--------------------------------------------
*/

#include <time.h>
#include <string.h>

/* #ifdef __linux__ */
#ifndef __win32__
  #include <termio.h>
  #include <termios.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <sys/select.h>
/*  #include <sys/time.h> */
#else
  #include <conio.h>
/*
     #include <windows.h>
     #include <winbase.h>
     #include <wincon.h>
     #include <io.h>
*/
#endif


/* prototypes */

#ifdef __linux__
  /* le 1 caracter do teclado, fica bloqueado ate ler, nao espera <enter>, nao imprime */
  int getch(void);
  /* retorna verdadeiro/falso se existir um caracter no buffer a ser lido */
  int kbbuf(void);
  /* espera por um caracter por usec microsegundos e retorna verdadeiro/falso */
  int kbwait(long usec);
  /* retorna verdadeiro/falso se existir um caracter no buffer a ser lido (testa com select) */
  int kbhit(void);
  /* le apenas os caracteres na string s. Imprime o numero turn */
  //char getfrom(char *s, int turn);
#endif

char getfrom(char *s);
/* muda um valor x1 no intervalo [min1, max1] para outro proporcional no intervalo [min2, max2] */
float mudaintervalo(float min1, float max1, float min2, float max2, float x1);
/* retorna inteiro entre [min,max[ */
int rand_minmax(int min, int max);
/* retorna o minimo entre dois inteiros */
int min(int a, int b);
/* retorna verdadeiro se existe algum caracter no buffer para ser lido */
int pollinput (void);

#ifdef __linux__
  /*--------------------------------------------
   function: int getch(void)
   usage example:
          int t=getch();


   function description:
          blocks until it gets a char from keyboard buffer
          returns immediatly, no need to press return
          do not echoes char to terminal

   To disable break, define:

  #define GETCH_DISABLE_CTRLC
  */

  int getch(void)
  {
      char ch;
      int fd;
      struct termio term_velho, term_novo;

      fd = fileno(stdin);
      ioctl(fd, TCGETA, &term_velho);
      term_novo = term_velho;
  #ifdef GETCH_DISABLE_CTRLC
          term_novo.c_lflag &= ~(ICANON | ECHO | ISIG);
  #else
          term_novo.c_lflag &= ~(ICANON | ECHO );
  #endif
      ioctl(fd, TCSETA, &term_novo);
      fread(&ch, 1, sizeof(ch), stdin);
      ioctl(fd, TCSETA, &term_velho);

      return ch;
  }

  /*--------------------------------------------
   function: int kbbuf(void)
   usage exemple:
          while(!kbbuf())
               printf("press a key to stop loop\n");

   function description:
          tests (read/unread) for available char in keyboard buffer
          returns true or false immediatly
  */

  int kbbuf(void)
  {
    char ch;
    int fd;
    struct termios term_velho, term_novo;

    tcgetattr(STDIN_FILENO, &term_velho);
    term_novo = term_velho;
    term_novo.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &term_novo);
    fd = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, fd | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &term_velho);
    fcntl(STDIN_FILENO, F_SETFL, fd);

    if(ch != EOF)
    {
      ungetc(ch, stdin);
      return 1;
    }

    return 0;
  }

  /*--------------------------------------------
   function: int kbwait(long usec)
   usage exemple:
          printf("Waiting 1 sec for a key press");
          if(kbwait(1000000))
               printf("key pressed during the interval of 1 sec! ");
          else
               printf("no key pressed during the interval of 1 sec! ");

   function description:
          waits for available char in keyboard buffer for usec microseconds
          returns true immediatly, or false after usec.
  */

  int kbwait(long usec)
  {
     fd_set rdfs;
     struct timeval tv;
     struct termios normalTTY, rawTTY;
     int ret=0;

     /* setup terminal */
     if (tcgetattr (0, &normalTTY) == -1)
       exit (-1);
     rawTTY = normalTTY;
     rawTTY.c_lflag &= ~ICANON;
     rawTTY.c_lflag &= ~ECHO;
     if (tcsetattr (0, TCSANOW, &rawTTY) == -1)
       exit (-1);

     FD_ZERO (&rdfs);
     FD_SET (0, &rdfs);
     tv.tv_sec = 0;
     tv.tv_usec = usec;

     if (select (1, &rdfs, NULL, NULL, &tv) == 1)
     {
        if (FD_ISSET (0, &rdfs))
           ret = 1;
     }
     else
        ret=0;

     /* restore terminal */
     if (tcsetattr (0, TCSANOW, &normalTTY) == -1)
        exit (-1);

    return ret;
  }

  /*--------------------------------------------
   function: int kbhit(void)
   usage exemple:
          while(!kbhit())
               printf("press a key to stop loop\n");

   function description:
          tests (poll) for available char in keyboard buffer
          returns true or false immediatly
  */

  int kbhit(void)
  {
      struct timeval tv;
      struct termios normalTTY, rawTTY;
      fd_set rfds;
      int ret=0;

      /* setup terminal */
      if (tcgetattr (0, &normalTTY) == -1)
        exit (-1);
      rawTTY = normalTTY;
      rawTTY.c_lflag &= ~ICANON;
      rawTTY.c_lflag &= ~ECHO;
      if (tcsetattr (0, TCSANOW, &rawTTY) == -1)
        exit (-1);

      tv.tv_sec = 0;
      tv.tv_usec = 0; /* 10e-3 sec = 1 milisec */

      /* Watch stdin (fd 0) to see when it has input. */
      FD_ZERO(&rfds);
      FD_SET(0, &rfds);

     if (select (1, &rfds, NULL, NULL, &tv) == 1)
     {
        if (FD_ISSET (0, &rfds))
           ret = 1;
     }
     else
        ret=0;

     /* restore terminal */
     if (tcsetattr (0, TCSANOW, &normalTTY) == -1)
        exit (-1);

     return ret;
  }
#endif

/* 
   retorna verdadeiro se existe algum caracter no buffer para ser lido
   return true if there are some character in the buffer to be read
*/
int pollinput (void)
{
#ifndef __win32__
    /* linux e outros */
    static fd_set readfds;
    struct timeval tv;
    int ret;

    FD_ZERO (&readfds);
    FD_SET (fileno (stdin), &readfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    select (16, &readfds, NULL, NULL, &tv);
    ret = FD_ISSET (fileno (stdin), &readfds);
    return (ret);
#else
    /* windows */
    static int init = 0;
    static int pipe;
    static HANDLE inh;
    static INPUT_RECORD Buffer[256];

    DWORD dw = 0;
    if (!init)
    {
        init = 1;
        inh = GetStdHandle (STD_INPUT_HANDLE);
        pipe = !GetConsoleMode (inh, &dw);
        if (!pipe)
        {
            SetConsoleMode (inh,
                            dw & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT));
            FlushConsoleInputBuffer (inh);
        }
    }
    if (pipe)
    {
        if (!PeekNamedPipe (inh, NULL, 0, NULL, &dw, NULL))
            return 1;
        return dw;
    }
    else
    {
        GetNumberOfConsoleInputEvents (inh, &dw);
        return (dw <= 1 ? 0 : dw);
    }
#endif
}

/* char getfrom(char *s, int turn) */
char getfrom(char *s)
{
    char r;
    r=getch();
    /* if(strchr(s,r)==NULL)
    {     printf("\nPress one letter from set '%s'.\n%d>", s, turn+1);
          printf("Press one letter from set '%s'.\n", s);
    }*/
    while(strchr(s,r)==NULL)
        r=getch();
    return r;
}

float mudaintervalo(float min1, float max1, float min2, float max2, float x1)
{
    float x2;

    if(min1==max1)
        return 0;

    x2 = x1 * ((min2 - max2)/(min1 - max1)) + max2 - max1 * ((min2 - max2)/(min1 - max1));
    return x2;
}

/* retorna inteiro entre [min,max[ */
int rand_minmax(int min, int max)
{
    static int n1=0;
    float sorteio;

    if(!n1)
    {
        time_t seed=time(NULL);
        srand(seed);
        n1=1;
    }
    sorteio=mudaintervalo(0.0, (float)RAND_MAX, (float)min, (float)max, (float)rand());
    return ((int)(sorteio));
}

/* retorna o menor entre a e b */
int min(int a, int b)
{
    return (a<b?a:b);
}
