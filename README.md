# LukHudX -> [![test_wf](https://github.com/FSLART/LukHudX/actions/workflows/test_wf.yml/badge.svg?branch=master)](https://github.com/FSLART/LukHudX/actions/workflows/test_wf.yml) [![deploy_wf](https://github.com/FSLART/LukHudX/actions/workflows/deploy_wf.yml/badge.svg?branch=master)](https://github.com/FSLART/LukHudX/actions/workflows/deploy_wf.yml)

Bem vindo ao LukHudX!

Este repo tem o propósito de uma implementação do software que será usado tanto para o volante do T14 para fazer alguns testes, como no futuro para modelos mais recentes. A sua implementação atualmente encontra-se em QT 6.0 com base nas bibliotecas nativas em C++. Para compilar o projeto necessitarás de qMake(uma implementação inspirada no software Cmake e make) existindo cerca de 4 ambientes, estes sendo: **Profile, Debug, Release e Testing**

## v1.0.0

Apesar de não ter havido versionamento pois o desenvolvimento foi apressado e houveram prioridades este release é exclusivamente para as variáveis do T-14 e não deve ser usado para outros veiculos pois terão variaveis de controlo diferente às usadas.

# Ambientes

Deverás usar o software qtcreator já que ajuda a executar o qmake e o make com todo o make eyecandy para poderes fazer debugging e dar set up automatico de alguns ambientes listados acima. Três dos quatro ambientes são criados automaticamente pelo qtcreator, mas um deles (Testing) terá de ser criado manualmente no qtcreator, pois não vem por defeito.

Para compilar o programa no ambiente testing atualmente(13-02-2023), é necessário adicionar a flag `CONFIG+=test`, ou seja, o comando quando escrito manualmente deve ser:

```bash
qmake -makefile LukHudX.pro CONFIG+=test
```

No entanto, como dito anteriormente para manter o eyecandy que o qtcreator oferece basta ir a Projects (barra vertical no lado esquerdo do ecrã); Clicar no Kit que desejas criar o ambiente para (no meu caso é Desktop, mas no futuro poderás fazer este passo para o kit do BananaPi M2, se este estiver devidamente configurado); Build; Escolher a opção "Profile"; Clicar Clone; e adicionar CONFIG+=test ao aditional arguments no qmake Build Steps.

O comando escrito no Effective qmake call por isso fica algo do genero:

```bash
/usr/bin/qmake <DIRETORIA>/LukHudX_1/LukHudX.pro -spec linux-g++ CONFIG+=qml_debug CONFIG+=qtquickcompiler CONFIG+=force_debug_info CONFIG+=separate_debug_info CONFIG+=test && /usr/bin/make qmake_all
```

A notar que devido à configuração atual o target terá o sufixo de ``_testes`` se preferires que so seja produzido um binario que dependa da ultima compilação deves ir ao ficheiro com a extensão .pro e alterar:

```cmake
test{
    message(A configurar a build de testes...)
    #Linha que diz o nome do binario de output
    #(LINHA ANTIGA) -----> TARGET = LukHudX_testes
    TARGET = LukHudX

    QT += testlib
    SOURCES -= src/main.cpp
    #...Blah Blah os outros ficheiros para os testes
}

```

**ATENÇÃO**
---------

Se mudares a linha e tua responsabilidade mudares as alterações de volta antes de fazeres um commit. O .pro nao deve ser ignorado pois causa a que toda a gente tenha de manualmente criar o ficheiro. Ignorar isto pode causar problemas futuros.

---

# Dependencias

Vais precisar do standard c++ stuff, Clang, make, etc... nao vou listar isto porque provavelmente já os tens instalados, e o que importa sao as versões
//TODO adicionar versoes relevantes :P

QT 6.0+
**O software foi desenvolvido em 6.2.1**
QSerialPort 6.0+
Terás de usar uma versão correspondente a acima

Para o BSON irás precisar de uma biblioteca json, especificamente:
https://github.com/nlohmann/json
Poderás usar a seguinte package com o pacman ``nlohmann-json``. Foi usada a versão `nlohmann-json-3.11.2-1-any` para o desenvolvimento

Para testes terás de instalar gengetopt e socat.

# Manutenção do README

Se notares que o .readme está bastante desatualizado e pertences à equipa deves chatear uma pessoa apropriada. A partida no futuro existerá um manual tanto para utilizadores como para developers.

Se leste o acima mereces um presente eis um quick ascii art que o criador original da repo fez(se é o primeiro que encontras procura os outros):

```sh

_____________       _____________      _____________      ____________      ____
|___         |     /  	        |      \           /     |___     __  >     \   \ 
    \    ____|    /    /--->»>»/   .    \---___---/          \   <  >  º    |    \
    |   |         |   |                     |||              |    ~~  +     |    ~
   <    \_____>    \   \_________     *     |||              |   ____-      |   |
    |	____/       \__________  /          |||              |   |      *   |   |
    /   /           __________/  /  *       |||              |   |          |   |_____
    \   \          \             \     _____\\\_____         \   \   -   ___|         |
    |___|           ------------      |_____________|        |___|       |____________|
```
Bom o acima está desatualizado por isso
```sh

      ____            ___        _____________      ______________________________
      \   \          //_ \\      \             \    |[ ] Software Up to standards|
      |    \        //^ ^ \\     |    [O]--[O]  )  /|[X] Werks on My Machine     |
      | ___/       ||  ^ ^ \\    |             /  / |_________\__________________|
      |   |        ||      \/\   <    (_____)  \               |0xFFAA|
      |   |        ||=/=====|/   |	__/    \_\ >               |0xA253|
      |   |_____   ||/======||   /   /      \_+ \              |0xB208|
   .__|         |  ||       ||   \   \       \_  >             ╫______╫
   |____________|  ||       ||   |___|        \___\            ╫      ╫
          (AD esteve aq.)                                      ╫╩╩╩╩╩╩╫
               
```
 
    
 
