#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define HP(level) ((level-1)*5)+20
					
#define NUM_ARMOR_TYPES 5
#define NUM_WEAPON_TYPES 5


typedef struct{
	char name[25];
	int armorClass;
}Armor;

typedef struct{
	char name[25];	
	int rolls;
	int maxDmg;
}Weapon;	

typedef struct{
	char name[25];
	Armor parmor;
	Weapon pweapon;
	int hp;
	int level;
	int xp;
	int id;
}Entity;	

//Armor Array
const static Armor aArray[] = {{"Cloth",10},
				 {"Studded Leather",12},
				 {"Ring Mail",14},
				 {"Chain Mail",16},
				 {"Plate",18}
				};
//Weapon Array
const static Weapon wArray[] = {{"Dagger",1,4},
				 {"Short Sword",1,6},
				 {"Long Sword",1,8},
				 {"Great Sword",2,6},
				 {"Great Axe",1,12}
			       };

int rollDice(int sides){

	return (rand()%sides);
}

int calcXP(int level){
	int i;
	int value = 0;
	for(i = 1;i<=level;i++){
		if(i == 1){
			value += 2;	
		}else{
			value = value*2;
		}
	}
	return value*1000;
}

Armor getArmorChoice(){
	int selection = 6;
	while(selection >= NUM_ARMOR_TYPES){
		printf("List of avaliable armors:\n");
		int i;
		for(i = 0;i<NUM_ARMOR_TYPES;i++){
			printf("%d: %s (AC=%d)\n",i,aArray[i].name,aArray[i].armorClass);	
		}		
		printf("Please select the armor you would like to use(0-4):");
		scanf("%d",&selection);
	}
	return aArray[selection];
}

Weapon getWeaponChoice(){
	int selection = 6;
	while(selection >= NUM_WEAPON_TYPES){
		printf("List of avaliable weapons:\n");
		int i;
		for(i = 0;i<NUM_WEAPON_TYPES;i++){
			printf("%d: %s (damage=%dd%d)\n",i,wArray[i].name,wArray[i].rolls,wArray[i].maxDmg);	
		}		
		printf("Please select the weapon  you would like to use(0-4):");
		scanf("%d",&selection);
	}
	return wArray[selection];
}


void stats(Entity e){
	printf("%d: [%s: hp=%d, armor=%s, weapon=%s, level=%d, xp=%d]\n",e.id,e.name,e.hp,e.parmor.name,e.pweapon.name,e.level,e.xp);	
}

void look(Entity ens[10]){
	printf("You take a look around, assessing the vile wretched scum that inhabit the land\n");
	int i;
	for(i = 0;i<10;i++){
		stats(ens[i]);
	}
}

Entity generateEnemy(int id,int playerLvl){
	Entity e = {0};
	if(id == 0){
		strcpy(e.name,"Sauron");
		e.parmor = aArray[4];
		e.pweapon = wArray[4];
		e.level = 20;
		e.xp = calcXP(e.level);
		e.id = id;
		e.hp = HP(e.level);
	}
	else if(id == 9){	
		strcpy(e.name,"Golem");
		e.parmor = aArray[0];
		e.pweapon = wArray[0];
		e.level = 1;
		e.xp = calcXP(e.level);
		e.id = id;
		e.hp = HP(e.level);
	}
	if(id >= 1 && id <= 8){
		strcpy(e.name,"Orc");
		e.parmor = aArray[rollDice(4)];
		e.pweapon = wArray[rollDice(4)];
		int temp = rollDice(playerLvl);
		if(temp == 0){
			e.level = 1;
		}else{
			e.level = temp;
		}	
		e.xp = calcXP(e.level);
		e.id = id;
		e.hp = HP(e.level);
	}	
	return e;
}

int calculateDamage(Entity a){
	int dmg = 0;
	int i;
	for(i=0;i<a.pweapon.rolls;i++){
		dmg+= rollDice(a.pweapon.maxDmg);
	}
	return dmg;

}

void fight(Entity* a,Entity* b){
	while(a->hp > 0 && b->hp > 0){
		int atkRoll = rollDice(20)+1;
		if(atkRoll >= b->parmor.armorClass){
			int dmg = calculateDamage(*a);
			b->hp -= dmg;
			printf("%s hits %s for %d damage: (attack roll %d)\n",a->name,b->name,dmg,atkRoll);
		}else{
			printf("%s misses %s: (attack roll %d)\n",a->name,b->name,atkRoll);
		}
		atkRoll = rand() % (20-1+1)+1;
		if(atkRoll >= a->parmor.armorClass){
			int dmg = calculateDamage(*b);
			a->hp -= dmg;
			printf("%s hits %s for %d damage: (attack roll %d)\n",b->name,a->name,dmg,atkRoll);
		}else{
			printf("%s misses %s: (attack roll %d)\n",b->name,a->name,atkRoll);
		}
	}
	if(a->hp >= 0 && b->hp <= 0){
		printf("%s has slain %s\n",a->name,b->name);
		printf("Exchange for %s's %s armor?(y/n):",b->name,b->parmor.name);
		char decision;
		scanf(" %c",&decision);
		if(decision == 'y'){
			a->parmor = b->parmor;
		}
		printf("Exchange for %s's %s?(y/n):",b->name,b->pweapon.name);
		scanf(" %c",&decision);
		if(decision == 'y'){
			a->pweapon = b->pweapon;
		}
		a->xp += 2000*b->level;
		if(a->xp >= calcXP(a->level+1)){
			a->level += 1;
			printf("Congratulation! You bave leveled up!\n");
		}
		a->hp = HP(a->level);	
		stats(*a);
		printf("Respawning %s...\n",b->name);
		*b = generateEnemy(b->id,a->level);
	}else{
		if(a->hp <= 0 && b->hp >= 0){
			printf("%s has slain %s\n",b->name,a->name);
		}else{
			printf("You have both succomb to death's sweet embrace\n");
			printf("Respawning %s...\nRespawning...%s\n",a->name,b->name);	
			*b = generateEnemy(b->id,a->level);
		}
		a->xp = calcXP(a->level);
		a->hp = HP(a->level);
	}	
}

void save(Entity* e,Entity enemies[10]){
	FILE* fp = fopen("rpg.save","wb");
	if(fp == NULL){
		printf("Error");
	}
	fwrite(e,sizeof(Entity),1,fp);
	fwrite(enemies,sizeof(Entity),10,fp);
	fclose(fp);
}

int load(Entity* pl,Entity enemies[10]){
	FILE* fp = fopen("rpg.save","rb");
	if(fp == NULL){
		return 0;
	}
	fread(pl,sizeof(Entity),1,fp);
	fread(enemies,sizeof(Entity),10,fp);
	fclose(fp);
	return 1;
}

Entity player = {0};
Entity enemies[10];
void sig_handle(int signo){
	if(signo == SIGTERM || signo == SIGINT){
		save(&player,enemies);
	}
	if(signo == SIGRTMIN){
		
	}
}


int main(char* argc,char** argv){
		
	srand(time(NULL));
	signal(SIGTERM,sig_handle);
	signal(SIGINT,sig_handle);
	char response;
	int loaded = 0;
	if(load(&player,enemies) == 1){
		loaded = 1;
		printf("Save file found, would you like to continue?:(y/n)");
		scanf("%c",&response);	
	}
	if(response != 'y' || loaded == 0){
		printf("What is your name?:");	
		scanf(" %s",&player.name);
		printf("\n");
		player.parmor = getArmorChoice();
		player.pweapon = getWeaponChoice();	
		player.level = 1;
		player.xp = calcXP(1);
		player.id = -1;
		player.hp = HP(player.level);

		printf("You're ready to begin your journey!\n");
		int i;
		for(i = 0;i<10;i++){
			enemies[i] = generateEnemy(i,player.level);
		}
		stats(player);
	}
	int running = 1;
	while(running == 1){
		char command[15] = {0};
		printf("command>>");
		scanf(" ");
		fgets(command,15,stdin);	
		if(strncmp(command,"stats",4) == 0){
			stats(player);
		}else if(strncmp(command,"quit",3) == 0){
			save(&player,enemies);	
			running = 0;
		}else if(strncmp(command,"look",3) == 0){
			look(enemies);
		}
		else if(strncmp(command,"fight",4)==0){	
			int id = command[6] - '0';
			fight(&player,&enemies[id]);
		}		
	}
}



