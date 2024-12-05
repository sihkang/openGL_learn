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
uint32_t vbo;
uint32_t vbo_color;
uint32_t ebo;

glm::vec4 moveCur = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

int mousePressed = GL_FALSE;
glm::vec2 dragStart;

glm::mat4 mat = glm::mat4( 1.0f );
glm::mat4 matDrag = glm::mat4( 1.0f );
glm::mat4 matUpdated = glm::mat4( 1.0f );

GLfloat theta = 0;
glm::vec3 angle = { 0.0f, 0.0f, 0.0f };
glm::vec3 dir = {0.0f, 0.0f, 0.0f };
std::chrono::system_clock::time_point lastTime = std::chrono::system_clock::now();


glm::vec4 vertPos[] = { // 5 vertices
	{ 0.0F, 0.5F, 0.0F, 1.0F }, // v0
	{ 0.5F, -0.3F, 0.0F, 1.0F }, // v1
	{ 0.0F, -0.3F, -0.5F, 1.0F }, // v2
	{ -0.5F, -0.3F, 0.0F, 1.0F }, // v3
	{ 0.0F, -0.3F, 0.5F, 1.0F }, // v4
};

glm::vec4 vertColor[] = { // 5 colors
	{ 1.0F, 1.0F, 1.0F, 1.0F, }, // v0: white
	{ 1.0F, 0.3F, 0.3F, 1.0F, }, // v1: red
	{ 0.3F, 1.0F, 0.3F, 1.0F, }, // v2: green
	{ 0.3F, 0.3F, 1.0F, 1.0F, }, // v3: blue
	{ 1.0F, 1.0F, 0.3F, 1.0F, }, // v4: yellow
};

GLuint indices[] = { // 6 * 3 = 18 indices
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

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertPos), glm::value_ptr(vertPos[0]), GL_STATIC_DRAW);
    GLuint locPos = glGetAttribLocation(prog, "aPos");
    glVertexAttribPointer(locPos, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
    glEnableVertexAttribArray(locPos);

    glGenBuffers(1, &vbo_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertColor), glm::value_ptr(vertColor[0]), GL_STATIC_DRAW);
    GLuint locColor = glGetAttribLocation(prog, "aColor");
    glVertexAttribPointer(locColor, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
    glEnableVertexAttribArray(locColor);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}



void updateFunc()
{
    // std::chrono::system_clock::time_point curTime = std::chrono::system_clock::now();
    // std::chrono::milliseconds elapsedTimeMSEC = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - lastTime); // in millisecond
    // theta = (elapsedTimeMSEC.count() / 1000.0F) * (float)M_PI; // in <math.h>, M_PI_2 = pi/2
    // angle += theta * dir;
    // lastTime = curTime;

    // mat = glm::mat4(1.0f);
    // mat = glm::rotate(mat, angle.z, glm::vec3(0.0f, 0.0f, 1.0f));
    // mat = glm::rotate(mat, angle.y, glm::vec3(0.0f, 1.0f, 0.0f));
    // mat = glm::rotate(mat, angle.x, glm::vec3(1.0f, 0.0f, 0.0f));
}

int cullMode = 0;

void drawFunc()
{    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLuint locMat = glGetUniformLocation(prog, "uMat");
    glUniformMatrix4fv(locMat, 1, GL_FALSE, glm::value_ptr(mat));

    // glDrawArrays(GL_TRIANGLES, 0, 18);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
    // glFinish();
}

glm::vec3 calcUnitVec(const glm::vec2& raw)
{
    glm::vec2 scr = raw;

    std::cout << raw.x << ", " << raw.y << " | ";
    scr.x = std::clamp(raw.x, 0.0F, (float)WIN_W);
    scr.y = std::clamp(raw.y, 0.0F, (float)WIN_H);
    std::cout << scr.x << ", " << scr.y << std::endl;

    // normal processing
    const GLfloat radius = sqrtf(WIN_W * WIN_W + WIN_H * WIN_H) / 2.0f;
    glm::vec3 v;
    v.x = (scr.x - WIN_W / 2.0f) / radius;
    v.y = (WIN_H / 2.0f - scr.y) / radius;
    v.z = sqrtf(1.0f - v.x * v.x - v.y * v.y);
    return v;
}

glm::mat4 calcTrackball(const glm::vec2& start, const glm::vec2& cur)
{
    glm::vec3 org = calcUnitVec( start );
    glm::vec3 dst = calcUnitVec( cur );
    glm::quat q = glm::rotation( org, dst );
    glm::mat4 m = glm::toMat4( q );
    return m;
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
        // can add more key actions

        case (GLFW_KEY_Q):
            if (action == GLFW_PRESS)
                dir.x = 1.0f;
            break;
        
        case (GLFW_KEY_W):
            if (action == GLFW_PRESS)
                dir.y = 1.0f;
            break;
        
        case (GLFW_KEY_E):
            if (action == GLFW_PRESS)
                dir.z = 1.0f;
            break;
        
        case (GLFW_KEY_A):
            if (action == GLFW_PRESS)
                dir.x = 0.0f;
            break;
        
        case (GLFW_KEY_S):
            if (action == GLFW_PRESS)
                dir.y = 0.0f;
            break;
        
        case (GLFW_KEY_D):
            if (action == GLFW_PRESS)
                dir.z = 0.0f;
            break;
        
        case (GLFW_KEY_Z):
            if (action == GLFW_PRESS)
                dir.x = -1.0f;
            break;
        
        case (GLFW_KEY_X):
            if (action == GLFW_PRESS)
                dir.y = -1.0f;
            break;
        
        case (GLFW_KEY_C):
            if (action == GLFW_PRESS)
                dir.z = -1.0f;
            break;
        
        default:
            break;
    }
}

void cursorEnterFunc(GLFWwindow* win, int entered) {
	printf("cursor %s the window\n", (entered == GL_FALSE) ? "leaving" : "entering");
	fflush(stdout);
}

void cursorPosFunc(GLFWwindow* win, double xscr, double yscr)
{
    if (mousePressed == GL_TRUE)
    {
        glm::vec2 dragCur = glm::vec2((GLfloat)xscr, (GLfloat)yscr);
        matDrag = calcTrackball( dragStart, dragCur );
        mat = matDrag * matUpdated;
    }
}

void mouseButtonFunc(GLFWwindow* win, int button, int action, int mods)
{
    GLdouble x, y;
    switch (action)
    {
    case GLFW_PRESS:
        mousePressed = GL_TRUE;
        glfwGetCursorPos(win, &x, &y);
        dragStart = glm::vec2((GLfloat)x, (GLfloat)y);
        break;

    case GLFW_RELEASE:
        mousePressed = GL_FALSE;
        glfwGetCursorPos(win, &x, &y);
        glm::vec2 dragCur = glm::vec2((GLfloat)x, (GLfloat)y);
        matDrag = calcTrackball( dragStart, dragCur );
        mat = matDrag * matUpdated;
        matDrag = glm::mat4(1.0f);
        matUpdated = mat;
        break;
    }
    fflush(stdout);
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
    // glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

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
    glfwSetCursorEnterCallback(window, cursorEnterFunc);
    glfwSetCursorPosCallback(window, cursorPosFunc);
    glfwSetMouseButtonCallback(window, mouseButtonFunc);
    glClearColor( 0.5F, 0.8F, 0.8F, 1.0F );

    // main loop
    initFunc();
    while (!glfwWindowShouldClose(window)) 
    {
        // draw
        // updateFunc();
        drawFunc();

        // GLFW actions
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // done
    glfwTerminate();
    return 0;
}