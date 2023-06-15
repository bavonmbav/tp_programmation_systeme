
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define BUFFER_SIZE 100

// Les arguments d'un thread
typedef struct {
    char **args;
    int num_files;
    int begin;
    int argc;
} thread_args;

// Copie d'un seul fichier
void __copy(FILE* s, FILE* d)
{
    char buffer[BUFFER_SIZE];
    size_t nwrite, nread;
    while ((nread = fread(buffer, 1, sizeof(buffer), s)) > 0)
    {
        nwrite = fwrite(buffer, 1, nread, d);
        if(nread != nwrite)
        {
            printf("[TRHEAD : %lu] lecture et ecriture differer\n", pthread_self());
        }
    }
}

// Fonction permettant aux threads de les copiers.
void* copy_file(void *arg)
{
    thread_args *args = (thread_args*) arg;
    FILE *src = NULL,
         *dst = NULL;
    // Debut de copie du fichier
    int i = args->begin;
    // Tant qu'ont a pas encore fini a copier les fichiers, on itere.
    while(args->num_files)
    {
        if(args->argc <= i)
            pthread_exit(NULL);
        src = fopen(args->args[i], "rb");
        if (src == NULL)
        {
            printf("-------%d------\n", i);
            printf("[TRHEAD : %lu] Echec d'ouverture du fichier '%s'\n", pthread_self(), args->args[i]);
        }

        char dst_filename[256];
        sprintf(dst_filename, "%s.COPIE", args->args[i]);
        // Ouvre le fichier de destination
        dst = fopen(dst_filename, "wb");
        if (dst == NULL)
        {
            printf("[TRHEAD : %lu] Echec de creation du fichier '%s'\n", pthread_self(), dst_filename);
            if(src != NULL)
            {
                fclose(src);
            }
        }

        // Si l'ouverture s'est bien fait on copie
        if(src != NULL && dst != NULL)
        {
            __copy(src, dst);
            printf("[TRHEAD : %lu] Fichier '%s' copier\n", pthread_self(), args->args[i]);
            fclose(src);
            fclose(dst);
        }
        i++;
        args->num_files--;
    }
    // Libere l'espace occuper par l'argument
    free(args);
    // Quitte le thhread
    pthread_exit(NULL);
}


int main(int argc, char **argv)
{
    pthread_t *threads;
    if (argc < 2)
    {
        printf("Usage:\n\t'copier' <nombre thread> <fichier 1> <fichier 2> ... <fichier N>\n");
        exit(1);
    }

    int beg               = 2 , i,
        num_files         = argc - 2,
        num_threads       = atoi(argv[1]),
        num_files_thread  = num_files / num_threads,
        num_other_file    = num_files / num_threads;

    threads = (pthread_t*) malloc(sizeof(pthread_t) * num_threads);
    for (i = 0; i < num_threads; i += 1)
    {
        if (i == (num_threads - 1))
        {
            num_files_thread += num_other_file;
        }

        thread_args *arg_t = (thread_args*) malloc(sizeof(thread_args) * num_files);

        arg_t->args = argv;
        arg_t->begin = beg;
        arg_t->argc = argc;
        if(num_files_thread == 0)
            arg_t->num_files = num_files;
        else arg_t->num_files = num_files_thread;

        pthread_create(&threads[i], NULL, copy_file, arg_t);
        beg += num_files_thread;
        // Si le nombre des fichiers et inferieur au nombre des threads
        if(num_files_thread == 0)
            break;
    }

    // Attend l'execution des threads
    for (i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
        // Si le nombre des fichiers et inferieur au nombre des threads
        if(num_files_thread == 0)
            break;
    }
    // Libere la memoire
    for (i = 0; i < num_threads; i++)
    {
        free(threads[i]);
    }

    return 0;
}
