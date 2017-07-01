#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <locale.h>

typedef struct perprincipal {
    char nome [50];
    char sublocal [50];
    int hp;
    int mhp;
    int armor;
    int weapon;
    char wname[20];
} personagemprinc;

typedef struct persecundario {
    char nome [50];
    char sublocal [50];
    char frase [2000];
} personagemsec;

run, damage, valid, next, ehp, n, i;

struct enemystruct {
    char name [15];
    int hp1;
    int ap;
    int wh;
    int run;
};

struct enemystruct e[3] = {{"Cobra", 50, 5, 10, 1},
                           {"Pantera", 50, 25, 30, 0},
                           {"Javali", 30, 15, 20, 1}};

struct enemystruct e2[2] = {{"Ghoul", 30, 10, 50, 0},
                            {"Zumbi", 20, 5, 10, 1}};

struct enemystruct e3[2] = {{"Cobra",50,5,10,1},
                            {"Salamandra", 30, 4, 75, 0}};

struct enemystruct e4[2] = {{"Lobo", 30, 10, 30, 0},
                            {"Aranha Gigante", 20, 5, 25, 1}};

struct enemystruct e5[2] = {{"Necromancer", 120, 30, 50, 1},
                            {"Oslun", 35, 40, 5, 1}};

struct enemystruct chefe[5] = {{"Rei Uzgal", 600, 33, 33, 0},
                               {"Iklaun", 5000, 5, 40, 0},
                               {"Nazeru", 2200, 133, 340, 0},
                               {"Minasaer", 1100, 80, 50, 0},
                               {"Guardião", 1784, 192, 531, 0}};

personagemprinc *criaprincipal(char *nome);
personagemsec *criasecundario(char *nome, char *sublocal, char *frase);
personagemsec **criasecundarios(void);
void limpaTela(void);
void readkey(void);
void mostrahistoria();
int existeSublocal(personagemsec **secundarios, char *sublocal);
void visitaLocal(personagemprinc *principal,personagemsec **secundarios);
void visitaPersonagem(personagemprinc *principal, personagemsec **secundarios);
int fazTeste(void);
void mostramapa(void);
void soundtrack(void);
void janelainicial(void);
void carregarimagem(personagemprinc *principal);
int escolheimagem(personagemprinc *principal);
char* LeNome();
void Luta(int m,personagemprinc *principal);
void escolhemenu(int i, personagemprinc *principal, personagemsec **secundarios);
personagemprinc* atributo(personagemprinc *principal);
void carregarMonstro(int i);
int escolhemonstro(personagemprinc *principal);
void mostrafim(char *nomeArq);

void main ()
{
    system("TITLE Ilha da Ilusao");
    system("color 47");
    setlocale(LC_ALL,"portuguese");

    janelainicial();

    srand(time(NULL));

    personagemprinc *p;
    personagemsec **vet;

    char *nome = LeNome();
    char op;
    int resposta = 0, ct = 0;

    p = criaprincipal(nome);
    vet = criasecundarios();

    limpaTela();
    mostrahistoria("files\\introduction.txt");

    while ((ct < 2) && (resposta == 0)) {
        limpaTela();
        printf("Local: %s \n", p->sublocal);
        printf("Nome: %s \n", p->nome);
        printf("\n");
        printf("Digite o número da opcão desejada: \n");
        printf("\n");
        printf("1) Ir para outro local \n");
        printf("2) Ver Mapa \n");
        printf("3) Ver Local \n");
        scanf(" %c", &op);

        switch (op) {
            case '1':
                limpaTela();
                visitaLocal(p, vet);
                visitaPersonagem(p, vet);
                i = escolhemonstro(p);
                Luta(i, p);
                if (i == 15) ct++;
                break;

            case '2':
                mostramapa();
                break;

            case '3':
                carregarimagem(p);
        }

        if (ct) {
            limpaTela();
            mostrafim("images\\texto_final.txt");
            exit(0);
        }
    }
}

personagemprinc *criaprincipal(char *nome)
{
    personagemprinc *pon;
    pon = (personagemprinc*) malloc(sizeof(personagemprinc));
    strcpy(pon->nome, nome);
    strcpy(pon->sublocal, "Praia");
    pon->hp = 100;
    pon->mhp = 100;
    pon->armor = 20;
    pon->weapon = 25;
    strcpy(pon->wname, "Punho");

    return pon;
}

personagemsec * criasecundario(char *nome, char *sublocal, char *frase)
{
    personagemsec *ptr;
    ptr = (personagemsec*)malloc(sizeof(personagemsec));
    strcpy(ptr->nome, nome);
    strcpy(ptr->sublocal, sublocal);
    strcpy(ptr->frase, frase);

    return ptr;
}

personagemsec **criasecundarios()
{
    personagemsec **vets;
    vets = (personagemsec**) malloc(11 * sizeof(personagemsec));

    vets[5] = criasecundario("Iklaun", "Floresta Assombrada", "Iklaun: Quem ousa me pertubar!");
    vets[4] = criasecundario("Minasaer", "Selva", "Minasaer: HAHAHAHAHAHAHAHAH... ");
    vets[1] = criasecundario("Thrimin", "Floresta", "Thrimin: Viajante você parece precisar de ajuda, certo? Pense bem, muitos ja perderam suas vidas andando por essa floresta. Quanto menos tempo passar nela, melhor!");
    vets[0] = criasecundario("Geoffry","Praia","Geoffry: Você tá legal?");
    vets[6] = criasecundario("Orund","Vulcão","Orund: Monstros, monstros e mais monstros... que lugar infernal!");
    vets[7] = criasecundario("Nazeru","Tubos de Lava","Nazeru: Medo...");
    vets[2] = criasecundario("Uzgal","Caverna de Corais","Rei Uzgal: Sinto cheiro de carne...");
    vets[3] = criasecundario("Mesym", "Campos da Ilusão","Mesym: A quanto tempo não vejo um viajante perdido por aqui...");
    vets[8] = criasecundario("Guardião","Templo Perdido","Guardião: Seja bem-vindo!");

    return vets;
}

void limpaTela()
{
    system("cls");
}

void readkey()
{
	puts("\n\n");
	system("pause");
}

int existeSublocal(personagemsec **secundarios, char *sublocal)
{
    int i = 0;

    do {
        if (strcmp(secundarios[i]->sublocal, sublocal) == 0) return 1;
        i++;
    } while (i < 10);

    return 0;
}

void visitaLocal(personagemprinc *principal,personagemsec **secundarios)
{
    int i, resposta = 0;
    personagemsec **vets = secundarios;

    do {
        printf("insira o numero do lugar que voce quer ir: \n");
        printf("1 - Praia \n");
        printf("2 - Floresta \n");
        printf("3 - Caverna de Corais \n");
        printf("4 - Campos da Ilusão \n");
        printf("5 - Selva \n");
        printf("6 - Floresta Assombrada\n");
        printf("7 - Vulcao \n");
        printf("8 - Tubos Vulcanicos \n");
        printf("9 - Templo Perdido \n");
        scanf("%d",&i);

        switch (i) {
            case 1:
                resposta = existeSublocal(vets, "Praia");
                if (resposta == 1) strcpy(principal->sublocal, "Praia");
                break;

            case 2:
                resposta = existeSublocal(vets,"Floresta");
                if(resposta == 1) strcpy(principal->sublocal,"Floresta");
                break;

            case 3:
                resposta = existeSublocal(vets,"Caverna de Corais");
                if(resposta == 1) strcpy(principal->sublocal,"Caverna de Corais");
                break;

            case 4:
                resposta = existeSublocal(vets,"Campos da Ilusão");
                if(resposta == 1) strcpy(principal->sublocal,"Campos da Ilusão");
                break;

            case 5:
                resposta = existeSublocal(vets,"Selva");
                if(resposta == 1) strcpy(principal->sublocal,"Selva");
                break;

            case 6:
                resposta = existeSublocal(vets,"Floresta Assombrada");
                if(resposta == 1) strcpy(principal->sublocal,"Floresta Assombrada");
                break;

            case 7:
                resposta = existeSublocal(vets,"Vulcão");
                if(resposta == 1) strcpy(principal->sublocal,"Vulcão");
                break;

            case 8:
                resposta = existeSublocal(vets,"Tubos de Lava");
                if(resposta == 1) strcpy(principal->sublocal,"Tubos de Lava");
                break;

            case 9:
                resposta = existeSublocal(vets,"Templo Perdido");
                if(resposta == 1) strcpy(principal->sublocal,"Templo Perdido");
                break;

            default:
                printf("Digite novamente");
        }
    } while (i < 1 || i > 9);

    limpaTela();
}

void visitaPersonagem(personagemprinc *principal, personagemsec **secundarios)
{
    int i;
    char escolhido[81];

    printf("Digite o nome do personagem, caso queira falar com ele: \n");

    for (i = 0; i < 9; i++)
        if (strcmp(secundarios[i]->sublocal, principal->sublocal) == 0)
            printf("%s\n",secundarios[i]->nome);

    scanf("%s", escolhido);

    limpaTela();

    for (i = 0; i < 9; i++) {
        if (strcmp(secundarios[i]->nome, escolhido) == 0) {
            int k;

            printf(" %s \n ",secundarios[i]->frase);

            fflush(stdin);

            readkey();
            limpaTela();

            puts("Deseja continuar dialogo:");
            puts("\n1) Sim");
            puts("2) Não\n");

            scanf("%d",&k);

            if (k == 1) {
                fflush(stdin);
                limpaTela();
                LeDialogo(principal);
                principal = atributo(principal);

                switch (i) {
                    case 0:
                        puts("Você recebeu Faca de Madeira");
                        break;

                    case 1:
                        puts("Você recebeu Espada Antiga");
                        break;

                    case 2:
                        puts("Você recebeu Faca de Prata");
                        break;

                    case 3:
                        puts("Você recebeu Espada Militar");
                        break;

                    case 4:
                        puts("Você recebeu Sabre Heroíco");
                        break;

                    case 5:
                        puts("Você recebeu Espada Amaldiçoada");
                        break;

                    case 6:
                        puts("Você recebeu Espada Fantasma");
                        break;

                    case 7:
                        puts("Você recebeu Vingança Dourada");
                        break;


                }

                readkey();
            }
        }
    }

    limpaTela();
}

void mostrahistoria(char *nomeArq)
{
    FILE *fp;
    char linha[1000];
    if ((fp = fopen("files\\introduction.txt", "rt")) == NULL) {
        printf("Erro \n");
        exit(1);
    }
    while (!feof(fp)) {
        fscanf(fp, " %[^\n]", linha);
        printf("%s", linha);
        readkey();
        fflush(stdin);
        limpaTela();
    }
    fclose(fp);
}

void mostramapa()
{
    const int LARGURA_TELA = 620;
    const int ALTURA_TELA = 500;
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_BITMAP *imagem = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_AUDIO_STREAM *musica = NULL;
    ALLEGRO_SAMPLE *sample = NULL;

    if (!al_init())
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");

    if (!al_install_audio()) {
        fprintf(stderr, "Falha ao inicializar áudio.\n");
        return false;
    }

    if (!al_init_acodec_addon()) {
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
        return false;
    }

    if (!al_reserve_samples(1)) {
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        return false;
    }

    musica = al_load_audio_stream("songs\\mapa.ogg", 4, 1024);
    if (!musica) {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
    }

    if (!al_init_image_addon())
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
        fprintf(stderr, "Falha ao criar janela.\n");

    imagem = al_load_bitmap("images\\mapa.jpg");
    if (!imagem) {
        fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
        al_destroy_display(janela);
    }

    fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
    }
    al_set_window_title(janela, "Ilha da Ilusao");

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_draw_bitmap(imagem, 0, 0, 0);
    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    al_set_audio_stream_playing(musica, true);
    al_flip_display();

    while (1) {
        ALLEGRO_EVENT evento;
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, 0.05);

        int tem_eventos = al_wait_for_event_until(fila_eventos, &evento, &timeout);

        if ((tem_eventos && evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE))
            break;

        al_draw_bitmap(imagem, 0, 0, 0);
        al_flip_display();
    }

    al_destroy_audio_stream(musica);
    al_destroy_sample(sample);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
}

const int LARGURA_TELA = 800;
const int ALTURA_TELA = 600;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_AUDIO_STREAM *musica = NULL;
ALLEGRO_SAMPLE *sample = NULL;

bool inicializar();

void janelainicial(void)
{
    bool sair = false;
    int tecla = 0;

    if (!inicializar())
        return -1;

    al_draw_bitmap(fundo, 0, 0, 0);
    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    al_set_audio_stream_playing(musica, true);

    while (!sair) {
        while(!al_is_event_queue_empty(fila_eventos)) {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(evento.keyboard.keycode)
                {
                case ALLEGRO_KEY_ENTER:
                    tecla = 1;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    tecla = 2;
                    break;
                }
            }
            else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                sair = true;
            }
        }

        if (tecla)
        {
            al_draw_bitmap(fundo, 0, 0, 0);

            if(tecla == 1)
            {
               sair = true;
            }
            if(tecla == 2)
            {
                exit(0);
                al_destroy_audio_stream(musica);
                al_destroy_sample(sample);
                al_destroy_display(janela);
                al_destroy_event_queue(fila_eventos);
            }
        }

        al_flip_display();
    }

    al_destroy_audio_stream(musica);
    al_destroy_sample(sample);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
}

bool inicializar()
{
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return false;
    }

    if (!al_install_audio()) {
        fprintf(stderr, "Falha ao inicializar áudio.\n");
        return false;
    }

    if (!al_init_acodec_addon()) {
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
        return false;
    }

    if (!al_reserve_samples(1)) {
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        return false;
    }

    musica = al_load_audio_stream("songs\\title.ogg", 4, 1024);
    if (!musica) {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
    }

    if (!al_init_image_addon()) {
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return false;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela) {
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }
    al_set_window_title(janela, "Ilha da Ilusao");

    fila_eventos = al_create_event_queue();
    if (!fila_eventos) {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return false;
    }

    fundo = al_load_bitmap("images\\title.jpg");
    if (!fundo) {
        fprintf(stderr, "Falha ao carregar imagem de fundo.\n");
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return false;
    }

    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

    return true;
}

void carregarimagem(personagemprinc *principal)
{
    ALLEGRO_BITMAP *imagem = NULL;
    ALLEGRO_SAMPLE *sample = NULL;
    ALLEGRO_AUDIO_STREAM *musica = NULL;
    ALLEGRO_DISPLAY *janela = al_create_display(800, 600);
    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();

    switch (escolheimagem(principal)) {
        case 1:
            imagem = al_load_bitmap("images\\praia.jpg");
            musica = al_load_audio_stream("songs\\praia.ogg", 4, 1024);
            break;

        case 2:
            imagem = al_load_bitmap("images\\floresta.jpg");
            musica = al_load_audio_stream("songs\\floresta.ogg", 4, 1024);
            break;

        case 3:
            imagem = al_load_bitmap("images\\caverna_corais.png");
            musica = al_load_audio_stream("songs\\caverna_corais.ogg", 4, 1024);
            break;

        case 4:
            imagem = al_load_bitmap("images\\campos_ilusao.jpg");
            musica = al_load_audio_stream("songs\\campos_ilusao.ogg", 4, 1024);
            break;

        case 5:
            imagem = al_load_bitmap("images\\selva.jpg");
            musica = al_load_audio_stream("songs\\selva.ogg", 4, 1024);
            break;

        case 6:
            imagem = al_load_bitmap("images\\floresta_assombrada.jpg");
            musica = al_load_audio_stream("songs\\floresta_assombrada.ogg", 4, 1024);
            break;

        case 7:
            imagem = al_load_bitmap("images\\vulcao.jpg");
            musica = al_load_audio_stream("songs\\vulcao.ogg", 4, 1024);
            break;

        case 8:
            imagem = al_load_bitmap("images\\tubos_vulcanicos.jpg");
            musica = al_load_audio_stream("songs\\tubos_vulcanicos.ogg", 4, 1024);
            break;

        case 9:
            imagem = al_load_bitmap("images\\templo_perdido.jpg");
            musica = al_load_audio_stream("songs\\templo_perdido.ogg", 4, 1024);
    }

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_draw_bitmap(imagem, 0, 0, 0);
    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    al_set_audio_stream_playing(musica, true);
    al_flip_display();

    while (1) {
        ALLEGRO_EVENT evento;
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, 0.05);

        int tem_eventos = al_wait_for_event_until(fila_eventos, &evento, &timeout);

        if (tem_eventos && evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;

        al_draw_bitmap(imagem, 0, 0, 0);
        al_flip_display();
    }

    al_destroy_audio_stream(musica);
    al_destroy_sample(sample);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
}

int escolheimagem(personagemprinc *principal)
{
         if (!strcmp(principal->sublocal, "Praia"))               return 1;
    else if (!strcmp(principal->sublocal, "Floresta"))            return 2;
    else if (!strcmp(principal->sublocal, "Caverna de Corais"))   return 3;
    else if (!strcmp(principal->sublocal, "Campos da Ilusão"))    return 4;
    else if (!strcmp(principal->sublocal, "Selva"))               return 5;
    else if (!strcmp(principal->sublocal, "Floresta Assombrada")) return 6;
    else if (!strcmp(principal->sublocal, "Vulcão"))              return 7;
    else if (!strcmp(principal->sublocal, "Tubos de Lava"))       return 8;
    else if (!strcmp(principal->sublocal, "Templo Perdido"))      return 9;
}

void LeDialogo(personagemprinc *principal)
{
    FILE *fp;
    char linha[1000];
    fflush(stdin);

    int i = escolheimagem(principal);

    switch (i) {
        case 1:
            fp = fopen("files\\praia.txt","rt");
            break;

        case 2:
            fp = fopen("files\\floresta.txt","rt");
            break;

        case 3:
            fp = fopen("files\\caverna_corais.txt","rt");
            break;

        case 4:
            fp = fopen("files\\campos_ilusao.txt","rt");
            break;

        case 5:
            fp = fopen("files\\selva.txt","rt");
            break;

        case 6:
            fp = fopen("files\\floresta_assombrada.txt","rt");
            break;

        case 7:
            fp = fopen("files\\vulcao.txt","rt");
            break;

        case 8:
            fp = fopen("files\\tubos_vulcanicos.txt","rt");
            break;

        case 9:
            fp = fopen("files\\templo_perdido.txt","rt");
    }

    while (!feof(fp)) {
        fscanf(fp," %[^\n]",linha);
        printf("%s",linha);
        readkey();
        limpaTela();
    }

    fflush(stdin);
    fclose(fp);
}



///------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------///

void pressenter ()
{
printf ("\n%*sAperte Enter para continuar", 24, " ");
getchar ();
fflush (stdin);
}


void printstat(personagemprinc *principal)
{

    printf ("Estatisticas:\nArmadura: %i\n", principal->armor);
    printf ("Nome da Arma: %s\n", principal->wname);
    printf ("Dano da Arma: %i\n", principal->weapon);
    printf ("HP: %i/%i\n", principal->hp, principal->mhp);
}

void choice (int *n, int max)
{
if (!max) max = 4;
scanf ("%d", &(*n));
fflush (stdin);
if (*n > 0 && *n <= max) valid = 1;
}



void Luta(int m, personagemprinc *principal){

    ehp = e[m].hp1;
    while (ehp > 0 && (principal->hp) > 0 && (run < 13 || !e [m].run))
    {
    fflush(stdin);
    for (valid = 0; !valid; choice (&n, 4))
    {
    system ("cls");
    printf ("Voce esta em combate com: %s\n", e[m].name);
    printf ("HP Inimigo:%s%i%sSeu HP:%s%i/%i\n",
    "\t", ehp, "\t", "\t", principal->hp, principal->mhp);
    printf ("Armadura Inimigo:%s%i%sSua Armadura:"
    "%s%i\n", "\t", e[m].ap, "\t", "\t", principal->armor);
    printf ("Arma Inimigo:%s%i%sSua Arma:%s%i\n\n",
    "\t", e[m].wh, "\t", "\t", principal->weapon);
    printf ("[1] Atacar\n[2] Correr\n[3] Estatisticas\n[4] Imagem Inimigo\n");
    }
        if (n == 1 && (principal->hp) > 0)
    {
    damage = (principal->weapon) - (e[m].ap) * rand()%3;
    printf ("Voce deu %i de dano!\n\n", damage);

    if ((ehp -= damage) < 1)
    {
    printf ("Voce matou o inimigo\n");
    readkey();
    principal->hp = principal->mhp;

    }
    }

    if (n == 1 && ehp > 0)
    {
    damage = (e[m].wh) - (principal->armor) * rand()%3;
    printf ("Voce recebeu %i de dano!\n", damage);
    readkey();

    if (((principal->hp) -= damage) < 1){
            limpaTela();
            printf ("Voce Perdeu!\n");
            readkey();
            exit(0);

    }
    }

    if (n == 2 && (run = rand () % 26) > 12 && e[m].run){
    printf ("\nVoce conseguiu fugir da batalha!\n");
    readkey();}
    if (n == 2 && (run < 13 || !e[m].run)){
    printf ("\nVoce falhou em fugir da batalha!\n");
    fflush(stdin);
    readkey();}

    if (n == 3){
    limpaTela();
    printstat (principal);
    readkey();}

    if(n == 4) {
       if(!strcmp(e[m].name,"Cobra")){
            carregarMonstro(1);}
       else if (!strcmp(e[m].name,"Pantera")){
            carregarMonstro(2);}
       else if (!strcmp(e[m].name,"Javali")){
            carregarMonstro(3);}
       else if(!strcmp(e[m].name,"Ghoul")){
            carregarMonstro(4);}
       else if (!strcmp(e[m].name,"Zumbi")){
            carregarMonstro(5);}
       else if(!strcmp(e[m].name,"Salamandra")){
            carregarMonstro(6);}
       else if (!strcmp(e[m].name,"Lobo")){
            carregarMonstro(7);}
       else if(!strcmp(e[m].name,"Aranha Gigante")){
            carregarMonstro(8);}
      else if(!strcmp(e[m].name,"Oslun")){
            carregarMonstro(9);}
      else if(!strcmp(e[m].name,"Necromancer")){
            carregarMonstro(10);}
      else if(!strcmp(e[m].name,"Rei Uzgal")){
            carregarMonstro(11);}
      else if(!strcmp(e[m].name,"Minasaer")){
            carregarMonstro(12);}
      else if(!strcmp(e[m].name,"Iklaun")){
            carregarMonstro(13);}
      else if(!strcmp(e[m].name,"Nazeru")){
            carregarMonstro(14);}
      else if(!strcmp(e[m].name,"Guardião")){
            carregarMonstro(15);}
    }
 }
}

void carregarMonstro(int i){
    const int LARGURA_TELA = 800;
    const int ALTURA_TELA = 600;

    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_BITMAP *imagem = NULL;
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
    ALLEGRO_AUDIO_STREAM *musica = NULL;
    ALLEGRO_SAMPLE *sample = NULL;

            if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return -1;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }

    if(i==1){
        imagem = al_load_bitmap("images\\cobra.jpg");
        if (!imagem)
        {
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(janela);
        }
         musica = al_load_audio_stream("songs\\monstros.ogg", 4, 1024);
        if (!musica)
        {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
        }
    }

    if(i==2){
        imagem = al_load_bitmap("images\\pantera.jpg");
        if (!imagem)
        {
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(janela);
        }
         musica = al_load_audio_stream("songs\\monstros.ogg", 4, 1024);
        if (!musica)
        {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
        }
    }

    if(i==3){
        imagem = al_load_bitmap("images\\javali.jpg");
        if (!imagem)
        {
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(janela);
        }
         musica = al_load_audio_stream("songs\\monstros.ogg", 4, 1024);
        if (!musica)
        {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
        }
    }

    if(i==4){
        imagem = al_load_bitmap("images\\ghoul.jpg");
        if (!imagem)
        {
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(janela);
        }
        musica = al_load_audio_stream("songs\\monstros.ogg", 4, 1024);
        if (!musica)
        {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
        }
    }
    if(i==5){
        imagem = al_load_bitmap("images\\zumbi.jpg");
        if (!imagem)
        {
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(janela);
        }
         musica = al_load_audio_stream("songs\\monstros.ogg", 4, 1024);
        if (!musica)
        {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
        }
    }
    if(i==6){
        imagem = al_load_bitmap("images\\salamandra.jpg");
        if (!imagem)
        {
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(janela);
        }
         musica = al_load_audio_stream("songs\\monstros.ogg", 4, 1024);
        if (!musica)
        {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
        }
    }
    if(i==7){
        imagem = al_load_bitmap("images\\lobo.jpg");
        if (!imagem)
        {
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(janela);
        }
         musica = al_load_audio_stream("songs\\monstros.ogg", 4, 1024);
        if (!musica)
        {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
        }
    }
    if(i==8){
        imagem = al_load_bitmap("images\\big_spider.jpg");
        if (!imagem)
        {
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(janela);
        }
         musica = al_load_audio_stream("songs\\monstros.ogg", 4, 1024);
        if (!musica)
        {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
        }
    }
    if(i==9){
        imagem = al_load_bitmap("images\\oslun.jpg");
        if (!imagem)
        {
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(janela);
        }
         musica = al_load_audio_stream("songs\\monstros.ogg", 4, 1024);
        if (!musica)
        {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
        }
    }
    if(i==10){
        imagem = al_load_bitmap("images\\necromancer.jpg");
        if (!imagem)
        {
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(janela);
        }
         musica = al_load_audio_stream("songs\\monstros.ogg", 4, 1024);
        if (!musica)
        {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
        }
    }
    if(i==11){
        imagem = al_load_bitmap("images\\rei_uzgal.jpg");
        if (!imagem)
        {
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(janela);
        }
         musica = al_load_audio_stream("songs\\rei_uzgal.ogg", 4, 1024);
        if (!musica)
        {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
        }
    }
    if(i==12){
        imagem = al_load_bitmap("images\\minasaer.jpg");
        if (!imagem)
        {
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(janela);
        }
         musica = al_load_audio_stream("songs\\minasaer.ogg", 4, 1024);
        if (!musica)
        {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
        }
    }
    if(i==13){
        imagem = al_load_bitmap("images\\iklaun.jpg");
        if (!imagem)
        {
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(janela);
        }
         musica = al_load_audio_stream("songs\\iklaun.ogg", 4, 1024);
        if (!musica)
        {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
        }
    }
    if(i==14){
        imagem = al_load_bitmap("images\\nazeru.jpg");
        if (!imagem)
        {
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(janela);
        }
         musica = al_load_audio_stream("songs\\minasaer.ogg", 4, 1024);
        if (!musica)
        {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
        }
    }
    if(i==15){
        imagem = al_load_bitmap("images\\guardiao.jpg");
        if (!imagem)
        {
            fprintf(stderr, "Falha ao carregar o arquivo de imagem.\n");
            al_destroy_display(janela);
        }
         musica = al_load_audio_stream("songs\\guardiao.ogg", 4, 1024);
        if (!musica)
        {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
        }
    }


    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return -1;
    }

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

    al_draw_bitmap(imagem, 0, 0, 0);
    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
    al_set_audio_stream_playing(musica, true);


    al_flip_display();

    while (1)
    {
        ALLEGRO_EVENT evento;
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, 0.05);

        int tem_eventos = al_wait_for_event_until(fila_eventos, &evento, &timeout);

        if (tem_eventos && evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }

        al_draw_bitmap(imagem, 0, 0, 0);
        al_flip_display();
    }
    al_destroy_audio_stream(musica);
    al_destroy_sample(sample);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
}

char* LeNome(){
      char *nome = (char*) malloc(sizeof(char)*51);
      puts("Digite o seu nome:");
      fflush(stdin);
      gets(nome);
      return nome;
}


personagemprinc* atributo(personagemprinc *principal){
   int i = escolheimagem(principal);
   int k=rand()%2;
    if(i==1){

        if(k==0){
                    (principal->hp)+=rand()%40+30;
                    (principal->mhp)=(principal->hp);
                    (principal->weapon)+=rand()%30+30;
                    (principal->armor)+=rand()%20+30;
                    strcpy(principal->wname,"Faca de Madeira");

                }
        if(k==1){
                    (principal->hp)+=rand()%30+20;
                    (principal->mhp)=(principal->hp);
                    (principal->weapon)+=rand()%40+40;
                    (principal->armor)+=rand()%20+30;
                    strcpy(principal->wname,"Faca de Madeira");
                }
   }
   if(i==2){
        if(k==0){
                    (principal->hp)+=rand()%40+60;
                    (principal->mhp)=(principal->hp);
                    (principal->weapon)+=rand()%30+50;
                    (principal->armor)+=rand()%20+50;
                    strcpy(principal->wname,"Espada Antiga");

                }
        if(k==1){
                (principal->hp)+=rand()%30+40;
                (principal->mhp)=(principal->hp);
                (principal->weapon)+=rand()%40+70;
                (principal->armor)+=rand()%20+50;
                strcpy(principal->wname,"Espada Antiga");
            }
   }
   if(i==3){
        if(k==0){
                (principal->hp)+=rand()%40+90;
                (principal->mhp)=(principal->hp);
                (principal->weapon)+=rand()%30+70;
                (principal->armor)+=rand()%20+70;
                strcpy(principal->wname,"Faca de Prata");

        }
        if(k==1){
                (principal->hp)+=rand()%30+60;
                (principal->mhp)=(principal->hp);
                (principal->weapon)+=rand()%40+100;
                (principal->armor)+=rand()%20+70;
                strcpy(principal->wname,"Faca de Prata");

        }
   }
   if(i==4){
        if(k==0){
            (principal->hp)+=rand()%40+120;
            (principal->mhp)=(principal->hp);
            (principal->weapon)+=rand()%30+90;
            (principal->armor)+=rand()%20+90;
            strcpy(principal->wname,"Espada Militar");

        }
        if(k==1){
            (principal->hp)+=rand()%30+80;
            (principal->mhp)=(principal->hp);
            (principal->weapon)+=rand()%40+130;
            (principal->armor)+=rand()%20+90;
            strcpy(principal->wname,"Espada Militar");

        }
   }
   if(i==5){

        if(k==0){
            (principal->hp)+=rand()%40+150;
            (principal->mhp)=(principal->hp);
            (principal->weapon)+=rand()%30+110;
            (principal->armor)+=rand()%20+110;
            strcpy(principal->wname,"Sabre Heróico");


        }
        if(k==1){
            (principal->hp)+=rand()%30+100;
            (principal->mhp)=(principal->hp);
            (principal->weapon)+=rand()%40+160;
            (principal->armor)+=rand()%20+110;
            strcpy(principal->wname,"Sabre Heróico");

        }
   }
   if(i==6){

        if(k==0){
            (principal->hp)+=rand()%40+180;
            (principal->mhp)=(principal->hp);
            (principal->weapon)+=rand()%30+130;
            (principal->armor)+=rand()%20+130;
            strcpy(principal->wname,"Espada Amaldiçoada");

        }
        if(k==1){
            (principal->hp)+=rand()%30+120;
            (principal->mhp)=(principal->hp);
            (principal->weapon)+=rand()%40+190;
            (principal->armor)+=rand()%20+130;
            strcpy(principal->wname,"Espada Amaldiçoada");
        }
   }
   if(i==7){

        if(k==0){
            (principal->hp)+=rand()%40+210;
            (principal->mhp)=(principal->hp);
            (principal->weapon)+=rand()%30+150;
            (principal->armor)+=rand()%20+150;
            strcpy(principal->wname,"Espada Fantasma");

        }
        if(k==1){
            (principal->hp)+=rand()%30+140;
            (principal->mhp)=(principal->hp);
            (principal->weapon)+=rand()%40+210;
            (principal->armor)+=rand()%20+150;
            strcpy(principal->wname,"Espada Fantasma");

        }
   }
   if(i==8){

        if(k==0){
            (principal->hp)+=rand()%40+240;
            (principal->mhp)=(principal->hp);
            (principal->weapon)+=rand()%30+170;
            (principal->armor)+=rand()%20+170;
            strcpy(principal->wname,"Vingança Dourada");

        }
        if(k==1){
            (principal->hp)+=rand()%30+160;
            (principal->mhp)=(principal->hp);
            (principal->weapon)+=rand()%40+240;
            (principal->armor)+=rand()%20+170;
            strcpy(principal->wname,"Vingança Dourada");

        }

    return principal;
   }
}



int escolhemonstro(personagemprinc *principal){
           if(!strcmp(principal->sublocal,"Praia")){
            return (rand()%2+4);
            }
       else if(!strcmp(principal->sublocal,"Floresta")){
            return (rand()%3);
            }
       else if (!strcmp(principal->sublocal,"Caverna de Corais")){
            return 11;
            }
       else if (!strcmp(principal->sublocal,"Campos da Ilusão")){
            return (rand()%2+7);
            }
       else if(!strcmp(principal->sublocal,"Selva")){
            return 14;
            }
       else if (!strcmp(principal->sublocal,"Floresta Assombrada")){
            return 12;
            }
       else if(!strcmp(principal->sublocal,"Vulcão")){
            return (rand()%2+9);
            }
       else if (!strcmp(principal->sublocal,"Tubos de Lava")){
            return 13;
            }
       else if(!strcmp(principal->sublocal,"Templo Perdido")){
            return 15;
            }
       else {return 0;}

}

void mostrafim(char *nomeArq)
{
	FILE *fp;
	char linha[1000];
	fp = fopen("files\\texto_final.txt","rt");
	if(fp == NULL){
	printf("Erro \n");
	exit (1);
	}
	 while(!feof(fp)){
	               fscanf(fp," %[^\n]",linha);
	               printf("%s",linha);
	               readkey();
	               fflush(stdin);
	               limpaTela();
	               }
	fclose(fp);
}

bool inicializar2();

bool inicializar2()
{
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return false;
    }

    if (!al_install_audio())
    {
        fprintf(stderr, "Falha ao inicializar áudio.\n");
        return false;
    }

    if (!al_init_acodec_addon())
    {
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
        return false;
    }

    if (!al_reserve_samples(1))
    {
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        return false;
    }

    musica = al_load_audio_stream("songs\\creditos.ogg", 4, 1024);
    if (!musica)
    {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_sample(sample);
        return false;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return false;
    }

    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }

    al_set_window_title(janela, "Ilha da Ilusao");

    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return false;
    }

    fundo = al_load_bitmap("images\\creditos.jpg");
    if (!fundo)
    {
        fprintf(stderr, "Falha ao carregar imagem de fundo.\n");
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return false;
    }

    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

    return true;
}
