# MECHANICAL-TV-SIMPLE-TEST-GENERATOR-DFTA206
Petite mire (générateur de signal vidéo simple et fixe) pour la mise au point de votre TV à balayage mécanique

Ce dispositif permet également de fournir un signal unumérique de 0,6V pouvant être utilisé pour tester la partie son.

Pour l'instant sa fréquence est de 1KHz peut elle peut bien entendu être modifiée par le code.

Nous pouvons inhiber l'atténuation sonore par la position du cavalier sur J8.

Le Top de synchro du signal vidéo composite de la 32ème ligne peut ou non être omis par la position du cavalier sur J1.

Le Seeeduino XIAO fournit sur son port DAC (A0) le signal vidéo.

Il fournit également sur sur son port D1 le signal numérique de synchro. Ce signal a besoin d'être atténué en suite par le pont diviseur R7 et R9 avant d'être additionné par le sommateur U2A.

La résistance RV2 permet d'ajuster au mieux au format NBTV le signal de vidéo composite en sortie U2A.

Le deuxième des 4 AOP constituant le LM324 U2B est mis à profit pour être utilisé comme montage suiveur. Le signal vidéo composite est accessible que ce soit par 
