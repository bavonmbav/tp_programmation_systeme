#include <stdio.h>
#include <windows.h>
#include <time.h>


int main(int argc, LPTSTR argv[])
{
    if (argc != 3) {
        printf("Utilisation : programme.exe fichier_source fichier_destination\n");
        return 1;
    }

    // Calculer le temps de copie
    clock_t start = clock();

    // Copier le contenu du fichier source vers le fichier destination
    if (!CopyFile(argv[1], argv[2], FALSE)) {
        printf("Erreur lors de la copie du fichier. %x\n", GetLastError());
        return 4;
    }

    // Calculer le temps écoulé
    clock_t end = clock();
    double calculTemp = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Copie du fichier terminée avec succès.\n");
    printf("Temps de copie : %.2f secondes\n", calculTemp);
    return 0;
}