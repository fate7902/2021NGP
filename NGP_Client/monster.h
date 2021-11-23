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



/*
나루토의 위치 : narutoCoord(컴그팀플.cpp drawScene에서 계속 해서 업데이트)
*/



//주인공이동(x,y,z순)

float M_pointx = 2.25f;
float M_pointz = -10.0f;

glm::vec3 MonsterCoord(2.25f, 0.0f, 0.0f);

float M_setat = 0.0f;      //이동시 다리가 움직이는 애니메이션을 하기위한 각도
float M_setam = 180.0f;      //몸통 회전을 맡음
float M_setaRighthand = 60.0f;
float M_setaLefthand = 60.0f;

//bool map2 = false;



void drawMonster(GLuint program, int vertexCount, GLuint vao[], GLuint vbo[], glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {

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
    transMatrixpoint = glm::translate(transMatrixpoint, glm::vec3(MonsterCoord.x, MonsterCoord.y, MonsterCoord.z));
    rotateMatrixm = glm::rotate(rotateMatrixm, glm::radians(M_setam), glm::vec3(0.0f, 1.0f, 0.0f));      //몸통 회전



    glUseProgram(program);
    //바디
    //월드변환 및 각 행렬 대입
    basicChange = glm::mat4(1.0f);      //초기화   
    transformMatrix = glm::mat4(1.0f);   //초기화
    basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
    transformMatrix = basicChange * transformMatrix;
    basicChange = glm::mat4(1.0f);      //초기화
    basicChange = glm::scale(basicChange, glm::vec3(5.0f, 4.0f, 2.5f));      //사이즈 조절
    transformMatrix = basicChange * transformMatrix;


    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(0.0, 0.8f, 0.0));      //바닥의 윗면으로 이동
    transformMatrix = basicChange * transformMatrix;
    transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
    transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
    transformMatrix = charTerrace * transformMatrix;      //난간위로 이동

    objColor = glm::vec3(0.1f, 0.1f, 0.1f);

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


    //머리
    //월드변환 및 각 행렬 대입
    transformMatrix = glm::mat4(1.0f);
    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));      //원점이동
    transformMatrix = basicChange * transformMatrix;
    basicChange = glm::mat4(1.0f);      //초기화
    basicChange = glm::scale(basicChange, glm::vec3(3.5f, 1.8f, 3.5f));      //사이즈 조절
    transformMatrix = basicChange * transformMatrix;
    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(0.0, 3.8f, 0.0));      //1층의 윗면으로 이동
    transformMatrix = basicChange * transformMatrix;
    transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
    transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
    transformMatrix = charTerrace * transformMatrix;
    objColor = glm::vec3(0.0f, 0.0f, 0.0f);
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

    //코
    transformMatrix = glm::mat4(1.0f);
    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
    transformMatrix = basicChange * transformMatrix;
    basicChange = glm::mat4(1.0f);      //초기화
    basicChange = glm::scale(basicChange, glm::vec3(0.3f, 0.2f, 0.4f));      //사이즈 조절
    transformMatrix = basicChange * transformMatrix;
    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(-0.3, 3.8f,-2.0));      //1층의 윗면으로 이동
    transformMatrix = basicChange * transformMatrix;
    transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
    transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
    transformMatrix = charTerrace * transformMatrix;
    objColor = glm::vec3(1.0f, 0.0f, 0.0f);
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

    //우측손
    //월드변환 및 각 행렬 대입
    transformMatrix = glm::mat4(1.0f);
    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
    transformMatrix = basicChange * transformMatrix;
    basicChange = glm::mat4(1.0f);      //초기화
    transformMatrix = scalehandfoot * transformMatrix;      //손크기
    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(0.0f, -0.25f, 0.0));      //회전축 이동
    transformMatrix = basicChange * transformMatrix;
    rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(M_setaRighthand), glm::vec3(1.0f, 0.0f, 0.0f));   //이동시 팔의 움직임
    transformMatrix = rotateMatrixt * transformMatrix;
    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(-2.8f, 0.9f, 0.0));      //본래의 위치로 이동
    transformMatrix = basicChange * transformMatrix;
    transformMatrix = rotateMatrixm * transformMatrix;         //몸통의 회전
    transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
    transformMatrix = charTerrace * transformMatrix;
    objColor = glm::vec3(0.0f, 0.0f, 0.0f);
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

    //좌측쪽손
    //월드변환 및 각 행렬 대입
    transformMatrix = glm::mat4(1.0f);
    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
    transformMatrix = basicChange * transformMatrix;
    basicChange = glm::mat4(1.0f);      //초기화
    transformMatrix = scalehandfoot * transformMatrix;      //손크기
    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(0.0f, -0.25f, 0.0));      //회전축 이동
    transformMatrix = basicChange * transformMatrix;
    rotateMatrixt = glm::mat4(1.0f);
    rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(M_setaLefthand), glm::vec3(1.0f, 0.0f, 0.0f));   //이동시 팔의 움직임
    transformMatrix = rotateMatrixt * transformMatrix;
    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(2.7f, 0.9f, 0.0));      //본래의 위치로 이동
    transformMatrix = basicChange * transformMatrix;
    transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
    transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
    transformMatrix = charTerrace * transformMatrix;
    objColor = glm::vec3(0.0f, 0.0f, 0.0f);
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

    //왼쪽 발
    //월드변환 및 각 행렬 대입
    transformMatrix = glm::mat4(1.0f);
    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
    transformMatrix = basicChange * transformMatrix;
    basicChange = glm::mat4(1.0f);      //초기화
    transformMatrix = scalehandfoot * transformMatrix;      //크기
    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(0.0f, -0.25f, 0.0));      //회전축 이동
    transformMatrix = basicChange * transformMatrix;
    rotateMatrixt = glm::mat4(1.0f);
    rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(-M_setat), glm::vec3(1.0f, 0.0f, 0.0f));   //이동시 발의 움직임
    transformMatrix = rotateMatrixt * transformMatrix;
    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(-1.3f, -2.0f, 0.0));      //본래의 위치로 이동
    transformMatrix = basicChange * transformMatrix;
    transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
    transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
    transformMatrix = charTerrace * transformMatrix;
    objColor = glm::vec3(0.0f, 0.0f, 0.0f);
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

    //오른쪽 발
    //월드변환 및 각 행렬 대입
    transformMatrix = glm::mat4(1.0f);
    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
    transformMatrix = basicChange * transformMatrix;
    basicChange = glm::mat4(1.0f);      //초기화
    transformMatrix = scalehandfoot * transformMatrix;      //크기
    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(0.0f, -0.25f, 0.0));      //회전축 이동
    transformMatrix = basicChange * transformMatrix;
    rotateMatrixt = glm::mat4(1.0f);
    if (leftWire || rightWire)
       rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(-M_setat), glm::vec3(1.0f, 0.0f, 0.0f));
    else
        rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(M_setat), glm::vec3(1.0f, 0.0f, 0.0f));

    transformMatrix = rotateMatrixt * transformMatrix;
    basicChange = glm::mat4(1.0f);
    basicChange = glm::translate(basicChange, glm::vec3(0.8, -0.0f, 0.0));      //본래의 위치로 이동
    transformMatrix = basicChange * transformMatrix;
    transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
    transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
    objColor = glm::vec3(0.0f, 0.0f, 0.0f);
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


  /* //와이어
    if (leftWire || rightWire) {
        transformMatrix = glm::mat4(1.0f);

        objColor = glm::vec3(0.0f, 0.0f, 0.0f);

        transformLocation = glGetUniformLocation(program, "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));
        objcolorLocation = glGetUniformLocation(program, "objectColor");
        glUniform3f(objcolorLocation, objColor.x, objColor.y, objColor.z);
        glBindVertexArray(vao[2]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(wireCoord), wireCoord, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_LINES, 0, 2);

    }*/
}



