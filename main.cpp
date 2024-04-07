#include <Windows.h>
#include <iostream>
#include <vector>
#include <Psapi.h>

class MainMenu {
public:
    void show() {
        std::cout << "==================== Главное меню ====================\n\n";
        std::string MainMenuText = "[1] - Инъекция\n"
            "[0] - Выход";
        std::cout << MainMenuText << std::endl;
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            injectionMenu();
            break;
        case 0:
            std::cout << "Выход\n";
            break;
        default:
            std::cout << "Неверный выбор\n";
        }
    }

private:
    void injectionMenu() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        std::cout << "====================== Инъекция ======================\n\n";
        std::string InjectMenuText = "[1] - Выбор процесса из списка\n"
            "[2] - Выбор процесса вручную по имени\n"
            "[3] - Выбор процесса по имени окна\n";
        std::cout << InjectMenuText;

        if (changeTextColor) {
            SetConsoleTextAttribute(hConsole, 3);
        }
        std::cout << "[4] - Инъекция\n";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        std::cout << "[0] - Назад" << std::endl;

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            selectProcess();
            break;
        case 2:
        case 3:
            break;
        case 4:
            break;
        case 0:
            show();
            break;
        default:
            std::cout << "Неверный выбор\n";
        }
    }

    void selectProcess() {
        std::cout << "====================== Список процессов ======================\n\n";
        std::vector<std::string> processList = getProcessList();
        for (size_t i = 0; i < processList.size(); ++i) {
            std::cout << "[" << i + 1 << "] - " << processList[i] << std::endl;
        }

        std::cout << "[0] - Назад" << std::endl;

        int choice;
        std::cin >> choice;

        if (choice == 0) {
            injectionMenu();
        }
        else if (choice >= 1 && choice <= processList.size()) {
            selectedProcess = processList[choice - 1];
            std::cout << "Выбран процесс: " << selectedProcess << std::endl;
            injectionMenu();
        }
        else {
            std::cout << "Неверный выбор\n";
        }
    }

    std::vector<std::string> getProcessList() {
        std::vector<std::string> processList;
        DWORD processes[1024], cbNeeded, cProcesses;

        if (!EnumProcesses(processes, sizeof(processes), &cbNeeded)) {
            std::cerr << "Ошибка при получении списка процессов.\n";
            return processList;
        }

        cProcesses = cbNeeded / sizeof(DWORD);

        for (size_t i = 0; i < cProcesses; ++i) {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
            if (hProcess != NULL) {
                char szProcessName[MAX_PATH] = "<unknown>";
                HMODULE hMod;
                DWORD cbNeeded;

                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
                    GetModuleBaseNameA(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(char));
                    processList.push_back(szProcessName);
                }

                CloseHandle(hProcess);
            }
        }

        return processList;
    }

    std::string selectedProcess;
    bool changeTextColor = true;
};

int main() {
    setlocale(LC_ALL, "RU");
    MainMenu mm;
    mm.show();
}
