# Yawara

Jogo desenvolvido para o trabalho final de Introdução ao Desenvolvimento de Jogos da Universidade de Brasília no primeiro semestre de 2019.

## Iniciando

Estas instruções vão ensinar a como compilar e rodar o jogo 'Yawara' desenvolvido para a disciplina de Introdução ao Desenvolvimento de Jogos

### Pré-requisitos

Para rodar o programa, os seguintes fazem-se necessários:
```
g++ 4.7 ou superior
SDL2
SDL2_image
SDL2_mixer
SDL2_ttf
```

### Instalando

#### em sistema operacional Linux (via terminal):

* g++ (distrubuição Debian):
    ```
    sudo apt-get install build-essencial
    ```

* g++ (distribuição Arch):
    ```
    sudo pacman -S base-devel
    ```

* SDL2 e suas dependências (Debian):
    ```
    sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf
    ```

* SDL2 e suas dependências (Arch):
    ```
    sudo pacman -S sdl2 sdl2-image sdl2-mixer sdl2-ttf
    ```

#### em sistema operacional Windows:
* Primeiramente, precisa-se de um programa de terminal
    * TDM-GCC é recomendado: 
    [TDM-GCC](http://tdm-gcc.tdragon.net/download)
        * durante a intalação, adicione ao PATH
        * TDM-GCC vem com compiladores de C e C++

* SDL2:
    * Baixe SDL2-devel-2.x.x-mingw.tar.gz (ou SDL2-devel-2.x.x-VC.zip se você usa Visual Code) do [link](http://libsdl.org/download-2.0.php)
    * Extraia o arquivo zipado e copie as pastas:
    ```
    bin
    include
    lib
    ```
    para
    ```
    C:\SDL-2.x.x
    ```
* SDL2 image, mixer e ttf:

    * use os links:
        * [SDL_image](http://www.libsdl.org/projects/SDL_image/)
        * [SDL_mixer](http://www.libsdl.org/projects/SDL_mixer/)
        * [SDL_ttf](http://www.libsdl.org/projects/SDL_ttf/)

    * dessas páginas, baixe os arquivos terminados em [...]-devel-2.x.x-mingw.zip
    * assim como com a SDL2, extraia os arquivos zipados e copie as pastas

    ```
    bin
    include
    lib
    ```
    para
    ```
    C:\SDL-2.x.x
    ```
    * fundindo com as pastas previamente instaladas
    * abra a pasta "lib", que contém todos os arquivos .DLL
    ```
    C:\SDL-2.x.x\bin
    ```
    * copie estes arquivos .DLL para  a pasta que contém o executável (.exe)

## Compilando e jogando

Abra o diretório de instalação (no Terminal, caso use Linux ou no TDM-GCC case use Windows) e compile com o comando
```
make
```
Feito isso, rode o jogo com o comando
```
./yawara
```

## Feito com
* [SDL2](https://wiki.libsdl.org/)
* [SDL_image](http://www.libsdl.org/projects/SDL_image/)
* [SDL_mixer](http://www.libsdl.org/projects/SDL_mixer/)
* [SDL_ttf](http://www.libsdl.org/projects/SDL_ttf/)

## Desenvolvedores

**Programadores:**
- Bruno Takashi
    - Responsável por Yawara, câmera, carregamento do cenário, transição de cena, estágios do jogo, colisão do mapa, cobra, cursor, eventos de música, hitbox.
- Gabriel Vieira
    - Responsável por Capelobo, inimigos, cobra.
- Leonardo Alves
    - Responsável por Tapu, adaptação de funções de easing, estágios de título, pausa e fim de jogo, interações (runas, cristais e espíritos corrompidos), hacks, projéteis (Bullets), eventos.

**Designers:**
- Adriel Ramon
- Bruna Dias
- Luciana Liu Lu
- Raimundo Marques

**Músicos:**
- Daniel Araujo
- Rafael Cardim
- Rafael Gama

## O Jogo

### História
Os irmãos Jaci, deusa da lua, e Guaraci, deus do sol, eram venerados e adorados por todos enquanto Tupã, deus da tempestade, era ignorado. Tupã sentia um rancor por ser menosprezado pelos deuses e em um ataque de fúria, acerta Guaraci por trás e o mata. Assim, Tupã reivindica seu lugar nos céus e joga poderes em animais que se tornam seus guardiões.

Jaci, fica em choque e é levada por Tupã pra ser selada. Enquanto isso Guaraci está deitado no chão sangrando. As gotas de sangue caem sobre um lobo guará jovem, que acaba por se tornar Yawara, a reencarnação de Guaraci. Jaci, por sua vez, chora e a partir de suas lágrimas surge o Tapa, espírito guardião de Guaraci.

O objetivo de Yawara é derrotar Tupã e seus guardiões pra recuperar a força de Guaraci para assim voltar aos céus junto de sua querida irmã Jaci.

### Gênero do jogo

Yawara é um jogo com foco em boss-fighting, entretando contém elementos de exploração, para realmente contar a história do jogo, e de observação, trazendo o território brasileiro como cenário, dando um ar aventuresco ao jogo.

### Controles
Controles de Yawara (lobo-guará):
* <kbd>W</kbd>, <kbd>A</kbd>, <kbd>S</kbd>, <kbd>D</kbd>: movimenta para cima, esquerda, baixo e direita. Combinaçoes de teclas (<kbd>W</kbd> + <kbd>A</kbd> ou <kbd>S</kbd> + <kbd>D</kbd> por exemplo) movimentam em direções intermediárias;
* <kbd>Espaço</kbd>: "dash", movimento de velocidade de Yawara que não aplica dano a inimigos;
* <kbd>E</kbd>: uivo, interage com o ambiente onde possível (runas, espíritos corrompidos e cristais);
* Clique direito: mordida, ataque de proximidade co dano mais elevado.

Controles de Tapu (espírito companheiro):
* Mouse: movimenta dentro de uma área ao redor de Yawara;
* Clique esquerdo: ataque de energia, ataque a distância com dano reduzido.

Câmera:
* Foco automático entre o cursor do mouse e Yawara;

### Hacks

* <kbd>F1</kbd>: eleva vida (HP), defesa e dano de Yawara;
* <kbd>F2</kbd>: retorna stats (vida, defesa e dano) aos originais;
* <kbd>F3</kbd>: suicídio

### Jogando

Ao jogar Yawara, seu objetivo é derrotar as forças do mal que corrompem a vida brasileira. Durante o jogo, deve-se vencer Capelobo para restaurar o Cerrado brasileiro e Boiuna para restaurar a Mata Atlântica, salvando assim o território brasileiro.
Caso as forças do mal derrotem Yawara, retirando sua força vital, o Brasil fica corrompido e o mal vence.