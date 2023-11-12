#include <math.h>

void Enemy_Update(RTdata *data, EnemyType *enemy);
void Enemy_Collision(RTdata *data, EnemyType *enemy);

int SizeOfEnemies(RTdata *data) {
    return sizeof(data->rtenemies.enemies) / sizeof(data->rtenemies.enemies[0]);
}

struct Enemy *EnemySelect(int i, RTdata *data) {
    return data->rtenemies.enemies[i];
}

void Init_Enemies(RTdata *data) 
{
    struct RTEnemies rtenemies;

    for (int x = 0; x < SizeOfEnemies(data); x++) 
    {
        rtenemies.enemies[x] = NULL;
    }

    rtenemies.spawnTick = 0;

    data->rtenemies = rtenemies;
}

int EnemiesInWorld(RTdata *data) 
{
    int t = 0;
    for (int x = 0; x < SizeOfEnemies(data); x++) 
    {
        if (data->rtenemies.enemies[x] != NULL) t++;
    }

    return t;
}

void Spawn_Enemy(RTdata *data) 
{
    int selectedIndex = -1;

    for (int i = 0; i < SizeOfEnemies(data); i++) {
        if (EnemySelect(i, data) == NULL) {
            selectedIndex = i;
            break;
        }
    }

    if (selectedIndex == -1) return;

    struct Enemy *enemy = malloc(sizeof(struct Enemy));
    if (enemy == NULL) exit(1); // NO MOMERY LEFT
    
    enemy->size = 4;

    int x = rand() % 2;

    if (x == 0) {
        enemy->position.x = rand() % 20;
        enemy->position.y = rand() % (LCD_2IN_HEIGHT - enemy->size);
    }

    if (x == 1) {
        enemy->position.x = rand() % (LCD_2IN_WIDTH - enemy->size);
        enemy->position.y = rand() % 20;
    }

    enemy->i = selectedIndex;

    data->rtenemies.enemies[selectedIndex] = enemy;
}

void Destroy_Enemy(RTdata *data, EnemyType *enemy) {
    data->rtenemies.enemies[enemy->i] = NULL;
    free(enemy);
}

void Enemies_Loop(RTdata *data)
{           
    data->rtenemies.spawnTick++;

    if (rand() % 4 == 0 && data->rtenemies.spawnTick > 120) {
        Spawn_Enemy(data);
        data->rtenemies.spawnTick = 0;
    } 

    for (int i = 0; i < SizeOfEnemies(data); i++) {
        struct Enemy *enemy = data->rtenemies.enemies[i];
        if (enemy == NULL) continue;

        Enemy_Update(data, enemy);
    
        Paint_DrawRectangle(
            enemy->position.x, enemy->position.y,
            enemy->position.x + enemy->size, enemy->position.y + enemy->size,
            RED,
            DOT_PIXEL_1X1, DRAW_FILL_EMPTY
        );
    }
}

struct Vec {
    double x;
    double y;
};

typedef struct Vec VecType;

struct Vec *CreateVector(double x, double y) {
    struct Vec *v = malloc(sizeof(struct Vec));

    v->x = x;
    v->y = y;

    return v;
}

double Vector_Mag(VecType *v) {
    return sqrt(pow(v->x, 2) + pow(v->y, 2));
}

struct Vec *Vector_Sub(VecType *v1, VecType *v2) {
    return CreateVector(v1->x - v2->x, v1->y - v2->y);
}

struct Vec *Vector_Unit(VecType *v) {
    double mag = Vector_Mag(v);
    if (mag == 0) mag = 0.1; 
    return CreateVector(v->x / mag, v->y / mag);
}

struct Vec *Player_VectorDirectionFromEnemy(RTdata *data, EnemyType *enemy) {
    struct Vec *playerVec = CreateVector((double) data->position.x, (double) data->position.y);
    struct Vec *enemyVec = CreateVector((double) enemy->position.x, (double) enemy->position.y);

    struct Vec *difVec = Vector_Sub(playerVec, enemyVec);
    struct Vec *difUnit = Vector_Unit(difVec);

    free(difVec);
    free(playerVec);
    free(enemyVec);

    return difUnit;
}

bool Player_EnemyCollided(RTdata *data, EnemyType *enemy) {
    struct Vec *playerVec = CreateVector((double) data->position.x, (double) data->position.y);
    struct Vec *enemyVec = CreateVector((double) enemy->position.x, (double) enemy->position.y);

    struct Vec *difVec = Vector_Sub(playerVec, enemyVec);

    int mag = Vector_Mag(difVec);

    free(difVec);
    free(playerVec);
    free(enemyVec);

    return mag < enemy->size + data->size;    
}


void Enemy_Update(RTdata *data, EnemyType *enemy) 
{
    struct Vec *direc = Player_VectorDirectionFromEnemy(data, enemy);
    
    enemy->position.x += (int) (direc->x * 1.5);
    enemy->position.y += (int) (direc->y * 1.5);

    if (Player_EnemyCollided(data, enemy)) Enemy_Collision(data, enemy);

    free(direc); 
}

void Enemy_Collision(RTdata *data, EnemyType *enemy) {
    Destroy_Enemy(data, enemy);

    if (data->usingEnergy) {
        data->points++;
    } else {
        data->health -= rand() % 10 + 5;
    }
}
