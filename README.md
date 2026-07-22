# Aseprite Auto-Compilador (C++)
Este é um automatizado feito em **C++ e Raylib** para facilitar a construção oficial do Aseprite no Windows. Ele configura todas as dependências complexas (Skia e Ninja) de forma "blindada".

# Créditos
Eu obviamente preciso credibilizar os desenvolvedores do [Aseprite](https://aseprite.com/), sem Aseprite, sem AsepriteAutoBuild, e caso você queira apoiar os desenvolvedores, considere comprar o Aseprite.

# O que este projeto faz?
Compilar o Aseprite manualmente é famoso por dar muitos erros, principalmente com a biblioteca **Skia**. Este script resolve isso:

*   Baixa automaticamente a versão correta do **Skia (M124)** pré-compilada.
*   Configura o **Ninja** e o **CMake** sem que você precise mexer em variáveis de ambiente.
*   Clona a versão mais recente **v1.3.16** e realiza o build completo.

# Requisitos
*   **Windows 10 ou 11 x64**.
*   Conexão com a internet.
#  Como usar:
1. Vá na aba **Releases** aqui no GitHub e baixe o arquivo [AsepriteAutoBuild.exe](https://github.com/tomanikandre-max/AsepriteAutoBuild/releases/download/1.0.5/Aseprite-Auto-Build.exe) ou [baixe o código fonte](https://github.com/tomanikandre-max/AsepriteAutoBuild/archive/refs/heads/main.zip) e use o compilador que prefere para compilar, de preferência [Visual Studio Community](https://aka.ms/vs/17/release/vs_community.exe), caso tenha dificuldades para compilar consulte [instruções.md](https://github.com/tomanikandre-max/AsepriteAutoBuild/blob/main/Instru%C3%A7%C3%B5es.md), ou se você não souber usar o programa consulte [Utilização.md](https://github.com/tomanikandre-max/AsepriteAutoBuild/blob/main/Utiliza%C3%A7%C3%A3o.md).
2. Execute o Arquivo.
3. Selecione a linguagem e o local onde instalar, e clique em instalar.


# Onde encontro o programa pronto?
Após o término da compilação, o executável estará em:
`pasta_escolhida\aseprite\build\bin\aseprite.exe`

---

# Sobre o Autor
Sou um desenvolvedor de 11 anos apaixonado por **C++/C** e automação de sistemas. Criei este instalador para ajudar artistas e desenvolvedores a terem acesso ao Aseprite através da compilação oficial de forma simples e rápida!# AsepriteAutoBuild
