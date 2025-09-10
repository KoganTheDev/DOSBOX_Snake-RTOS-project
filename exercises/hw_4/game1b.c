/* game1b.c - main, prntr */

// CHANGE 

#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <bios.h>
#include <sem.h>

//#include <dos.h>
//#include <stdio.h>

// END OF CHANGE

#define ARROW_NUMBER 30
#define TARGET_NUMBER 4
#define ARRSIZE 1000

//void  interrupt (*old_int9)(void);

char entered_ascii_codes[ARRSIZE];
int tail = -1;
char display[2001];

char ch_arr[ARRSIZE];
int front = -1;
int rear = -1;

int point_in_cycle;
int gcycle_length;
int gno_of_pids;

int initial_run = 1;
  int gun_position;           
  int no_of_arrows;
  int target_disp = 80/TARGET_NUMBER;
  char ch;

  int no_of_targets;
// CHANGE

extern SYSCALL  sleept(int);
extern struct intmap far *sys_imp;

// END OF CHANGE

// CHANGE

int sched_arr_pid[5] = {-1};
int sched_arr_int[5] = {-1};
int receiver_pid;
int shdpid;
int semid;

// END OF CHANGE


//void my_halt()
//{
//   setvect(9, old_int9);
//   asm {CLI}
// CHANGE

//   exit();
//  asm { INT 27 }

//} // my_halt()

// END OF CHANGE
   

// CHANGE

//void  interrupt new_int9(void)
//{
// char result = 0;
// int scan = 0;
//  int ascii = 0;

//(*old_int9)();


//asm {
//  MOV AH,1
//  INT 16h
//  JZ Skip1
//  MOV AH,0
//  INT 16h
//  MOV BYTE PTR scan,AH
//  MOV BYTE PTR ascii,AL
// } //asm

// ascii = 0;
// if (scan == 75)
//   ascii = 'a';
// else
//   if (scan == 72)
//     ascii = 'w';
//   else
//   if (scan == 77)
//      ascii = 'd';
// if ((scan == 46)&& (ascii == 3)) // Ctrl-C?
//   if (scan == 1) // Esc?
 //    my_halt(); // terminate program
// END OF CHANGE

 // if ((ascii != 0) && (tail < ARRSIZE))
 //  {
 //     entered_ascii_codes[++tail] = ascii;
 //  } // if

//Skip1:

//} // new_int9



INTPROC new_int9(int mdevno)
{
 char result = 0;
 int scan = 0;
  int ascii = 0;

asm {
  MOV AH,1
  INT 16h
  JZ Skip1
  MOV AH,0
  INT 16h
  MOV BYTE PTR scan,AH
  MOV BYTE PTR ascii,AL
 } //asm

 ascii = 0;
 if (scan == 75)
   result = 'a';
 else
   if (scan == 72)
     result = 'w';
   else
   if (scan == 77)
      result = 'd';
 //if ((scan == 46)&& (ascii == 3)) // Ctrl-C?
if (scan == 1)
   asm INT 27; // terminate xinu

  if ((result != 0) && (tail < ARRSIZE))
   {
      entered_ascii_codes[++tail] = result;
   } // if

   send(receiver_pid, result); 

Skip1:
} // new_int9


void set_new_int9_newisr()
{
  int i;
  for(i=0; i < 32; i++)
    if (sys_imp[i].ivec == 9)
    {
     sys_imp[i].newisr = new_int9;
     return;
    }

} // set_new_int9_newisr

// END OF CHANGE





typedef struct position
{
  int x;
  int y;

}  POSITION;



void displayer( void )
{

   // CHANGE

	while (1)
         {
//             receive();               
            wait(semid);
               printf(display);
         } //while
// END OF CHANGE

} // prntr

void receiver()
{
    char temp;

 // CHANGE

  while(1)
  {
   receive();
    while( tail > -1)
    {
      temp = entered_ascii_codes[tail];
      rear++;
      tail--;
      if (rear < ARRSIZE)
          ch_arr[rear] = temp;
      if (front == -1)
         front = 0;
   } // while
} // while

// END OF CHANGE

} //  receiver


char display_draft[25][80];
POSITION target_pos[TARGET_NUMBER];
POSITION arrow_pos[ARROW_NUMBER];


void updater()
{

  int i,j;


  if (initial_run == 1)
  {
    initial_run = 0;
    no_of_arrows = 0;

    no_of_targets = TARGET_NUMBER;

    gun_position = 39;



    target_pos[0].x = 3;
    target_pos[0].y = 0; 


    for(i=1; i < TARGET_NUMBER; i++)
    {
      target_pos[i].x = i*target_disp;
      target_pos[i].y = 0; 

    } // for
    for(i=0; i < ARROW_NUMBER; i++)
         arrow_pos[i].x =  arrow_pos[i].y = -1;

   } // if (initial_run ==1)

// CHANGE 
while(1)
{
//   receive();

    wait(semid);


   while(front != -1)
   {
     ch = ch_arr[front];
     if(front != rear)
       front++;
     else
       front = rear = -1;

     if ( (ch == 'a') || (ch == 'A') )
       if (gun_position >= 2 )
              gun_position--;
       else;
     else if ( (ch == 'd') || (ch == 'D') )
       if (gun_position <= 78 )
         gun_position++;
       else;
     else if ( (ch == 'w') || (ch == 'W') )
       if (no_of_arrows < ARROW_NUMBER)
       {
         arrow_pos[no_of_arrows].x = gun_position;
         arrow_pos[no_of_arrows].y = 23;
         no_of_arrows++;

       } // if
   } // while(front != -1)

     ch = 0;
     for(i=0; i < 25; i++)
        for(j=0; j < 80; j++)
            display_draft[i][j] = ' ';  // blank

    display_draft[22][gun_position] = '^';
    display_draft[23][gun_position-1] = '/';
    display_draft[23][gun_position] = '|';
    display_draft[23][gun_position+1] = '\\';
    display_draft[24][gun_position] = '|';

    for(i=0; i < ARROW_NUMBER; i++)
       if (arrow_pos[i].x != -1)
       {
         if (arrow_pos[i].y > 0)
           arrow_pos[i].y--;
           display_draft[arrow_pos[i].y][arrow_pos[i].x] = '^';
           display_draft[arrow_pos[i].y+1][arrow_pos[i].x] = '|';

       } // if

    for(i=0; i < TARGET_NUMBER; i++)
       if (target_pos[i].x != -1)
        {
         if (target_pos[i].y < 22)
              target_pos[i].y++;
         display_draft[target_pos[i].y][target_pos[i].x] = '*';
        } // if

    for(i=0; i < 25; i++)
      for(j=0; j < 80; j++)
        display[i*80+j] = display_draft[i][j];
    display[2000] = '\0';

} // while

//  END OF CHANGE

} // updater 

SYSCALL schedule(int no_of_pids, int cycle_length, int pid1, ...)
{
  int i;
  int ps;
  int *iptr;

  disable(ps);

  gcycle_length = cycle_length;
  point_in_cycle = 0;
  gno_of_pids = no_of_pids;

  iptr = &pid1;
  for(i=0; i < no_of_pids; i++)
  {
    sched_arr_pid[i] = *iptr;
    iptr++;
    sched_arr_int[i] = *iptr;
    iptr++;
  } // for
  restore(ps);

} // schedule 

void scheduler()
{
  int interval_ticks, pid_index;

      point_in_cycle++;
       if (point_in_cycle == gcycle_length)
         point_in_cycle = 0;

   interval_ticks = gcycle_length / gno_of_pids; 
   pid_index = 0;

  while(1)
  {
     sleept(interval_ticks);

//     send(sched_arr_pid[pid_index], 11);
     signal(semid);
     pid_index++;
     if (pid_index == gno_of_pids)
       pid_index = 0;
  } // while

} // scheduler

// END OF CHANGE


// CHANGE
//main()
xmain()
// END OF CHANGE
{
        int uppid, dispid, recvpid;

// CHANGE

//  old_int9 = getvect(9);
//  setvect(9, new_int9);

        semid = screate(0);
        set_new_int9_newisr();

        resume( dispid = create(displayer, INITSTK, INITPRIO, "DISPLAYER", 0) );
        resume( recvpid = create(receiver, INITSTK, INITPRIO+3, "RECIVEVER", 0) );
        resume( uppid = create(updater, INITSTK, INITPRIO, "UPDATER", 0) );
        receiver_pid = recvpid;

    schedule(2, 12, uppid,  0, dispid, 6) ;

        resume( shdpid = create(scheduler, INITSTK, INITPRIO+1, "DISPLAYER", 0) );


 // while(1)
 // {
 //    receiver();
 //    updater();
 //    displayer();
   
 //      delay(2700);
 //    sleep(1);

 // } // while

// END OF CHANGE

} // main
