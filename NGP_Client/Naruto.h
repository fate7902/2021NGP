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
#include "network.h"

bool gamestart = true;

bool version2 = false;
bool version3 = false;
bool perfectclear = false;

glm::vec3 narutoCoord[3]{ glm::vec3(0.0f, 0.0f, 0.0f) };

float setat = 0.0f;      //이동시 다리가 움직이는 애니메이션을 하기위한 각도

void drawNaruto(Network network, GLuint program, int vertexCount, GLuint vao[], GLuint vbo[],glm::mat4 viewMatrix, glm::mat4 projectionMatrix, int id) {
        //행렬 
        glm::mat4 transformMatrix(1.0f);      //월드변환
        glm::mat4 basicChange(1.0f);         //기본축사용시
        glm::mat4 basicChange2(1.0f);         //기본축사용시

        glm::mat4 rotateMatrixm(1.0f);         //명령m/M
        glm::mat4 rotateMatrixt(1.0f);         //명령t/T
        glm::mat4 scalehandfoot(1.0f);         //손과 발의 사이즈 책정
        glm::mat4 transMatrixpoint(1.0f);      //몸통의 이동
        glm::mat4 charTerrace(1.0f);
        charTerrace = glm::translate(charTerrace, glm::vec3(0.0f, 2.0f, 0.0f));

        //색깔
        glm::vec3 objColor(1.0f, 1.0f, 1.0f);
        //버텍스 연결(유니폼 연결)
        unsigned int transformLocation;
        unsigned int viewLocation;
        unsigned int projectionLocation;
        unsigned int objcolorLocation;

        //명령
        scalehandfoot = glm::scale(basicChange, glm::vec3(0.1f, 0.5f, 0.1f));         //손과발의 크기 축소(기본행렬)
        transMatrixpoint = glm::translate(transMatrixpoint, glm::vec3(narutoCoord[id].x, narutoCoord[id].y, narutoCoord[id].z));
        rotateMatrixm = glm::rotate(rotateMatrixm, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));      //몸통 회전


        glUseProgram(program);
        //바디
        //월드변환 및 각 행렬 대입
        basicChange = glm::mat4(1.0f);      //초기화   
        transformMatrix = glm::mat4(1.0f);   //초기화
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //초기화
        basicChange = glm::scale(basicChange, glm::vec3(1.0f, 0.5f, 1.0f));      //사이즈 조절
        transformMatrix = basicChange * transformMatrix;


        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0, 0.65f, 0.0));      //바닥의 윗면으로 이동
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
        transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
        transformMatrix = charTerrace * transformMatrix;      //난간위로 이동

        if (id == network.getMyId())
            objColor = glm::vec3(0.0, 1.0f, 0.0f);
        else
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


        //머리
        //월드변환 및 각 행렬 대입
        transformMatrix = glm::mat4(1.0f);
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));      //원점이동
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //초기화
        basicChange = glm::scale(basicChange, glm::vec3(0.7f, 0.3f, 0.7f));      //사이즈 조절
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0, 1.05f, 0.0));      //1층의 윗면으로 이동
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
        transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
        transformMatrix = charTerrace * transformMatrix;
        objColor = glm::vec3(0.8f, 0.8f, 0.8f);
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
        basicChange = glm::scale(basicChange, glm::vec3(0.3f, 0.1f, 0.1f));      //사이즈 조절
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0, 1.05f, 0.355));      //1층의 윗면으로 이동
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
        //rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(setaRighthand), glm::vec3(1.0f, 0.0f, 0.0f));   //이동시 팔의 움직임
        transformMatrix = rotateMatrixt * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.55f, 0.9f, 0.0));      //본래의 위치로 이동
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;         //몸통의 회전
        transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
        transformMatrix = charTerrace * transformMatrix;
        objColor = glm::vec3(0.2f, 0.2f, 1.0f);
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
        //rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(setaLefthand), glm::vec3(1.0f, 0.0f, 0.0f));   //이동시 팔의 움직임
        transformMatrix = rotateMatrixt * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.55f, 0.9f, 0.0));      //본래의 위치로 이동
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
        transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
        transformMatrix = charTerrace * transformMatrix;
        objColor = glm::vec3(0.2f, 0.2f, 1.0f);
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
        rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(-setat), glm::vec3(1.0f, 0.0f, 0.0f));   //이동시 발의 움직임
        transformMatrix = rotateMatrixt * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.25f, 0.5f, 0.0));      //본래의 위치로 이동
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
        transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
        transformMatrix = charTerrace * transformMatrix;
        objColor = glm::vec3(1.0f, 1.0f, 0.2f);
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
        rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(setat), glm::vec3(1.0f, 0.0f, 0.0f));

        transformMatrix = rotateMatrixt * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.25f, 0.5f, 0.0));      //본래의 위치로 이동
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //몸통의 회전
        transformMatrix = transMatrixpoint * transformMatrix;      //몸통의 이동
        transformMatrix = charTerrace * transformMatrix;
        objColor = glm::vec3(1.0f, 1.0f, 0.2f);
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


