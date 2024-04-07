#include <Windows.h>
#include <iostream>
#include <vector>
#include <Psapi.h>

class MainMenu {
public:
    void show() {
        std::cout << "==================== ������� ���� ====================\n\n";
        std::string MainMenuText = "[1] - ��������\n"
            "[0] - �����";
        std::cout << MainMenuText << std::endl;
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            injectionMenu();
            break;
        case 0:
            std::cout << "�����\n";
            break;
        default:
            std::cout << "�������� �����\n";
        }
    }

private:
    void injectionMenu() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        std::cout << "====================== �������� ======================\n\n";
        std::string InjectMenuText = "[1] - ����� �������� �� ������\n"
            "[2] - ����� �������� ������� �� �����\n"
            "[3] - ����� �������� �� ����� ����\n";
        std::cout << InjectMenuText;

        if (changeTextColor) {
            SetConsoleTextAttribute(hConsole, 3);
        }
        std::cout << "[4] - ��������\n";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        std::cout << "[0] - �����" << std::endl;

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
            std::cout << "�������� �����\n";
        }
    }

    void selectProcess() {
        std::cout << "====================== ������ ��������� ======================\n\n";
        std::vector<std::string> processList = getProcessList();
        for (size_t i = 0; i < processList.size(); ++i) {
            std::cout << "[" << i + 1 << "] - " << processList[i] << std::endl;
        }

        std::cout << "[0] - �����" << std::endl;

        int choice;
        std::cin >> choice;

        if (choice == 0) {
            injectionMenu();
        }
        else if (choice >= 1 && choice <= processList.size()) {
            selectedProcess = processList[choice - 1];
            std::cout << "������ �������: " << selectedProcess << std::endl;
            injectionMenu();
        }
        else {
            std::cout << "�������� �����\n";
        }
    }

    std::vector<std::string> getProcessList() {
        std::vector<std::string> processList;
        DWORD processes[1024], cbNeeded, cProcesses;

        if (!EnumProcesses(processes, sizeof(processes), &cbNeeded)) {
            std::cerr << "������ ��� ��������� ������ ���������.\n";
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
