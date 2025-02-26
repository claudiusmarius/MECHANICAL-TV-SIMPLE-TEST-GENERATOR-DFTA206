// DFT_#A206 MECHANICAL TV SIMPLE TEST GENERATOR
// SEEEDUINO XIAO
// Voir GitHub : https://github.com/claudiusmarius/MECHANICAL-TV-SIMPLE-TEST-GENERATOR-DFTA206 pour de plus amples explications
// Il s'agit d'une mire trés simple, NBTV signal generator pour s'ssurer du bon fonctionnement d'une télévision mécanique
// 4 images sont sélectionnables par SW1 (voir schéma)
// Ce dispositif est évolutif pour contenir des images plus complexes, je joindrai les codes au fur et à mesure de leur mise au point dans mon GitHub relatif à ce projet
// Ma chaine YouTube : https://www.youtube.com/c/ClaudeDufourmont
// Mon GitHub : https://github.com/claudiusmarius
#define VIDEO_PIN A0  // Sortie DAC (signal vidéo)
#define SYNC_PIN D1   // Sortie numérique (synchro)
#define SOUND_PIN D2  // Sortie du signal sonore
#define SELECT_PIN A3 // Entrée pour lire la tension du sélecteur
#define INHIBER_SYNCHRO_PIN D4 // Pin de contrôle pour inhiber la synchro sur la ligne 32

#define NB_LIGNES 32
#define DUREE_SYNCHRO 200   // Durée du top synchro en µs
#define PERIODE_LIGNE 2500  // Période d'une ligne en µs

#define VIDEO_NOIR 0        // 0V (Noir)
#define VIDEO_GRIS 512      // ~1.6V (Gris)
#define VIDEO_BLANC 768     // ~2.4V (Blanc)

#define INHIBER_SYNCHRO_LIGNE_32 true  // Supprimer la synchro ligne 32 si true

// Seuils ajustés avec des marges plus larges
#define SEUIL_MODE_1_MIN 0
#define SEUIL_MODE_1_MAX 450   // 0V-0.75V
#define SEUIL_MODE_2_MIN 800
#define SEUIL_MODE_2_MAX 1100  // 0.75V-1.647V
#define SEUIL_MODE_3_MIN 1500
#define SEUIL_MODE_3_MAX 2200  // 1.647V-2.74V
#define SEUIL_MODE_4_MIN 3000
#define SEUIL_MODE_4_MAX 4095  // 2.74V-3.3V

void setup() {
  Serial.begin(115200);  // Démarre la communication série pour monitorer les valeurs
  
  analogReadResolution(12); // Résolution de 12 bits (plage de 0 à 4095)
  pinMode(VIDEO_PIN, OUTPUT);
  pinMode(SYNC_PIN, OUTPUT);
  pinMode(SOUND_PIN, OUTPUT);
  pinMode(SELECT_PIN, INPUT);
  //pinMode(INHIBER_SYNCHRO_PIN, INPUT_PULLUP); // Configuration de D4 avec résistance de pull-up
  pinMode(INHIBER_SYNCHRO_PIN, INPUT);

  // Démarrer le signal sonore de 1 kHz sur SOUND_PIN
  tone(SOUND_PIN, 1000);
}

void loop() {
  // Lire la valeur du sélecteur sur A3
  int valeurADC = analogRead(SELECT_PIN); // Lecture de la valeur ADC sur A3

  // Afficher la valeur ADC lue sur le Serial Monitor
  Serial.print("Valeur ADC lue sur le sélecteur : ");
  Serial.println(valeurADC);
  
  // Détecter le mode sélectionné en fonction de la valeur de l'ADC
  int mode = detecterMode(valeurADC);
  Serial.print("Mode détecté : ");
  Serial.println(mode);

  // Générer la trame vidéo en fonction du mode sélectionné
  for (int ligne = 1; ligne <= NB_LIGNES; ligne++) {
    // Vérification si D4 est à HIGH ou LOW
    bool inhiberSynchro = (digitalRead(INHIBER_SYNCHRO_PIN) == LOW);  // Si D4 est LOW, on inhibe la synchro sur la ligne 32

    int dureeSynchro = (inhiberSynchro && ligne == 32) ? 0 : DUREE_SYNCHRO;

    // Debug : Vérification de la suppression de la synchro pour la ligne 32
    if (ligne == 32 && inhiberSynchro) {
      Serial.println("Ligne 32 : Suppression du top synchro");
    }

    // Génération du signal de synchro
    digitalWrite(SYNC_PIN, LOW);
    analogWrite(VIDEO_PIN, VIDEO_NOIR); // Noir pendant la synchro
    delayMicroseconds(dureeSynchro);
    digitalWrite(SYNC_PIN, HIGH);

    // Sélectionner la trame vidéo selon le mode
    int niveauDAC = genererTrame(mode, ligne);

    // Appliquer la valeur correcte au DAC
    analogWrite(VIDEO_PIN, niveauDAC);

    // Attente pour compléter la ligne
    delayMicroseconds(PERIODE_LIGNE - dureeSynchro);
  }
}

// Fonction pour détecter le mode sélectionné en fonction de la tension lue
int detecterMode(int valeurADC) {
  // Vérification des seuils pour chaque mode
  if (valeurADC >= SEUIL_MODE_1_MIN && valeurADC <= SEUIL_MODE_1_MAX) {
    return 1;  // Mode 1
  }
  if (valeurADC >= SEUIL_MODE_2_MIN && valeurADC <= SEUIL_MODE_2_MAX) {
    return 2;  // Mode 2
  }
  if (valeurADC >= SEUIL_MODE_3_MIN && valeurADC <= SEUIL_MODE_3_MAX) {
    return 3;  // Mode 3
  }
  if (valeurADC >= SEUIL_MODE_4_MIN && valeurADC <= SEUIL_MODE_4_MAX) {
    return 4;  // Mode 4
  }
  return 1;  // Par défaut, mode 1
}

// Fonction pour générer la trame vidéo en fonction du mode sélectionné
int genererTrame(int mode, int ligne) {
  switch (mode) {
    case 1: // Trame originale : Noir, Gris, Blanc
      if (ligne <= 8) return VIDEO_GRIS;
      if (ligne <= 16) return VIDEO_NOIR;
      if (ligne <= 24) return VIDEO_BLANC;
      return VIDEO_NOIR;

    case 2: // Dégradé progressif du Noir au Blanc
      return map(ligne, 1, NB_LIGNES, VIDEO_NOIR, VIDEO_BLANC);

    case 3: // Effet damier (Noir et Blanc en alternance)
      return (ligne % 2 == 0) ? VIDEO_NOIR : VIDEO_BLANC;

    case 4: // Test pattern : Noir, Gris foncé, Gris clair, Blanc
      if (ligne <= 8) return VIDEO_NOIR;
      if (ligne <= 16) return VIDEO_GRIS / 2;
      if (ligne <= 24) return (VIDEO_GRIS + VIDEO_BLANC) / 2;
      return VIDEO_BLANC;
  }
  return VIDEO_NOIR; // Sécurité si le mode est invalide
}

