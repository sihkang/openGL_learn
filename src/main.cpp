// #include <glad/glad.h>
#include "glad.c"
#include <GLFW/glfw3.h>

#define GLM_FORCE_SWIZZLE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>   // for quaternion
#include <glm/gtx/string_cast.hpp>  // for glm::to_string()
#include <glm/gtc/type_ptr.hpp>     // for glm::make_mat4()

#include "common.h"

uint32_t vert = 0;
uint32_t frag = 0;
uint32_t prog = 0;

uint32_t vao;
uint32_t vboP;
uint32_t vboB;
uint32_t ebo;

glm::mat4 matPyramid = glm::mat4( 1.0f );
glm::mat4 matBox = glm::mat4(1.0f);

GLfloat theta = 0;
std::chrono::system_clock::time_point lastTime = std::chrono::system_clock::now();


glm::vec4 vertBox[] = {
    { -0.5F, -0.5F, -0.5F, 1.0F }, { 1.0F, 0.3F, 0.3F, 1.0F },// v0
	{ +0.5F, -0.5F, -0.5F, 1.0F }, { 1.0F, 0.3F, 0.3F, 1.0F },// v1
	{ +0.5F, -0.5F, +0.5F, 1.0F }, { 1.0F, 0.3F, 0.3F, 1.0F },// v2
	{ -0.5F, -0.5F, +0.5F, 1.0F }, { 1.0F, 0.3F, 0.3F, 1.0F },// v3
	{ -0.5F, +0.5F, -0.5F, 1.0F }, { 0.3F, 1.0F, 1.0F, 1.0F },// v4
	{ +0.5F, +0.5F, -0.5F, 1.0F }, { 1.0F, 0.3F, 1.0F, 1.0F },// v5
	{ +0.5F, +0.5F, +0.5F, 1.0F }, { 1.0F, 1.0F, 0.3F, 1.0F },// v6
	{ -0.5F, +0.5F, +0.5F, 1.0F }, { 1.0F, 1.0F, 0.3F, 1.0F },// v7
};

glm::vec4 vertPyramid[] = { // 5 vertices
	{ 0.0F, 0.5F, 0.0F, 1.0F }, { 1.0F, 1.0F, 1.0F, 1.0F, },// v0
	{ 0.5F, -0.3F, 0.0F, 1.0F }, { 1.0F, 0.3F, 0.3F, 1.0F, },// v1
	{ 0.0F, -0.3F, -0.5F, 1.0F }, { 0.3F, 1.0F, 0.3F, 1.0F, },// v2
	{ -0.5F, -0.3F, 0.0F, 1.0F }, { 0.3F, 0.3F, 1.0F, 1.0F, },// v3
	{ 0.0F, -0.3F, 0.5F, 1.0F }, { 1.0F, 1.0F, 0.3F, 1.0F, }// v4
};

GLuint indicesBox[] = {
    0, 3, 2, 0, 2, 1,
    1, 2, 6, 1, 6, 5,
    2, 3, 7, 2, 7, 6,
    3, 0, 4, 3, 4, 7,
    1, 5, 4, 1, 4, 0,
    4, 5, 6, 4, 6, 7,
};

GLuint indicesPyramid[] = { // 6 * 3 = 18 indices
	0, 1, 2, // face 0: v0-v1-v2
	0, 2, 3, // face 1: v0-v2-v3
	0, 3, 4, // face 2: v0-v3-v4
	0, 4, 1, // face 3: v0-v4-v1
	1, 4, 3, // face 4: v1-v4-v3
	1, 3, 2, // face 5: v1-v3-v2
};

void initFunc(void) 
{
    // vert: vertex shader
    auto vertexShader = LoadTextFile("/Users/sihkang/openGL/shader/vertex.vs");
    auto fragmentShader = LoadTextFile("/Users/sihkang/openGL/shader/fragment.fs");
        
    const char* vs = vertexShader.c_str();
    const char* fs = fragmentShader.c_str();

    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vs, NULL);
    glCompileShader(vert); // compile to get .OBJ

    // frag: fragment shader
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fs, NULL);
    glCompileShader(frag); // compile to get .OBJ

    // prog: program
    prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog); // link to get .EXE

    int32_t success = 0;
    char* infoLog = (char *)malloc(sizeof(char) * 1024);
    // Vertex Shader 확인
    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vert, 512, NULL, infoLog);
        std::cerr << "Vertex Shader Compilation Error:\n" << infoLog << std::endl;
        exit(1);
    }
    std::clog << "Vertex Shader Complete : " << vert << "\n";

    // Fragment Shader 확인
    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag, 512, NULL, infoLog);
        std::cerr << "Fragment Shader Compilation Error:\n" << infoLog << std::endl;
        exit(1);
    }
    std::clog << "fragment Shader Complete : " << frag << "\n";

    // Program Link 확인
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(prog, 512, NULL, infoLog);
        std::cerr << "Shader Program Linking Error:\n" << infoLog << std::endl;
        exit(1);
    }
    std::clog << "Program create Complete : " << prog << "\n";
    free(infoLog);
    
    // execute it!
    glUseProgram(prog);

    glEnable(GL_DEPTH_TEST);
    glDepthRange(0.0, 1.0);
    glClearDepthf(1.0f);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPyramid) + sizeof(indicesBox), 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indicesPyramid), indicesPyramid);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPyramid), sizeof(indicesBox), indicesBox);
}



void updateFunc()
{
    std::chrono::system_clock::time_point curTime = std::chrono::system_clock::now();
    std::chrono::milliseconds elapsedTimeMSEC = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - lastTime); // in millisecond
    theta = (elapsedTimeMSEC.count() / 1000.0F) * (float)M_PI; // in <math.h>, M_PI_2 = pi/2
    
    matPyramid = glm::mat4(1.0f);
    matPyramid = glm::translate(matPyramid, glm::vec3(-0.4f, 0.0f, 0.0f));
    matPyramid = glm::rotate(matPyramid, theta, glm::vec3(0.0f, 1.0f, 0.0f));
    matPyramid = glm::scale(matPyramid, glm::vec3(0.5f, 0.5f, 0.5f));

    matBox = glm::mat4(1.0f);
    matBox = glm::translate(matBox, glm::vec3(0.4f, 0.0f, 0.0f));
    matBox = glm::rotate(matBox, theta, glm::vec3(1.0f, 0.0f, 0.0f));
    matBox = glm::scale(matBox, glm::vec3(0.3f, 0.3f, 0.3f));
}

void drawFunc()
{    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLuint locMat = glGetUniformLocation(prog, "uMat");
    glUniformMatrix4fv(locMat, 1, GL_FALSE, glm::value_ptr(matPyramid));

    glGenBuffers(1, &vboP);
    glBindBuffer(GL_ARRAY_BUFFER, vboP);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertPyramid), glm::value_ptr(vertPyramid[0]), GL_STATIC_DRAW);

    GLuint locPos = glGetAttribLocation(prog, "aPos");
    glVertexAttribPointer(locPos, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 2, 0);
    glEnableVertexAttribArray(locPos);
    GLuint locCol = glGetAttribLocation(prog, "aColor");
    glVertexAttribPointer(locCol, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 2, (void *)sizeof(glm::vec4));
    glEnableVertexAttribArray(locCol);

    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

    glUniformMatrix4fv(locMat, 1, GL_FALSE, glm::value_ptr(matBox));

    glGenBuffers(1, &vboB);
    glBindBuffer(GL_ARRAY_BUFFER, vboB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertBox), glm::value_ptr(vertBox[0]), GL_STATIC_DRAW);
    locPos = glGetAttribLocation(prog, "aPos");
    glVertexAttribPointer(locPos, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 2, 0);
    // glEnableVertexAttribArray(locPos);
    locCol = glGetAttribLocation(prog, "aColor");
    glVertexAttribPointer(locCol, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 2, (void *)sizeof(glm::vec4));
    // glEnableVertexAttribArray(locCol);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void *)(sizeof(GLuint) * 18));

    // glDrawArrays(GL_TRIANGLES, 0, 18);
    // glFinish();
}

void refreshFunc(GLFWwindow* window) 
{
    // refresh
    printf("refresh called\n");
    drawFunc();

    // GLFW action
    glfwSwapBuffers(window);
}

void keyboardCBFunc(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
        case (GLFW_KEY_ESCAPE):
            if (action == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                printf("Escape key pressed, exit.\n");
            }
            break;

        case (GLFW_KEY_R):
            if (action == GLFW_PRESS)
            {
                lastTime = std::chrono::system_clock::now();
            }
            break;
       
        default:
            break;
    }
}

int main(int argc, char* argv[])
{
    // get your program name
    #if defined(_WIN32) || defined(_WIN64)
    char* win_name = (strrchr(argv[0], '\\') == NULL) ? argv[0] : (strrchr(argv[0], '\\') + 1);
    #else // Unix, Linux, MacOS
    char* win_name = (strrchr(argv[0], '/') == NULL) ? argv[0] : (strrchr(argv[0], '/') + 1);
    #endif

    // start GLFW & GLEW
    glfwInit();

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL 4.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // OpenGL x.1
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Core profile

    GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, win_name, NULL, NULL);
    glfwSetWindowPos(window, WIN_X, WIN_Y);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cerr << "Failed to initialize Glad!" << std::endl;
        return -1;
    }
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // prepare
    glfwSetWindowRefreshCallback(window, refreshFunc);
    glfwSetKeyCallback(window, keyboardCBFunc);
    glClearColor( 0.5F, 0.8F, 0.8F, 1.0F );

    // main loop
    initFunc();
    while (!glfwWindowShouldClose(window)) 
    {
        // draw
        updateFunc();
        drawFunc();

        // GLFW actions
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // done
    glfwTerminate();
    return 0;
}