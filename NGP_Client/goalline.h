#pragma once
#pragma once
#include <iostream>
#include <cstdio>
#include <conio.h>
#include <gl/glew.h>
#include <gl/glut.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <ctime>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "sound.h"
#include "map.h"

float G_pointx = 2.25f;
float G_pointz = 0.0f;

glm::vec3 GoalLineCoord(0.0, 0.0f, 0.0f);

float G_setam = 180.0f; 
float G_setaRighthand = 60.0f;
float G_setaLefthand = 60.0f;



void drawGoalLine(GLuint program, int vertexCount, GLuint vao[], GLuint vbo[], glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {

    //행렬 
    glm::mat4 transformMatrix(1.0f);      //월드변환
    glm::mat4 basicChange(1.0f);         //기본축사용시
    glm::mat4 basicChange2(1.0f);         //기본축사용시

    glm::mat4 rotateMatrixm(1.0f);         //명령m/M
    glm::mat4 rotateMatrixt(1.0f);         //명령t/T
    glm::mat4 scalehandfoot(5.0f);         //손과 발의 사이즈 책정
    glm::mat4 transMatrixpoint(1.0f);      //몸통의 이동
    glm::mat4 charTerrace(1.0f);
    charTerrace = glm::translate(charTerrace, glm::vec3(0.0f, 2.0f, 0.0f));

    //색깔
    glm::vec3 objColor(0.0f, 0.0f, 0.0f);
    //버텍스 연결(유니폼 연결)
    unsigned int transformLocation;
    unsigned int viewLocation;
    unsigned int projectionLocation;
    unsigned int objcolorLocation;

    //명령
    scalehandfoot = glm::scale(basicChange, glm::vec3(0.5f, 2.5f, 0.5f));         //손과발의 크기 축소(기본행렬)
    transMatrixpoint = glm::translate(transMatrixpoint, glm::vec3(GoalLineCoord.x, GoalLineCoord.y, GoalLineCoord.z));
    rotateMatrixm = glm::rotate(rotateMatrixm, glm::radians(G_setam), glm::vec3(0.0f, 1.0f, 0.0f));      //몸통 회전



    glUseProgram(program);
    //바디
    //월드변환 및 각 행렬 대입
    basicChange = glm::mat4(1.0f);      //초기화   
    transformMatrix = glm::mat4(1.0f);   //초기화
    basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
    transformMatrix = basicChange * transformMatrix;
    basicChange = glm::mat4(1.0f);      //초기화
    basicChange = glm::scale(basicChange, glm::vec3(18.0f, 1.7f, 3.0f));      //사이즈 조절
    transformMatrix = basicChange * transformMatrix;


    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(0.0, 0.0f, GOALLINE));      //바닥의 윗면으로 이동
    transformMatrix = basicChange * transformMatrix;
    transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
    transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
    transformMatrix = charTerrace * transformMatrix;      //난간위로 이동

    objColor = glm::vec3(0.0f, 0.0f, 1.0f);

    transformLocation = glGetUniformLocation(program, "transform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
    viewLocation = glGetUniformLocation(program, "viewTransform");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    projectionLocation = glGetUniformLocation(program, "projectionTransform");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
    objcolorLocation = glGetUniformLocation(program, "objectColor");
    glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);
    glBindVertexArray(vao[1]);
    glDrawElements(GL_TRIANGLES, vertexCount * 3, GL_UNSIGNED_INT, 0);







}