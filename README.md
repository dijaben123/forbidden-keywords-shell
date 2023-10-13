# L3Miage Shell

Le L3Miage Shell est un interpréteur de commandes simple basé sur C. Il offre des fonctionnalités de base pour la saisie, l'analyse et l'exécution de commandes système. En outre, il permet de gérer une variable d'environnement spéciale appelée FORBIDDEN, qui peut être utilisée pour interdire l'exécution de certaines commandes.

## Fonctionnalités

- Saisie de commandes utilisateur.
- Analyse des commandes en mots-clés (tokens).
- Exécution des commandes système.
- Gestion de la variable d'environnement FORBIDDEN pour interdire certaines commandes.

## Compilation

Pour compiler le L3Miage Shell, vous pouvez utiliser la commande suivante :

```shell
gcc l3miage_shell.c -o l3miage_shell
Assurez-vous que vous avez un compilateur GCC installé sur votre système:
`-Utilisation :`
    Exécutez le L3Miage Shell en utilisant la commande ./l3miage_shell.
    ```schell 

    Vous pouvez saisir des commandes système comme vous le feriez avec un terminal normal.

    Le shell prend en charge les commandes spéciales newf et rmf pour gérer la variable FORBIDDEN. Exemple :
        newf <keyword> : Ajoute un mot-clé à la variable FORBIDDEN.
        rmf <keyword> : Supprime un mot-clé de la variable FORBIDDEN.

    Le shell gère également la commande exit pour quitter le programme.

-Gestion de la variable FORBIDDEN

La variable d environnement FORBIDDEN est utilisée pour interdire l exécution de certaines commandes système. Vous pouvez ajouter des mots-clés à cette variable avec newf et les supprimer avec rmf.

Exemple d utilisation :
newf rm
Cette commande ajoutera le mot-clé "rm" à la variable FORBIDDEN. Toute tentative d exécution de la commande "rm" entraînera un avertissement.
Remarques:

Ce projet est destiné à des fins éducatives et est un interpréteur de commandes minimaliste. Il ne possède pas toutes les fonctionnalités d un shell complet comme Bash. Vous êtes invités à l améliorer et à l adapter selon vos besoins.

Assurez-vous d inclure les détails spécifiques de votre projet, tels que le nom du fichier source (l3miage_shell.c) et le nom de l exécutable (l3miage_shell), ainsi que tout autre détail ou instruction pertinent pour les utilisateurs.

N hésitez pas à personnaliser ce README en fonction de vos besoins spécifiques et des détails de votre projet.

Cela devrait inclure la demande de saisir "exit" pour quitter le shell au lieu d'utiliser Ctrl+C.