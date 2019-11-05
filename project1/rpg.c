#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int hp;
	int armorClass;
	char* armorType;
	char* weaponType;
	int numDice;
	int dmgRange;	
}Player;

static int armorClasses[5] = {10,12,14,16,18};	

int calculateDamage(Player p){
	int dmg = 0;
	int i;
	for(i = 0;i<p.numDice;i++){
		dmg += rand() %(p.dmgRange-1+1)+1;
	}
	return dmg;
}

void fight(Player a,Player b){
	int roundNum = 0;
	int aHealth = a.hp;
	int bHealth = b.hp;
	while(aHealth > 0 && bHealth > 0){
		printf("------ %d ------\n",roundNum);
		int aAtkRoll = rand() % (20-1+1)+1;
		if(aAtkRoll >= b.armorClass){
			int dmg = calculateDamage(a);
			bHealth -= dmg;
			printf("Player 1 hits Player 2 for %d damage,(attack roll %d)\n",dmg,aAtkRoll);	
		}else{
			printf("Player 1 misses Player 2 (attack roll %d)\n",aAtkRoll);
		}

		int bAtkRoll = rand() % (20-1+1)+1;
		if(bAtkRoll >= a.armorClass){
			int dmg =  calculateDamage(b);
			aHealth -= dmg;
			printf("Player 2 hits Player 1 for %d damage, (attack roll %d)\n",dmg,bAtkRoll);
		}else{
			printf("Player 2 misses Player 1 (attack roll %d)\n",bAtkRoll);
		}
		printf("[Player 1: HP=%d, Armor=%s, Weapon=%s]\n",aHealth,a.armorType,a.weaponType);
		printf("[Player 2: HP=%d, Armor=%s, Weapon=%s]\n",bHealth,a.armorType,a.weaponType); 
		roundNum++;
	}
	if(aHealth > bHealth){
		printf("\n Player 1 Wins!\n");
	}
	if(bHealth > aHealth){
		printf("\n Player 2 Wins!\n");
	}
	
	if(aHealth == bHealth){
		printf("The Fight Ends With Both Warriors In Deaths Embrace, It is a tie");
	}		
}

void findWeaponType(int choice,Player* player){
	switch(choice){
		case 0:
			player->weaponType = "Dagger";
			player->numDice = 1;
			player->dmgRange = 4;
			break;
		case 1:
			player->weaponType = "Short Sword";
			player->numDice = 1;
			player->dmgRange = 6;
			break;
		case 2:
			player->weaponType = "Long Sword";
			player->numDice = 1;
			player->dmgRange = 8;
			break;
		case 3:
			player->weaponType = "Great Sword";
			player->numDice = 2;
			player->dmgRange = 6;
			break;
		case 4:
			player->weaponType = "Great Axe";
			player->numDice = 1;
			player->dmgRange = 12;
			break;
		default:
			player->weaponType = NULL;
			player->numDice = 0;
			player->dmgRange=0;
			break;
	}
}

char* findArmorType(int armorClass){
	switch(armorClass){
		case 10:
			return "Cloth";
			break;
		case 12:
			return "Studded Armor";
			break;
		case 14:
			return "Ring Mail";
			break;
		case 16:
			return "Chain Mail";
			break;
		case 18:
			return "Plate";
			break;
		default:
			return NULL;
			break;
	}
}

int main(int argc,char** argcv){
	srand((unsigned int)time(NULL));
	printf("List of Avaliable Armors:\n0: Cloth (AC=10)\n1: Studded Armor (AC=12)\n2: Ring Mail (AC=14)\n3: Chain Mail (AC=16)\n4: Plate (AC=18)\n");

	
	Player player1 = {0};
	Player player2 = {0};
	int selection; 
	while(1){
		printf("Player 1 Armor selection:");
		scanf("%d",&selection);
		printf("\n");
		player1.armorClass = armorClasses[selection];
		player1.armorType = findArmorType(player1.armorClass);
		if(player1.armorType != NULL){
			break;	
		}
	}
	while(1){
		printf("Player 2 Armor selection:");
		scanf("%d",&selection);
		printf("\n");
		player2.armorClass = armorClasses[selection];
		player2.armorType = findArmorType(player2.armorClass);	
		if(player2.armorType != NULL){
			break;
		}
	}


	printf("List of Avaliable Weapons:\n0: Dagger (DMG=1d4)\n1: Short Sword (DMG=1d6)\n2: Long Sword (DMG=1d8)\n3: Great Sword (DMG=2d6)\n4: Great Axe (DMG=1d12)\n");
	while(1){
		printf("Player 1 Weapon Selection:");
		scanf("%d",&selection);
		printf("\n");
		findWeaponType(selection,&player1);
		if(player1.weaponType != NULL){
			break;
		}
	}
	while(1){


		printf("Player 2 Weapon Selection:");
		scanf("%d",&selection);
		printf("\n");
		findWeaponType(selection,&player2);
		if(player2.weaponType != NULL){
			break;
		}
	}
	player1.hp = 20;
	player2.hp = 20;
	printf("Player Settings Complete:\n[Player 1: HP=%d, Armor=%s, Weapon=%s]\n[Player 2: HP=%d, Armor=%s, Weapon=%s]\n",player1.hp,player1.armorType,player1.weaponType,player2.hp,player2.armorType,player2.weaponType);
	
	printf("\nBegin Fight (Y/N)?:");
	char begin = '1';
	scanf(" %c",&begin);	
	printf("\n");
	while(begin == 'Y' || begin == 'y'){
		fight(player1,player2);
		printf("Fight again (Y/N)?:");
		scanf(" %c",&begin);
	}
		
	return 0;
}
