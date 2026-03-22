#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/random.hpp>
#include <shaderClass.h>
#include "stb_image.h"
#include <iostream>
#include <random>
const int starCount = 10000;
int width = 1920, height = 1080;

glm::vec3 cameraPos = glm::vec3(0.0f, 50.0f, 150.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float currentFrame;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = width / 2, lastY = height / 2;
float yaw = -90.0f,pitch = -15.0f;
float sensitivity = 0.15f;
float FOV = 45.0f;
bool mouseFirst = true;


struct body {
	glm::vec3 size;
	glm::vec3 rotation; 
	float rot_speed; //rotation speed
	glm::vec3 orbit;
	float orbit_speed; //orbit speed
	glm::vec3 position;
	glm::vec4 color;
};


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (mouseFirst) {
		lastX = xpos;
		lastY = ypos;
		mouseFirst = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY-ypos;
	lastX = xpos;
	lastY = ypos;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void keyboardInput(GLFWwindow* window) {
	const float cameraSpeed = 60.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraFront * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraFront * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		cameraPos += cameraUp * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		cameraPos -= cameraUp * cameraSpeed;
	}
}
body sun, mercury, venus, earth, mars, jupiter, saturn, uranus, neptune, pluton;
void initBodies() {
	
	sun.size = glm::vec3(25.0f, 25.0f, 25.0f);
	sun.position = glm::vec3(0.0f, 0.0f, 0.0f);
	sun.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	sun.rot_speed = 5.0f;
	sun.orbit = glm::vec3(0.0f, 1.0f, 0.0f);
	sun.orbit_speed = 0.0f;
	sun.color = glm::vec4(1.0f, 0.9f, 0.5f, 1.0f);

	
	mercury.size = glm::vec3(0.5f, 0.5f, 0.5f);
	mercury.position = glm::vec3(0.0f, 0.0f, -39.0f);
	mercury.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	mercury.rot_speed = 1.0f;
	mercury.orbit = glm::vec3(0.121f, 0.992f, 0.0f);
	mercury.orbit_speed = 21.0f;
	mercury.color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

	venus.size = glm::vec3(0.6f, 0.6f, 0.6f);
	venus.position = glm::vec3(0.0f, 0.0f, -72.0f);
	venus.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	venus.rot_speed = 5.0f;
	venus.orbit = glm::vec3(0.059f, 0.998f, 0.0f);
	venus.orbit_speed = 7.0f;
	venus.color = glm::vec4(0.9f, 0.8f, 0.6f, 1.0f);


	earth.size = glm::vec3(1.0f, 1.0f, 1.0f);
	earth.position = glm::vec3(0.0f, 0.0f, -100.0f);
	earth.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	earth.rot_speed = 20.0f;
	earth.orbit = glm::vec3(0.0f, 1.0f, 0.0f);
	earth.orbit_speed = 5.0f;
	earth.color = glm::vec4(0.2f, 0.4f, 1.0f, 1.0f);

	mars.size = glm::vec3(0.7f, 0.7f, 0.7f);
	mars.position = glm::vec3(0.0f, 0.0f, -152.0f);
	mars.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	mars.rot_speed = 25.0f;
	mars.orbit = glm::vec3(0.032f, 0.999f, 0.0f);
	mars.orbit_speed = 2.5f;
	mars.color = glm::vec4(0.8f, 0.4f, 0.3f, 1.0f);


	jupiter.size = glm::vec3(5.0f, 5.0f, 5.0f);
	jupiter.position = glm::vec3(0.0f, 0.0f, -240.0f);
	jupiter.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	jupiter.rot_speed = 15.0f;
	jupiter.orbit = glm::vec3(0.022f, 0.999f, 0.0f);
	jupiter.orbit_speed = 0.5f;
	jupiter.color = glm::vec4(0.8f, 0.7f, 0.5f, 1.0f);

	
	saturn.size = glm::vec3(4.0f, 4.0f, 4.0f);
	saturn.position = glm::vec3(0.0f, 0.0f, -425.0f);
	saturn.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	saturn.rot_speed = 10.0f;
	saturn.orbit = glm::vec3(0.043f, 0.999f, 0.0f);
	saturn.orbit_speed = 0.25f;
	saturn.color = glm::vec4(0.9f, 0.8f, 0.5f, 1.0f);


	uranus.size = glm::vec3(3.0f, 3.0f, 3.0f);
	uranus.position = glm::vec3(0.0f, 0.0f, -900.0f);
	uranus.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	uranus.rot_speed = 30.0f;
	uranus.orbit = glm::vec3(0.014f, 0.999f, 0.0f);
	uranus.orbit_speed = 0.05f;
	uranus.color = glm::vec4(0.6f, 0.9f, 0.9f, 1.0f);


	neptune.size = glm::vec3(3.0f, 3.0f, 3.0f);
	neptune.position = glm::vec3(0.0f, 0.0f, -1200.0f);
	neptune.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	neptune.rot_speed = 30.0f;
	neptune.orbit = glm::vec3(0.031f, 0.999f, 0.0f);
	neptune.orbit_speed = 0.025f;
	neptune.color = glm::vec4(0.3f, 0.5f, 1.0f, 1.0f);


	pluton.size = glm::vec3(0.2f, 0.2f, 0.2f);
	pluton.position = glm::vec3(0.0f, 0.0f, -1700.0f);
	pluton.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	pluton.rot_speed = 30.0f;
	pluton.orbit = glm::vec3(0.294f, 0.955f, 0.0f);
	pluton.orbit_speed = 0.005f;
	pluton.color = glm::vec4(0.7f, 0.6f, 0.5f, 1.0f);
	
	
}


int main() {
	glfwInit(); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//creating a window
	GLFWwindow* window = glfwCreateWindow(width, height, "mini Solar System", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create a window !" << std::endl;
		glfwTerminate();
		return -1;
	}
	//setting up context
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, width, height);

	//celestial bodies
	float vertices[] = {
		//front
		-0.5f,-0.5f,0.5f, //bottom left
		0.5f,-0.5f,0.5f, //bottom right
		0.5f,0.5f,0.5f,	 //top right		
		-0.5f,0.5f,0.5f, //top left	

		//back
		-0.5f,-0.5f,-0.5f, //bottom left
		0.5f,-0.5f,-0.5f, //bottom right
		0.5f,0.5f,-0.5f,	 //top right		
		-0.5f,0.5f,-0.5f, //top left	
	};

	unsigned int indicies[] = {
		//front
		0,1,2,
		2,3,0,

		//back
		4,5,6,
		6,7,4,

		//right
		1,5,6,
		6,2,1,

		//left
		0,4,7,
		7,3,0,

		//top
		3,2,6,
		6,7,3,

		//bottom
		0,1,5,
		5,4,0
	};
	unsigned int VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indicies),indicies,GL_STATIC_DRAW);

	Shader shader("vertexShader.txt", "fragmentShader.txt");
	Shader starsShader("starsVSh.txt", "starsFSh.txt");
	//stars
	float starVeritces[starCount * 3];
	for (int i = 0; i < starCount*3; i += 3) {
		glm::vec3 starPos = glm::sphericalRand(1000.0f);
		starVeritces[i] = starPos.x;
		starVeritces[i+1] = starPos.y;
		starVeritces[i+2] = starPos.z;
	}
	unsigned int starsVBO, starsVAO;
	glGenVertexArrays(1, &starsVAO);
	glBindVertexArray(starsVAO);
	
	glGenBuffers(1, &starsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, starsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(starVeritces), starVeritces, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	


	glm::mat4 projection = glm::mat4(1.0f); 
	projection = glm::perspective(glm::radians(FOV), (float)width / (float)height,0.1f,4000.0f);

	initBodies();
	body bodies[] = { sun,mercury,venus,earth,mars,jupiter,saturn,uranus,neptune,pluton };
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	//render loop 
	while (!glfwWindowShouldClose(window)) {
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//input
		keyboardInput(window);
		glClearColor(0.01f, 0.01f, 0.01f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//draw
		shader.use();
		glBindVertexArray(VAO);

		glm::mat4 view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
		for (int i = 0;i < 10;i++) {
			glm::mat4 model = glm::mat4(1.0f);

			//orbit
			model = glm::rotate(model, glm::radians((float)glfwGetTime()) * bodies[i].orbit_speed*10, bodies[i].orbit);
			
			//distance
			model = glm::translate(model, bodies[i].position);
			
			//self rotation
			model = glm::rotate(model, glm::radians((float)glfwGetTime()) * bodies[i].rot_speed, bodies[i].rotation);
			
			//size
			model = glm::scale(model, bodies[i].size);

			shader.setMatrix4fv("model", model);
			shader.setMatrix4fv("view", view);
			shader.setMatrix4fv("projection", projection);
			shader.setF4("color", bodies[i].color.x, bodies[i].color.y, bodies[i].color.z, bodies[i].color.w);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(starsVAO);
		starsShader.use();
		glm::mat4 starView = glm::mat4(glm::mat3(view));
		starsShader.setMatrix4fv("view", starView);
		starsShader.setMatrix4fv("projection", projection);
		
		glDrawArrays(GL_POINTS, 0, starCount);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
    glfwTerminate();
    return 0;
}

