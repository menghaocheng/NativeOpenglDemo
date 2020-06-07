//
// Created by yangw on 2019-2-23.
//

#ifndef NATIVEOPENGLDEMO_WLSHADERUTIL_H
#define NATIVEOPENGLDEMO_WLSHADERUTIL_H

#include <GLES2/gl2.h>
#include "../log/WlAndroidLog.h"
#include "errno.h"

static int loadShaders(int shaderType, const char *code)
{
    int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &code, 0);
    glCompileShader(shader);
    return  shader;
}

static int createProgrm(const char *vertex , const char * fragment)
{
    int vertexShader = loadShaders(GL_VERTEX_SHADER, vertex);
    if(vertexShader <= 0){
        LOGE("loadShaders(vertexShader=%d) failed:%s", vertexShader, strerror(errno));
        return vertexShader;
    }
    int fragmentShader = loadShaders(GL_FRAGMENT_SHADER, fragment);
    if(fragmentShader <= 0){
        LOGE("loadShaders(fragmentShader=%d) failed:%s", fragmentShader, strerror(errno));
        return fragmentShader;
    }
    int program = glCreateProgram();
    if(program <= 0){
        LOGE("glCreateProgram(program=%d) failed:%s", program, strerror(errno));
        return program;
    }
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    return program;
}




#endif //NATIVEOPENGLDEMO_WLSHADERUTIL_H
