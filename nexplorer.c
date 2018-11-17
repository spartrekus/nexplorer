
//////////////////
//     NEXPLORER
//////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include <curses.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h> 
#include <time.h> 


int rows, cols; 
int  curs_y;
int  curs_x;
int  user_sel = 1;
int  user_scrolly =0 ;
char user_fileselection[PATH_MAX]; 
char filefilter[PATH_MAX]; 
int  tc_det_dir_type = 0;

void gfxbox( int y1, int x1, int y2, int x2 )
{
    int fooy, foox; 
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
    for( foox = x1 ; foox <= x2 ; foox++) 
         mvaddch( fooy , foox , ' ' );
}



void mvcenter( int myposypass, char *mytext )
{
   mvprintw( myposypass, cols/2 - strlen( mytext )/2  , "%s", mytext );
}


///////////////////////////////
///////////////////////////////
///////////////////////////////
char *strtimestamp()
{
      long t;
      struct tm *ltime;
      time(&t);
      ltime=localtime(&t);
      char charo[50];  int fooi ; 
      fooi = snprintf( charo, 50 , "%04d%02d%02d%02d%02d%02d",
	1900 + ltime->tm_year, ltime->tm_mon +1 , ltime->tm_mday, 
	ltime->tm_hour, ltime->tm_min, ltime->tm_sec 
	);
    size_t siz = sizeof charo ; 
    char *r = malloc( sizeof charo );
    return r ? memcpy(r, charo, siz ) : NULL;
}
char *strrlf(char *str)
{  // seems to work
      char ptr[strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {
        if (str[i] != '\n' && str[i] != '\n') 
        ptr[j++]=str[i];
      } 
      ptr[j]='\0';
      size_t siz = sizeof ptr ; 
      char *r = malloc( sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}


char *strcut( char *str , int myposstart, int myposend )
{  
      char ptr[strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {
        if ( ( str[i] != '\0' ) && ( str[i] != '\0') )
         if ( ( i >=  myposstart-1 ) && (  i <= myposend-1 ) )
           ptr[j++]=str[i];
      } 
      ptr[j]='\0';
      size_t siz = sizeof ptr ; 
      char *r = malloc( sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}






////////////////////////////////
void ncurses_runwith( char *thecmd , char *thestrfile  ) //thecmd first
{
       char cmdi[PATH_MAX];
       def_prog_mode();
       endwin();
       printf( "<APP-CMD: Command...>\n" );
       strncpy( cmdi , "  " , PATH_MAX );
       strncat( cmdi , thecmd , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , " " , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , " \"" , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , thestrfile , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , "\" " , PATH_MAX - strlen( cmdi ) -1 );
       printf( "<APP-CMD: %s>\n", cmdi );
       system( cmdi );
       reset_prog_mode();
}




////////////////////////
////////////////////////
////////////////////////
////////////////////////
char *strninput( char *myinitstring )
{

   int strninput_gameover = 0; 
   char strmsg[PATH_MAX];
   char charo[PATH_MAX];
   strncpy( strmsg, myinitstring , PATH_MAX );

   int ch ;  int foo ; 
   int fooj; 
   while ( strninput_gameover == 0 )
   {
                  getmaxyx( stdscr, rows, cols);
                  attroff( A_REVERSE );
                  for ( fooj = 0 ; fooj <= cols-1;  fooj++)
                  {
                    mvaddch( rows-1, fooj , ' ' );
                  }
                  mvprintw( rows-1, 0, ":> %s" , strrlf( strmsg ) );
                  attron( A_REVERSE );
                  printw( " " );
                  attroff( A_REVERSE );
                  attroff( A_REVERSE );
                  attroff( A_BOLD );
                  refresh() ; 

                  curs_set( 0 );
                  ch = getch();

  if ( ch == 8 ) 
    strncpy( strmsg, strcut( strmsg, 1 , strlen( strmsg ) -1 )  ,  PATH_MAX );

   else if ( ch == 20 ) strncpy( strmsg, strtimestamp() ,  PATH_MAX );  //ctrl+t

   else if ( ch == 4 ) 
     strncat( strmsg ,  strtimestamp()  , PATH_MAX - strlen( strmsg ) -1 );  //ctrl+d for dateit

  else if ( ch == 2 ) strncpy( strmsg,  ""  ,  PATH_MAX );  // bsd

  else if ( ch == 27 )  
  {
    strncpy( strmsg, ""  ,  PATH_MAX );
    strninput_gameover = 1;
  }

  else if ( ch == 274 )  
  {
    strncpy( strmsg, ""  ,  PATH_MAX );
    strninput_gameover = 1;
  }

  else if ( ch == 263 )  
    strncpy( strmsg, strcut( strmsg, 1 , strlen( strmsg ) -1 )  ,  PATH_MAX );



		  else if ( ch == 4 ) 
		  {
			   strncpy( strmsg, ""  ,  PATH_MAX );
		  }
		  else if ( ch == 27 ) 
		  {
			   strncpy( strmsg, ""  ,  PATH_MAX );
		  }
	          else if (
			(( ch >= 'a' ) && ( ch <= 'z' ) ) 
		        || (( ch >= 'A' ) && ( ch <= 'Z' ) ) 
		        || (( ch >= '1' ) && ( ch <= '9' ) ) 
		        || (( ch == '0' ) ) 
		        || (( ch == '~' ) ) 
		        || (( ch == '!' ) ) 
		        || (( ch == '&' ) ) 
		        || (( ch == '=' ) ) 
		        || (( ch == ':' ) ) 
		        || (( ch == ';' ) ) 
		        || (( ch == '<' ) ) 
		        || (( ch == '>' ) ) 
		        || (( ch == ' ' ) ) 
		        || (( ch == '|' ) ) 
		        || (( ch == '#' ) ) 
		        || (( ch == '?' ) ) 
		        || (( ch == '+' ) ) 
		        || (( ch == '/' ) ) 
		        || (( ch == '\\' ) ) 
		        || (( ch == '.' ) ) 
		        || (( ch == '$' ) ) 
		        || (( ch == '%' ) ) 
		        || (( ch == '-' ) ) 
		        || (( ch == ',' ) ) 
		        || (( ch == '{' ) ) 
		        || (( ch == '}' ) ) 
		        || (( ch == '(' ) ) 
		        || (( ch == ')' ) ) 
		        || (( ch == ']' ) ) 
		        || (( ch == '[' ) ) 
		        || (( ch == '*' ) ) 
		        || (( ch == '"' ) ) 
		        || (( ch == '@' ) ) 
		        || (( ch == '-' ) ) 
		        || (( ch == '_' ) ) 
		        || (( ch == '^' ) ) 
		        || (( ch == '\'' ) ) 
	             ) 
		  {
                           foo = snprintf( charo, PATH_MAX , "%s%c",  strmsg, ch );
			   strncpy( strmsg,  charo ,  PATH_MAX );
		  }
		  else if ( ch == 10 ) 
		  {
                        strninput_gameover = 1;
		  }
     }
     char ptr[PATH_MAX];
     strncpy( ptr, strmsg, PATH_MAX );
     size_t siz = sizeof ptr ; 
     char *r = malloc( sizeof ptr );
     return r ? memcpy(r, ptr, siz ) : NULL;
}








/////////////////////////////////
/////////////////////////////////
/////////////////////////////////
int fexist(char *a_option)
{
  char dir1[PATH_MAX]; 
  char *dir2;
  DIR *dip;
  strncpy( dir1 , "",  PATH_MAX  );
  strncpy( dir1 , a_option,  PATH_MAX  );

  struct stat st_buf; 
  int status; 
  int fileordir = 0 ; 

  status = stat ( dir1 , &st_buf);
  if (status != 0) {
    fileordir = 0;
  }

  // this is compatible to check if a file exists
  FILE *fp2check = fopen( dir1  ,"r");
  if( fp2check ) {
  // exists
  fileordir = 1; 
  fclose(fp2check);
  } 

  if (S_ISDIR (st_buf.st_mode)) {
    fileordir = 2; 
  }
return fileordir;
/////////////////////////////
}



///////////////// new
char *fextension(char *str)
{ 
    char ptr[strlen(str)+1];
    int i,j=0;
    //char ptrout[strlen(ptr)+1];  
    char ptrout[25];

    if ( strstr( str, "." ) != 0 )
    {
      for(i=strlen(str)-1 ; str[i] !='.' ; i--)
      {
        if ( str[i] != '.' ) 
            ptr[j++]=str[i];
      } 
      ptr[j]='\0';

      j = 0; 
      for( i=strlen(ptr)-1 ;  i >= 0 ; i--)
            ptrout[j++]=ptr[i];
      ptrout[j]='\0';
    }
    else
     ptrout[0]='\0';

    size_t siz = sizeof ptrout ; 
    char *r = malloc( sizeof ptrout );
    return r ? memcpy(r, ptrout, siz ) : NULL;
}






////////////////////////////////
void nruncmd( char *thecmd   )
{
       char cmdi[PATH_MAX];
       def_prog_mode();
       endwin();
       strncpy( cmdi , "  " , PATH_MAX );
       strncat( cmdi , thecmd , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , " " , PATH_MAX - strlen( cmdi ) -1 );
       system( cmdi );
       reset_prog_mode();
}




///////////////////////////////////
void colornorm(void);
void crossgraphvga_init(void)
{
  initscr();			
  keypad(stdscr, true);
  noecho();
  curs_set( 0 );
  start_color();

  /// basic ncurses colors
  init_pair(0, COLOR_BLACK, COLOR_BLACK);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_WHITE);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_CYAN, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_WHITE,  COLOR_BLACK);

  /// extra tc colors
  init_pair(8, COLOR_BLUE,   COLOR_YELLOW);
  init_pair(9, COLOR_YELLOW, COLOR_BLUE);

  init_pair(10, COLOR_BLUE, COLOR_YELLOW);
  init_pair(11, COLOR_BLUE, COLOR_WHITE);
  init_pair(12, COLOR_BLUE, COLOR_MAGENTA);
  init_pair(13, COLOR_CYAN, COLOR_BLACK);
  init_pair(14, COLOR_BLUE, COLOR_CYAN);

}



void crossgraphvga_colors(void)
{
  color_set( 7 , NULL ); attroff( A_REVERSE ); mvprintw( 2,2, "Darker  ;  Brighter (bold)" );
  getmaxyx( stdscr, rows, cols );

  int x = 5; int i = 1;
  for( i = 0; i <= rows-1 ; i++)  //ncurses as by default 8 colors
  { 
    colornorm(); 

    color_set( i , NULL );
    mvprintw( x, x , "%d ", i );

    attron( A_REVERSE );
    attroff( A_BOLD );
    color_set( i , NULL );
    printw( "%d", i );
    colornorm(); printw( " " );

    attroff( A_REVERSE );
    attron(  A_BOLD );
    color_set( i , NULL );
    printw( " ; %d", i );
    colornorm(); printw( " " );

    // reverse and bold
    color_set( i , NULL );
    attron( A_REVERSE );
    attroff( A_BOLD ); //for the console
    attron(  A_BLINK ); //for the console
    printw( " R%d", i );
    colornorm(); printw( " " );


    color_set( i , NULL );
    attron(  A_REVERSE );
    attron(  A_BOLD ); //for the console
    attroff( A_BLINK ); //for the console
    printw( " Rev,Bold%d", i );
    colornorm(); printw( " " );

    color_set( i , NULL );
    attron(  A_REVERSE );
    attron(  A_BOLD ); //for the console
    attron(  A_BLINK ); //for the console
    printw( " Rev,Bold,Blink%d", i );
    colornorm(); printw( " " );

    x += 1; 
    colornorm(); printw( " " );
    color_set( 7 , NULL ); printw( " (#.%d)", i );

    mvprintw( rows-1, cols-8, "|%d,%d|" , rows, cols );
  } 

  refresh();
  x += 2; 
  attroff( A_REVERSE );
  color_set( 7, NULL ); mvprintw(  x, 5 , "<Press Key>" );
}





void colornorm(void)
{
    attroff(  A_REVERSE );
    attroff( A_BOLD );
    attroff( A_BLINK );
    attroff( A_UNDERLINE );
    color_set( 7, NULL ); 
}





void gfxhline( int y1, int x1, int x2 )
{
    int foox ;
    for( foox = x1 ; foox <= x2 ; foox++) 
        mvaddch( y1 , foox , ' ' );
}








/////////////////////////
/////////////////////////
/////////////////////////
void printdir()
{
   DIR *dirp; int posy = 0;  int posx, chr ; 
   int fooselection = 0;
   posy = 1; posx = cols/2;
   char cwd[PATH_MAX];
   struct dirent *dp;
   dirp = opendir( "." );
   attron( A_REVERSE );

   color_set( 10, NULL );
   attroff( A_REVERSE );
   gfxhline( 0, 0, cols-1 );
   //mvprintw( 0, cols/2, "[DIR: %s]", getcwd( cwd, PATH_MAX ) );
   mvprintw( 0, 0, "[DIR: %s]", getcwd( cwd, PATH_MAX ) );

   strncpy( user_fileselection, "", PATH_MAX );

   attron( A_REVERSE );
   color_set( 11, NULL );

   int entrycounter = 0;
   while  ((dp = readdir( dirp )) != NULL ) 
   if ( posy <= rows-3 )
   {
        entrycounter++;
        color_set( 11, NULL ); attron( A_REVERSE ); attroff( A_BOLD );

        if ( entrycounter <= user_scrolly )
              continue;

        if ( strcmp( filefilter , "" ) != 0 )
        { 
               if ( strstr( dp->d_name, filefilter ) != 0 )
               //     strncpy( idata[ n++ ] , dp->d_name , 250 );
               {
               }
               else
                  continue;
        }
        //else  
        //      strncpy( idata[ n++ ] , dp->d_name , 250 );


        if (  dp->d_name[0] !=  '.' ) 
        if (  strcmp( dp->d_name, "." ) != 0 )
        if (  strcmp( dp->d_name, ".." ) != 0 )
        {
            posy++;  fooselection++;

            if ( dp->d_type == DT_DIR ) 
            {
                 color_set( 10 , NULL );
                 mvaddch( posy, posx++ , '/' );
            }
            else if ( dp->d_type == 0 )
            {
               if ( tc_det_dir_type == 1 )
               if ( fexist( dp->d_name ) == 2 )
               {
                 color_set( 10 , NULL );
                 mvaddch( posy, posx++ , '/' );
               }
            }

            if ( user_sel == fooselection ) 
            {
                  strncpy( user_fileselection, dp->d_name , PATH_MAX );
                  color_set( 13, NULL );
            }

            for ( chr = 0 ;  chr <= strlen(dp->d_name) ; chr++) 
            {
              if  ( dp->d_name[chr] == '\n' )
                  posx = cols/2;
              else if  ( dp->d_name[chr] == '\0' )
                  posx = cols/2;
              else
                 mvaddch( posy, posx++ , dp->d_name[chr] );
            }
        }
   }
   closedir( dirp );

   color_set( 10, NULL );
   attroff( A_REVERSE );
   gfxhline( rows-1, 0, cols-1 );
   //mvprintw( rows-1, cols/2, "[FILE: %s]", user_fileselection );
   mvprintw( rows-1, 0, "[FILE: %s]", user_fileselection );
}








char *fbasename(char *name)
{
  char *base = name;
  while (*name)
    {
      if (*name++ == '/')
	{
	  base = name;
	}
    }
  return (base);
}






void gfxframe( int y1, int x1, int y2, int x2 )
{
    int foo, fooy , foox ;
    foo = x1;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
        mvaddch( fooy , foo , ACS_VLINE );
    foo = x2;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
         mvaddch( fooy , foo , ACS_VLINE );
    foo = y1;
    for( foox = x1 ; foox <= x2 ; foox++) 
         mvaddch( foo , foox , ACS_HLINE );
    foo = y2;
    for( foox = x1 ; foox <= x2 ; foox++) 
         mvaddch( foo , foox , ACS_HLINE );
    mvaddch( y1 , x1 , ACS_ULCORNER );
    mvaddch( y1 , x2 , ACS_URCORNER );
    mvaddch( y2 , x1 , ACS_LLCORNER );
    mvaddch( y2 , x2 , ACS_LRCORNER );
}





   ////////////////////////////////////
   void printfile( char *ttuxfile )
   {
       int posyy = 2 ;  
       int zui = 0 ;  FILE *fpsource;  char ptrout[PATH_MAX];
       //color_set( 7, NULL );  
       attron( A_REVERSE );
       gfxbox( 0, cols/2-2, rows-1, cols-1 );
       gfxframe( 0, cols/2-2, rows-1, cols-1 );
           if ( fexist( ttuxfile ) == 1 )
           {  
             mvprintw( 0, cols/2, "[FILE: %s]", fbasename( ttuxfile ) );
             posyy = 2;
             fpsource = fopen( ttuxfile , "r");
             while(  !feof(fpsource) )
             {
                 if ( !feof(fpsource) )
                 {
                   fgets( ptrout , PATH_MAX , fpsource ); 
                   if ( posyy <= rows-3 )
                    for(zui=0; ptrout[zui]!='\0'; zui++)
                     if ( ptrout[zui] != '\n' )
                      if ( zui <=  cols/2 -2 )
                        mvprintw( posyy, cols/2 + zui , "%c", ptrout[zui] );
                   posyy++;
                 }
             }
             fclose(fpsource);
           }
   }



/////////////////////////////////////
/////////////////////////////////////
void printfile_viewer( char *filex )
{
    char foostr[PATH_MAX];
    char foocwd[PATH_MAX];
    int ch ; 
                      strncpy( foostr , getcwd( foocwd, PATH_MAX ), PATH_MAX );
                      if ( fexist( filex ) == 1 )
                      {
                        color_set( 14, NULL ); attron( A_BOLD ); attron( A_REVERSE );
                        printfile( filex ); 
                        ch = getch(); 
                        color_set( 0, NULL ); attroff( A_BOLD ); attroff( A_REVERSE );
                        if       ( ch == 'v' )  ncurses_runwith( " vim " , filex );
                        else if  ( ch == 'n' )  ncurses_runwith( " screen -d -m nedit  " , filex );
                        else if  ( ch == 'r' )  ncurses_runwith( " tcview  " , filex );
                        else if  ( ch == '!' )  ncurses_runwith( strninput("") , filex );
                      }
                      chdir( foostr );
}





//////////////////////////////////////////////
//////////////////////////////////////////////
//////////////////////////////////////////////
int main( int argc, char *argv[])
{ 
   if ( argc >= 2)
    if ( strcmp( argv[1] , "" ) !=  0 )
     chdir( argv[ 1 ] );

  int printdir_show = 1;
  crossgraphvga_init();
  char foocmd[PATH_MAX];
  strncpy( user_fileselection, "", PATH_MAX );
  strncpy( filefilter, "", PATH_MAX );

  getmaxyx( stdscr, rows, cols );
  int i, j; int gameover = 0;
  int posy ; 
  int ch = 0; 
  ch = 0;
  while( gameover == 0)
  {
     getmaxyx( stdscr, rows, cols);

     if ( user_sel <= 1 ) user_sel = 1;
     if ( user_scrolly <= 0 ) user_scrolly = 0;

     attroff( A_BOLD );
     attron( A_REVERSE );
     color_set( 3, NULL );

     erase();
     for ( j = 0 ; j <= rows-1  ; j++) 
      for ( i = 0 ;  i <= cols-1  ; i++) 
        mvaddch( j, i , ' ' );

      mvaddch( curs_y, curs_x, 'O' );
      posy = curs_y;
      mvprintw( posy++, curs_x, "-----------------------------");
      color_set( 10 , NULL ); attron( A_BOLD );
      posy++;
      mvprintw( posy++, curs_x, " ** NEXPLORER ** ");
      attroff( A_BOLD ); color_set( 3 , NULL ); 
      mvprintw( posy++, curs_x, "-----------------------------");
      mvprintw( posy++, curs_x, "s: Show dir type (val: %d)", tc_det_dir_type);
      mvprintw( posy++, curs_x, "q: Quit");
      mvprintw( posy++, curs_x, "?: Help");

      color_set( 10, NULL );
      if ( printdir_show == 1 ) printdir();

      ch = getch();
      switch( ch )
      { 

          case KEY_F(1):
          case KEY_F(10):
          case 'q':
          case 'i':
             gameover =1 ;
             break; 

          case KEY_PPAGE:
             user_scrolly = user_scrolly-4;
             break; 
          case KEY_NPAGE:
             user_scrolly = user_scrolly+4;
             break; 

          case 'u':
             user_scrolly = user_scrolly-4;
             break; 

          case 'd':
          case 'n':
             user_scrolly = user_scrolly+4;
             break; 


          case 'k':
             user_sel--;
             break; 
          case 'j':
             user_sel++;
             break; 


           case KEY_END:
           case 'G':
             curs_x = cols-2 -6 ;
             curs_y = rows-2 ;
             break; 

           case KEY_DOWN:
             curs_y +=1 ;
             break; 
           case KEY_UP:
             curs_y -=1 ;
             break; 
           case KEY_RIGHT:
             curs_x +=1 ;
             break; 
           case KEY_LEFT:
             curs_x -=1 ;
             break; 

           case 'c':
             erase();
             crossgraphvga_colors();
             getch();
             break; 

           case 'w':
             if ( printdir_show == 0 )
             printdir_show = 1;
             else
             printdir_show = 0;
             break; 

           case KEY_BACKSPACE:
           case 'h':
              chdir( ".." ); 
              user_sel = 1;
              user_scrolly =0 ;
              break;
           case 'l':
              chdir( user_fileselection ); 
              user_sel = 1;
              user_scrolly =0 ;
              break;

           case 10:
              if ( fexist( user_fileselection ) == 2 )
              {
                  chdir( user_fileselection ); 
                  user_sel = 1;
                  user_scrolly =0 ;
              }
              else if ( fexist( user_fileselection ) == 1 )
              {
                if ( strcmp( fextension( user_fileselection ) , "png" ) == 0 )
                       ncurses_runwith( " feh -FZ   " , user_fileselection ); 
                else if ( strcmp( fextension( user_fileselection ) , "PNG" ) == 0 )
                       ncurses_runwith( " feh -FZ   " , user_fileselection ); 
                else if ( strcmp( fextension( user_fileselection ) , "JPG" ) == 0 )
                       ncurses_runwith( " feh -FZ   " , user_fileselection ); 

                else if ( strcmp( fextension( user_fileselection ) , "jpg" ) == 0 )
                       ncurses_runwith( " feh -FZ   " , user_fileselection ); 

                else if ( strcmp( fextension( user_fileselection ) , "eps" ) == 0 )
                       ncurses_runwith( " epsview   " , user_fileselection ); 

                else if ( strcmp( fextension( user_fileselection ) , "pdf" ) == 0 )
                {
                     if ( strcmp( getenv( "TERM" ) , "linux" ) == 0 ) 
                       ncurses_runwith( " fbgs -r 270  " , user_fileselection ); 
                     else
                       ncurses_runwith( " screen -d -m mupdf " , user_fileselection ); 
                }
              }
              break;


           case KEY_HOME:
           case 'g':
             curs_x =1 ;
             curs_y =1 ;
             user_sel =1 ;
             user_scrolly =0 ;
             break; 

           case 15:
              strncpy( foocmd , strninput("") , PATH_MAX ); 
              chdir( foocmd );
              break;

           case ':':
              strncpy( foocmd , strninput("") , PATH_MAX ); 
              erase();
              attron( A_REVERSE ); color_set( 3, NULL );
              gfxbox( 0,0, rows-1,cols -1 );
              if       ( strcmp( foocmd, "ndesk" ) == 0 )  
                     nruncmd( " ndesk " );
              else if  ( strcmp( foocmd, "bash" ) == 0 )  
                     nruncmd( " bash  " );
              else if  ( strcmp( foocmd, "nbash" ) == 0 )  
                     nruncmd( " nbash  " );
              else if  ( strcmp( foocmd, "lotus" ) == 0 )  
                     nruncmd( " freelotus123  " );
              else if  ( strcmp( foocmd, "nc" ) == 0 )  
                     nruncmd( " nc  " );
              else if  ( strcmp( foocmd, "display" ) == 0 )  
                   { mvcenter( 0, "INFORMATION" ); mvprintw( 2, 0, "DISPLAY: %d %d", rows, cols ); getch();   }
              else if  ( strcmp( foocmd, "home" ) == 0 )  
                   chdir( getenv( "HOME" ) );
              break;

           case '/':
              chdir( "/" );
              break;
           case '~':
              chdir( getenv( "HOME" ) );
              break;

           case '!':
              ncurses_runwith( strninput("") , user_fileselection ); 
              break;
           case '$':
              nruncmd( strninput("") );
              break;

           case 'o':
              if ( fexist( user_fileselection ) == 1 ) 
                printfile_viewer(  user_fileselection ); 
              break;

           case 'r':
              ncurses_runwith( " tcview  " , user_fileselection ); 
              break;

           case 'v':
              ncurses_runwith( " vim  " , user_fileselection ); 
              break;
           case 's':
              if ( tc_det_dir_type == 0 ) 
                tc_det_dir_type = 1;
              else
                tc_det_dir_type = 0;
              break;

           case 'f':
             attroff( A_REVERSE ); 
             strncpy( filefilter, strninput( "" ), PATH_MAX );
             //selection = 0;  scrolly=0;
             //loadlist();
             curs_x =1 ;
             curs_y =1 ;
             user_sel =1 ;
             user_scrolly =0 ;
             break;

      }
  }


  curs_set( 1 );
  endwin();			/* End curses mode		  */
  return(0);
}






