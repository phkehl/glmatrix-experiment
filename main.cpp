// flipflip's glmatrix experiment
//
// Copyright (c) 2022 Philippe Kehl (flipflip at oinkzwurgl dot org),
// https://oinkzwurgl.org/hacking/glmatrix
//
// Permission to use, copy, modify, distribute, and sell this software and its documentation for any purpose is hereby
// granted without fee, provided that the above copyright notice appear in all copies and that both that copyright
// notice and this permission notice appear in supporting documentation.  No representations are made about the
// suitability of this software for any purpose.  It is provided "as is" without express or implied warranty.

#include <cstdio>
#include <stdlib.h>

#include "GL/gl3w.h"
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include "glmatrix.hpp"

#define UNUSED(thing) (void)thing
#define ERROR(fmt, ...)   fprintf(stderr, "\e[0;35m%07.3f \e[1;31mError: "   fmt "\e[m\n", glfwGetTime(), ## __VA_ARGS__)
#define WARNING(fmt, ...) fprintf(stderr, "\e[0;35m%07.3f \e[1;33mWarning: " fmt "\e[m\n", glfwGetTime(), ## __VA_ARGS__)
#define DEBUG(fmt, ...)   fprintf(stderr, "\e[0;35m%07.3f \e[0;36m"          fmt "\e[m\n", glfwGetTime(), ## __VA_ARGS__)

static void sGlfwErrorCallback(int error, const char* description)
{
    ERROR("GLFW error %d: %s", error, description);
}

static void sGlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    UNUSED(scancode);
    UNUSED(mods);
    if ( (key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS) )
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void sGlHandleDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    const char *sourceStr = "?";
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:               sourceStr   = "API";                          break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     sourceStr   = "WINDOW_SYSTEM";                break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:   sourceStr   = "SHADER_COMPILER";              break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:       sourceStr   = "THIRD_PARTY";                  break;
        case GL_DEBUG_SOURCE_APPLICATION:       sourceStr   = "APPLICATION";                  break;
        case GL_DEBUG_SOURCE_OTHER:             sourceStr   = "OTHER";                        break;
    }
    const char *typeStr = "?";
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               typeStr     = "ERROR";                        break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr     = "DEPRECATED_BEHAVIOR";          break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeStr     = "UNDEFINED_BEHAVIOR";           break;
        case GL_DEBUG_TYPE_PORTABILITY:         typeStr     = "PORTABILITY";                  break;
        case GL_DEBUG_TYPE_PERFORMANCE:         typeStr     = "PERFORMANCE";                  break;
        case GL_DEBUG_TYPE_MARKER:              typeStr     = "MARKER";                       break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          typeStr     = "PUSH_GROUP";                   break;
        case GL_DEBUG_TYPE_POP_GROUP:           typeStr     = "POP_GROUP";                    break;
        case GL_DEBUG_TYPE_OTHER:               typeStr     = "OTHER";                        break;
    }
    const char *severityStr = "?";
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:            severityStr = "HIGH";                         break;
        case GL_DEBUG_SEVERITY_MEDIUM:          severityStr = "MEDIUM";                       break;
        case GL_DEBUG_SEVERITY_LOW:             severityStr = "LOW";                          break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:    severityStr = "NOTIFICATION";                 break;
    }

    UNUSED(length);
    UNUSED(userParam);
    if ( (severity == GL_DEBUG_SEVERITY_MEDIUM) || (severity == GL_DEBUG_SEVERITY_HIGH) )
    {
        WARNING("GL (%s, %s, %s, %d) %s", sourceStr, typeStr, severityStr, id, message);
    }
    else
    {
        DEBUG("GL (%s, %s, %s, %d) %s", sourceStr, typeStr, severityStr, id, message);
    }
}

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    if (!gl3wInit())
    {
        ERROR("gl3wInit() fail!");
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(sGlfwErrorCallback);
    if (glfwInit() != GLFW_TRUE)
    {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    GLFWwindow *window = glfwCreateWindow(1280, 768, "flipflip's glmatrix experiment :-)", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    glfwSetWindowSizeLimits(window, 100, 100, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetKeyCallback(window, sGlfwKeyCallback);

    glDebugMessageCallback(sGlHandleDebug, nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    GlMatrix glMatrix;
    if (glMatrix.Init())
    {

        while (!glfwWindowShouldClose(window))
        {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

#if 0
            glViewport(0, 0, width, height);
            glClearColor(0.2, 0.0, 0.0, 0.5);
            glClear(GL_COLOR_BUFFER_BIT);
#else
            glMatrix.Animate();
            glMatrix.Render(width, height);
#endif

            glfwSwapBuffers(window);
            //DEBUG("frame %dx%d", width, height);

            glfwPollEvents();
        }
    }

    glMatrix.Destroy();

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}