// DFT_#A206 MECHANICAL TV ULTRA SIMPLE TEST GENERATOR
// SEEEDUINO XIAO
// Voir GitHub : https://github.com/claudiusmarius/MECHANICAL-TV-SIMPLE-TEST-GENERATOR-DFTA206 pour de plus amples explications
// Il s'agit d'une mire trés simple, NBTV signal generator pour s'ssurer du bon fonctionnement d'une télévision à balayage mécanique 32 lignes
// 4 images sont sélectionnables par SW1 (voir schéma)
// Ce dispositif est évolutif pour contenir des images plus complexes, je joindrai les codes au fur et à mesure de leur mise au point dans mon GitHub relatif à ce projet
// Ma chaine YouTube : https://www.youtube.com/c/ClaudeDufourmont
// Mon GitHub : https://github.com/claudiusmarius

#define VIDEO_PIN A0  // Sortie DAC (signal vidéo)
#define SYNC_PIN D1   // Sortie numérique (synchro)
#define SOUND_PIN D2  // Sortie du signal sonore
#define SELECT_PIN A3 // Entrée pour lire la tension du sélecteur
#define INHIBIT_PIN D4 // Entrée pour inhiber la suppression du top de synchro ligne 32

#define NB_LIGNES 32
#define DUREE_SYNCHRO 200     // Durée du top synchro en µs
#define PERIODE_LIGNE 2300    // Durée d'une ligne en µs (valeur approximative)
#define VIDEO_NOIR 0          // 0V (Noir)
#define VIDEO_GRIS 512        // ~1.6V (Gris)
#define VIDEO_BLANC 768       // ~2.4V (Blanc)

#define SEUIL_MODE_1_MIN 0    // Fourchette correspondant à la position 1 de SW1 (typiquement 0V)
#define SEUIL_MODE_1_MAX 450   

#define SEUIL_MODE_2_MIN 800    // Fourchette correspondant à la position 2 de SW1 (typiquement 0.75V)
#define SEUIL_MODE_2_MAX 1100   

#define SEUIL_MODE_3_MIN 1500   // Fourchette correspondant à la position 3 de SW1 (typiquement 1.5V)
#define SEUIL_MODE_3_MAX 2200  

#define SEUIL_MODE_4_MIN 3000   // Fourchette correspondant à la position 4 de SW1 (typiquement 2.64V)
#define SEUIL_MODE_4_MAX 4095  

void setup() {
  Serial.begin(115200);  // Démarre la communication série pour monitorer les valeurs
  analogReadResolution(12); // Résolution de 12 bits en lecture analogique (plage de 0 à 4095)
  
  pinMode(VIDEO_PIN, OUTPUT);
  pinMode(SYNC_PIN, OUTPUT);
  pinMode(SOUND_PIN, OUTPUT);
  pinMode(SELECT_PIN, INPUT);
  pinMode(INHIBIT_PIN, INPUT); // Résistance de pull up physique en place (R6)

  // Démarrer le signal sonore de 1 kHz sur SOUND_PIN
  tone(SOUND_PIN, 1000);
}

void loop() {
  // Lire la valeur du sélecteur sur A3
  int valeurADC = analogRead(SELECT_PIN);

  // Lire l'état de l'inhibition de la suppression de la synchro de la ligne 32 (inversion de la logique)
  bool inhiberSynchro = !digitalRead(INHIBIT_PIN);

  // Afficher les valeurs lues sur le Serial Monitor
  Serial.print("Valeur ADC : ");
  Serial.print(valeurADC);
  Serial.print(" | Inhiber Synchro : ");
  Serial.println(inhiberSynchro ? "OUI" : "NON");

  // Détecter le mode sélectionné
  int mode = detecterMode(valeurADC);
  Serial.print("Mode détecté : ");
  Serial.println(mode);

  // Générer la trame vidéo en fonction du mode sélectionné
  for (int ligne = 1; ligne <= NB_LIGNES; ligne++) {
    int dureeSynchro = (inhiberSynchro && ligne == 32) ? 0 : DUREE_SYNCHRO;

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
  if (valeurADC >= SEUIL_MODE_1_MIN && valeurADC <= SEUIL_MODE_1_MAX) return 1;
  if (valeurADC >= SEUIL_MODE_2_MIN && valeurADC <= SEUIL_MODE_2_MAX) return 2;
  if (valeurADC >= SEUIL_MODE_3_MIN && valeurADC <= SEUIL_MODE_3_MAX) return 3;
  if (valeurADC >= SEUIL_MODE_4_MIN && valeurADC <= SEUIL_MODE_4_MAX) return 4;
  return 1;  // Par défaut, mode 1
}

// Fonction pour générer la trame vidéo en fonction du mode sélectionné
int genererTrame(int mode, int ligne) {
  for (int t = 0; t < PERIODE_LIGNE; t++) { // Simulation de la ligne
    switch (mode) {
      case 1: // Trame originale : Noir, Gris, Blanc
        if (ligne <= 8) return VIDEO_GRIS;
        if (ligne <= 16) return VIDEO_NOIR;
        if (ligne <= 24) return VIDEO_BLANC;
        return VIDEO_NOIR;

      case 2: // Mode 2 : Bandes alternées ajustées
        if (ligne % 4 == 0 || ligne % 4 == 1) return VIDEO_NOIR;
        else return VIDEO_BLANC;
      
      case 3: // Mode 3 : Bandes noires et blanches alternées
        return (ligne % 2 == 0) ? VIDEO_NOIR : VIDEO_BLANC;

      case 4: // Mode 4 : Bandes alternées mais avec ligne % 3
        if (ligne % 3 == 0 || ligne % 3 == 1) {
          return VIDEO_NOIR;  // Noires pour les lignes 0, 1, 3, 4, ...
        } else {
          return VIDEO_BLANC; // Blanches pour les lignes 2, 5, ...
        }

    }
  }
  return VIDEO_NOIR; // Sécurité si le mode est invalide
}
