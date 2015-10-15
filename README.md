*Thibaut Castanié, M2 IMAGINA*

# Compte rendu TP3 Moteur jeux

### Client/serveur

Une classe serveur est créé comprenant l'horloge qui règle le cycle des saisons des clients. Le serveur doit envoyer des message à chacun des clients. Pour cela, on utilise des *slots* qui envoient les message via un *socket*. 

La classe client permet de récupérer les messages envoyés via le *socket*. Lorsque celui-ci est reçu, un slot est exécuté afin d'indiquer à la fenêtre les nouvelles instructions (le changement de saison).

### Saisons

Afin de représenter les saisons sur chacune des fenêtres clientes, il est judicieux de changer les couleurs des polygones, ainsi que d'ajouter des effets de lumière et de particules. Le calcul est effectué côté client, c'est à dire que c'est lui qui possède les informations à changer en fonction de l'argument qu'il reçoit du serveur. Ceci sera implémentée dans une prochaine version, incessamment sous peu.

### Particules

**Qt** possède un système de particules via son module **Qt Quick** (`QtQuick.Particles 2.0`), possédant toutes les méthodes nécessaires à l'utilisation et l'interaction des particules. La pluie et la neige peuvent donc être générée via ce système. La puit serait représentée par des petits droits tombant de façon verticale, tandis  que la neige serait composée de petits disques blancs tombant lentement.

### Bonus

Pour accumuler les particules, il faudrait développer un système permettant de détecter le moment où la particule atteint le "sol" (quand son axe z = l'axe z du point du sol), et si un point voisin possède un z inférieur, non occupé par une particule, la faire se déplacer sur celui-ci.