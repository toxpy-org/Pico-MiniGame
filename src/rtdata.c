struct Position
{
    int x;
    int y;
};

struct SpeedOpt
{
    int normal;
    int fast;
    int usedFast;
};


struct Enemy {
    struct Position position;
    int size;
    int i; 
};

struct RTEnemies {
    struct Enemy *enemies[12];
    int spawnTick;
};

struct RuntimeData
{
    struct Position position;
    int size;
    struct SpeedOpt speedOpt;
    int speed;
    int energy;
    int boost;
    int health;
    long last;
    struct RTEnemies rtenemies;
    bool gameOver;
    bool usingEnergy; 
    int points;
};

typedef struct RuntimeData RTdata;

typedef struct Enemy EnemyType;
