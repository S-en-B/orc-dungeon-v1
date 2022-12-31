/**********************************************************************/
/*                                                                    */
/* Program Name: Orc Dungeon - A simple dungeon crawler               */
/* Author:       SenB                                                 */
/* Date Started: December 21, 2022                                    */
/* Last Upload:  December 31, 2022                                    */
/*                                                                    */
/**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

/**********************************************************************/
/*                         Symbolic Constants                         */
/**********************************************************************/
#define ENEMY_TYPE_LENGTH  10
#define PLAYER_TYPE_LENGTH 10
#define BASE_HEALTH_P      10
#define BASE_ATTACK_P       2
#define BASE_HEALTH_E       5
#define BASE_ATTACK_E       1
#define COIN                1
#define POTION              3
#define PLAYER_ALLOC_ERR    1
#define ROOM_ALLOC_ERR      2
#define ENEMY_ALLOC_ERR     3
/**********************************************************************/
/*                          Program Structures                        */
/**********************************************************************/
/* Enemy stat structure                                               */
struct enemy_stats
{
   char enemy_type[ENEMY_TYPE_LENGTH];
   int  health,
        attack;
};
typedef struct enemy_stats ENEMY;

/* Player stat structure                                              */
struct player_stats
{
   char player_type[PLAYER_TYPE_LENGTH];
   int health,
       attack,
       coins;
};
typedef struct player_stats PLAYER;

/* Room contents structure                                            */
struct room_contents
{
   int  coins,
        potions,
        enemies;
};
typedef struct room_contents ROOM;

/**********************************************************************/
/*                        Function Prototypes                         */
/**********************************************************************/
void program_instructions();
   /* Prints the program instructions, and explains the game          */
char get_response();
   /* Get response to continue or quit                                */
PLAYER *create_player();
   /* Create a new player with default stats                          */
ROOM *create_room(int rooms_cleared);
   /* Create the next room for the player to explore                  */
ENEMY *create_enemy(int rooms_cleared);
   /* Create a new enemy for the player to fight                      */
void player_turn(PLAYER *p_player, ROOM *p_room, ENEMY *p_enemy,
                                                     int rooms_cleared);
   /* Present a menu of action items                                  */
void enemy_turn(PLAYER *p_player, ENEMY *p_enemy);
   /* Process the ememies turn                                        */
void player_attack(PLAYER *p_player, ENEMY *p_enemy);
   /* Decide how strong the player attack hits                        */
void shop(PLAYER *p_player);
   /* A list of items the player can buy                              */
int player_run();
   /* Gives the player a chance to escape without fighting the Orc    */
void test_function(PLAYER *p_player, ROOM *p_room, ENEMY *p_enemy,
                                                     int rooms_cleared);

/**********************************************************************/
/*                             Main Function                          */
/**********************************************************************/
int main()
{
   PLAYER *p_player;
   ROOM   *p_room;
   ENEMY  *p_enemy;
   int rooms_cleared = 0;
   time_t t;
   srand((unsigned) time(&t));

   /* Print program instructions                                      */
   printf("\n\n\n\n");
   program_instructions();

   /* Loop running the game until the user says to start              */
   while(get_response() == 'y')
   {
      /* Create player stats */
      p_player = create_player();

      while(p_player->health > 0)
      {
         /* Create the next room and create a new enemy               */
         p_room = create_room(rooms_cleared);
         p_enemy = create_enemy(rooms_cleared);

         /* Player's turn                                             */
         printf("\n\n");
         player_turn(p_player, p_room, p_enemy, rooms_cleared);

         /* Enemy's turn                                              */
         enemy_turn(p_player, p_enemy);

         /* If the player is alive, reward them                       */
         if(p_player->health > 0)
         {
            rooms_cleared++;
            p_player->coins  += p_room->coins;
            p_player->health += (p_room->potions * 2);
         }
         else
            printf("\nThe Orc has killed you! You cleared %d rooms.",
                                                         rooms_cleared);
         /* Free memory allocated for the current room and enemy      */
         free(p_room);
         free(p_enemy);
      }

      /* Free memory given to player stats */
      free(p_player);
   }
   return 0;
}


/**********************************************************************/
/*                          Program Instructions                      */
/**********************************************************************/
void program_instructions()
{
   printf("\nWelcome to the Orc Dungeon!");
   printf("\n\nIn this game you will have to progress through a series");
   printf(  "\nof rooms. Most rooms will have an Orc that you must defeat");
   printf(  "\nEvery few rooms, your health and attack will level up");
   printf(  "\nEvery room you have a chance to recieve coins and potions");
   printf(  "\nPotions will heal you slightly and coins can be used to buy");
   printf(  "\nhealth upgrades and attack upgrades.");

   return;
}

/**********************************************************************/
/*                  Get response to continue or quit                  */
/**********************************************************************/
char get_response()
{
   char response[2]; /* Response to continue or quit the program      */
   printf("\n\n");
   do
   {
      printf("Would you like to play the game? (y or n): ");
      scanf ("%1s", response);
      response[0] = tolower(response[0]);
   }
   while (response[0] != 'y' && response[0] != 'n');
   printf("\n\n\n\n\n\n");
   return response[0];
}

/**********************************************************************/
/*               Create new player and give default stats             */
/**********************************************************************/
PLAYER *create_player()
{
   PLAYER *p_new_player; /* Points to the newly created player        */

   if ((p_new_player = (PLAYER *) malloc(sizeof(PLAYER))) == NULL)
   {
      printf("\nError %d occured in create_player.", PLAYER_ALLOC_ERR);
      printf("\nCannot allocate memory for the player.");
      printf("\nThe game is ending and program is aborting.");
      exit (PLAYER_ALLOC_ERR);

   }
   p_new_player->health = BASE_HEALTH_P;
   p_new_player->attack = BASE_ATTACK_P;
   return p_new_player;
}

/**********************************************************************/
/*                Create rooms for the player to explore              */
/**********************************************************************/
ROOM *create_room(int rooms_cleared)
{
   ROOM *p_new_level; /* Points to the newly created room             */
   int enemy_chance = 0;

   if ((p_new_level = (ROOM *) malloc(sizeof(ROOM))) == NULL)
   {
      printf("\n\nError %d occured in create_rooms.", ROOM_ALLOC_ERR);
      printf(  "\nCannot allocate memory for the player.");
      printf(  "\nThe game is ending and program is aborting.");
      exit (ROOM_ALLOC_ERR);
   }

   p_new_level->coins   = ((rand() % 8) + 2);
   p_new_level->potions = ((rand() % 2) + 1);
   enemy_chance = ((rand() % 10) + 1);
   if(enemy_chance > 1)
      p_new_level->enemies = 1;
   else
      p_new_level->enemies = 1;

   return p_new_level;
}

/**********************************************************************/
/*               Create new enemy and give default stats              */
/**********************************************************************/
ENEMY *create_enemy(int rooms_cleared)
{
   ENEMY *p_new_enemy; /* Points to the newly created enemy          */

   if ((p_new_enemy = (ENEMY *) malloc(sizeof(ENEMY))) == NULL)
   {
      printf("\n\nError %d occured in create_enemy.", ENEMY_ALLOC_ERR);
      printf(  "\nCannot allocate memory for the enemy.");
      printf(  "\nThe game is ending and program is aborting.");
      exit (ENEMY_ALLOC_ERR);

   }
   p_new_enemy->health = (BASE_HEALTH_E + (rooms_cleared % 5));
   p_new_enemy->attack = (BASE_ATTACK_E + (rooms_cleared % 5));
   return p_new_enemy;
}

/**********************************************************************/
/*                          Player action menu                        */
/**********************************************************************/
void player_turn(PLAYER *p_player, ROOM *p_room, ENEMY *p_enemy, int rooms_cleared)
{
   int player_move;

   printf("\nYou have entered room %d", (rooms_cleared + 1));
   if(p_room->enemies != 1)
      printf("\nThere are no Orcs in this room");
   else
   {
      printf("\nAn Orc blocks your path!");
      printf("\nThere are %d coins and %d potions in this room",
                                        p_room->coins, p_room->potions);
      while(p_enemy->health > 0 && p_player->health > 0)
      {
         do
         {
         printf("\n");
         printf("\nPlayer HP: %2d  ATK: %2d  $%d", p_player->health,
                                     p_player->attack, p_player->coins);
         printf("\nEnemy  HP: %2d  ATK: %2d", p_enemy->health,
                                                       p_enemy->attack);
         printf("\nAttack: 1   Block:    2");
         printf("\nShop:   3   Run Away: 4");
         printf("\nWhat do you want to do? ");
         scanf("%d", &player_move);
         }
         while(player_move < 1 && player_move > 4);
         {
            switch(player_move)
            {
               case 1:
                  printf("\nYou raise your sword in an offensive position.");
                  printf("\nYou dealt %d damage", p_player->attack);
                  printf("\nThe Orc had %d health", p_enemy->health);
                  player_attack(p_player, p_enemy);
                  printf(" and now has %d health", p_enemy->health);
                  break;

               case 2:
                  printf("\nYou raise your sword in a defensive position.");

                  break;

               case 3:
                  printf("\nYou begin to think out what you could buy.");
                  shop(p_player);
                  break;

               default:
                  printf("\nYou try to run past the Orc");
                  if(player_run())
                  {
                     printf("\nYou got past the Orc and made it to the door!");
                     printf("\nIn your haste you could not grab any treasure");
                     p_enemy->health = 0;
                  }
                  else
                     printf("\nThe Orc prevented you from running!");
                  break;
            }
         }
      }
   }
   return;
}

/**********************************************************************/
/*                            Enemy's Turn                            */
/**********************************************************************/
void enemy_turn(PLAYER *p_player, ENEMY *p_enemy)
{


   if(p_enemy->health > 0)
   {
      if(((rand() % 10)) >= 5)
      {
         p_player->health -= p_enemy->attack;
         printf("\nThe Orc struck you. He did %d damage", p_enemy->attack);
      }
      else
         printf("\nYou blocked the Orc's attack");
   }
   return;
}

/**********************************************************************/
/*                       Player attack command                        */
/**********************************************************************/
void player_attack(PLAYER *p_player, ENEMY *p_enemy)
{
   p_enemy->health -= p_player->attack;
   if(p_enemy->health < 0)
      p_enemy->health = 0;
   return;
}

/**********************************************************************/
/*                       Player attack command                        */
/**********************************************************************/
void shop(PLAYER *p_player)
{
   int shop_choice;
   printf("\n\n");
   do
   {
      printf("\nYou have %d coins", p_player->coins);
      printf("\n1: +5 Health (20 coins)");
      printf("\n2: +1 Attack (20 coins)");
      printf("\n3: \"Magic Coin\"  (100 coins)");
      printf("\nWhat would you like to buy? ");
      scanf("%d", &shop_choice);
   }
   while(shop_choice < 1 && shop_choice > 3);

   switch(shop_choice)
   {
      case 1:
         if(p_player->coins >= 10)
         {
            printf("\nHealth upgraded from %d to %d", p_player->health,
                                                (p_player->health + 5));
            p_player->health += 5;
            p_player->coins -= 10;
         }
         else
            printf("\nYou do not have enough coins for that item");
         break;
      case 2:
         if(p_player->coins >= 10)
         {
            printf("\nAttack upgraded from %d to %d", p_player->attack,
                                                (p_player->attack + 1));
            p_player->attack += 5;
            p_player->coins -= 10;
         }
         else
            printf("\nYou do not have enough coins for that item");
         break;
      default:
         if(p_player->coins >= 100)
         {
            printf("\nYou purchased a \"Magic Coin\". Looking closely, you");
            printf("\ncan see the word \"Escape\" on the back of the coin.");
            printf("\nYou feel your body rising through the ground as you ");
            printf("\nfinally escape the caves.");
            printf("\n\n\n\n\n\n");
            exit(0);
         }
         else
            printf("\nYou do not have enough coins for that item");
         break;
   }
   return;
}

/**********************************************************************/
/*                       Player attack command                        */
/**********************************************************************/
int player_run()
{
   int escape_chance;
   escape_chance = ((rand() % 20) + 1);
   return(escape_chance < 3);
}
