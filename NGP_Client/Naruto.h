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

/*
�������� ��ġ : narutoCoord(�ı�����.cpp drawScene���� ��� �ؼ� ������Ʈ)
*/

//��� ����
bool boolw = false;
bool boola = false;
bool boolgravity = false;
bool boold = false;
bool boolj = false;
bool boolj2 = false;
bool gameclear = false;
bool sound_6 = true;
bool sound_7 = true;
//���ΰ��̵�(x,y,z��)
float transj = 0.0f;
float pointx = 2.25f;
float pointz = 0.0f;

glm::vec3 narutoCoord[3]{ glm::vec3(0.0f, 0.0f, 0.0f) };


float setat = 0.0f;      //�̵��� �ٸ��� �����̴� �ִϸ��̼��� �ϱ����� ����
float setam = 180.0f;      //���� ȸ���� ����
float setaRighthand = 60.0f;
float setaLefthand = 60.0f;

bool map2 = false;

void drawNaruto(GLuint program, int vertexCount, GLuint vao[], GLuint vbo[],glm::mat4 viewMatrix, glm::mat4 projectionMatrix, int id) {
    //for (const auto& client : net.clients) {
          

        //��� 
        glm::mat4 transformMatrix(1.0f);      //���庯ȯ
        glm::mat4 basicChange(1.0f);         //�⺻�����
        glm::mat4 basicChange2(1.0f);         //�⺻�����

        glm::mat4 rotateMatrixm(1.0f);         //���m/M
        glm::mat4 rotateMatrixt(1.0f);         //���t/T
        glm::mat4 scalehandfoot(1.0f);         //�հ� ���� ������ å��
        glm::mat4 transMatrixpoint(1.0f);      //������ �̵�
        glm::mat4 charTerrace(1.0f);
        charTerrace = glm::translate(charTerrace, glm::vec3(0.0f, 2.0f, 0.0f));

        //����
        glm::vec3 objColor(1.0f, 1.0f, 1.0f);
        //���ؽ� ����(������ ����)
        unsigned int transformLocation;
        unsigned int viewLocation;
        unsigned int projectionLocation;
        unsigned int objcolorLocation;

        //���
        scalehandfoot = glm::scale(basicChange, glm::vec3(0.1f, 0.5f, 0.1f));         //�հ����� ũ�� ���(�⺻���)
        transMatrixpoint = glm::translate(transMatrixpoint, glm::vec3(narutoCoord[id].x, narutoCoord[id].y, narutoCoord[id].z));
        rotateMatrixm = glm::rotate(rotateMatrixm, glm::radians(setam), glm::vec3(0.0f, 1.0f, 0.0f));      //���� ȸ��


        glUseProgram(program);
        //�ٵ�
        //���庯ȯ �� �� ��� ����
        basicChange = glm::mat4(1.0f);      //�ʱ�ȭ   
        transformMatrix = glm::mat4(1.0f);   //�ʱ�ȭ
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //�ʱ�ȭ
        basicChange = glm::scale(basicChange, glm::vec3(1.0f, 0.5f, 1.0f));      //������ ����
        transformMatrix = basicChange * transformMatrix;


        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0, 0.65f, 0.0));      //�ٴ��� �������� �̵�
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //������ ȸ��
        transformMatrix = transMatrixpoint * transformMatrix;      //������ �̵�
        transformMatrix = charTerrace * transformMatrix;      //�������� �̵�

        objColor = glm::vec3(1.0f, 0.2f, 0.2f);

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


        //�Ӹ�
        //���庯ȯ �� �� ��� ����
        transformMatrix = glm::mat4(1.0f);
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));      //�����̵�
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //�ʱ�ȭ
        basicChange = glm::scale(basicChange, glm::vec3(0.7f, 0.3f, 0.7f));      //������ ����
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0, 1.05f, 0.0));      //1���� �������� �̵�
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //������ ȸ��
        transformMatrix = transMatrixpoint * transformMatrix;      //������ �̵�
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

        //��
        transformMatrix = glm::mat4(1.0f);
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //�ʱ�ȭ
        basicChange = glm::scale(basicChange, glm::vec3(0.3f, 0.1f, 0.1f));      //������ ����
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0, 1.05f, 0.355));      //1���� �������� �̵�
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //������ ȸ��
        transformMatrix = transMatrixpoint * transformMatrix;      //������ �̵�
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

        //������
        //���庯ȯ �� �� ��� ����
        transformMatrix = glm::mat4(1.0f);
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //�ʱ�ȭ
        transformMatrix = scalehandfoot * transformMatrix;      //��ũ��
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0f, -0.25f, 0.0));      //ȸ���� �̵�
        transformMatrix = basicChange * transformMatrix;
        rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(setaRighthand), glm::vec3(1.0f, 0.0f, 0.0f));   //�̵��� ���� ������
        transformMatrix = rotateMatrixt * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.55f, 0.9f, 0.0));      //������ ��ġ�� �̵�
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;         //������ ȸ��
        transformMatrix = transMatrixpoint * transformMatrix;      //������ �̵�
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

        //�����ʼ�
        //���庯ȯ �� �� ��� ����
        transformMatrix = glm::mat4(1.0f);
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //�ʱ�ȭ
        transformMatrix = scalehandfoot * transformMatrix;      //��ũ��
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0f, -0.25f, 0.0));      //ȸ���� �̵�
        transformMatrix = basicChange * transformMatrix;
        rotateMatrixt = glm::mat4(1.0f);
        rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(setaLefthand), glm::vec3(1.0f, 0.0f, 0.0f));   //�̵��� ���� ������
        transformMatrix = rotateMatrixt * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.55f, 0.9f, 0.0));      //������ ��ġ�� �̵�
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //������ ȸ��
        transformMatrix = transMatrixpoint * transformMatrix;      //������ �̵�
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

        //���� ��
        //���庯ȯ �� �� ��� ����
        transformMatrix = glm::mat4(1.0f);
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //�ʱ�ȭ
        transformMatrix = scalehandfoot * transformMatrix;      //ũ��
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0f, -0.25f, 0.0));      //ȸ���� �̵�
        transformMatrix = basicChange * transformMatrix;
        rotateMatrixt = glm::mat4(1.0f);
        rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(-setat), glm::vec3(1.0f, 0.0f, 0.0f));   //�̵��� ���� ������
        transformMatrix = rotateMatrixt * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.25f, 0.5f, 0.0));      //������ ��ġ�� �̵�
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //������ ȸ��
        transformMatrix = transMatrixpoint * transformMatrix;      //������ �̵�
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

        //������ ��
        //���庯ȯ �� �� ��� ����
        transformMatrix = glm::mat4(1.0f);
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(-0.5f, -0.5f, -0.5f));
        transformMatrix = basicChange * transformMatrix;
        basicChange = glm::mat4(1.0f);      //�ʱ�ȭ
        transformMatrix = scalehandfoot * transformMatrix;      //ũ��
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.0f, -0.25f, 0.0));      //ȸ���� �̵�
        transformMatrix = basicChange * transformMatrix;
        rotateMatrixt = glm::mat4(1.0f);
        rotateMatrixt = glm::rotate(rotateMatrixt, glm::radians(setat), glm::vec3(1.0f, 0.0f, 0.0f));

        transformMatrix = rotateMatrixt * transformMatrix;
        basicChange = glm::mat4(1.0f);
        basicChange = glm::translate(basicChange, glm::vec3(0.25f, 0.5f, 0.0));      //������ ��ġ�� �̵�
        transformMatrix = basicChange * transformMatrix;
        transformMatrix = rotateMatrixm * transformMatrix;      //������ ȸ��
        transformMatrix = transMatrixpoint * transformMatrix;      //������ �̵�
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


