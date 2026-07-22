#include <filesystem>
#include <string>
#include <fstream>
#pragma warning(disable : 4996)
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <direct.h>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic> 
#include <algorithm> 

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define NOGDI             
#define NOUSER  
#endif
#undef Rectangle
#undef CloseWindow
#undef ShowCursor
#undef PlaySound

extern "C" {
#include "raylib.h"
}

#define RAYGUI_IMPLEMENTATION
#define MB_OK 0x00000000L
#define MB_ICONINFORMATION   0x00000040L

extern "C" {
    __declspec(dllimport) int __stdcall MessageBoxA(void* hWnd, const char* lpText, const char* lpCaption, unsigned int uType);
}

#define MessageBox MessageBoxA
#include "raygui.h"

extern "C" int __stdcall WinExec(const char* lpCmdLine, unsigned int uCmdShow);
#define SW_HIDE 0

namespace fs = std::filesystem;

std::vector<std::string> ultimasLinhas;
std::mutex logMutex;
std::atomic<bool> processandoOtimizacao = false;
std::string etapaAtual = "";
bool editMode = false;
int passo = 0;
double tempoInicio = GetTime();
float tempoRestante = 0;
char pasta[512] = "C:\\Aseprite";
bool prontoParaProximo = false;
int pagina = 0;

void ExecutarComLog(std::string comando);
void Thread(char lingua);
bool IsPathValid(const std::string& p) {
    if (p.find('&') != std::string::npos || p.find('|') != std::string::npos ||
        p.find('>') != std::string::npos || p.find('<') != std::string::npos) {
        return false;
    }
    return true;
}

int main() {
    char lingua2 = ' ';
    char path[500];
    int x = 600, y = 450;

    char comando1[560];
    char comando2[560];
    char comando3[560];

    _getcwd(path, sizeof(path));

    InitWindow(x, y, "Instalador Aseprite");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
    SetTargetFPS(60);

    sprintf_s(comando2, "%s\\Roboto\\Roboto-VariableFont_wdth,wght.ttf", path);
    Font Roboto = LoadFontEx(comando2, 800, 0, 800);

    sprintf_s(comando1, "%s\\Icones\\icon1.png", path);
    Texture2D logo = LoadTexture(comando1);

    sprintf_s(comando1, "%s\\Icones\\icon2.png", path);
    Texture2D logo2 = LoadTexture(comando1);

    sprintf_s(comando2, "%s\\Icones\\icon1.png", path);
    Image icone = LoadImage(comando2);

    sprintf_s(comando3, "%s\\Icones\\file.png", path);
    Texture2D arquivo = LoadTexture(comando3);

    SetWindowIcon(icone);
    UnloadImage(icone);

    while (!WindowShouldClose()) {
        Color corFundo = { 33, 37, 43, 255 };
        if (pagina == 0 || pagina == 1) {
            corFundo = { 33, 37, 43, 255 };
        }
        else {
            corFundo = { 235, 235, 240, 255 };
        }

        BeginDrawing();
        ClearBackground(corFundo);

        if (pagina == 0) {
            DrawRectangleRounded({ 20, 20, 560, 410 }, 0.1f, 10, { 45, 49, 58, 255 });
            DrawRectangleRoundedLines({ 20, 20, 560, 410 }, 0.1f, 10, { 60, 65, 75, 255 });
            DrawTexture(logo2, 280, 130, WHITE);

            GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
            GuiSetFont(Roboto);
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt({ 33, 37, 43, 255 }));
            GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
            GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt({ 80, 85, 95, 255 }));
            GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
            GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(corFundo));
            GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(BLACK));

            GuiLabel({ 30, 50, 300, 30 }, "Welcome to");
            GuiLabel({ 30, 80, 400, 30 }, "Aseprite setup");
            GuiLabel({ 30, 140, 300, 30 }, "Bem vindo ao");
            GuiLabel({ 30, 170, 400, 30 }, "instalador do Aseprite");
            GuiLabel({ 30, 230, 300, 30 }, "Selecione sua lingua:");
            GuiLabel({ 30, 320, 400, 30 }, "Select your language:");

            if (GuiButton({ 370, 30, 190, 70 }, "Portugues")) {
                lingua2 = 'p';
                pagina = 1;
            }
            if (GuiButton({ 370, 350, 190, 70 }, "English")) {
                lingua2 = 'e';
                pagina = 1;
            }
            if (IsKeyPressed(KEY_ENTER)) {
                lingua2 = 'e';
                pagina = 1;
            }
        }
        else if (pagina == 1) {
            if (!processandoOtimizacao) {
                const char* titulo = (lingua2 == 'p') ? "DIRETORIO DE INSTALACAO:" : "SETUP DIRECTORY:";

                DrawRectangleRounded({ 25, 115, 550, 40 }, 0.2f, 10, { 25, 28, 35, 255 });

                if (GuiTextBox({ 30, 120, 540, 30 }, pasta, 512, editMode)) {
                    editMode = !editMode;
                }
                DrawTexture(arquivo, 370, 140, WHITE);
                GuiLabel({ 30, 50, 500, 40 }, titulo);

                if (GuiButton({ 30, 380, 100, 40 }, (lingua2 == 'p' ? "Voltar" : "Back"))) {
                    pagina = 0;
                }
                if (GuiButton({ 150, 250, 300, 70 }, lingua2 == 'p' ? "Compilar" : "START") || IsKeyPressed(KEY_ENTER)) {
                    if (IsPathValid(pasta)) {
                        std::thread(Thread, lingua2).detach();
                    }
                    else {
                        MessageBoxA(NULL, "O diretorio contem caracteres invalidos!", "Erro", MB_OK);
                        strcpy_s(pasta, "C:\\Aseprite");
                    }
                }
            }
            else {
                DrawRectangle(25, 25, 550, 380, { 20, 22, 26, 255 });
                GuiSetStyle(DEFAULT, TEXT_SIZE, 18);
                std::string etapaCopia;
                {
                    std::lock_guard<std::mutex> trava(logMutex);
                    etapaCopia = etapaAtual;
                }

                if (etapaCopia.empty()) {
                    DrawText(lingua2 == 'p' ? "Iniciando..." : "Starting...", 40, 45, 22, LIME);
                }
                else {
                    DrawText(etapaCopia.c_str(), 40, 45, 22, LIME);
                }

                int espacamentoY = 95;
                {
                    std::lock_guard<std::mutex> trava(logMutex);
                    for (const auto& linhaLog : ultimasLinhas) {
                        if (!linhaLog.empty()) {
                            std::string linhaCurta = linhaLog.substr(0, 52);
                            DrawText(linhaCurta.c_str(), 40, espacamentoY, 15, RAYWHITE);
                            espacamentoY += 32;
                        }
                    }
                }
                GuiLock();
                GuiButton({ 380, 330, 180, 60 }, lingua2 == 'p' ? "RODANDO..." : "RUNNING...");
                GuiUnlock();
                GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
            }
        }
        else if (pagina == 2) {
            DrawRectangleRounded({ 20, 20, 560, 410 }, 0.1f, 10, { 45, 49, 58, 255 });
            DrawRectangleRoundedLines({ 20, 20, 560, 410 }, 0.1f, 10, { 60, 65, 75, 255 });
            DrawTexture(logo2, 280, 130, WHITE);

            GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
            GuiSetFont(Roboto);
            GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt({ 33, 37, 43, 255 }));
            GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
            GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt({ 80, 85, 95, 255 }));
            GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(WHITE));
            GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(corFundo));
            GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(BLACK));

            const char* titulo = (lingua2 == 'p') ? "INSTALADO\n\nCOM SUCESSO:" : "SUCESS:";
            const char* btnTexto = "OK";

            GuiLabel({ 200, 100, 500, 40 }, titulo);
            if (GuiButton({ 150, 250, 300, 70 }, btnTexto)) {
                break;
            }
        }
        EndDrawing();
    }
    UnloadTexture(logo);
    UnloadTexture(logo2);
    UnloadTexture(arquivo);
    UnloadFont(Roboto);
    CloseWindow();

    return 0;
}

void ExecutarComLog(std::string comando) {
    FILE* pipe = _popen(comando.c_str(), "r");
    if (!pipe) return;

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        std::string linha = buffer;
        if (!linha.empty() && linha.back() == '\n') linha.pop_back();

        if (!linha.empty()) {
            std::lock_guard<std::mutex> trava(logMutex);
            ultimasLinhas.push_back(linha);
            if (ultimasLinhas.size() > 8) {
                ultimasLinhas.erase(ultimasLinhas.begin());
            }
        }
    }
    _pclose(pipe);
}
void Thread(char lingua) {
    std::string p = fs::path(pasta).make_preferred().string();
    processandoOtimizacao = true;
    {
        std::lock_guard<std::mutex> trava(logMutex);
        ultimasLinhas.clear();
    }

    const char* pf86 = std::getenv("ProgramFiles(x86)");
    const char* pf = std::getenv("ProgramFiles");

    std::vector<std::string> listaComandos;
    if (passo == 0) {
        {
            std::lock_guard<std::mutex> trava(logMutex);
            etapaAtual = (lingua == 'p') ? "Verificando instalacao do Visual Studio..." : "Checking Visual Studio installation...";
        }
        if (!fs::exists(p)) {
            fs::create_directory(p);
        }
        std::string caminhoVSExistente = "";
        if (pf86) {
            listaComandos.push_back("\"\"" + std::string(pf86) + "\\Microsoft Visual Studio\\Installer\\vswhere.exe\" -latest -property installationPath\"");
        }
        if (pf) {
            listaComandos.push_back("\"\"" + std::string(pf) + "\\Microsoft Visual Studio\\Installer\\vswhere.exe\" -latest -property installationPath\"");
        }
        listaComandos.push_back("vswhere.exe -latest -property installationPath");
        for (const auto& cmd : listaComandos) {
            std::wstring wcmd(cmd.begin(), cmd.end());
            FILE* pipe = _wpopen(wcmd.c_str(), L"r");
            if (!pipe) continue;

            wchar_t buffer[256];
            std::wstring resultadoW = L"";
            while (fgetws(buffer, 256, pipe) != NULL) {
                resultadoW += buffer;
            }
            _pclose(pipe);

            if (!resultadoW.empty()) {
                std::string resultadoA(resultadoW.begin(), resultadoW.end());
                resultadoA.erase(resultadoA.find_last_not_of(" \n\r\t") + 1);
                if (!resultadoA.empty() && resultadoA.find("vswhere") == std::string::npos && resultadoA.find("'") == std::string::npos) {
                    if (fs::exists(resultadoA)) {
                        caminhoVSExistente = resultadoA;
                        break;
                    }
                }
            }
        }
        if (!caminhoVSExistente.empty()) {
            if (lingua == 'p') {
                MessageBoxA(NULL,
                    "Visual Studio detectado no sistema!\n\n"
                    "Para garantir que a compilacao funcione, certifique-se de que\n"
                    "o seu Visual Studio possui estas cargas instaladas:\n"
                    "- Desenvolvimento para desktop com C++\n"
                    "- SDK do Windows 10 e 11\n"
                    "- Ferramentas de CMake do C++\n\n"
                    "Clique em OK para prosseguir automaticamente.",
                    "Visual Studio Detectado", MB_OK | MB_ICONINFORMATION);
            }
            else {
                MessageBoxA(NULL,
                    "Visual Studio detected on your system!\n\n"
                    "To ensure the compilation works, make sure your\n"
                    "Visual Studio has these workloads installed:\n"
                    "- Desktop development with C++\n"
                    "- Windows 10 and 11 SDK\n"
                    "- C++ CMake tools\n\n"
                    "Click OK to proceed automatically.",
                    "Visual Studio Detected", MB_OK | MB_ICONINFORMATION);
            }

            passo = 1; 
        }
        else {
            if (lingua == 'p') {
                MessageBoxA(NULL,
                    "Estamos prosseguindo com a instalacao do Visual Studio.\n\n"
                    "O instalador sera baixado e configurara automaticamente:\n"
                    "- Desenvolvimento para desktop com C++\n"
                    "- SDK do Windows 10 e 11\n"
                    "- Ferramentas de CMake\n\n"
                    "Aguarde o termino do processo em segundo plano.",
                    "Instalacao do Visual Studio", MB_OK | MB_ICONINFORMATION);
            }
            else {
                MessageBoxA(NULL,
                    "We are proceeding with the installation of Visual Studio.\n\n"
                    "The installer will download and automatically configure:\n"
                    "- Desktop development with C++\n"
                    "- Windows 10 and 11 SDK\n"
                    "- CMake tools\n\n"
                    "Please wait for the background process to finish.",
                    "Visual Studio Installation", MB_OK | MB_ICONINFORMATION);
            }
            std::string arquivoHttpCode = p + "\\http_code.txt";
            std::string cmdBaixar = "curl -L -s -w \"%{http_code}\" -o \"" + p + "\\vs.exe\" "
                "https://aka.ms/vs/17/release/vs_community.exe > \"" + arquivoHttpCode + "\" 2>&1";

            ExecutarComLog(cmdBaixar);

            int httpCode = 0;
            std::ifstream fileHttp(arquivoHttpCode);
            if (fileHttp.is_open()) {
                fileHttp >> httpCode;
                fileHttp.close();
                remove(arquivoHttpCode.c_str());
            }

            std::uintmax_t tamanhoArquivo = 0;
            std::string caminhoVsExe = p + "\\vs.exe";
            if (fs::exists(caminhoVsExe)) {
                std::ifstream in(caminhoVsExe, std::ifstream::ate | std::ifstream::binary);
                tamanhoArquivo = in.tellg();
            }

            if (httpCode == 200 && tamanhoArquivo > 1000000) {
                std::string cmdInstalar = "\"" + p + "\\vs.exe\" --passive --norestart --wait "
                    "--add Microsoft.VisualStudio.Workload.NativeDesktop "
                    "--add Microsoft.VisualStudio.Component.Windows11SDK.22621 "
                    "--add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 "
                    "--add Microsoft.VisualStudio.Component.Windows10SDK.18362 "
                    "--add Microsoft.VisualStudio.Component.VC.CMake.Project 2>&1";

                {
                    std::lock_guard<std::mutex> trava(logMutex);
                    etapaAtual = (lingua == 'p') ? "Instalando dependencias do Visual Studio..." : "Installing Visual Studio dependencies...";
                }
                ExecutarComLog(cmdInstalar);

                passo = 1;
            }
            else {
                if (lingua == 'p') {
                    MessageBoxA(NULL, "Falha ao baixar o Visual Studio.\nPor favor, verifique sua conexao.", "Erro de Download", MB_OK);
                }
                else {
                    MessageBoxA(NULL, "Failed to download Visual Studio.\nPlease check your internet connection.", "Download Error", MB_OK);
                }
                passo = -1;
                return;
            }
        }
    }
    if (passo == 1) {
        prontoParaProximo = false;
        {
            std::lock_guard<std::mutex> trava(logMutex);
            etapaAtual = (lingua == 'p') ? "Baixando CMake..." : "Downloading CMake...";
        }

        std::string cleanCmd = "cmd.exe /c rd /s /q \"" + p + "\\Cmake\" 2>&1";
        ExecutarComLog(cleanCmd);

        std::string arquivoHttpCode = p + "\\http_code.txt";
        std::string cmdBaixar = "curl -L -s -w \"%{http_code}\" -o \"" + p + "\\cmake.zip\" "
            "https://github.com/Kitware/CMake/releases/download/v3.31.11/cmake-3.31.11-windows-x86_64.zip > \"" + arquivoHttpCode + "\" 2>&1";

        ExecutarComLog(cmdBaixar);

        int httpCode = 0;
        std::ifstream fileHttp(arquivoHttpCode);
        if (fileHttp.is_open()) {
            fileHttp >> httpCode;
            fileHttp.close();
            remove(arquivoHttpCode.c_str());
        }

        std::uintmax_t tamanhoArquivo = 0;
        std::string caminhoCmakeZip = p + "\\cmake.zip";
        if (fs::exists(caminhoCmakeZip)) {
            std::ifstream in(caminhoCmakeZip, std::ifstream::ate | std::ifstream::binary);
            tamanhoArquivo = in.tellg();
        }

        if (httpCode == 200 && tamanhoArquivo > 1000000) {
            std::string cmdExtrairEOrganizar = "cmd.exe /c tar -xf \"" + p + "\\cmake.zip\" -C \"" + p + "\" && "
                "move \"" + p + "\\cmake-3.31.11-windows-x86_64\" \"" + p + "\\Cmake\" && "
                "del \"" + p + "\\cmake.zip\"";

            ExecutarComLog(cmdExtrairEOrganizar);

            std::string pathVerificacao2 = p + "\\Cmake\\bin\\cmake.exe";
            if (fs::exists(pathVerificacao2)) {
                prontoParaProximo = true;
                passo = 2;
            }
            else {
                passo = -1;
                return;
            }
        }
        else {
            MessageBoxA(NULL, "Falha ao baixar o CMake.\nPor favor, baixe-o manualmente colocando na pasta Cmake e execute o instalador novamente.", "Erro de Download", MB_OK);
            passo = -1;
            return;
        }
    }
    if (passo == 2) {
        prontoParaProximo = false;
        {
            std::lock_guard<std::mutex> trava(logMutex);
            etapaAtual = (lingua == 'p') ? "Baixando Skia..." : "Downloading Skia...";
        }
        std::string cleanCmd = "cmd.exe /c rd /s /q \"" + p + "\\skia\" 2>&1";
        ExecutarComLog(cleanCmd);

        std::string arquivoHttpCode = p + "\\http_code.txt";
        std::string cmdBaixar = "curl -L -s -w \"%{http_code}\" -o \"" + p + "\\skia.zip\" "
            "https://github.com/aseprite/skia/releases/download/m124-08a5439a6b/Skia-Windows-Release-x64.zip > \"" + arquivoHttpCode + "\" 2>&1";

        ExecutarComLog(cmdBaixar);

        int httpCode = 0;
        std::ifstream fileHttp(arquivoHttpCode);
        if (fileHttp.is_open()) {
            fileHttp >> httpCode;
            fileHttp.close();
            remove(arquivoHttpCode.c_str());
        }

        std::uintmax_t tamanhoArquivo = 0;
        std::string caminhoSkiaZip = p + "\\skia.zip";
        if (fs::exists(caminhoSkiaZip)) {
            std::ifstream in(caminhoSkiaZip, std::ifstream::ate | std::ifstream::binary);
            tamanhoArquivo = in.tellg();
        }

        if (httpCode == 200 && tamanhoArquivo > 1000000) {
            std::string pastaSkia = p + "\\skia";
            if (!fs::exists(pastaSkia)) fs::create_directory(pastaSkia);

            std::string skiaInstall = "cmd.exe /c tar -xf \"" + p + "\\skia.zip\" -C \"" + p + "\\skia\" && "
                "del \"" + p + "\\skia.zip\" 2>&1";
            ExecutarComLog(skiaInstall);
            std::string pathVerificacaoSkia = p + "\\skia\\out\\Release-x64\\skia.lib";

            if (fs::exists(pathVerificacaoSkia)) {
                passo = 3;
            }
            else {
                MessageBoxA(NULL, "O download do Skia terminou, mas os arquivos nao puderam ser extraidos corretamente.", "Erro de Extracao", MB_OK);
                passo = -1;
                return;
            }
        }
        else {
            MessageBoxA(NULL, "Falha ao baixar o Skia.\nPor favor, baixe-o manualmente colocando na pasta skia e execute o instalador novamente.", "Erro de Download", MB_OK);
            passo = -1;
            return;
        }
    }
    if (passo == 3) {
        prontoParaProximo = false;

        {
            std::lock_guard<std::mutex> trava(logMutex);
            etapaAtual = (lingua == 'p') ? "Baixando Git..." : "Downloading Git...";
        }

        std::string arquivoHttpCode = p + "\\http_code.txt";
        std::string cmdBaixar = "curl -L -s -w \"%{http_code}\" -o \"" + p + "\\Git.7z.exe\" https://github.com/git-for-windows/git/releases/download/v2.53.0.windows.3/PortableGit-2.53.0.3-64-bit.7z.exe > \"" + arquivoHttpCode + "\" 2>&1";

        ExecutarComLog(cmdBaixar);

        int httpCode = 0;
        std::ifstream fileHttp(arquivoHttpCode);
        if (fileHttp.is_open()) {
            fileHttp >> httpCode;
            fileHttp.close();
            remove(arquivoHttpCode.c_str());
        }

        std::uintmax_t tamanhoArquivo = 0;
        std::string caminhoGitExe = p + "\\Git.7z.exe";

        if (fs::exists(caminhoGitExe)) {
            std::ifstream in(caminhoGitExe, std::ifstream::ate | std::ifstream::binary);
            tamanhoArquivo = in.tellg();
        }
        if (httpCode == 200 && tamanhoArquivo > 40000000) {
            std::string gitCmd = "cmd.exe /c \"\"" + p + "\\Git.7z.exe\" -y -gm2 -InstallPath=\"" + p + "\"\" 2>&1";
            ExecutarComLog(gitCmd);
            prontoParaProximo = true;

            if (prontoParaProximo) {
                passo = 4;
            }
        }
        else {
            MessageBoxA(NULL, "Falha ao baixar o Git.\nPor favor, execute o instalador novamente.", "Erro de Download", MB_OK);
            passo = -1;
            return;
        }
    }


    if (passo == 4) {
        prontoParaProximo = false;
        {
            std::lock_guard<std::mutex> trava(logMutex);
            etapaAtual = (lingua == 'p') ? "Baixando Ninja..." : "Downloading Ninja...";
        }
        std::string pastaNinja = p + "\\ninja";
        if (!fs::exists(pastaNinja)) fs::create_directory(pastaNinja);

        std::string arquivoHttpCode = p + "\\http_code.txt";
        std::string cmdBaixar = "curl -L -s -w \"%{http_code}\" -o \"" + p + "\\ninja-win.zip\" "
            "https://github.com/ninja-build/ninja/releases/download/v1.13.1/ninja-win.zip > \"" + arquivoHttpCode + "\" 2>&1";
        ExecutarComLog(cmdBaixar);

        int httpCode = 0;
        std::ifstream fileHttp(arquivoHttpCode);
        if (fileHttp.is_open()) {
            fileHttp >> httpCode;
            fileHttp.close();
            remove(arquivoHttpCode.c_str());
        }

        std::uintmax_t tamanhoArquivo = 0;
        std::string caminhoNinjaZip = p + "\\ninja-win.zip";
        if (fs::exists(caminhoNinjaZip)) {
            std::ifstream in(caminhoNinjaZip, std::ifstream::ate | std::ifstream::binary);
            tamanhoArquivo = in.tellg();
        }

        if (httpCode == 200 && tamanhoArquivo > 100000) {
            std::string ninjaInstall = "cmd.exe /c tar -xf \"" + p + "\\ninja-win.zip\" -C \"" + p + "\\ninja\" && "
                "tar -xf \"" + p + "\\ninja-win.zip\" -C \"" + p + "\\Cmake\\bin\" && "
                "del \"" + p + "\\ninja-win.zip\" 2>&1";
            ExecutarComLog(ninjaInstall);

            std::string pathVerificacaoNinja = p + "\\ninja\\ninja.exe";
            if (fs::exists(pathVerificacaoNinja)) {
                prontoParaProximo = true;
                passo = 5;
            }
            else {
                MessageBoxA(NULL, "O download do Ninja terminou, mas os arquivos nao foram extraidos.", "Erro de Extracao", MB_OK);
                passo = -1;
                return;
            }
        }
        else {
            MessageBoxA(NULL, "Falha ao baixar o Ninja Build.\nPor favor, verifique sua conexao e tente novamente.", "Erro de Download", MB_OK);
            passo = -1;
            return;
        }
    }

    if (passo == 5) {
        prontoParaProximo = false;
        {
            std::lock_guard<std::mutex> trava(logMutex);
            etapaAtual = (lingua == 'p') ? "Clonando repositorio Github..." : "Cloning Github repository...";
        }
        std::string pastaAseprite = p + "\\aseprite";
        if (fs::exists(pastaAseprite)) {
            std::string cleanCmd = "cmd.exe /c rd /s /q \"" + pastaAseprite + "\" 2>&1";
            ExecutarComLog(cleanCmd);
        }
        std::string gitCloneCmd = "cmd.exe /c \"cd /d \"" + p + "\" && "
            "\"" + p + "\\PortableGit\\bin\\git.exe\" clone --recursive --progress https://github.com/aseprite/aseprite.git && "
            "cd aseprite && "
            "\"" + p + "\\PortableGit\\bin\\git.exe\" submodule update --init --recursive\" 2>&1";
        ExecutarComLog(gitCloneCmd);
        std::string arquivoValidacaoGit = p + "\\aseprite\\CMakeLists.txt";
        if (fs::exists(arquivoValidacaoGit)) {
            prontoParaProximo = true;
            passo = 6;
        }
        else {
            MessageBoxA(NULL, "Falha ao clonar o repositorio do Aseprite ou seus submodulos via Git.", "Erro de Clonagem", MB_OK);
            passo = -1;
            return;
        }
    }

    if (passo == 6) {
        {
            std::lock_guard<std::mutex> trava(logMutex);
            etapaAtual = (lingua == 'p') ? "Compilando..." : "Compiling...";
        }
        if (pf86) {
            listaComandos.push_back("\"\"" + std::string(pf86) + "\\Microsoft Visual Studio\\Installer\\vswhere.exe\" -latest -find Common7\\Tools\\VsDevCmd.bat\"");
        }
        if (pf) {
            listaComandos.push_back("\"\"" + std::string(pf) + "\\Microsoft Visual Studio\\Installer\\vswhere.exe\" -latest -find Common7\\Tools\\VsDevCmd.bat\"");
        }
        listaComandos.push_back("vswhere.exe -latest -find Common7\\Tools\\VsDevCmd.bat");

        std::string caminhoBatCompleto = "";

        // Executa o vswhere buscando o arquivo .bat diretamente
        for (const auto& cmd : listaComandos) {
            std::wstring wcmd(cmd.begin(), cmd.end());
            FILE* pipe = _wpopen(wcmd.c_str(), L"r");
            if (!pipe) continue;

            wchar_t buffer[256];
            std::wstring resultadoW = L"";
            while (fgetws(buffer, 256, pipe) != NULL) {
                resultadoW += buffer;
            }
            _pclose(pipe);

            if (!resultadoW.empty()) {
                std::string resultadoA(resultadoW.begin(), resultadoW.end());
                // Limpa quebras de linha e espacos que o CMD joga no final
                resultadoA.erase(resultadoA.find_last_not_of(" \n\r\t") + 1);
                if (!resultadoA.empty() && resultadoA.find("vswhere") == std::string::npos && resultadoA.find("'") == std::string::npos) {
                    if (fs::exists(resultadoA)) {
                        caminhoBatCompleto = resultadoA; // Caminho completo salvo com sucesso!
                        break;
                    }
                }
            }
        }
        std::string cmdCompilacao = "cmd.exe /c \"for /f \"usebackq tokens=*\" %i in (`\"%ProgramFiles(x86)%\\Microsoft Visual Studio\\Installer\\vswhere.exe\" -latest -property installationPath`) do ( "
            "call \"%i\\Common7\\Tools\\VsDevCmd.bat\" -arch=x64 && "
            "cd /d \"" + p + "\\aseprite\" && "
            "if not exist build mkdir build && "
            "cd build && "
            "\"" + p + "\\Cmake\\bin\\cmake.exe\" -DCMAKE_BUILD_TYPE=RelWithDebInfo -G Ninja -DLAF_BACKEND=skia "
            "-DSKIA_DIR=\"" + p + "\\skia\" -DSKIA_LIBRARY_DIR=\"" + p + "\\skia\\out\\Release-x64\" "
            "-DSKIA_LIBRARY=\"" + p + "\\skia\\out\\Release-x64\\skia.lib\" .. && "
            "\"" + p + "\\ninja\\ninja.exe\" aseprite "
            ")\" 2>&1";
        ExecutarComLog(cmdCompilacao);
        std::string executavelFinal = p + "\\aseprite\\build\\bin\\aseprite.exe";
        if (fs::exists(executavelFinal)) {
            prontoParaProximo = true;
            passo = 7;
        }
        else {
            MessageBoxA(NULL, "A compilacao terminou, mas o arquivo aseprite.exe nao foi gerado.\nVerifique o log de erros.", "Erro de Compilacao", MB_OK);
            passo = -1;
        }
        processandoOtimizacao = false;
    }

    if (passo == 7) {
        pagina = 2;
    }

    {
        std::lock_guard<std::mutex> trava(logMutex);
        etapaAtual = "";
    }
}
