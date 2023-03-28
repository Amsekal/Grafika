#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>


#define PI 3.14159265359

int main() {
    GLFWwindow* window = NULL;
    const GLubyte* renderer;
    const GLubyte* version;
    GLuint vao;

    GLuint vboCircle;
    GLuint vboColor;

    GLfloat x0 = 0.0f;
    GLfloat y0 = 0.0f;
    GLfloat r = 0.6f;

    GLfloat circle[306];
    GLfloat colors[306];

    GLfloat t;
    GLint i;

    GLfloat step = (2 * PI) / 100;

    const char* vertex_shader =
        "#version 410\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "out vec3 color;"
        "void main () {"
        "  color = vertex_color;"
        "  gl_Position = vec4(vertex_position, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 410\n"
        "in vec3 color;"
        "out vec4 frag_color;"
        "void main () {"
        "  frag_color = vec4(color, 1.0);"
        "}";

    GLuint vert_shader, frag_shader;
    GLuint shader_programme;

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

    ////// GENERATING CIRCLE POINTS ///////
    circle[0] = 0.0f;
    circle[1] = 0.0f;
    circle[2] = 0.0f;

    for (i =1 ; i < 102; i++) {
        t = i * step;
        circle[i * 3] = x0 + r * cos(t);
        circle[i * 3 + 1] = y0 + r * sin(t);
        circle[i * 3 + 2] = 0.0f;
    }

    colors[0] = 1.0f;
    colors[1] = 1.0f;
    colors[2] = 0.8f;

    for (i =1; i < 102; i++) {
        colors[i * 3] = 0.7f;
        colors[i * 3 + 1] = 0.3f;
        colors[i * 3 + 2] = 0.0f;
    }

    glGenBuffers(1, &vboCircle);
    glBindBuffer(GL_ARRAY_BUFFER, vboCircle);
    glBufferData(GL_ARRAY_BUFFER, 306 * sizeof(GLfloat), circle, GL_STATIC_DRAW);

    glGenBuffers(1, &vboColor);
    glBindBuffer(GL_ARRAY_BUFFER, vboColor);
    glBufferData(GL_ARRAY_BUFFER, 306 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vboCircle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, vboColor);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

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

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 102);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}