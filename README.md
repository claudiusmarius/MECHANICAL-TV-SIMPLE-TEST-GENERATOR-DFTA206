# MECHANICAL-TV-SIMPLE-TEST-GENERATOR-DFTA206
Petite mire (générateur de signal vidéo simple et fixe) pour la mise au point de votre TV à balayage mécanique

Ce dispositif permet également de fournir un signal unumérique de 0,6V pouvant être utilisé pour tester la partie son.

Pour l'instant sa fréquence est de 1KHz peut elle peut bien entendu être modifiée par le code.

Nous pouvons inhiber l'atténuation sonore par la position du cavalier sur J8.

Le Top de synchro du signal vidéo composite de la 32ème ligne peut ou non être omis par la position du cavalier sur J1.

Le Seeeduino XIAO fournit sur son port DAC (A0) le signal vidéo.

Il fournit également sur sur son port D1 le signal numérique de synchro. Ce signal a besoin d'être atténué ensuite par le pont diviseur R7 et R9 avant d'être additionné au signal vidéo pur, par le sommateur U2A.

La résistance RV2 permet d'ajuster au mieux au format NBTV le signal de vidéo composite en sortie U2A.

Le deuxième des 4 AOP constituant le LM324 U2B est mis à profit pour être utilisé comme montage suiveur. Le signal vidéo composite est accessible que ce soit par la cosse poignard TP3, le bornier à vis J3 ou la prise CINCH J2.

Le troisième AOP U2C est utilisé comme montage suiveur pour le signal sonal sonaore atténué (ou pas) et le quatrième AOP U2D puis qu'il est là, est utilisé pour faire suivre le signal sonnore ajusté par RV1.

Le commutateur rotatif à 4 positions permet de disposer de 4 images différentes le principe du choix est de faire varier la tension sur D3 par la combinaison de résistances R1 à R5. (R5 est superflue).

Enfin on peut remarquer l'ajout des pads sur RV1, RV2 et SW1. Ils ont pour rôle de pouvoir remplacer ces 3 composants de PCB par des éléments déportés selon le choix du "maker".
