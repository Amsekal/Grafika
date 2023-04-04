#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#define N 100
#define M 4


int main() {
    GLFWwindow* window = NULL;
    const GLubyte* renderer;
    const GLubyte* version;
    GLuint vao;
    GLuint vbo;

    GLuint vao2;
    GLuint vbo2;

    GLuint vao3;
    GLuint vbo3;

    GLfloat P1x = -0.5f;
    GLfloat P1y = 0.0f;

    GLfloat P2x = -0.25f;
    GLfloat P2y = 0.5f;

    GLfloat P3x = 0.25f;
    GLfloat P3y = 0.5f;

    GLfloat P4x = 0.5f;
    GLfloat P4y = 0.0f;

    GLfloat HermiteCurve[3*N];
    GLfloat HermiteLines[3 * M];

    GLfloat t;
    GLint i;

    GLfloat step = 3.0f / (N-1); // t \in [-1,2]
    GLfloat step2 = 3.0f / (M-1); // t \in [-1,2]

    const char* vertex_shader =
        "#version 410\n"
        "in vec3 vp;"
        "void main () {"
        "  gl_Position = vec4(vp, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 410\n"
        "out vec4 frag_colour;"
        "void main () {"
        "  frag_colour = vec4(1.0, 1.0, 0.0, 1.0);"
        "}";

    const char* vertex_shader2 =
        "#version 410\n"
        "in vec3 vp;"
        "void main () {"
        "  gl_Position = vec4(vp, 1.0);"
        "}";

    const char* fragment_shader2 =
        "#version 410\n"
        "out vec4 frag_colour;"
        "void main () {"
        "  frag_colour = vec4(1.0, 0.0, 0.0, 1.0);"
        "}";


    const char* vertex_shader3 =
        "#version 410\n"
        "in vec3 vp;"
        "void main () {"
        "  gl_Position = vec4(vp, 1.0);"
        "}";

    const char* fragment_shader3 =
        "#version 410\n"
        "out vec4 frag_colour;"
        "void main () {"
        "  frag_colour = vec4(0.0, 1.0, 0.4, 1.0);"
        "}";

    GLuint vert_shader, frag_shader, vert_shader2, frag_shader2, vert_shader3, frag_shader3;
    GLuint shader_programme, shader_programme2, shader_programme3;

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 640, "Hello Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    renderer = glGetString(GL_RENDERER);
    version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);

    ////// GENERATING THE POINTS OF THE HERMITE CURVE ///////

    HermiteCurve[0] = P1x;
    HermiteCurve[1] = P1y;
    HermiteCurve[2] = 0.0f;

    for (i = 1; i < N-1; i++) {
        t = -1 + i * step;
        HermiteCurve[i * 3] = P1x * (-1.0f / 6.0f * t * t * t + 1.0f / 2.0f * t * t - 1.0f / 3.0f * t) + P2x * (1.0f / 2.0f * t * t * t - t * t - 1.0f / 2.0f * t + 1) + P3x * (-1.0f / 2.0f * t * t * t + 1.0f / 2.0f * t * t + t) + P4x * (1.0f / 6.0f * t * t * t - 1.0f / 6.0f * t);
        HermiteCurve[i * 3 + 1] = P1y * (-1.0f / 6.0f * t * t * t + 1.0f / 2.0f * t * t - 1.0f / 3.0f * t) + P2y * (1.0f / 2.0f * t * t * t - t * t - 1.0f / 2.0f * t + 1) + P3y * (-1.0f / 2.0f * t * t * t + 1.0f / 2.0f * t * t + t) + P4y * (1.0f / 6.0f * t * t * t - 1.0f / 6.0f * t);
        HermiteCurve[i * 3 + 2] = 0.0f;
    }

    HermiteCurve[297] = P4x;
    HermiteCurve[298] = P4y;
    HermiteCurve[299] = 0.0f;


    HermiteLines[0] = P1x;
    HermiteLines[1] = P1y;
    HermiteLines[2] = 0.0f;

    for (i = 1; i < M-1; i++) {
        t = -1 + i * step2;
        HermiteLines[i * 3] = P1x * (-1.0f / 6.0f * t * t * t + 1.0f / 2.0f * t * t - 1.0f / 3.0f * t) + P2x * (1.0f / 2.0f * t * t * t - t * t - 1.0f / 2.0f * t + 1) + P3x * (-1.0f / 2.0f * t * t * t + 1.0f / 2.0f * t * t + t) + P4x * (1.0f / 6.0f * t * t * t - 1.0f / 6.0f * t);
        HermiteLines[i * 3 + 1] = P1y * (-1.0f / 6.0f * t * t * t + 1.0f / 2.0f * t * t - 1.0f / 3.0f * t) + P2y * (1.0f / 2.0f * t * t * t - t * t - 1.0f / 2.0f * t + 1) + P3y * (-1.0f / 2.0f * t * t * t + 1.0f / 2.0f * t * t + t) + P4y * (1.0f / 6.0f * t * t * t - 1.0f / 6.0f * t);
        HermiteLines[i * 3 + 2] = 0.0f;
    }

    HermiteLines[9] = P4x;
    HermiteLines[10] = P4y;
    HermiteLines[11] = 0.0f;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3*N * sizeof(GLfloat), HermiteCurve, GL_STATIC_DRAW);

        glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, 3 * M * sizeof(GLfloat), HermiteLines, GL_STATIC_DRAW);



    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);





    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);






    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vertex_shader, NULL);
    glCompileShader(vert_shader);

    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &fragment_shader, NULL);
    glCompileShader(frag_shader);

    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, frag_shader);
    glAttachShader(shader_programme, vert_shader);
    glLinkProgram(shader_programme);

    vert_shader2 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader2, 1, &vertex_shader2, NULL);
    glCompileShader(vert_shader2);

    frag_shader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader2, 1, &fragment_shader2, NULL);
    glCompileShader(frag_shader2);

    shader_programme2 = glCreateProgram();
    glAttachShader(shader_programme2, frag_shader2);
    glAttachShader(shader_programme2, vert_shader2);
    glLinkProgram(shader_programme2);

    vert_shader3 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader3, 1, &vertex_shader3, NULL);
    glCompileShader(vert_shader3);

    frag_shader3 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader3, 1, &fragment_shader3, NULL);
    glCompileShader(frag_shader3);

    shader_programme3 = glCreateProgram();
    glAttachShader(shader_programme3, frag_shader3);
    glAttachShader(shader_programme3, vert_shader3);
    glLinkProgram(shader_programme3);

    glPointSize(5.0f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);

        glUseProgram(shader_programme);
       
        glDrawArrays(GL_LINE_STRIP, 0, N);
  
        glUseProgram(shader_programme2);
        glBindVertexArray(vao2);
        glDrawArrays(GL_LINE_STRIP, 0, M);
        glUseProgram(shader_programme3);
        glDrawArrays(GL_POINTS, 0, M);
       
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}