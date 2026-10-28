*This project has been created as part of the 42 curriculum by jquer.*

# Libft

## Description

**Libft** est le premier projet du cursus 42 : la création notre propre bibliothèque C (`libft.a`) qui reprend des fonctions de la libc, ajoute des fonctions utilitaires, et sert de base à tous nos futurs projets.

Le but est de comprendre le fonctionnement de ces fonctions (gestion mémoire, chaînes de caractères, pointeurs, listes chaînées) en les recodant sans utiliser les originales.

## Instructions

### Compilation

```bash
make          # compile la bibliothèque libft.a
make clean    # supprime les fichiers objets (.o)
make fclean   # supprime les .o et libft.a
make re       # fclean puis recompile
```

### Utilisation dans un projet

1. Inclure le header dans le code :

```c
#include "libft.h"
```

2. Compiler en liant la bibliothèque :

```bash
cc -Wall -Wextra -Werror main.c -L. -lft -o programme
```

### Exemple

```c
#include "libft.h"

int main(void)
{
    t_list *liste = NULL;

    ft_lstadd_front(&liste, ft_lstnew(ft_strdup("B")));
    ft_lstadd_front(&liste, ft_lstnew(ft_strdup("A")));
    // liste → [A] → [B] → NULL
    return (0);
}
```

## Contenu

### Partie 1 : fonctions de la libc

| Catégorie | Fonctions |
|---|---|
| Caractères | `ft_isalpha` `ft_isdigit` `ft_isalnum` `ft_isascii` `ft_isprint` `ft_toupper` `ft_tolower` |
| Chaînes | `ft_strlen` `ft_strlcpy` `ft_strlcat` `ft_strchr` `ft_strrchr` `ft_strncmp` `ft_strnstr` `ft_strdup` `ft_atoi` |
| Mémoire | `ft_memset` `ft_bzero` `ft_memcpy` `ft_memmove` `ft_memchr` `ft_memcmp` `ft_calloc` |

### Partie 2 : fonctions additionnelles

| Fonction | Description |
|---|---|
| `ft_substr` | Extrait une sous-chaîne |
| `ft_strjoin` | Concatène deux chaînes dans une nouvelle |
| `ft_strtrim` | Retire un ensemble de caractères au début et à la fin |
| `ft_split` | Découpe une chaîne selon un séparateur |
| `ft_itoa` | Convertit un entier en chaîne |
| `ft_strmapi` | Applique une fonction à chaque caractère (nouvelle chaîne) |
| `ft_striteri` | Applique une fonction à chaque caractère |
| `ft_putchar_fd` `ft_putstr_fd` `ft_putendl_fd` `ft_putnbr_fd` | Écriture sur un descripteur de fichier |

### Partie 3 : listes chaînées

La structure utilisée :

```c
typedef struct s_list
{
    void            *content;
    struct s_list   *next;
}   t_list;
```

| Fonction | Description |
|---|---|
| `ft_lstnew` | Crée un nouveau maillon |
| `ft_lstadd_front` | Ajoute un maillon au début |
| `ft_lstadd_back` | Ajoute un maillon à la fin |
| `ft_lstsize` | Compte les maillons |
| `ft_lstlast` | Renvoie le dernier maillon |
| `ft_lstdelone` | Supprime un maillon |
| `ft_lstclear` | Supprime et libère toute la liste |
| `ft_lstiter` | Applique une fonction à chaque maillon |
| `ft_lstmap` | Crée une nouvelle liste à partir d'une fonction appliquée à chaque maillon |

## Ressources

- `man` des fonctions de la libc (`man 3 strlcpy`, etc.)
- [Documentation C de cppreference](https://en.cppreference.com/w/c)
- [Tutoriels C de geeksforgeeks](https://www.geeksforgeeks.org/search/?gq=c)
- Norme 42 (Norminette)

### Utilisation de l'IA

L'IA a été utilisée pour comprendre certains concepts et pour aider a mettre en forme ce README. Le code a été écrit et testé personnellement.