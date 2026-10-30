/* ************************************************************************** */
/*  main_test.c - tests pour les fonctions ft_* de la libft (Linux / glibc)   */
/*                                                                            */
/*  Compilation :                                                             */
/*    cc -Wall -Wextra -Werror main_test.c ft_*.c -o test && ./test           */
/*                                                                            */
/*  Avec ta libft.a (ATTENTION : "cc main_test.c libft.a" ne marche pas,      */
/*  toutes les fonctions seraient vues comme "pas encore ecrites") :          */
/*    cc main_test.c -Wl,--whole-archive libft.a -Wl,--no-whole-archive \    */
/*       -o test && ./test                                                    */
/*                                                                            */
/*  - Les fonctions pas encore ecrites sont ignorees.                         */
/*  - Chaque fonction est testee dans un processus a part : un segfault ou    */
/*    une boucle infinie est signale, et les tests suivants continuent.       */
/*  - malloc/free sont surveilles : fuites, taille allouee, echec de malloc.  */
/*  - Couleurs desactivables avec : NO_COLOR=1 ./test                         */
/* ************************************************************************** */

/* Active MAP_ANONYMOUS, strsignal... meme avec -std=c99 / c11 */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdint.h>
#include <limits.h>
#include <stdarg.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>

/* Prototypes declares "weak" : une fonction pas encore ecrite a une adresse
   NULL au lieu de provoquer une erreur de link. */
#define W __attribute__((weak))

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

W int		ft_isalpha(int c);
W int		ft_isdigit(int c);
W int		ft_isalnum(int c);
W int		ft_isascii(int c);
W int		ft_isprint(int c);
W size_t	ft_strlen(const char *s);
W void	*ft_memset(void *s, int c, size_t n);
W void	ft_bzero(void *s, size_t n);
W void	*ft_memcpy(void *dest, const void *src, size_t n);
W void	*ft_memmove(void *dest, const void *src, size_t n);
W size_t	ft_strlcpy(char *dst, const char *src, size_t size);
W size_t	ft_strlcat(char *dst, const char *src, size_t size);
W int		ft_toupper(int c);
W int		ft_tolower(int c);
W char	*ft_strchr(const char *s, int c);
W char	*ft_strrchr(const char *s, int c);
W int		ft_strncmp(const char *s1, const char *s2, size_t n);
W void	*ft_memchr(const void *s, int c, size_t n);
W int		ft_memcmp(const void *s1, const void *s2, size_t n);
W char	*ft_strnstr(const char *big, const char *little, size_t len);
W int		ft_atoi(const char *nptr);
W void	*ft_calloc(size_t nmemb, size_t size);
W char	*ft_strdup(const char *s);
W char	*ft_substr(char const *s, unsigned int start, size_t len);
W char	*ft_strjoin(char const *s1, char const *s2);
W char	*ft_strtrim(char const *s1, char const *set);
W char	**ft_split(char const *s, char c);
W char	*ft_itoa(int n);
W char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
W void	ft_striteri(char *s, void (*f)(unsigned int, char *));
W void	ft_putchar_fd(char c, int fd);
W void	ft_putstr_fd(char *s, int fd);
W void	ft_putendl_fd(char *s, int fd);
W void	ft_putnbr_fd(int n, int fd);
W t_list	*ft_lstnew(void *content);
W void	ft_lstadd_front(t_list **lst, t_list *new);
W int		ft_lstsize(t_list *lst);
W t_list	*ft_lstlast(t_list *lst);
W void	ft_lstadd_back(t_list **lst, t_list *new);
W void	ft_lstdelone(t_list *lst, void (*del)(void *));
W void	ft_lstclear(t_list **lst, void (*del)(void *));
W void	ft_lstiter(t_list *lst, void (*f)(void *));
W t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

/* ========================================================================= */
/*  Surveillance de malloc / free                                            */
/* ========================================================================= */

extern void	*__libc_malloc(size_t n);
extern void	*__libc_calloc(size_t n, size_t m);
extern void	*__libc_realloc(void *p, size_t n);
extern void	__libc_free(void *p);

#define MAX_TRACK 4096

static int		g_track = 0;      /* 1 pendant un appel a une fonction ft_ */
static int		g_fail = 0;       /* k > 0 : le k-ieme prochain malloc renvoie NULL */
static int		g_nalloc = 0;     /* nombre de malloc faits par les ft_ */
static size_t	g_last_size = 0;  /* taille du dernier malloc fait par une ft_ */
static int		g_live = 0;       /* blocs alloues par les ft_ pas encore liberes */
static void		*g_ptrs[MAX_TRACK];
static size_t	g_sizes[MAX_TRACK];
static char		g_canary[MAX_TRACK];   /* 1 : 16 octets temoins apres le bloc */

#define CANARY 16

static int		g_ncalloc = 0;     /* appels a calloc faits par les ft_ */
static int		g_overflow = 0;    /* blocs dont les octets temoins ont ete ecrases */
static size_t	g_ovf_size = 0;
static size_t	g_ovf_bytes = 0;
static char		g_ovf_case[600];
static char		*g_cur_case = NULL;  /* le test en cours (pointe dans g->current) */

#define MAX_NODES 2048
#define MAX_QUAR 4096

static int		g_double = 0;     /* free sur un bloc deja libere */
static int		g_bad_free = 0;   /* free sur un content de test (au lieu de del) */
static int		g_vals[64];       /* contents des listes de test */
static void		*g_nodes[MAX_NODES];      /* noeuds crees par le testeur */
static char		g_node_freed[MAX_NODES];
static int		g_nnodes = 0;
static void		*g_quar[MAX_QUAR];        /* blocs liberes gardes empoisonnes */
static int		g_nquar = 0;

static void	track_add(void *p, size_t n, int canary)
{
	for (int i = 0; i < MAX_TRACK; i++)
		if (!g_ptrs[i])
		{
			g_ptrs[i] = p;
			g_sizes[i] = n;
			g_canary[i] = (char)canary;
			g_live++;
			return ;
		}
}

/* verifie les 16 octets temoins places juste apres le bloc i */
static void	check_canary(int i)
{
	unsigned char	*c = (unsigned char *)g_ptrs[i] + g_sizes[i];
	size_t			bad = 0;

	if (!g_canary[i])
		return ;
	for (size_t k = 0; k < CANARY; k++)
		if (c[k] != 0xCD)
			bad = k + 1;
	if (bad && g_overflow++ == 0)
	{
		g_ovf_size = g_sizes[i];
		g_ovf_bytes = bad;
		snprintf(g_ovf_case, sizeof(g_ovf_case), "%s", g_cur_case ? g_cur_case : "");
	}
	memset(c, 0xCD, CANARY);
}

static int	track_del(void *p)
{
	if (g_live == 0)
		return (0);
	for (int i = 0; i < MAX_TRACK; i++)
		if (g_ptrs[i] == p)
		{
			check_canary(i);
			g_ptrs[i] = NULL;
			g_live--;
			return (1);
		}
	return (0);
}

static void	check_all_canaries(void)
{
	for (int i = 0; i < MAX_TRACK; i++)
		if (g_ptrs[i])
			check_canary(i);
}

/* taille demandee a malloc pour le bloc p (ou (size_t)-1 si inconnu) */
static size_t	block_size(const void *p)
{
	for (int i = 0; i < MAX_TRACK; i++)
		if (p && g_ptrs[i] == p)
			return (g_sizes[i]);
	return ((size_t)-1);
}

/* 1 si ce malloc doit echouer (simulation d'echec) */
static int	should_fail(void)
{
	if (g_fail > 0 && --g_fail == 0)
		return (1);
	return (0);
}

void	*malloc(size_t n)
{
	void	*p;

	if (!g_track)
		return (__libc_malloc(n));
	g_nalloc++;
	g_last_size = n;
	if (should_fail())
		return (NULL);
	if (n > SIZE_MAX - CANARY)
		return (NULL);
	p = __libc_malloc(n + CANARY);
	if (p)
	{
		memset(p, 0xBE, n);   /* comme ca, un champ oublie n'est pas 0 par chance */
		memset((char *)p + n, 0xCD, CANARY);   /* octets temoins : detectent un debordement */
		track_add(p, n, 1);
	}
	return (p);
}

void	*calloc(size_t n, size_t m)
{
	void	*p;

	if (!g_track)
		return (__libc_calloc(n, m));
	g_nalloc++;
	g_ncalloc++;
	g_last_size = n * m;
	if (should_fail())
		return (NULL);
	p = __libc_calloc(n, m);
	if (p)
		track_add(p, n * m, 0);
	return (p);
}

void	*realloc(void *p, size_t n)
{
	int		tracked = (p && track_del(p));
	void	*r = __libc_realloc(p, n);

	if (r && (tracked || g_track))
		track_add(r, n, 0);
	return (r);
}

static int	node_index(const void *p)
{
	for (int i = 0; i < g_nnodes; i++)
		if (g_nodes[i] == p)
			return (i);
	return (-1);
}

static int	in_quarantine(const void *p)
{
	for (int i = 0; i < g_nquar; i++)
		if (g_quar[i] == p)
			return (1);
	return (0);
}

/* Un bloc libere par une fonction ft_ n'est pas vraiment rendu au systeme :
   il est rempli de 0xAB. Relire node->next apres free(node) donne alors
   0xABABABAB... et fait planter au lieu de marcher par chance. */
void	free(void *p)
{
	int		k;
	size_t	size;

	if (!p)
		return ;
	if ((char *)p >= (char *)g_vals && (char *)p < (char *)(g_vals + 64))
	{
		g_bad_free++;
		return ;
	}
	k = node_index(p);
	if (k >= 0)
	{
		if (g_node_freed[k])
			g_double++;
		g_node_freed[k] = 1;
		memset(p, 0xAB, 2 * sizeof(void *));
		return ;
	}
	if (in_quarantine(p))
	{
		g_double++;
		return ;
	}
	size = block_size(p);
	if (g_track && size != (size_t)-1 && g_nquar < MAX_QUAR && track_del(p))
	{
		memset(p, 0xAB, size);
		g_quar[g_nquar++] = p;
		return ;
	}
	track_del(p);
	__libc_free(p);
}

/* Appel d'une fonction ft_ avec surveillance de malloc */
#define FT(expr) __extension__ ({ g_track = 1; __typeof__(expr) r_ = (expr); g_track = 0; r_; })
#define FTV(expr) do { g_track = 1; expr; g_track = 0; } while (0)

/* ========================================================================= */
/*  Affichage                                                                */
/* ========================================================================= */

#define MAX_SHOWN 5   /* nombre max de KO affiches par fonction */
#define TIMEOUT 5     /* secondes avant de considerer une boucle infinie */

/* Partage entre le processus principal et le processus de test */
static struct s_shared
{
	int		ok;
	int		ko;
	int		fn_ko;
	char	current[600];   /* le test en cours */
}	*g;
static int	g_skip = 0;

static const char	*GREEN = "";
static const char	*RED = "";
static const char	*GREY = "";
static const char	*BOLD = "";
static const char	*RESET = "";

static void	init_colors(void)
{
	if (isatty(STDOUT_FILENO) && !getenv("NO_COLOR"))
	{
		GREEN = "\033[32m";
		RED = "\033[31m";
		GREY = "\033[90m";
		BOLD = "\033[1m";
		RESET = "\033[0m";
	}
}

/* printf dans un buffer tournant (pour construire les messages) */
static char	*f(const char *fmt, ...)
{
	static char	buf[16][600];
	static int	i = 0;
	char		*b = buf[i++ % 16];
	va_list		ap;

	va_start(ap, fmt);
	vsnprintf(b, 600, fmt, ap);
	va_end(ap);
	return (b);
}

/* n octets entre guillemets, caracteres speciaux echappes (\0, \n, \310...) */
static char	*mem(const void *p, size_t n)
{
	const unsigned char	*s = p;
	char				tmp[500];
	size_t				j = 0;

	if (!p)
		return ("NULL");
	for (size_t i = 0; i < n; i++)
	{
		if (j > 120)
		{
			j += sprintf(tmp + j, "...(%zu octets)", n);
			break ;
		}
		if (s[i] == '\0')
			j += sprintf(tmp + j, "\\0");
		else if (s[i] == '\n')
			j += sprintf(tmp + j, "\\n");
		else if (s[i] == '\t')
			j += sprintf(tmp + j, "\\t");
		else if (s[i] == '"' || s[i] == '\\')
			j += sprintf(tmp + j, "\\%c", s[i]);
		else if (s[i] < 32 || s[i] >= 127)
			j += sprintf(tmp + j, "\\%03o", s[i]);
		else
			tmp[j++] = s[i];
	}
	tmp[j] = '\0';
	return (f("\"%s\"", tmp));
}

static char	*q(const char *s)
{
	if (!s)
		return ("NULL");
	return (mem(s, strlen(s)));
}

/* un caractere : 'a' s'il est affichable, sinon sa valeur */
static char	*chr(int c)
{
	if (c == 0)
		return ("'\\0'");
	if (c == '\n')
		return ("'\\n'");
	if (c >= 33 && c <= 126 && c != '\'' && c != '\\')
		return (f("'%c'", c));
	return (f("%d", c));
}

/* un pointeur vu par rapport au debut de la chaine : "s + 4" ou "NULL" */
static char	*ptr(const char *name, const void *base, const void *p)
{
	if (!p)
		return ("NULL");
	if (p == base)
		return (f("%s (le debut)", name));
	return (f("%s + %td", name, (const char *)p - (const char *)base));
}

static const char	*sign_name(int n)
{
	if (n < 0)
		return ("negatif");
	if (n > 0)
		return ("positif");
	return ("0");
}

static void	ko(const char *expected, const char *got, const char *hint)
{
	g->ko++;
	g->fn_ko++;
	if (g->fn_ko == 1)
		printf("\n");
	if (g->fn_ko > MAX_SHOWN)
		return ;
	printf("  %sKO%s  %s\n", RED, RESET, g->current);
	printf("      attendu : %s\n", expected);
	printf("      obtenu  : %s\n", got);
	if (hint && *hint)
		printf("      %s-> %s%s\n", GREY, hint, RESET);
}

/* Decrit le test qui va etre lance (affiche en cas d'erreur ou de crash) */
#define CASE(...) snprintf(g->current, sizeof(g->current), __VA_ARGS__)

/* Les messages ne sont construits qu'en cas d'echec */
#define TEST(cond, expected, got, hint) do { \
	if (cond) g->ok++; \
	else ko(expected, got, hint); \
} while (0)

/* Lance une serie de tests dans un processus a part */
static void	run(const char *name, int present, void (*fn)(void))
{
	pid_t	pid;
	int		st;

	if (!present)
	{
		printf("%s%-13s%s %s-- pas encore ecrite%s\n", BOLD, name, RESET, GREY, RESET);
		g_skip++;
		return ;
	}
	g->fn_ko = 0;
	g->current[0] = '\0';
	printf("%s%-13s%s ", BOLD, name, RESET);
	pid = fork();
	if (pid == 0)
	{
		alarm(TIMEOUT);
		fn();
		check_all_canaries();
		if (g_overflow > 0)
		{
			snprintf(g->current, sizeof(g->current), "%s", g_ovf_case);
			ko("rien d'ecrit apres la fin de la zone allouee",
				f("%zu octet(s) ecrit(s) apres la fin d'un malloc(%zu)%s", g_ovf_bytes, g_ovf_size,
					g_overflow > 1 ? f(" (et %d autre(s) bloc(s) deborde(s))", g_overflow - 1) : ""),
				"debordement : souvent le '\\0' ecrit une case trop loin, ou un malloc trop petit");
		}
		if (g_live > 0)
		{
			CASE("toute la serie de tests de ft_%s", name);
			ko("0 bloc non libere", f("%d bloc(s) alloue(s) jamais libere(s)", g_live),
				"fuite memoire : chaque malloc fait dans la fonction doit etre libere");
		}
		if (g_ncalloc > 0)
		{
			CASE("toute la serie de tests de ft_%s", name);
			ko("seulement malloc", f("calloc appele %d fois", g_ncalloc),
				"calloc n'est pas autorise : malloc, puis mise a 0 (ft_bzero), ou ton ft_calloc");
		}
		if (g_double > 0)
		{
			CASE("toute la serie de tests de ft_%s", name);
			ko("aucun double free", f("%d free sur un bloc deja libere", g_double),
				"un meme pointeur ne doit etre libere qu'une seule fois");
		}
		_exit(0);
	}
	if (pid < 0)
		fn();
	else if (waitpid(pid, &st, 0) > 0 && WIFSIGNALED(st))
	{
		int	sig = WTERMSIG(st);

		g->ko++;
		if (g->fn_ko++ == 0)
			printf("\n");
		printf("  %sCRASH%s  %s\n", RED, RESET, g->current);
		if (sig == SIGALRM)
			printf("      -> bloque plus de %d s : boucle infinie ?\n", TIMEOUT);
		else
			printf("      -> %s (signal %d)%s\n", strsignal(sig), sig,
				sig == SIGSEGV ? " : acces memoire invalide (NULL, hors du buffer, lecture apres free...)" : "");
		printf("      %sles tests suivants de cette fonction n'ont pas ete lances%s\n",
			GREY, RESET);
	}
	if (g->fn_ko == 0)
		printf("%sOK%s\n", GREEN, RESET);
	else
	{
		if (g->fn_ko > MAX_SHOWN)
			printf("  ... et %d autre(s) KO non affiche(s)\n", g->fn_ko - MAX_SHOWN);
		printf("  %s%d erreur(s)%s\n\n", RED, g->fn_ko, RESET);
	}
}

/* ========================================================================= */
/*  References pour les fonctions absentes de la libc Linux                  */
/*  (meme comportement que l'implementation BSD d'origine)                   */
/* ========================================================================= */

static size_t	ref_strlcpy(char *d, const char *s, size_t size)
{
	size_t	len = strlen(s);
	size_t	n;

	if (size)
	{
		n = (len < size - 1) ? len : size - 1;
		memcpy(d, s, n);
		d[n] = '\0';
	}
	return (len);
}

static size_t	ref_strlcat(char *d, const char *s, size_t size)
{
	size_t	dl = 0;

	while (dl < size && d[dl])
		dl++;
	if (dl == size)
		return (size + strlen(s));
	return (dl + ref_strlcpy(d + dl, s, size - dl));
}

static char	*ref_strnstr(const char *big, const char *little, size_t len)
{
	size_t	n = strlen(little);

	if (n == 0)
		return ((char *)big);
	for (size_t i = 0; big[i] && i < len && n <= len - i; i++)
		if (strncmp(big + i, little, n) == 0)
			return ((char *)big + i);
	return (NULL);
}

static int	ref_isascii(int c)
{
	return (c >= 0 && c <= 127);
}

/* premier octet different entre a et b, ou n si identiques */
static size_t	diff_at(const void *a, const void *b, size_t n)
{
	for (size_t i = 0; i < n; i++)
		if (((const unsigned char *)a)[i] != ((const unsigned char *)b)[i])
			return (i);
	return (n);
}

/* Renvoie n octets places juste avant une page memoire interdite :
   toute lecture au-dela de ces n octets provoque un segfault. */
static char	*guard_slot(int slot, const char *content, size_t n)
{
	static char	*pages[2] = {NULL, NULL};
	long		ps = sysconf(_SC_PAGESIZE);

	if (!pages[slot])
	{
		pages[slot] = mmap(NULL, ps * 2, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		mprotect(pages[slot] + ps, ps, PROT_NONE);
	}
	memcpy(pages[slot] + ps - n, content, n);
	return (pages[slot] + ps - n);
}

static char	*guard(const char *content, size_t n)
{
	return (guard_slot(0, content, n));
}

#define NO_OVERREAD "si ca plante ici : ta fonction lit au-dela de la limite donnee"

/* ========================================================================= */
/*  is* et to* : tous les caracteres de -1 (EOF) a 255                       */
/* ========================================================================= */

static const char	*g_name;
static int			(*g_ft)(int);
static int			(*g_ref)(int);

static void	test_is(void)
{
	for (int c = -1; c <= 255; c++)
	{
		CASE("ft_%s(%s)", g_name, chr(c));
		int	r = FT(g_ft(c));
		int	e = (c == -1) ? 0 : !!g_ref(c);

		TEST(r == e, f("%d", e), f("%d", r),
			(r != 0 && r != 1) ? "doit renvoyer exactement 1 (pas juste une valeur non nulle)"
			: c >= 128 ? "les caracteres de 128 a 255 ne sont pas ASCII : doit renvoyer 0" : "");
	}
}

static void	test_to(void)
{
	for (int c = -1; c <= 255; c++)
	{
		CASE("ft_%s(%s)", g_name, chr(c));
		int	r = FT(g_ft(c));
		int	e = g_ref(c);

		TEST(r == e, chr(e), chr(r),
			c >= 128 || c < 0 ? "hors de 'a'-'z' / 'A'-'Z', c doit etre renvoye tel quel" : "");
	}
}

static void	run_is(const char *name, int (*ft)(int), int (*ref)(int))
{
	g_name = name;
	g_ft = ft;
	g_ref = ref;
	run(name, ft != NULL, test_is);
}

static void	run_to(const char *name, int (*ft)(int), int (*ref)(int))
{
	g_name = name;
	g_ft = ft;
	g_ref = ref;
	run(name, ft != NULL, test_to);
}

/* ========================================================================= */
/*  strlen                                                                   */
/* ========================================================================= */

static void	test_strlen(void)
{
	static char	big[10001];
	const char	*t[] = {"", "a", "hello", "hello world\n", "\t\n\v", "\200\310\377", big};

	memset(big, 'a', 10000);
	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		CASE("ft_strlen(%s)", t[i] == big ? "chaine de 10000 'a'" : q(t[i]));
		size_t	r = FT(ft_strlen(t[i]));
		size_t	e = strlen(t[i]);

		TEST(r == e, f("%zu", e), f("%zu", r), "");
	}
}

/* ========================================================================= */
/*  memset / bzero                                                           */
/* ========================================================================= */

static void	test_memset(void)
{
	struct { int c; size_t n; const char *hint; } t[] = {
		{'A', 10, ""},
		{'A', 20, ""},
		{'\0', 20, ""},
		{200, 7, "les valeurs de 128 a 255 doivent etre ecrites telles quelles"},
		{-1, 5, "c doit etre converti en unsigned char (-1 -> 255)"},
		{300, 5, "c doit etre converti en unsigned char (300 -> 44 = ',')"},
		{'Z', 0, "n = 0 : ne doit rien modifier"},
	};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		char	a[20], b[20];

		memset(a, 'x', 20);
		memset(b, 'x', 20);
		CASE("ft_memset(buf, %s, %zu)  avec buf = \"xxxxxxxxxxxxxxxxxxxx\"", chr(t[i].c), t[i].n);
		void	*r = FT(ft_memset(a, t[i].c, t[i].n));
		memset(b, t[i].c, t[i].n);
		TEST(memcmp(a, b, 20) == 0, f("buf = %s", mem(b, 20)), f("buf = %s", mem(a, 20)),
			t[i].hint);
		TEST(r == a, "retourne buf", f("retourne %s", ptr("buf", a, r)),
			"doit renvoyer son premier argument");
	}
}

static void	test_bzero(void)
{
	size_t	t[] = {5, 20, 1, 0};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		char	a[20], b[20];

		memset(a, 'x', 20);
		memset(b, 'x', 20);
		CASE("ft_bzero(buf, %zu)  avec buf = \"xxxxxxxxxxxxxxxxxxxx\"", t[i]);
		FTV(ft_bzero(a, t[i]));
		bzero(b, t[i]);
		TEST(memcmp(a, b, 20) == 0, f("buf = %s", mem(b, 20)), f("buf = %s", mem(a, 20)),
			t[i] == 0 ? "n = 0 : ne doit rien modifier" : "");
	}
}

/* ========================================================================= */
/*  memcpy / memmove                                                         */
/* ========================================================================= */

static void	test_memcpy(void)
{
	const char	src[] = "He\310lo\0Wo\377ld!";
	size_t		t[] = {5, 13, 1, 0};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		char	a[20], b[20];

		memset(a, 'x', 20);
		memset(b, 'x', 20);
		CASE("ft_memcpy(buf, %s, %zu)  avec buf = \"xxxxxxxxxxxxxxxxxxxx\"", mem(src, 13), t[i]);
		void	*r = FT(ft_memcpy(a, src, t[i]));
		memcpy(b, src, t[i]);
		TEST(memcmp(a, b, 20) == 0, f("buf = %s", mem(b, 20)), f("buf = %s", mem(a, 20)),
			t[i] == 13 ? "doit copier exactement n octets, meme apres un '\\0'" : "");
		TEST(r == a, "retourne buf", f("retourne %s", ptr("buf", a, r)),
			"doit renvoyer son premier argument");
	}
	{
		char	a[20];

		memset(a, 'x', 20);
		char	*src5 = guard("hello", 5);

		memset(a, 'x', 20);
		CASE("ft_memcpy(buf, src, 5)  avec src = 5 octets \"hello\" suivis de memoire interdite");
		FTV(ft_memcpy(a, src5, 5));
		TEST(memcmp(a, "helloxxx", 8) == 0, "buf = \"helloxxx...\"", f("buf = %s", mem(a, 20)),
			NO_OVERREAD);
		memset(a, 'x', 20);
		CASE("ft_memcpy(buf, NULL, 0)");
		void	*r = FT(ft_memcpy(a, NULL, 0));
		TEST(r == a && a[0] == 'x', "retourne buf, buf inchange",
			f("retourne %s, buf = %s", ptr("buf", a, r), mem(a, 20)),
			"n = 0 : ne doit rien lire ni ecrire");
	}
}

static void	test_memmove(void)
{
	struct { size_t dst; size_t src; size_t n; const char *hint; } t[] = {
		{2, 0, 5, "dest apres src (chevauchement) : il faut copier en partant de la fin"},
		{0, 2, 5, "dest avant src (chevauchement) : copier en partant du debut"},
		{1, 0, 9, "dest apres src (chevauchement)"},
		{0, 1, 9, "dest avant src (chevauchement)"},
		{0, 0, 5, "dest == src"},
		{0, 5, 5, "sans chevauchement"},
		{3, 0, 0, "n = 0 : ne doit rien modifier"},
	};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		char	a[11] = "01234\31089";
		char	b[11] = "01234\31089";

		CASE("ft_memmove(s + %zu, s + %zu, %zu)  avec s = %s", t[i].dst, t[i].src, t[i].n,
			mem("01234\31089", 10));
		void	*r = FT(ft_memmove(a + t[i].dst, a + t[i].src, t[i].n));
		memmove(b + t[i].dst, b + t[i].src, t[i].n);
		TEST(memcmp(a, b, 11) == 0, f("s = %s", mem(b, 10)), f("s = %s", mem(a, 10)),
			t[i].hint);
		TEST(r == a + t[i].dst, f("retourne s + %zu", t[i].dst), f("retourne %s", ptr("s", a, r)),
			"doit renvoyer son premier argument");
	}
	/* gros buffers qui se chevauchent, avec tous les octets de 0 a 255 */
	for (int dir = 0; dir < 2; dir++)
	{
		static unsigned char	a[3000], b[3000];
		size_t					d = dir ? 0 : 777, s = dir ? 777 : 0, idx;

		for (size_t i = 0; i < 3000; i++)
			a[i] = b[i] = (unsigned char)(i * 7);
		CASE("ft_memmove(buf + %zu, buf + %zu, 2000)  sur 3000 octets (valeurs 0 a 255)", d, s);
		FTV(ft_memmove(a + d, a + s, 2000));
		memmove(b + d, b + s, 2000);
		idx = diff_at(a, b, 3000);
		TEST(idx == 3000, "tous les octets corrects",
			f("buf[%zu] = %d au lieu de %d", idx, idx < 3000 ? a[idx] : 0,
				idx < 3000 ? b[idx] : 0),
			"le chevauchement doit marcher quelle que soit la taille (pas de buffer temporaire fixe)");
	}
	{
		char	a[20];

		memset(a, 'x', 20);
		CASE("ft_memmove(buf, NULL, 0)");
		void	*r = FT(ft_memmove(a, NULL, 0));
		TEST(r == a && a[0] == 'x', "retourne buf, buf inchange",
			f("retourne %s, buf = %s", ptr("buf", a, r), mem(a, 20)),
			"n = 0 : ne doit rien lire ni ecrire");
	}
}

/* ========================================================================= */
/*  strlcpy / strlcat                                                        */
/* ========================================================================= */

static const char	*size_txt(size_t n)
{
	if (n == SIZE_MAX)
		return ("SIZE_MAX");
	return (f("%zu", n));
}

static void	test_strlcpy(void)
{
	struct { const char *src; size_t size; } t[] = {
		{"coucou", 0}, {"coucou", 1}, {"coucou", 2}, {"coucou", 5}, {"coucou", 6},
		{"coucou", 7}, {"coucou", 8}, {"coucou", 20}, {"coucou", SIZE_MAX},
		{"", 42}, {"", 0}, {"1", 0}, {"a\310\377b", 20}, {"a\310\377b", 3},
	};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		char	a[20], b[20];
		size_t	e;

		memset(a, 'x', 20);
		memset(b, 'x', 20);
		CASE("ft_strlcpy(dst, %s, %s)  avec dst = \"xxxxxxxxxxxxxxxxxxxx\"",
			q(t[i].src), size_txt(t[i].size));
		size_t	r = FT(ft_strlcpy(a, t[i].src, t[i].size));
		e = ref_strlcpy(b, t[i].src, t[i].size);
		TEST(r == e && memcmp(a, b, 20) == 0,
			f("retourne %zu, dst = %s", e, mem(b, 20)),
			f("retourne %zu, dst = %s", r, mem(a, 20)),
			r != e ? "doit renvoyer strlen(src), quelle que soit size"
			: "copie au plus size - 1 caracteres, puis un seul '\\0' (rien du tout si size = 0)");
	}
	{
		CASE("ft_strlcpy(NULL, \"abc\", 0)");
		size_t	r = FT(ft_strlcpy(NULL, "abc", 0));
		TEST(r == 3, "retourne 3", f("retourne %zu", r),
			"size = 0 : dst ne doit pas etre touche du tout, meme s'il vaut NULL");
	}
}

static void	test_strlcat(void)
{
	struct { const char *dst; const char *src; size_t size; } t[] = {
		{"abc", "def", 20}, {"abc", "def", 7}, {"abc", "def", 6}, {"abc", "def", 5},
		{"abc", "def", 4}, {"abc", "def", 3}, {"abc", "def", 2}, {"abc", "def", 1},
		{"abc", "def", 0}, {"abc", "def", SIZE_MAX},
		{"", "123", 0}, {"", "123", 1}, {"", "123", 2}, {"", "123", 3}, {"", "123", 4},
		{"", "", 0}, {"", "", 42}, {"abc", "", 20}, {"abc", "", 2},
		{"1111111111", "AAAAAAAAA", 5}, {"1111111111", "", 15},
		{"a\310", "\377b", 20},
	};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		char	a[20], b[20];
		size_t	e;

		memset(a, 'x', 20);
		memset(b, 'x', 20);
		memcpy(a, t[i].dst, strlen(t[i].dst) + 1);
		memcpy(b, t[i].dst, strlen(t[i].dst) + 1);
		CASE("ft_strlcat(dst, %s, %s)  avec dst = %s", q(t[i].src), size_txt(t[i].size),
			q(t[i].dst));
		size_t	r = FT(ft_strlcat(a, t[i].src, t[i].size));
		e = ref_strlcat(b, t[i].src, t[i].size);
		TEST(r == e && memcmp(a, b, 20) == 0,
			f("retourne %zu, dst = %s", e, q(b)),
			f("retourne %zu, dst = %s", r, memchr(a, 0, 20) ? q(a) : mem(a, 20)),
			r != e ? "retour : size + strlen(src) si size <= strlen(dst), sinon strlen(dst) + strlen(src)"
			: "dst doit faire au plus size - 1 caracteres au total, termine par '\\0'");
	}
	{
		char	*a = guard("12345", 5);   /* pas de '\0', puis memoire interdite */

		CASE("ft_strlcat(dst, \"abc\", 5)  avec dst = 5 octets \"12345\" SANS '\\0',"
			" suivis de memoire interdite");
		size_t	r = FT(ft_strlcat(a, "abc", 5));
		TEST(r == 8 && memcmp(a, "12345", 5) == 0, "retourne 8, dst inchange",
			f("retourne %zu, dst = %s", r, mem(a, 5)),
			"ne doit pas lire dst au-dela de size octets (pas de strlen(dst) direct)");
	}
	{
		CASE("ft_strlcat(NULL, \"abc\", 0)");
		size_t	r = FT(ft_strlcat(NULL, "abc", 0));
		TEST(r == 3, "retourne 3", f("retourne %zu", r),
			"size = 0 : l'original ne lit pas dst du tout, meme s'il vaut NULL");
	}
}

/* ========================================================================= */
/*  strchr / strrchr                                                         */
/* ========================================================================= */

static char	*(*g_chr_ft)(const char *, int);
static char	*(*g_chr_ref)(const char *, int);

static void	test_chr(void)
{
	const char	*s1 = "bonjour le monde";
	const char	*s2 = "ab\310cd\310ef";
	const char	*s3 = "";
	struct { const char *s; int c; const char *hint; } t[] = {
		{s1, 'o', ""}, {s1, 'b', ""}, {s1, 'e', ""}, {s1, 'd', ""}, {s1, 'z', ""},
		{s1, '\0', "chercher '\\0' doit renvoyer un pointeur sur le '\\0' final"},
		{s1, 'o' + 256, "c doit etre converti en char ('o' + 256 = 367 -> 'o')"},
		{s2, 200, "200 doit trouver l'octet \\310 (= 200) : convertir c en char avant de comparer"},
		{s2, -56, "-56 et 200 donnent le meme char : doit trouver \\310"},
		{s3, 'a', ""},
		{s3, '\0', "chaine vide, chercher '\\0' : doit renvoyer s"},
	};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		CASE("ft_%s(s, %s)  avec s = %s", g_name, chr(t[i].c), q(t[i].s));
		char	*r = FT(g_chr_ft(t[i].s, t[i].c));
		char	*e = g_chr_ref(t[i].s, t[i].c);

		TEST(r == e, ptr("s", t[i].s, e), ptr("s", t[i].s, r), t[i].hint);
	}
}

static void	test_chr_all(void);

static void	run_chr(const char *name, char *(*ft)(const char *, int),
	char *(*ref)(const char *, int))
{
	g_name = name;
	g_chr_ft = ft;
	g_chr_ref = ref;
	run(name, ft != NULL, test_chr_all);
}

/* ========================================================================= */
/*  strncmp / memcmp / memchr                                                */
/* ========================================================================= */

static void	test_strncmp(void)
{
	struct { const char *a; const char *b; size_t n; const char *hint; } t[] = {
		{"t", "", 0, "n = 0 doit renvoyer 0"},
		{"1234", "1235", 3, "ne doit comparer que les n premiers caracteres"},
		{"1234", "1235", 4, ""},
		{"1234", "1235", SIZE_MAX, ""},
		{"", "", 42, ""},
		{"Tripouille", "Tripouille", 42, ""},
		{"Tripouille", "tripouille", 42, ""},
		{"Tripouille", "TriPouille", 42, ""},
		{"Tripouille", "TripouillE", 42, ""},
		{"Tripouille", "TripouilleX", 42, ""},
		{"Tripouille", "Tripouill", 42, ""},
		{"", "1", 0, ""}, {"1", "", 0, ""}, {"", "1", 1, ""}, {"1", "", 1, ""},
		{"", "", 1, ""},
		{"abc\0x", "abc\0y", 5, "doit s'arreter au premier '\\0' (contrairement a memcmp)"},
		{"abc", "abc", SIZE_MAX, "doit s'arreter au '\\0' meme si n est enorme"},
		{"test\200", "test", 6, "il faut comparer en unsigned char (\\200 = 128 > 0)"},
		{"test", "test\200", 6, "il faut comparer en unsigned char"},
		{"\377", "\001", 1, "il faut comparer en unsigned char (255 > 1)"},
		{"test", "tes\326", 4, "il faut comparer en unsigned char (\\326 = 214, -42 en char)"},
	};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		CASE("ft_strncmp(%s, %s, %s)", q(t[i].a), q(t[i].b), size_txt(t[i].n));
		int	r = FT(ft_strncmp(t[i].a, t[i].b, t[i].n));
		int	e = strncmp(t[i].a, t[i].b, t[i].n);

		TEST((r > 0) - (r < 0) == (e > 0) - (e < 0), sign_name(e),
			f("%d (%s)", r, sign_name(r)), t[i].hint);
	}
	{
		const char	*b[] = {"abc", "abd", "abb"};

		for (int i = 0; i < 3; i++)
		{
			char	*a = guard("abc", 3);

			CASE("ft_strncmp(a, %s, 3)  avec a = 3 octets \"abc\" SANS '\\0', suivis de memoire"
				" interdite", q(b[i]));
			int	r = FT(ft_strncmp(a, b[i], 3));
			int	e = strncmp("abc", b[i], 3);

			TEST((r > 0) - (r < 0) == (e > 0) - (e < 0), sign_name(e),
				f("%d (%s)", r, sign_name(r)), "ne doit pas lire plus de n caracteres");
		}
	}
}

static void	test_memcmp(void)
{
	struct { const char *a; const char *b; size_t n; const char *hint; } t[] = {
		{"abcd", "abcd", 4, ""},
		{"abcd", "abce", 4, ""},
		{"abce", "abcd", 4, ""},
		{"abcd", "abce", 3, "ne doit comparer que les n premiers octets"},
		{"abc", "xyz", 0, "n = 0 doit renvoyer 0"},
		{"abc\0x", "abc\0y", 5, "doit continuer apres les '\\0' (contrairement a strncmp)"},
		{"\200", "\0", 1, "il faut comparer en unsigned char (\\200 = 128 > 0)"},
		{"\0", "\200", 1, "il faut comparer en unsigned char"},
		{"\377", "\376", 1, ""},
		{"a\310", "a\001", 2, "il faut comparer en unsigned char (\\310 = 200 > 1)"},
	};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		CASE("ft_memcmp(%s, %s, %zu)", mem(t[i].a, t[i].n ? t[i].n : 3),
			mem(t[i].b, t[i].n ? t[i].n : 3), t[i].n);
		int	r = FT(ft_memcmp(t[i].a, t[i].b, t[i].n));
		int	e = memcmp(t[i].a, t[i].b, t[i].n);

		TEST((r > 0) - (r < 0) == (e > 0) - (e < 0), sign_name(e),
			f("%d (%s)", r, sign_name(r)), t[i].hint);
	}
	{
		char	*a = guard("abc", 3);

		CASE("ft_memcmp(a, \"abc\", 3)  avec a = 3 octets \"abc\" suivis de memoire interdite");
		int	r = FT(ft_memcmp(a, "abc", 3));
		TEST(r == 0, "0", f("%d", r), "ne doit pas lire plus de n octets");
	}
}

static void	test_memchr(void)
{
	const char	s[] = {'a', 'b', '\0', 'c', (char)200, (char)255, 'd', 'e'};
	struct { int c; size_t n; const char *hint; } t[] = {
		{'a', 8, ""}, {'b', 8, ""},
		{'\0', 8, ""},
		{'c', 8, "doit continuer apres les '\\0'"},
		{'e', 8, ""},
		{'e', 7, "ne doit pas lire plus de n octets"},
		{'z', 8, ""},
		{'a', 0, "n = 0 doit renvoyer NULL"},
		{200, 8, "doit trouver l'octet 200 : comparer en unsigned char"},
		{255, 8, "doit trouver l'octet 255 : comparer en unsigned char"},
		{-56, 8, "c doit etre converti en unsigned char (-56 -> 200)"},
		{'b' + 256, 8, "c doit etre converti en unsigned char ('b' + 256 -> 'b')"},
	};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		CASE("ft_memchr(s, %s, %zu)  avec s = %s", chr(t[i].c), t[i].n, mem(s, 8));
		void	*r = FT(ft_memchr(s, t[i].c, t[i].n));
		void	*e = memchr(s, t[i].c, t[i].n);

		TEST(r == e, ptr("s", s, e), ptr("s", s, r), t[i].hint);
	}
	{
		char	*g4 = guard("abcd", 4);

		CASE("ft_memchr(s, 'z', 4)  avec s = 4 octets \"abcd\" suivis de memoire interdite");
		void	*r = FT(ft_memchr(g4, 'z', 4));
		TEST(r == NULL, "NULL", ptr("s", g4, r), "ne doit pas lire plus de n octets");
	}
}

/* ========================================================================= */
/*  strnstr                                                                  */
/* ========================================================================= */

static void	test_strnstr(void)
{
	const char	*h = "aaabcabcd";
	struct { const char *big; const char *little; size_t len; const char *hint; } t[] = {
		{h, "aabc", 0, "len = 0 : rien a chercher"},
		{h, "aabc", SIZE_MAX, "apres un debut de correspondance rate, reprendre au caractere suivant"},
		{h, "a", SIZE_MAX, ""},
		{h, "c", SIZE_MAX, ""},
		{"", "", SIZE_MAX, "little vide : doit renvoyer big"},
		{"", "", 0, "little vide : doit renvoyer big, meme si len = 0"},
		{h, "", 0, "little vide : doit renvoyer big, meme si len = 0"},
		{"", "coucou", SIZE_MAX, ""},
		{h, "aaabc", 5, "little tient exactement dans les len premiers caracteres"},
		{"", "12345", 5, ""},
		{h, "abcd", 9, ""},
		{h, "cd", 8, "little depasse les len premiers caracteres"},
		{h, "a", 1, ""},
		{"1", "a", 1, ""},
		{"22", "b", 2, ""},
		{"22", "22", 3, "len plus grand que big"},
		{"abc", "abcdef", SIZE_MAX, "little plus long que big"},
		{"ab\310cd", "\310c", 5, ""},
	};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		CASE("ft_strnstr(big, %s, %s)  avec big = %s", q(t[i].little), size_txt(t[i].len),
			q(t[i].big));
		char	*r = FT(ft_strnstr(t[i].big, t[i].little, t[i].len));
		char	*e = ref_strnstr(t[i].big, t[i].little, t[i].len);

		TEST(r == e, ptr("big", t[i].big, e), ptr("big", t[i].big, r), t[i].hint);
	}
	{
		const char	*l[] = {"cd", "de", "e", "ef", "abcdef"};
		const char	*ex[] = {"big + 2", "big + 3", "big + 4", "NULL", "NULL"};

		for (int i = 0; i < 5; i++)
		{
			char	*big = guard("abcde", 5);

			CASE("ft_strnstr(big, %s, 5)  avec big = 5 octets \"abcde\" SANS '\\0', suivis de"
				" memoire interdite", q(l[i]));
			char	*r = FT(ft_strnstr(big, l[i], 5));
			const char	*plain = "abcde";
			char		*e = ref_strnstr(plain, l[i], 5);

			TEST((r ? r - big : -1) == (e ? e - plain : -1), ex[i], ptr("big", big, r),
				"ne doit lire que les len premiers caracteres de big");
		}
	}
}

/* ========================================================================= */
/*  atoi                                                                     */
/* ========================================================================= */

static void	test_atoi(void)
{
	const char	*t[] = {
		"0", "1", "42", "-42", "+42", "-0", "+0", "000123", "-000123",
		" \t\n\v\f\r1", " \t\n\v\f\r-1", " \t\n\v\f\r+101", " \t\n\v\f\r+42lyon",
		" \t\n\v\f\ra1", " \t\n\v\f\r--1", " \t\n\v\f\r++1",
		"2147483647", "-2147483648", " \t\n\v\f\r2147483647", " \t\n\v\f\r-2147483648",
		"-+42", "+-42", "+ 42", "- 42", "+\t42", "1 42", "-1\n42", "42abc", "abc42",
		"", " ", "+", "-", "\x1b" "42", "\200" "42", "12\200" "3", "\0" "42",
	};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		CASE("ft_atoi(%s)", q(t[i]));
		int	r = FT(ft_atoi(t[i]));
		int	e = atoi(t[i]);

		TEST(r == e, f("%d", e), f("%d", r),
			t[i][0] == '\x1b' ? "seuls ' ', \\t, \\n, \\v, \\f et \\r sont des espaces a sauter" : "");
	}
}

/* ========================================================================= */
/*  calloc / strdup                                                          */
/* ========================================================================= */

static void	test_calloc(void)
{
	struct { size_t n; size_t m; const char *txt; } ok[] = {
		{2, 2, "2, 2"}, {50, 1, "50, 1"}, {10, sizeof(int), "10, sizeof(int)"}, {3, 7, "3, 7"},
	};
	struct { size_t n; size_t m; const char *txt; int null; } edge[] = {
		{0, 0, "0, 0", 0}, {0, 5, "0, 5", 0}, {5, 0, "5, 0", 0},
		{0, (size_t)-5, "0, -5", 0}, {(size_t)-5, 0, "-5, 0", 0},
		{SIZE_MAX, SIZE_MAX, "SIZE_MAX, SIZE_MAX", 1},
		{SIZE_MAX / 2 + 2, 2, "SIZE_MAX / 2 + 2, 2", 1},
		{(size_t)INT_MAX, (size_t)INT_MAX, "INT_MAX, INT_MAX", 1},
		{(size_t)INT_MIN, (size_t)INT_MIN, "INT_MIN, INT_MIN", 1},
		{(size_t)-5, (size_t)-5, "-5, -5", 1},
		{3, (size_t)-5, "3, -5", 1}, {(size_t)-5, 3, "-5, 3", 1},
	};

	for (size_t i = 0; i < sizeof(ok) / sizeof(*ok); i++)
	{
		size_t			total = ok[i].n * ok[i].m;
		size_t			bad;

		CASE("ft_calloc(%s)", ok[i].txt);
		g_nalloc = 0;
		unsigned char	*p = FT(ft_calloc(ok[i].n, ok[i].m));
		TEST(p != NULL, "un pointeur non NULL", "NULL", "");
		if (!p)
			continue ;
		TEST(g_nalloc == 1 && g_last_size == total, f("1 malloc de %zu octet(s)", total),
			f("%d malloc, le dernier de %zu octet(s)", g_nalloc, g_last_size),
			"doit faire un seul malloc de nmemb * size octets");
		bad = diff_at(p, (unsigned char[64]){0}, total);
		TEST(bad == total, f("%zu octets a 0", total),
			f("l'octet %zu vaut %d", bad, bad < total ? p[bad] : 0),
			"toute la zone (nmemb * size octets) doit etre mise a 0");
		free(p);
	}
	for (size_t i = 0; i < sizeof(edge) / sizeof(*edge); i++)
	{
		CASE("ft_calloc(%s)", edge[i].txt);
		void	*p = FT(ft_calloc(edge[i].n, edge[i].m));
		if (edge[i].null)
			TEST(p == NULL, "NULL", "un pointeur non NULL",
				"nmemb * size depasse SIZE_MAX : il faut le detecter AVANT de multiplier"
				" (ex : if (size && nmemb > SIZE_MAX / size)) et renvoyer NULL");
		else
			TEST(p != NULL, "un pointeur non NULL (liberable avec free)", "NULL",
				"avec 0, le vrai calloc renvoie un pointeur unique, pas NULL");
		free(p);
	}
	{
		CASE("ft_calloc(5, 5)  avec malloc qui echoue (renvoie NULL)");
		g_fail = 1;
		void	*p = FT(ft_calloc(5, 5));
		g_fail = 0;
		TEST(p == NULL, "NULL", "un pointeur non NULL",
			"si malloc renvoie NULL, il faut renvoyer NULL sans toucher a la memoire");
		free(p);
	}
}

static void	test_strdup(void)
{
	static char	big[10001];
	const char	*t[] = {"coucou", "", "une chaine un peu plus longue\n", "a\200\310\377z", big};

	memset(big, 'b', 10000);
	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		size_t	len = strlen(t[i]);

		CASE("ft_strdup(%s)", t[i] == big ? "chaine de 10000 'b'" : q(t[i]));
		g_nalloc = 0;
		char	*r = FT(ft_strdup(t[i]));
		TEST(r != NULL, "un pointeur non NULL", "NULL", "");
		if (!r)
			continue ;
		TEST(strcmp(r, t[i]) == 0, t[i] == big ? "la meme chaine" : q(t[i]), q(r), "");
		TEST(r != t[i], "une nouvelle zone allouee", "le pointeur d'origine",
			"il faut malloc une copie, pas renvoyer s");
		TEST(g_nalloc == 1 && g_last_size == len + 1, f("1 malloc de %zu octet(s)", len + 1),
			f("%d malloc, le dernier de %zu octet(s)", g_nalloc, g_last_size),
			"doit allouer exactement strlen(s) + 1 octets");
		free(r);
	}
	{
		CASE("ft_strdup(\"coucou\")  avec malloc qui echoue (renvoie NULL)");
		g_fail = 1;
		char	*r = FT(ft_strdup("coucou"));
		g_fail = 0;
		TEST(r == NULL, "NULL", "un pointeur non NULL",
			"si malloc renvoie NULL, il faut renvoyer NULL sans rien copier");
		free(r);
	}
}

/* ========================================================================= */
/*  PARTIE 2                                                                 */
/* ========================================================================= */

/* Verifie que r a ete alloue avec exactement n octets */
static void	check_size(const void *r, size_t n, const char *what)
{
	size_t	got = block_size(r);

	TEST(got == n, f("%s alloue avec malloc(%zu)", what, n),
		got == (size_t)-1 ? f("%s pas alloue avec malloc", what)
		: f("%s alloue avec malloc(%zu)", what, got),
		got == (size_t)-1 ? "le resultat doit venir de malloc (pas une chaine litterale ou statique)"
		: "il faut allouer exactement la place necessaire, '\\0' compris");
}

/* Simule l'echec de chaque malloc, un par un.
   La fonction doit alors renvoyer NULL et ne rien laisser alloue. */
static void	malloc_fail(const char *desc, void *(*call)(void), void (*release)(void *))
{
	void	*r;
	int		n;
	int		live0;

	g_nalloc = 0;
	CASE("%s", desc);
	r = call();
	n = g_nalloc;
	if (r)
		release(r);
	for (int k = 1; k <= n; k++)
	{
		live0 = g_live;
		CASE("%s  avec le malloc n.%d (sur %d) qui echoue", desc, k, n);
		g_fail = k;
		r = call();
		g_fail = 0;
		TEST(r == NULL, "NULL", "un pointeur non NULL",
			"si un malloc echoue, la fonction doit renvoyer NULL");
		if (r)
			release(r);
		TEST(g_live == live0, "aucune fuite", f("%d bloc(s) jamais libere(s)", g_live - live0),
			"avant de renvoyer NULL, il faut free tout ce qui avait deja ete alloue");
	}
}

static void	release_str(void *p)
{
	free(p);
}

/* ----------------------------- substr ----------------------------------- */

static void	*mf_substr(void)
{
	return (FT(ft_substr("hello world", 3, 5)));
}

static void	test_substr(void)
{
	struct { const char *s; unsigned int start; size_t len; const char *txt; } t[] = {
		{"tripouille", 0, 42000, "42000"}, {"tripouille", 1, 1, "1"},
		{"tripouille", 100, 1, "1"}, {"1", 42, 42000000, "42000000"},
		{"0123456789", 9, 10, "10"}, {"42", 0, 0, "0"},
		{"BONJOUR LES HARICOTS !", 8, 14, "14"}, {"test", 1, 2, "2"},
		{"hello", 0, 5, "5"}, {"hello", 5, 1, "1"}, {"hello", 4, SIZE_MAX, "SIZE_MAX"},
		{"hello", 2, 3, "3"}, {"", 0, 5, "5"}, {"", 1, 5, "5"},
		{"hello", UINT_MAX, 3, "3"}, {"a\310\377b", 1, 2, "2"},
	};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		size_t	sl = strlen(t[i].s);
		size_t	n = 0;
		char	e[64];

		if (t[i].start < sl)
			n = (t[i].len < sl - t[i].start) ? t[i].len : sl - t[i].start;
		memcpy(e, t[i].s + (n ? t[i].start : 0), n);
		e[n] = '\0';
		CASE("ft_substr(%s, %u, %s)", q(t[i].s), t[i].start, t[i].txt);
		char	*r = FT(ft_substr(t[i].s, t[i].start, t[i].len));
		TEST(r != NULL, q(e), "NULL", "");
		if (!r)
			continue ;
		TEST(strcmp(r, e) == 0, q(e), q(r),
			t[i].start >= sl ? "si start >= strlen(s), il faut renvoyer une chaine vide \"\" allouee"
			: "le resultat fait au plus len caracteres, a partir de s[start]");
		check_size(r, n + 1, "resultat");
		free(r);
	}
	{
		char	*gs = guard("abc", 4);

		CASE("ft_substr(s, 10, 5)  avec s = \"abc\" suivi de memoire interdite");
		char	*r = FT(ft_substr(gs, 10, 5));
		TEST(r && r[0] == '\0', "\"\"", q(r),
			"start depasse la fin de s : il ne faut pas lire s + start");
		free(r);
	}
	malloc_fail("ft_substr(\"hello world\", 3, 5)", mf_substr, release_str);
}

/* ----------------------------- strjoin ---------------------------------- */

static void	*mf_strjoin(void)
{
	return (FT(ft_strjoin("hello ", "world")));
}

static void	test_strjoin(void)
{
	static char	big1[5001], big2[5001], bige[10001];
	struct { const char *a; const char *b; } t[] = {
		{"tripouille", "42"}, {"", "42"}, {"42", ""}, {"", ""},
		{"hello ", "world"}, {"a\310", "\377b"}, {big1, big2},
	};

	memset(big1, 'a', 5000);
	memset(big2, 'b', 5000);
	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		size_t	la = strlen(t[i].a);
		size_t	lb = strlen(t[i].b);
		char	*e = bige;

		memcpy(e, t[i].a, la);
		memcpy(e + la, t[i].b, lb + 1);
		if (t[i].a == big1)
			CASE("ft_strjoin(5000 'a', 5000 'b')");
		else
			CASE("ft_strjoin(%s, %s)", q(t[i].a), q(t[i].b));
		char	*r = FT(ft_strjoin(t[i].a, t[i].b));
		TEST(r != NULL, q(e), "NULL", "");
		if (!r)
			continue ;
		TEST(strcmp(r, e) == 0, q(e), q(r), "");
		check_size(r, la + lb + 1, "resultat");
		free(r);
	}
	malloc_fail("ft_strjoin(\"hello \", \"world\")", mf_strjoin, release_str);
}

/* ----------------------------- strtrim ---------------------------------- */

static void	*mf_strtrim(void)
{
	return (FT(ft_strtrim("  xxhello worldxx  ", " x")));
}

static void	test_strtrim(void)
{
	struct { const char *s; const char *set; const char *e; const char *hint; } t[] = {
		{"   xxxtripouille", " x", "tripouille", ""},
		{"tripouille   xxx", " x", "tripouille", ""},
		{"   xxxtripouille   xxx", " x", "tripouille", ""},
		{"   xxx   xxx", " x", "", "tout est dans set : il faut renvoyer \"\" alloue"},
		{"", "123", "", ""},
		{"123", "", "123", "set vide : rien a enlever"},
		{"", "", "", ""},
		{"abcdba", "acb", "d", ""},
		{"ababa", "a", "bab", "seulement au debut et a la fin, pas au milieu"},
		{"  hello  world  ", " ", "hello  world", "seulement au debut et a la fin, pas au milieu"},
		{"x", "x", "", ""},
		{"abc", "abc", "", ""},
		{"\310\310abc\377", "\310\377", "abc", "les caracteres de 128 a 255 doivent marcher aussi"},
	};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		CASE("ft_strtrim(%s, %s)", q(t[i].s), q(t[i].set));
		char	*r = FT(ft_strtrim(t[i].s, t[i].set));
		TEST(r != NULL, q(t[i].e), "NULL", "");
		if (!r)
			continue ;
		TEST(strcmp(r, t[i].e) == 0, q(t[i].e), q(r), t[i].hint);
		check_size(r, strlen(t[i].e) + 1, "resultat");
		free(r);
	}
	malloc_fail("ft_strtrim(\"  xxhello worldxx  \", \" x\")", mf_strtrim, release_str);
}

/* ----------------------------- split ------------------------------------ */

static void	free_tab(void *p)
{
	char	**t = p;

	for (size_t i = 0; t && t[i] && i < 5000; i++)
		free(t[i]);
	free(t);
}

static char	*tab_str(char *const *t, size_t max)
{
	char	buf[500];
	size_t	j = 0;

	if (!t)
		return ("NULL");
	j += snprintf(buf + j, sizeof(buf) - j, "{");
	for (size_t i = 0; i < max && j < 400; i++)
	{
		if (!t[i])
		{
			j += snprintf(buf + j, sizeof(buf) - j, "NULL}");
			return (f("%s", buf));
		}
		j += snprintf(buf + j, sizeof(buf) - j, "%s, ", q(t[i]));
	}
	snprintf(buf + j, sizeof(buf) - j, "... (pas de NULL a la fin ?)}");
	return (f("%s", buf));
}

static void	*mf_split(void)
{
	return (FT(ft_split("  aa bb  cc dd ", ' ')));
}

static void	test_split(void)
{
	struct { const char *s; char c; const char *e[8]; } t[] = {
		{"  tripouille  42  ", ' ', {"tripouille", "42", NULL}},
		{"tripouille", '\0', {"tripouille", NULL}},
		{"     ", ' ', {NULL}},
		{"", '\0', {NULL}},
		{"chinimala", ' ', {"chinimala", NULL}},
		{"", ' ', {NULL}},
		{"Tripouille ", ' ', {"Tripouille", NULL}},
		{" Tripouille", ' ', {"Tripouille", NULL}},
		{" Tripouille ", ' ', {"Tripouille", NULL}},
		{"--1-2--3---4----5-----42", '-', {"1", "2", "3", "4", "5", "42", NULL}},
		{"a,b,,c", ',', {"a", "b", "c", NULL}},
		{"hello world", 'o', {"hell", " w", "rld", NULL}},
		{"x", 'x', {NULL}},
		{"\310a\310\310b\310", (char)200, {"a", "b", NULL}},
	};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		size_t	ne = 0;
		size_t	ng = 0;
		int		same = 1;

		while (t[i].e[ne])
			ne++;
		CASE("ft_split(%s, %s)", q(t[i].s), chr((unsigned char)t[i].c));
		char	**r = FT(ft_split(t[i].s, t[i].c));
		TEST(r != NULL, tab_str((char *const *)t[i].e, 8), "NULL",
			"meme sans aucun mot, il faut renvoyer un tableau qui contient juste NULL");
		if (!r)
			continue ;
		while (ng <= ne && r[ng])
			ng++;
		if (ng != ne)
			same = 0;
		for (size_t k = 0; same && k < ne; k++)
			if (strcmp(r[k], t[i].e[k]) != 0)
				same = 0;
		TEST(same, tab_str((char *const *)t[i].e, 8), tab_str(r, ne + 1),
			"les delimiteurs consecutifs ou au debut/a la fin ne donnent pas de mot vide");
		if (same)
		{
			check_size(r, (ne + 1) * sizeof(char *), f("tableau de %zu pointeurs (%zu mots + NULL)",
				ne + 1, ne));
			for (size_t k = 0; k < ne; k++)
				check_size(r[k], strlen(r[k]) + 1, f("le mot %s", q(r[k])));
		}
		free_tab(r);
	}
	{
		char	*gs = guard("ab cd", 6);

		CASE("ft_split(s, ' ')  avec s = \"ab cd\" suivi de memoire interdite");
		char	**r = FT(ft_split(gs, ' '));
		TEST(r && r[0] && r[1] && !r[2] && !strcmp(r[0], "ab") && !strcmp(r[1], "cd"),
			"{\"ab\", \"cd\", NULL}", tab_str(r, 3), "il ne faut pas lire apres le '\\0' de s");
		free_tab(r);
	}
	malloc_fail("ft_split(\"  aa bb  cc dd \", ' ')", mf_split, free_tab);
}

/* ----------------------------- itoa ------------------------------------- */

static const int	g_ints[] = {
	0, 1, -1, 9, -9, 10, -10, 42, -42, 100, -100, 123456789, -987654321,
	INT_MAX, INT_MIN, INT_MAX - 1, INT_MIN + 1,
};

static void	*mf_itoa(void)
{
	return (FT(ft_itoa(INT_MIN)));
}

static void	test_itoa(void)
{
	for (size_t i = 0; i < sizeof(g_ints) / sizeof(*g_ints); i++)
	{
		char	e[16];

		snprintf(e, sizeof(e), "%d", g_ints[i]);
		CASE("ft_itoa(%s)", g_ints[i] == INT_MIN ? "INT_MIN" : g_ints[i] == INT_MAX ? "INT_MAX" : e);
		char	*r = FT(ft_itoa(g_ints[i]));
		TEST(r != NULL, q(e), "NULL", "");
		if (!r)
			continue ;
		TEST(strcmp(r, e) == 0, q(e), q(r),
			g_ints[i] == INT_MIN ? "-INT_MIN ne tient pas dans un int : passe par un long"
			" ou traite ce cas a part" : "");
		check_size(r, strlen(e) + 1, "resultat");
		free(r);
	}
	malloc_fail("ft_itoa(INT_MIN)", mf_itoa, release_str);
}

/* ----------------------------- strmapi / striteri ----------------------- */

static unsigned int	g_idx[64];
static const char	*g_ptr[64];
static int			g_ncalls;

static char	map_add_index(unsigned int i, char c)
{
	return ((char)(i + c));
}

static char	map_record(unsigned int i, char c)
{
	if (g_ncalls < 64)
		g_idx[g_ncalls] = i;
	g_ncalls++;
	return (c);
}

static void	*mf_strmapi(void)
{
	return (FT(ft_strmapi("hello", map_add_index)));
}

/* verifie que f a ete appelee exactement pour les indices 0, 1, ..., n - 1 */
static void	check_indices(size_t n)
{
	int	ok = (g_ncalls == (int)n);

	for (size_t k = 0; ok && k < n && k < 64; k++)
		if (g_idx[k] != k)
			ok = 0;
	TEST(ok, f("f appelee %zu fois, avec les indices 0 a %zu dans l'ordre", n, n ? n - 1 : 0),
		f("f appelee %d fois%s", g_ncalls, g_ncalls && g_ncalls < 64
			? f(", indices %u, %u...", g_idx[0], g_ncalls > 1 ? g_idx[1] : g_idx[0]) : ""),
		"f doit etre appelee une fois par caractere, avec son indice (pas pour le '\\0')");
}

static void	test_strmapi(void)
{
	struct { const char *s; const char *e; } t[] = {
		{"1234", "1357"}, {"", ""}, {"aaaa", "abcd"}, {"\310\310", "\310\311"},
	};
	const char	*rec[] = {"hello", "", "a", "\310\377 x"};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		CASE("ft_strmapi(%s, f)  avec f(i, c) = c + i", q(t[i].s));
		char	*r = FT(ft_strmapi(t[i].s, map_add_index));
		TEST(r != NULL, q(t[i].e), "NULL", "");
		if (!r)
			continue ;
		TEST(strcmp(r, t[i].e) == 0, q(t[i].e), q(r), "");
		check_size(r, strlen(t[i].e) + 1, "resultat");
		free(r);
	}
	for (size_t i = 0; i < sizeof(rec) / sizeof(*rec); i++)
	{
		g_ncalls = 0;
		CASE("ft_strmapi(%s, f)  en comptant les appels a f", q(rec[i]));
		char	*r = FT(ft_strmapi(rec[i], map_record));
		check_indices(strlen(rec[i]));
		TEST(r && strcmp(r, rec[i]) == 0, q(rec[i]), q(r), "");
		free(r);
	}
	malloc_fail("ft_strmapi(\"hello\", f)", mf_strmapi, release_str);
}

static char	*g_iter_base;

static void	iter_record(unsigned int i, char *c)
{
	if (g_ncalls < 64)
	{
		g_idx[g_ncalls] = i;
		g_ptr[g_ncalls] = c;
	}
	g_ncalls++;
	*c = (char)(*c + 1);
}

static void	test_striteri(void)
{
	const char	*t[] = {"hello", "", "a", "\310\376 x"};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		char	s[32];
		char	e[32];
		size_t	n = strlen(t[i]);
		int		ptr_ok = 1;

		memcpy(s, t[i], n + 1);
		for (size_t k = 0; k <= n; k++)
			e[k] = k < n ? (char)(t[i][k] + 1) : '\0';
		g_ncalls = 0;
		g_nalloc = 0;
		g_iter_base = s;
		CASE("ft_striteri(s, f)  avec s = %s et f(i, c) : *c = *c + 1", q(t[i]));
		FTV(ft_striteri(s, iter_record));
		check_indices(n);
		for (int k = 0; k < g_ncalls && k < 64; k++)
			if (g_ptr[k] != s + g_idx[k])
				ptr_ok = 0;
		TEST(ptr_ok, "f recoit l'adresse de s[i] (&s[i])", "une autre adresse",
			"il faut passer &s[i], pour que f puisse modifier la chaine elle-meme");
		TEST(memcmp(s, e, n + 1) == 0, f("s = %s", q(e)), f("s = %s", q(s)), "");
		TEST(g_nalloc == 0, "aucun malloc", f("%d malloc", g_nalloc),
			"striteri n'a droit a aucune fonction externe");
	}
}

/* ----------------------------- put*_fd ---------------------------------- */

static int		g_pipe[2];
static int		g_opipe[2];
static int		g_saved_stdout;
static char		g_out[20001];
static size_t	g_outlen;
static char		g_sout[256];
static size_t	g_soutlen;

/* La fonction ecrit dans g_pipe[1] ; la sortie standard est capturee a part
   pour detecter une fonction qui ecrit sur 1 au lieu de fd. */
static int	capture_start(void)
{
	if (pipe(g_pipe) < 0 || pipe(g_opipe) < 0)
		return (-1);
	g_saved_stdout = dup(STDOUT_FILENO);
	dup2(g_opipe[1], STDOUT_FILENO);
	close(g_opipe[1]);
	g_nalloc = 0;
	return (g_pipe[1]);
}

static size_t	read_all(int fd, char *buf, size_t size)
{
	size_t	len = 0;
	ssize_t	r;

	while (len < size - 1 && (r = read(fd, buf + len, size - 1 - len)) > 0)
		len += r;
	buf[len] = '\0';
	return (len);
}

static void	capture_end(void)
{
	dup2(g_saved_stdout, STDOUT_FILENO);
	close(g_saved_stdout);
	close(g_pipe[1]);
	g_outlen = read_all(g_pipe[0], g_out, sizeof(g_out));
	g_soutlen = read_all(g_opipe[0], g_sout, sizeof(g_sout));
	close(g_pipe[0]);
	close(g_opipe[0]);
}

static void	check_output(const char *e, size_t elen)
{
	TEST(g_outlen == elen && memcmp(g_out, e, elen) == 0,
		f("ecrit %s sur fd", mem(e, elen)),
		g_soutlen ? f("ecrit %s sur la sortie standard (fd 1) au lieu de fd",
			mem(g_sout, g_soutlen)) : f("ecrit %s sur fd", mem(g_out, g_outlen)),
		g_soutlen ? "il faut faire write(fd, ...), pas write(1, ...)" : "");
	TEST(g_nalloc == 0, "aucun malloc", f("%d malloc", g_nalloc),
		"seule la fonction write est autorisee");
}

static void	test_putchar_fd(void)
{
	int	cs[] = {'a', 'Z', '0', '\n', ' ', '\0', 200, 255};

	for (size_t i = 0; i < sizeof(cs) / sizeof(*cs); i++)
	{
		char	c = (char)cs[i];
		int		fd = capture_start();

		CASE("ft_putchar_fd(%s, fd)", chr(cs[i]));
		FTV(ft_putchar_fd(c, fd));
		capture_end();
		check_output(&c, 1);
	}
	CASE("ft_putchar_fd('a', -1)  (fd invalide : ne doit pas planter)");
	capture_start();
	FTV(ft_putchar_fd('a', -1));
	capture_end();
	g->ok++;
}

static void	test_putstr_fd(void)
{
	static char	big[10001];
	const char	*t[] = {"42", "", "hello world\n", "a\310\377b", big};

	memset(big, 'z', 10000);
	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		int	fd = capture_start();

		CASE("ft_putstr_fd(%s, fd)", t[i] == big ? "10000 'z'" : q(t[i]));
		FTV(ft_putstr_fd((char *)t[i], fd));
		capture_end();
		check_output(t[i], strlen(t[i]));
	}
	CASE("ft_putstr_fd(\"abc\", -1)  (fd invalide : ne doit pas planter)");
	capture_start();
	FTV(ft_putstr_fd("abc", -1));
	capture_end();
	g->ok++;
}

static void	test_putendl_fd(void)
{
	const char	*t[] = {"42", "", "hello world", "a\310\377b"};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		char	e[64];
		size_t	n = strlen(t[i]);
		int		fd = capture_start();

		memcpy(e, t[i], n);
		e[n] = '\n';
		CASE("ft_putendl_fd(%s, fd)", q(t[i]));
		FTV(ft_putendl_fd((char *)t[i], fd));
		capture_end();
		check_output(e, n + 1);
	}
	CASE("ft_putendl_fd(\"abc\", -1)  (fd invalide : ne doit pas planter)");
	capture_start();
	FTV(ft_putendl_fd("abc", -1));
	capture_end();
	g->ok++;
}

static void	test_putnbr_fd(void)
{
	for (size_t i = 0; i < sizeof(g_ints) / sizeof(*g_ints); i++)
	{
		char	e[16];
		int		fd = capture_start();

		snprintf(e, sizeof(e), "%d", g_ints[i]);
		CASE("ft_putnbr_fd(%s, fd)", g_ints[i] == INT_MIN ? "INT_MIN" : g_ints[i] == INT_MAX
			? "INT_MAX" : e);
		FTV(ft_putnbr_fd(g_ints[i], fd));
		capture_end();
		check_output(e, strlen(e));
	}
	CASE("ft_putnbr_fd(42, -1)  (fd invalide : ne doit pas planter)");
	capture_start();
	FTV(ft_putnbr_fd(42, -1));
	capture_end();
	g->ok++;
}

/* ========================================================================= */
/*  Tests supplementaires : lecture hors limites et entrees non modifiees    */
/*  Les chaines sont placees juste avant une page memoire interdite : lire   */
/*  un seul octet apres leur '\0' fait planter.                              */
/* ========================================================================= */

#define GUARD_NOTE "suivi de memoire interdite"

/* une entree const doit ressortir identique */
static void	ro_check(const char *g2, const char *orig, size_t n, const char *what)
{
	TEST(memcmp(g2, orig, n) == 0, f("%s inchange : %s", what, mem(orig, n)),
		f("%s = %s", what, mem(g2, n)),
		f("%s est const : la fonction ne doit pas le modifier", what));
}

static void	extra_strlen(void)
{
	char	*s = guard("hello", 6);

	CASE("ft_strlen(s)  avec s = \"hello\" " GUARD_NOTE);
	size_t	r = FT(ft_strlen(s));
	TEST(r == 5, "5", f("%zu", r), NO_OVERREAD);
}

static void	extra_chr(void)
{
	struct { int c; int pos; } t[] = {{'z', -1}, {'\0', 3}, {'a', 0}, {'c', 2}};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		char	*s = guard("abc", 4);

		CASE("ft_%s(s, %s)  avec s = \"abc\" " GUARD_NOTE, g_name, chr(t[i].c));
		char	*r = FT(g_chr_ft(s, t[i].c));
		TEST(r == (t[i].pos < 0 ? NULL : s + t[i].pos), t[i].pos < 0 ? "NULL" : f("s + %d", t[i].pos),
			ptr("s", s, r), t[i].pos < 0 ? "c absent : il faut s'arreter au '\\0' et renvoyer NULL"
			: NO_OVERREAD);
		ro_check(s, "abc", 4, "s");
	}
}

static void	extra_strdup(void)
{
	char	*s = guard("hello", 6);

	CASE("ft_strdup(s)  avec s = \"hello\" " GUARD_NOTE);
	char	*r = FT(ft_strdup(s));
	TEST(r && strcmp(r, "hello") == 0, "\"hello\"", q(r), NO_OVERREAD);
	ro_check(s, "hello", 6, "s");
	free(r);
}

static void	extra_strlcpy(void)
{
	size_t	sizes[] = {20, 3, 0};

	for (size_t i = 0; i < 3; i++)
	{
		char	*src = guard("hello", 6);
		char	d[20];
		char	e[20];

		memset(d, 'x', 20);
		memset(e, 'x', 20);
		ref_strlcpy(e, "hello", sizes[i]);
		CASE("ft_strlcpy(dst, src, %zu)  avec src = \"hello\" " GUARD_NOTE, sizes[i]);
		size_t	r = FT(ft_strlcpy(d, src, sizes[i]));
		TEST(r == 5 && memcmp(d, e, 20) == 0, f("retourne 5, dst = %s", mem(e, 20)),
			f("retourne %zu, dst = %s", r, mem(d, 20)), NO_OVERREAD);
		ro_check(src, "hello", 6, "src");
	}
}

static void	extra_strlcat(void)
{
	size_t	sizes[] = {20, 5, 0};

	for (size_t i = 0; i < 3; i++)
	{
		char	*src = guard("def", 4);
		char	d[20];
		char	e[20];

		memset(d, 'x', 20);
		memset(e, 'x', 20);
		memcpy(d, "abc", 4);
		memcpy(e, "abc", 4);
		size_t	re = ref_strlcat(e, "def", sizes[i]);
		CASE("ft_strlcat(dst, src, %zu)  avec dst = \"abc\", src = \"def\" " GUARD_NOTE, sizes[i]);
		size_t	r = FT(ft_strlcat(d, src, sizes[i]));
		TEST(r == re && memcmp(d, e, 20) == 0, f("retourne %zu, dst = %s", re, q(e)),
			f("retourne %zu, dst = %s", r, memchr(d, 0, 20) ? q(d) : mem(d, 20)), NO_OVERREAD);
		ro_check(src, "def", 4, "src");
	}
}

static void	extra_atoi(void)
{
	const char	*t[] = {"123", "-42", "   7", "0"};
	int			e[] = {123, -42, 7, 0};

	for (size_t i = 0; i < 4; i++)
	{
		char	*s = guard(t[i], strlen(t[i]) + 1);

		CASE("ft_atoi(s)  avec s = %s " GUARD_NOTE, q(t[i]));
		int	r = FT(ft_atoi(s));
		TEST(r == e[i], f("%d", e[i]), f("%d", r), NO_OVERREAD);
	}
}

static void	extra_memmove(void)
{
	char	*src = guard("hello", 5);
	char	d[20];

	memset(d, 'x', 20);
	CASE("ft_memmove(buf, src, 5)  avec src = 5 octets \"hello\" " GUARD_NOTE);
	FTV(ft_memmove(d, src, 5));
	TEST(memcmp(d, "helloxxx", 8) == 0, "buf = \"helloxxx...\"", f("buf = %s", mem(d, 20)),
		NO_OVERREAD);
	ro_check(src, "hello", 5, "src");
}

static void	extra_strnstr(void)
{
	const char	*big = "abcde";
	char		*little = guard_slot(1, "cd", 3);

	CASE("ft_strnstr(\"abcde\", little, 5)  avec little = \"cd\" " GUARD_NOTE);
	char	*r = FT(ft_strnstr(big, little, 5));
	TEST(r == big + 2, "big + 2", ptr("big", big, r), NO_OVERREAD);
	ro_check(little, "cd", 3, "little");
}

static void	extra_substr(void)
{
	char	*s = guard("hello", 6);

	CASE("ft_substr(s, 1, 3)  avec s = \"hello\" " GUARD_NOTE);
	char	*r = FT(ft_substr(s, 1, 3));
	TEST(r && strcmp(r, "ell") == 0, "\"ell\"", q(r), NO_OVERREAD);
	ro_check(s, "hello", 6, "s");
	free(r);
}

static void	extra_strjoin(void)
{
	char	*a = guard_slot(0, "ab", 3);
	char	*b = guard_slot(1, "cd", 3);

	CASE("ft_strjoin(s1, s2)  avec s1 = \"ab\" et s2 = \"cd\", chacun " GUARD_NOTE);
	char	*r = FT(ft_strjoin(a, b));
	TEST(r && strcmp(r, "abcd") == 0, "\"abcd\"", q(r), NO_OVERREAD);
	ro_check(a, "ab", 3, "s1");
	ro_check(b, "cd", 3, "s2");
	free(r);
}

static void	extra_strtrim(void)
{
	const char	*t[] = {"  ab  ", "   ", "ab"};
	const char	*e[] = {"ab", "", "ab"};

	for (int i = 0; i < 3; i++)
	{
		size_t	n = strlen(t[i]) + 1;
		char	*s = guard_slot(0, t[i], n);
		char	*set = guard_slot(1, " ", 2);

		CASE("ft_strtrim(s1, set)  avec s1 = %s et set = \" \", chacun " GUARD_NOTE, q(t[i]));
		char	*r = FT(ft_strtrim(s, set));
		TEST(r && strcmp(r, e[i]) == 0, q(e[i]), q(r), NO_OVERREAD);
		ro_check(s, t[i], n, "s1");
		ro_check(set, " ", 2, "set");
		free(r);
	}
}

static void	extra_split(void)
{
	static char	big[3001];
	char		*s;

	{
		s = guard("ab cd", 6);
		CASE("ft_split(s, ' ')  avec s = \"ab cd\" " GUARD_NOTE " (entree const)");
		char	**r = FT(ft_split(s, ' '));
		ro_check(s, "ab cd", 6, "s");
		free_tab(r);
	}
	for (int i = 0; i < 1000; i++)
		memcpy(big + 3 * i, "ab ", 3);
	big[3000] = '\0';
	CASE("ft_split(s, ' ')  avec s = 1000 fois \"ab \" (1000 mots)");
	char	**r = FT(ft_split(big, ' '));
	int		n = 0;
	int		ok = (r != NULL);

	while (ok && n <= 1000 && r[n])
	{
		if (strcmp(r[n], "ab") != 0)
			ok = 0;
		n++;
	}
	TEST(ok && n == 1000, "1000 mots \"ab\", puis NULL", r ? f("%d mot(s)%s", n, ok ? ""
		: ", dont un different de \"ab\"") : "NULL", "");
	if (r)
		check_size(r, 1001 * sizeof(char *), "tableau de 1001 pointeurs");
	free_tab(r);
}

static void	extra_strmapi(void)
{
	char	*s = guard("abc", 4);

	CASE("ft_strmapi(s, f)  avec s = \"abc\" " GUARD_NOTE " et f(i, c) = c + i");
	char	*r = FT(ft_strmapi(s, map_add_index));
	TEST(r && strcmp(r, "ace") == 0, "\"ace\"", q(r), NO_OVERREAD);
	ro_check(s, "abc", 4, "s");
	free(r);
}

static void	extra_striteri(void)
{
	char	*s = guard("abc", 4);

	g_ncalls = 0;
	CASE("ft_striteri(s, f)  avec s = \"abc\" " GUARD_NOTE " et f : *c = *c + 1");
	FTV(ft_striteri(s, iter_record));
	TEST(memcmp(s, "bcd", 4) == 0 && g_ncalls == 3, "s = \"bcd\", f appelee 3 fois",
		f("s = %s, f appelee %d fois", mem(s, 4), g_ncalls), NO_OVERREAD);
}

static void	extra_put(int endl)
{
	char	*s = guard("hi", 3);
	int		fd = capture_start();

	CASE("ft_%s(s, fd)  avec s = \"hi\" " GUARD_NOTE, endl ? "putendl_fd" : "putstr_fd");
	if (endl)
		FTV(ft_putendl_fd(s, fd));
	else
		FTV(ft_putstr_fd(s, fd));
	capture_end();
	check_output(endl ? "hi\n" : "hi", endl ? 3 : 2);
	ro_check(s, "hi", 3, "s");
}

static void	test_strlen_all(void) { test_strlen(); extra_strlen(); }
static void	test_chr_all(void) { test_chr(); extra_chr(); }
static void	test_strdup_all(void) { test_strdup(); extra_strdup(); }
static void	test_strlcpy_all(void) { test_strlcpy(); extra_strlcpy(); }
static void	test_strlcat_all(void) { test_strlcat(); extra_strlcat(); }
static void	test_atoi_all(void) { test_atoi(); extra_atoi(); }
static void	test_memmove_all(void) { test_memmove(); extra_memmove(); }
static void	test_strnstr_all(void) { test_strnstr(); extra_strnstr(); }
static void	test_substr_all(void) { test_substr(); extra_substr(); }
static void	test_strjoin_all(void) { test_strjoin(); extra_strjoin(); }
static void	test_strtrim_all(void) { test_strtrim(); extra_strtrim(); }
static void	test_split_all(void) { test_split(); extra_split(); }
static void	test_strmapi_all(void) { test_strmapi(); extra_strmapi(); }
static void	test_striteri_all(void) { test_striteri(); extra_striteri(); }
static void	test_putstr_fd_all(void) { test_putstr_fd(); extra_put(0); }
static void	test_putendl_fd_all(void) { test_putendl_fd(); extra_put(1); }

/* ========================================================================= */
/*  PARTIE 3 : listes chainees                                               */
/*  Les contents des listes de test sont des entiers : la liste              */
/*  0 -> 1 -> 2 -> NULL a trois noeuds dont les contents valent 0, 1 et 2.   */
/* ========================================================================= */

static int		g_mapped[64];
static void		*g_f_args[256];
static int		g_nf;
static void		*g_del_args[256];
static int		g_ndel;

static void	rec_del(void *p)
{
	if (g_ndel < 256)
		g_del_args[g_ndel] = p;
	g_ndel++;
}

static void	rec_iter(void *p)
{
	if (g_nf < 256)
		g_f_args[g_nf] = p;
	g_nf++;
}

/* f de lstmap : x -> x + 100 (renvoie un nouveau pointeur) */
static void	*rec_map(void *p)
{
	int	k;

	rec_iter(p);
	if ((char *)p < (char *)g_vals || (char *)p >= (char *)(g_vals + 64))
		return (NULL);
	k = (int *)p - g_vals;
	g_mapped[k] = g_vals[k] + 100;
	return (&g_mapped[k]);
}

static int	is_mapped(const void *p)
{
	return ((char *)p >= (char *)g_mapped && (char *)p < (char *)(g_mapped + 64));
}

static char	*cdesc(const void *p)
{
	if (!p)
		return ("NULL");
	if ((char *)p >= (char *)g_vals && (char *)p < (char *)(g_vals + 64))
		return (f("%d", *(const int *)p));
	if (is_mapped(p))
		return (f("%d", *(const int *)p));
	return ("?");
}

/* noeud cree par le testeur ; v < 0 : content = NULL */
static t_list	*mk_node(int v)
{
	t_list	*n = __libc_malloc(sizeof(t_list));

	if (v >= 0)
		g_vals[v] = v;
	n->content = (v >= 0) ? &g_vals[v] : NULL;
	n->next = NULL;
	if (g_nnodes < MAX_NODES)
		g_nodes[g_nnodes++] = n;
	return (n);
}

/* liste de n noeuds de contents from, from + 1, ... (from < 0 : contents NULL) */
static t_list	*mk_list(int from, int n)
{
	t_list	*head = NULL;
	t_list	**p = &head;

	for (int i = 0; i < n; i++)
	{
		*p = mk_node(from < 0 ? -1 : from + i);
		p = &(*p)->next;
	}
	return (head);
}

static int	was_freed(const void *node)
{
	int	k = node_index(node);

	return (k >= 0 && g_node_freed[k]);
}

static char	*list_str(const t_list *l)
{
	char	buf[400];
	size_t	j = 0;
	int		n = 0;

	while (l && n < 20)
	{
		j += snprintf(buf + j, sizeof(buf) - j, "%s -> ", cdesc(l->content));
		l = l->next;
		n++;
	}
	snprintf(buf + j, sizeof(buf) - j, "%s", l ? "... (boucle ?)" : "NULL");
	return (f("%s", buf));
}

/* position d'un noeud dans la liste (ou -1) */
static int	node_pos(const t_list *l, const t_list *p)
{
	for (int i = 0; l && i < 2000; i++, l = l->next)
		if (l == p)
			return (i);
	return (-1);
}

/* verifie que la fonction a ete appelee n fois, sur from, from + 1... dans l'ordre */
static void	check_calls(const char *who, void **args, int got, int from, int n,
	const char *hint)
{
	char	e[200] = "";
	char	g2[200] = "";
	size_t	j = 0;
	int		ok = (got == n);

	for (int k = 0; k < n; k++)
	{
		j += snprintf(e + j, sizeof(e) - j, "%s%d", k ? ", " : "", from + k);
		if (ok && args[k] != &g_vals[from + k])
			ok = 0;
	}
	j = 0;
	for (int k = 0; k < got && k < 20; k++)
		j += snprintf(g2 + j, sizeof(g2) - j, "%s%s", k ? ", " : "", cdesc(args[k]));
	TEST(ok, n ? f("%s appelee %d fois, sur %s dans l'ordre", who, n, e)
		: f("%s jamais appelee", who),
		got ? f("%s appelee %d fois, sur %s", who, got, g2) : f("%s jamais appelee", who),
		hint);
}

static void	free_mapped(t_list *r)
{
	t_list	*nx;

	for (int i = 0; r && i < 100; i++)
	{
		nx = r->next;
		free(r);
		r = nx;
	}
}

/* ----------------------------- lstnew ----------------------------------- */

static void	*mf_lstnew(void)
{
	return (FT(ft_lstnew(&g_vals[3])));
}

static void	test_lstnew(void)
{
	void	*cs[] = {&g_vals[3], NULL};

	g_vals[3] = 3;
	for (int i = 0; i < 2; i++)
	{
		CASE("ft_lstnew(%s)", cs[i] ? "content" : "NULL");
		t_list	*r = FT(ft_lstnew(cs[i]));
		TEST(r != NULL, "un nouveau noeud", "NULL", "");
		if (!r)
			continue ;
		TEST(r->content == cs[i], cs[i] ? "node->content = content" : "node->content = NULL",
			r->content ? "node->content = un autre pointeur" : "node->content = NULL",
			"il faut stocker le pointeur recu tel quel (pas une copie)");
		TEST(r->next == NULL, "node->next = NULL", "node->next pas a NULL",
			"next doit etre initialise a NULL");
		check_size(r, sizeof(t_list), "noeud");
		free(r);
	}
	malloc_fail("ft_lstnew(content)", mf_lstnew, release_str);
}

/* ----------------------------- lstadd_front / lstadd_back --------------- */

static void	test_add(int back)
{
	const char	*name = back ? "ft_lstadd_back" : "ft_lstadd_front";
	struct { int from; int n; int nfrom; int nn; const char *e; const char *hint; } t[] = {
		{0, 0, 10, 1, "10 -> NULL", "liste vide (*lst == NULL) : il faut faire *lst = new"},
		{0, 1, 10, 1, back ? "0 -> 10 -> NULL" : "10 -> 0 -> NULL", ""},
		{0, 3, 10, 1, back ? "0 -> 1 -> 2 -> 10 -> NULL" : "10 -> 0 -> 1 -> 2 -> NULL",
			back ? "il faut aller jusqu'au dernier noeud, puis last->next = new"
			: "il faut faire new->next = *lst, puis *lst = new"},
		{0, 2, 10, 2, back ? "0 -> 1 -> 10 -> 11 -> NULL" : NULL,
			"new a deja un noeud apres lui : il faut garder new->next"},
	};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		t_list	*lst;
		t_list	*n;
		t_list	*head;

		if (!t[i].e)
			continue ;
		lst = mk_list(t[i].from, t[i].n);
		n = mk_list(t[i].nfrom, t[i].nn);
		head = lst;
		CASE("%s(&lst, new)  avec lst = %s, new = %s", name, list_str(lst), list_str(n));
		g_nalloc = 0;
		if (back)
			FTV(ft_lstadd_back(&lst, n));
		else
			FTV(ft_lstadd_front(&lst, n));
		TEST(strcmp(list_str(lst), t[i].e) == 0 && lst == (back && head ? head : n),
			f("lst = %s", t[i].e), f("lst = %s", list_str(lst)), t[i].hint);
		TEST(g_nalloc == 0, "aucun malloc", f("%d malloc", g_nalloc),
			"il faut ajouter le noeud recu, pas en creer un nouveau");
	}
}

static void	test_lstadd_front(void)
{
	test_add(0);
}

static void	test_lstadd_back(void)
{
	test_add(1);
}

/* ----------------------------- lstsize / lstlast ------------------------ */

static void	test_lstsize(void)
{
	int	t[] = {0, 1, 2, 5, 1000};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		t_list	*l = mk_list(t[i] > 5 ? -1 : 0, t[i]);

		CASE("ft_lstsize(lst)  avec lst = %s", t[i] > 5 ? "1000 noeuds" : list_str(l));
		g_nalloc = 0;
		int	r = FT(ft_lstsize(l));
		TEST(r == t[i], f("%d", t[i]), f("%d", r), t[i] == 0 ? "liste vide (NULL) : 0" : "");
		TEST(g_nalloc == 0, "aucun malloc", f("%d malloc", g_nalloc), "");
	}
}

static void	test_lstlast(void)
{
	int	t[] = {0, 1, 2, 5};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		t_list	*l = mk_list(0, t[i]);
		t_list	*e = l;
		int		pos;

		while (e && e->next)
			e = e->next;
		CASE("ft_lstlast(lst)  avec lst = %s", list_str(l));
		t_list	*r = FT(ft_lstlast(l));
		pos = node_pos(l, r);
		TEST(r == e, e ? f("le noeud %s (le dernier)", cdesc(e->content)) : "NULL",
			!r ? "NULL" : pos < 0 ? "un pointeur qui n'est pas dans la liste"
			: f("le noeud %s", cdesc(r->content)),
			t[i] == 0 ? "liste vide : il faut renvoyer NULL" : "le dernier noeud est celui dont next vaut NULL");
	}
}

/* ----------------------------- lstdelone / lstclear --------------------- */

static void	test_lstdelone(void)
{
	t_list	*l = mk_list(0, 3);
	t_list	*second = l->next;
	t_list	*third = second->next;

	CASE("ft_lstdelone(lst, del)  avec lst = 0 -> 1 -> 2 -> NULL (supprime le noeud 0)");
	g_ndel = 0;
	g_bad_free = 0;
	g_nalloc = 0;
	FTV(ft_lstdelone(l, rec_del));
	TEST(g_ndel == 1 && g_del_args[0] == &g_vals[0], "del appelee 1 fois, sur le content 0",
		g_ndel ? f("del appelee %d fois, la 1re sur %s", g_ndel, cdesc(g_del_args[0]))
		: "del jamais appelee", "il faut appeler del(lst->content) une seule fois");
	TEST(g_bad_free == 0, "le content libere seulement par del", "free appele sur le content",
		"c'est del qui s'occupe du content : pas de free(lst->content)");
	TEST(was_freed(l), "le noeud libere (free(lst))", "le noeud n'est pas libere",
		"apres del(lst->content), il faut free(lst)");
	TEST(!was_freed(second) && !was_freed(third), "les noeuds 1 et 2 intacts",
		"un noeud suivant a ete libere", "lstdelone ne supprime QUE le noeud donne");
	TEST(g_nalloc == 0, "aucun malloc", f("%d malloc", g_nalloc), "");
}

static void	test_lstclear(void)
{
	int	sizes[] = {5, 1};

	for (size_t t = 0; t < 2; t++)
	{
		int		n = sizes[t];
		t_list	*l = mk_list(0, n);
		t_list	*nodes[8];
		t_list	*p = l;
		int		nfreed = 0;
		int		del_ok = (1);

		for (int i = 0; i < n; i++, p = p->next)
			nodes[i] = p;
		CASE("ft_lstclear(&lst, del)  avec lst = %s", list_str(l));
		g_ndel = 0;
		g_bad_free = 0;
		FTV(ft_lstclear(&l, rec_del));
		TEST(l == NULL, "lst = NULL a la fin", "lst n'est pas remis a NULL",
			"apres avoir tout libere, il faut faire *lst = NULL");
		for (int i = 0; i < n; i++)
		{
			int	cnt = 0;

			for (int k = 0; k < g_ndel && k < 256; k++)
				if (g_del_args[k] == &g_vals[i])
					cnt++;
			if (cnt != 1)
				del_ok = 0;
			if (was_freed(nodes[i]))
				nfreed++;
		}
		TEST(del_ok && g_ndel == n, f("del appelee %d fois, une fois par content", n),
			f("del appelee %d fois", g_ndel), "il faut appeler del sur le content de chaque noeud");
		TEST(nfreed == n, f("les %d noeuds liberes", n), f("%d noeud(s) libere(s) sur %d", nfreed, n),
			"chaque noeud doit etre libere (free), pas seulement son content");
		TEST(g_bad_free == 0, "les contents liberes seulement par del", "free appele sur un content",
			"c'est del qui s'occupe du content : pas de free(content)");
	}
	{
		t_list	*l = NULL;

		CASE("ft_lstclear(&lst, del)  avec lst = NULL");
		g_ndel = 0;
		FTV(ft_lstclear(&l, rec_del));
		TEST(l == NULL && g_ndel == 0, "rien ne se passe", f("del appelee %d fois", g_ndel),
			"liste vide : il n'y a rien a liberer");
	}
}

/* ----------------------------- lstiter ---------------------------------- */

static void	test_lstiter(void)
{
	int	t[] = {5, 1, 0};

	for (size_t i = 0; i < sizeof(t) / sizeof(*t); i++)
	{
		t_list		*l = mk_list(0, t[i]);
		const char	*before = list_str(l);
		char		saved[400];

		snprintf(saved, sizeof(saved), "%s", before);
		CASE("ft_lstiter(lst, f)  avec lst = %s", saved);
		g_nf = 0;
		g_nalloc = 0;
		FTV(ft_lstiter(l, rec_iter));
		check_calls("f", g_f_args, g_nf, 0, t[i],
			"f doit etre appelee sur le content de chaque noeud, du premier au dernier");
		TEST(strcmp(list_str(l), saved) == 0, f("lst inchangee : %s", saved),
			f("lst = %s", list_str(l)), "lstiter ne doit pas modifier la liste");
		TEST(g_nalloc == 0, "aucun malloc", f("%d malloc", g_nalloc), "");
	}
}

/* ----------------------------- lstmap ----------------------------------- */

static void	test_lstmap(void)
{
	t_list		*l = mk_list(0, 4);
	const char	*orig = "0 -> 1 -> 2 -> 3 -> NULL";
	int			n_alloc;
	t_list		*r;

	CASE("ft_lstmap(lst, f, del)  avec lst = %s et f(x) = x + 100", orig);
	g_nf = 0;
	g_ndel = 0;
	g_nalloc = 0;
	r = FT(ft_lstmap(l, rec_map, rec_del));
	n_alloc = g_nalloc;
	TEST(strcmp(list_str(r), "100 -> 101 -> 102 -> 103 -> NULL") == 0,
		"100 -> 101 -> 102 -> 103 -> NULL", list_str(r), "");
	TEST(strcmp(list_str(l), orig) == 0, f("lst d'origine inchangee : %s", orig),
		f("lst d'origine = %s", list_str(l)),
		"lstmap cree une NOUVELLE liste, sans modifier l'ancienne");
	check_calls("f", g_f_args, g_nf, 0, 4, "f doit etre appelee une fois par noeud, dans l'ordre");
	TEST(g_ndel == 0, "del jamais appelee", f("del appelee %d fois", g_ndel),
		"del ne sert qu'en cas d'echec d'un malloc");
	{
		t_list	*p = r;

		for (int k = 0; p && k < 4; k++, p = p->next)
		{
			TEST(node_pos(l, p) < 0, f("noeud %d : un nouveau noeud", k),
				f("noeud %d : un noeud de la liste d'origine", k),
				"chaque noeud de la nouvelle liste doit etre cree avec malloc");
			check_size(p, sizeof(t_list), f("noeud %d", k));
		}
	}
	if (node_pos(l, r) < 0)
		free_mapped(r);
	{
		CASE("ft_lstmap(NULL, f, del)");
		g_nf = 0;
		r = FT(ft_lstmap(NULL, rec_map, rec_del));
		TEST(r == NULL && g_nf == 0, "NULL, sans appeler f", r ? "un pointeur non NULL"
			: f("NULL, mais f appelee %d fois", g_nf), "liste vide : nouvelle liste vide (NULL)");
	}
	for (int k = 1; k <= n_alloc; k++)
	{
		int	live0 = g_live;
		int	del_ok;

		CASE("ft_lstmap(lst, f, del)  avec lst = %s et le malloc n.%d (sur %d) qui echoue",
			orig, k, n_alloc);
		g_nf = 0;
		g_ndel = 0;
		g_fail = k;
		r = FT(ft_lstmap(l, rec_map, rec_del));
		g_fail = 0;
		TEST(r == NULL, "NULL", list_str(r), "si un malloc echoue, lstmap doit renvoyer NULL");
		if (r && node_pos(l, r) < 0)
			free_mapped(r);
		TEST(g_live == live0, "aucun noeud oublie", f("%d noeud(s) jamais libere(s)", g_live - live0),
			"avant de renvoyer NULL, il faut liberer les noeuds deja crees (ft_lstclear)");
		del_ok = (g_ndel == g_nf);
		for (int j = 0; j < g_ndel && j < 256; j++)
			if (!is_mapped(g_del_args[j]))
				del_ok = 0;
		TEST(del_ok, f("del appelee sur les %d resultat(s) de f", g_nf),
			f("del appelee %d fois pour %d appel(s) a f", g_ndel, g_nf),
			"chaque resultat de f doit passer par del, y compris celui du noeud dont le"
			" malloc a echoue (et jamais un content de la liste d'origine)");
		TEST(strcmp(list_str(l), orig) == 0, f("lst d'origine intacte : %s", orig),
			f("lst d'origine = %s", list_str(l)), "en cas d'echec, la liste d'origine ne doit pas bouger");
	}
}

/* ========================================================================= */

int	main(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	init_colors();
	g = mmap(NULL, sizeof(*g), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (g == MAP_FAILED)
		return (perror("mmap"), 1);
	g_cur_case = g->current;
	printf("%sLes buffers \"xxxx...\" sont pre-remplis de 'x' pour voir si ta fonction"
		" ecrit trop loin.\nLes octets non affichables sont ecrits en octal (\\310 = 200, \\033 = 27).%s\n\n",
		GREY, RESET);
	run_is("isalpha", ft_isalpha, isalpha);
	run_is("isdigit", ft_isdigit, isdigit);
	run_is("isalnum", ft_isalnum, isalnum);
	run_is("isascii", ft_isascii, ref_isascii);
	run_is("isprint", ft_isprint, isprint);
	run("strlen", ft_strlen != NULL, test_strlen_all);
	run("memset", ft_memset != NULL, test_memset);
	run("bzero", ft_bzero != NULL, test_bzero);
	run("memcpy", ft_memcpy != NULL, test_memcpy);
	run("memmove", ft_memmove != NULL, test_memmove_all);
	run("strlcpy", ft_strlcpy != NULL, test_strlcpy_all);
	run("strlcat", ft_strlcat != NULL, test_strlcat_all);
	run_to("toupper", ft_toupper, toupper);
	run_to("tolower", ft_tolower, tolower);
	run_chr("strchr", ft_strchr, strchr);
	run_chr("strrchr", ft_strrchr, strrchr);
	run("strncmp", ft_strncmp != NULL, test_strncmp);
	run("memchr", ft_memchr != NULL, test_memchr);
	run("memcmp", ft_memcmp != NULL, test_memcmp);
	run("strnstr", ft_strnstr != NULL, test_strnstr_all);
	run("atoi", ft_atoi != NULL, test_atoi_all);
	run("calloc", ft_calloc != NULL, test_calloc);
	run("strdup", ft_strdup != NULL, test_strdup_all);
	printf("\n%s--- Partie 2 ---%s\n\n", BOLD, RESET);
	run("substr", ft_substr != NULL, test_substr_all);
	run("strjoin", ft_strjoin != NULL, test_strjoin_all);
	run("strtrim", ft_strtrim != NULL, test_strtrim_all);
	run("split", ft_split != NULL, test_split_all);
	run("itoa", ft_itoa != NULL, test_itoa);
	run("strmapi", ft_strmapi != NULL, test_strmapi_all);
	run("striteri", ft_striteri != NULL, test_striteri_all);
	run("putchar_fd", ft_putchar_fd != NULL, test_putchar_fd);
	run("putstr_fd", ft_putstr_fd != NULL, test_putstr_fd_all);
	run("putendl_fd", ft_putendl_fd != NULL, test_putendl_fd_all);
	run("putnbr_fd", ft_putnbr_fd != NULL, test_putnbr_fd);
	printf("\n%s--- Partie 3 ---%s\n\n", BOLD, RESET);
	run("lstnew", ft_lstnew != NULL, test_lstnew);
	run("lstadd_front", ft_lstadd_front != NULL, test_lstadd_front);
	run("lstsize", ft_lstsize != NULL, test_lstsize);
	run("lstlast", ft_lstlast != NULL, test_lstlast);
	run("lstadd_back", ft_lstadd_back != NULL, test_lstadd_back);
	run("lstdelone", ft_lstdelone != NULL, test_lstdelone);
	run("lstclear", ft_lstclear != NULL, test_lstclear);
	run("lstiter", ft_lstiter != NULL, test_lstiter);
	run("lstmap", ft_lstmap != NULL, test_lstmap);
	printf("\n%s%d OK%s, %s%d KO%s, %d fonction(s) pas encore ecrite(s)\n",
		GREEN, g->ok, RESET, g->ko ? RED : "", g->ko, RESET, g_skip);
	return (g->ko != 0);
}
