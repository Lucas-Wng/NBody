#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <iostream>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Simulation.h"
#include "shader.h"

// Callback function
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

float randomFloat(float min, float max) {
    static std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "N-Body Simulation with ImGui", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Set clear color and enable point size
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Dark gray background
    glEnable(GL_PROGRAM_POINT_SIZE); // Enable point size in shaders

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    Shader shader("src/shaders/nbody.vert", "src/shaders/nbody.frag");

    // Initialize the simulation with random particles
    Simulation simulation;
    for (int i = 0; i < 100; i++) {
        glm::vec3 randomPosition = glm::vec3(randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f));
        glm::vec3 randomVelocity = glm::vec3(randomFloat(-0.1f, 0.1f), randomFloat(-0.1f, 0.1f), randomFloat(-0.1f, 0.1f));
        float randomMass = randomFloat(0.5f, 1000.0f);

        Particle p{randomPosition, randomVelocity, randomMass};
        simulation.addParticle(p);

        // Debug output
        std::cout << "Particle " << i << ": Position ("
                  << randomPosition.x << ", " << randomPosition.y << ", " << randomPosition.z << ")" << std::endl;
    }

    // Setup OpenGL buffers
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, simulation.getParticlePositions().size() * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    // Create an orthographic projection matrix
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    shader.use();
    shader.setMat4("projection", projection);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Simulation update
        simulation.update(0.01f); // Update with a small time step

        // Update particle positions in OpenGL buffer
        std::vector<glm::vec3> positions = simulation.getParticlePositions();
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(glm::vec3), positions.data());

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui content
        ImGui::Begin("N-Body Simulation");
        ImGui::Text("Particle Count: %lu", positions.size());
        ImGui::End();

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Render particles
        shader.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, positions.size());

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
