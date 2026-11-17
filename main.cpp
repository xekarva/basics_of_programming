// ---------------------------------------------------------------------------
// Простой графический калькулятор на Dear ImGui + GLFW (backend: OpenGL3)
//
// Интерфейс в стиле Immediate Mode: два числовых поля ("Число А", "Число Б"),
// четыре кнопки операций в ряд (+, -, *, /), крупный вывод результата и
// проверка деления на ноль.
// ---------------------------------------------------------------------------

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include <cmath>
#include <cstdio>
#include <cstring>

// Размер окна фиксирован: пользователь не может его изменить
static const int WINDOW_WIDTH  = 480;
static const int WINDOW_HEIGHT = 260;

// Состояние приложения
struct CalculatorState
{
    float numberA      = 0.0f;   // Число А
    float numberB      = 0.0f;   // Число Б
    float result       = 0.0f;   // Последний результат операции
    bool  hasResult    = false;  // Была ли уже выполнена хотя бы одна операция
    bool  divideByZero = false;  // Флаг ошибки "деление на ноль"
};

// Выполнение операции над числами А и Б
static void PerformOperation(CalculatorState& st, char op)
{
    st.divideByZero = false;

    switch (op)
    {
        case '+': st.result = st.numberA + st.numberB; break;
        case '-': st.result = st.numberA - st.numberB; break;
        case '*': st.result = st.numberA * st.numberB; break;
        case '/':
            if (st.numberB == 0.0f)
            {
                // Проверка на деление на ноль
                st.divideByZero = true;
                st.result       = 0.0f;
            }
            else
            {
                st.result = st.numberA / st.numberB;
            }
            break;
        default: return;
    }

    st.hasResult = true;
}

int main(void)
{
    // --- Инициализация GLFW ------------------------------------------------
    if (!glfwInit())
        return 1;

    // OpenGL 3.3 Core Profile — кроссплатформенно работает и под MinGW/Windows
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // Запрещаем пользователю изменять размер окна
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                          "Калькулятор (ImGui + GLFW)",
                                          nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Вертикальная синхронизация

    // --- Инициализация Dear ImGui ------------------------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Управление с клавиатуры

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    CalculatorState state;

    // --- Главный цикл --------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Панель калькулятора, занимающая всё клиентское пространство окна.
        // Флаги NoMove/NoResize/NoCollapse/no titlebar делают окно фиксированным
        // и без возможности перемещения или изменения самой панели ImGui.
        ImGuiWindowFlags panel_flags =
            ImGuiWindowFlags_NoMove     |
            ImGuiWindowFlags_NoResize   |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoScrollbar;

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT));
        ImGui::Begin("CalculatorPanel", nullptr, panel_flags);

        // --- Два числовых поля ввода ----------------------------------------
        ImGui::Text("Число А:");
        ImGui::InputFloat("##A", &state.numberA, 0.0f, 0.0f, "%.6g");
        if (ImGui::IsItemActive())
            state.hasResult = false; // при редактировании скрываем старый результат

        ImGui::Text("Число Б:");
        ImGui::InputFloat("##B", &state.numberB, 0.0f, 0.0f, "%.6g");
        if (ImGui::IsItemActive())
            state.hasResult = false;

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // --- Четыре кнопки операций в ряд ------------------------------------
        const float buttonWidth  = 90.0f;
        const float buttonHeight = 40.0f;
        const float spacing      = ImGui::GetStyle().ItemInnerSpacing.x;

        ImGui::PushItemWidth(buttonWidth);

        if (ImGui::Button("+", ImVec2(buttonWidth, buttonHeight)))
            PerformOperation(state, '+');
        ImGui::SameLine(0.0f, spacing);
        if (ImGui::Button("-", ImVec2(buttonWidth, buttonHeight)))
            PerformOperation(state, '-');
        ImGui::SameLine(0.0f, spacing);
        if (ImGui::Button("*", ImVec2(buttonWidth, buttonHeight)))
            PerformOperation(state, '*');
        ImGui::SameLine(0.0f, spacing);
        if (ImGui::Button("/", ImVec2(buttonWidth, buttonHeight)))
            PerformOperation(state, '/');

        ImGui::PopItemWidth();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // --- Вывод результата крупным текстом --------------------------------
        if (state.divideByZero)
        {
            // Ошибка: красный крупный текст вместо числа
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
            ImGui::SetWindowFontScale(2.5f);
            ImGui::Text("Ошибка: деление на ноль!");
            ImGui::SetWindowFontScale(1.0f);
            ImGui::PopStyleColor();
        }
        else if (state.hasResult)
        {
            ImGui::Text("Результат:");
            ImGui::SetWindowFontScale(2.5f);
            ImGui::Text("%.6g", (double)state.result);
            ImGui::SetWindowFontScale(1.0f);
        }
        else
        {
            ImGui::TextDisabled("Введите числа и нажмите кнопку операции.");
        }

        ImGui::End();

        // --- Отрисовка ---------------------------------------------------------
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.10f, 0.10f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // --- Завершение работы ---------------------------------------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
