#include <raylib.h> // for the graphics
#include <stdlib.h> //for the random starting speed
#include <math.h> //for the power function
#include <time.h> // for time(null)

struct pawn{
    float x;
    float y;
    float speed;
    float radius;
};

struct ball{
    float x;
    float y;
    float speedX;
    float speedY;
    float accel;
    float radius;
}; // accel for acceleration



/* pawnMovement: controls the movement of the pawns , uses pointers so that the variables of our pawns can change*/
void pawnMovement(struct pawn *p1,struct pawn *p2,struct pawn *gp1,struct pawn *gp2);

/* pawnCollision: check's if pawn's go through each other and does not allow it*/
void pawnCollision(struct pawn *p1,struct pawn *p2,struct pawn *gp1,struct pawn *gp2);

/* ballCollision: this function is responsible for the balls movement when interacting(colliding) with the pawns */
void ballCollision(struct pawn *p1,struct pawn *p2,struct pawn *gp1,struct pawn *gp2,struct ball *b);

/* boundaries: this function is responsible for the boundaries that the pawns and the ball have around the screen*/
void boundaries(struct pawn *p1,struct pawn *p2,struct pawn *gp1,struct pawn *gp2,struct ball *b);

/* ballMovement: controls the position, speed and acceleration of the ball */
void ballMovement(struct ball *b);

/* isgoalscored: this function is responsible for reseting the pawns and the ball after a goal is scored */
void isgoalscored(struct pawn *p1,struct pawn *p2,struct pawn *gp1,struct pawn *gp2,struct ball *b);

/* gamegraphics: visualizes the game in the screen */
void gamegraphics(struct pawn player1,struct pawn player2, struct pawn gkeeperplayer1,struct pawn gkeeperplayer2,struct ball ball,bool exitWindowRequested);



int main(void)
{
    /* Screen's size */
    const int screenWidth = 2800;
    const int screenHeight = 1600;

    /* Initialization of the Game */
    InitWindow(screenWidth,screenHeight,"E.C.E. AUTh Football Club");
    SetWindowState(FLAG_VSYNC_HINT);

    /* Declaring the game's objects */
    struct pawn player1;
    struct pawn gkeeperplayer1;
    struct pawn player2;
    struct pawn gkeeperplayer2;
    struct ball ball;

    /* Initializing our objects variables */

    /* Ball start variable values*/
    ball.radius = 20;
    ball.x = screenWidth / 2;
    ball.y = screenHeight / 2;
    ball.accel = 350;
    
    /* Players starting values */
    player1.x = (screenWidth / 2) - 475;
    player1.y = screenHeight / 2;
    player1.speed = 400;
    player1.radius = 45;

    player2.x = (screenWidth / 2) + 475;
    player2.y = screenHeight / 2;
    player2.speed = 400;
    player2.radius = 45;

    /* Goalkeepers starting values */
    gkeeperplayer1.x =  (screenWidth / 2) - 1100;
    gkeeperplayer1.y = screenHeight / 2;
    gkeeperplayer1.speed = 200;
    gkeeperplayer1.radius = 30;

    gkeeperplayer2.x =  (screenWidth / 2) + 1100;
    gkeeperplayer2.y = screenHeight / 2;
    gkeeperplayer2.speed = 200;
    gkeeperplayer2.radius = 30;

    /* Random starting ball speed */
    srand(time(NULL));
    int random = rand() % 2;
    /* if random = 0, negative speed */
    /* if random = 1, positive speed */
    /* random = 0: blue has ball*/
    if(random == 0)
        ball.speedX = -300;
    else /* random = 1: red has ball*/
        ball.speedX = 300;


    /* Tools for a nice exit screen */
    SetExitKey(KEY_NULL);       // Disable KEY_ESCAPE to close window, X-button still works
    
    bool exitWindowRequested = false;   // Flag to request window to exit
    bool exitWindow = false;    // Flag to set window to exit



    /* Game loop */
    while(!exitWindow)
    {

        /* For the exit screen */
        if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) exitWindowRequested = true;
        if (exitWindowRequested)
        {
            // A request for close window has been issued, we can save data before closing
            // or just show a message asking for confirmation
            
            if (IsKeyPressed(KEY_Y)) exitWindow = true;
            else if (IsKeyPressed(KEY_N)) exitWindowRequested = false;
        }

    /*-----------------------------------------------------------------------------------*/

        /* Pawns movement */
        /* If a goal has been scored, pawns can't move. If it hasn't, they can */
        if(!(((ball.x + ball.radius) < 200 - 15) || ((ball.x - ball.radius) > 2600 + 15)))
            pawnMovement(&player1,&player2,&gkeeperplayer1,&gkeeperplayer2);
        
        /* Checking if pawns interact //  Not allowing enemy pawns to go through each other */
        /* Ally pawns can pass through each other */
        pawnCollision(&player1,&player2,&gkeeperplayer1,&gkeeperplayer2);
        
        /* Ball's Movement */
        ballMovement(&ball);

        /* Setting up the boundaries for the ball and the pawns */
        boundaries(&player1,&player2,&gkeeperplayer1,&gkeeperplayer2,&ball);

        /* Ball's collisions with the pawns */
        ballCollision(&player1,&player2,&gkeeperplayer1,&gkeeperplayer2,&ball);

        /* Reseting positions and restarting the game if a goal is scored*/
        isgoalscored(&player1,&player2,&gkeeperplayer1,&gkeeperplayer2,&ball);

    


    /*-------------------------------------------------------------------------------*/
        /* Draw everything */
        gamegraphics(player1,player2,gkeeperplayer1,gkeeperplayer2,ball,exitWindowRequested);
    }

    


    /* End Game*/
    CloseWindow();

    return 0;
}



/* ----------------------- FUNCTIONS ----------------------- */



void pawnMovement(struct pawn *p1,struct pawn *p2,struct pawn *gp1,struct pawn *gp2)
{

    float time = GetFrameTime();
    /* Player 2 & Goalkeeper 2*/
    if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_UP))
        gp2->y -= gp2->speed * time;
    else if(IsKeyDown(KEY_UP))
        p2->y -= p2->speed * time;


    if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_DOWN))
        gp2->y += gp2->speed * time;
    else if(IsKeyDown(KEY_DOWN))
        p2->y += p2->speed * time;


    if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_LEFT))
        gp2->x -= gp2->speed * time;
    else if(IsKeyDown(KEY_LEFT))
        p2->x -= p2->speed * time;


    if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_RIGHT))
        gp2->x += gp2->speed * time;
    else if(IsKeyDown(KEY_RIGHT))
        p2->x += p2->speed * time;



    /* Player 1 & Goalkeeper 1 */
    if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_W))
        gp1->y -= gp1->speed * time;
    else if(IsKeyDown(KEY_W))
        p1->y -= p1->speed * time;


    if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_S))
        gp1->y += gp1->speed * time;
    else if(IsKeyDown(KEY_S))
        p1->y += p1->speed * time;


    if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_A))
        gp1->x -= gp1->speed * time;
    else if(IsKeyDown(KEY_A))
        p1->x -= p1->speed * time;


    if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_D))
        gp1->x += gp1->speed * GetFrameTime();
    else if(IsKeyDown(KEY_D))
        p1->x += p1->speed * GetFrameTime();
}


/* ------------------------------------------------------------------------------- */



void pawnCollision(struct pawn *p1,struct pawn *p2,struct pawn *gp1,struct pawn *gp2)
{
    /* Checking if pawns interact //  Not allowing enemy pawns to go through each other */
    /* Ally pawns can pass through each other */

    /* Creating raylib library shapes to pass into the collision functions*/
    Vector2 pcenter1 = {p1->x, p1->y};
    Vector2 pcenter2 = {p2->x, p2->y};
    Vector2 gpcenter1 = {gp1->x,gp1->y};
    Vector2 gpcenter2 = {gp2->x,gp2->y};

    float time = GetFrameTime();


    /* Checking and denying the collision of player 1 and player 2*/

    if(CheckCollisionCircles(pcenter1,p1->radius,pcenter2,p2->radius))
    {
        /* Case 1: Horizontal impact */
        if(p1->y == p2->y)
        {
            /* player 1 is more to the left than player 2*/
            if(p1->x < p2->x)
            {
                if(IsKeyDown(KEY_LEFT))
                    p2->x += p2->speed * time;
                if(IsKeyDown(KEY_D))
                    p1->x -= p1->speed * time;
            }
            else
            {   /* player 1 is more to the right than player 2*/
                if(IsKeyDown(KEY_RIGHT))
                    p2->x -= p2->speed * time;
                if(IsKeyDown(KEY_A))
                    p1->x += p1->speed * time;
            }
                
        }/* Case 2: Vertical impact */
        else if(p1->x == p2->x)
        {
            /* player 1 is higher than player 2 */
            if(p1->y < p2->y)
            {
                if(IsKeyDown(KEY_UP))
                    p2->y += p2->speed * time;

                 if(IsKeyDown(KEY_S))
                    p1->y -= p1->speed * time;
            }
            else
            {   /* player 1 is lower than player 2 */
                if(IsKeyDown(KEY_DOWN))
                    p2->y -= p2->speed * time;

                if(IsKeyDown(KEY_W))
                    p1->y += p1->speed * time;
            }
                
                
        } /* Case 3(the hardest): Side impact */
        else
        {
            /* Subcase 1:if player 1 is higher and more to the left than player 2*/
            if((p1->y < p2->y) && (p1->x < p2->x))
            {
                /* Not letting player 2 go left and up */
                if(IsKeyDown(KEY_UP))
                    p2->y += p2->speed * time;

                if(IsKeyDown(KEY_LEFT))
                    p2->x += p2->speed * time;

                /* Not letting player 1 go down and right */
                if(IsKeyDown(KEY_S))
                    p1->y -= p1->speed * time;

                if(IsKeyDown(KEY_D))
                    p1->x -= p1->speed * time;

            }  /* Subcase 2: if player 1 is higher and more to the right than player 2 */
            else if((p1->y < p2->y) && (p1->x > p2->x))
            {
                /* Not letting player 2 go up and right */
                if(IsKeyDown(KEY_UP))
                    p2->y += p2->speed * time;

                if(IsKeyDown(KEY_RIGHT))
                    p2->x -= p2->speed * time;
                /* Not letting player 1 go down and left */
                if(IsKeyDown(KEY_S))
                    p1->y -= p1->speed * time;
                if(IsKeyDown(KEY_A))
                    p1->x += p1->speed * time;

            }   /* Subcase 3: if player 2 is higher and more to the left than player 1 */
            else if((p1->y > p2->y) && (p1->x > p2->x))
            {
                /* Not letting player 2 go down and right */
                if(IsKeyDown(KEY_DOWN))
                    p2->y -= p2->speed * time;

                if(IsKeyDown(KEY_RIGHT))
                    p2->x -= p2->speed * time;

                /* Not letting player 1 go up and left */
                if(IsKeyDown(KEY_W))
                    p1->y += p1->speed * time;

                if(IsKeyDown(KEY_A))
                    p1->x += p1->speed * time;

            }/* Subcase 4: if player 2 is higher and more to the right than player 1*/
            else if((p1->y > p2->y) && (p1->x < p2->x))
            {
                /* Not letting player 2 go down and left */
                if(IsKeyDown(KEY_DOWN))
                    p2->y -= p2->speed * time;

                if(IsKeyDown(KEY_LEFT))
                    p2->x += p2->speed * time;

                /* Not letting player 1 go up and right */
                if(IsKeyDown(KEY_W))
                    p1->y += p1->speed * time;

                if(IsKeyDown(KEY_D))
                    p1->x -= p1->speed * time;
            }
        }
    }

/*---------------------------------------------------------------------------------*/

    /* Checking and denying the collision of player 2 and gkeeperplayer 1*/

    if(CheckCollisionCircles(gpcenter1,gp1->radius,pcenter2,p2->radius))
    {
        /* Case 1: Horizontal impact */
        if(gp1->y == p2->y)
        {
            /* gkeeperplayer 1 is more to the left than player 2*/
            if(gp1->x < p2->x)
            {
                if(IsKeyDown(KEY_LEFT))
                    p2->x += p2->speed * time;
                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_D))
                    gp1->x -= gp1->speed * time;
            }
            else
            {   /* gkeeperplayer 1 is more to the right than player 2*/
                if(IsKeyDown(KEY_RIGHT))
                    p2->x -= p2->speed * time;
                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_A))
                    gp1->x += gp1->speed * time;
            }
                
        }/* Case 2: Vertical impact */
        else if(gp1->x == p2->x)
        {
            /* gkeeperplayer 1 is higher than player 2 */
            if(gp1->y < p2->y)
            {
                if(IsKeyDown(KEY_UP))
                    p2->y += p2->speed * time;

                 if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_S))
                    gp1->y -= gp1->speed * time;
            }
            else
            {   /* gkeeperplayer 1 is lower than player 2 */
                if(IsKeyDown(KEY_DOWN))
                    p2->y -= p2->speed * time;

                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_W))
                    gp1->y += gp1->speed * time;
            }
                
                
        } /* Case 3(the hardest): Side impact */
        else
        {
            /* Subcase 1:if gkeeperplayer 1 is higher and more to the left than player 2*/
            if((gp1->y < p2->y) && (gp1->x < p2->x))
            {
                /* Not letting player 2 go left and up */
                if(IsKeyDown(KEY_UP))
                    p2->y += p2->speed * time;

                if(IsKeyDown(KEY_LEFT))
                    p2->x += p2->speed * time;

                /* Not letting gkeeperplayer 1 go down and right */
                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_S))
                    gp1->y -= gp1->speed * time;

                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_D))
                    gp1->x -= gp1->speed * time;

            }  /* Subcase 2: if gkeeperplayer 1 is higher and more to the right than player 2 */
            else if((gp1->y < p2->y) && (gp1->x > p2->x))
            {
                /* Not letting player 2 go up and right */
                if(IsKeyDown(KEY_UP))
                    p2->y += p2->speed * time;

                if(IsKeyDown(KEY_RIGHT))
                    p2->x -= p2->speed * time;
                /* Not letting gkeeperplayer 1 go down and left */
                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_S))
                    gp1->y -= gp1->speed * time;
                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_A))
                    gp1->x += gp1->speed * time;

            }   /* Subcase 3: if player 2 is higher and more to the left than gkeeperplayer 1 */
            else if((gp1->y > p2->y) && (gp1->x > p2->x))
            {
                /* Not letting player 2 go down and right */
                if(IsKeyDown(KEY_DOWN))
                    p2->y -= p2->speed * time;

                if(IsKeyDown(KEY_RIGHT))
                    p2->x -= p2->speed * time;

                /* Not letting gkeeperplayer 1 go up and left */
                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_W))
                    gp1->y += gp1->speed * time;

                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_A))
                    gp1->x += gp1->speed * time;

            }/* Subcase 4: if player 2 is higher and more to the right than gkeeperplayer 1*/
            else if((gp1->y > p2->y) && (gp1->x < p2->x))
            {
                /* Not letting player 2 go down and left */
                if(IsKeyDown(KEY_DOWN))
                    p2->y -= p2->speed * time;

                if(IsKeyDown(KEY_LEFT))
                    p2->x += p2->speed * time;

                /* Not letting gkeeperplayer 1 go up and right */
                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_W))
                    gp1->y += gp1->speed * time;

                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_D))
                    gp1->x -= gp1->speed * time;
            }
        }
    }

/*---------------------------------------------------------------------------------*/

    /* Checking and denying the collision of player 1 and gkeeperplayer 2*/

    if(CheckCollisionCircles(pcenter1,p1->radius,gpcenter2,gp2->radius))
    {
        /* Case 1: Horizontal impact */
        if(p1->y == gp2->y)
        {
            /* player 1 is more to the left than gkeeperplayer 2*/
            if(p1->x < gp2->x)
            {
                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_LEFT))
                    gp2->x += gp2->speed * time;
                if(IsKeyDown(KEY_D))
                    p1->x -= p1->speed * time;
            }
            else
            {   /* player 1 is more to the right than gkeeperplayer 2*/
                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_RIGHT))
                    gp2->x -= gp2->speed * time;
                if(IsKeyDown(KEY_A))
                    p1->x += p1->speed * time;
            }
                
        }/* Case 2: Vertical impact */
        else if(p1->x == gp2->x)
        {
            /* player 1 is higher than gkeeperplayer 2 */
            if(p1->y < gp2->y)
            {
                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_UP))
                    gp2->y += gp2->speed * time;

                 if(IsKeyDown(KEY_S))
                    p1->y -= p1->speed * time;
            }
            else
            {   /* player 1 is lower than gkeeperplayer 2 */
                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_DOWN))
                    gp2->y -= gp2->speed * time;

                if(IsKeyDown(KEY_W))
                    p1->y += p1->speed * time;
            }
                
                
        } /* Case 3(the hardest): Side impact */
        else
        {
            /* Subcase 1:if player 1 is higher and more to the left than gkeeperplayer 2*/
            if((p1->y < gp2->y) && (p1->x < gp2->x))
            {
                /* Not letting gkeeperplayer 2 go left and up */
                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_UP))
                    gp2->y += gp2->speed * time;

                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_LEFT))
                    gp2->x += gp2->speed * time;

                /* Not letting player 1 go down and right */
                if(IsKeyDown(KEY_S))
                    p1->y -= p1->speed * time;

                if(IsKeyDown(KEY_D))
                    p1->x -= p1->speed * time;

            }  /* Subcase 2: if player 1 is higher and more to the right than gkeeperplayer 2 */
            else if((p1->y < gp2->y) && (p1->x > gp2->x))
            {
                /* Not letting gkeeperplayer 2 go up and right */
                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_UP))
                    gp2->y += gp2->speed * time;

                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_RIGHT))
                    gp2->x -= gp2->speed * time;
                /* Not letting player 1 go down and left */
                if(IsKeyDown(KEY_S))
                    p1->y -= p1->speed * time;
                if(IsKeyDown(KEY_A))
                    p1->x += p1->speed * time;

            }   /* Subcase 3: if gkeeperplayer 2 is higher and more to the left than player 1 */
            else if((p1->y > gp2->y) && (p1->x > gp2->x))
            {
                /* Not letting gkeeperplayer 2 go down and right */
                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_DOWN))
                    gp2->y -= gp2->speed * time;

                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_RIGHT))
                    gp2->x -= gp2->speed * time;

                /* Not letting player 1 go up and left */
                if(IsKeyDown(KEY_W))
                    p1->y += p1->speed * time;

                if(IsKeyDown(KEY_A))
                    p1->x += p1->speed * time;

            }/* Subcase 4: if gkeeperplayer 2 is higher and more to the right than player 1*/
            else if((p1->y > gp2->y) && (p1->x < gp2->x))
            {
                /* Not letting gkeeperplayer 2 go down and left */
                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_DOWN))
                    gp2->y -= gp2->speed * time;

                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_LEFT))
                    gp2->x += gp2->speed * time;

                /* Not letting player 1 go up and right */
                if(IsKeyDown(KEY_W))
                    p1->y += p1->speed * time;

                if(IsKeyDown(KEY_D))
                    p1->x -= p1->speed * time;
            }
        }
    }
}


void ballCollision(struct pawn *p1,struct pawn *p2,struct pawn *gp1,struct pawn *gp2,struct ball *b)
{
    /* Checking if the ball interacts with the pawns */
    /* Checking for all the possible collision scenarios, just like with denyCollision*/
    // Max distance == 25 (radiusp1 - radiusball)
    /* Creating raylib library shapes to pass into the collision functions*/
    Vector2 pcenter1 = {p1->x, p1->y};
    Vector2 pcenter2 = {p2->x, p2->y};
    Vector2 gpcenter1 = {gp1->x,gp1->y};
    Vector2 gpcenter2 = {gp2->x,gp2->y};
    Vector2 bcenter = {b->x,b->y};


    
    float time = GetFrameTime();

    /* Collision with player 1 */
    if(CheckCollisionCircles(pcenter1,p1->radius,bcenter,b->radius))
    {
        /* Case 1: horizontal impact with player 1 and the ball */
        if(p1->y == b->y)
        {   /* Subcase 1: player 1 is more to the left than the ball*/
            if((p1->x < b->x))
            {
                /* The ball goes right */
                b->speedX += 200 * (b->x - p1->x) * time;

                /* Player 1 cant move right  */
                if(IsKeyDown(KEY_D))
                    p1->x -= p1->speed * time;
            }
            else /* Subcase 2: player 1 is more to the right than the ball */
            {
                /* The ball goes left */
                b->speedX -= 200 * (p1->x - b->x) * time;

                /* Player 1 cant go left*/
                if(IsKeyDown(KEY_A))
                    p1->x += p1->speed * time;
            }
        }   /* Case 2: vertical impact with player 1 and the ball */
        else if(p1->x == b->x)
        {
            /* Subcase 1: player 1 is higher than the ball*/
            if(p1->y < b->y)
            {
                /* The ball goes down */
                b->speedY += 200 * (b->y - p1->y) * time;
                /* Player 1 cant go down*/
                if(IsKeyDown(KEY_S))
                    p1->y -= p1->speed * time;
            }
            else /* Subcase 2: player 1 is lower than the ball*/
            {
                /* The ball goes up */
                b->speedY -= 200 * (p1->y - b->y) * time;
                /* Player 1 cant go up */
                if(IsKeyDown(KEY_W))
                    p1->y += p1->speed * time;
            }
        }
        else    /* Case 3(again, the hardest): Side impact with player 1 and the ball */
        {
            /* Subcase 1: player 1 is lower and more to the left than the ball*/
            if((p1->y > b->y) && (p1->x < b->x))
            {
                 /* The ball goes up and right */
                b->speedX += 200 * (b->x - p1->x) * time;
                b->speedY -= 200 * (p1->y - b->y) * time;

                 /* Player 1 cant go up and right */
                if(IsKeyDown(KEY_W))
                    p1->y += p1->speed * time;

                if(IsKeyDown(KEY_D))
                    p1->x -= p1->speed * time;

            }   /* Subcase 2: player 1 is lower and more to the right than the ball*/
            else if((p1->y > b->y) && (p1->x > b->x))
            {
                /* The ball goes up and left*/
                b->speedX -= 200 * (p1->x - b->x) * time;
                b->speedY -= 200 * (p1->y - b->y) * time;

                /* Player 1 cant go up and left */
                if(IsKeyDown(KEY_W))
                    p1->y += p1->speed * time;

                if(IsKeyDown(KEY_A))
                    p1->x += p1->speed * time;

            }   /* Subcase 3: player 1 is higher and more to the left than the ball */
            else if((p1->x < b->x) && (p1->y < b->y))
            {
                /* The ball goes down and right */
                b->speedX += 200 * (b->x - p1->x) * time;
                b->speedY += 200 * (b->y - p1->y) * time;

                /* Player 1 cant go down and right */
                if(IsKeyDown(KEY_S))
                    p1->y -= p1->speed * time;

                if(IsKeyDown(KEY_D))
                    p1->x -= p1->speed * time;

            }   /* Subcase 4: player 1 is higher and more to the right than the ball*/
            else if((p1->x > b->x) && (p1->y < b->y))
            {
                /* The ball goes down and left */
                b->speedX -= 200 * (p1->x - b->x) * time;
                b->speedY += 200 * (b->y - p1->y) * time;

                /* Player 1 cant go down and left */
                if(IsKeyDown(KEY_S))
                    p1->y -= p1->speed * time;

                if(IsKeyDown(KEY_A))
                    p1->x += p1->speed * time;
            }
        }
    }


    /* --------------------------------------------------------------------- */


    /* Collision with player 2 */
    if(CheckCollisionCircles(pcenter2,p2->radius,bcenter,b->radius))
    {
        /* Case 1: horizontal impact with player 2 and the ball */
        if(p2->y == b->y)
        {   /* Subcase 1: player 2 is more to the left than the ball*/
            if((p2->x < b->x))
            {
                /* The ball goes right */
                b->speedX += 200 * (b->x - p2->x) * time;

                /* Player 2 cant move right  */
                if(IsKeyDown(KEY_RIGHT))
                    p2->x -= p2->speed * time;
            }
            else /* Subcase 2: player 2 is more to the right than the ball */
            {
                /* The ball goes left */
                b->speedX -= 200 * (p2->x - b->x) * time;

                /* Player 2 cant go left*/
                if(IsKeyDown(KEY_LEFT))
                    p2->x += p2->speed * time;
            }
        }   /* Case 2: vertical impact with player 2 and the ball */
        else if(p2->x == b->x)
        {
            /* Subcase 1: player 2 is higher than the ball*/
            if(p2->y < b->y)
            {
                /* The ball goes down */
                b->speedY += 200 * (b->y - p2->y) * time;
                /* Player 2 cant go down*/
                if(IsKeyDown(KEY_DOWN))
                    p2->y -= p2->speed * time;
            }
            else /* Subcase 2: player 2 is lower than the ball*/
            {
                /* The ball goes up */
                b->speedY -= 200 * (p2->y - b->y) * time;
                /* Player 2 cant go up */
                if(IsKeyDown(KEY_UP))
                    p2->y += p2->speed * time;
            }
        }
        else    /* Case 3(again, the hardest): Side impact with player 2 and the ball */
        {
            /* Subcase 1: player 2 is lower and more to the left than the ball*/
            if((p2->y > b->y) && (p2->x < b->x))
            {
                 /* The ball goes up and right */
                b->speedX += 200 * (b->x - p2->x) * time;
                b->speedY -= 200 * (p2->y - b->y) * time;

                 /* Player 2 cant go up and right */
                if(IsKeyDown(KEY_UP))
                    p2->y += p2->speed * time;

                if(IsKeyDown(KEY_RIGHT))
                    p2->x -= p2->speed * time;

            }   /* Subcase 2: player 2 is lower and more to the right than the ball*/
            else if((p2->y > b->y) && (p2->x > b->x))
            {
                /* The ball goes up and left*/
                b->speedX -= 200 * (p2->x - b->x) * time;
                b->speedY -= 200 * (p2->y - b->y) * time;

                /* Player 2 cant go up and left */
                if(IsKeyDown(KEY_UP))
                    p2->y += p2->speed * time;

                if(IsKeyDown(KEY_LEFT))
                    p2->x += p2->speed * time;

            }   /* Subcase 3: player 2 is higher and more to the left than the ball */
            else if((p2->x < b->x) && (p2->y < b->y))
            {
                /* The ball goes down and right */
                b->speedX += 200 * (b->x - p2->x) * time;
                b->speedY += 200 * (b->y - p2->y) * time;

                /* Player 2 cant go down and right */
                if(IsKeyDown(KEY_DOWN))
                    p2->y -= p2->speed * time;

                if(IsKeyDown(KEY_RIGHT))
                    p2->x -= p2->speed * time;

            }   /* Subcase 4: player 2 is higher and more to the right than the ball*/
            else if((p2->x > b->x) && (p2->y < b->y))
            {
                /* The ball goes down and left */
                b->speedX -= 200 * (p2->x - b->x) * time;
                b->speedY += 200 * (b->y - p2->y) * time;

                /* Player 2 cant go down and left */
                if(IsKeyDown(KEY_DOWN))
                    p2->y -= p2->speed * time;

                if(IsKeyDown(KEY_LEFT))
                    p2->x += p2->speed * time;
            }
        }
    }


    /* --------------------------------------------- */


    /* Collision with gkeeperplayer 1 */
    if(CheckCollisionCircles(gpcenter1,gp1->radius,bcenter,b->radius))
    {
        /* Case 1: horizontal impact with gkeeperplayer 1 and the ball */
        if(gp1->y == b->y)
        {   /* Subcase 1: gkeeperplayer 1 is more to the left than the ball*/
            if((gp1->x < b->x))
            {
                /* The ball goes right */
                b->speedX += 200 * (b->x - gp1->x) * time;

                /* Gkeeperplayer 1 cant move right  */
                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_D))
                    gp1->x -= gp1->speed * time;
            }
            else /* Subcase 2: gkeeperplayer 1 is more to the right than the ball */
            {
                /* The ball goes left */
                b->speedX -= 200 * (gp1->x - b->x) * time;

                /* Gkeeperplayer 1 cant go left*/
                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_A))
                    gp1->x += gp1->speed * time;
            }
        }   /* Case 2: vertical impact with gkeeperplayer 1 and the ball */
        else if(gp1->x == b->x)
        {
            /* Subcase 1: player 1 is higher than the ball*/
            if(gp1->y < b->y)
            {
                /* The ball goes down */
                b->speedY += 200 * (b->y - gp1->y) * time;
                /* Gkeeperplayer 1 cant go down*/
                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_S))
                    gp1->y -= gp1->speed * time;
            }
            else /* Subcase 2: gkeeperplayer 1 is lower than the ball*/
            {
                /* The ball goes up */
                b->speedY -= 200 * (gp1->y - b->y) * time;
                /* Gkeeperplayer 1 cant go up */
                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_W))
                    gp1->y += gp1->speed * time;
            }
        }
        else    /* Case 3(again, the hardest): Side impact with gkeeperplayer 1 and the ball */
        {
            /* Subcase 1: gkeeperplayer 1 is lower and more to the left than the ball*/
            if((gp1->y > b->y) && (gp1->x < b->x))
            {
                 /* The ball goes up and right */
                b->speedX += 200 * (b->x - gp1->x) * time;
                b->speedY -= 200 * (gp1->y - b->y) * time;

                 /* Gkeeperplayer 1 cant go up and right */
                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_W))
                    gp1->y += gp1->speed * time;

                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_D))
                    gp1->x -= gp1->speed * time;

            }   /* Subcase 2: gkeeperplayer 1 is lower and more to the right than the ball*/
            else if((gp1->y > b->y) && (gp1->x > b->x))
            {
                /* The ball goes up and left*/
                b->speedX -= 200 * (gp1->x - b->x) * time;
                b->speedY -= 200 * (gp1->y - b->y) * time;

                /* Gkeeperplayer 1 cant go up and left */
                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_W))
                    gp1->y += gp1->speed * time;

                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_A))
                    gp1->x += gp1->speed * time;

            }   /* Subcase 3: gkeeperplayer 1 is higher and more to the left than the ball */
            else if((gp1->x < b->x) && (gp1->y < b->y))
            {
                /* The ball goes down and right */
                b->speedX += 200 * (b->x - gp1->x) * time;
                b->speedY += 200 * (b->y - gp1->y) * time;

                /* Gkeeperplayer 1 cant go down and right */
                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_S))
                    gp1->y -= gp1->speed * time;

                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_D))
                    gp1->x -= gp1->speed * time;

            }   /* Subcase 4: gkeeperplayer 1 is higher and more to the right than the ball*/
            else if((gp1->x > b->x) && (gp1->y < b->y))
            {
                /* The ball goes down and left */
                b->speedX -= 200 * (gp1->x - b->x) * time;
                b->speedY += 200 * (b->y - gp1->y) * time;

                /* Gkeeperplayer 1 cant go down and left */
                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_S))
                    gp1->y -= gp1->speed * time;

                if(IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_A))
                    gp1->x += gp1->speed * time;
            }
        }
    }




    /* ------------------------------------------------------------------------ */

    /* Collision with gkeeperplayer 2 */
    if(CheckCollisionCircles(gpcenter2,gp2->radius,bcenter,b->radius))
    {
        /* Case 1: horizontal impact with gkeeperplayer 2 and the ball */
        if(gp2->y == b->y)
        {   /* Subcase 1: gkeeperplayer 2 is more to the left than the ball*/
            if((gp2->x < b->x))
            {
                /* The ball goes right */
                b->speedX += 200 * (b->x - gp2->x) * time;

                /* Gkeeperplayer 2 cant move right  */
                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_RIGHT))
                    gp2->x -= gp2->speed * time;
            }
            else /* Subcase 2: gkeeperplayer 2 is more to the right than the ball */
            {
                /* The ball goes left */
                b->speedX -= 200 * (gp2->x - b->x) * time;

                /* Gkeeperplayer 2 cant go left*/
                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_LEFT))
                    gp2->x += gp2->speed * time;
            }
        }   /* Case 2: vertical impact with gkeeperplayer 2 and the ball */
        else if(gp2->x == b->x)
        {
            /* Subcase 1: gkeeperplayer 2 is higher than the ball*/
            if(gp2->y < b->y)
            {
                /* The ball goes down */
                b->speedY += 200 * (b->y - gp2->y) * time;
                /* Gkeeperplayer 2 cant go down*/
                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_DOWN))
                    gp2->y -= gp2->speed * time;
            }
            else /* Subcase 2: gkeeperplayer 2 is lower than the ball*/
            {
                /* The ball goes up */
                b->speedY -= 200 * (gp2->y - b->y) * time;
                /* Gkeeperplayer 2 cant go up */
                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_UP))
                    gp2->y += gp2->speed * time;
            }
        }
        else    /* Case 3(again, the hardest): Side impact with gkeeperplayer 2 and the ball */
        {
            /* Subcase 1: gkeeperplayer 2 is lower and more to the left than the ball*/
            if((gp2->y > b->y) && (gp2->x < b->x))
            {
                 /* The ball goes up and right */
                b->speedX += 200 * (b->x - gp2->x) * time;
                b->speedY -= 200 * (gp2->y - b->y) * time;

                 /* Gkeeperplayer 2 cant go up and right */
                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_UP))
                    gp2->y += gp2->speed * time;

                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_RIGHT))
                    gp2->x -= gp2->speed * time;

            }   /* Subcase 2: gkeeperplayer 2 is lower and more to the right than the ball*/
            else if((gp2->y > b->y) && (gp2->x > b->x))
            {
                /* The ball goes up and left*/
                b->speedX -= 200 * (gp2->x - b->x) * time;
                b->speedY -= 200 * (gp2->y - b->y) * time;

                /* Gkeeperplayer 2 cant go up and left */
                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_UP))
                    gp2->y += gp2->speed * time;

                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_LEFT))
                    gp2->x += gp2->speed * time;

            }   /* Subcase 3: gkeeperplayer 2 is higher and more to the left than the ball */
            else if((gp2->x < b->x) && (gp2->y < b->y))
            {
                /* The ball goes down and right */
                b->speedX += 200 * (b->x - gp2->x) * time;
                b->speedY += 200 * (b->y - gp2->y) * time;

                /* Gkeeperplayer 2 cant go down and right */
                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_DOWN))
                    gp2->y -= gp2->speed * time;

                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_RIGHT))
                    gp2->x -= gp2->speed * time;

            }   /* Subcase 4: gkeeperplayer 2 is higher and more to the right than the ball*/
            else if((gp2->x > b->x) && (gp2->y < b->y))
            {
                /* The ball goes down and left */
                b->speedX -= 200 * (gp2->x - b->x) * time;
                b->speedY += 200 * (b->y - gp2->y) * time;

                /* Gkeeperplayer 2 cant go down and left */
                if(IsKeyDown(KEY_RIGHT_CONTROL) && IsKeyDown(KEY_DOWN))
                    gp2->y -= gp2->speed * time;

                if(IsKeyDown(KEY_LEFT))
                    gp2->x += gp2->speed * time;
            }
        }
    }
}



void boundaries(struct pawn *p1,struct pawn *p2,struct pawn *gp1,struct pawn *gp2,struct ball *b)
{
    const int screenHeight = 1600;
    

    /* Pawns Boundaries */

    /* Boundaries for player 1  == can't leave the field lines*/
    /* Horizontally */
    if((p1->x + p1->radius) > 2600) p1->x = 2600 - p1->radius;
    if((p1->x - p1->radius) < 200) p1->x = 200 + p1->radius;
    /* Vertically */
    if((p1->y - p1->radius) < 50) p1->y = 50 + p1->radius;
    if((p1->y + p1->radius) > 1550) p1->y = 1550 - p1->radius;

    /* Boundaries for player 2 == can't leave the field lines*/
    /* Horizontally */
    if((p2->x + p2->radius) > 2600) p2->x = 2600 - p2->radius;
    if((p2->x - p2->radius) < 200) p2->x = 200 + p2->radius;
    /* Vertically */
    if((p2->y - p2->radius) < 50) p2->y = 50 + p2->radius;
    if((p2->y + p2->radius) > 1550) p2->y = 1550 - p2->radius;


    /* Boundaries for Goalkeeper 1 == can't leave the big goalpost area 1 */
    /* Horizontally */
    if((gp1->x + gp1->radius) > 630) gp1->x = 630 - gp1->radius;
    if((gp1->x - gp1->radius) < 200) gp1->x = 200 + gp1->radius;
    /* Vertically */
    if((gp1->y - gp1->radius) < 300 + 20) gp1->y = 320 + gp1->radius;
    if((gp1->y + gp1->radius) > 300 + 1000 - 20) gp1->y = 1280 - gp1->radius;

    /* Boundaries for goalkeeper 2 == can't leave the big goalpost area 2*/
    /* Horizontally */
    if((gp2->x + gp2->radius) > 2600) gp2->x = 2600 - gp2->radius;
    if((gp2->x - gp2->radius) < 2170) gp2->x = 2170 + gp2->radius;
    /* Vertically */
    if((gp2->y - gp2->radius) < 320) gp2->y = 320 + gp2->radius;
    if((gp2->y + gp2->radius) > 1280) gp2->y = 1280 - gp2->radius;


    /* Ball's Boundaries */

    /* Boundaries when the ball is inside the goalpost for the Y axis */
    /* When balls edge's have passed the goalposts inner line */
    if(((b->x - b->radius) < 200 - 15) || ((b->x + b->radius) > 2600 + 15))
    {
        if((b->y - b->radius) < (screenHeight / 2) - 250 + 15)
        {
            b->y = (screenHeight / 2) - 250 + 15 + b->radius;
            b->speedY *= -0.25; // 0.25 to slow down
        }
        if((b->y + b->radius) > (screenHeight / 2) + 250 - 15) 
        {
            b->y = (screenHeight / 2) + 250 - 15 - b->radius;
            b->speedY *= -0.25; // 0.25 to slow down
        }  
    }
    else
    {
        if((b->y - b->radius) < 50 + 30)
        {
            b->y = 50 + 30 + b->radius;
            b->speedY *= -1;
        }
        if((b->y + b->radius) > 1600 - 50 - 30)
        {
            b->y = 1600 - 50 - 30 - b->radius;
            b->speedY *= -1;
        }
    }

    /* Boundaries when the ball is inside the goalpost for the X axis*/
    if(((b->y - b->radius) < (screenHeight / 2) - 250 + 15) || (b->y + b->radius) > (screenHeight / 2) + 250 - 15)
    {
        if(b->x - b->radius < 200 + 30)
        {
            b->x = 200 + 30 + b->radius;
            b->speedX *= -1;
        }
        if((b->x + b->radius) > 2600 - 30)
        {
            b->x = 2600 - 30 - b->radius;
            b->speedX *= -1;
        }
    }
    else
    {
        if((b->x - b->radius) < 0 + 15)
        {
            b->x = b->radius + 15;
            b->speedX *= -0.25; // 0.25 to slow down 
        }
        if((b->x + b->radius) > 2800 - 15)
        {
            b->x = 2800 - 15 - b->radius;
            b->speedX *= -0.25; // 0.25 to slow down
        }
    }
}

void ballMovement(struct ball *b)
{
    float time = GetFrameTime();
    /* Ball Movement and Velocity */
    /* The ball must decelerate, to act like a football */
    if(b->speedX > 0)
        b->speedX -= b->accel * time;
            
    else if(b->speedX < 0)
        b->speedX += b->accel * time;

    if(b->speedY > 0)
        b->speedY -= b->accel * time;
    else if(b->speedY < 0)
        b->speedY += b->accel * time;
        
    /* Because time = GetTimeFrame() is a floating point number and speedX and speedY have integer values, speedX and speedY NEVER become 0 */
    /* To make the ball stop, we treat(approximate) a very small speed as 0 */
    if((((b->speedX > 0) && (b->speedX < 5)) || ((b->speedX > -5) && (b->speedX < 0))) && b->speedX != 0)
        b->speedX = 0;

    if((((b->speedY > 0) && (b->speedY < 5)) || ((b->speedY > -5) && (b->speedY < 0))) && b->speedY != 0)
        b->speedY = 0;

    /* Decelerating X*/
    if(b->speedX != 0)
    {
        b->x += (b->speedX * time) - ((b->accel * pow(time,2)) / 2);
    }
    /* Decelerating Y*/
    if(b->speedY != 0)
    {
        b->y += (b->speedY * time) - ((b->accel * pow(time,2)) / 2);
    }
}


void isgoalscored(struct pawn *p1,struct pawn *p2,struct pawn *gp1,struct pawn *gp2,struct ball *b)
{
    int screenWidth = 2800;
    int screenHeight = 1600;
    int random;

    /* Goal's - Enter to start again */

    /* If a goal has been scored */
    if(((b->x + b->radius) < 200) || ((b->x - b->radius) > 2600))
    {
        

        if(IsKeyPressed(KEY_ENTER))
        {
            /* Put all the players in their initial position */
            p1->x = (screenWidth / 2) - 475;
            p1->y = screenHeight / 2;

            p2->x = (screenWidth / 2) + 475;
            p2->y = screenHeight / 2;

            gp1->x =  (screenWidth / 2) - 1100;
            gp1->y = screenHeight / 2;

            gp2->x =  (screenWidth / 2) + 1100;
            gp2->y = screenHeight / 2;

            /* Put the ball in its initial position and stop's it from moving */
            b->x = screenWidth / 2;
            b->y = screenHeight / 2;
            b->speedX = 0;
            b->speedY = 0;

            /* Random Starting ball speed */
            srand(time(NULL));
            random = rand() % 2;

            /* random = 0: blue has ball*/
            if(random == 0)
                b->speedX = -300;
            else /* random = 1: red has ball*/
                b->speedX = 300;
        }
    }
}



void gamegraphics(struct pawn player1,struct pawn player2, struct pawn gkeeperplayer1,struct pawn gkeeperplayer2,struct ball ball,bool exitWindowRequested)
{

    int screenWidth = 2800;
    int screenHeight = 1600;
/* Drawing the object's */
    BeginDrawing();
        ClearBackground(WHITE);

        if (exitWindowRequested)
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
            DrawText("Are you sure you want to exit program? [Y/N]", (screenWidth / 2) - 550, (screenHeight / 2) - 80, 40, WHITE);
        }
        else
        {
            /* START OF NOT NEEDED OBJECTS */
            /* Drawing the grass on the field */
            for(int i=0; i<(screenWidth / 200); i++)
            {
                if(i % 2 == 0) DrawRectangle((200 * i),0,200,1600,DARKGREEN);
                else DrawRectangle((200 * i),0,200,1600,LIME);
            }

            /* White field lines */
            Rectangle field = {200,50,2400,1500};
            DrawRectangleLinesEx(field, 30, WHITE);

            /* Middle circle */
            for(float i=1; i<20; i+=0.1) 
                DrawCircleLines((screenWidth / 2),(screenHeight / 2),(190 + i),WHITE);

            /* Middle Vertical Line*/
            DrawRectangle((screenWidth / 2 - 6),80,12,1500-60,WHITE);

            /* Goalkeepers Area 1 */
            Rectangle keepersArea1 = {200, 300, 450, 1000};
            DrawRectangleLinesEx(keepersArea1,20,WHITE);

            /* Goalkeepers Area 2 */
            Rectangle keepersArea2 = {2150, 300, 450, 1000};
            DrawRectangleLinesEx(keepersArea2,20,WHITE);

            /* Penalty and start circles */
            DrawCircle(screenWidth / 2,(screenHeight / 2), 25, WHITE);
            DrawCircle(screenWidth / 2 - 875,(screenHeight) / 2, 25, WHITE);
            DrawCircle(screenWidth / 2 + 875,(screenHeight) / 2, 25, WHITE);

            /* Critical Goal Area 1 */
            Rectangle keepers1 = {200,(screenHeight / 2) - 400, 200, 800};
            DrawRectangleLinesEx(keepers1,15,WHITE);
            /* Critical Goal Area 2 */
            Rectangle keepers2 = {2800-200-200,(screenHeight / 2) - 400, 200, 800};
            DrawRectangleLinesEx(keepers2,15,WHITE);

            /* Corner's */
            Vector2 corner1 = {200 + 30,50 + 30};
            for(float i=0; i<10; i+=0.1) DrawCircleSectorLines(corner1, 50 + i, 0.0f, 90.0f,0,WHITE);
            Vector2 corner2 = {2600 - 30, 50 + 30};
            for(float i=0; i<10; i+=0.1) DrawCircleSectorLines(corner2, 50 + i, 360.0f, 270.0f,0,WHITE);
            Vector2 corner3 = {200 + 30,1600 - 50 - 30};
            for(float i=0; i<10; i+=0.1) DrawCircleSectorLines(corner3, 50 + i, 180.0f, 90.0f,0,WHITE);
            Vector2 corner4 = {2600 - 30, 1600 - 50 - 30};
            for(float i=0; i<10; i+=0.1) DrawCircleSectorLines(corner4, 50 + i, 270.0f, 180.0f,0,WHITE);

            /* END OF NOT NEEDED OBJECTS */
            /*-------------------------------------------------------------------------------*/

            /* START OF NEEDED OBJECTS */

            /* Drawing the Ball */
            DrawCircleGradient(ball.x,ball.y,ball.radius,WHITE,BLACK);

            /* Drawing the pawns */
            /* Player 1*/
            DrawCircle(player1.x,player1.y,player1.radius,BLACK);
            DrawCircle(player1.x,player1.y,40,BLUE);

            /* Player 2 */
            DrawCircle(player2.x,player2.y,player2.radius,BLACK);
            DrawCircle(player2.x,player2.y,40,RED);

            /* Goalkeeper 1*/
            DrawCircle(gkeeperplayer1.x,gkeeperplayer1.y,gkeeperplayer1.radius,BLACK);
            DrawCircle(gkeeperplayer1.x,gkeeperplayer1.y,25,BLUE);
            DrawText("G",gkeeperplayer1.x - 5,gkeeperplayer1.y - 7,20,BLACK);

            /* Goalkeeper 2 */
            DrawCircle(gkeeperplayer2.x,gkeeperplayer2.y,gkeeperplayer2.radius,BLACK);
            DrawCircle(gkeeperplayer2.x,gkeeperplayer2.y,25,RED);
            DrawText("G",gkeeperplayer2.x - 5,gkeeperplayer2.y - 7,20,BLACK);



            /* Drawing the goalpost's */

            /* Drawing the goalposts after we draw the ball, so the ball can go underneath the goalposts */

            /* Goalpost 1 */
            Rectangle goalpost1 = {0, (screenHeight / 2) - 250,200,500};
            DrawRectangleLinesEx(goalpost1,15,DARKBLUE);

            /* Vertical Goalpost Net 1*/
            for(int i=0; i<4; i++)
            {
                DrawRectangle(10 + 8*i + 30*(i+1),550 + 10,8,480,WHITE);
            }
            /* Horizontal Goalpost Net 1*/
            for(int i=0; i<8; i++)
            {
                DrawRectangle(10,550 + 10 +8*i + 46*(i+1),180,8,WHITE);
            }

            /* Goalpost 2 */
            Rectangle goalpost2 = {2600, (screenHeight / 2) - 250,200,500};
            DrawRectangleLinesEx(goalpost2,15,MAROON);

            /* Vertical Goalpost Net 2*/
            for(int i=0; i<4; i++)
            {
                DrawRectangle(2600+ 10 + 8*i + 30*(i+1),550 + 10,8,480,WHITE);
            }

            /* Horizontal Goalpost Net 2*/
            for(int i=0; i<8; i++)
            {
                DrawRectangle(2600+10,550 + 10 +8*i + 46*(i+1),180,8,WHITE);
            }

            /* END OF NEEDED OBJECTS */

            
            /* ------------------------------------------------------------------------------------------------ */

            /* Text's to help the user understand the game */
            DrawRectangle(25,(screenHeight / 2) + 250 + 100,150,180,GREEN);
            DrawRectangle(2625,(screenHeight / 2) + 250 + 100,150,180,GREEN);
            DrawText("TIP:",25 + 40,(screenHeight / 2) + 250 + 100 + 20,30,BLACK);
            DrawText("TIP:",2625 + 40,(screenHeight / 2) + 250 + 100 + 20,30,BLACK); 
            DrawText("WASD + SPACE",25 + 5,(screenHeight / 2) + 250 + 120 + 40,19,BLACK);
            DrawText("ARROW'S + CTRL",2625 + 1,(screenHeight / 2) + 250 + 120 + 40,18,BLACK);
            DrawText("to access the",25 + 10,(screenHeight / 2) + 250 + 120 + 70,19,BLACK);
            DrawText("to access the",2625 + 10,(screenHeight / 2) + 250 + 120 + 70,19,BLACK);
            DrawText("goalkeeper!",25 + 15,(screenHeight / 2) + 250 + 120 + 100,19,BLACK);
            DrawText("goalkeeper!",2625 + 15,(screenHeight / 2) + 250 + 120 + 100,19,BLACK);



            /* What happens when a goal is scored */
            if(((ball.x + ball.radius) < 200) || ((ball.x - ball.radius) > 2600))
            {
                if(!IsKeyPressed(KEY_ENTER))
                {
                    DrawText("GOAL!",(screenWidth / 2) - 440, (screenHeight / 2) - 150,300, BLACK);
                    DrawText("GOAL!",(screenWidth / 2) - 425, (screenHeight / 2) - 150,300, YELLOW);
                    DrawText("Press enter to continue playing",(screenWidth / 2) - 445,(screenHeight / 2) + 100,50,BLACK);
                }  
            }


            DrawFPS(10,10);
        }
            
    EndDrawing();
}