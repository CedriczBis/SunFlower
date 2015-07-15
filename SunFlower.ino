/*       *******   Sun_Flower   *******

Petite station de mesure de lumiere pour connaire le meilleur endroit de la maison ou placer vos plantes vertes.

*/

// On inclu les librairies necessaires
#include "Wire.h"
#include "LiquidCrystal.h"

// On defini les pins utilisees par les differents composants :
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int droiteCapt = A3;
const int hautCapt = A2;
const int gaucheCapt = A1;
const int basCapt = A0;
const int swich = 8;

// On defini les parametres de fonctionnement
const int delaiMesure = 864; // temps EN SECONDE entre deux prise de mesure (mode releve) -> 864 000 pour 100 mesures sur une journee (24h)
const int nbtableau = 101 ; // la taille des tableau de valeurs necessaire pour les 4 capteurs -> 101

// On initialise les tableaux de valeurs
int droite [nbtableau]; 
int gauche [nbtableau]; 
int haut [nbtableau];
int bas [nbtableau];
// On initialise les variables maximum des capteurs
int maxBas = 0;
int maxHaut = 0;
int maxGauche = 0;
int maxDroit = 0;
// On initialise le compteur de tour
int tour = 0;


void setup() {
  // On defini la taille de l'ecran LCD, on lance le serial
  lcd.begin(16, 2);
  Serial.begin(9600);
  // Le swich est une entree
  pinMode (swich, INPUT);
  // On lance la sequence de d�marrage
  lcd.print("   Sun Flower");
  lcd.setCursor(0,1);
  lcd.print ("  Made in zBis");
  delay(3000);

}

void loop() {  
	/* Demarrage par defaut en mode instantanee :
	On nettoi l'ecran et on recupere les valeurs des capteurs (remapper en %) pour l'afficher toute les secondes*/

    lcd.clear();
    lcd.print(map(analogRead(gaucheCapt),0,1023,0,100));
    lcd.setCursor(6,0);
    lcd.print(map(analogRead(hautCapt),0,1023,0,100));
    lcd.setCursor(13,0);
    lcd.print(map(analogRead(droiteCapt),0,1023,0,100));
    lcd.setCursor(0,1);
    lcd.print("Inst");
    lcd.setCursor(6,1);
    lcd.print(map(analogRead(basCapt),0,1023,0,100));
    delay(1000);

	// On initialise les valeurs d'un eventuel precedent releve
    tour = 0;
    maxBas = 0;
    maxHaut = 0;
    maxGauche = 0;
    maxDroit = 0;
    
	// Quand le swich passe en position releve, on lance la boucle correspondantes :
  while (digitalRead (swich) == 1){

	// On recupere les valeurs mappees et on les stocks dans la case du tableau correspondante au tour
    droite[tour] = map(analogRead(droiteCapt),0,1023,0,100);
    gauche[tour] = map(analogRead(gaucheCapt),0,1023,0,100);
    bas[tour] = map(analogRead(basCapt),0,1023,0,100);
    haut[tour] = map(analogRead(hautCapt),0,1023,0,100);

    // On verifie si une des valeurs relevees correspond a un max
    if (droite[tour] > maxDroit){
      maxDroit = droite[tour];
      }
    if (gauche[tour] > maxGauche){
      maxGauche = gauche[tour];
      }
    if (bas[tour] > maxBas){
      maxBas = bas[tour];
      }
    if (haut[tour] > maxHaut){
      maxHaut = haut[tour];
      }    

  	// on initialise les valeurs moyennes
    int moyDroite = 0;
    int moyGauche = 0;
    int moyBas = 0;
    int moyHaut = 0;

  	// On calcule les valeurs moyennes en ajoutant toutes les cases du tableau et en divisant par leur nombre
    for(int i = 0; i < tour+1; i++){
      moyGauche += gauche[i];
      moyDroite += droite[i];
      moyHaut += haut[i];
      moyBas += bas[i];
      }
    moyGauche = moyGauche / (tour+1);
    moyDroite = moyDroite / (tour+1);
    moyHaut = moyHaut / (tour+1);
    moyBas = moyBas / (tour+1);
	
	// On nettoie l'ecran pour afficher la nouvelle valeur moyenne de chaque capteur
    lcd.clear();
    lcd.print(moyGauche);
    lcd.setCursor(6,0);
    lcd.print(moyHaut);
    lcd.setCursor(13,0);
    lcd.print(moyDroite);
    lcd.setCursor(0,1);
    lcd.print("Wait");
    lcd.setCursor(6,1);
    lcd.print(moyBas);
    lcd.setCursor(11,1);
    lcd.print(tour+1);
    lcd.print("%");  

	// On attend le delais prevu et on incremente le tour
    for (int j=0; j < 1000; j++){
      delay(delaiMesure);
      }
    tour = tour + 1;
    
    // On verifie si le nombre de tour atteind sa limite. Si c'est le cas on affiche les valeurs moyennes
    if (tour == nbtableau-2){
      lcd.clear();
      lcd.print(moyGauche);
      lcd.setCursor(6,0);
      lcd.print(moyHaut);
      lcd.setCursor(13,0);
      lcd.print(moyDroite);
      lcd.setCursor(0,1);
      lcd.print("Fini");
      lcd.setCursor(6,1);
      lcd.print(moyBas);
      lcd.setCursor(11,1);
	  // On recherche la plus grande des valeurs maxi et on l'affiche ainsi que le capteurs d'ou elle provien
      int maxi = max(max(maxDroit,maxGauche),(max(maxBas,maxHaut)));
      if (maxi == maxDroit){
        lcd.print("Dr ");
        lcd.print(maxi);
      }
      if (maxi == maxGauche){
        lcd.print("Ga ");
        lcd.print(maxi);
      }
      if (maxi == maxHaut){
        lcd.print("Ha ");
        lcd.print(maxi);
      }
      if (maxi == maxBas){
        lcd.print("Ba ");
        lcd.print(maxi);
      }
      
      //Pour ne pas relancer un nouveau cycle on rentre dans une boucle vide (Peut etre utiliser pour recuperer plus de valeurs en serie)
      while (digitalRead (swich) == 1){
        Serial.print ("max total =");
        Serial.print (maxi);
        Serial.print ("max Gauche =");
        Serial.print (maxGauche);
        Serial.print ("max Droit =");
        Serial.print (maxDroit);
        Serial.print ("max Haut =");
        Serial.print (maxBas);
        Serial.print ("max Bas =");
        Serial.println (maxHaut);
        Serial.print ("Moyenne Gauche : ");
        Serial.print (moyGauche);
        Serial.print ("Moyenne Droite : ");
        Serial.print (moyDroite);
        Serial.print ("Moyenne Haut : ");
        Serial.print (moyHaut);
        Serial.print ("Moyenne Bas : ");
        Serial.println (moyBas);
        delay (1000);
      }
      }
        
  
  }
}
