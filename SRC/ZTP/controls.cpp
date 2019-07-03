#include "../COMMON.H"

/***
In case you are wondering, I reused code from my early FPS tests, so you can see stuff like "is_firing" which is obviously not for the model converter...
***/

extern int DisplayTEST;
extern int ZT_FRAMERATE;

#define KB_A 65
#define KB_D 68
#define KB_W 87
#define KB_S 83
#define KB_Z 90
#define KB_Q 81
#define KB_E 69
#define KB_C 67
#define KB_ESC 126

#define AslMulFX(a,b) (a*b)

sKeyState m_keys[256], m_mouse[5];
sKeyState GetKey(int nKeyID){ return m_keys[nKeyID]; }


short   m_keyNewState[256]    = { 0 };
short   m_keyOldState[256]    = { 0 };
POINT   p                     = { 0 };
VECTOR  m_pos                 = { 0 };

void resetKeyPresses(player_t * player)
{
    if (player->PLAYER_ID >= 4) return;

    for (int i = 0; i < 256; i++)
    {
        m_keyNewState[i] = GetAsyncKeyState(i);
		m_keys[i].bPressed = false;	m_keys[i].bReleased = false;

		if (m_keyNewState[i] != m_keyOldState[i])
		{
            if (m_keyNewState[i] & 0x8000)
			{
                m_keys[i].bPressed = !m_keys[i].bHeld;
				m_keys[i].bHeld = true;
            }
			else
			{
                m_keys[i].bReleased = true;
				m_keys[i].bHeld = false;
            }
        }
        m_keyOldState[i] = m_keyNewState[i];
    }

}


#define SPEED_INCREASE (0.5f*ZT_FRAMERATE)
#define FRICTION        (0.25f)
#define SPEED_VALUE     (3.0f)
#define TURN_INCREASE   (0.001375f * ZT_FRAMERATE)
#define LOOK_INCREASE   (0.001f * ZT_FRAMERATE)
/**Movement**/
void move_front(player_t * currentPlayer){   if ((currentPlayer->MOVEMENT_SPEED+=SPEED_INCREASE)>SPEED_VALUE)       currentPlayer->MOVEMENT_SPEED=SPEED_VALUE;}
void move_back(player_t * currentPlayer){    if ((currentPlayer->MOVEMENT_SPEED-=SPEED_INCREASE)<-SPEED_VALUE)      currentPlayer->MOVEMENT_SPEED=-SPEED_VALUE;}
void move_left(player_t * currentPlayer){    if ((currentPlayer->LATERAL_SPEED -= SPEED_INCREASE) < -SPEED_VALUE)   currentPlayer->LATERAL_SPEED = -SPEED_VALUE;}
void move_right(player_t * currentPlayer){   if ((currentPlayer->LATERAL_SPEED += SPEED_INCREASE) > SPEED_VALUE)    currentPlayer->LATERAL_SPEED = SPEED_VALUE;}

void move_friction(player_t * currentPlayer)
{
    if (currentPlayer->MOVEMENT_SPEED > SPEED_INCREASE)          currentPlayer->MOVEMENT_SPEED-=FRICTION*ZT_FRAMERATE;
    else if (currentPlayer->MOVEMENT_SPEED < -SPEED_INCREASE)   currentPlayer->MOVEMENT_SPEED+=FRICTION*ZT_FRAMERATE;
    else currentPlayer->MOVEMENT_SPEED = 0;
}


/**For the Lookup/down values : DEGtoANG(45) = 45/360 * 65536.0**/
void look_up(player_t * currentPlayer)   {    currentPlayer->ROTATION[Z]+=LOOK_INCREASE;    if (currentPlayer->ROTATION[Z] > 8192)  currentPlayer->ROTATION[Z] = 8192;}
void look_down(player_t * currentPlayer) {    currentPlayer->ROTATION[Z] -= LOOK_INCREASE;    if (currentPlayer->ROTATION[Z] < -8192) currentPlayer->ROTATION[Z] = -8192;}

/**Turn left/right - SATURN ONLY, DIGITAL PAD ONLY**/
void turn_left(player_t * currentPlayer){    currentPlayer->ROTATION[Z] -= TURN_INCREASE;}
void turn_right(player_t * currentPlayer){    currentPlayer->ROTATION[Z] += TURN_INCREASE;}

/**Fire current player's weapon**/
void fire_weapon(player_t * currentPlayer)
{ if (currentPlayer->IS_FIRING==0)
    {
        currentPlayer->IS_FIRING=32;
    }
}//Should be set depending on the weapons' value. Kind of like a timer to allow you to fire again, the firerate...

void resetPlayer(player_t * player)
{
    player->POSITION[X]=player->POSITION[Y]=player->POSITION[Z]=0;
    player->IS_FIRING=0;
    player->LATERAL_SPEED=0;
    player->MOVEMENT_SPEED=0;
    player->ROTATION[X]=player->ROTATION[Y]=player->ROTATION[Z]=0;

    player->POSITION[Z]=50;
    player->ROTATION[Z]=-180.0;
}
void debugKeys(player_t * player)
{
    if (m_keys['X'].bHeld || m_keys['V'].bPressed)
    {if (++DisplayTEST > 1) DisplayTEST=0;}
    if (m_keys['B'].bPressed)
    {if (--DisplayTEST < 0) DisplayTEST=1;}

    if (m_keys['R'].bPressed)
        resetPlayer(player);

    if (m_keys['F'].bPressed)
    {
        ZT_FRAMERATE++;
        if (ZT_FRAMERATE>4) ZT_FRAMERATE=1;
    }

    extern int enableInterpolation;
    if (m_keys['I'].bPressed)
        enableInterpolation ^= 1;
}

void mouse_controls(player_t * player)
{
    ShowCursor(false);
    if (GetCursorPos(&p))
    {
        p.x = p.x - 640 * 0.5f;
        p.y = p.y - 480 * 0.5f;
    }
    player->ROTATION[Z] -= p.x * 0.0625f;
    player->ROTATION[X] += p.y * 0.0625f;
    SetCursorPos(640*0.5f,480*0.5f);
}


int keyboard_controls(player_t * player)
{
    if (m_keys[KB_ESC].bHeld || m_keys['O'].bHeld) return -1;
    /**REMOVED FOR DEBUGGING**/
    if ((player->STATUS & CAN_JUMP)&&(m_keys[' '].bPressed || (GetKeyState(VK_RBUTTON) < 0)))
    {
        player->SPEED[Y] = -20.0f;
        //player->STATUS &= ~CAN_JUMP;
    }

    if (m_keys['D'].bHeld)
        move_right(player);
    else if (m_keys['A'].bHeld)
        move_left(player);
    else if (player->LATERAL_SPEED > SPEED_INCREASE) player->LATERAL_SPEED-=SPEED_INCREASE;
    else if (player->LATERAL_SPEED < -SPEED_INCREASE) player->LATERAL_SPEED+=SPEED_INCREASE;
    else player->LATERAL_SPEED=0;

    if (fabs(player->LATERAL_SPEED)< FRICTION) player->LATERAL_SPEED=0;

    if (m_keys['W'].bHeld)
    {
        move_front(player);
    }
    else if (m_keys['S'].bHeld)
    {
        move_back(player);
    }
    else move_friction(player);

    /**For debugging, no collision**/
    if (GetKeyState(VK_RBUTTON) < 0)
        player->SPEED[Y]-=1.0f*ZT_FRAMERATE;
    else if (GetKeyState(VK_LBUTTON) < 0)
        player->SPEED[Y]+=1.0f*ZT_FRAMERATE;
    else player->SPEED[Y]*=0.5f;

    if (fabs(player->SPEED[Y]) < 1.0f/32.0f) player->SPEED[Y]=0;

    debugKeys(player);

    return 0;
}


/**Update player's data**/
void update(player_t * player)
{
    player->ROTATION[Y] = player->ROTATION[Z];

    player->SPEED[Z] = AslMulFX(std::cos(player->ROTATION[Y]*conv), (player->MOVEMENT_SPEED)) +
        AslMulFX(std::sin(player->ROTATION[Y]*conv), (player->LATERAL_SPEED));

    player->SPEED[X] = -AslMulFX(std::sin(player->ROTATION[Y]*conv), (player->MOVEMENT_SPEED)) +
        AslMulFX(std::cos(player->ROTATION[Y]*conv), (player->LATERAL_SPEED));

    if (player->SPEED[Y] > 15.0f) player->SPEED[Y] = 15.0f;
    player->NEXT_POSITION[X] = player->POSITION[X] + player->SPEED[X] * ZT_FRAMERATE;
    player->NEXT_POSITION[Y] = player->POSITION[Y] + player->SPEED[Y] * ZT_FRAMERATE;
    player->NEXT_POSITION[Z] = player->POSITION[Z] + player->SPEED[Z] * ZT_FRAMERATE;

    //Disabled for debug
    //player->STATUS &= ~CAN_JUMP;
}
/**Temporary functions...**/
int myControls(player_t * player)
{


   /* if (is_input_available(currentPlayer->PLAYER_ID) == false) return;

    if(Smpc_Peripheral[currentPlayer->PLAYER_ID].id == 0x16)
        {analog_control(currentPlayer);}
	else
        {digital_control(currentPlayer);}

    update(currentPlayer);*/


   // player->SPEED[Y] += 3.0f * ZT_FRAMERATE;

    mouse_controls(player);
    resetKeyPresses(player);
    if (keyboard_controls(player)==-1) return -1;

    update(player);
       //     cout << "Player Position : " << (int)player->POSITION.x << ", " << (int)player->POSITION.y << ", " << (int)player->POSITION.z << "\n";
    return 0;
}
